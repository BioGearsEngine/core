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
class BIOGEARS_API VolumePerPressureUnit : public CCompoundUnit {
public:
  VolumePerPressureUnit(const char* u);
  VolumePerPressureUnit(const std::string& u);

  virtual ~VolumePerPressureUnit();

  bool operator==(const VolumePerPressureUnit&) const;
  bool operator!=(const VolumePerPressureUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const VolumePerPressureUnit& GetCompoundUnit(const char* unit);
  static const VolumePerPressureUnit& GetCompoundUnit(const std::string& unit);

  static const VolumePerPressureUnit L_Per_Pa;
  static const VolumePerPressureUnit L_Per_cmH2O;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<VolumePerPressureUnit>;

class BIOGEARS_API SEScalarVolumePerPressure : public SEScalarQuantity<VolumePerPressureUnit> {
public:
  SEScalarVolumePerPressure();
  virtual ~SEScalarVolumePerPressure();

  CDM::ScalarVolumePerPressureData* Unload() const;

  using SEScalarQuantity<VolumePerPressureUnit>::SetValue;
  using SEScalarQuantity<VolumePerPressureUnit>::GetValue;

  bool operator==(const SEScalarVolumePerPressure&) const;
  bool operator!=(const SEScalarVolumePerPressure&) const;
};
}