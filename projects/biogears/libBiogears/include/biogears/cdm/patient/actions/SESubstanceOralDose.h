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
class SETransmucosalState;

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

class BIOGEARS_API SETransmucosalState {
public:
  SETransmucosalState(const SESubstance& sub);
  ~SETransmucosalState();
  virtual void Clear();

  virtual bool Load(const CDM::TransmucosalStateData& in);
  virtual CDM::TransmucosalStateData* Unload() const;

  bool Initialize(SEScalarMass& dose);

 
  SEScalarMass& GetMouthSolidMass();
  SEScalarMassPerVolume& GetSalivaConcentration();
  std::vector<SEScalarMassPerVolume*>& GetBuccalConcentrations();
  std::vector<SEScalarMassPerVolume*>& GetSublingualConcentrations();
  std::vector<double> GetBuccalConcentrations(const MassPerVolumeUnit& unit);
  std::vector<double> GetSublingualConcentrations(const MassPerVolumeUnit& unit);
  bool SetBuccalConcentrations(std::vector<double>& bMasses, const MassPerVolumeUnit& unit);
  bool SetSublingualConcentrations(std::vector<double>& slMasses, const MassPerVolumeUnit& unit);

protected:
  virtual void Unload(CDM::TransmucosalStateData& data) const;
  //Transumucosal specific values
  const SESubstance* m_Substance;
  SEScalarMass* m_MouthSolidMass;
  SEScalarMassPerVolume* m_SalivaConcentration;
  std::vector<SEScalarMassPerVolume*> m_BuccalConcentrations;
  std::vector<SEScalarMassPerVolume*> m_SublingualConcentrations;
  size_t m_NumBuccalRegions;
  size_t m_NumSublingualRegions;
};
}