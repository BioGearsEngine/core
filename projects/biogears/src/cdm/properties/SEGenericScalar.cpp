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
#include <biogears/cdm/properties/SEGenericScalar.h>

namespace biogears {
	//-------------------------------------------------------------------------------
  SEGenericScalar::SEGenericScalar(Logger * logger)
    : Loggable(logger)
  {
    m_Scalar = nullptr;
    m_UnitScalar = nullptr;
  }

  //-------------------------------------------------------------------------------
  bool SEGenericScalar::HasScalar()
  {
    return m_Scalar != nullptr;
  }
  //-------------------------------------------------------------------------------
  void SEGenericScalar::SetScalar(const SEScalar& s)
  {
    m_Scalar = &s;
    m_UnitScalar = dynamic_cast<const SEUnitScalar*>(m_Scalar);
  }

  //-------------------------------------------------------------------------------
  bool SEGenericScalar::IsValid()
  {
    return m_UnitScalar == nullptr ? m_Scalar->IsValid() : m_UnitScalar->IsValid();
  }

  //-------------------------------------------------------------------------------
  bool SEGenericScalar::IsInfinity()
  {
    return m_Scalar->IsInfinity();
  }

  //-------------------------------------------------------------------------------
  bool SEGenericScalar::HasUnit()
  {
    return m_UnitScalar != nullptr;
  }
  //-------------------------------------------------------------------------------
  const CCompoundUnit* SEGenericScalar::GetUnit()
  {
    if (m_UnitScalar == nullptr)
    {
      return nullptr;
    }
    else { return m_UnitScalar->GetUnit(); };
  }
  //-------------------------------------------------------------------------------
  const CCompoundUnit* SEGenericScalar::GetCompoundUnit(const char* unit) const
  {
    return GetCompoundUnit( std::string{ unit });
  }
  //-------------------------------------------------------------------------------
  const CCompoundUnit* SEGenericScalar::GetCompoundUnit(const std::string& unit) const
  {
    if (m_UnitScalar == nullptr)
      return nullptr;
    return m_UnitScalar->GetCompoundUnit(unit);
  }
  //-------------------------------------------------------------------------------
  bool SEGenericScalar::IsValidUnit(const CCompoundUnit& unit) const
  {
    if (m_UnitScalar == nullptr)
      return false;
    return false;
  }

  //-------------------------------------------------------------------------------
  double SEGenericScalar::GetValue() const
  {
    return (m_Scalar) ? m_Scalar->GetValue() : m_UnitScalar->GetValue();;
  }
  //-------------------------------------------------------------------------------
  double SEGenericScalar::GetValue(const char* unit) const
  {
    if (m_UnitScalar == nullptr)
      return SEScalar::dNaN();
    return m_UnitScalar->GetValue(unit);
  }
  //-------------------------------------------------------------------------------
  double SEGenericScalar::GetValue(const std::string& unit) const
  {
    if (m_UnitScalar == nullptr)
      return SEScalar::dNaN();
    return m_UnitScalar->GetValue(unit);
  }
}