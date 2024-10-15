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


#include <vector>
#include <map>


#pragma warning(disable : 4661)

namespace std {
extern template class vector<biogears::SEGasTransportVertex*>;
extern template class vector<biogears::SEGasTransportEdge*>;
extern template class map<const biogears::SEGasTransportVertex*, size_t>;
extern template class map<const biogears::SEGasTransportVertex*, vector<biogears::SEGasTransportEdge*>*>;
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


protected:
  void BalanceByIntensive() override;
};

}

namespace std {
  extern template class vector<biogears::SEGasCompartmentGraph*> ;
  extern template class map<string, biogears::SEGasCompartmentGraph*> ;
}
#pragma warning(default : 4661)