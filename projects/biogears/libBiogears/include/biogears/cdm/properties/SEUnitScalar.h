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

namespace biogears {
class BIOGEARS_API SEUnitScalar : public SEScalar {
public:
  SEUnitScalar(bool ro = false);
  SEUnitScalar(const SEUnitScalar&);

  ~SEUnitScalar() override;

  std::string ToString() const override = 0;

  virtual const CCompoundUnit* GetUnit() const = 0;
  virtual const CCompoundUnit* GetCompoundUnit(const char* unit) const = 0;
  virtual const CCompoundUnit* GetCompoundUnit(const std::string& unit) const = 0;
  virtual double GetValue() const = 0;
  virtual double GetValue(const char* unit) const = 0;
  virtual double GetValue(const std::string& unit) const = 0;

  virtual SEUnitScalar& IncrementValue(double d, const char* unit) = 0;
  virtual SEUnitScalar& IncrementValue(double d, const std::string& unit) = 0;
  virtual SEUnitScalar& DecrementValue(double d, const char* unit) = 0;
  virtual SEUnitScalar& DecrementValue(double d, const std::string& unit) = 0;

  virtual bool operator==(const SEUnitScalar&) const = 0;
  virtual bool operator!=(const SEUnitScalar&) const = 0;
};
}