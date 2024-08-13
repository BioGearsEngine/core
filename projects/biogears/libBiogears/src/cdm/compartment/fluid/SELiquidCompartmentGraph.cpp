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
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>

#include "io/cdm/Compartment.h"
#include <biogears/cdm/compartment/SECompartmentGraph.inl>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/SECompartmentTransportGraph.inl>
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.inl>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.inl>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/schema/cdm/Compartment.hxx>


namespace std {
template class vector<biogears::SELiquidTransportVertex*>;
template class vector<biogears::SELiquidTransportEdge*>;
template class map<const biogears::SELiquidTransportVertex*, size_t>;
template class map<const biogears::SELiquidTransportVertex*, vector<biogears::SELiquidTransportEdge*>*>;
}

namespace biogears {
SELiquidCompartmentGraph::SELiquidCompartmentGraph(const char* name, Logger* logger)
  : SELiquidCompartmentGraph(std::string { name }, logger) {};

SELiquidCompartmentGraph::SELiquidCompartmentGraph(const std::string& name, Logger* logger)
  : SECompartmentTransportGraph(name, logger) {};

SELiquidCompartmentGraph::~SELiquidCompartmentGraph() { }

void SELiquidCompartmentGraph::BalanceByIntensive()
{
  for (auto cmpt : GetCompartments())
    cmpt->Balance(BalanceLiquidBy::Concentration);
}

void SELiquidCompartmentGraph::AddGraph(SELiquidCompartmentGraph& graph)
{
  for (SELiquidCompartment* cmpt : graph.GetCompartments())
    AddCompartment(*cmpt);
  for (SELiquidCompartmentLink* lnk : graph.GetLinks())
    AddLink(*lnk);
}
}