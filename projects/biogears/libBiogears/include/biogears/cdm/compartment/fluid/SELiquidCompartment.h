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
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentLink.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/substance/SESubstanceTransport.h>


#include <map>
#include <string>
#include <vector>

namespace biogears {
class SETissueCompartment;
class SECompartmentManager;
class SESubstanceManager;
class SELiquidSubstanceQuantity;
namespace io {
  class Compartment;
}


#pragma warning(disable : 4661)
extern template class SEFluidCompartment<SELiquidCompartmentLink, SELiquidTransportVertex, SELiquidTransportSubstance, SELiquidSubstanceQuantity>;
class BIOGEARS_API SELiquidCompartment : public SEFluidCompartment<SELiquidCompartmentLink, SELiquidTransportVertex, SELiquidTransportSubstance, SELiquidSubstanceQuantity> {
  friend class SETissueCompartment;
  friend class SECompartmentManager;
  friend class SELiquidSubstanceQuantity;
  friend io::Compartment;

protected:
  SELiquidCompartment(const char* name, Logger* logger);
  SELiquidCompartment(const std::string& name, Logger* logger);

public:
  virtual ~SELiquidCompartment();

  virtual void Clear() override;

  bool operator==(SELiquidCompartment const&) const;
  bool operator!=(SELiquidCompartment const&) const;
  bool operator==(SEFluidCompartment<SELiquidCompartmentLink, SELiquidTransportVertex, SELiquidTransportSubstance, SELiquidSubstanceQuantity> const&) const override;
  bool operator!=(SEFluidCompartment<SELiquidCompartmentLink, SELiquidTransportVertex, SELiquidTransportSubstance, SELiquidSubstanceQuantity> const&) const override;

public:
  virtual const SEScalar* GetScalar(const char* name) override;
  virtual const SEScalar* GetScalar(const std::string& name) override;

  virtual void StateChange() override;

  virtual void Balance(BalanceLiquidBy by); // Balance all substances based on a specific property

  virtual bool HasPH() const;
  virtual SEScalar& GetPH();
  virtual double GetPH() const;

  virtual bool HasWaterVolumeFraction() const;
  virtual SEScalarFraction& GetWaterVolumeFraction();
  virtual double GetWaterVolumeFraction() const;

  virtual void AddChild(SELiquidCompartment& child);
  virtual const std::vector<SELiquidCompartment*>& GetChildren() { return m_Children; }
  virtual const std::vector<SELiquidCompartment*>& GetLeaves() { return m_Leaves; }

protected:
  virtual SELiquidSubstanceQuantity& CreateSubstanceQuantity(SESubstance& substance);

  SEScalar* m_pH;
  SEScalarFraction* m_WaterVolumeFraction;

  std::vector<SELiquidCompartment*> m_Children;
  std::vector<SELiquidCompartment*> m_Leaves;
};
#pragma warning(default : 4661)
} // namespace biogears
#pragma warning(disable : 4661)

namespace std {
extern template class vector<biogears::SELiquidCompartment*>;
extern template class map<string, biogears::SELiquidCompartment*>;
}
#pragma warning(default : 4661)
