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
#include <sstream>

#include <biogears/cdm/utils/GeneralMath.h>

namespace biogears {
double SEScalar::NaN = std::numeric_limits<double>::quiet_NaN();

double SEScalar::dNaN()
{
  return NaN;
}
//-------------------------------------------------------------------------------
const NoUnit NoUnit::unitless = NoUnit();

const std::string unitless = "unitless";

NoUnit::NoUnit()
{
}
//-------------------------------------------------------------------------------
NoUnit::~NoUnit()
{
}
//-------------------------------------------------------------------------------
SEScalar::SEScalar(double v)
  : SEScalar()

{
  SetValue(v);
}
//-------------------------------------------------------------------------------
SEScalar::SEScalar()
  : SEProperty()
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
    if (!("unitless" == u || "" == u || u.empty())) {
      throw CommonDataModelException("CDM::Scalar API is intended to be unitless, You are trying to load a ScalarData with a unit defined");
    }
  }
  m_readOnly = in.readOnly();
}

//-------------------------------------------------------------------------------
CDM::ScalarData* SEScalar::Unload() const
{
  if (!IsValid()) {
    return nullptr;
  }
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
  if (!s.IsValid()) {
    return false;
  }
  if (m_readOnly) {
#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
    throw CommonDataModelException("Scalar is marked read-only");
#else
    return false;
#endif
  }
  m_value = s.m_value;
  return true;
}

//-------------------------------------------------------------------------------
void SEScalar::Copy(const SEScalar& s)
{
  if (m_readOnly) {
#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
    throw CommonDataModelException("Scalar is marked read-only");
#else
    return;
#endif
  }
  m_value = s.m_value;
}

//-------------------------------------------------------------------------------
void SEScalar::Invalidate()
{
  if (m_readOnly) {
#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
    throw CommonDataModelException("Scalar is marked read-only");
#else
    return;
#endif
  }
  m_value = NaN;
}

//-------------------------------------------------------------------------------
bool SEScalar::IsValid() const
{
  return !std::isnan(m_value);
}

//-------------------------------------------------------------------------------
bool SEScalar::IsZero(double limit) const
{
  if (!IsValid()) {
    return false;
  }
  return SEScalar::IsZero(m_value, limit);
}

//-------------------------------------------------------------------------------
bool SEScalar::IsPositive() const
{
  if (!IsValid()) {
    return false;
  }
  return m_value > 0;
}

//-------------------------------------------------------------------------------
bool SEScalar::IsNegative() const
{
  if (!IsValid()) {
    return false;
  }
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
  if (!IsValid()) {
    throw CommonDataModelException("Value is NaN");
  }
#endif
  return m_value;
}

//-------------------------------------------------------------------------------
void SEScalar::SetValue(double d)
{
  if (m_readOnly) {
#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
    throw CommonDataModelException("Scalar is marked read-only");
#else
    return;
#endif
  }
  m_value = d;
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
    SetValue(1.0 / d);
    return 1 / d;
  }
  SetValue(m_value / d);
  return m_value;
}
//-------------------------------------------------------------------------------
bool SEScalar::Equals(const SEScalar& to) const
{
  if (std::isnan(m_value) && std::isnan(to.m_value)) //This Violates C++ Spec
    return true;
  if (std::isnan(m_value) || std::isnan(to.m_value))
    return false;
  if (std::isinf(m_value) && std::isinf(to.m_value)) //This implies -> -inf == +inf
    return true;
  if (std::isinf(m_value) || std::isinf(to.m_value))
    return false;
  return std::abs(GeneralMath::PercentDifference(m_value, to.m_value)) <= 1e-15;
}

//-------------------------------------------------------------------------------
std::string SEScalar::ToString() const
{
  #ifndef ANDROID
  return std::to_string(m_value);
  #else
  std::stringstream ss;
  ss << m_value;
  return ss.str();
  #endif
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
bool SEScalar::operator<(const SEScalar& rhs) const
{
  if (!IsValid() || !rhs.IsValid()) {
    return false;
  }
  return m_value < rhs.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalar::operator<=(const SEScalar& rhs) const
{
  if (!IsValid() || !rhs.IsValid()) {
    return false;
  }
  return m_value <= rhs.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalar::operator>(const SEScalar& rhs) const
{
  if (!IsValid() || !rhs.IsValid()) {
    return false;
  }
  return m_value > rhs.m_value;
}
//-------------------------------------------------------------------------------
bool SEScalar::operator>=(const SEScalar& rhs) const
{
  if (!IsValid() || !rhs.IsValid()) {
    return false;
  }
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