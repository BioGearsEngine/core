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
#include <limits>

#include <biogears/cdm/utils/GeneralMath.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
//-------------------------------------------------------------------------------
template <typename Unit>
const Unit* SEScalarQuantity<Unit>::GetUnit() const
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
SEScalarQuantity<Unit>::SEScalarQuantity(const SEScalarQuantity& obj)
  : SEUnitScalar(obj)
  , m_unit(obj.m_unit)

{
}
//-------------------------------------------------------------------------------
template <typename Unit>
SEScalarQuantity<Unit>::SEScalarQuantity(double d, const Unit& u)
  : SEUnitScalar()
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
  SEProperty::Clear();
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
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::IsValid() const
{
  if (m_unit == nullptr)
    return false;
  return !std::isnan(m_value);
}
//-------------------------------------------------------------------------------
template <typename Unit>
void SEScalarQuantity<Unit>::Load(const CDM::ScalarData& in)
{
  this->Clear();
  SEProperty::Load(in);
  if (in.unit().present()) {
    this->SetValue(in.value(), Unit::GetCompoundUnit(in.unit().get()));
  } else
    throw CommonDataModelException("ScalarQuantity attempted to load a ScalarData with no unit, must have a unit.");
  m_readOnly = in.readOnly();
}
//-------------------------------------------------------------------------------
template <typename Unit>
CDM::ScalarData* SEScalarQuantity<Unit>::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarData* data = new CDM::ScalarData();
  this->Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
template <typename Unit>
void SEScalarQuantity<Unit>::Unload(CDM::ScalarData& data) const
{
  SEProperty::Unload(data);
  data.value(m_value);
  data.unit(m_unit->GetString());
  data.readOnly(m_readOnly);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::Set(const SEScalarQuantity<Unit>& s)
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
void SEScalarQuantity<Unit>::Copy(const SEScalarQuantity<Unit>& s)
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
double SEScalarQuantity<Unit>::GetValue(const Unit& unit) const
{
#if defined(BIOGEARS_THROW_NAN_EXCEPTIONS)
  if (std::isnan(m_value)) {
    throw CommonDataModelException("Value is NaN");
  }
#else
  assert(!std::isnan(m_value));
#endif
  if (std::isinf(m_value))
    return m_value;
  if (m_value == 0)
    return m_value;
  if (m_unit == &unit)
    return m_value;
  return Convert(m_value, *m_unit, unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
double SEScalarQuantity<Unit>::GetValue(const char* unit) const
{
  return GetValue(std::string { unit });
}
//-------------------------------------------------------------------------------
template <typename Unit>
double SEScalarQuantity<Unit>::GetValue(const std::string& unit) const
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
  const Unit* unit_t = GetCompoundUnit(unit);
  if (m_unit == unit_t)
    return m_value;
  return Convert(m_value, *m_unit, *unit_t);
}
//-------------------------------------------------------------------------------
template <typename Unit>
void SEScalarQuantity<Unit>::SetValue(double d, const Unit& unit)
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
bool SEScalarQuantity<Unit>::Equals(const SEScalarQuantity<Unit>& to) const
{
  if (m_unit == nullptr)
    return false;
  if (std::isnan(m_value) && std::isnan(to.m_value)) //This Violates C++ Spec
    return true;
  if (std::isnan(m_value) || std::isnan(to.m_value))
    return false;
  if (std::isinf(m_value) && std::isinf(to.m_value)) //This implies -> -inf == +inf
    return true;
  if (std::isinf(m_value) || std::isinf(to.m_value))
    return false;
  double t = to.GetValue(*m_unit);
  return std::abs(GeneralMath::PercentDifference(m_value, t)) <= 1e-15;
}
//-------------------------------------------------------------------------------
template <typename Unit>
const Unit* SEScalarQuantity<Unit>::GetCompoundUnit(const char* unit) const
{
  return &Unit::GetCompoundUnit(unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
const Unit* SEScalarQuantity<Unit>::GetCompoundUnit(const std::string& unit) const
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
    //return std::to_string(m_value);
  }
  else {
    std::stringstream ss;
    ss << m_value << "(" <<  m_unit->ToString() << ")";
    return ss.str();
   //return std::to_string(m_value) + "(" + m_unit->ToString() + ")" ;
  }
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::Increment(const SEScalarQuantity<Unit>& s) -> SEScalarQuantity&
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
auto SEScalarQuantity<Unit>::IncrementValue(double d, const char* unit) -> SEScalarQuantity&
{
  return IncrementValue(d, std::string { unit });
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::IncrementValue(double d, const std::string& unit) -> SEScalarQuantity&
{
  return IncrementValue(d, *GetCompoundUnit(unit));
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::IncrementValue(double d, const Unit& unit) -> SEScalarQuantity&
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
auto SEScalarQuantity<Unit>::Decrement(const SEScalarQuantity& s) -> SEScalarQuantity&
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
auto SEScalarQuantity<Unit>::DecrementValue(double d, const char* unit) -> SEScalarQuantity&
{
  return DecrementValue(d, std::string { unit });
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::DecrementValue(double d, const std::string& unit) -> SEScalarQuantity&
{
  return DecrementValue(d, *GetCompoundUnit(unit));
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::DecrementValue(double d, const Unit& unit) -> SEScalarQuantity&
{
  return IncrementValue(-d, unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::Multiply(const SEScalar& s) -> SEScalarQuantity&
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
auto SEScalarQuantity<Unit>::Divide(const SEScalar& s) -> SEScalarQuantity&
{
  if (!s.IsValid())
    this->Invalidate();
  else
    this->DivideValue(s.GetValue());
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
bool SEScalarQuantity<Unit>::operator<(const SEScalarQuantity& rhs) const
{
  return m_value < Convert(rhs.m_value, *rhs.m_unit, *m_unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator<=(const SEScalarQuantity& rhs) const
{
  return m_value <= Convert(rhs.m_value, *rhs.m_unit, *m_unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator>(const SEScalarQuantity& rhs) const
{
  return m_value > Convert(rhs.m_value, *rhs.m_unit, *m_unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator>=(const SEScalarQuantity& rhs) const
{
  return m_value >= Convert(rhs.m_value, *rhs.m_unit, *m_unit);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator==(const SEScalarQuantity& rhs) const
{
  return Equals(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
bool SEScalarQuantity<Unit>::operator!=(const SEScalarQuantity& rhs) const
{
  return !Equals(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator+(const SEScalarQuantity& rhs) const -> SEScalarQuantity
{
  SEScalarQuantity result { *this };
  return result.Increment(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator+=(const SEScalarQuantity& rhs) -> SEScalarQuantity&
{
  return this->Increment(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator-(const SEScalarQuantity& rhs) const -> SEScalarQuantity
{
  SEScalarQuantity result { *this };
  return result.Decrement(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator-=(const SEScalarQuantity& rhs) -> SEScalarQuantity&
{
  return this->Decrement(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator/(const SEScalar& rhs) const -> SEScalarQuantity
{
  SEScalarQuantity result { *this };
  return result.Divide(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator/=(const SEScalar& rhs) -> SEScalarQuantity&
{
  return this->Divide(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator*(const SEScalar& rhs) const -> SEScalarQuantity
{
  SEScalarQuantity result { *this };
  return result.Multiply(rhs);
}
//-------------------------------------------------------------------------------
template <typename Unit>
auto SEScalarQuantity<Unit>::operator*=(const SEScalar& rhs) -> SEScalarQuantity&
{
  return this->Multiply(rhs);
}

} //namespace biogears
