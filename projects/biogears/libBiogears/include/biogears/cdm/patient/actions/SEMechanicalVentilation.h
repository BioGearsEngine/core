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
#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/cdm/enums/SEPatientActionsEnums.h>
#include <biogears/cdm/enums/SEPropertyEnums.h>


#include <random>

namespace biogears {
class Serializer;
class SESubstance;
class SESubstanceFraction;
class SEMechanicalVentilationConfiguration;
class SEScalarVolumePerTime;
class VolumePerTimeUnit;
class SEScalarPressure;
class PressureUnit;
namespace io {
  class PatientActions;
}
class BIOGEARS_API SEMechanicalVentilation : public SEPatientAction {
  friend io::PatientActions;

protected:
public:
  SEMechanicalVentilation();
  virtual ~SEMechanicalVentilation() override;

  static constexpr const char* TypeTag() { return "SEMechanicalVentilation"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override;

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual SEOnOff GetState() const;
  virtual void SetState(SEOnOff name);
  virtual bool HasState() const;
  virtual void InvalidateState();

  virtual bool HasFlow() const;
  virtual SEScalarVolumePerTime& GetFlow();
  virtual double GetFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasPressure() const;
  virtual SEScalarPressure& GetPressure();
  virtual double GetPressure(const PressureUnit& unit) const;

  bool HasGasFraction() const;
  bool HasGasFraction(const SESubstance& substance) const;
  const std::vector<SESubstanceFraction*>& GetGasFractions();
  const std::vector<const SESubstanceFraction*>& GetGasFractions() const;
  SESubstanceFraction& GetGasFraction(SESubstance& substance);
  const SESubstanceFraction* GetGasFraction(const SESubstance& substance) const;
  void RemoveGasFraction(const SESubstance& substance);
  void RemoveGasFractions();

  virtual void ToString(std::ostream& str) const override;

  bool operator==(const SEMechanicalVentilation& rhs) const;
  bool operator!=(const SEMechanicalVentilation& rhs) const;

protected:
  std::stringstream m_ss;

  SEOnOff m_State;
  SEScalarVolumePerTime* m_Flow;
  SEScalarPressure* m_Pressure;

  std::vector<SESubstanceFraction*> m_GasFractions;
  std::vector<const SESubstanceFraction*> m_cGasFractions;
};
}