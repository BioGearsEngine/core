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
#include <biogears/cdm/compartment/SECompartmentGraph.h>

namespace biogears {
template <COMPARTMENT_GRAPH_TEMPLATE>
SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::SECompartmentGraph(const char* name, Logger* logger)
  : SECompartmentGraph(std::string{ name }, logger)
{
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::SECompartmentGraph(const std::string& name, Logger* logger)
  : Loggable(logger)
  , m_Name(name)
{
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::~SECompartmentGraph()
{
  Clear();
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
void SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::Clear()
{
  m_Compartments.clear();
  m_CompartmentLinks.clear();
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
std::string SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
const char* SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
void SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::AddCompartment(CompartmentType& cmpt)
{
  if (GetCompartment(cmpt.GetName()) == nullptr)
    m_Compartments.push_back(&cmpt);
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
CompartmentType* SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetCompartment(const char* name)
{
  return GetCompartment(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
CompartmentType* SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetCompartment(const std::string& name)
{
  for (CompartmentType* c : m_Compartments) {
    if (c->GetName() == name)
      return c;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
const CompartmentType* SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetCompartment(const char* name) const
{
  return GetCompartment(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
const CompartmentType* SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetCompartment(const std::string& name) const
{
  for (CompartmentType* c : m_Compartments) {
    if (c->GetName() == name)
      return c;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
const std::vector<CompartmentType*>& SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetCompartments() const
{
  return m_Compartments;
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
void SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::RemoveCompartment(const CompartmentType& Compartment)
{
  size_t i = 0;
  for (CompartmentType* n : m_Compartments) {
    if (n == &Compartment) {
      m_Compartments.erase(m_Compartments.begin() + i);
      return;
    }
    i++;
  }
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
void SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::RemoveCompartment(const char* name)
{
  RemoveCompartment(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
void SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::RemoveCompartment(const std::string& name)
{
  size_t i = 0;
  for (CompartmentType* n : m_Compartments) {
    if (n->GetName() == name) {
      m_Compartments.erase(m_Compartments.begin() + i);
      return;
    }
    i++;
  }
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
void SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::AddLink(CompartmentLinkType& link)
{
  if (GetLink(link.GetName()) == nullptr)
    m_CompartmentLinks.push_back(&link);
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
CompartmentLinkType* SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetLink(const char* name)
{
  return GetLink(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
CompartmentLinkType* SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetLink(const std::string& name)
{
  for (CompartmentLinkType* p : m_CompartmentLinks) {
    if (p->GetName() == name)
      return p;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
const CompartmentLinkType* SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetLink(const char* name) const
{
  return GetLink(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
const CompartmentLinkType* SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetLink(const std::string& name) const
{
  for (CompartmentLinkType* p : m_CompartmentLinks) {
    if (p->GetName() == name)
      return p;
  }
  return nullptr;
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
const std::vector<CompartmentLinkType*>& SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::GetLinks() const
{
  return m_CompartmentLinks;
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
void SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::RemoveLink(const CompartmentLinkType& CompartmentLink)
{
  size_t i = 0;
  for (CompartmentLinkType* p : m_CompartmentLinks) {
    if (p == &CompartmentLink) {
      m_CompartmentLinks.erase(m_CompartmentLinks.begin() + i);
      return;
    }
    i++;
  }
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
void SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::RemoveLink(const char* name)
{
  RemoveLink(std::string{ name });
}
//-----------------------------------------------------------------------------
template <COMPARTMENT_GRAPH_TEMPLATE>
void SECompartmentGraph<COMPARTMENT_GRAPH_TYPES>::RemoveLink(const std::string& name)
{
  size_t i = 0;
  for (CompartmentLinkType* p : m_CompartmentLinks) {
    if (p->GetName() == name) {
      m_CompartmentLinks.erase(m_CompartmentLinks.begin() + i);
      return;
    }
    i++;
  }
}
//-----------------------------------------------------------------------------
}