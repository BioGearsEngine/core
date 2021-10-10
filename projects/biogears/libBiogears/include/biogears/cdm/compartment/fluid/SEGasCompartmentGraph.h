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
#include "biogears/cdm/CommonDataModel.h"
#include <biogears/cdm/compartment/SECompartmentTransportGraph.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentLink.h>
#include <biogears/schema/cdm/Compartment.hxx>

#include <vector>
#include <map>

CDM_BIND_DECL(GasCompartmentGraphData);


namespace std {
BG_EXT template class BIOGEARS_API vector<biogears::SEGasTransportVertex*>;
BG_EXT template class BIOGEARS_API vector<biogears::SEGasTransportEdge*>;
BG_EXT template class BIOGEARS_API map<const biogears::SEGasTransportVertex*, size_t>;
BG_EXT template class BIOGEARS_API map<const biogears::SEGasTransportVertex*, vector<biogears::SEGasTransportEdge*>*>;
}

namespace biogears {
class SECompartmentManager;
namespace io {
  class Compartment;
}

class BIOGEARS_API SEGasCompartmentGraph : public SECompartmentTransportGraph<SEGasTransportGraph, SEGasTransportVertex, SEGasTransportEdge, SEGasCompartment, SEGasCompartmentLink> {
  friend class SECompartmentManager;
  friend io::Compartment;

protected:
  SEGasCompartmentGraph(const char* name, Logger* logger);
  SEGasCompartmentGraph(const std::string& name, Logger* logger);

public:
  virtual ~SEGasCompartmentGraph();

  void AddGraph(SEGasCompartmentGraph& graph);

  virtual bool Load(const CDM::GasCompartmentGraphData& in, SECompartmentManager& cmptMgr);
  virtual CDM::GasCompartmentGraphData* Unload();

protected:
  virtual void Unload(CDM::GasCompartmentGraphData& data);

protected:
  void BalanceByIntensive() override;
};
}

namespace std {
  BG_EXT template class BIOGEARS_API vector<biogears::SEGasCompartmentGraph*> ;
  BG_EXT template class BIOGEARS_API map<string, biogears::SEGasCompartmentGraph*> ;
}