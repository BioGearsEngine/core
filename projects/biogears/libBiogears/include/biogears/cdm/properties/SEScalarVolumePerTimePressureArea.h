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
class BIOGEARS_API VolumePerTimePressureAreaUnit : public CCompoundUnit {
public:
  VolumePerTimePressureAreaUnit(const char* u);
  VolumePerTimePressureAreaUnit(const std::string& u);

  virtual ~VolumePerTimePressureAreaUnit();

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const VolumePerTimePressureAreaUnit& GetCompoundUnit(const char* unit);
  static const VolumePerTimePressureAreaUnit& GetCompoundUnit(const std::string& unit);

  static const VolumePerTimePressureAreaUnit mL_Per_min_mmHg_m2;
  static const VolumePerTimePressureAreaUnit mL_Per_s_mmHg_m2;

  bool operator==(const VolumePerTimePressureAreaUnit&) const;
  bool operator!=(const VolumePerTimePressureAreaUnit&) const;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<VolumePerTimePressureAreaUnit>;

class BIOGEARS_API SEScalarVolumePerTimePressureArea : public SEScalarQuantity<VolumePerTimePressureAreaUnit> {
public:
  SEScalarVolumePerTimePressureArea();
  virtual ~SEScalarVolumePerTimePressureArea();

  CDM::ScalarVolumePerTimePressureAreaData* Unload() const override; 

  using SEScalarQuantity<VolumePerTimePressureAreaUnit>::SetValue;
  using SEScalarQuantity<VolumePerTimePressureAreaUnit>::GetValue;

  bool operator==(const SEScalarVolumePerTimePressureArea&) const;
  bool operator!=(const SEScalarVolumePerTimePressureArea&) const;
};
}