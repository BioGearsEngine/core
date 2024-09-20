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
#include <biogears/exports.h>

#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.h>
#include <biogears/cdm/substance/SESubstanceTransport.h>


#include <vector>
#include <map>
#include <string>

namespace biogears {
class SELiquidCompartment;
class SECompartmentManager;

#pragma warning(disable : 4661)

extern template class SEFluidCompartmentLink<SELiquidTransportEdge, SELiquidTransportVertex, SELiquidCompartment>;

class BIOGEARS_API SELiquidCompartmentLink : public SEFluidCompartmentLink<SELiquidTransportEdge, SELiquidTransportVertex, SELiquidCompartment> {
  friend class SECompartmentManager;

protected:
  SELiquidCompartmentLink(SELiquidCompartment& src, SELiquidCompartment& tgt, const char* name);
  SELiquidCompartmentLink(SELiquidCompartment& src, SELiquidCompartment& tgt, const std::string& name);

public:
  ~SELiquidCompartmentLink() override;

};
#pragma warning(default : 4661)

}    // namespace biogears
#pragma warning(disable : 4661)

namespace std{
extern template class vector<biogears::SELiquidCompartmentLink*>;
extern template class map<string, biogears::SELiquidCompartmentLink*>;
}
#pragma warning(default : 4661)
