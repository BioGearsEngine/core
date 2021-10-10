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
class BIOGEARS_API MassPerTimeUnit : public CCompoundUnit {
public:
  MassPerTimeUnit(const char* u);
  MassPerTimeUnit(const std::string& u);

  virtual ~MassPerTimeUnit();   

  bool operator==(const MassPerTimeUnit&) const;
  bool operator!=(const MassPerTimeUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const MassPerTimeUnit& GetCompoundUnit(const char* unit);
  static const MassPerTimeUnit& GetCompoundUnit(const std::string& unit);

  static const MassPerTimeUnit g_Per_s;
  static const MassPerTimeUnit g_Per_min;
  static const MassPerTimeUnit g_Per_day;
  static const MassPerTimeUnit mg_Per_s;
  static const MassPerTimeUnit mg_Per_min;
  static const MassPerTimeUnit ug_Per_min;
  static const MassPerTimeUnit ug_Per_s;
  static const MassPerTimeUnit kg_Per_s;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<MassPerTimeUnit>;

class BIOGEARS_API SEScalarMassPerTime : public SEScalarQuantity<MassPerTimeUnit> {
public:
  SEScalarMassPerTime();
  virtual ~SEScalarMassPerTime();

  CDM::ScalarMassPerTimeData* Unload() const override;

  using SEScalarQuantity<MassPerTimeUnit>::SetValue;
  using SEScalarQuantity<MassPerTimeUnit>::GetValue;

  bool operator==(const SEScalarMassPerTime&) const;
  bool operator!=(const SEScalarMassPerTime&) const;
};
}