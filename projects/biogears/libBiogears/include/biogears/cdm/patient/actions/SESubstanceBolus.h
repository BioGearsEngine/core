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
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class SESubstance;

class BIOGEARS_API SESubstanceBolus : public SESubstanceAdministration {
public:
  SESubstanceBolus(const SESubstance& substance);
  virtual ~SESubstanceBolus();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool Load(const CDM::SubstanceBolusData& in);
  virtual CDM::SubstanceBolusData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceBolusData& data) const;

public:
  virtual CDM::enumBolusAdministration::value GetAdminRoute() const;
  virtual void SetAdminRoute(CDM::enumBolusAdministration::value name);
  virtual bool HasAdminRoute() const;
  virtual void InvalidateAdminRoute();

  virtual bool HasAdminTime() const;
  virtual SEScalarTime& GetAdminTime();

  virtual bool HasConcentration() const;
  virtual SEScalarMassPerVolume& GetConcentration();

  virtual bool HasDose() const;
  virtual SEScalarVolume& GetDose();

  virtual SESubstance& GetSubstance() const;

  virtual void ToString(std::ostream& str) const;

protected:
  CDM::enumBolusAdministration::value m_AdminRoute;
  SEScalarTime* m_AdminTime;
  SEScalarMassPerVolume* m_Concentration;
  SEScalarVolume* m_Dose;
  const SESubstance& m_Substance;
};

class BIOGEARS_API SESubstanceBolusState {
public:
  SESubstanceBolusState(const SESubstance& sub);
  ~SESubstanceBolusState();

  virtual bool Load(const CDM::SubstanceBolusStateData& in);
  virtual CDM::SubstanceBolusStateData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceBolusStateData& data) const;

public:
  SEScalarTime& GetElapsedTime() { return m_elapsedTime; }
  SEScalarVolume& GetAdministeredDose() { return m_administeredDose; }

protected:
  const SESubstance& m_substance;
  SEScalarTime m_elapsedTime;
  SEScalarVolume m_administeredDose;
};
}