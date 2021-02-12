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
class SERespiratorySystem;
class SEScalarVolume;
class SEScalarVolumePerTime;
class SEFunctionVolumeVsTime;
namespace io {
  class PatientAssessments;
}
class BIOGEARS_API SEPulmonaryFunctionTest : public SEPatientAssessment {
friend io::PatientAssessments;
public:
  SEPulmonaryFunctionTest();
  virtual ~SEPulmonaryFunctionTest();

  static constexpr const char* TypeTag() { return "SEPulmonaryFunctionTest"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Reset();
  virtual void Clear();

  virtual bool Load(const CDM::PulmonaryFunctionTestData& in);
  virtual CDM::PulmonaryFunctionTestData* Unload();

protected:
  virtual void Unload(CDM::PulmonaryFunctionTestData& data);

public:
  virtual int GetNumberOfPlotPoints() { return m_NumberOfPlotPoints; }
  virtual void SetNumberOfPlotPoints(int n) { m_NumberOfPlotPoints = n; }

  virtual bool HasExpiratoryReserveVolume();
  virtual SEScalarVolume& GetExpiratoryReserveVolume();

  virtual bool HasForcedVitalCapacity();
  virtual SEScalarVolume& GetForcedVitalCapacity();

  virtual bool HasForcedExpiratoryVolume();
  virtual SEScalarVolume& GetForcedExpiratoryVolume();

  virtual bool HasForcedExpiratoryFlow();
  virtual SEScalarVolumePerTime& GetForcedExpiratoryFlow();

  virtual bool HasFunctionalResidualCapacity();
  virtual SEScalarVolume& GetFunctionalResidualCapacity();

  virtual bool HasInspiratoryCapacity();
  virtual SEScalarVolume& GetInspiratoryCapacity();

  virtual bool HasInspiratoryReserveVolume();
  virtual SEScalarVolume& GetInspiratoryReserveVolume();

  virtual bool HasMaximumVoluntaryVentilation();
  virtual SEScalarVolume& GetMaximumVoluntaryVentilation();

  virtual bool HasPeakExpiratoryFlow();
  virtual SEScalarVolumePerTime& GetPeakExpiratoryFlow();

  virtual bool HasResidualVolume();
  virtual SEScalarVolume& GetResidualVolume();

  virtual bool HasSlowVitalCapacity();
  virtual SEScalarVolume& GetSlowVitalCapacity();

  virtual bool HasTotalLungCapacity();
  virtual SEScalarVolume& GetTotalLungCapacity();

  virtual bool HasVitalCapacity();
  virtual SEScalarVolume& GetVitalCapacity();

  virtual bool HasLungVolumePlot();
  virtual SEFunctionVolumeVsTime& GetLungVolumePlot();

protected:
  int m_NumberOfPlotPoints;
  SEScalarVolume* m_ExpiratoryReserveVolume;
  SEScalarVolume* m_ForcedVitalCapacity;
  SEScalarVolume* m_ForcedExpiratoryVolume;
  SEScalarVolumePerTime* m_ForcedExpiratoryFlow;
  SEScalarVolume* m_FunctionalResidualCapacity;
  SEScalarVolume* m_InspiratoryCapacity;
  SEScalarVolume* m_InspiratoryReserveVolume;
  SEScalarVolume* m_MaximumVoluntaryVentilation;
  SEScalarVolumePerTime* m_PeakExpiratoryFlow;
  SEScalarVolume* m_ResidualVolume;
  SEScalarVolume* m_SlowVitalCapacity;
  SEScalarVolume* m_TotalLungCapacity;
  SEScalarVolume* m_VitalCapacity;

  SEFunctionVolumeVsTime* m_LungVolumePlot;
};
}