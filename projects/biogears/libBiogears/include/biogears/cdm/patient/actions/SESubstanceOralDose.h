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
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class SESubstance;
class SEOralTransmucosalState;
class SEOralGastrointestinalState;

class BIOGEARS_API SESubstanceOralDose : public SESubstanceAdministration {
public:
  SESubstanceOralDose(const SESubstance& substance);
  virtual ~SESubstanceOralDose();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool Load(const CDM::SubstanceOralDoseData& in);
  virtual CDM::SubstanceOralDoseData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceOralDoseData& data) const;

public:
  virtual CDM::enumOralAdministration::value GetAdminRoute() const;
  virtual void SetAdminRoute(CDM::enumOralAdministration::value name);
  virtual bool HasAdminRoute() const;

  virtual bool HasDose() const;
  virtual SEScalarMass& GetDose();

  virtual SESubstance& GetSubstance() const;

  virtual void ToString(std::ostream& str) const;

protected:
  CDM::enumOralAdministration::value m_AdminRoute;
  SEScalarMass* m_Dose;
  const SESubstance& m_Substance;
};

class BIOGEARS_API SESubstanceOralState {
public:
  SESubstanceOralState(const SESubstance& sub);
  ~SESubstanceOralState();
  virtual void Clear();

  virtual bool Load(const CDM::SubstanceOralStateData& in);
  virtual CDM::SubstanceOralStateData* Unload() const;

  void Initialize(SEScalarMass& dose, CDM::enumOralAdministration::value route);

  SEOralTransmucosalState& GetTransmucosalSpecificData();
  SEOralGastrointestinalState& GetGastrointestinalSpecificData();
  SEScalarMass& GetStomachDissolvedMass();
  SEScalarMassPerVolume& GetLumenConcentration();
  SEScalarMassPerVolume& GetEnterocyteConcentration();
  SEScalarMassPerVolume& GetVilliConcentration();

protected:
  virtual void Unload(CDM::SubstanceOralStateData& data) const;
  const SESubstance& m_Substance;
  SEOralTransmucosalState* m_TransmucosalState;
  SEOralGastrointestinalState* m_GastrointestinalState;
  //Common pathway for both transmucosal and gastrointestinal routes
  SEScalarMass* m_StomachDissolvedMass;
  SEScalarMassPerVolume* m_LumenConcentration;
  SEScalarMassPerVolume* m_EnterocyteConcentration;
  SEScalarMassPerVolume* m_VilliConcentration;
};

class BIOGEARS_API SEOralTransmucosalState {
public:
  SEOralTransmucosalState();
  ~SEOralTransmucosalState();
  void Clear();
  void Initialize(double dose_ug);

  virtual bool Load(const CDM::OralTransmucosalStateData& in);
  virtual CDM::OralTransmucosalStateData* Unload() const;

  SEScalarMass& GetMouthSolidDrugMass();
  SEScalarMassPerVolume& GetSalivaConcentration();
  SEScalarMassPerVolume& GetEpitheliumConcentration();
  SEScalarMassPerVolume& GetLaminaConcentration();

protected:
  virtual void Unload(CDM::OralTransmucosalStateData& data) const;
  //Transumucosal specific values
  SEScalarMass* m_MouthSolidDrugMass;
  SEScalarMassPerVolume* m_SalivaConcentration;
  SEScalarMassPerVolume* m_EpitheliumConcentration;
  SEScalarMassPerVolume* m_LaminaConcentration;
};

class BIOGEARS_API SEOralGastrointestinalState {
public:
  SEOralGastrointestinalState();
  ~SEOralGastrointestinalState();
  void Clear();
  void Initialize(double dose_ug);

  virtual bool Load(const CDM::OralGastrointestinalStateData& in);
  virtual CDM::OralGastrointestinalStateData* Unload() const;

  SEScalarMass& GetStomachSolidDrugMass();

protected:
  virtual void Unload(CDM::OralGastrointestinalStateData& data) const;
  SEScalarMass* m_StomachSolidDrugMass;
};
}