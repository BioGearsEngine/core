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
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class SESubstance;
class SEScalarMassPerVolume;
class SEScalarVolumePerTime;
namespace io {
  class PatientActions;
}
class BIOGEARS_API SESubstanceInfusion : public SESubstanceAdministration {
friend io::PatientActions;
public:
  SESubstanceInfusion(const SESubstance& substance);
  virtual ~SESubstanceInfusion();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool Load(const CDM::SubstanceInfusionData& in);
  virtual CDM::SubstanceInfusionData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceInfusionData& data) const;

public:
  virtual bool HasConcentration() const;
  virtual SEScalarMassPerVolume& GetConcentration();

  virtual bool HasRate() const;
  virtual SEScalarVolumePerTime& GetRate();

  virtual SESubstance& GetSubstance() const;

  virtual void ToString(std::ostream& str) const;

protected:
  SEScalarMassPerVolume* m_Concentration;
  SEScalarVolumePerTime* m_Rate;
  const SESubstance& m_Substance;
};
}