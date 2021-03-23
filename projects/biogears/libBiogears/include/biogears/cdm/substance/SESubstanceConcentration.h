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
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/exports.h>

CDM_BIND_DECL(SubstanceConcentrationData)

namespace biogears {
class SESubstance;
class SESubstanceManager;
class SEEnvironmentalConditions;
namespace io {
  class Substance;
}
class BIOGEARS_API SESubstanceConcentration : public Loggable {
  friend io::Substance;

protected:
  friend SEEnvironmentalConditions; // So it can add substances to the manager
public:
  SESubstanceConcentration(SESubstance& substance);
  SESubstanceConcentration(SESubstance& substance, SEScalarMassPerVolume concentration);
  SESubstanceConcentration(SESubstance& substance, double concentration, const MassPerVolumeUnit& unit);
  virtual ~SESubstanceConcentration();

  virtual void Clear();

  virtual bool Load(const CDM::SubstanceConcentrationData& in);
  virtual CDM::SubstanceConcentrationData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceConcentrationData& data) const;

public:
  virtual bool HasConcentration() const;
  virtual SEScalarMassPerVolume& GetConcentration();
  virtual double GetConcentration(const MassPerVolumeUnit& unit) const;

  virtual SESubstance& GetSubstance() const;

  bool operator==(SESubstanceConcentration const& rhs) const;
  bool operator!=(SESubstanceConcentration const& rhs) const;
  SESubstanceConcentration& operator=(SESubstanceConcentration const& rhs);

protected:
  SESubstance* m_Substance;
  SEScalarMassPerVolume m_Concentration;
};
}