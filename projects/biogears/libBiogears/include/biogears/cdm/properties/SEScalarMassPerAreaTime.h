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

OPEN_BIOGEARS_NAMESPACE
class BIOGEARS_API MassPerAreaTimeUnit : public CCompoundUnit {
public:
  MassPerAreaTimeUnit(const char* u);
  MassPerAreaTimeUnit(const std::string& u);

  virtual ~MassPerAreaTimeUnit();

  bool operator==(const MassPerAreaTimeUnit&) const;
  bool operator!=(const MassPerAreaTimeUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const MassPerAreaTimeUnit& GetCompoundUnit(const char* unit);
  static const MassPerAreaTimeUnit& GetCompoundUnit(const std::string& unit);

  static const MassPerAreaTimeUnit g_Per_cm2_s;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<MassPerAreaTimeUnit>;

class BIOGEARS_API SEScalarMassPerAreaTime : public SEScalarQuantity<MassPerAreaTimeUnit> {
public:
  SEScalarMassPerAreaTime();
  virtual ~SEScalarMassPerAreaTime();

  CDM::ScalarMassPerAreaTimeData* Unload() const override;

  using SEScalarQuantity<MassPerAreaTimeUnit>::SetValue;
  using SEScalarQuantity<MassPerAreaTimeUnit>::GetValue;

  bool operator==(const SEScalarMassPerAreaTime&) const;
  bool operator!=(const SEScalarMassPerAreaTime&) const;
};
CLOSE_BIOGEARS_NAMESPACE