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
#include <biogears/cdm/properties/SEScalarQuantity.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
class BIOGEARS_API VolumePerTimeMassUnit : public CCompoundUnit {
public:
  VolumePerTimeMassUnit(const char* u);
  VolumePerTimeMassUnit(const std::string& u);

  virtual ~VolumePerTimeMassUnit();

  bool operator==(const VolumePerTimeMassUnit&) const;
  bool operator!=(const VolumePerTimeMassUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const VolumePerTimeMassUnit& GetCompoundUnit(const char* unit);
  static const VolumePerTimeMassUnit& GetCompoundUnit(const std::string& unit);

  static const VolumePerTimeMassUnit L_Per_s_g;
  static const VolumePerTimeMassUnit mL_Per_s_g;
  static const VolumePerTimeMassUnit mL_Per_min_kg;
  static const VolumePerTimeMassUnit mL_Per_s_kg;
  static const VolumePerTimeMassUnit uL_Per_min_kg;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<VolumePerTimeMassUnit>;

class BIOGEARS_API SEScalarVolumePerTimeMass : public SEScalarQuantity<VolumePerTimeMassUnit> {
public:
  SEScalarVolumePerTimeMass();
  virtual ~SEScalarVolumePerTimeMass();

  CDM::ScalarVolumePerTimeMassData* Unload() const override;

  using SEScalarQuantity<VolumePerTimeMassUnit>::SetValue;
  using SEScalarQuantity<VolumePerTimeMassUnit>::GetValue;

  bool operator==(const SEScalarVolumePerTimeMass&) const;
  bool operator!=(const SEScalarVolumePerTimeMass&) const;
};
}