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

//----------------------------------------------------------------------------
/// @file PScalar.h
/// @author Chris Volpe
///
/// PScalar is short for Physics Scalar. It is an encapsulation of a
/// double precision floating point value and a CompoundUnit object.
/// It allows all the expected arithmetic operations, as well as the
/// functions pow() for raising to a power, and sqrt() for computing the
/// square root.
//----------------------------------------------------------------------------

#pragma once
#include <biogears/cdm/utils/unitconversion/UnitConversionEngine.h>
#include <biogears/cdm/utils/unitconversion/CompoundUnit.h>

namespace biogears{
class BIOGEARS_API CPScalar {
public:
  // Default ctor
  CPScalar()
    : m_dValue(0.0)
  {
    // Do nothing
  }

  // Construct from value and string unit spec
  CPScalar(double val, const char* unitSpec);
  // Construct from value and string unit spec
  CPScalar(double val, const std::string& unitSpec);

  // Construct from value. No units.
  explicit CPScalar(double val)
    : m_dValue(val)
  {
    // Do nothing
  }

  // Construct from string unit. Initial value is 1.0 so that we can multiply by a pure
// unit specification.
  CPScalar(const char* unitSpec);
  // Construct from string unit. Initial value is 1.0 so that we can multiply by a pure
  // unit specification.
  CPScalar(const std::string& unitSpec);

  // Copy ctor
  CPScalar(const CPScalar& src)
    : m_dValue(src.m_dValue)
    , m_CCU(src.m_CCU)
  {
    // Do nothing
  }

  // Overload arithmetic operators in the proper fashion.
  CPScalar& operator=(const CPScalar& rhs)
  {
    if (this != &rhs) {
      m_dValue = rhs.m_dValue;
      m_CCU = rhs.m_CCU;
    }
    return *this;
  }

  CPScalar& operator*=(const CPScalar& rhs)
  {
    m_dValue *= rhs.m_dValue;
    m_CCU *= rhs.m_CCU;
    return *this;
  }

  CPScalar& operator/=(const CPScalar& rhs)
  {
    m_dValue /= rhs.m_dValue;
    m_CCU /= rhs.m_CCU;
    return *this;
  }

  CPScalar operator*(const CPScalar& rhs) const
  {
    return CPScalar(*this) *= rhs;
  }

  CPScalar operator/(const CPScalar& rhs) const
  {
    return CPScalar(*this) /= rhs;
  }

  // Addition and subtraction require a little care. Adding values requires
  // they be in the same units, so we will convert the right hand side operand
  // to the units of the left hand side. We use the interval conversion function
  // so that we don't get thrown off by biases in the conversion process

  CPScalar& operator+=(const CPScalar& rhs)
  {
    CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
    double newVal = uce.ConvertValueInterval(rhs.m_dValue, rhs.m_CCU, m_CCU);
    m_dValue += newVal;
    return *this;
  }

  CPScalar& operator-=(const CPScalar& rhs)
  {
    CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
    double newVal = uce.ConvertValueInterval(rhs.m_dValue, rhs.m_CCU, m_CCU);
    m_dValue -= newVal;
    return *this;
  }

  // Use ConvertValue here rather than ConvertValueInterval so that we can
  // correctly determine that 0 degC == 32 degF
  bool operator==(const CPScalar& rhs) const
  {
    CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
    double newVal = uce.ConvertValue(rhs.m_dValue, rhs.m_CCU, m_CCU);
    return m_dValue == newVal;
  }

  // We can implement this in terms of overloaded == because in case of type mismatch, NaN is
  // returned by the conversion, which "==" will treat as false, and we want "!=" to
  // yield true in that circumstance
  bool operator!=(const CPScalar& rhs) const
  {
    return !(*this == rhs);
  }

  // Instead of implementing < in terms of !(>=), and > in terms of !(<=) (or vice versa),
  // we provide separate implementations for them all, because we don't want anything
  // to return true in case of NaN (see above), since NAN is always supposed to have an
  // unordered result.
  bool operator<(const CPScalar& rhs) const
  {
    CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
    double newVal = uce.ConvertValue(rhs.m_dValue, rhs.m_CCU, m_CCU);
    return m_dValue < newVal;
  }

  bool operator>(const CPScalar& rhs) const
  {
    CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
    double newVal = uce.ConvertValue(rhs.m_dValue, rhs.m_CCU, m_CCU);
    return m_dValue > newVal;
  }

