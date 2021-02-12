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
class SEScalarVolume;
class SEScalarVolumePerTime;
class SESubstanceCompound;
namespace io {
  class PatientActions;
}
class BIOGEARS_API SESubstanceCompoundInfusion : public SESubstanceAdministration {
  friend io::PatientActions;

public:
  SESubstanceCompoundInfusion(const SESubstanceCompound& compound);
  virtual ~SESubstanceCompoundInfusion();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool Load(const CDM::SubstanceCompoundInfusionData& in);
  virtual CDM::SubstanceCompoundInfusionData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceCompoundInfusionData& data) const;

public:
  virtual bool HasBagVolume() const;
  virtual SEScalarVolume& GetBagVolume();

  virtual bool HasRate() const;
  virtual SEScalarVolumePerTime& GetRate();

  virtual SESubstanceCompound& GetSubstanceCompound() const;

  virtual void ToString(std::ostream& str) const;

protected:
  SEScalarVolume* m_BagVolume;
  SEScalarVolumePerTime* m_Rate;
  const SESubstanceCompound& m_Compound;
};
}