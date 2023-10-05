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
#include <biogears/cdm/compartment/SECompartmentTransportGraph.h>

namespace biogears {
template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::SECompartmentTransportGraph(const char* name, Logger* logger)
  : SECompartmentTransportGraph(std::string { name }, logger)
{
}
//-------------------------------------------------------------------------------
template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::SECompartmentTransportGraph(const std::string& name, Logger* logger)
  : SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>(name, logger)
{
}
//-------------------------------------------------------------------------------

template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::~SECompartmentTransportGraph()
{
  Clear();
}
//-------------------------------------------------------------------------------

template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
void SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::Clear()
{
  SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::Clear();
  for (auto& itr : m_TargetEdgeMap)
    delete itr.second;
  for (auto& itr : m_SourceEdgeMap)
    delete itr.second;
  m_Verticies.clear();
  m_VertexIndicies.clear();
  m_TargetEdgeMap.clear();
  m_SourceEdgeMap.clear();
}
//-------------------------------------------------------------------------------

template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
std::string SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::GetName() const
{
  return this->m_Name;
}
//-------------------------------------------------------------------------------

template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
const char* SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::GetName_cStr() const
{
  return this->m_Name.c_str();
}
//-------------------------------------------------------------------------------

template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
void SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::StateChange()
{
  for (auto& itr : m_TargetEdgeMap)
    delete itr.second;
  for (auto& itr : m_SourceEdgeMap)
    delete itr.second;
  m_TargetEdgeMap.clear();
  m_SourceEdgeMap.clear();
  // Cache what paths are connected to what nodes
  for (CompartmentLinkType* link : this->m_CompartmentLinks) {
    CompartmentType* cSrc = &link->GetSourceCompartment();
    CompartmentType* cTgt = &link->GetTargetCompartment();
    // Source Edges
    std::vector<GraphEdgeType*>* srcEdges = m_SourceEdgeMap[cSrc];
    if (srcEdges == nullptr) {
      srcEdges = new std::vector<GraphEdgeType*>();
      m_SourceEdgeMap[cSrc] = srcEdges;
    }
    srcEdges->push_back(link);
    // Target Edges
    std::vector<GraphEdgeType*>* tgtEdges = m_TargetEdgeMap[cTgt];
    if (tgtEdges == nullptr) {
      tgtEdges = new std::vector<GraphEdgeType*>();
      m_TargetEdgeMap[cTgt] = tgtEdges;
    }
    tgtEdges->push_back(link);
  }
  // Now push the compartments into graph lists for transport
  int i = 0;
  m_Verticies.clear();
  m_VertexIndicies.clear();
  for (CompartmentType* c : this->m_Compartments) {
    m_Verticies.push_back(c);
    m_VertexIndicies[c] = i++;
  }
}
//-------------------------------------------------------------------------------

template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
size_t SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::GetVertexIndex(const GraphVertexType& v) const
{
  auto itr = m_VertexIndicies.find(&v);
  if (itr == m_VertexIndicies.end()) {
    this->Error("Vertex is not in Graph Index Map.");
    return -1;
  }
  return itr->second;
}
//-------------------------------------------------------------------------------
template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
const std::vector<GraphVertexType*>& SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::GetVerticies() const
{
  return m_Verticies;
}
//-------------------------------------------------------------------------------
template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
const std::vector<GraphEdgeType*>* SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::GetSourceEdges(const GraphVertexType& v) const
{
  auto itr = m_SourceEdgeMap.find(&v);
  if (itr == m_SourceEdgeMap.end())
    return nullptr;
  return itr->second;
}
//-------------------------------------------------------------------------------
template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
const std::vector<GraphEdgeType*>* SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::GetTargetEdges(const GraphVertexType& v) const
{
  auto itr = m_TargetEdgeMap.find(&v);
  if (itr == m_TargetEdgeMap.end())
    return nullptr;
  return itr->second;
}
//-------------------------------------------------------------------------------
template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
bool SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::operator==(SECompartmentTransportGraph const& rhs) const
{

  bool equivilant = m_Verticies.size() == rhs.m_Verticies.size();
  if (equivilant) {
    for (auto i = 0; equivilant && i < m_Verticies.size(); ++i) {
      equivilant &= (m_Verticies[i] && rhs.m_Verticies[i])
        ? m_Verticies[i]->operator==(*rhs.m_Verticies[i])
        : m_Verticies[i] == rhs.m_Verticies[i];
    }
  }

  equivilant &= m_VertexIndicies.size() == rhs.m_VertexIndicies.size();
  if (equivilant) {
    for (auto itr = m_VertexIndicies.begin(); equivilant && itr != m_VertexIndicies.end(); ++itr) {
      equivilant &= (itr->second && rhs.m_VertexIndicies.at(itr->first))
        ? itr->second == rhs.m_VertexIndicies.at(itr->first)
        : itr->second == rhs.m_VertexIndicies.at(itr->first);
    }
  }

  equivilant &= m_SourceEdgeMap.size() == rhs.m_SourceEdgeMap.size();
  if (equivilant) {
    for (auto itr = m_SourceEdgeMap.begin(); equivilant && itr != m_SourceEdgeMap.end(); ++itr) {
      auto rhs_itr = rhs.m_SourceEdgeMap.at(itr->first);
      equivilant &= (itr->second->size() && rhs_itr->size());
      for (auto i = 0; equivilant && i < itr->second->size(); ++i) {
        equivilant &= (itr->second->operator[](i) && rhs_itr->operator[](i))
          ? itr->second->operator[](i)->operator==(*rhs_itr->operator[](i))
          : itr->second->operator[](i) == rhs_itr->operator[](i);
      }
    }
  }

  equivilant &= m_TargetEdgeMap.size() == rhs.m_TargetEdgeMap.size();
  if (equivilant) {
    for (auto itr = m_TargetEdgeMap.begin(); equivilant && itr != m_TargetEdgeMap.end(); ++itr) {
      auto rhs_itr = rhs.m_TargetEdgeMap.at(itr->first);
      equivilant &= (itr->second->size() && rhs_itr->size());
      for (auto i = 0; equivilant && i < itr->second->size(); ++i) {
        equivilant &= (itr->second->operator[](i) && rhs_itr->operator[](i))
          ? itr->second->operator[](i)->operator==(*rhs_itr->operator[](i))
          : itr->second->operator[](i) == rhs_itr->operator[](i);
      }
    }
  }
  return equivilant;
}

template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
bool SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::operator!=(SECompartmentTransportGraph const& rhs) const
{
  return !(this->operator==(rhs));
}
//-------------------------------------------------------------------------------
template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
bool SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::operator==(GraphEdgeType const& rhs) const
{
  auto ptr = dynamic_cast<decltype(this)>(&rhs);
  return (ptr) ? this->operator==(*ptr) : false;
}
template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
bool SECompartmentTransportGraph<COMPARTMENT_TRANSPORT_GRAPH_TYPES>::operator!=(GraphEdgeType const& rhs) const
{
  return !(this->operator==(rhs));
}
}