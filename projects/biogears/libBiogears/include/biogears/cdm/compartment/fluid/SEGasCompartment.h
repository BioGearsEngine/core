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
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentLink.h>
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/substance/SESubstanceTransport.h>

#include <vector>
#include <map>

namespace biogears {
namespace io {
  class Compartment;
}

enum class BalanceGasBy { Volume,
                          VolumeFraction };

class SECompartmentManager;
class BIOGEARS_API SEGasCompartment : public SEFluidCompartment<SEGasCompartmentLink, SEGasTransportVertex, SEGasTransportSubstance, SEGasSubstanceQuantity> {
  friend class SECompartmentManager;
  friend io::Compartment;

protected:
  SEGasCompartment(const char* name, Logger* logger);
  SEGasCompartment(const std::string& name, Logger* logger);

public:
  virtual ~SEGasCompartment();

  virtual bool Load(const CDM::GasCompartmentData& in, SESubstanceManager& subMgr, SECircuitManager* circuits = nullptr);
  virtual CDM::GasCompartmentData* Unload() override;

protected:
  virtual void Unload(CDM::GasCompartmentData& data);

public:
  virtual void StateChange() override;

  virtual void Balance(BalanceGasBy by);

  virtual void AddChild(SEGasCompartment& child);
  virtual const std::vector<SEGasCompartment*>& GetChildren() { return m_Children; }
  virtual const std::vector<SEGasCompartment*>& GetLeaves() { return m_Leaves; }

protected:
  virtual SEGasSubstanceQuantity& CreateSubstanceQuantity(SESubstance& substance);

  std::vector<SEGasCompartment*> m_Children;
  std::vector<SEGasCompartment*> m_Leaves;
};
} //namespace biogears

namespace std {
BG_EXT template class BIOGEARS_API vector<biogears::SEGasCompartment*>;
BG_EXT template class BIOGEARS_API map<string, biogears::SEGasCompartment*>;
}
