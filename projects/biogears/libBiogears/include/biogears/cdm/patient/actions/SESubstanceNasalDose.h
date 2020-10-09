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

#include <biogears/cdm/patient/actions/SESubstanceAdministration.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class SESubstance;

class BIOGEARS_API SESubstanceNasalDose : public SESubstanceAdministration {
public:
  SESubstanceNasalDose(const SESubstance& substance);
  virtual ~SESubstanceNasalDose();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool Load(const CDM::SubstanceNasalDoseData& in);
  virtual CDM::SubstanceNasalDoseData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceNasalDoseData& data) const;

public:
  virtual bool HasDose() const;
  virtual SEScalarMass& GetDose();

  virtual SESubstance& GetSubstance() const;

  virtual void ToString(std::ostream& str) const;

protected:
  SEScalarMass* m_Dose;
  const SESubstance& m_Substance;
};

class BIOGEARS_API SENasalState {
public:
  SENasalState(const SESubstance& sub);
  ~SENasalState();
  virtual void Clear();

  virtual bool Load(const CDM::NasalStateData& in);
  virtual CDM::NasalStateData* Unload() const;

  bool Initialize(SEScalarMass& dose);

  SEScalarMass& GetTotalNasalDose();
  SEScalarMassPerVolume& GetVenaCavaConcentration();
  std::vector<SEScalarMass*>& GetUnreleasedNasalMasses();
  std::vector<SEScalarMass*>& GetReleasedNasalMasses();
  std::vector<double> GetUnreleasedNasalMasses(const MassUnit& unit);
  std::vector<double> GetReleasedNasalMasses(const MassUnit& unit);
  bool SetUnreleasedNasalMasses(std::vector<double>& uMasses, const MassUnit& unit);
  bool SetReleasedNasalMasses(std::vector<double>& rMasses, const MassUnit& unit);

protected:
  virtual void Unload(CDM::NasalStateData& data) const;
  //Nasal specific values
  const SESubstance* m_Substance;
  SEScalarMass* m_TotalNasalDose;
  SEScalarMassPerVolume* m_VenaCavaConcentration;
  std::vector<SEScalarMass*> m_UnreleasedDrugMasses;
  std::vector<SEScalarMass*> m_ReleasedDrugMasses;
  size_t m_NumUnreleasedMasses;
  size_t m_NumReleasedMasses;
};
}