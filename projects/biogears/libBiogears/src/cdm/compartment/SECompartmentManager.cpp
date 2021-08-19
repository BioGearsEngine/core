/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#include <biogears/cdm/substance/SESubstanceManager.h>

#include <biogears/cdm/compartment/SECompartmentGraph.inl>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/SECompartmentTransportGraph.inl>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.inl>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/schema/cdm/Compartment.hxx>

namespace std {
  template class vector<biogears::SEGasCompartment*> ;
  template class map<string, biogears::SEGasCompartment*> ;
  template class vector<biogears::SEGasCompartmentLink*> ;
  template class map<string, biogears::SEGasCompartmentLink*> ;
  template class vector<biogears::SEGasCompartmentGraph*> ;
  template class map<string, biogears::SEGasCompartmentGraph*> ;
  template class vector<biogears::SESubstance*> ;
  
  template class vector<biogears::SELiquidCompartment*> ;
  template class map<string, biogears::SELiquidCompartment*> ;
  template class vector<biogears::SELiquidCompartmentLink*> ;
  template class map<string, biogears::SELiquidCompartmentLink*> ;
  template class vector<biogears::SELiquidCompartmentGraph*> ;
  template class map<string, biogears::SELiquidCompartmentGraph*> ;
  
  template class vector<biogears::SEThermalCompartment*> ;
  template class map<string, biogears::SEThermalCompartment*> ;
  template class vector<biogears::SEThermalCompartmentLink*> ;
  template class map<string, biogears::SEThermalCompartmentLink*> ;
  
