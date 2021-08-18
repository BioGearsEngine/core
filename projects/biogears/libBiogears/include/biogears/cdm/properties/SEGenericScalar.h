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
#include <biogears/cdm/properties/SEScalarQuantity.h>

#include <ostream>
#include <string>

namespace biogears {
//-------------------------------------------------------------------------------
// I created this class for use in connecting DataRequests to SEScalars for the PhysiologyEngineTrack class
/**
 * @brief If you want to querry what a scalar is and don't know what scalar type you have...
 * @details Be aware, I did not really protect this class, I assume you know what you are doing
 * If you use this class without setting the scalar it will produce nullptr errors and other CDM Exceptions, use with caution and smarts.
 */
class BIOGEARS_API SEGenericScalar : public Loggable {
public:
  SEGenericScalar(Logger* logger);
  virtual ~SEGenericScalar();

  virtual bool HasScalar() const;
  virtual void SetScalar(const SEScalar& s);

  virtual bool IsValid() const;
  virtual bool IsInfinity() const;

  virtual bool HasUnit() const;
  virtual const CCompoundUnit* GetUnit();
  virtual bool IsValidUnit(const CCompoundUnit& unit) const;
  virtual const CCompoundUnit* GetCompoundUnit(const char* unit) const;
  virtual const CCompoundUnit* GetCompoundUnit(const std::string& unit) const;

  virtual double GetValue() const;
  virtual double GetValue(const char* unit) const;
  virtual double GetValue(const std::string& unit) const;

  std::string ToString() const;

  bool operator==(const SEGenericScalar&) const;
  bool operator!=(const SEGenericScalar&) const;

protected:
  const SEScalar* m_Scalar;
  const SEUnitScalar* m_UnitScalar;
};

std::ostream& operator<<(std::ostream& os, SEGenericScalar const& v);
}