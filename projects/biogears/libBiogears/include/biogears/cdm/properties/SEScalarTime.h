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
class BIOGEARS_API TimeUnit : public CCompoundUnit {
public:
  TimeUnit(const char* u);
  TimeUnit(const std::string& u);

  virtual ~TimeUnit();

  bool operator==(const TimeUnit&) const;
  bool operator!=(const TimeUnit&) const;

  static bool IsValidUnit(const char* unit);
  static bool IsValidUnit(const std::string& unit);
  static const TimeUnit& GetCompoundUnit(const char* unit);
  static const TimeUnit& GetCompoundUnit(const std::string& unit);

  static const TimeUnit s;
  static const TimeUnit min;
  static const TimeUnit hr;
  static const TimeUnit day;
  static const TimeUnit yr;
};

BG_EXT template class BIOGEARS_API SEScalarQuantity<TimeUnit>;

class BIOGEARS_API SEScalarTime : public SEScalarQuantity<TimeUnit> {
public:
  SEScalarTime();
  virtual ~SEScalarTime();

  CDM::ScalarTimeData* Unload() const override; 

  using SEScalarQuantity<TimeUnit>::SetValue;
  using SEScalarQuantity<TimeUnit>::GetValue;

  bool operator==(const SEScalarTime&) const;
  bool operator!=(const SEScalarTime&) const;
};
}