  bool operator<=(const CPScalar& rhs) const
  {
    CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
    double newVal = uce.ConvertValue(rhs.m_dValue, rhs.m_CCU, m_CCU);
    return m_dValue <= newVal;
  }

  bool operator>=(const CPScalar& rhs) const
  {
    CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
    double newVal = uce.ConvertValue(rhs.m_dValue, rhs.m_CCU, m_CCU);
    return m_dValue >= newVal;
  }

  CPScalar operator+(const CPScalar& rhs) const
  {
    return CPScalar(*this) += rhs;
  }

  CPScalar operator-(const CPScalar& rhs) const
  {
    return CPScalar(*this) -= rhs;
  }

  CPScalar operator+() const // unary +
  {
    return CPScalar(*this); // make a copy anyway!
  }

  CPScalar& Negate(void)
  {
    m_dValue *= -1.0;
    return *this;
  }

  CPScalar operator-() const // unary -
  {
    return CPScalar(*this).Negate();
  }

  // conversion operators. Casting to the designated type invokes these functions.
  operator bool() const
  {
    return m_dValue != 0.0;
  }

  operator int() const
  {
    return static_cast<int>(static_cast<double>(*this));
  }

  operator float() const
  {
    return static_cast<float>(static_cast<double>(*this));
  }

  operator double() const
  {
    double newval;
    CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
    static CCompoundUnit unitless(""); // dimensionless unitless CompoundUnit
    if (*unitless.GetDimension() == *m_CCU.GetDimension()) {
      // We are dimensionless, but we potentially have a scale factor
      // due to quantity-type cancellation, such as knots/mph. So
      // convert to *unitless* to incorporate that bigness difference
      // into the value.

      newval = uce.ConvertValue(m_dValue, m_CCU, unitless);
    } else {
      throw "PScalar value can't be cast to numeric unless already dimensionless!";
    }
    return newval;
  }

  double GetValue() const
  {
    return m_dValue;
  }

  // Function call operator! Overload to take a string unit spec and return a new
  // value equal to the conversion of the current instance to the specified unit.
  // Usage: myPScalar("newUnit").
  // This creates a new CPScalar value and does not change the state of the object
  // upon which it is invoked (unlike the method it calls: ConvertTo)
  CPScalar operator()(const std::string& unitSpec) const
  {
    return CPScalar(*this).ConvertTo(unitSpec);
  }

  // Raise myself to a given power
  // Return reference to self so it can be applied to the temporary created
  // in implementation of pow(). Same thing below for SQRoot() and sqrt()
  CPScalar& Raise(double pwr)
  {
    m_dValue = std::pow(m_dValue, pwr);
    m_CCU.Raise(pwr);
    return *this;
  }

  // Compute square root of myself
  // This method modifies the current object
  CPScalar& SQRoot()
  {
    // Rather than just invoking Raise(0.5) on ourselves, use sqrt(double) on the
    // value so that we can take advantage of intrinsic implementations on the X86
    m_dValue = std::sqrt(m_dValue);
    m_CCU.Raise(0.5);
    return *this;
  }

  // Compute absolute value of myself
  // This method modified the current object
  CPScalar& Abs()
  {
    m_dValue = std::abs(m_dValue);
    return *this;
  }

  // Convert this object to a new set of units
  CPScalar& ConvertTo(const char* unitSpec);
  // Convert this object to a new set of units
  CPScalar& ConvertTo(const std::string& unitSpec);

  // Convert this object to a new set of units
  CPScalar& ConvertTo(const CCompoundUnit& newUnit)
  {
    CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
    double newval = uce.ConvertValue(m_dValue, m_CCU, newUnit);
    m_CCU = newUnit;
    m_dValue = newval;
    return *this;
  }

  // Convert this object to the units of another object
  CPScalar& ConvertTo(const CPScalar& target)
  {
    return ConvertTo(target.m_CCU);
  }

  std::ostream& PrintSelf(std::ostream& output) const
  {
    return output << m_dValue << " " << m_CCU;
  }

  bool IsSameType(const CPScalar& target) const
  {
    return (*m_CCU.GetDimension() == *target.m_CCU.GetDimension());
  }

  bool IsDimensionless() const
  {
    return m_CCU.IsDimensionless();
  }

  // Is this CompoundUnit in "decibel" mode
  bool IsDecible() const
  {
    return m_CCU.IsDecibel();
  }

