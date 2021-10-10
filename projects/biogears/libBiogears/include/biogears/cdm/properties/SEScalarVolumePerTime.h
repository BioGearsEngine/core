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
class BIOGEARS_API VolumePerTimeUnit : public CCompoundUnit {
public:
  VolumePerTimeUnit(const char* u);
  VolumePerTimeUnit(const std::string& u);

  virtual ~VolumePerTimeUnit();

  bool operator==(const VolumePerTimeUnit&) const;
  bool operator!=(const VolumePerTimeUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const VolumePerTimeUnit& GetCompoundUnit(const char* unit);
  static const VolumePerTimeUnit& GetCompoundUnit(const std::string& unit);

  static const VolumePerTimeUnit L_Per_s;
  static const VolumePerTimeUnit L_Per_day;
  static const VolumePerTimeUnit mL_Per_s;
  static const VolumePerTimeUnit mL_Per_day;
  static const VolumePerTimeUnit L_Per_min;
  static const VolumePerTimeUnit m3_Per_s;
  static const VolumePerTimeUnit mL_Per_min;
  static const VolumePerTimeUnit mL_Per_hr;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<VolumePerTimeUnit>;

class BIOGEARS_API SEScalarVolumePerTime : public SEScalarQuantity<VolumePerTimeUnit> {
public:
  SEScalarVolumePerTime();
  virtual ~SEScalarVolumePerTime();

  CDM::ScalarVolumePerTimeData* Unload() const override;

  using SEScalarQuantity<VolumePerTimeUnit>::SetValue;
  using SEScalarQuantity<VolumePerTimeUnit>::GetValue;

  bool operator==(const SEScalarVolumePerTime&) const;
  bool operator!=(const SEScalarVolumePerTime&) const;
};
}