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

#include <biogears/cdm/properties/SEScalar.h>

#include <cassert>
#include <cmath>
#include <limits>

#include <biogears/cdm/utils/GeneralMath.h>

namespace biogears {
double SEScalar::NaN = std::numeric_limits<double>::quiet_NaN();

double SEScalar::dNaN()
{
  return NaN;
}

const NoUnit NoUnit::unitless = NoUnit();
const std::string unitless = "unitless";

SEScalar::SEScalar(double v)
  : SEScalar()

{
  SetValue(v);
}
//-------------------------------------------------------------------------------
SEScalar::SEScalar()
  : SEProperty()
  , m_isnan(false)
  , m_isinf(false)
  , m_readOnly(false)
  , m_value(1.0)
{
  Clear();
}

//-------------------------------------------------------------------------------
SEScalar::~SEScalar()
{
  Clear();
}

//-------------------------------------------------------------------------------
void SEScalar::Clear()
{
  SEProperty::Clear();
  m_readOnly = false;
  Invalidate();
}

//-------------------------------------------------------------------------------
void SEScalar::Load(const CDM::ScalarData& in)
{
  Clear();
  SEProperty::Load(in);
  SetValue(in.value());
  if (in.unit().present()) {
    std::string u = in.unit().get();
    if (!("unitless" == u || "" == u || u.empty()))
      throw CommonDataModelException("CDM::Scalar API is intended to be unitless, You are trying to load a ScalarData with a unit defined");
  }
  m_readOnly = in.readOnly();
}

//-------------------------------------------------------------------------------
CDM::ScalarData* SEScalar::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarData* data(new CDM::ScalarData());
  Unload(*data);
  return data;
}

//-------------------------------------------------------------------------------
void SEScalar::Unload(CDM::ScalarData& data) const
{
  SEProperty::Unload(data);
  data.value(m_value);
  data.readOnly(m_readOnly);
}

//-------------------------------------------------------------------------------
bool SEScalar::Set(const SEScalar& s)
{
  if (dynamic_cast<const SEUnitScalar*>(&s) != nullptr)
    std::cerr << " HALT ";
  if (!s.IsValid())
    return false;
#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
  if (m_readOnly) {
    throw CommonDataModelException("Scalar is marked read-only");
  }
#else
  if (m_readOnly) {
    return false;
  }
#endif
  m_value = s.m_value;
  m_isnan = (std::isnan(m_value)) ? true
                                  : false;
  m_isinf = (std::isinf(m_value)) ? true : false;
  return true;
}

//-------------------------------------------------------------------------------
void SEScalar::Copy(const SEScalar& s)
{
#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
  if (m_readOnly) {
    throw CommonDataModelException("Scalar is marked read-only");
  }
#else
  if (m_readOnly) {
    return;
  }
#endif
  m_value = s.m_value;
  m_isnan = s.m_isnan;
  m_isinf = s.m_isinf;
}

//-------------------------------------------------------------------------------
void SEScalar::Invalidate()
{
#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
  if (m_readOnly) {
    throw CommonDataModelException("Scalar is marked read-only");
  }
#else
  if (m_readOnly) {
    return;
  }
#endif
  m_isnan = true;
  m_isinf = false;
  m_value = NaN;
}

//-------------------------------------------------------------------------------
bool SEScalar::IsValid() const
{
  if (m_isnan)
    return false;
  return true;
}

//-------------------------------------------------------------------------------
bool SEScalar::IsZero(double limit) const
{
  if (!IsValid())
    return false;
  return SEScalar::IsZero(m_value, limit);
}

//-------------------------------------------------------------------------------
bool SEScalar::IsPositive() const
{
  if (!IsValid())
    return false;
  return m_value > 0;
}

//-------------------------------------------------------------------------------
bool SEScalar::IsNegative() const
{
  if (!IsValid())
    return false;
  return m_value < 0;
}

//-------------------------------------------------------------------------------
void SEScalar::SetReadOnly(bool b)
{
  m_readOnly = b;
}
//-------------------------------------------------------------------------------
bool SEScalar::IsReadOnly() const
{
  return m_readOnly;
}

//-------------------------------------------------------------------------------
double SEScalar::GetValue() const
{
#if defined(BIOGEARS_THROW_NAN_EXCEPTIONS)
  if (m_isnan) {
    throw CommonDataModelException("Value is NaN");
  }
#else
  assert(!std::isnan(m_value));
#endif
  return m_value;
}

//-------------------------------------------------------------------------------
void SEScalar::SetValue(double d)
{
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  m_value = d;
  m_isnan = (std::isnan(m_value))
    ? true
    : false;
  m_isinf = (std::isinf(m_value)) ? true : false;
}

//-------------------------------------------------------------------------------
double SEScalar::Increment(const SEScalar& s)
{
  if (!s.IsValid())
    Invalidate();
  else
    IncrementValue(s.GetValue());
  return m_value;
}

