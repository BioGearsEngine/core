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

#include <limits>

#include <biogears/cdm/properties/SEScalarQuantity.h>
#include <biogears/cdm/utils/GeneralMath.h>

namespace biogears {
//-------------------------------------------------------------------------------
template <typename Unit>
Unit const* SEScalarQuantity<Unit>::GetUnit() const
{
  return m_unit;
}
//-------------------------------------------------------------------------------
template <typename Unit>
SEScalarQuantity<Unit>::SEScalarQuantity()
  : SEUnitScalar()
  , m_unit(nullptr)
{
}
//-------------------------------------------------------------------------------
template <typename Unit>
SEScalarQuantity<Unit>::SEScalarQuantity(SEScalarQuantity const& obj)
  : SEUnitScalar(obj)
  , m_unit(obj.m_unit)

{
}
//-------------------------------------------------------------------------------
template <typename Unit>
SEScalarQuantity<Unit>::SEScalarQuantity(SEScalarQuantity && obj)
  : SEUnitScalar(std::move(obj))
  , m_unit(std::exchange(obj.m_unit, nullptr))
{
}
//-------------------------------------------------------------------------------
template <typename Unit>
SEScalarQuantity<Unit>::SEScalarQuantity(double d, Unit const& u, bool ro)
  : SEUnitScalar(ro)
  , m_unit(nullptr)
{
  SetValue(d, u);
}
//-------------------------------------------------------------------------------
template <typename Unit>
SEScalarQuantity<Unit>::~SEScalarQuantity()
{
  this->Clear();
}
//-------------------------------------------------------------------------------
template <typename Unit>
void SEScalarQuantity<Unit>::Clear()
{
  SEUnitScalar::Clear();
  m_readOnly = false;
  Invalidate();
  m_unit = nullptr;
}
//-------------------------------------------------------------------------------
template <typename Unit>
void SEScalarQuantity<Unit>::Invalidate()
{
  if (m_readOnly) {
#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
    throw CommonDataModelException("Scalar is marked read-only");

#else
    return;
#endif
  }
  m_value = NaN;
  m_unit = nullptr;
}
#pragma optimize("", off)
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::IsValid() const
{
  if (m_unit == nullptr)
    return false;
  return !std::isnan(m_value);
}
#pragma optimize("", on)
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::Set(SEScalarQuantity<Unit> const& s)
{
  if (!s.IsValid()) {
    return false;
  }
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
  m_unit = s.m_unit;
  return true;
}
//-------------------------------------------------------------------------------
template <typename Unit>
void SEScalarQuantity<Unit>::Copy(SEScalarQuantity<Unit> const& s)
{
  if (m_readOnly) {
#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
    throw CommonDataModelException("Scalar is marked read-only");

#else
    return;
#endif
  }
  m_value = s.m_value;
  m_unit = s.m_unit;
}
//-------------------------------------------------------------------------------
template <typename Unit>
double SEScalarQuantity<Unit>::GetValue() const
{
  if (m_unit) {
    return GetValue(*m_unit);
  }
  return SEScalar::dNaN();
}

//-------------------------------------------------------------------------------
template <typename Unit>
double SEScalarQuantity<Unit>::GetValue(Unit const& unit) const
{
#if defined(BIOGEARS_THROW_NAN_EXCEPTIONS)
  if (std::isnan(m_value) && m_unit != nullptr) {
     throw CommonDataModelException("Value is NaN");
  }
#else
  assert(!std::isnan(m_value));
#endif
  if (std::isnan(m_value)
      || std::isinf(m_value)
      || (m_value == 0)
      || (m_unit == &unit))
    return m_value;
  return Convert(m_value, *m_unit, unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
double SEScalarQuantity<Unit>::GetValue(char const* unit) const
{
  return GetValue(std::string { unit });
}
//-------------------------------------------------------------------------------
template <typename Unit>
double SEScalarQuantity<Unit>::GetValue(std::string const& unit) const
{
#if defined(BIOGEARS_THROW_NAN_EXCEPTIONS)
  if (std::isnan(m_value)) {
    throw CommonDataModelException("Value is NaN");
  }
#else
  assert(!std::isnan(m_value));
#endif
  if (std::isinf(m_value)) {
    return m_value;
  }
  if (m_value == 0) {
    return m_value;
  }
  Unit const* unit_t = GetCompoundUnit(unit);
  if (m_unit == unit_t)
    return m_value;
  return Convert(m_value, *m_unit, *unit_t);
}
//-------------------------------------------------------------------------------
template <typename Unit>
void SEScalarQuantity<Unit>::SetValue(double d, Unit const& unit)
{
  if (m_readOnly) {
#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
    throw CommonDataModelException("Scalar is marked read-only");
#else
    return;
#endif
  }
  m_value = d;
  m_unit = &unit;
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::Equals(SEScalarQuantity<Unit> const& rhs) const
{
  if (this == &rhs)
    return true;

  if (this->m_unit && rhs.m_unit) {
    // Nan != Nan is a fundamental part of IEEE1394
    // We should move towards supporting Nan != Nan
    // If we want to support NotValid == NotValid we should express it
    // as !unit == !unit or something, but our real problem is

    if (std::isfinite(m_value) && std::isfinite(rhs.m_value)) {
      double t = rhs.GetValue(*m_unit);
      return std::abs(GeneralMath::PercentDifference(m_value, t)) <= 1e-15;
    }

    if (m_unit == rhs.m_unit) {
      if (  (std::isinf(m_value) && std::isinf(rhs.m_value)) // This implies -> -inf == +inf
          || ((std::isnan(m_value) && std::isnan(rhs.m_value)))) // This Violates C++ Spec
      {
        return true;
      }
    }
  }

  return !this->m_unit && !rhs.m_unit;
}
//-------------------------------------------------------------------------------
template <typename Unit>
Unit const* SEScalarQuantity<Unit>::GetCompoundUnit(char const* unit) const
{
  return &Unit::GetCompoundUnit(unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
Unit const* SEScalarQuantity<Unit>::GetCompoundUnit(std::string const& unit)const
{
  return &Unit::GetCompoundUnit(unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
std::string SEScalarQuantity<Unit>::ToString() const
{
  if (std::isnan(m_value) || std::isinf(m_value)) {
    std::stringstream ss;
    ss << m_value;
    return ss.str();
    // return std::to_string(m_value);
  } else {
    std::stringstream ss;
    ss << m_value << "(" << m_unit->ToString() << ")";
    return ss.str();
    // return std::to_string(m_value) + "(" + m_unit->ToString() + ")" ;
  }
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::Increment(SEScalarQuantity<Unit> const& s) -> SEScalarQuantity&
{
  if (!s.IsValid()) {
    this->Invalidate();
  } else {
    this->IncrementValue(s.m_value, *s.m_unit);
  }
  return *this;
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::IncrementValue(double d, char const* unit) -> SEScalarQuantity&
{
  return IncrementValue(d, std::string { unit });
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::IncrementValue(double d, std::string const& unit) -> SEScalarQuantity&
{
  return IncrementValue(d, *GetCompoundUnit(unit));
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::IncrementValue(double d, Unit const& unit) -> SEScalarQuantity&
{
  if (!IsValid()) {
    this->SetValue(d, unit);
  } else {
    this->SetValue(m_value + Convert(d, unit, *m_unit), *m_unit);
  }
  return *this;
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::Decrement(SEScalarQuantity const& s) -> SEScalarQuantity&
{
  if (!s.IsValid()) {
    this->Invalidate();
  } else {
    this->IncrementValue(-s.m_value, *s.m_unit);
  }
  return *this;
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::DecrementValue(double d, char const* unit) -> SEScalarQuantity&
{
  return DecrementValue(d, std::string { unit });
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::DecrementValue(double d, std::string const& unit) -> SEScalarQuantity&
{
  return DecrementValue(d, *GetCompoundUnit(unit));
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::DecrementValue(double d, Unit const& unit) -> SEScalarQuantity&
{
  return IncrementValue(-d, unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::Multiply(SEScalar const& s) -> SEScalarQuantity&
{
  if (!s.IsValid()) {
    this->Invalidate();
  } else {
    this->MultiplyValue(s.GetValue());
  }
  return *this;
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::MultiplyValue(double d) -> SEScalarQuantity&
{
  if (!IsValid()) {
    this->SetValue(d, *m_unit);
  } else {
    m_value = m_value * d;
  }
  return *this;
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::Divide(SEScalar const& s) -> SEScalarQuantity&
{
  if (!s.IsValid())
    this->Invalidate();
  else
    this->DivideValue(s.GetValue());
  return *this;
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator=(SEScalarQuantity const& rhs) -> SEScalarQuantity&
{
  if (this == &rhs)
    return *this;

  this->SEScalar::operator=(rhs);
  this->m_unit = rhs.m_unit;

  return *this;
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator=(SEScalarQuantity&& rhs) -> SEScalarQuantity&
{
  if (this == &rhs)
    return *this;

  this->SEScalar::operator=(std::move(rhs));
  this->m_unit = std::exchange(rhs.m_unit, nullptr);

  return *this;
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::DivideValue(double d) -> SEScalarQuantity&
{
  if (!IsValid()) {
    this->SetValue(0, *m_unit);
  } else {
    m_value = m_value / d;
  }
  return *this;
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator<(SEScalarQuantity const& rhs) const
{
  return m_value < Convert(rhs.m_value, *rhs.m_unit, *m_unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator<=(SEScalarQuantity const& rhs) const
{
  return m_value <= Convert(rhs.m_value, *rhs.m_unit, *m_unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator>(SEScalarQuantity const& rhs) const
{
  return m_value > Convert(rhs.m_value, *rhs.m_unit, *m_unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator>=(SEScalarQuantity const& rhs) const
{
  return m_value >= Convert(rhs.m_value, *rhs.m_unit, *m_unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator==(SEScalarQuantity const& rhs) const
{
  return Equals(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator!=(SEScalarQuantity const& rhs) const
{
  return !Equals(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator==(SEUnitScalar const& rhs) const
{
  auto rhs_ptr = dynamic_cast<decltype(this)>(&rhs);
  return (rhs_ptr) ? this->operator==(*rhs_ptr) : false;
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator!=(SEUnitScalar const& rhs) const
{
  return !this->operator==(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator+(SEScalarQuantity const& rhs) const -> SEScalarQuantity
{
  SEScalarQuantity result { *this };
  return result.Increment(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator+=(SEScalarQuantity const& rhs) -> SEScalarQuantity&
{
  return this->Increment(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator-(SEScalarQuantity const& rhs) const -> SEScalarQuantity
{
  SEScalarQuantity result { *this };
  return result.Decrement(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator-=(SEScalarQuantity const& rhs) -> SEScalarQuantity&
{
  return this->Decrement(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator/(SEScalar const& rhs) const -> SEScalarQuantity
{
  SEScalarQuantity result { *this };
  return result.Divide(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator/=(SEScalar const& rhs) -> SEScalarQuantity&
{
  return this->Divide(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator*(SEScalar const& rhs) const -> SEScalarQuantity
{
  SEScalarQuantity result { *this };
  return result.Multiply(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator*=(SEScalar const& rhs) -> SEScalarQuantity&
{
  return this->Multiply(rhs);
}

} // namespace biogears
