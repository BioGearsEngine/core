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

#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>

#include "io/cdm/Circuit.h"
#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/circuit/SECircuitLedger.h>

namespace std {
template class vector<biogears::SEThermalCircuitNode*>;
template class vector<biogears::SEThermalCircuitPath*>;
template class map<const biogears::SEThermalCircuitNode*, vector<biogears::SEThermalCircuitPath*>*>;
template class map<const biogears::SEThermalCircuitNode*, size_t>;
}

namespace biogears {

template class SECircuit<CDM::ThermalCircuitData, SEThermalCircuitNode, CDM::ThermalCircuitNodeData, SEThermalCircuitPath, CDM::ThermalCircuitPathData>;

SEThermalCircuit::SEThermalCircuit(const char* name, SECircuitManager& mgr)
  : SEThermalCircuit(std::string { name }, mgr)
{
}
//-------------------------------------------------------------------------------
SEThermalCircuit::SEThermalCircuit(const std::string& name, SECircuitManager& mgr)
  : SECircuit<CDM::ThermalCircuitData, SEThermalCircuitNode, CDM::ThermalCircuitNodeData, SEThermalCircuitPath, CDM::ThermalCircuitPathData>(name, mgr.GetLogger())
  , m_Mgr(mgr)
{
}
//-------------------------------------------------------------------------------
SEThermalCircuit::~SEThermalCircuit()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEThermalCircuit::Unload(CDM::ThermalCircuitData& data) const
{
  io::Circuit::UnMarshall(*this, data);
}
bool SEThermalCircuit::Load(const CDM::ThermalCircuitData& in, SECircuitLedger<SEThermalCircuitNode, SEThermalCircuitPath, SEThermalCircuit> const& ledger)
{ // note: not clearing here as the derived class needs to clear and call this super class Load last to get the ref node hooked up
  //io::Circuit::Marshall(in, ledger, *this);
  return true;
}
//-------------------------------------------------------------------------------
SEThermalCircuitNode& SEThermalCircuit::CreateNode(const char* name)
{
  return CreateNode(std::string { name });
}
//-------------------------------------------------------------------------------
SEThermalCircuitNode& SEThermalCircuit::CreateNode(const std::string& name)
{
  SEThermalCircuitNode* node = m_Mgr.GetThermalNode(name);
  if (node == nullptr)
    node = &m_Mgr.CreateThermalNode(name);
  AddNode(*node);
  return *node;
}
//-------------------------------------------------------------------------------
SEThermalCircuitPath& SEThermalCircuit::CreatePath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const char* name)
{
  return CreatePath(src, tgt, std::string { name });
}
//-------------------------------------------------------------------------------
SEThermalCircuitPath& SEThermalCircuit::CreatePath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name)
{
  SEThermalCircuitPath* path = m_Mgr.GetThermalPath(name);
  if (path == nullptr)
    path = &m_Mgr.CreateThermalPath(src, tgt, name);
  AddPath(*path);
  return *path;
}
//-------------------------------------------------------------------------------
void SEThermalCircuit::AddCircuit(SEThermalCircuit& circuit)
{
  for (SEThermalCircuitNode* node : circuit.GetNodes())
    AddNode(*node);
  for (SEThermalCircuitPath* path : circuit.GetPaths())
    AddPath(*path);
  for (SEThermalCircuitNode* node : circuit.GetReferenceNodes())
    AddReferenceNode(*node);
}
//-------------------------------------------------------------------------------
}