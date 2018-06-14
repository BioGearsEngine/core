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
#include <biogears/schema/ScalarVolumePerTimeAreaData.hxx>

class BIOGEARS_API VolumePerTimeAreaUnit : public CCompoundUnit {
public:
  VolumePerTimeAreaUnit(const std::string& u)
    : CCompoundUnit(u)
  {
  }
  virtual ~VolumePerTimeAreaUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const VolumePerTimeAreaUnit& GetCompoundUnit(const std::string& unit);

  static const VolumePerTimeAreaUnit mL_Per_min_m2;
  static const VolumePerTimeAreaUnit mL_Per_s_m2;
  static const VolumePerTimeAreaUnit L_Per_min_m2;
};

class BIOGEARS_API SEScalarVolumePerTimeArea : public SEScalarQuantity<VolumePerTimeAreaUnit> {
public:
  SEScalarVolumePerTimeArea() {}
  virtual ~SEScalarVolumePerTimeArea() {}

  CDM::ScalarVolumePerTimeAreaData* Unload() const;
};
