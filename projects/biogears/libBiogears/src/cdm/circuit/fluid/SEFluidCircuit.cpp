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
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>

namespace biogears {
template class SECircuit<CDM::FluidCircuitData, SEFluidCircuitNode, CDM::FluidCircuitNodeData, SEFluidCircuitPath, CDM::FluidCircuitPathData>;

template class std::vector<SEFluidCircuitNode*>;
template class std::vector<SEFluidCircuitPath*>;
template class std::map<const SEFluidCircuitNode*, std::vector<SEFluidCircuitPath*>*>;
template class std::map<const SEFluidCircuitNode*, size_t>;


SEFluidCircuit::SEFluidCircuit(const char* name, SECircuitManager& mgr)
  : SECircuit<CDM::FluidCircuitData, SEFluidCircuitNode, CDM::FluidCircuitNodeData, SEFluidCircuitPath, CDM::FluidCircuitPathData>(name, mgr.GetLogger())
  , m_Mgr(mgr)
{
}
//-----------------------------------------------------------------------------
SEFluidCircuit::SEFluidCircuit(const std::string& name, SECircuitManager& mgr)
  : SECircuit<CDM::FluidCircuitData, SEFluidCircuitNode, CDM::FluidCircuitNodeData, SEFluidCircuitPath, CDM::FluidCircuitPathData>(name, mgr.GetLogger())
  , m_Mgr(mgr)
{
}
//-----------------------------------------------------------------------------
SEFluidCircuit::~SEFluidCircuit()
{
  Clear();
}
//-----------------------------------------------------------------------------
SEFluidCircuitNode& SEFluidCircuit::CreateNode(const char* name)
{
  return CreateNode(std::string { name });
}
//-----------------------------------------------------------------------------
SEFluidCircuitNode& SEFluidCircuit::CreateNode(const std::string& name)
{
  SEFluidCircuitNode* node = m_Mgr.GetFluidNode(name);
  if (node == nullptr)
    node = &m_Mgr.CreateFluidNode(name);
  AddNode(*node);
  return *node;
}
//-----------------------------------------------------------------------------
SEFluidCircuitPath& SEFluidCircuit::CreatePath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const char* name)
{
  return CreatePath(src, tgt, std::string { name });
}
//-----------------------------------------------------------------------------
SEFluidCircuitPath& SEFluidCircuit::CreatePath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const std::string& name)
{
  SEFluidCircuitPath* path = m_Mgr.GetFluidPath(name);
  if (path == nullptr)
    path = &m_Mgr.CreateFluidPath(src, tgt, name);
  AddPath(*path);
  return *path;
}
//-----------------------------------------------------------------------------
void SEFluidCircuit::AddCircuit(SEFluidCircuit& circuit)
{
  for (SEFluidCircuitNode* node : circuit.GetNodes())
    AddNode(*node);
  for (SEFluidCircuitPath* path : circuit.GetPaths())
    AddPath(*path);
  for (SEFluidCircuitNode* node : circuit.GetReferenceNodes())
    AddReferenceNode(*node);
}
//-----------------------------------------------------------------------------
}