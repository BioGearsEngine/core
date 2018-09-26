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
class SEScalarAmount;

class BIOGEARS_API SEUrinalysisMicroscopic : public SEPatientAssessment {
public:
  SEUrinalysisMicroscopic(Logger* logger);
  virtual ~SEUrinalysisMicroscopic();

  virtual void Reset();
  virtual void Clear();

  virtual bool Load(const CDM::UrinalysisMicroscopicData& in);
  virtual CDM::UrinalysisMicroscopicData* Unload();

protected:
  virtual void Unload(CDM::UrinalysisMicroscopicData& data);

public:
  virtual bool HasObservationType() const;
  virtual CDM::enumMicroscopicObservationType::value GetObservationType() const;
  virtual void SetObservationType(CDM::enumMicroscopicObservationType::value p);
  virtual void InvalidateObservationType();

  virtual bool HasRedBloodCellsResult() const;
  virtual SEScalarAmount& GetRedBloodCellsResult();

  virtual bool HasWhiteBloodCellsResult() const;
  virtual SEScalarAmount& GetWhiteBloodCellsResult();

  virtual bool HasEpithelialCellsResult() const;
  virtual CDM::enumMicroscopicObservationAmount::value GetEpithelialCellsResult() const;
  virtual void SetEpithelialCellsResult(CDM::enumMicroscopicObservationAmount::value p);
  virtual void InvalidateEpithelialCellsResult();

  virtual bool HasCastsResult() const;
  virtual SEScalarAmount& GetCastsResult();

  virtual bool HasCrystalsResult() const;
  virtual CDM::enumMicroscopicObservationAmount::value GetCrystalsResult() const;
  virtual void SetCrystalsResult(CDM::enumMicroscopicObservationAmount::value p);
  virtual void InvalidateCrystalsResult();

  virtual bool HasBacteriaResult() const;
  virtual CDM::enumMicroscopicObservationAmount::value GetBacteriaResult() const;
  virtual void SetBacteriaResult(CDM::enumMicroscopicObservationAmount::value p);
  virtual void InvalidateBacteriaResult();

  virtual bool HasTrichomonadsResult() const;
  virtual CDM::enumMicroscopicObservationAmount::value GetTrichomonadsResult() const;
  virtual void SetTrichomonadsResult(CDM::enumMicroscopicObservationAmount::value p);
  virtual void InvalidateTrichomonadsResult();

  virtual bool HasYeastResult() const;
  virtual CDM::enumMicroscopicObservationAmount::value GetYeastResult() const;
  virtual void SetYeastResult(CDM::enumMicroscopicObservationAmount::value p);
  virtual void InvalidateYeastResult();

protected:
  CDM::enumMicroscopicObservationType::value m_ObservationType;
  SEScalarAmount* m_RedBloodCells;
  SEScalarAmount* m_WhiteBloodCells;
  CDM::enumMicroscopicObservationAmount::value m_EpithelialCells;
  SEScalarAmount* m_Casts;
  CDM::enumMicroscopicObservationAmount::value m_Crystals;
  CDM::enumMicroscopicObservationAmount::value m_Bacteria;
  CDM::enumMicroscopicObservationAmount::value m_Trichomonads;
  CDM::enumMicroscopicObservationAmount::value m_Yeast;
};
}