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
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/compartment/SECompartmentTransportGraph.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentLink.h>
#include <biogears/cdm/substance/SESubstanceTransport.h>
#include <biogears/schema/cdm/Compartment.hxx>

#include <vector>
#include <map>
#include <string>


CDM_BIND_DECL(LiquidCompartmentGraphData)

namespace biogears {
class SECompartmentManager;
namespace io {
  class Compartment;
}
} //namespace biogears

namespace std {
BG_EXT template class BIOGEARS_API vector<biogears::SELiquidTransportVertex*>;
BG_EXT template class BIOGEARS_API vector<biogears::SELiquidTransportEdge*>;
BG_EXT template class BIOGEARS_API map<const biogears::SELiquidTransportVertex*, size_t>;
BG_EXT template class BIOGEARS_API map<const biogears::SELiquidTransportVertex*, vector<biogears::SELiquidTransportEdge*>*>;
}

namespace biogears {
class BIOGEARS_API SELiquidCompartmentGraph : public SECompartmentTransportGraph<SELiquidTransportGraph, SELiquidTransportVertex, SELiquidTransportEdge, SELiquidCompartment, SELiquidCompartmentLink> {
  friend class SECompartmentManager;
  friend io::Compartment;

protected:
  SELiquidCompartmentGraph(const char* name, Logger* logger);
  SELiquidCompartmentGraph(const std::string& name, Logger* logger);

public:
  virtual ~SELiquidCompartmentGraph() override;

  void AddGraph(SELiquidCompartmentGraph& graph);

  virtual bool Load(const CDM::LiquidCompartmentGraphData& in, SECompartmentManager& cmptMgr);
  virtual CDM::LiquidCompartmentGraphData* Unload();

protected:
  virtual void Unload(CDM::LiquidCompartmentGraphData& data);

protected:
  void BalanceByIntensive() override;
};
} //namespace biogears

namespace std {
BG_EXT template class BIOGEARS_API vector<biogears::SELiquidCompartmentGraph*>;
BG_EXT template class BIOGEARS_API map<string, biogears::SELiquidCompartmentGraph*>;
}