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
#include <biogears/cdm/properties/SEProperty.h>
#include <biogears/cdm/utils/unitconversion/UCCommon.h>

#include <random>


namespace biogears {
class SEGenericScalar;
namespace io {
  class Property;
}

static constexpr double ZERO_APPROX = 1e-10;

class BIOGEARS_API NoUnit {
public:
  NoUnit();
  virtual ~NoUnit();

  static const NoUnit unitless;
};

class BIOGEARS_API SEScalar : public SEProperty {
protected:
  double m_value;
  bool m_readOnly;

public:
  friend io::Property;
  SEScalar();
  SEScalar(SEScalar const&);
  SEScalar(SEScalar&&);
  SEScalar(double v, bool ro=false);

  virtual ~SEScalar();

  virtual void Clear();
  /**
  Sets the value to NaN and removes the unit
  Note that this does not affect bounds
  */
  virtual void Invalidate();

  /**
   * Copies ONLY the value and unit
   * ONLY if the provided scalar is valid.
   */
  bool Set(SEScalar const& s);

  /**
   * Copies the entire contents
   * of the provided scalar to this.
   * Even if s is invalid.
   */
  void Copy(SEScalar const& s);

  virtual bool IsValid() const;
  bool IsInfinity() const { return std::isinf(m_value); }

  bool IsPositive() const;
  bool IsNegative() const;
  bool IsZero(double limit = ZERO_APPROX) const;

  void SetReadOnly(bool b);
  bool IsReadOnly() const;

  virtual double GetValue() const;
  virtual void SetValue(double d);

  double Increment(SEScalar const& s);
  double IncrementValue(double d);
  double Decrement(SEScalar const& s);
  double DecrementValue(double d);
  double Multiply(SEScalar const& s);
  double MultiplyValue(double d);
  double Divide(SEScalar const& s);
  double DivideValue(double d);

  bool Equals(SEScalar const& to) const;

  virtual std::string ToString() const;

  static double dNaN();
  static double NaN;
  static bool IsZero(double value, double limit);
  static bool IsValue(double target, double value);

  static const std::string unitless;

  SEScalar& operator=(SEScalar const& rhs);
  SEScalar& operator=(SEScalar&& rhs);

  bool operator<(SEScalar const& rhs) const;
  bool operator<=(SEScalar const& rhs) const;
  bool operator>(SEScalar const& rhs) const;
  bool operator>=(SEScalar const& rhs) const;

  bool operator==(SEScalar const& rhs) const { return Equals(rhs); }
  bool operator!=(SEScalar const& rhs) const { return !Equals(rhs); }

  SEScalar operator+(SEScalar const& rhs) const;
  SEScalar& operator+=(SEScalar const& rhs);
  SEScalar operator-(SEScalar const& rhs) const;
  SEScalar& operator-=(SEScalar const& rhs);
  SEScalar operator/(SEScalar const& rhs) const;
  SEScalar& operator/=(SEScalar const& rhs);
  SEScalar operator*(SEScalar const& rhs) const;
  SEScalar& operator*=(SEScalar const& rhs);

};
//-------------------------------------------------------------------------------
inline SEScalar operator+(double lhs, SEScalar const& rhs) { return SEScalar { lhs }.Increment(rhs); };
inline SEScalar operator-(double lhs, SEScalar const& rhs) { return SEScalar { lhs }.Decrement(rhs); };
inline SEScalar operator/(double lhs, SEScalar const& rhs) { return SEScalar { lhs }.Divide(rhs); };
inline SEScalar operator*(double lhs, SEScalar const& rhs) { return SEScalar { lhs }.Multiply(rhs); };
inline bool operator<(double lhs, SEScalar const& rhs) { return SEScalar { lhs } < rhs; };
inline bool operator<=(double lhs, SEScalar const& rhs) { return SEScalar { lhs } <= rhs; };
inline bool operator>(double lhs, SEScalar const& rhs) { return SEScalar { lhs } > rhs; };
inline bool operator>=(double lhs, SEScalar const& rhs) { return SEScalar { lhs } >= rhs; };
inline bool operator==(double lhs, SEScalar const& rhs) { return rhs.operator==(lhs); }
inline bool operator!=(double lhs, SEScalar const& rhs) { return rhs.operator!=(lhs); }
//-------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& out, SEScalar const* s)
{
  if (s == nullptr)
    out << SEScalar::NaN;
  else
    out << (*s).ToString();
  return out;
}
//-------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& out, SEScalar const& s)
{
  out << s.ToString();
  return out;
}
inline void Override(SEScalar const& from, SEScalar& to)
{
  bool b = to.IsReadOnly();
  to.SetReadOnly(false);
  to.Set(from);
  to.SetReadOnly(b);
}

//-------------------------------------------------------------------------------
inline void ValueOverride(SEScalar& s, double value)
{
  bool b = s.IsReadOnly();
  s.SetReadOnly(false);
  s.SetValue(value);
  s.SetReadOnly(b);
}
//-------------------------------------------------------------------------------
inline void IncrementOverride(SEScalar& s, double value)
{
  bool b = s.IsReadOnly();
  s.SetReadOnly(false);
  s.IncrementValue(value);
  s.SetReadOnly(b);
}
}
