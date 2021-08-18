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

CDM_BIND_DECL(ScalarData)

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
  SEScalar(double);

  virtual ~SEScalar();

  virtual void Clear();
  /**
  Sets the value to NaN and removes the unit
  Note that this does not affect bounds
  */
  virtual void Invalidate();
  virtual void Load(const CDM::ScalarData& in);
  virtual CDM::ScalarData* Unload() const;

  /**
   * Copies ONLY the value and unit
   * ONLY if the provided scalar is valid.
   */
  bool Set(const SEScalar& s);

  /**
   * Copies the entire contents
   * of the provided scalar to this.
   * Even if s is invalid.
   */
  void Copy(const SEScalar& s);

  virtual bool IsValid() const;
  bool IsInfinity() const { return std::isinf(m_value); }

  bool IsPositive() const;
  bool IsNegative() const;
  bool IsZero(double limit = ZERO_APPROX) const;

  void SetReadOnly(bool b);
  bool IsReadOnly() const;

  virtual double GetValue() const;
  virtual void SetValue(double d);

  double Increment(const SEScalar& s);
  double IncrementValue(double d);
  double Decrement(const SEScalar& s);
  double DecrementValue(double d);
  double Multiply(const SEScalar& s);
  double MultiplyValue(double d);
  double Divide(const SEScalar& s);
  double DivideValue(double d);

  bool Equals(const SEScalar& to) const;

  virtual std::string ToString() const;

  static double dNaN();
  static double NaN;
  static bool IsZero(double value, double limit);
  static bool IsValue(double target, double value);

  static const std::string unitless;

  bool operator<(const SEScalar& rhs) const;
  bool operator<=(const SEScalar& rhs) const;
  bool operator>(const SEScalar& rhs) const;
  bool operator>=(const SEScalar& rhs) const;

  bool operator==(const SEScalar& rhs) const { return Equals(rhs); }
  bool operator!=(const SEScalar& rhs) const { return !Equals(rhs); }

  SEScalar operator+(const SEScalar& rhs) const;
  SEScalar& operator+=(const SEScalar& rhs);
  SEScalar operator-(const SEScalar& rhs) const;
  SEScalar& operator-=(const SEScalar& rhs);
  SEScalar operator/(const SEScalar& rhs) const;
  SEScalar& operator/=(const SEScalar& rhs);
  SEScalar operator*(const SEScalar& rhs) const;
  SEScalar& operator*=(const SEScalar& rhs);

protected:
  virtual void Unload(CDM::ScalarData& s) const;
};
//-------------------------------------------------------------------------------
inline SEScalar operator+(double lhs, const SEScalar& rhs) { return SEScalar { lhs }.Increment(rhs); };
inline SEScalar operator-(double lhs, const SEScalar& rhs) { return SEScalar { lhs }.Decrement(rhs); };
inline SEScalar operator/(double lhs, const SEScalar& rhs) { return SEScalar { lhs }.Divide(rhs); };
inline SEScalar operator*(double lhs, const SEScalar& rhs) { return SEScalar { lhs }.Multiply(rhs); };
inline bool operator<(double lhs, const SEScalar& rhs) { return SEScalar { lhs } < rhs; };
inline bool operator<=(double lhs, const SEScalar& rhs) { return SEScalar { lhs } <= rhs; };
inline bool operator>(double lhs, const SEScalar& rhs) { return SEScalar { lhs } > rhs; };
inline bool operator>=(double lhs, const SEScalar& rhs) { return SEScalar { lhs } >= rhs; };
inline bool operator==(double lhs, const SEScalar& rhs) { return rhs == lhs; }
inline bool operator!=(double lhs, const SEScalar& rhs) { return rhs != lhs; }
//-------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& out, const SEScalar* s)
{
  if (s == nullptr)
    out << SEScalar::NaN;
  else
    out << (*s).ToString();
  return out;
}
//-------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& out, const SEScalar& s)
{
  out << s.ToString();
  return out;
}
inline void Override(const SEScalar& from, SEScalar& to)
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
