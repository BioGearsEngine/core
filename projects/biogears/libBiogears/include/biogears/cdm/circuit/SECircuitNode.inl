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

//Project Includes
#include <biogears/cdm/circuit/SECircuitNode.h>
#include <biogears/schema/cdm/Circuit.hxx>

namespace biogears {
template <CIRCUIT_NODE_TEMPLATE>
SECircuitNode<CIRCUIT_NODE_TYPES>::SECircuitNode(const char* name, Logger* logger)
  : SECircuitNode(std::string{ name }, logger)
{
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
SECircuitNode<CIRCUIT_NODE_TYPES>::SECircuitNode(const std::string& name, Logger* logger)
  : Loggable(logger)
  , m_Name(name)
{
  m_Potential = nullptr;
  m_NextPotential = nullptr;
  m_Quantity = nullptr;
  m_NextQuantity = nullptr;
  m_QuantityBaseline = nullptr;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
SECircuitNode<CIRCUIT_NODE_TYPES>::~SECircuitNode()
{
  Clear();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::Clear()
{
  SAFE_DELETE(m_Potential);
  SAFE_DELETE(m_NextPotential);
  SAFE_DELETE(m_Quantity);
  SAFE_DELETE(m_NextQuantity);
  SAFE_DELETE(m_QuantityBaseline);
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::Load(const CDM::CircuitNodeData& in)
{
  Clear();
  return true;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
void SECircuitNode<CIRCUIT_NODE_TYPES>::Unload(CDM::CircuitNodeData& data) const
{
  data.Name(m_Name);
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
std::string SECircuitNode<CIRCUIT_NODE_TYPES>::GetName() const
{
  return m_Name;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
const char* SECircuitNode<CIRCUIT_NODE_TYPES>::GetName_cStr() const
{
  return m_Name.c_str();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::HasPotential() const
{
  return m_Potential == nullptr ? false : m_Potential->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
PotentialScalar& SECircuitNode<CIRCUIT_NODE_TYPES>::GetPotential()
{
  if (m_Potential == nullptr)
    m_Potential = new PotentialScalar();
  return *m_Potential;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::HasNextPotential() const
{
  return m_NextPotential == nullptr ? false : m_NextPotential->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
PotentialScalar& SECircuitNode<CIRCUIT_NODE_TYPES>::GetNextPotential()
{
  if (m_NextPotential == nullptr)
    m_NextPotential = new PotentialScalar();
  return *m_NextPotential;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::HasQuantity() const
{
  return m_Quantity == nullptr ? false : m_Quantity->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
QuantityScalar& SECircuitNode<CIRCUIT_NODE_TYPES>::GetQuantity()
{
  if (m_Quantity == nullptr)
    m_Quantity = new QuantityScalar();
  return *m_Quantity;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::HasNextQuantity() const
{
  return m_NextQuantity == nullptr ? false : m_NextQuantity->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
QuantityScalar& SECircuitNode<CIRCUIT_NODE_TYPES>::GetNextQuantity()
{
  if (m_NextQuantity == nullptr)
    m_NextQuantity = new QuantityScalar();
  return *m_NextQuantity;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
bool SECircuitNode<CIRCUIT_NODE_TYPES>::HasQuantityBaseline() const
{
  return m_QuantityBaseline == nullptr ? false : m_QuantityBaseline->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_NODE_TEMPLATE>
QuantityScalar& SECircuitNode<CIRCUIT_NODE_TYPES>::GetQuantityBaseline()
{
  if (m_QuantityBaseline == nullptr)
    m_QuantityBaseline = new QuantityScalar();
  return *m_QuantityBaseline;
}
//-------------------------------------------------------------------------------
}