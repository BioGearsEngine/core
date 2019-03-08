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
#include <biogears/cdm/compartment/SECompartmentNodes.h>
#include <biogears/cdm/properties/SEScalar.h>

namespace biogears {
template <COMPARTMENT_NODE_TEMPLATE>
SECompartmentNodes<COMPARTMENT_NODE_TYPES>::SECompartmentNodes(Logger* logger)
  : Loggable(logger)
{
  m_Quantity = nullptr;
  m_Potential = nullptr;
}

template <COMPARTMENT_NODE_TEMPLATE>
SECompartmentNodes<COMPARTMENT_NODE_TYPES>::~SECompartmentNodes()
{
  Clear();
}

template <COMPARTMENT_NODE_TEMPLATE>
void SECompartmentNodes<COMPARTMENT_NODE_TYPES>::Clear()
{
  m_QuantityNodes.clear();
  m_AllNodes.clear();
  SAFE_DELETE(m_Quantity);
  SAFE_DELETE(m_Potential);
}

template <COMPARTMENT_NODE_TEMPLATE>
void SECompartmentNodes<COMPARTMENT_NODE_TYPES>::MapNode(NodeType& node)
{
  if (std::find(m_AllNodes.begin(), m_AllNodes.end(), &node) == m_AllNodes.end())
    m_AllNodes.push_back(&node);
  SortNode(node);
}
template <COMPARTMENT_NODE_TEMPLATE>
void SECompartmentNodes<COMPARTMENT_NODE_TYPES>::RemoveNode(NodeType& node)
{
  Remove(m_AllNodes, &node);
  StateChange();
}

template <COMPARTMENT_NODE_TEMPLATE>
QuantityScalar& SECompartmentNodes<COMPARTMENT_NODE_TYPES>::GetQuantity()
{
  if (m_QuantityNodes.size() == 1)
    return m_QuantityNodes[0]->GetNextQuantity();
  if (m_Quantity == nullptr)
    m_Quantity = new QuantityScalar();
  m_Quantity->SetReadOnly(false);
  m_Quantity->Invalidate();
  for (NodeType* n : m_QuantityNodes)
    m_Quantity->Increment(n->GetNextQuantity());
  m_Quantity->SetReadOnly(true);
  return *m_Quantity;
}
template <COMPARTMENT_NODE_TEMPLATE>
double SECompartmentNodes<COMPARTMENT_NODE_TYPES>::GetQuantity(const QuantityUnit& unit) const
{
  if (m_QuantityNodes.empty())
    return SEScalar::dNaN();
  if (m_QuantityNodes.size() == 1)
    return m_QuantityNodes[0]->GetNextQuantity().GetValue(unit);
  double d = 0;
  for (NodeType* n : m_QuantityNodes)
    d += n->GetNextQuantity().GetValue(unit);
  return d;
}

template <COMPARTMENT_NODE_TEMPLATE>
bool SECompartmentNodes<COMPARTMENT_NODE_TYPES>::HasPotential() const
{
  for (NodeType* n : m_AllNodes)
    if (n->HasNextPotential())
      return true;
  return false;
}
template <COMPARTMENT_NODE_TEMPLATE>
PotentialScalar& SECompartmentNodes<COMPARTMENT_NODE_TYPES>::GetPotential()
{
  if (m_AllNodes.size() == 1)
    return m_AllNodes[0]->GetNextPotential();

  const PotentialUnit* punit = m_AllNodes[0]->GetNextPotential().GetUnit();

  if (m_Potential == nullptr)
    m_Potential = new PotentialScalar();
  m_Potential->SetReadOnly(false);
  m_Potential->Invalidate();
  if (punit != nullptr)
    m_Potential->SetValue(GetPotential(*punit), *punit);
  m_Potential->SetReadOnly(true);
  return *m_Potential;
}
template <COMPARTMENT_NODE_TEMPLATE>
double SECompartmentNodes<COMPARTMENT_NODE_TYPES>::GetPotential(const PotentialUnit& punit) const
{
  if (!HasPotential())
    return SEScalar::dNaN();
  if (m_AllNodes.size() == 1)
    return m_AllNodes[0]->GetNextPotential().GetValue(punit);

  // Every node will have a potential after its calculated
  // But in case your nodes are not calculated, i need to see if you have a pressure

  double t = 0;
  if (!m_QuantityNodes.empty()) {
    // Volume weight by only the nodes with volume and pressure
    const QuantityUnit* qunit = m_QuantityNodes[0]->GetNextQuantity().GetUnit();
    double totalQuantity = 0;
    for (NodeType* n : m_QuantityNodes)
      if (n->HasNextPotential())
        totalQuantity += n->GetNextQuantity().GetValue(*qunit);
    for (NodeType* n : m_QuantityNodes)
      if (n->HasNextPotential())
        t += n->GetNextPotential().GetValue(punit) * (n->GetNextQuantity().GetValue(*qunit) / totalQuantity);
  } else {
    int num = 0;
    for (NodeType* n : m_AllNodes)
      if (n->HasNextPotential()) {
        num++;
        t += n->GetNextPotential().GetValue(punit);
      }
    t = t / num;
  }
  return t;
}

template <COMPARTMENT_NODE_TEMPLATE>
void SECompartmentNodes<COMPARTMENT_NODE_TYPES>::SortNode(NodeType& node)
{
  if (!Contains(m_QuantityNodes, node)) {
    if (node.HasQuantityBaseline())
      m_QuantityNodes.push_back(&node);
  } else {
    if (!node.HasQuantityBaseline())
      Remove(m_QuantityNodes, &node);
  }
}
template <COMPARTMENT_NODE_TEMPLATE>
void SECompartmentNodes<COMPARTMENT_NODE_TYPES>::StateChange()
{
  m_QuantityNodes.clear();
  for (NodeType* node : m_AllNodes)
    SortNode(*node);
}
}