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

#include <biogears/cdm/circuit/SECombinedCircuit.h>


namespace biogears {

template <COMBINED_CIRCUIT_TEMPLATE>
SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::SECombinedCircuit(const char* name, Logger* logger)
  : SECombinedCircuit(std::string{ name }, logger)
{
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::SECombinedCircuit(const std::string& name, Logger* logger)
  : CircuitType(name, logger)
{
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::~SECombinedCircuit()
{
  Clear();
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::Clear()
{
  m_Nodes.clear();
  m_Paths.clear();
  DELETE_VECTOR(m_LinkNodes);
  DELETE_VECTOR(m_LinkPaths);
  m_RemovedNodes.clear();
  m_RemovedPaths.clear();
  m_Circuits.clear();
  CircuitType::Clear();
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::AddCircuit(CircuitType& c)
{
  for (NodeType* n : c.GetNodes()) {
    NodeType* mine = CircuitType::GetNode(n->GetName());
    if (!Contains(m_Nodes, (*n)))
      m_Nodes.push_back(n); // No Copies
    if (mine != nullptr && mine != n)
      Warning(GetName() + "Circuit already contains a node named : " + n->GetName() + " from " + c.GetName());
  }
  for (PathType* p : c.GetPaths()) {
    PathType* mine = CircuitType::GetPath(p->GetName());
    if (!Contains(m_Paths, (*p)))
      m_Paths.push_back(p); // No Copies
    if (mine != nullptr && mine != p)
      Warning(GetName() + "Circuit already contains a path named : " + p->GetName() + " from " + c.GetName());
  }
  for (NodeType* ref : c.GetReferenceNodes())
    AddReferenceNode(*ref);
  m_Circuits[c.GetName()] = &c;
  StateChange();
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::RemoveCircuit(CircuitType& c)
{
  // make sure we have this circuit in our map
  // For each node and path, only remove it if it's not in another sub circuit
  // Make sure we update the removed nodes/paths lists as well
  Fatal("Unimplemented method :: SECombinedCircuit::RemoveCircuit"); // I am being lazy...
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
CircuitType* SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::GetCircuit(const char* name)
{
  return GetCircuit(std::string{ name });
}
//-----------------------------------------------------------------------------
  template <COMBINED_CIRCUIT_TEMPLATE>
CircuitType* SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::GetCircuit(const std::string& name)
{
  auto c = m_Circuits.find(name);
  if (c == m_Circuits.end())
    return nullptr;
  return c->second;
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
NodeType& SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::CreateNode(const char* name)
{
  return CreateNode(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
NodeType& SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::CreateNode(const std::string& name)
{
  NodeType& n = CircuitType::CreateNode(name);
  m_LinkNodes.push_back(&n);
  return n;
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
NodeType* SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::GetNode(const char* name) const
{
  return GetNode(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
NodeType* SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::GetNode(const std::string& name) const
{
  for (NodeType* n : m_Nodes) {
    if (n->GetName() == name)
      return n;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::DeleteNode(const NodeType& node)
{
  size_t i = 0;
  for (NodeType* n : m_LinkNodes) {
    if (n == &node) {
      RemoveNode(node);
      m_LinkNodes.erase(m_LinkNodes.begin() + i);
      SAFE_DELETE(n);
      return;
    }
    i++;
  }
  Warning(GetName() + " does not own node " + node.GetName() + ", node is not deleted");
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::DeleteNode(const char* name)
{
  DeleteNode(std::string{ name });
}
//-----------------------------------------------------------------------------
  template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::DeleteNode(const std::string& name)
{
  size_t i = 0;
  for (NodeType* n : m_LinkNodes) {
    if (n->GetName() == name) {
      RemoveNode(name);
      m_LinkNodes.erase(m_LinkNodes.begin() + i);
      SAFE_DELETE(n);
      return;
    }
    i++;
  }
  Warning(GetName() + " does not own node " + name + ", node is not deleted");
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::RemoveNode(const NodeType& node)
{
  size_t i = 0;
  for (NodeType* n : m_Nodes) {
    if (n == &node) {
      m_Nodes.erase(m_Nodes.begin() + i);
      m_RemovedNodes.push_back(node.GetName());
      return;
    }
    i++;
  }
  Warning(GetName() + " does not have node " + node.GetName() + ", node is not removed");
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::RemoveNode(const char* name)
{
  RemoveNode(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::RemoveNode(const std::string& name)
{
  size_t i = 0;
  for (NodeType* n : m_Nodes) {
    if (n->GetName() == name) {
      m_Nodes.erase(m_Nodes.begin() + i);
      m_RemovedNodes.push_back(name);
      return;
    }
    i++;
  }
  Warning(GetName() + " does not have node " + name + ", node is not removed");
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
PathType& SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::CreatePath(NodeType& src, NodeType& tgt, const char* name)
{
  return CreatePath(src, tgt, std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
PathType& SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::CreatePath(NodeType& src, NodeType& tgt, const std::string& name)
{
  PathType& p = CircuitType::CreatePath(src, tgt, name);
  m_LinkPaths.push_back(&p);
  return p;
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
PathType* SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::GetPath(const char* path) const
{
  return GetPath(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
PathType* SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::GetPath(const std::string& path) const
{
  for (PathType* p : m_Paths) {
    if (p->GetName() == name)
      return p;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::DeletePath(const PathType& path)
{
  size_t i = 0;
  for (PathType* p : m_LinkPaths) {
    if (p == &path) {
      RemovePath(path);
      m_LinkPaths.erase(m_LinkPaths.begin() + i);
      SAFE_DELETE(p);
      return;
    }
    i++;
  }
  Warning(GetName() + " does not own path " + path.GetName() + ", path is not deleted");
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::DeletePath(const char* name)
{
  DeletePath(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::DeletePath(const std::string& name)
{
  size_t i = 0;
  for (PathType* p : m_LinkPaths) {
    if (p->GetName() == name) {
      RemovePath(name);
      m_LinkPaths.erase(m_LinkPaths.begin() + i);
      SAFE_DELETE(p);
      return;
    }
    i++;
  }
  Warning(GetName() + " does not own path " + name + ", path is not deleted");
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::RemovePath(const PathType& path)
{
  size_t i = 0;
  for (PathType* p : m_Paths) {
    if (p == &path) {
      m_Paths.erase(m_Paths.begin() + i);
      m_RemovedPaths.push_back(path.GetName());
      return;
    }
    i++;
  }
  Warning(GetName() + " does not have path " + path.GetName() + ", path is not removed");
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::RemovePath(const char* name)
{
  RemovePath(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMBINED_CIRCUIT_TEMPLATE>
void SECombinedCircuit<COMBINED_CIRCUIT_TYPES>::RemovePath(const std::string& name)
{
  size_t i = 0;
  for (PathType* p : m_Paths) {
    if (p->GetName() == name) {
      m_Paths.erase(m_Paths.begin() + i);
      m_RemovedPaths.push_back(name);
      return;
    }
    i++;
  }
  Warning(GetName() + " does not have path " + name + ", path is not removed");
}
//-----------------------------------------------------------------------------
}