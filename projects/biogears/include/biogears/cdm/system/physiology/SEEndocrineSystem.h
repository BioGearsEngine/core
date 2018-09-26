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
#include <biogears/cdm/system/SESystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SEScalarFraction;
class SEScalarMass;
class MassUnit;
class SEScalarTemperature;
class TemperatureUnit;
class SEScalarAmountPerTime;
class AmountPerTimeUnit;
class SEScalarPressure;
class PressureUnit;
class SEScalarMassPerTime;
class MassPerTimeUnit;
class SEScalarPower;
class PowerUnit;

class BIOGEARS_API SEEndocrineSystem : public SESystem {
public:
  SEEndocrineSystem(Logger* logger);
  virtual ~SEEndocrineSystem();

  virtual void Clear(); // Deletes all members

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::EndocrineSystemData& in);
  virtual CDM::EndocrineSystemData* Unload() const;

protected:
  virtual void Unload(CDM::EndocrineSystemData& data) const;

public:
  virtual bool HasInsulinSynthesisRate() const;
  virtual SEScalarAmountPerTime& GetInsulinSynthesisRate();
  virtual double GetInsulinSynthesisRate(const AmountPerTimeUnit& unit) const;

  virtual bool HasGlucagonSynthesisRate() const;
  virtual SEScalarAmountPerTime& GetGlucagonSynthesisRate();
  virtual double GetGlucagonSynthesisRate(const AmountPerTimeUnit& unit) const;

protected:
  SEScalarAmountPerTime* m_InsulinSynthesisRate;
  SEScalarAmountPerTime* m_GlucagonSynthesisRate;
};
}