//-------------------------------------------------------------------------------
double SEScalar::IncrementValue(double d)
{
  if (!IsValid()) {
    SetValue(d);
    return d;
  }
  SetValue(m_value + d);
  return m_value;
}
//-------------------------------------------------------------------------------
double SEScalar::Decrement(const SEScalar& s)
{
  if (!s.IsValid())
    Invalidate();
  else
    IncrementValue(-1.0 * s.GetValue());
  return m_value;
}
//-------------------------------------------------------------------------------
double SEScalar::DecrementValue(double d)
{
  return IncrementValue(-1.0 * d);
}
//-------------------------------------------------------------------------------
double SEScalar::Multiply(const SEScalar& s)
{
  if (!s.IsValid())
    Invalidate();
  else
    MultiplyValue(s.GetValue());
  return m_value;
}
//-------------------------------------------------------------------------------
double SEScalar::MultiplyValue(double d)
{
  if (!IsValid()) {
    SetValue(d);
    return d;
  }
  SetValue(m_value * d);
  return m_value;
}
//-------------------------------------------------------------------------------
double SEScalar::Divide(const SEScalar& s)
{
  if (!s.IsValid())
    Invalidate();
  else
    DivideValue(s.GetValue());
  return m_value;
}
//-------------------------------------------------------------------------------
double SEScalar::DivideValue(double d)
{
  if (!IsValid()) {
    SetValue(1.0/d);
    return 1/d;
  }
  SetValue(m_value / d);
  return m_value;
}
//-------------------------------------------------------------------------------
bool SEScalar::Equals(const SEScalar& to) const
{
  if (m_isnan && to.m_isnan) //This Violates C++ Spec
    return true;
  if (m_isnan || to.m_isnan)
    return false;
  if (m_isinf && to.m_isinf)
    return true;
  if (m_isinf || to.m_isinf)
    return false;
  return std::abs(GeneralMath::PercentDifference(m_value, to.m_value)) <= 1e-15;
}

//-------------------------------------------------------------------------------
void SEScalar::ToString(std::ostream& str) const
{
  str << m_value << std::flush;
}

//-------------------------------------------------------------------------------
bool SEScalar::IsValue(double target, double value)
{
  if (value < (target + 1e-10) && value > (target - 1e-10))
    return true;
  return false;
}

//-------------------------------------------------------------------------------
bool SEScalar::IsZero(double d, double limit)
{
  if (d < limit && d > -limit)
    return true;
  return false;
}

//-------------------------------------------------------------------------------
SEGenericScalar::SEGenericScalar(Logger* logger)
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
    return nullptr;
  return m_UnitScalar->GetUnit();
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
  return m_Scalar->GetValue();
}
//-------------------------------------------------------------------------------
double SEGenericScalar::GetValue(const CCompoundUnit& unit) const
{
  if (m_UnitScalar == nullptr)
    return SEScalar::dNaN();
  return m_UnitScalar->GetValue(unit);
}

//-------------------------------------------------------------------------------
double Convert(double d, const CCompoundUnit& from, const CCompoundUnit& to)
{
  if (&from == &to)
    return d;
  // I am assuming we are not going to do Quantity A to Quantity B Conversions
  return CUnitConversionEngine::GetEngine().QuickConvertValue(d, from, to);
}
//-------------------------------------------------------------------------------
bool CompatibleUnits(const CCompoundUnit& from, const CCompoundUnit& to)
{
  if (from == to)
    return true;
  if (from.GetDimension() == to.GetDimension())
    return true;
  // See if the quantity types (Dimensions) are convertable
  double fromExp;
  CCompoundUnit mappingUnit;
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
  if (uce.GetQuantityConversionParams(from.GetDimension(), to.GetDimension(), fromExp, mappingUnit))
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool SEScalar::operator<(const SEScalar& rhs) const
{
  if (m_isnan || rhs.m_isnan)
    return false;
  return m_value < rhs.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalar::operator<=(const SEScalar& rhs) const
{
  if (m_isnan || rhs.m_isnan)
    return false;
  return m_value <= rhs.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalar::operator>(const SEScalar& rhs) const
{
  if (m_isnan || rhs.m_isnan)
    return false;
  return m_value > rhs.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalar::operator>=(const SEScalar& rhs) const
{
  if (m_isnan || rhs.m_isnan)
    return false;
  return m_value >= rhs.m_value;
}
//-------------------------------------------------------------------------------
SEScalar SEScalar::operator+(const SEScalar& rhs) const
{

  SEScalar result;
  result.Set(*this);
  result.Increment(rhs);
  return result;
}
//-------------------------------------------------------------------------------
SEScalar& SEScalar::operator+=(const SEScalar& rhs)
{
  Increment(rhs);
  return *this;
}
//-------------------------------------------------------------------------------
SEScalar SEScalar::operator-(const SEScalar& rhs) const
{
  SEScalar result;
  result.Set(*this);
  result.Decrement(rhs);
  return result;
}
//-------------------------------------------------------------------------------
SEScalar& SEScalar::operator-=(const SEScalar& rhs)
{
  Decrement(rhs);
  return *this;
}
//-------------------------------------------------------------------------------
SEScalar SEScalar::operator/(const SEScalar& rhs) const
{
  SEScalar result;
  result.Set(*this);
  result.Divide(rhs);
  return result;
}
//-------------------------------------------------------------------------------
SEScalar& SEScalar::operator/=(const SEScalar& rhs)
{
  Divide(rhs);
  return *this;
}
//-------------------------------------------------------------------------------
SEScalar SEScalar::operator*(const SEScalar& rhs) const
{
  SEScalar result;
  result.Set(*this);
  result.Multiply(rhs);
  return result;
}
//-------------------------------------------------------------------------------
SEScalar& SEScalar::operator*=(const SEScalar& rhs)
{
  Multiply(rhs);
  return *this;
}
} // namespace Biogears