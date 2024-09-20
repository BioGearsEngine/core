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
#include <biogears/cdm/enums/SEPatientAssessmentEnums.h>

namespace biogears {
class SEScalarAmount;
namespace io {
  class PatientAssessments;
}
class BIOGEARS_API SEUrinalysisMicroscopic : public SEPatientAssessment {
  friend io::PatientAssessments;

public:
  SEUrinalysisMicroscopic();
  virtual ~SEUrinalysisMicroscopic();

  static constexpr const char* TypeTag() { return "SEUrinalysisMicroscopic"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Reset() override;
  virtual void Clear() override;

public:
  virtual bool HasObservationType() const;
  virtual SEMicroscopicObservationType GetObservationType() const;
  virtual void SetObservationType(SEMicroscopicObservationType p);
  virtual void InvalidateObservationType();

  virtual bool HasRedBloodCellsResult() const;
  virtual SEScalarAmount& GetRedBloodCellsResult();

  virtual bool HasWhiteBloodCellsResult() const;
  virtual SEScalarAmount& GetWhiteBloodCellsResult();

  virtual bool HasEpithelialCellsResult() const;
  virtual SEMicroscopicObservationAmount GetEpithelialCellsResult() const;
  virtual void SetEpithelialCellsResult(SEMicroscopicObservationAmount p);
  virtual void InvalidateEpithelialCellsResult();

  virtual bool HasCastsResult() const;
  virtual SEScalarAmount& GetCastsResult();

  virtual bool HasCrystalsResult() const;
  virtual SEMicroscopicObservationAmount GetCrystalsResult() const;
  virtual void SetCrystalsResult(SEMicroscopicObservationAmount p);
  virtual void InvalidateCrystalsResult();

  virtual bool HasBacteriaResult() const;
  virtual SEMicroscopicObservationAmount GetBacteriaResult() const;
  virtual void SetBacteriaResult(SEMicroscopicObservationAmount p);
  virtual void InvalidateBacteriaResult();

  virtual bool HasTrichomonadsResult() const;
  virtual SEMicroscopicObservationAmount GetTrichomonadsResult() const;
  virtual void SetTrichomonadsResult(SEMicroscopicObservationAmount p);
  virtual void InvalidateTrichomonadsResult();

  virtual bool HasYeastResult() const;
  virtual SEMicroscopicObservationAmount GetYeastResult() const;
  virtual void SetYeastResult(SEMicroscopicObservationAmount p);
  virtual void InvalidateYeastResult();

  bool operator==(SEUrinalysisMicroscopic const&) const;
  bool operator!=(SEUrinalysisMicroscopic const&) const;

protected:
  SEMicroscopicObservationType m_ObservationType;
  SEScalarAmount* m_RedBloodCells;
  SEScalarAmount* m_WhiteBloodCells;
  SEMicroscopicObservationAmount m_EpithelialCells;
  SEScalarAmount* m_Casts;
  SEMicroscopicObservationAmount m_Crystals;
  SEMicroscopicObservationAmount m_Bacteria;
  SEMicroscopicObservationAmount m_Trichomonads;
  SEMicroscopicObservationAmount m_Yeast;
};
}