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
class BIOGEARS_API TimeMassPerVolumeUnit : public CCompoundUnit {
public:
  TimeMassPerVolumeUnit(const char* u);
  TimeMassPerVolumeUnit(const std::string& u);

  virtual ~TimeMassPerVolumeUnit();   

  bool operator==(const TimeMassPerVolumeUnit&) const;
  bool operator!=(const TimeMassPerVolumeUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const TimeMassPerVolumeUnit& GetCompoundUnit(const char* unit);
  static const TimeMassPerVolumeUnit& GetCompoundUnit(const std::string& unit);

  static const TimeMassPerVolumeUnit s_g_Per_L;
  static const TimeMassPerVolumeUnit min_g_Per_L;
  static const TimeMassPerVolumeUnit hr_g_Per_L;
  static const TimeMassPerVolumeUnit s_ug_Per_mL;
  static const TimeMassPerVolumeUnit min_ug_Per_mL;
  static const TimeMassPerVolumeUnit hr_ug_Per_mL;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<TimeMassPerVolumeUnit>;

class BIOGEARS_API SEScalarTimeMassPerVolume : public SEScalarQuantity<TimeMassPerVolumeUnit> {
public:
  SEScalarTimeMassPerVolume();
  virtual ~SEScalarTimeMassPerVolume();

  CDM::ScalarTimeMassPerVolumeData* Unload() const override;

  using SEScalarQuantity<TimeMassPerVolumeUnit>::SetValue;
  using SEScalarQuantity<TimeMassPerVolumeUnit>::GetValue;

  bool operator==(const SEScalarTimeMassPerVolume&) const;
  bool operator!=(const SEScalarTimeMassPerVolume&) const;
};
}