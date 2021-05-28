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
#include <biogears/cdm/patient/assessments/SEPatientAssessment.h>
#include <biogears/schema/cdm/PatientAssessments.hxx>

namespace biogears {
class SEPatient;
class SEScalar;
class SEScalarAmountPerVolume;
class SEScalarPressure;

namespace io {
  class PatientAssessments;
}
class BIOGEARS_API SEArterialBloodGasAnalysis : public SEPatientAssessment {
friend io::PatientAssessments;
public:
  SEArterialBloodGasAnalysis();
  virtual ~SEArterialBloodGasAnalysis();

  static constexpr const char* TypeTag() { return "SEArterialBloodGasAnalysis"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Reset() override;
  virtual void Clear() override;

  virtual bool Load(const CDM::ArterialBloodGasAnalysisData& in);
  virtual CDM::ArterialBloodGasAnalysisData* Unload() override;

protected:
  virtual void Unload(CDM::ArterialBloodGasAnalysisData& data);

public:
  virtual bool HaspH();
  virtual SEScalar& GetpH();

  virtual bool HasPartialPressureOxygen();
  virtual SEScalarPressure& GetPartialPressureOxygen();

  virtual bool HasPartialPressureCarbonDioxide();
  virtual SEScalarPressure& GetPartialPressureCarbonDioxide();

  virtual bool HasBaseExcess();
  virtual SEScalarAmountPerVolume& GetBaseExcess();

  virtual bool HasStandardBicarbonate();
  virtual SEScalarAmountPerVolume& GetStandardBicarbonate();

  virtual bool HasOxygenSaturation();
  virtual SEScalar& GetOxygenSaturation();

protected:
  SEScalar* m_pH;
  SEScalarPressure* m_PartialPressureOxygen;
  SEScalarPressure* m_PartialPressureCarbonDioxide;
  SEScalarAmountPerVolume* m_BaseExcess;
  SEScalarAmountPerVolume* m_StandardBicarbonate;
  SEScalar* m_OxygenSaturation;
};
}