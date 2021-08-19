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

#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/circuit/electrical/SEElectricalCircuit.h>

namespace std {
 template class vector<biogears::SEElectricalCircuitNode*>;
 template class vector<biogears::SEElectricalCircuitPath*>;
 template class map<const biogears::SEElectricalCircuitNode*, vector<biogears::SEElectricalCircuitPath*>*>;
 template class map<const biogears::SEElectricalCircuitNode*, size_t>;
}
namespace biogears {

template class SECircuit<CDM::ElectricalCircuitData, SEElectricalCircuitNode, CDM::ElectricalCircuitNodeData, SEElectricalCircuitPath, CDM::ElectricalCircuitPathData>;

SEElectricalCircuit::SEElectricalCircuit(const std::string& name, SECircuitManager& mgr)
  : SECircuit<CDM::ElectricalCircuitData, SEElectricalCircuitNode, CDM::ElectricalCircuitNodeData, SEElectricalCircuitPath, CDM::ElectricalCircuitPathData>(name, mgr.GetLogger())
  , m_Mgr(mgr)
{
}
//-------------------------------------------------------------------------------
SEElectricalCircuit::SEElectricalCircuit(const char* name, SECircuitManager& mgr)
  : SECircuit<CDM::ElectricalCircuitData, SEElectricalCircuitNode, CDM::ElectricalCircuitNodeData, SEElectricalCircuitPath, CDM::ElectricalCircuitPathData>(name, mgr.GetLogger())
  , m_Mgr(mgr)
{
}
//-------------------------------------------------------------------------------
SEElectricalCircuit::~SEElectricalCircuit()
{
}
//-------------------------------------------------------------------------------
SEElectricalCircuitNode& SEElectricalCircuit::CreateNode(const char* name)
{
  return CreateNode(std::string { name });
}
//-------------------------------------------------------------------------------
SEElectricalCircuitNode& SEElectricalCircuit::CreateNode(const std::string& name)
{
  SEElectricalCircuitNode* node = m_Mgr.GetElectricalNode(name);
  if (node == nullptr)
    node = &m_Mgr.CreateElectricalNode(name);
  AddNode(*node);
  return *node;
}
//-------------------------------------------------------------------------------
SEElectricalCircuitPath& SEElectricalCircuit::CreatePath(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const char* name)
{
  return CreatePath(src, tgt, std::string { name });
}
//-------------------------------------------------------------------------------
SEElectricalCircuitPath& SEElectricalCircuit::CreatePath(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const std::string& name)
{
  SEElectricalCircuitPath* path = m_Mgr.GetElectricalPath(name);
  if (path == nullptr)
    path = &m_Mgr.CreateElectricalPath(src, tgt, name);
  AddPath(*path);
  return *path;
}
//-------------------------------------------------------------------------------
void SEElectricalCircuit::AddCircuit(SEElectricalCircuit& circuit)
{
  for (SEElectricalCircuitNode* node : circuit.GetNodes())
    AddNode(*node);
  for (SEElectricalCircuitPath* path : circuit.GetPaths())
    AddPath(*path);
  for (SEElectricalCircuitNode* node : circuit.GetReferenceNodes())
    AddReferenceNode(*node);
}
//-------------------------------------------------------------------------------
}