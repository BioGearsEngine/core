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
#include <biogears/engine/Controller/BioGearsCompartments.h>

//Standard Includes
#include <string>
#include <vector>
//Project Includes
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
auto BioGearsCompartments::make_unique(BioGears& bg) -> std::unique_ptr<BioGearsCompartments>
{
  return std::unique_ptr<BioGearsCompartments>(new BioGearsCompartments(bg));
}

BioGearsCompartments::BioGearsCompartments(BioGears& bg)
  : SECompartmentManager(bg.GetSubstances())
  , m_data(bg)
{
  Clear();
}

BioGearsCompartments::~BioGearsCompartments()
{
  Clear();
}

void BioGearsCompartments::Clear()
{
  SECompartmentManager::Clear();
  m_CombinedCardiovascularGraph = nullptr;
  m_CardiovascularGraph = nullptr;
  m_CerebralGraph = nullptr;
  m_RenalGraph = nullptr;
  m_RespiratoryGraph = nullptr;
  m_AnesthesiaMachineGraph = nullptr;
  m_CombinedRespiratoryAnesthesiaGraph = nullptr;
  m_CombinedRespiratoryInhalerGraph = nullptr;
  m_CombinedRespiratoryMechanicalVentilatorGraph = nullptr;
  m_AerosolGraph = nullptr;
  m_CombinedAerosolInhalerGraph = nullptr;

  m_TemperatureCompartments.clear();
  m_TemperatureLeafCompartments.clear();
  m_ChymeCompartments.clear();
  m_ChymeLeafCompartments.clear();
  m_PulmonaryCompartments.clear();
  m_PulmonaryLeafCompartments.clear();
  m_TissueCompartments.clear();
  m_TissueLeafCompartments.clear();
  m_UrineCompartments.clear();
  m_UrineLeafCompartments.clear();
  m_VascularCompartments.clear();
  m_VascularLeafCompartments.clear();
  m_AnesthesiaMachineCompartments.clear();
  m_AnesthesiaMachineLeafCompartments.clear();
  m_AerosolCompartments.clear();
  m_AerosolLeafCompartments.clear();
  m_InhalerCompartments.clear();
  m_InhalerLeafCompartments.clear();
  m_InhalerAerosolCompartments.clear();
  m_InhalerAerosolLeafCompartments.clear();
  m_MechanicalVentilatorCompartments.clear();
  m_MechanicalVentilatorLeafCompartments.clear();

  m_ExtracellularFluid.clear();
  m_IntracellularFluid.clear();
}

bool BioGearsCompartments::Load(const CDM::CompartmentManagerData& in, SECircuitManager* circuits)
{
  if (!SECompartmentManager::Load(in, circuits))
    return false;

  m_CombinedCardiovascularGraph = GetLiquidGraph(BGE::Graph::ActiveCardiovascular);
  if (m_CombinedCardiovascularGraph == nullptr) {
    Error("Could not find required Graph " + std::string(BGE::Graph::ActiveCardiovascular));
    return false;
  }
  m_CardiovascularGraph = GetLiquidGraph(BGE::Graph::Cardiovascular);
  if (m_CardiovascularGraph == nullptr) {
    Error("Could not find required Graph " + std::string(BGE::Graph::Cardiovascular));
    return false;
  }
  m_CerebralGraph = GetLiquidGraph(BGE::Graph::Cerebral);
  if (m_CerebralGraph == nullptr) {
    Error("Could not find required Graph " + std::string(BGE::Graph::Cerebral));
    return false;
  }
  m_RenalGraph = GetLiquidGraph(BGE::Graph::Renal);
  if (m_RenalGraph == nullptr) {
    Error("Could not find required Graph " + std::string(BGE::Graph::Renal));
    return false;
  }
  m_RespiratoryGraph = GetGasGraph(BGE::Graph::Respiratory);
  if (m_RespiratoryGraph == nullptr) {
    Error("Could not find required Graph " + std::string(BGE::Graph::Respiratory));
    return false;
  }
  m_AnesthesiaMachineGraph = GetGasGraph(BGE::Graph::AnesthesiaMachine);
  if (m_AnesthesiaMachineGraph == nullptr) {
    Error("Could not find required Graph " + std::string(BGE::Graph::AnesthesiaMachine));
    return false;
  }
  m_CombinedRespiratoryAnesthesiaGraph = GetGasGraph(BGE::Graph::RespiratoryAndAnesthesiaMachine);
  if (m_CombinedRespiratoryAnesthesiaGraph == nullptr) {
    Error("Could not find required Graph " + std::string(BGE::Graph::RespiratoryAndAnesthesiaMachine));
    return false;
  }
  m_CombinedRespiratoryInhalerGraph = GetGasGraph(BGE::Graph::RespiratoryAndInhaler);
  if (m_CombinedRespiratoryInhalerGraph == nullptr) {
    Error("Could not find required Graph " + std::string(BGE::Graph::RespiratoryAndInhaler));
    return false;
  }
  m_AerosolGraph = GetLiquidGraph(BGE::Graph::Aerosol);
  if (m_AerosolGraph == nullptr) {
    Error("Could not find required Graph " + std::string(BGE::Graph::Aerosol));
    return false;
  }
  m_CombinedAerosolInhalerGraph = GetLiquidGraph(BGE::Graph::AerosolAndInhaler);
  if (m_CombinedAerosolInhalerGraph == nullptr) {
    Error("Could not find required Graph " + std::string(BGE::Graph::AerosolAndInhaler));
    return false;
  }
  m_CombinedRespiratoryMechanicalVentilatorGraph = GetGasGraph(BGE::Graph::RespiratoryAndMechanicalVentilator);
  if (m_CombinedRespiratoryMechanicalVentilatorGraph == nullptr) {
    Error("Could not find required Graph " + std::string(BGE::Graph::RespiratoryAndMechanicalVentilator));
    return false;
  }

  return true;
}

