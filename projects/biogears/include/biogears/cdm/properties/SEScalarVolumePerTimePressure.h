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
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/schema/cdm/Properties.hxx>

class BIOGEARS_API VolumePerTimePressureUnit : public CCompoundUnit {
public:
  VolumePerTimePressureUnit(const std::string& u)
    : CCompoundUnit(u)
  {
  }
  virtual ~VolumePerTimePressureUnit() = default;

  static bool IsValidUnit(const std::string& unit);
  static const VolumePerTimePressureUnit& GetCompoundUnit(const std::string& unit);

  static const VolumePerTimePressureUnit L_Per_s_mmHg;
  static const VolumePerTimePressureUnit mL_Per_s_mmHg;
  static const VolumePerTimePressureUnit L_Per_min_mmHg;
  static const VolumePerTimePressureUnit mL_Per_min_mmHg;
};

class BIOGEARS_API SEScalarVolumePerTimePressure : public SEScalarQuantity<VolumePerTimePressureUnit> {
public:
  SEScalarVolumePerTimePressure() = default;
  virtual ~SEScalarVolumePerTimePressure() = default;

  CDM::ScalarVolumePerTimePressureData* Unload() const override;
};
