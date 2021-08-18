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
class BIOGEARS_API VolumePerTimeAreaUnit : public CCompoundUnit {
public:
  VolumePerTimeAreaUnit(const char* u);
  VolumePerTimeAreaUnit(const std::string& u);

  virtual ~VolumePerTimeAreaUnit();

  bool operator==(const VolumePerTimeAreaUnit&) const;
  bool operator!=(const VolumePerTimeAreaUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const VolumePerTimeAreaUnit& GetCompoundUnit(const char* unit);
  static const VolumePerTimeAreaUnit& GetCompoundUnit(const std::string& unit);

  static const VolumePerTimeAreaUnit mL_Per_min_m2;
  static const VolumePerTimeAreaUnit mL_Per_s_m2;
  static const VolumePerTimeAreaUnit L_Per_min_m2;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<VolumePerTimeAreaUnit>;

class BIOGEARS_API SEScalarVolumePerTimeArea : public SEScalarQuantity<VolumePerTimeAreaUnit> {
public:
  SEScalarVolumePerTimeArea();
  virtual ~SEScalarVolumePerTimeArea();

  CDM::ScalarVolumePerTimeAreaData* Unload() const override;

  using SEScalarQuantity<VolumePerTimeAreaUnit>::SetValue;
  using SEScalarQuantity<VolumePerTimeAreaUnit>::GetValue;

  bool operator==(const SEScalarVolumePerTimeArea&) const;
  bool operator!=(const SEScalarVolumePerTimeArea&) const;
};
}