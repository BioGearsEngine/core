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
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.h>
#include <biogears/cdm/patient/assessments/SEPatientAssessment.h>
#include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>
#include <biogears/schema/cdm/PatientAssessments.hxx>

namespace biogears {
class SEPatient;
class SERenalSystem;
class SEAnatomy;

class BIOGEARS_API SEUrinalysis : public SEPatientAssessment {
public:
  SEUrinalysis(Logger* logger);
  virtual ~SEUrinalysis();

  virtual void Reset();
  virtual void Clear();

  virtual bool Load(const CDM::UrinalysisData& in);
  virtual CDM::UrinalysisData* Unload();

protected:
  virtual void Unload(CDM::UrinalysisData& data);

public:
  virtual bool HasColorResult() const;
  virtual CDM::enumUrineColor::value GetColorResult() const;
  virtual void SetColorResult(CDM::enumUrineColor::value color);
  virtual void InvalidateColorResult();

  virtual bool HasAppearanceResult() const;
  virtual CDM::enumClarityIndicator::value GetAppearanceResult() const;
  virtual void SetAppearanceResult(CDM::enumClarityIndicator::value c);
  virtual void InvalidateAppearanceResult();

  virtual bool HasGlucoseResult() const;
  virtual CDM::enumPresenceIndicator::value GetGlucoseResult() const;
  virtual void SetGlucoseResult(CDM::enumPresenceIndicator::value p);
  virtual void InvalidateGlucoseResult();

  virtual bool HasKetoneResult() const;
  virtual CDM::enumPresenceIndicator::value GetKetoneResult() const;
  virtual void SetKetoneResult(CDM::enumPresenceIndicator::value p);
  virtual void InvalidateKetoneResult();

  virtual bool HasBilirubinResult() const;
  virtual SEScalar& GetBilirubinResult();

  virtual bool HasSpecificGravityResult() const;
  virtual SEScalar& GetSpecificGravityResult();

  virtual bool HasBloodResult() const;
  virtual CDM::enumPresenceIndicator::value GetBloodResult() const;
  virtual void SetBloodResult(CDM::enumPresenceIndicator::value p);
  virtual void InvalidateBloodResult();

  virtual bool HasPHResult() const;
  virtual SEScalar& GetPHResult();

  virtual bool HasProteinResult() const;
  virtual CDM::enumPresenceIndicator::value GetProteinResult() const;
  virtual void SetProteinResult(CDM::enumPresenceIndicator::value p);
  virtual void InvalidateProteinResult();

  virtual bool HasUrobilinogenResult() const;
  virtual SEScalarMassPerVolume& GetUrobilinogenResult();

  virtual bool HasNitriteResult() const;
  virtual CDM::enumPresenceIndicator::value GetNitriteResult() const;
  virtual void SetNitriteResult(CDM::enumPresenceIndicator::value p);
  virtual void InvalidateNitriteResult();

  virtual bool HasLeukocyteEsteraseResult() const;
  virtual CDM::enumPresenceIndicator::value GetLeukocyteEsteraseResult() const;
  virtual void SetLeukocyteEsteraseResult(CDM::enumPresenceIndicator::value p);
  virtual void InvalidateLeukocyteEsteraseResult();

  virtual bool HasMicroscopicResult() const;
  virtual SEUrinalysisMicroscopic& GetMicroscopicResult();
  virtual void RemoveMicroscopicResult();

protected:
  CDM::enumUrineColor::value m_Color;
  CDM::enumClarityIndicator::value m_Appearance;
  CDM::enumPresenceIndicator::value m_Glucose;
  CDM::enumPresenceIndicator::value m_Ketone;
  SEScalar* m_Bilirubin;
  SEScalar* m_SpecificGravity;
  CDM::enumPresenceIndicator::value m_Blood;
  SEScalar* m_pH;
  CDM::enumPresenceIndicator::value m_Protein;
  SEScalarMassPerVolume* m_Urobilinogen;
  CDM::enumPresenceIndicator::value m_Nitrite;
  CDM::enumPresenceIndicator::value m_LeukocyteEsterase;

  SEUrinalysisMicroscopic* m_Microscopic;
};
}