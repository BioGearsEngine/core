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
#include <biogears/cdm/enums/SEPatientAssessmentEnums.h>
#include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>

namespace biogears {

namespace io {
  class PatientAssessments;
}

class BIOGEARS_API SEUrinalysis : public SEPatientAssessment {
  friend io::PatientAssessments;

public:
  SEUrinalysis();
  virtual ~SEUrinalysis();

  static constexpr const char* TypeTag() { return "SEUrinalysis"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Reset() override;
  virtual void Clear() override;

public:
  virtual bool HasColorResult() const;
  virtual SEUrineColor GetColorResult() const;
  virtual void SetColorResult(SEUrineColor color);
  virtual void InvalidateColorResult();

  virtual bool HasAppearanceResult() const;
  virtual SEClarityIndicator GetAppearanceResult() const;
  virtual void SetAppearanceResult(SEClarityIndicator c);
  virtual void InvalidateAppearanceResult();

  virtual bool HasGlucoseResult() const;
  virtual SEPresenceIndicator GetGlucoseResult() const;
  virtual void SetGlucoseResult(SEPresenceIndicator p);
  virtual void InvalidateGlucoseResult();

  virtual bool HasKetoneResult() const;
  virtual SEPresenceIndicator GetKetoneResult() const;
  virtual void SetKetoneResult(SEPresenceIndicator p);
  virtual void InvalidateKetoneResult();

  virtual bool HasBilirubinResult() const;
  virtual SEScalar& GetBilirubinResult();

  virtual bool HasSpecificGravityResult() const;
  virtual SEScalar& GetSpecificGravityResult();

  virtual bool HasBloodResult() const;
  virtual SEPresenceIndicator GetBloodResult() const;
  virtual void SetBloodResult(SEPresenceIndicator p);
  virtual void InvalidateBloodResult();

  virtual bool HasPHResult() const;
  virtual SEScalar& GetPHResult();

  virtual bool HasProteinResult() const;
  virtual SEPresenceIndicator GetProteinResult() const;
  virtual void SetProteinResult(SEPresenceIndicator p);
  virtual void InvalidateProteinResult();

  virtual bool HasUrobilinogenResult() const;
  virtual SEScalarMassPerVolume& GetUrobilinogenResult();

  virtual bool HasNitriteResult() const;
  virtual SEPresenceIndicator GetNitriteResult() const;
  virtual void SetNitriteResult(SEPresenceIndicator p);
  virtual void InvalidateNitriteResult();

  virtual bool HasLeukocyteEsteraseResult() const;
  virtual SEPresenceIndicator GetLeukocyteEsteraseResult() const;
  virtual void SetLeukocyteEsteraseResult(SEPresenceIndicator p);
  virtual void InvalidateLeukocyteEsteraseResult();

  virtual bool HasMicroscopicResult() const;
  virtual SEUrinalysisMicroscopic& GetMicroscopicResult();
  virtual void RemoveMicroscopicResult();

  bool operator==(SEUrinalysis const&) const;
  bool operator!=(SEUrinalysis const&) const;

protected:
  SEUrineColor m_Color;
  SEClarityIndicator m_Appearance;
  SEPresenceIndicator m_Glucose;
  SEPresenceIndicator m_Ketone;
  SEScalar* m_Bilirubin;
  SEScalar* m_SpecificGravity;
  SEPresenceIndicator m_Blood;
  SEScalar* m_pH;
  SEPresenceIndicator m_Protein;
  SEScalarMassPerVolume* m_Urobilinogen;
  SEPresenceIndicator m_Nitrite;
  SEPresenceIndicator m_LeukocyteEsterase;

  SEUrinalysisMicroscopic* m_Microscopic;
};
}