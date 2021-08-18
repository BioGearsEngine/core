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
#include <biogears/cdm/properties/SEScalarQuantity.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
class BIOGEARS_API PressureTimePerVolumeAreaUnit : public CCompoundUnit {
public:
  PressureTimePerVolumeAreaUnit(const char* u);
  PressureTimePerVolumeAreaUnit(const std::string& u);

  virtual ~PressureTimePerVolumeAreaUnit();    

  bool operator==(const PressureTimePerVolumeAreaUnit&) const;
  bool operator!=(const PressureTimePerVolumeAreaUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const PressureTimePerVolumeAreaUnit& GetCompoundUnit(const char* unit);
  static const PressureTimePerVolumeAreaUnit& GetCompoundUnit(const std::string& unit);

  static const PressureTimePerVolumeAreaUnit mmHg_min_Per_mL_m2;
  static const PressureTimePerVolumeAreaUnit mmHg_s_Per_mL_m2;
  static const PressureTimePerVolumeAreaUnit dyn_s_Per_cm5_m2;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<PressureTimePerVolumeAreaUnit>;

class BIOGEARS_API SEScalarPressureTimePerVolumeArea : public SEScalarQuantity<PressureTimePerVolumeAreaUnit> {
public:
  SEScalarPressureTimePerVolumeArea();
  virtual ~SEScalarPressureTimePerVolumeArea();

  CDM::ScalarPressureTimePerVolumeAreaData* Unload() const override;

  using SEScalarQuantity<PressureTimePerVolumeAreaUnit>::SetValue;
  using SEScalarQuantity<PressureTimePerVolumeAreaUnit>::GetValue;

  bool operator==(const SEScalarPressureTimePerVolumeArea&) const;
  bool operator!=(const SEScalarPressureTimePerVolumeArea&) const;
};
}