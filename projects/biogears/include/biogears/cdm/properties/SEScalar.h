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

#define ZERO_APPROX 1e-10

class BIOGEARS_API NoUnit {
public:
  NoUnit() {}
  virtual ~NoUnit() {}

  static const NoUnit unitless;
};

class BIOGEARS_API SEScalar : public SEProperty {
protected:
  double m_value;

  bool m_isnan;
  bool m_isinf;
  bool m_readOnly;

public:
  SEScalar();
  virtual ~SEScalar();

  virtual void Clear();
  /**
  Sets the value to NaN and removes the unit
  Note that this does not affect bounds
  */
  virtual void Invalidate();

  virtual void Load(const CDM::ScalarData& in);
  virtual CDM::ScalarData* Unload() const;

protected:
  virtual void Unload(CDM::ScalarData& s) const;

public:
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
  bool IsInfinity() const { return m_isinf; }

  bool IsPositive() const;
  bool IsNegative() const;
  bool IsZero(double limit = ZERO_APPROX) const;

  void SetReadOnly(bool b);
  bool IsReadOnly() const;

  double GetValue() const;
  void SetValue(double d);

  double Increment(const SEScalar& s);
  double IncrementValue(double d);

  void Average(int cnt);

  bool Equals(const SEScalar& to) const;

  virtual void ToString(std::ostream& str) const;

  static double dNaN();
  static unsigned long long int NaN;
  static bool IsZero(double value, double limit);
  static bool IsValue(double target, double value);

  static const std::string unitless;
};

inline std::ostream& operator<<(std::ostream& out, const SEScalar* s)
{
  if (s == nullptr)
    out << SEScalar::NaN << std::flush;
  else
    (*s).ToString(out);
  return out;
}
inline std::ostream& operator<<(std::ostream& out, const SEScalar& s)
{
  s.ToString(out);
  return out;
}

/**
 * @brief - An interface to be used for gaining access to a scalar with any unit type
 * @details - This interface allows you to have a pointer to a scalar with units
 *            but you don't need to now what units it's associated with
 */
class BIOGEARS_API SEUnitScalar : public SEScalar {
  friend SEGenericScalar;

public:
  SEUnitScalar()
    : SEScalar()
  {
  }
  virtual ~SEUnitScalar() {}

  virtual bool IsValid() const = 0;
  virtual void Invalidate() = 0;
  virtual const CCompoundUnit* GetUnit() const = 0;

  virtual bool Set(const SEScalar& s) = 0;
  virtual void Copy(const SEScalar& s) = 0;
  virtual double GetValue(const CCompoundUnit& unit) const = 0;
  virtual void SetValue(double d, const CCompoundUnit& unit) = 0;
  virtual double IncrementValue(double d, const CCompoundUnit& unit) = 0;

protected:
  virtual const CCompoundUnit* GetCompoundUnit(const std::string& unit) const = 0;
};

template <typename Unit>
class SEScalarQuantity : public SEUnitScalar {
public:
  SEScalarQuantity();
  virtual ~SEScalarQuantity();

  virtual void Clear();
  virtual void Invalidate();
  virtual bool IsValid() const;

  virtual void Load(const CDM::ScalarData& in);
  virtual CDM::ScalarData* Unload() const;

protected:
  virtual void Unload(CDM::ScalarData& s) const;

  // We need to support the SEUnitScalar interface,  but make these protected
  // If you want access in a generic unit way, us an SEGenericScalar wrapper
  bool Set(const SEScalar& s);
  void Copy(const SEScalar& s);

  virtual double GetValue(const CCompoundUnit& unit) const;
  virtual void SetValue(double d, const CCompoundUnit& unit);
  virtual double IncrementValue(double d, const CCompoundUnit& unit);

  virtual const CCompoundUnit* GetCompoundUnit(const std::string& unit) const;

public:
  virtual bool Set(const SEScalarQuantity<Unit>& s);
  virtual void Copy(const SEScalarQuantity<Unit>& s);

  virtual const Unit* GetUnit() const;

  double GetValue() const = delete; // Must provide a unit
  virtual double GetValue(const Unit& unit) const;

  void SetValue(double d) = delete; // Must provide a unit
  virtual void SetValue(double d, const Unit& unit);

  double IncrementValue(double d) = delete; // Must provide a unit
  virtual double IncrementValue(double d, const Unit& unit);

  double Increment(const SEScalar& s) = delete; // Must provide a unit
  virtual double Increment(const SEScalarQuantity& s);

  bool Equals(const SEScalar& to) const = delete; // Must provide a unit
  virtual bool Equals(const SEScalarQuantity<Unit>& to) const;

  virtual void ToString(std::ostream& str) const;

protected:
  const Unit* m_unit;
};

// I created this class for use in connecting DataRequests to SEScalars for the PhysiologyEngineTrack class
/**
 * @brief If you want to querry what a scalar is and don't know what scalar type you have...
 * @details Be aware, I did not really protect this class, I assume you know what you are doing
 * If you use this class without setting the scalar it will produce nullptr errors and other CDM Exceptions, use with caution and smarts.
 */
class BIOGEARS_API SEGenericScalar : public Loggable {
public:
  SEGenericScalar(Logger* logger);
  virtual ~SEGenericScalar(){};

  virtual bool HasScalar();
  virtual void SetScalar(const SEScalar& s);

  virtual bool IsValid();
  virtual bool IsInfinity();

  virtual bool HasUnit();
  virtual const CCompoundUnit* GetUnit();
  virtual bool IsValidUnit(const CCompoundUnit& unit) const;
  virtual const CCompoundUnit* GetCompoundUnit(const std::string& unit) const;

  virtual double GetValue() const;
  virtual double GetValue(const CCompoundUnit& unit) const;

protected:
  const SEScalar* m_Scalar;
  const SEUnitScalar* m_UnitScalar;
};

BIOGEARS_API double Convert(double d, const CCompoundUnit& from, const CCompoundUnit& to);
BIOGEARS_API bool CompatibleUnits(const CCompoundUnit& u1, const CCompoundUnit& u2);

inline void Override(const SEScalar& from, SEScalar& to)
{
  bool b = to.IsReadOnly();
  to.SetReadOnly(false);
  to.Set(from);
  to.SetReadOnly(b);
}
template <class Unit>
inline void Override(const SEScalarQuantity<Unit>& from, SEScalarQuantity<Unit>& to)
{
  bool b = to.IsReadOnly();
  to.SetReadOnly(false);
  to.Set(from);
  to.SetReadOnly(b);
}

inline void ValueOverride(SEScalar& s, double value)
{
  bool b = s.IsReadOnly();
  s.SetReadOnly(false);
  s.SetValue(value);
  s.SetReadOnly(b);
}
template <class Unit>
inline void ValueOverride(SEScalarQuantity<Unit>& s, double value, const Unit& unit)
{
  bool b = s.IsReadOnly();
  s.SetReadOnly(false);
  s.SetValue(value, unit);
  s.SetReadOnly(b);
}

inline void IncrementOverride(SEScalar& s, double value)
{
  bool b = s.IsReadOnly();
  s.SetReadOnly(false);
  s.IncrementValue(value);
  s.SetReadOnly(b);
}
template <class Unit>
inline void IncrementOverride(SEScalarQuantity<Unit>& s, double value, const Unit& unit)
{
  bool b = s.IsReadOnly();
  s.SetReadOnly(false);
  s.IncrementValue(value, unit);
  s.SetReadOnly(b);
}

#include <biogears/cdm/properties/SEScalar.inl>
