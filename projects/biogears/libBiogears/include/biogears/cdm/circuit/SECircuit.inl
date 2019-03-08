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
#pragma once

#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/schema/cdm/Circuit.hxx>

#define OPEN_RESISTANCE 1e100

namespace biogears {
template <CIRCUIT_TEMPLATE>
SECircuit<CIRCUIT_TYPES>::SECircuit(const char* name, Logger* logger)
  : Loggable(logger)
  , m_Name(name)
{
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
SECircuit<CIRCUIT_TYPES>::SECircuit(const std::string& name, Logger* logger)
  : Loggable(logger)
  , m_Name(name)
{
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
SECircuit<CIRCUIT_TYPES>::~SECircuit()
{
  Clear();
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::Clear()
{
  m_ReferenceNodes.clear();
  for (auto& itr : m_TargetPathMap)
    delete itr.second;
  for (auto& itr : m_SourcePathMap)
    delete itr.second;
  for (auto& itr : m_ConnectedPathMap)
    delete itr.second;
  m_Nodes.clear();
  m_Paths.clear();
  m_TargetPathMap.clear();
  m_SourcePathMap.clear();
  m_ConnectedPathMap.clear();
  m_CalculatorIndex.clear();
  m_ValvePaths.clear();
  m_PolarizedElementPaths.clear();
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::Load(const CircuitBindType& in, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths)
{ // note: not clearing here as the derived class needs to clear and call this super class Load last to get the ref node hooked up
  Clear();
  m_Name = in.Name();
  for (auto name : in.Node()) {
    auto idx = nodes.find(name);
    if (idx == nodes.end()) {
      Error(m_Name + " could not find node " + name.c_str());
      return false;
    }
    AddNode(*idx->second);
  }
  for (auto name : in.Path()) {
    auto idx = paths.find(name);
    if (idx == paths.end()) {
      Error(m_Name + " could not find path " + name.c_str());
      return false;
    }
    AddPath(*idx->second);
  }
  for (auto name : in.ReferenceNode()) {
    auto idx = nodes.find(name);
    if (idx == nodes.end()) {
      Error(m_Name + " could not find reference node " + name.c_str());
      return false;
    }
    AddReferenceNode(*idx->second);
  }
  StateChange();
  return true;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
CircuitBindType* SECircuit<CIRCUIT_TYPES>::Unload() const
{
  CircuitBindType* data = new CircuitBindType();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::Unload(CircuitBindType& data) const
{
  data.Name(m_Name);
  if (HasReferenceNode()) {
    for (NodeType* n : m_ReferenceNodes)
      data.ReferenceNode().push_back(n->GetName());
  }
  for (auto* n : m_Nodes)
    data.Node().push_back(n->GetName());
  for (auto* p : m_Paths)
    data.Path().push_back(p->GetName());
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::StateChange()
{
  // Cache target and source paths to save lots of time later
  for (auto& itr : m_TargetPathMap)
    delete itr.second;
  for (auto& itr : m_SourcePathMap)
    delete itr.second;
  for (auto& itr : m_ConnectedPathMap)
    delete itr.second;
  m_TargetPathMap.clear();
  m_SourcePathMap.clear();
  m_ConnectedPathMap.clear();
  m_ValvePaths.clear();
  m_PolarizedElementPaths.clear();

  for (PathType* p : m_Paths) {
    if (!p->HasValidElements()) {
      ///\error Fatal: Invalid Path in the Circuit.
      m_ss << p->GetName() << " has invalid elements";
      Fatal(m_ss);
    }

    //There should never be a NextFlow value set on a path
    //Flow sources are defined using NextFlowSource
    /* if (p->HasNextFlux())
    {
    ///\error Fatal: Invalid Path in the Circuit.
    m_ss << p->GetName() << " has flux and will be ignored and overwritten. Use a flux source.";
    Warning(m_ss);
    }*/

    // Cache what paths are connected to what nodes
    NodeType* nTgt = &p->GetTargetNode();
    NodeType* nSrc = &p->GetSourceNode();
    std::vector<PathType*>* tgtPaths = m_TargetPathMap[nTgt];
    std::vector<PathType*>* srcPaths = m_SourcePathMap[nSrc];
    std::vector<PathType*>* conSrcPaths = m_ConnectedPathMap[nSrc];
    std::vector<PathType*>* conTgtPaths = m_ConnectedPathMap[nTgt];
    if (tgtPaths == nullptr) {
      tgtPaths = new std::vector<PathType*>();
      m_TargetPathMap[nTgt] = tgtPaths;
    }
    if (srcPaths == nullptr) {
      srcPaths = new std::vector<PathType*>();
      m_SourcePathMap[nSrc] = srcPaths;
    }
    if (conTgtPaths == nullptr) {
      conTgtPaths = new std::vector<PathType*>();
      m_ConnectedPathMap[nTgt] = conTgtPaths;
    }
    if (conSrcPaths == nullptr) {
      conSrcPaths = new std::vector<PathType*>();
      m_ConnectedPathMap[nSrc] = conSrcPaths;
    }

    //Make sure we didn't accidentally define something twice
    if (std::find(m_ConnectedPathMap[nSrc]->begin(), m_ConnectedPathMap[nSrc]->end(), p) != m_ConnectedPathMap[nSrc]->end() || std::find(m_ConnectedPathMap[nTgt]->begin(), m_ConnectedPathMap[nTgt]->end(), p) != m_ConnectedPathMap[nTgt]->end()) {
      m_ss << p->GetName() << " is defined multiple times.";
      Fatal(m_ss);
    }

    m_TargetPathMap[nTgt]->push_back(p);
    m_SourcePathMap[nSrc]->push_back(p);
    m_ConnectedPathMap[nSrc]->push_back(p);
    m_ConnectedPathMap[nTgt]->push_back(p);

    // Cache Valves
    if (p->HasNextValve())
      m_ValvePaths.push_back(p);
    if (p->HasNextPolarizedState())
      m_PolarizedElementPaths.push_back(p);
  }

  size_t jIdx = 0;
  m_CalculatorIndex.clear();
  for (NodeType* n : m_Nodes) {
    //There should never be a next pressure value set on a node
    //  //Initializing a compliance "charge" is done on the current pressure value
    //  //Pressure sources are defined on a path
    //  if (n->HasNextPotential() && n!=GetReferenceNode())
    //  {
    //    m_ss << "You cannot set a pressure value without using a path pressure source.  The NextPressure value will be ignored and overwritten for Node " << n->GetName();
    //    Warning(m_ss);
    //  }
    if (!IsReferenceNode(*n))
      m_CalculatorIndex[n] = jIdx++;
  }
  if ((m_ValvePaths.size() + m_PolarizedElementPaths.size()) > 64) {
    ///\error Fatal: There are too many assumed state options.  The Circuit solver can only handle up to 64 Diodes and Polar Elements in a single circuit (i.e. ~1.8e19 possible combinations).
    Fatal("There are too many assumed state options.  The Circuit solver can only handle up to 64 Diodes and Polar Elements in a single circuit (i.e. ~1.8e19 possible combinations).");
  }
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
std::string SECircuit<CIRCUIT_TYPES>::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
const char* SECircuit<CIRCUIT_TYPES>::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasReferenceNode() const
{
  return !m_ReferenceNodes.empty();
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::IsReferenceNode(NodeType& node) const
{
  for (NodeType* n : m_ReferenceNodes)
    if (n == &node)
      return true;
  return false;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
const std::vector<NodeType*>& SECircuit<CIRCUIT_TYPES>::GetReferenceNodes() const
{
  return m_ReferenceNodes;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::AddReferenceNode(NodeType& node)
{
  // Make sure we have it
  bool found = false;
  for (NodeType* n : m_Nodes) {
    if (n == &node) {
      found = true;
      break;
    }
  }
  if (!found)
    AddNode(node);
  if (!Contains(m_ReferenceNodes, node))
    m_ReferenceNodes.push_back(&node);
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::AddNode(NodeType& node)
{
  if (!Contains(this->m_Nodes, node))
    this->m_Nodes.push_back(&node);
}
template <CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasNode(NodeType& node)
{
  return Contains(m_Nodes, node);
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasNode(const char* name)
{
  return HasNode(std::string{ name });
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasNode(const std::string& name)
{
  return GetNode(name) != nullptr;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
NodeType* SECircuit<CIRCUIT_TYPES>::GetNode(const char* name)
{
  return GetNode(std::string(name));
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
NodeType* SECircuit<CIRCUIT_TYPES>::GetNode(const std::string& name)
{
  for (NodeType* n : m_Nodes) {
    if (n->GetName() == name)
      return n;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
const NodeType* SECircuit<CIRCUIT_TYPES>::GetNode(const char* name) const
{
  return GetNode(std::string{ name });
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
const NodeType* SECircuit<CIRCUIT_TYPES>::GetNode(const std::string& name) const
{
  for (NodeType* n : m_Nodes) {
    if (n->GetName() == name)
      return n;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
const std::vector<NodeType*>& SECircuit<CIRCUIT_TYPES>::GetNodes() const
{
  return m_Nodes;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::RemoveNode(const NodeType& node)
{
  size_t i = 0;
  for (NodeType* n : m_Nodes) {
    if (n == &node) {
      m_Nodes.erase(m_Nodes.begin() + i);
      return;
    }
    i++;
  }
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::RemoveNode(const char* name)
{
  RemoveNode(std::string{ name });
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::RemoveNode(const std::string& name)
{
  size_t i = 0;
  for (NodeType* n : m_Nodes) {
    if (n->GetName() == name) {
      m_Nodes.erase(m_Nodes.begin() + i);
      return;
    }
    i++;
  }
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
size_t SECircuit<CIRCUIT_TYPES>::GetCalculatorIndex(const NodeType& node) const
{
  auto itr = m_CalculatorIndex.find(&node);
  if (itr == m_CalculatorIndex.end()) {
    Error("Node " + std::string{ node.GetName() } +" is not in Calculator Index Map.");
    return -1;
  }
  return itr->second;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::AddPath(PathType& path)
{
  if (!Contains(m_Paths, path))
    m_Paths.push_back(&path);
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasPath(PathType& path)
{
  return Contains(m_Paths, path);
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasPath(const char* name)
{
  return HasPath(std::string{ name });
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
bool SECircuit<CIRCUIT_TYPES>::HasPath(const std::string& name)
{
  return GetPath(name) != nullptr;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
PathType* SECircuit<CIRCUIT_TYPES>::GetPath(const char* name)
{
  return GetPath(std::string{ name });
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
PathType* SECircuit<CIRCUIT_TYPES>::GetPath(const std::string& name)
{
  for (PathType* p : m_Paths) {
    if (p->GetName() == name)
      return p;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
const PathType* SECircuit<CIRCUIT_TYPES>::GetPath(const char* name) const
{
  return  GetPath(std::string{ name });
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
const PathType* SECircuit<CIRCUIT_TYPES>::GetPath(const std::string& name) const
{
  for (PathType* p : m_Paths) {
    if (p->GetName() == name)
      return p;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
const std::vector<PathType*>& SECircuit<CIRCUIT_TYPES>::GetPaths() const
{
  return m_Paths;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::RemovePath(const PathType& path)
{
  size_t i = 0;
  for (PathType* p : m_Paths) {
    if (p == &path) {
      m_Paths.erase(m_Paths.begin() + i);
      return;
    }
    i++;
  }
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::RemovePath(const char* name)
{
  RemovePath(std::string{ name });
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::RemovePath(const std::string& name)
{
  size_t i = 0;
  for (PathType* p : m_Paths) {
    if (p->GetName() == name) {
      m_Paths.erase(m_Paths.begin() + i);
      return;
    }
    i++;
  }
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
const std::vector<PathType*>& SECircuit<CIRCUIT_TYPES>::GetValvePaths()
{
  return m_ValvePaths;
}
//-----------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
const std::vector<PathType*>& SECircuit<CIRCUIT_TYPES>::GetPolarizedElementPaths()
{
  return m_PolarizedElementPaths;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Returns all target paths for a node
///
/// \param  Circuit The circuit in question.
/// \param  node Circuit node
///
/// \return Vector of circuit paths
///
/// \details
/// Finds all paths where the input node is the target node. Returns a vector of the paths.
template <CIRCUIT_TEMPLATE>
const std::vector<PathType*>* SECircuit<CIRCUIT_TYPES>::GetTargetPaths(const NodeType& node) const
{
  auto itr = m_TargetPathMap.find(&node);
  if (itr == m_TargetPathMap.end()) {
    Error("Circuit does not have path information for node", node.GetName());
    return nullptr;
  }
  return itr->second;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Returns all source paths for a node
///
/// \param  Circuit The circuit in question.
/// \param  node Circuit node
///
/// \return Vector of circuit paths
///
/// \details
/// Finds all paths where the input node is the source node. Returns a vector of the paths.
template <CIRCUIT_TEMPLATE>
const std::vector<PathType*>* SECircuit<CIRCUIT_TYPES>::GetSourcePaths(const NodeType& node) const
{
  auto itr = m_SourcePathMap.find(&node);
  if (itr == m_SourcePathMap.end()) {
    Error("Circuit does not have path information for node", node.GetName());
    return nullptr;
  }
  return itr->second;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Returns all source paths for a node
///
/// \param  Circuit The circuit in question.
/// \param  node Circuit node
///
/// \return Vector of circuit paths
///
/// \details
/// Finds all paths where the input node is the source or target node. Returns a vector of the paths.
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
const std::vector<PathType*>* SECircuit<CIRCUIT_TYPES>::GetConnectedPaths(const NodeType& node) const
{
  auto itr = m_ConnectedPathMap.find(&node);
  if (itr == m_ConnectedPathMap.end()) {
    Error("Circuit does not have path information for node", node.GetName());
    return nullptr;
  }
  return itr->second;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Set all Current and Next values to the Baseline values for all Elements.
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_TEMPLATE>
void SECircuit<CIRCUIT_TYPES>::SetNextAndCurrentFromBaselines()
{
  for (PathType* p : m_Paths) {
    if (p->HasResistanceBaseline()) {
      p->GetResistance().Copy(p->GetResistanceBaseline());
      p->GetNextResistance().Copy(p->GetResistanceBaseline());
    }

    if (p->HasCapacitanceBaseline()) {
      p->GetCapacitance().Copy(p->GetCapacitanceBaseline());
      p->GetNextCapacitance().Copy(p->GetCapacitanceBaseline());
    }

    if (p->HasInductanceBaseline()) {
      p->GetInductance().Copy(p->GetInductanceBaseline());
      p->GetNextInductance().Copy(p->GetInductanceBaseline());
    }

    if (p->HasFluxSourceBaseline()) {
      p->GetFluxSource().Copy(p->GetFluxSourceBaseline());
      p->GetNextFluxSource().Copy(p->GetFluxSourceBaseline());
    }

    if (p->HasPotentialSourceBaseline()) {
      p->GetPotentialSource().Copy(p->GetPotentialSourceBaseline());
      p->GetNextPotentialSource().Copy(p->GetPotentialSourceBaseline());
    }

    if (p->HasSwitch())
      p->SetNextSwitch(p->GetSwitch());
    if (p->HasValve())
      p->SetNextValve(p->GetValve());
  }

  for (NodeType* n : m_Nodes) {
    if (n->HasQuantityBaseline()) {
      n->GetQuantity().Copy(n->GetQuantityBaseline());
      n->GetNextQuantity().Copy(n->GetQuantityBaseline());
    }
  }
}
}