  template <class T>
  friend CPScalar& operator*=(CPScalar& lhs, const T& rhs);
  template <class T>
  friend CPScalar& operator/=(CPScalar& lhs, const T& rhs);

private:
  double m_dValue;
  CCompoundUnit m_CCU;
};

inline CPScalar pow(const CPScalar& baseref, double pwr)
{
  return (CPScalar(baseref)).Raise(pwr);
}

inline CPScalar pow(const CPScalar& baseref, int pwr)
{
  return (CPScalar(baseref)).Raise(static_cast<double>(pwr));
}

inline CPScalar sqrt(const CPScalar& argref)
{
  return (CPScalar(argref)).SQRoot();
}

inline CPScalar abs(const CPScalar& argref)
{
  return (CPScalar(argref)).Abs();
}

inline std::ostream& operator<<(std::ostream& output, const CPScalar& self)
{
  return self.PrintSelf(output);
}

// Due to the fact that we have casts to double, etc defined, we can't
// rely on the CPScalar(double) constructor to promote non-CPScalar
// operands, since the conversion in the other direction is just as
// valid, according to the compiler, and therefore ambiguous.
// In these templates, "T" can be bool, int, float, double
// Each operator has two signatures, one in which the CPScalar
// value comes first, and one in which it comes second. In each case,
// we construct a CPScalar out of the templated arg and add it to the
// existing CPScalar object using the overloaded *member* form of the
// operator.
// Note that the assignment versions return a reference to the LHS operand

// Multiply
template <class T>
CPScalar operator*(const CPScalar& lhs, const T& rhs)
{
  return lhs * CPScalar(rhs);
}

template <class T>
CPScalar operator*(const T& lhs, const CPScalar& rhs)
{
  return CPScalar(lhs) * rhs;
}

template <class T>
CPScalar& operator*=(CPScalar& lhs, const T& rhs)
{
  lhs.m_dValue *= rhs;
  return lhs;
}

// Divide
template <class T>
CPScalar operator/(const CPScalar& lhs, const T& rhs)
{
  return lhs / CPScalar(rhs);
}

template <class T>
CPScalar operator/(const T& lhs, const CPScalar& rhs)
{
  return CPScalar(lhs) / rhs;
}

template <class T>
CPScalar& operator/=(CPScalar& lhs, const T& rhs)
{
  lhs.m_dValue /= rhs;
  return lhs;
}

// Add
template <class T>
CPScalar operator+(const CPScalar& lhs, const T& rhs)
{
  return lhs + CPScalar(rhs);
}

template <class T>
CPScalar operator+(const T& lhs, const CPScalar& rhs)
{
  return CPScalar(lhs) + rhs;
}

// Subtract
template <class T>
CPScalar operator-(const CPScalar& lhs, const T& rhs)
{
  return lhs - CPScalar(rhs);
}

template <class T>
CPScalar operator-(const T& lhs, const CPScalar& rhs)
{
  return CPScalar(lhs) - rhs;
}

// Less than
template <class T>
bool operator<(const CPScalar& lhs, const T& rhs)
{
  return lhs < CPScalar(rhs);
}

template <class T>
bool operator<(const T& lhs, const CPScalar& rhs)
{
  return CPScalar(lhs) < rhs;
}

// Less than or equal to
template <class T>
bool operator<=(const CPScalar& lhs, const T& rhs)
{
  return lhs <= CPScalar(rhs);
}

template <class T>
bool operator<=(const T& lhs, const CPScalar& rhs)
{
  return CPScalar(lhs) <= rhs;
}

// Greater than
template <class T>
bool operator>(const CPScalar& lhs, const T& rhs)
{
  return lhs > CPScalar(rhs);
}

template <class T>
bool operator>(const T& lhs, const CPScalar& rhs)
{
  return CPScalar(lhs) > rhs;
}

// Greater than or equal to
template <class T>
bool operator>=(const CPScalar& lhs, const T& rhs)
{
  return lhs >= CPScalar(rhs);
}

template <class T>
bool operator>=(const T& lhs, const CPScalar& rhs)
{
  return CPScalar(lhs) >= rhs;
}

// Equal
template <class T>
bool operator==(const CPScalar& lhs, const T& rhs)
{
  return lhs == CPScalar(rhs);
}

template <class T>
bool operator==(const T& lhs, const CPScalar& rhs)
{
  return CPScalar(lhs) == rhs;
}

// Not equal
template <class T>
bool operator!=(const CPScalar& lhs, const T& rhs)
{
  return lhs != CPScalar(rhs);
}

template <class T>
bool operator!=(const T& lhs, const CPScalar& rhs)
{
  return CPScalar(lhs) != rhs;
}
}