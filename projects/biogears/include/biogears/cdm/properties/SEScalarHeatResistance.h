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
#include <biogears/schema/ScalarHeatResistanceData.hxx>

class BIOGEARS_API HeatResistanceUnit : public CCompoundUnit
{
public:
  HeatResistanceUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~HeatResistanceUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const HeatResistanceUnit& GetCompoundUnit(const std::string& unit);

  static const HeatResistanceUnit K_Per_W;
  static const HeatResistanceUnit C_Per_W;
  static const HeatResistanceUnit K_s_Per_kcal;
  static const HeatResistanceUnit C_s_Per_kcal;
};

class BIOGEARS_API SEScalarHeatResistance : public SEScalarQuantity<HeatResistanceUnit>
{
public:
  SEScalarHeatResistance() {}
	virtual ~SEScalarHeatResistance() {}

  CDM::ScalarHeatResistanceData* Unload() const;
};