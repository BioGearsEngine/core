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
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>

#include <biogears/cdm/compartment/SECompartmentGraph.inl>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/SECompartmentNodes.inl>
#include <biogears/cdm/compartment/SECompartmentTransportGraph.inl>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/schema/cdm/Compartment.hxx>

namespace biogears {

template class std::vector<SEGasTransportVertex*>;
template class std::map<const SEGasTransportVertex*, size_t>;
template class std::map<const SEGasTransportVertex*, std::vector<SEGasTransportEdge*>*>;
template class std::map<const SEGasTransportVertex*, std::vector<SEGasTransportEdge*>*>;

SEGasCompartmentGraph::SEGasCompartmentGraph(const char* name, Logger* logger)
  : SEGasCompartmentGraph(std::string { name }, logger)
{
}
//-----------------------------------------------------------------------------
SEGasCompartmentGraph::SEGasCompartmentGraph(const std::string& name, Logger* logger)
  : SECompartmentTransportGraph(name, logger)
{
}
//-----------------------------------------------------------------------------
SEGasCompartmentGraph::~SEGasCompartmentGraph()
{
}
//-----------------------------------------------------------------------------
bool SEGasCompartmentGraph::Load(const CDM::GasCompartmentGraphData& in, SECompartmentManager& cmptMgr)
{
  m_Name = in.Name();
  for (auto name : in.Compartment()) {
    SEGasCompartment* cmpt = cmptMgr.GetGasCompartment(name);
    if (cmpt == nullptr) {
      Error("Could not find compartment " + std::string { name } + " for graph " + m_Name);
      return false;
    }
    AddCompartment(*cmpt);
  }
  for (auto name : in.Link()) {
    SEGasCompartmentLink* link = cmptMgr.GetGasLink(name);
    if (link == nullptr) {
      Error("Could not find link " + std::string { name } + " for graph " + m_Name);
      return false;
    }
    AddLink(*link);
  }
  return true;
}
//-----------------------------------------------------------------------------
CDM::GasCompartmentGraphData* SEGasCompartmentGraph::Unload()
{
  CDM::GasCompartmentGraphData* data = new CDM::GasCompartmentGraphData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEGasCompartmentGraph::Unload(CDM::GasCompartmentGraphData& data)
{
  data.Name(m_Name);
  for (SEGasCompartment* cmpt : m_Compartments)
    data.Compartment().push_back(cmpt->GetName());
  for (SEGasCompartmentLink* link : m_CompartmentLinks)
    data.Link().push_back(link->GetName());
}
//-----------------------------------------------------------------------------
void SEGasCompartmentGraph::BalanceByIntensive()
{
  for (auto cmpt : GetCompartments()) {
    // Make sure Volume Fractions sum to 1.0 and adjust accordingly
    double totalVolumeFraction = 0;
    for (SEGasSubstanceQuantity* subQ : cmpt->GetSubstanceQuantities()) {
      if (subQ->HasVolumeFraction())
        totalVolumeFraction += subQ->GetVolumeFraction().GetValue();
    }
    if (totalVolumeFraction == 0.0) {
      for (SEGasSubstanceQuantity* subQ : cmpt->GetSubstanceQuantities()) {
        if (subQ->HasVolumeFraction())
          subQ->SetToZero();
      }
      continue;
    } else {
      //Adjust to keep the volume fractions making sense
      //Make it a little more sensitive than the error check later just to be safe
      if (std::abs(1.0 - totalVolumeFraction) > (ZERO_APPROX / 10.0)) {
        for (SEGasSubstanceQuantity* subQ : cmpt->GetSubstanceQuantities()) {
          //Adjust everything the same amount to make sure the volume fraction is 1.0
          double volumeFractionErrorFraction = 1.0 / totalVolumeFraction; //<1 = too high; >1 = too low
          subQ->GetVolumeFraction().SetValue(subQ->GetVolumeFraction().GetValue() * volumeFractionErrorFraction);
        }
      }
    }
    cmpt->Balance(BalanceGasBy::VolumeFraction);
  }
}
//-----------------------------------------------------------------------------
void SEGasCompartmentGraph::AddGraph(SEGasCompartmentGraph& graph)
{
  for (SEGasCompartment* cmpt : graph.GetCompartments())
    AddCompartment(*cmpt);
  for (SEGasCompartmentLink* lnk : graph.GetLinks())
    AddLink(*lnk);
}
//-----------------------------------------------------------------------------
}