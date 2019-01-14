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
#include <biogears/cdm/compartment/substances/SESubstanceQuantity.h>
#include <biogears/cdm/substance/SESubstanceTransport.h>

namespace biogears {
class SEGasCompartment;

class BIOGEARS_API SEGasSubstanceQuantity : public SESubstanceQuantity, public SEGasTransportSubstance {
  friend class SEGasCompartment;

protected:
  SEGasSubstanceQuantity(SESubstance& sub, SEGasCompartment& compartment);

public:
  virtual ~SEGasSubstanceQuantity();

  virtual void Clear() override;
  virtual void Invalidate() override;

  virtual bool Load(const CDM::GasSubstanceQuantityData& in);
  virtual CDM::GasSubstanceQuantityData* Unload() override;

protected:
  virtual void Unload(CDM::GasSubstanceQuantityData& data);

public:
  virtual void SetToZero();
  virtual const SEScalar* GetScalar(const char* name) override;
  virtual const SEScalar* GetScalar(const std::string& name) override;

  virtual bool HasPartialPressure() const;
  virtual SEScalarPressure& GetPartialPressure();
  virtual double GetPartialPressure(const PressureUnit& unit) const;

  virtual bool HasVolume() const;
  virtual SEScalarVolume& GetVolume();
  virtual double GetVolume(const VolumeUnit& unit) const;

  virtual bool HasVolumeFraction() const;
  virtual SEScalarFraction& GetVolumeFraction();
  virtual double GetVolumeFraction() const;

protected:
  virtual void AddChild(SEGasSubstanceQuantity& subQ);

  virtual bool HasExtensive() const override { return HasVolume(); }
  virtual SEScalarVolume& GetExtensive() override { return GetVolume(); }

  virtual bool HasIntensive() const override { return HasVolumeFraction(); }
  virtual SEScalarFraction& GetIntensive() override { return GetVolumeFraction(); }

  SEScalarPressure* m_PartialPressure;
  SEScalarVolume* m_Volume;
  SEScalarFraction* m_VolumeFraction;

  SEGasCompartment& m_Compartment;
  std::vector<SEGasSubstanceQuantity*> m_Children;
};
}