  template class vector<biogears::SETissueCompartment*> ;
  template class map<string, biogears::SETissueCompartment*> ;
}
namespace biogears {

SECompartmentManager::SECompartmentManager(SESubstanceManager& subMgr)
  : Loggable(subMgr.GetLogger())
  , m_subMgr(subMgr)
{
  m_O2 = subMgr.GetSubstance("Oxygen");
  m_CO2 = subMgr.GetSubstance("CarbonDioxide");
  m_CO = subMgr.GetSubstance("CarbonMonoxide");
  m_Hb = subMgr.GetSubstance("Hemoglobin");
  m_HbO2 = subMgr.GetSubstance("Oxyhemoglobin");
  m_HbCO2 = subMgr.GetSubstance("Carbaminohemoglobin");
  m_HbO2CO2 = subMgr.GetSubstance("OxyCarbaminohemoglobin");
  m_HbCO = subMgr.GetSubstance("Carboxyhemoglobin");
  Clear();
}
SECompartmentManager::~SECompartmentManager()
{
  Clear();
}

void SECompartmentManager::Clear()
{
  DELETE_VECTOR(m_GasCompartments);
  m_GasName2Compartments.clear();
  m_GasLeafCompartments.clear();
  DELETE_VECTOR(m_GasLinks);
  m_GasName2Links.clear();
  m_GasSubstances.clear();
  DELETE_VECTOR(m_GasGraphs);
  m_GasName2Graphs.clear();

  DELETE_VECTOR(m_LiquidCompartments);
  m_LiquidName2Compartments.clear();
  m_LiquidLeafCompartments.clear();
  DELETE_VECTOR(m_LiquidLinks);
  m_LiquidName2Links.clear();
  m_LiquidSubstances.clear();
  DELETE_VECTOR(m_LiquidGraphs);
  m_LiquidName2Graphs.clear();

  DELETE_VECTOR(m_ThermalCompartments);
  m_ThermalName2Compartments.clear();
  m_ThermalLeafCompartments.clear();
  DELETE_VECTOR(m_ThermalLinks);
  m_ThermalLinks.clear();
  m_ThermalName2Links.clear();

  DELETE_VECTOR(m_TissueCompartments);
  m_TissueLeafCompartments.clear();
  m_TissueName2Compartments.clear();
}

#define LOAD_COMPARTMENT(type)                                                    \
  for (auto& cData : in.type##Compartment()) {                                    \
    if (!Create##type##Compartment(cData.Name()).Load(cData, m_subMgr, circuits)) \
      return false;                                                               \
  }
#define LOAD_THERMAL_COMPARTMENT(type)                                  \
  for (auto& cData : in.type##Compartment()) {                          \
    if (!Create##type##Compartment(cData.Name()).Load(cData, circuits)) \
      return false;                                                     \
  }
#define LOAD_LINK(type)                                                                                                              \
  for (auto& cData : in.type##Link()) {                                                                                              \
    auto* src = Get##type##Compartment(cData.SourceCompartment());                                                                   \
    if (src == nullptr) {                                                                                                            \
      Error("Unable to find source compartment " + std::string { cData.SourceCompartment() } + " for link " + cData.Name().c_str()); \
      return false;                                                                                                                  \
    }                                                                                                                                \
    auto* tgt = Get##type##Compartment(cData.TargetCompartment());                                                                   \
    if (src == nullptr) {                                                                                                            \
      Error("Unable to find target compartment " + std::string { cData.TargetCompartment() } + " for link " + cData.Name().c_str()); \
      return false;                                                                                                                  \
    }                                                                                                                                \
    if (!Create##type##Link(*src, *tgt, cData.Name()).Load(cData, circuits))                                                         \
      return false;                                                                                                                  \
  }
#define LOAD_HIERARCHY(type)                                                                    \
  for (auto& cData : in.type##Compartment()) {                                                  \
    auto* cmpt = Get##type##Compartment(cData.Name());                                          \
    for (auto name : cData.Child()) {                                                           \
      auto* child = Get##type##Compartment(name);                                               \
      if (child == nullptr) {                                                                   \
        Error("Could not find child " + std::string { name } + " for node " + cmpt->GetName()); \
        return false;                                                                           \
      }                                                                                         \
      cmpt->AddChild(*child);                                                                   \
    }                                                                                           \
  }
#define LOAD_GRAPH(type)                                       \
  for (auto& cData : in.type##Graph()) {                       \
    if (!Create##type##Graph(cData.Name()).Load(cData, *this)) \
      return false;                                            \
  }
#define LOAD_SUBSTANCE(type)                                        \
  for (auto subName : in.type##Substance()) {                       \
    SESubstance* sub = m_subMgr.GetSubstance(subName);              \
    if (sub == nullptr) {                                           \
      Error("Could not find substance " + std::string { subName }); \
      return false;                                                 \
    }                                                               \
    Add##type##CompartmentSubstance(*sub);                          \
  }

bool SECompartmentManager::Load(const CDM::CompartmentManagerData& in, SECircuitManager* circuits)
{
  Clear();

  LOAD_COMPARTMENT(Gas);
  LOAD_LINK(Gas);
  LOAD_HIERARCHY(Gas);
  LOAD_SUBSTANCE(Gas);
  LOAD_GRAPH(Gas);

  LOAD_COMPARTMENT(Liquid);
  LOAD_LINK(Liquid);
  LOAD_HIERARCHY(Liquid);
  LOAD_SUBSTANCE(Liquid);
  LOAD_GRAPH(Liquid);

  LOAD_THERMAL_COMPARTMENT(Thermal);
  LOAD_LINK(Thermal);
  LOAD_HIERARCHY(Thermal);

  LOAD_COMPARTMENT(Tissue);

  StateChange();
  return true;
}
//-------------------------------------------------------------------------------
CDM::CompartmentManagerData* SECompartmentManager::Unload() const
{
  CDM::CompartmentManagerData* data = new CDM::CompartmentManagerData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SECompartmentManager::Unload(CDM::CompartmentManagerData& data) const
{
  for (SELiquidCompartment* cmpt : m_LiquidCompartments)
    data.LiquidCompartment().push_back(std::unique_ptr<CDM::LiquidCompartmentData>(cmpt->Unload()));
  for (SELiquidCompartmentLink* link : m_LiquidLinks)
    data.LiquidLink().push_back(std::unique_ptr<CDM::LiquidCompartmentLinkData>(link->Unload()));
  for (SELiquidCompartmentGraph* graph : m_LiquidGraphs)
    data.LiquidGraph().push_back(std::unique_ptr<CDM::LiquidCompartmentGraphData>(graph->Unload()));
  for (SESubstance* sub : m_LiquidSubstances)
    data.LiquidSubstance().push_back(sub->GetName());

  for (SEGasCompartment* cmpt : m_GasCompartments)
    data.GasCompartment().push_back(std::unique_ptr<CDM::GasCompartmentData>(cmpt->Unload()));
  for (SEGasCompartmentLink* link : m_GasLinks)
    data.GasLink().push_back(std::unique_ptr<CDM::GasCompartmentLinkData>(link->Unload()));
  for (SEGasCompartmentGraph* graph : m_GasGraphs)
    data.GasGraph().push_back(std::unique_ptr<CDM::GasCompartmentGraphData>(graph->Unload()));
  for (SESubstance* sub : m_GasSubstances)
    data.GasSubstance().push_back(sub->GetName());

  for (SEThermalCompartment* cmpt : m_ThermalCompartments)
    data.ThermalCompartment().push_back(std::unique_ptr<CDM::ThermalCompartmentData>(cmpt->Unload()));
  for (SEThermalCompartmentLink* link : m_ThermalLinks)
    data.ThermalLink().push_back(std::unique_ptr<CDM::ThermalCompartmentLinkData>(link->Unload()));

  for (SETissueCompartment* cmpt : m_TissueCompartments)
    data.TissueCompartment().push_back(std::unique_ptr<CDM::TissueCompartmentData>(cmpt->Unload()));
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasCompartment(CDM::enumCompartmentType::value type, const char* name) const
{
  return HasCompartment(type, std::string { name });
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasCompartment(CDM::enumCompartmentType::value type, const std::string& name) const
{
  switch (type) {
  case CDM::enumCompartmentType::Gas:
    return HasGasCompartment(name);
  case CDM::enumCompartmentType::Liquid:
    return HasLiquidCompartment(name);
  case CDM::enumCompartmentType::Thermal:
    return HasThermalCompartment(name);
  case CDM::enumCompartmentType::Tissue:
    return HasTissueCompartment(name);
  default:
    return false;
  }
  return false;
}
//-------------------------------------------------------------------------------
SECompartment* SECompartmentManager::GetCompartment(CDM::enumCompartmentType::value type, const char* name)
{
  return GetCompartment(type, std::string { name });
}
//-------------------------------------------------------------------------------
SECompartment* SECompartmentManager::GetCompartment(CDM::enumCompartmentType::value type, const std::string& name)
{
  switch (type) {
  case CDM::enumCompartmentType::Gas:
    return GetGasCompartment(name);
  case CDM::enumCompartmentType::Liquid:
    return GetLiquidCompartment(name);
  case CDM::enumCompartmentType::Thermal:
    return GetThermalCompartment(name);
  case CDM::enumCompartmentType::Tissue:
    return GetTissueCompartment(name);
  default:
    return nullptr;
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
const SECompartment* SECompartmentManager::GetCompartment(CDM::enumCompartmentType::value type, const char* name) const
{
  return GetCompartment(type, std::string { name });
}
//-------------------------------------------------------------------------------
const SECompartment* SECompartmentManager::GetCompartment(CDM::enumCompartmentType::value type, const std::string& name) const
{
  switch (type) {
  case CDM::enumCompartmentType::Gas:
    return GetGasCompartment(name);
  case CDM::enumCompartmentType::Liquid:
    return GetLiquidCompartment(name);
  case CDM::enumCompartmentType::Thermal:
    return GetThermalCompartment(name);
  case CDM::enumCompartmentType::Tissue:
    return GetTissueCompartment(name);
  default:
    return nullptr;
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
//////////////////////
// GAS COMPARTMENTS //
//////////////////////
SEGasCompartment& SECompartmentManager::CreateGasCompartment(const char* name)
{
  return CreateGasCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
SEGasCompartment& SECompartmentManager::CreateGasCompartment(const std::string& name)
{
  return CreateCompartment<SEGasCompartment>(name, m_GasName2Compartments, &m_GasSubstances);
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteGasCompartment(const char* name)
{
  return DeleteGasCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteGasCompartment(const std::string& name)
{
  SEGasCompartment* cmpt = GetGasCompartment(name);
  if (cmpt != nullptr) {
    m_GasName2Compartments.erase(name);
    Remove(m_GasCompartments, cmpt);
    Remove(m_GasLeafCompartments, cmpt);
    for (SEGasCompartmentGraph* g : m_GasGraphs)
      g->RemoveCompartment(*cmpt);
    SAFE_DELETE(cmpt);
  }
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasGasCompartment(const char* name) const
{
  return HasGasCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasGasCompartment(const std::string& name) const
{
  return HasCompartment<SEGasCompartment>(name, m_GasName2Compartments);
}
//-------------------------------------------------------------------------------
SEGasCompartment* SECompartmentManager::GetGasCompartment(const char* name)
{
  return GetGasCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
SEGasCompartment* SECompartmentManager::GetGasCompartment(const std::string& name)
{
  return GetCompartment<SEGasCompartment>(name, m_GasName2Compartments);
}
//-------------------------------------------------------------------------------
const SEGasCompartment* SECompartmentManager::GetGasCompartment(const char* name) const
{
  return GetGasCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
const SEGasCompartment* SECompartmentManager::GetGasCompartment(const std::string& name) const
{
  return GetCompartment<SEGasCompartment>(name, m_GasName2Compartments);
}
//-------------------------------------------------------------------------------
const std::vector<SEGasCompartment*>& SECompartmentManager::GetGasCompartments()
{
  return m_GasCompartments;
}
//-------------------------------------------------------------------------------
const std::vector<SEGasCompartment*>& SECompartmentManager::GetGasLeafCompartments()
{
  return m_GasLeafCompartments;
}
//-------------------------------------------------------------------------------
SEGasCompartmentLink& SECompartmentManager::CreateGasLink(SEGasCompartment& src, SEGasCompartment& tgt, const char* name)
{
  return CreateGasLink(src, tgt, std::string { name });
}
//-------------------------------------------------------------------------------
SEGasCompartmentLink& SECompartmentManager::CreateGasLink(SEGasCompartment& src, SEGasCompartment& tgt, const std::string& name)
{
  return CreateLink<SEGasCompartmentLink, SEGasCompartment>(src, tgt, name, m_GasName2Links);
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteGasLink(const char* name)
{
  return DeleteGasLink(std::string { name });
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteGasLink(const std::string& name)
{
  SEGasCompartmentLink* link = GetGasLink(name);
  if (link != nullptr) {
    m_GasName2Links.erase(name);
    Remove(m_GasLinks, link);
    for (SEGasCompartmentGraph* g : m_GasGraphs)
      g->RemoveLink(*link);
    SAFE_DELETE(link);
  }
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasGasLink(const char* name) const
{
  return HasGasLink(std::string { name });
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasGasLink(const std::string& name) const
{
  return HasLink<SEGasCompartmentLink>(name, m_GasName2Links);
}
//-------------------------------------------------------------------------------
SEGasCompartmentLink* SECompartmentManager::GetGasLink(const char* name)
{
  return GetGasLink(std::string { name });
}
//-------------------------------------------------------------------------------
SEGasCompartmentLink* SECompartmentManager::GetGasLink(const std::string& name)
{
  return GetLink<SEGasCompartmentLink>(name, m_GasName2Links);
}
//-------------------------------------------------------------------------------
const SEGasCompartmentLink* SECompartmentManager::GetGasLink(const char* name) const
{
  return GetGasLink(std::string { name });
}
//-------------------------------------------------------------------------------
const SEGasCompartmentLink* SECompartmentManager::GetGasLink(const std::string& name) const
{
  return GetLink<SEGasCompartmentLink>(name, m_GasName2Links);
}
//-------------------------------------------------------------------------------
const std::vector<SEGasCompartmentLink*>& SECompartmentManager::GetGasLinks()
{
  return m_GasLinks;
} //-------------------------------------------------------------------------------
SEGasCompartmentGraph& SECompartmentManager::CreateGasGraph(const char* name)
{
  return CreateGasGraph(std::string { name });
}
//-------------------------------------------------------------------------------
SEGasCompartmentGraph& SECompartmentManager::CreateGasGraph(const std::string& name)
{
  SEGasCompartmentGraph* graph = nullptr;

  auto find = m_GasName2Graphs.find(name);
  if (find == end(m_GasName2Graphs)) {
    graph = new SEGasCompartmentGraph(name, GetLogger());
    m_GasName2Graphs[name] = graph;
    m_GasGraphs.push_back(graph);
  } else {
    graph = find->second;
    if (graph->GetName() != name)
      throw CommonDataModelException("Compartment Graph already exists for name(" + name + ")");
  }
  return *graph;
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteGasGraph(const char* name)
{
  return DeleteGasGraph(std::string { name });
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteGasGraph(const std::string& name)
{
  SEGasCompartmentGraph* graph = GetGasGraph(name);
  if (graph != nullptr) {
    m_GasName2Graphs.erase(name);
    Remove(m_GasGraphs, graph);
    SAFE_DELETE(graph);
  }
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasGasGraph(const char* name) const
{
  return HasGasGraph(std::string { name });
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasGasGraph(const std::string& name) const
{
  return m_GasName2Graphs.find(name) != end(m_GasName2Graphs);
}
//-------------------------------------------------------------------------------
SEGasCompartmentGraph* SECompartmentManager::GetGasGraph(const char* name)
{
  return GetGasGraph(std::string { name });
}
//-------------------------------------------------------------------------------
SEGasCompartmentGraph* SECompartmentManager::GetGasGraph(const std::string& name)
{
  auto it = m_GasName2Graphs.find(name);
  if (it != m_GasName2Graphs.end())
    return it->second;
  return nullptr;
}
//-------------------------------------------------------------------------------
const SEGasCompartmentGraph* SECompartmentManager::GetGasGraph(const char* name) const
{
  return GetGasGraph(std::string { name });
}
//-------------------------------------------------------------------------------
const SEGasCompartmentGraph* SECompartmentManager::GetGasGraph(const std::string& name) const
{
  auto it = m_GasName2Graphs.find(name);
  if (it != m_GasName2Graphs.end())
    return it->second;
  return nullptr;
}
//-------------------------------------------------------------------------------
const std::vector<SEGasCompartmentGraph*>& SECompartmentManager::GetGasGraphs()
{
  return m_GasGraphs;
}
//-------------------------------------------------------------------------------
void SECompartmentManager::AddGasCompartmentSubstance(SESubstance& s)
{
  if (!Contains(m_GasSubstances, s)) {
    m_GasSubstances.push_back(&s);
    for (auto itr : m_GasName2Compartments) {
      if (AllowGasSubstance(s, *itr.second))
        AddSubstance<SEGasCompartment>(s, *itr.second);
    }
  }
}
//-------------------------------------------------------------------------------
const std::vector<SESubstance*>& SECompartmentManager::GetGasCompartmentSubstances() const
{
  return m_GasSubstances;
}
//-------------------------------------------------------------------------------
/////////////////////////
// LIQUID COMPARTMENTS //
/////////////////////////
SELiquidCompartment& SECompartmentManager::CreateLiquidCompartment(const char* name)
{
  return CreateLiquidCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
SELiquidCompartment& SECompartmentManager::CreateLiquidCompartment(const std::string& name)
{
  return CreateCompartment<SELiquidCompartment>(name, m_LiquidName2Compartments, &m_LiquidSubstances);
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteLiquidCompartment(const char* name)
{
  return DeleteLiquidCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteLiquidCompartment(const std::string& name)
{
  SELiquidCompartment* cmpt = GetLiquidCompartment(name);
  if (cmpt != nullptr) {
    m_LiquidName2Compartments.erase(name);
    Remove(m_LiquidCompartments, cmpt);
    Remove(m_LiquidLeafCompartments, cmpt);
    for (SELiquidCompartmentGraph* g : m_LiquidGraphs)
      g->RemoveCompartment(*cmpt);
    SAFE_DELETE(cmpt);
  }
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasLiquidCompartment(const char* name) const
{
  return HasLiquidCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasLiquidCompartment(const std::string& name) const
{
  return HasCompartment<SELiquidCompartment>(name, m_LiquidName2Compartments);
}
//-------------------------------------------------------------------------------
SELiquidCompartment* SECompartmentManager::GetLiquidCompartment(const char* name)
{
  return GetLiquidCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
SELiquidCompartment* SECompartmentManager::GetLiquidCompartment(const std::string& name)
{
  return GetCompartment<SELiquidCompartment>(name, m_LiquidName2Compartments);
}
//-------------------------------------------------------------------------------
const SELiquidCompartment* SECompartmentManager::GetLiquidCompartment(const char* name) const
{
  return GetLiquidCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
const SELiquidCompartment* SECompartmentManager::GetLiquidCompartment(const std::string& name) const
{
  return GetCompartment<SELiquidCompartment>(name, m_LiquidName2Compartments);
}
//-------------------------------------------------------------------------------
const std::vector<SELiquidCompartment*>& SECompartmentManager::GetLiquidCompartments()
{
  return m_LiquidCompartments;
}
//-------------------------------------------------------------------------------
const std::vector<SELiquidCompartment*>& SECompartmentManager::GetLiquidLeafCompartments()
{
  return m_LiquidLeafCompartments;
}
//-------------------------------------------------------------------------------
SELiquidCompartmentLink& SECompartmentManager::CreateLiquidLink(SELiquidCompartment& src, SELiquidCompartment& tgt, const char* name)
{
  return CreateLiquidLink(src, tgt, std::string { name });
}
//-------------------------------------------------------------------------------
SELiquidCompartmentLink& SECompartmentManager::CreateLiquidLink(SELiquidCompartment& src, SELiquidCompartment& tgt, const std::string& name)
{
  return CreateLink<SELiquidCompartmentLink, SELiquidCompartment>(src, tgt, name, m_LiquidName2Links);
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteLiquidLink(const char* name)
{
  return DeleteLiquidLink(std::string { name });
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteLiquidLink(const std::string& name)
{
  SELiquidCompartmentLink* link = GetLiquidLink(name);
  if (link != nullptr) {
    m_LiquidName2Links.erase(name);
    Remove(m_LiquidLinks, link);
    for (SELiquidCompartmentGraph* g : m_LiquidGraphs)
      g->RemoveLink(*link);
    SAFE_DELETE(link);
  }
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasLiquidLink(const char* name) const
{
  return HasLiquidLink(std::string { name });
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasLiquidLink(const std::string& name) const
{
  return HasLink<SELiquidCompartmentLink>(name, m_LiquidName2Links);
}
//-------------------------------------------------------------------------------
SELiquidCompartmentLink* SECompartmentManager::GetLiquidLink(const char* name)
{
  return GetLiquidLink(std::string { name });
}
//-------------------------------------------------------------------------------
SELiquidCompartmentLink* SECompartmentManager::GetLiquidLink(const std::string& name)
{
  return GetLink<SELiquidCompartmentLink>(name, m_LiquidName2Links);
}
//-------------------------------------------------------------------------------
const SELiquidCompartmentLink* SECompartmentManager::GetLiquidLink(const char* name) const
{
  return GetLiquidLink(std::string { name });
}
//-------------------------------------------------------------------------------
const SELiquidCompartmentLink* SECompartmentManager::GetLiquidLink(const std::string& name) const
{
  return GetLink<SELiquidCompartmentLink>(name, m_LiquidName2Links);
}
//-------------------------------------------------------------------------------
const std::vector<SELiquidCompartmentLink*>& SECompartmentManager::GetLiquidLinks()
{
  return m_LiquidLinks;
}
//-------------------------------------------------------------------------------
SELiquidCompartmentGraph& SECompartmentManager::CreateLiquidGraph(const char* name)
{
  return CreateLiquidGraph(std::string { name });
}
//-------------------------------------------------------------------------------
SELiquidCompartmentGraph& SECompartmentManager::CreateLiquidGraph(const std::string& name)
{
  SELiquidCompartmentGraph* graph = nullptr;

  auto find = m_LiquidName2Graphs.find(name);
  if (find == end(m_LiquidName2Graphs)) {
    graph = new SELiquidCompartmentGraph(name, GetLogger());
    m_LiquidName2Graphs[name] = graph;
    m_LiquidGraphs.push_back(graph);
  } else {
    graph = find->second;
    if (graph->GetName() != name)
      throw CommonDataModelException("Compartment Graph already exists for name(" + name + ")");
  }
  return *graph;
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteLiquidGraph(const char* name)
{
  return DeleteLiquidGraph(std::string { name });
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteLiquidGraph(const std::string& name)
{
  SELiquidCompartmentGraph* graph = GetLiquidGraph(name);
  if (graph != nullptr) {
    m_LiquidName2Graphs.erase(name);
    Remove(m_LiquidGraphs, graph);
    SAFE_DELETE(graph);
  }
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasLiquidGraph(const char* name) const
{
  return HasLiquidGraph(std::string { name });
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasLiquidGraph(const std::string& name) const
{
  return m_LiquidName2Graphs.find(name) != end(m_LiquidName2Graphs);
}
//-------------------------------------------------------------------------------
SELiquidCompartmentGraph* SECompartmentManager::GetLiquidGraph(const char* name)
{
  return GetLiquidGraph(std::string { name });
}
//-------------------------------------------------------------------------------
SELiquidCompartmentGraph* SECompartmentManager::GetLiquidGraph(const std::string& name)
{
  auto it = m_LiquidName2Graphs.find(name);
  if (it != m_LiquidName2Graphs.end())
    return it->second;
  return nullptr;
}
//-------------------------------------------------------------------------------
const SELiquidCompartmentGraph* SECompartmentManager::GetLiquidGraph(const char* name) const
{
  return GetLiquidGraph(std::string { name });
}
//-------------------------------------------------------------------------------
const SELiquidCompartmentGraph* SECompartmentManager::GetLiquidGraph(const std::string& name) const
{
  auto it = m_LiquidName2Graphs.find(name);
  if (it != m_LiquidName2Graphs.end())
    return it->second;
  return nullptr;
}
//-------------------------------------------------------------------------------
const std::vector<SELiquidCompartmentGraph*>& SECompartmentManager::GetLiquidGraphs()
{
  return m_LiquidGraphs;
}
//-------------------------------------------------------------------------------
void SECompartmentManager::AddLiquidCompartmentSubstance(SESubstance& s)
{
  if (!Contains(m_LiquidSubstances, s)) {
    m_LiquidSubstances.push_back(&s);
    for (auto itr : m_LiquidName2Compartments) {
      if (AllowLiquidSubstance(s, *itr.second))
        AddSubstance<SELiquidCompartment>(s, *itr.second);
    }
  }
}
//-------------------------------------------------------------------------------
const std::vector<SESubstance*>& SECompartmentManager::GetLiquidCompartmentSubstances() const
{
  return m_LiquidSubstances;
}
//-------------------------------------------------------------------------------

//////////////////////////
// THERMAL COMPARTMENTS //
//////////////////////////
SEThermalCompartment& SECompartmentManager::CreateThermalCompartment(const char* name)
{
  return CreateThermalCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
SEThermalCompartment& SECompartmentManager::CreateThermalCompartment(const std::string& name)
{
  return CreateCompartment<SEThermalCompartment>(name, m_ThermalName2Compartments, nullptr);
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteThermalCompartment(const char* name)
{
  return DeleteThermalCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteThermalCompartment(const std::string& name)
{
  SEThermalCompartment* cmpt = GetThermalCompartment(name);
  if (cmpt != nullptr) {
    m_ThermalName2Compartments.erase(name);
    Remove(m_ThermalCompartments, cmpt);
    Remove(m_ThermalLeafCompartments, cmpt);
    SAFE_DELETE(cmpt);
  }
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasThermalCompartment(const char* name) const
{
  return HasThermalCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasThermalCompartment(const std::string& name) const
{
  return HasCompartment<SEThermalCompartment>(name, m_ThermalName2Compartments);
}
//-------------------------------------------------------------------------------
SEThermalCompartment* SECompartmentManager::GetThermalCompartment(const char* name)
{
  return GetThermalCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
SEThermalCompartment* SECompartmentManager::GetThermalCompartment(const std::string& name)
{
  return GetCompartment<SEThermalCompartment>(name, m_ThermalName2Compartments);
}
//-------------------------------------------------------------------------------
const SEThermalCompartment* SECompartmentManager::GetThermalCompartment(const char* name) const
{
  return GetThermalCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
const SEThermalCompartment* SECompartmentManager::GetThermalCompartment(const std::string& name) const
{
  return GetCompartment<SEThermalCompartment>(name, m_ThermalName2Compartments);
}
//-------------------------------------------------------------------------------
const std::vector<SEThermalCompartment*>& SECompartmentManager::GetThermalCompartments()
{
  return m_ThermalCompartments;
}
//-------------------------------------------------------------------------------
const std::vector<SEThermalCompartment*>& SECompartmentManager::GetThermalLeafCompartments()
{
  return m_ThermalLeafCompartments;
}
//-------------------------------------------------------------------------------
SEThermalCompartmentLink& SECompartmentManager::CreateThermalLink(SEThermalCompartment& src, SEThermalCompartment& tgt, const char* name)
{
  return CreateThermalLink(src, tgt, std::string { name });
}
//-------------------------------------------------------------------------------
SEThermalCompartmentLink& SECompartmentManager::CreateThermalLink(SEThermalCompartment& src, SEThermalCompartment& tgt, const std::string& name)
{
  return CreateLink<SEThermalCompartmentLink, SEThermalCompartment>(src, tgt, name, m_ThermalName2Links);
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteThermalLink(const char* name)
{
  return DeleteThermalLink(std::string { name });
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteThermalLink(const std::string& name)
{
  SEThermalCompartmentLink* link = GetThermalLink(name);
  if (link != nullptr) {
    m_ThermalName2Links.erase(name);
    Remove(m_ThermalLinks, link);
    SAFE_DELETE(link);
  }
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasThermalLink(const char* name) const
{
  return HasThermalLink(std::string { name });
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasThermalLink(const std::string& name) const
{
  return HasLink<SEThermalCompartmentLink>(name, m_ThermalName2Links);
}
//-------------------------------------------------------------------------------
SEThermalCompartmentLink* SECompartmentManager::GetThermalLink(const char* name)
{
  return GetThermalLink(std::string { name });
}
//-------------------------------------------------------------------------------
SEThermalCompartmentLink* SECompartmentManager::GetThermalLink(const std::string& name)
{
  return GetLink<SEThermalCompartmentLink>(name, m_ThermalName2Links);
}
//-------------------------------------------------------------------------------
const SEThermalCompartmentLink* SECompartmentManager::GetThermalLink(const char* name) const
{
  return GetThermalLink(std::string { name });
}
//-------------------------------------------------------------------------------
const SEThermalCompartmentLink* SECompartmentManager::GetThermalLink(const std::string& name) const
{
  return GetLink<SEThermalCompartmentLink>(name, m_ThermalName2Links);
}
//-------------------------------------------------------------------------------
const std::vector<SEThermalCompartmentLink*>& SECompartmentManager::GetThermalLinks()
{
  return m_ThermalLinks;
}
//-------------------------------------------------------------------------------

/////////////////////////
// TISSUE COMPARTMENTS //
/////////////////////////
SETissueCompartment& SECompartmentManager::CreateTissueCompartment(const char* name)
{
  return CreateTissueCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
SETissueCompartment& SECompartmentManager::CreateTissueCompartment(const std::string& name)
{
  if (HasTissueCompartment(name))
    throw CommonDataModelException("Compartment already exists for name(" + name + ")");

  SETissueCompartment* tissue = new SETissueCompartment(name, GetLogger());
  m_TissueName2Compartments[name] = tissue;
  return *tissue;
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteTissueCompartment(const char* name)
{
  return DeleteTissueCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
void SECompartmentManager::DeleteTissueCompartment(const std::string& name)
{
  SETissueCompartment* cmpt = GetTissueCompartment(name);
  if (cmpt != nullptr) {
    m_TissueName2Compartments.erase(name);
    Remove(m_TissueCompartments, cmpt);
    Remove(m_TissueLeafCompartments, cmpt);
    SAFE_DELETE(cmpt);
  }
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasTissueCompartment(const char* name) const
{
  return HasTissueCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
bool SECompartmentManager::HasTissueCompartment(const std::string& name) const
{
  return HasCompartment<SETissueCompartment>(name, m_TissueName2Compartments);
}
//-------------------------------------------------------------------------------
SETissueCompartment* SECompartmentManager::GetTissueCompartment(const char* name)
{
  return GetTissueCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
SETissueCompartment* SECompartmentManager::GetTissueCompartment(const std::string& name)
{
  return GetCompartment<SETissueCompartment>(name, m_TissueName2Compartments);
}
//-------------------------------------------------------------------------------
const SETissueCompartment* SECompartmentManager::GetTissueCompartment(const char* name) const
{
  return GetTissueCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
const SETissueCompartment* SECompartmentManager::GetTissueCompartment(const std::string& name) const
{
  return GetCompartment<SETissueCompartment>(name, m_TissueName2Compartments);
}
//-------------------------------------------------------------------------------
const std::vector<SETissueCompartment*>& SECompartmentManager::GetTissueCompartments()
{
  return m_TissueCompartments;
}
//-------------------------------------------------------------------------------
const std::vector<SETissueCompartment*>& SECompartmentManager::GetTissueLeafCompartments()
{
  return m_TissueLeafCompartments;
}
//-------------------------------------------------------------------------------
///////////////////////
// General Utilities //
///////////////////////

void SECompartmentManager::StateChange()
{
  // All of our Name 2 Compartment Maps are up to date
  // So track the compartments into our various vectors
  // for easier access and functional support

  // Also, get a list of all of our leaves
  // The hierarchy should be build and good to go

  m_GasCompartments.clear();
  m_GasLeafCompartments.clear();
  for (auto itr : m_GasName2Compartments) {
    m_GasCompartments.push_back(itr.second);
    if (!itr.second->HasChildren()) {
      m_GasLeafCompartments.push_back(itr.second);
    }
    itr.second->StateChange();
    SetSubstances<SEGasCompartment>(*itr.second, m_GasSubstances);
  }
  m_GasLinks.clear();
  for (auto itr : m_GasName2Links) {
    m_GasLinks.push_back(itr.second);
  }
  m_GasGraphs.clear();
  for (auto itr : m_GasName2Graphs) {
    m_GasGraphs.push_back(itr.second);
    itr.second->StateChange();
  }

  m_LiquidCompartments.clear();
  m_LiquidLeafCompartments.clear();
  for (auto itr : m_LiquidName2Compartments) {
    m_LiquidCompartments.push_back(itr.second);
    if (!itr.second->HasChildren()) {
      m_LiquidLeafCompartments.push_back(itr.second);
    }
    itr.second->StateChange();
    SetSubstances<SELiquidCompartment>(*itr.second, m_LiquidSubstances);
  }
  m_LiquidLinks.clear();
  for (auto itr : m_LiquidName2Links) {
    m_LiquidLinks.push_back(itr.second);
  }
  m_LiquidGraphs.clear();
  for (auto itr : m_LiquidName2Graphs) {
    m_LiquidGraphs.push_back(itr.second);
    itr.second->StateChange();
  }

  m_ThermalCompartments.clear();
  m_ThermalLeafCompartments.clear();
  for (auto itr : m_ThermalName2Compartments) {
    m_ThermalCompartments.push_back(itr.second);
    if (!itr.second->HasChildren()) {
      m_ThermalLeafCompartments.push_back(itr.second);
    }
    itr.second->StateChange();
  }
  m_ThermalLinks.clear();
  for (auto itr : m_ThermalName2Links) {
    m_ThermalLinks.push_back(itr.second);
  }

  m_TissueCompartments.clear();
  m_TissueLeafCompartments.clear();
  for (auto itr : m_TissueName2Compartments) {
    m_TissueCompartments.push_back(itr.second);
    if (!itr.second->HasChildren()) {
      m_TissueLeafCompartments.push_back(itr.second);
    }
    itr.second->StateChange();
  }

  UpdateLinks();
}
//-------------------------------------------------------------------------------
// These methods UpdateLink methods are used to set what links you want a comparmtent to use to calculate its InFlow and OutFlow
// By default, it will add all links to all compartments that are either a source or target to the link.
// If you set up multiple graphs for various configurations, you really only want the inflow/outflow for compartments associated with the active graph
// -- Such as a respiratory graph where the mouth is connected to the environment, and a respiratory equipment graph, were the mouth connects to the equipment)
// -- If the equipment is not on/connected, you want the mouth inflow to use the flow associated with the mouthToEnvironment link, and NOT the mouthToEquipment link
// -- and vice versa if the equipment is on/connected
// So you will need to call the UpdateLinks with the 'active' graph, so the manager can set all the links to comparments that it should only take into account when computing flows
// So If you know how you have your compartments and graphs set up, and you want to update just a subset of your compartments associated with a graph
// you can use the UpdateLinks method that takes in a graph, BUT if you have some compartments in multiple active graphs (not sure why, but you know what your doing right?)
// You cannot use the UpdateLinks(Graph) method because you will not be able to get all links to the compartments that overlap multiple 'active' graphs
// If you run into that situation, you will need to combine all compartments and links from all your active graphs into vectors and use the UpdateLinks(vector<Compartment>, vector<Link>) method to get your comparmtnets right
// Since I don't know a use case where you would have a comparment associated with multiple active graphs, I did not write a method to combine graphs, but should be easy for you to do

void SECompartmentManager::UpdateLinks()
{
  UpdateLinks<SELiquidCompartment, SELiquidCompartmentLink>(m_LiquidCompartments, m_LiquidLinks);
  UpdateLinks<SEGasCompartment, SEGasCompartmentLink>(m_GasCompartments, m_GasLinks);
  UpdateLinks<SEThermalCompartment, SEThermalCompartmentLink>(m_ThermalCompartments, m_ThermalLinks);
}
//-------------------------------------------------------------------------------
void SECompartmentManager::UpdateLinks(SEGasCompartmentGraph& graph)
{
  UpdateLinks<SEGasCompartment, SEGasCompartmentLink>(graph.GetCompartments(), graph.GetLinks());
}
//-------------------------------------------------------------------------------
void SECompartmentManager::UpdateLinks(SELiquidCompartmentGraph& graph)
{
  UpdateLinks<SELiquidCompartment, SELiquidCompartmentLink>(graph.GetCompartments(), graph.GetLinks());
}
//-------------------------------------------------------------------------------
template <typename CompartmentType, typename LinkType>
void SECompartmentManager::UpdateLinks(const std::vector<CompartmentType*>& compartments, const std::vector<LinkType*>& links) const
{
  for (CompartmentType* cmpt : compartments) {
    cmpt->m_Links.clear();
    cmpt->m_IncomingLinks.clear();
    cmpt->m_OutgoingLinks.clear();
  }
  for (LinkType* link : links) {
    CompartmentType& src = link->GetSourceCompartment();
    CompartmentType& tgt = link->GetTargetCompartment();

    src.m_Links.push_back(link);
    src.m_OutgoingLinks.push_back(link);
    tgt.m_Links.push_back(link);
    tgt.m_IncomingLinks.push_back(link);
  }
  for (CompartmentType* pnt : compartments) {
    if (pnt->HasChildren()) {
      for (CompartmentType* child : pnt->GetChildren())
        GetChildLinks<CompartmentType, LinkType>(pnt, child);
    }
  }
}
//-------------------------------------------------------------------------------
template <typename CompartmentType, typename LinkType>
void SECompartmentManager::GetChildLinks(CompartmentType* pnt, CompartmentType* child) const
{
  if (child->HasChildren()) {
    for (CompartmentType* grandchild : child->GetChildren())
      GetChildLinks<CompartmentType, LinkType>(child, grandchild);
  }
  for (LinkType* in : child->m_IncomingLinks) {
    if (!Contains(pnt->m_Links, (*in))) {
      pnt->m_Links.push_back(in);
      pnt->m_IncomingLinks.push_back(in);
    }
  }
  for (LinkType* out : child->m_OutgoingLinks) {
    if (!Contains(pnt->m_Links, (*out))) {
      pnt->m_Links.push_back(out);
      pnt->m_OutgoingLinks.push_back(out);
    }
  }
}
//-------------------------------------------------------------------------------
template <typename CompartmentType>
void SECompartmentManager::FindLeaves(CompartmentType* cmpt, std::vector<CompartmentType*>& leaves) const
{
  if (cmpt->HasChildren()) {
    for (CompartmentType* child : cmpt->GetChildren()) {
      if (child->HasChildren())
        FindLeaves(child, leaves);
      else
        leaves.push_back(child);
    }
  }
}
//-------------------------------------------------------------------------------
template <typename CompartmentType>
CompartmentType& SECompartmentManager::CreateCompartment(const std::string& name, std::map<std::string, CompartmentType*>& name2cmpt, std::vector<SESubstance*>* substances)
{
  CompartmentType* cmpt = nullptr;

  auto find = name2cmpt.find(name);
  if (find == end(name2cmpt)) {
    cmpt = new CompartmentType(name, GetLogger());
    name2cmpt[name] = cmpt;
    if (substances != nullptr)
      SetSubstances(*cmpt, *substances);
  } else {
    cmpt = find->second;
    if (cmpt->GetName() != name)
      throw CommonDataModelException("Compartment already exists for name(" + name + ")");
  }
  return *cmpt;
}
//-------------------------------------------------------------------------------
template <typename CompartmentType>
bool SECompartmentManager::HasCompartment(const std::string& name, const std::map<std::string, CompartmentType*>& name2cmpt) const
{
  return name2cmpt.find(name) != name2cmpt.end();
}
//-------------------------------------------------------------------------------
template <typename CompartmentType>
CompartmentType* SECompartmentManager::GetCompartment(const std::string& name, std::map<std::string, CompartmentType*>& name2cmpt) const
{
  auto it = name2cmpt.find(name);
  if (it != name2cmpt.end())
    return it->second;
  return nullptr;
}
//-------------------------------------------------------------------------------
template <typename CompartmentType>
const CompartmentType* SECompartmentManager::GetCompartment(const std::string& name, const std::map<std::string, CompartmentType*>& name2cmpt) const
{
  auto it = name2cmpt.find(name);
  if (it != name2cmpt.end())
    return it->second;
  return nullptr;
}
//-------------------------------------------------------------------------------
template <typename LinkType, typename CompartmentType>
LinkType& SECompartmentManager::CreateLink(CompartmentType& src, CompartmentType& tgt, const std::string& name, std::map<std::string, LinkType*>& name2link) const
{
  LinkType* link = nullptr;

  auto find = name2link.find(name);
  if (find == end(name2link)) {
    link = new LinkType(src, tgt, name);
    name2link[name] = link;
    src.AddLink(*link);
    tgt.AddLink(*link);
  } else {
    link = find->second;
    if (link->GetName() != name)
      throw CommonDataModelException("Link already exists for name(" + name + ")");
  }
  return *link;
}
//-------------------------------------------------------------------------------
template <typename LinkType>
bool SECompartmentManager::HasLink(const std::string& name, const std::map<std::string, LinkType*>& name2link) const
{
  return name2link.find(name) != end(name2link);
}
//-------------------------------------------------------------------------------
template <typename LinkType>
LinkType* SECompartmentManager::GetLink(const std::string& name, std::map<std::string, LinkType*>& name2link) const
{
  auto it = name2link.find(name);
  if (it != name2link.end())
    return it->second;
  return nullptr;
}
//-------------------------------------------------------------------------------
template <typename LinkType>
const LinkType* SECompartmentManager::GetLink(const std::string& name, const std::map<std::string, LinkType*>& name2link) const
{
  auto it = name2link.find(name);
  if (it != name2link.end())
    return it->second;
  return nullptr;
}
//-------------------------------------------------------------------------------
template <typename CompartmentType>
void SECompartmentManager::SetSubstances(CompartmentType& cmpt, std::vector<SESubstance*>& substances) const
{
  // Add any substance quantites to this new compartment, if its a quantity holding type
  SEGasCompartment* gcmpt = dynamic_cast<SEGasCompartment*>(&cmpt);
  SELiquidCompartment* lcmpt = dynamic_cast<SELiquidCompartment*>(&cmpt);
  if (gcmpt != nullptr || lcmpt != nullptr) {
    for (SESubstance* s : substances) {
      if (gcmpt != nullptr) {
        if (!AllowGasSubstance(*s, *gcmpt))
          continue;
        if (!gcmpt->HasSubstanceQuantity(*s))
          gcmpt->CreateSubstanceQuantity(*s);
      } else if (lcmpt != nullptr) {
        if (!AllowLiquidSubstance(*s, *lcmpt))
          continue;
        SELiquidSubstanceQuantity& subQ = lcmpt->CreateSubstanceQuantity(*s);
        if (s == m_O2 || s == m_CO2 || s == m_CO)
          subQ.SetHemoglobins(*m_Hb, *m_HbO2, *m_HbCO2, *m_HbO2CO2, *m_HbCO);
      }
    }
  }
}
//-------------------------------------------------------------------------------
template <typename CompartmentType>
void SECompartmentManager::AddSubstance(SESubstance& s, CompartmentType& cmpt) const
{
  if (cmpt.HasSubstanceQuantity(s))
    return;
  auto& subQ = cmpt.CreateSubstanceQuantity(s);
  SELiquidSubstanceQuantity* lsubQ = dynamic_cast<SELiquidSubstanceQuantity*>(&subQ);
  if (lsubQ != nullptr && (&s == m_O2 || &s == m_CO2 || &s == m_CO))
    lsubQ->SetHemoglobins(*m_Hb, *m_HbO2, *m_HbCO2, *m_HbO2CO2, *m_HbCO);
}
//-------------------------------------------------------------------------------
}