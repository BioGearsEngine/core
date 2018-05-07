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
#include <biogears/schema/ScalarHeatConductanceData.hxx>

class BIOGEARS_API HeatConductanceUnit : public CCompoundUnit
{
public:
  HeatConductanceUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~HeatConductanceUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const HeatConductanceUnit& GetCompoundUnit(const std::string& unit);

  static const HeatConductanceUnit W_Per_K;
  static const HeatConductanceUnit W_Per_C;
  static const HeatConductanceUnit kcal_Per_K_s;
  static const HeatConductanceUnit kcal_Per_C_s;
};

class BIOGEARS_API SEScalarHeatConductance : public SEScalarQuantity<HeatConductanceUnit>
{
public:
  SEScalarHeatConductance() {}
	virtual ~SEScalarHeatConductance() {}

  CDM::ScalarHeatConductanceData* Unload() const;
};