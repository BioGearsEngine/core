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
#include <biogears/schema/ScalarTimeData.hxx>

class BIOGEARS_API TimeUnit : public CCompoundUnit
{
public:
  TimeUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~TimeUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const TimeUnit& GetCompoundUnit(const std::string& unit);

  static const TimeUnit s;
  static const TimeUnit min;
  static const TimeUnit hr;
  static const TimeUnit day;
  static const TimeUnit yr;
};

class BIOGEARS_API SEScalarTime : public SEScalarQuantity<TimeUnit>
{
public:
  SEScalarTime() {}
	virtual ~SEScalarTime() {}

  CDM::ScalarTimeData* Unload() const;
};
