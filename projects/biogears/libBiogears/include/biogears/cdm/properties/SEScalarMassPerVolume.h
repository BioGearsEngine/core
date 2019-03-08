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
class BIOGEARS_API MassPerVolumeUnit : public CCompoundUnit {
public:
  MassPerVolumeUnit(const char* u);
  MassPerVolumeUnit(const std::string& u);

  virtual ~MassPerVolumeUnit() = default;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const MassPerVolumeUnit& GetCompoundUnit(const char* unit);
  static const MassPerVolumeUnit& GetCompoundUnit(const std::string& unit);

  static const MassPerVolumeUnit ug_Per_mL;
  static const MassPerVolumeUnit mg_Per_m3;
  static const MassPerVolumeUnit kg_Per_m3;
  static const MassPerVolumeUnit ug_Per_L;
  static const MassPerVolumeUnit g_Per_dL;
  static const MassPerVolumeUnit g_Per_L;
  static const MassPerVolumeUnit g_Per_mL;
  static const MassPerVolumeUnit g_Per_cm3;
  static const MassPerVolumeUnit g_Per_m3;
  static const MassPerVolumeUnit mg_Per_mL;
  static const MassPerVolumeUnit mg_Per_L;
  static const MassPerVolumeUnit mg_Per_dL;
  static const MassPerVolumeUnit kg_Per_mL;
  static const MassPerVolumeUnit kg_Per_L;
};

class BIOGEARS_API SEScalarMassPerVolume : public SEScalarQuantity<MassPerVolumeUnit> {
public:
  SEScalarMassPerVolume() = default;
  virtual ~SEScalarMassPerVolume() = default;

  CDM::ScalarMassPerVolumeData* Unload() const override;
};
}