#define SORT_CMPTS(bin, type)                                                                                        \
  m_##bin##Compartments.clear();                                                                                     \
  m_##bin##LeafCompartments.clear();                                                                                 \
  for (const std::string& name : BGE::bin##Compartment::GetValues()) {                                               \
    SE##type##Compartment* cmpt = Get##type##Compartment(name);                                                      \
    if (cmpt == nullptr) {                                                                                           \
      Warning("Could not find expected " + std::string(#bin) + " compartment, " + name + " in compartment manager"); \
      continue;                                                                                                      \
    }                                                                                                                \
    m_##bin##Compartments.push_back(cmpt);                                                                           \
    if (!cmpt->HasChildren())                                                                                        \
      m_##bin##LeafCompartments.push_back(cmpt);                                                                     \
  }
void BioGearsCompartments::StateChange()
{
  SECompartmentManager::StateChange();
  // Hook up our compartment categories
  // Anatomy
  SORT_CMPTS(Chyme, Liquid);
  SORT_CMPTS(Pulmonary, Gas);
  SORT_CMPTS(Temperature, Thermal);
  if (m_data.GetConfiguration().IsTissueEnabled()) {
    SORT_CMPTS(Tissue, Tissue);
    for (const std::string& name : BGE::ExtravascularCompartment::GetValues()) {
      if (GetLiquidCompartment(name) == nullptr)
        Warning("Could not find expected Extravascular compartment, " + name + " in compartment manager");
    }

    SELiquidCompartment* cmpt;
    m_ExtracellularFluid.clear();
    m_IntracellularFluid.clear();
    for (SETissueCompartment* t : m_TissueLeafCompartments) {
      cmpt = GetLiquidCompartment(std::string{ t->GetName() } + "Extracellular");
      if (cmpt == nullptr)
        Fatal(std::string{ "Could not find the tissue " } + t->GetName() + " Extracellular compartment");
      m_ExtracellularFluid[t] = cmpt;
      cmpt = GetLiquidCompartment(std::string{ t->GetName() } + "Intracellular");
      if (cmpt == nullptr)
        Fatal(std::string{ "Could not find the tissue " } + t->GetName() + " Intracellular compartment");
      m_IntracellularFluid[t] = cmpt;
    }
  }
  if (m_data.GetConfiguration().IsRenalEnabled()) {
    SORT_CMPTS(Urine, Liquid);
  } 
  SORT_CMPTS(Vascular, Liquid);
  // Equipment
  SORT_CMPTS(AnesthesiaMachine, Gas);
  SORT_CMPTS(Inhaler, Gas);

  // Aerosol liquid cmpts share the same names as the pulmonary gas compartments,
  // so the macro does not work with us
  m_InhalerAerosolCompartments.clear();
  m_InhalerAerosolLeafCompartments.clear();
  for (const std::string& name : BGE::InhalerCompartment::GetValues()) {
    SELiquidCompartment* cmpt = GetLiquidCompartment(name);
    if (cmpt == nullptr) {
      Warning("Could not find expected Aerosol compartment, " + name + " in compartment manager");
      continue;
    }
    m_InhalerAerosolCompartments.push_back(cmpt);
    if (!cmpt->HasChildren())
      m_InhalerAerosolLeafCompartments.push_back(cmpt);
  }

  m_AerosolCompartments.clear();
  m_AerosolLeafCompartments.clear();
  for (const std::string& name : BGE::PulmonaryCompartment::GetValues()) {
    SELiquidCompartment* cmpt = GetLiquidCompartment(name);
    if (cmpt == nullptr) {
      Warning("Could not find expected Aerosol compartment, " + name + " in compartment manager");
      continue;
    }
    m_AerosolCompartments.push_back(cmpt);
    if (!cmpt->HasChildren())
      m_AerosolLeafCompartments.push_back(cmpt);
  }

  SORT_CMPTS(MechanicalVentilator, Gas);

  // \todo Write some code to cross check compartments between what we have and what we should have
  // Here is some code to make sure all created compartments are in an enum
  // (We could have removed it from the enum, but not deleted the code that makes it)
  // It would also be nice to have code that makes sure there are no compartments that are made, but NOT in the enums
  // Note, Tissue Enum list does not contain the Tissue Extracellular or Intracellular names, NO list does
}

void BioGearsCompartments::AddGasCompartmentSubstance(SESubstance& sub)
{
  SECompartmentManager::AddGasCompartmentSubstance(sub);
}
void BioGearsCompartments::AddLiquidCompartmentSubstance(SESubstance& sub)
{
  SECompartmentManager::AddLiquidCompartmentSubstance(sub);
}

bool BioGearsCompartments::AllowGasSubstance(SESubstance& s, SEGasCompartment& cmpt) const
{
  return true;
}
bool BioGearsCompartments::AllowLiquidSubstance(SESubstance& s, SELiquidCompartment& cmpt) const
{
  if (s.HasAerosolization()) // It's ok to add it everywhere
    return true;
  else {
    // Don't add it to the environment aerosol
    const std::vector<std::string>& e = BGE::EnvironmentCompartment::GetValues();
    if (std::find(e.begin(), e.end(), cmpt.GetName()) != e.end())
      return false;
    // Don't add it to the aerosol compartments (Liquid version of Respiratory cmpts)
    const std::vector<std::string>& p = BGE::PulmonaryCompartment::GetValues();
    if (std::find(p.begin(), p.end(), cmpt.GetName()) != p.end())
      return false;
    // Don't add it to aerosol cmpts either
    const std::vector<std::string>& i = BGE::InhalerCompartment::GetValues();
    if (std::find(i.begin(), i.end(), cmpt.GetName()) != i.end())
      return false;
  }
  return true;
}

SELiquidCompartmentGraph& BioGearsCompartments::GetActiveCardiovascularGraph()
{
  if (m_CombinedCardiovascularGraph == nullptr)
    m_CombinedCardiovascularGraph = &CreateLiquidGraph(BGE::Graph::ActiveCardiovascular);
  return *m_CombinedCardiovascularGraph;
}
SELiquidCompartmentGraph& BioGearsCompartments::GetCardiovascularGraph()
{
  if (m_CardiovascularGraph == nullptr)
    m_CardiovascularGraph = &CreateLiquidGraph(BGE::Graph::Cardiovascular);
  return *m_CardiovascularGraph;
}
SELiquidCompartmentGraph& BioGearsCompartments::GetCerebralGraph()
{
  if (m_CerebralGraph == nullptr)
    m_CerebralGraph = &CreateLiquidGraph(BGE::Graph::Cerebral);
  return *m_CerebralGraph;
}
SELiquidCompartmentGraph& BioGearsCompartments::GetRenalGraph()
{
  if (m_RenalGraph == nullptr)
    m_RenalGraph = &CreateLiquidGraph(BGE::Graph::Renal);
  return *m_RenalGraph;
}
SEGasCompartmentGraph& BioGearsCompartments::GetActiveRespiratoryGraph()
{
  switch (m_data.GetAirwayMode()) {
  case CDM::enumBioGearsAirwayMode::Free:
    if (m_UpdateActiveAirwayGraph)
      m_data.GetCompartments().UpdateLinks(*m_RespiratoryGraph);
    m_UpdateActiveAirwayGraph = false;
    return *m_RespiratoryGraph;
  case CDM::enumBioGearsAirwayMode::AnesthesiaMachine:
    if (m_UpdateActiveAirwayGraph)
      m_data.GetCompartments().UpdateLinks(*m_CombinedRespiratoryAnesthesiaGraph);
    m_UpdateActiveAirwayGraph = false;
    return *m_CombinedRespiratoryAnesthesiaGraph;
  case CDM::enumBioGearsAirwayMode::Inhaler:
    if (m_UpdateActiveAirwayGraph)
      m_data.GetCompartments().UpdateLinks(*m_CombinedRespiratoryInhalerGraph);
    m_UpdateActiveAirwayGraph = false;
    return *m_CombinedRespiratoryInhalerGraph;
  case CDM::enumBioGearsAirwayMode::MechanicalVentilator:
    if (m_UpdateActiveAirwayGraph)
      m_data.GetCompartments().UpdateLinks(*m_CombinedRespiratoryMechanicalVentilatorGraph);
    m_UpdateActiveAirwayGraph = false;
    return *m_CombinedRespiratoryMechanicalVentilatorGraph;
  default:
    throw CommonDataModelException("Unknown airway mode");
  }
}
SEGasCompartmentGraph& BioGearsCompartments::GetRespiratoryGraph()
{
  if (m_RespiratoryGraph == nullptr)
    m_RespiratoryGraph = &CreateGasGraph(BGE::Graph::Respiratory);
  return *m_RespiratoryGraph;
}
SEGasCompartmentGraph& BioGearsCompartments::GetAnesthesiaMachineGraph()
{
  if (m_AnesthesiaMachineGraph == nullptr)
    m_AnesthesiaMachineGraph = &CreateGasGraph(BGE::Graph::AnesthesiaMachine);
  return *m_AnesthesiaMachineGraph;
}
SEGasCompartmentGraph& BioGearsCompartments::GetRespiratoryAndAnesthesiaMachineGraph()
{
  if (m_CombinedRespiratoryAnesthesiaGraph == nullptr)
    m_CombinedRespiratoryAnesthesiaGraph = &CreateGasGraph(BGE::Graph::RespiratoryAndAnesthesiaMachine);
  return *m_CombinedRespiratoryAnesthesiaGraph;
}

SEGasCompartmentGraph& BioGearsCompartments::GetRespiratoryAndInhalerGraph()
{
  if (m_CombinedRespiratoryInhalerGraph == nullptr)
    m_CombinedRespiratoryInhalerGraph = &CreateGasGraph(BGE::Graph::RespiratoryAndInhaler);
  return *m_CombinedRespiratoryInhalerGraph;
}
SELiquidCompartmentGraph& BioGearsCompartments::GetActiveAerosolGraph()
{
  switch (m_data.GetAirwayMode()) {
  case CDM::enumBioGearsAirwayMode::Free:
  case CDM::enumBioGearsAirwayMode::AnesthesiaMachine:
  case CDM::enumBioGearsAirwayMode::MechanicalVentilator: // Just use the regular graph
    if (m_UpdateActiveAerosolGraph)
      m_data.GetCompartments().UpdateLinks(*m_AerosolGraph);
    m_UpdateActiveAerosolGraph = false;
    return *m_AerosolGraph;
  case CDM::enumBioGearsAirwayMode::Inhaler:
    if (m_UpdateActiveAerosolGraph)
      m_data.GetCompartments().UpdateLinks(*m_CombinedAerosolInhalerGraph);
    m_UpdateActiveAerosolGraph = true;
    return *m_CombinedAerosolInhalerGraph;
  default:
    throw CommonDataModelException("Unknown airway mode");
  }
}
SELiquidCompartmentGraph& BioGearsCompartments::GetAerosolGraph()
{
  if (m_AerosolGraph == nullptr)
    m_AerosolGraph = &CreateLiquidGraph(BGE::Graph::Aerosol);
  return *m_AerosolGraph;
}
SELiquidCompartmentGraph& BioGearsCompartments::GetAerosolAndInhalerGraph()
{
  if (m_CombinedAerosolInhalerGraph == nullptr)
    m_CombinedAerosolInhalerGraph = &CreateLiquidGraph(BGE::Graph::AerosolAndInhaler);
  return *m_CombinedAerosolInhalerGraph;
}

SEGasCompartmentGraph& BioGearsCompartments::GetRespiratoryAndMechanicalVentilatorGraph()
{
  if (m_CombinedRespiratoryMechanicalVentilatorGraph == nullptr)
    m_CombinedRespiratoryMechanicalVentilatorGraph = &CreateGasGraph(BGE::Graph::RespiratoryAndMechanicalVentilator);
  return *m_CombinedRespiratoryMechanicalVentilatorGraph;
}
}