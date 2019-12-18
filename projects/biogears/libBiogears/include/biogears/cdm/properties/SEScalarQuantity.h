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
#include <biogears/cdm/properties/SEUnitScalar.h>
#include <biogears/cdm/utils/unitconversion/CompoundUnit.h>

CDM_BIND_DECL(ScalarData)

namespace biogears {


template <typename Unit>
class  SEScalarQuantity : public SEUnitScalar {
public:
  SEScalarQuantity();
  SEScalarQuantity(const SEScalarQuantity&);
  explicit SEScalarQuantity(double, const Unit&);
  ~SEScalarQuantity() override;

  void Clear() override;
  void Invalidate() override;
  bool IsValid() const override;

  virtual void Load(const CDM::ScalarData& in) override;
  virtual CDM::ScalarData* Unload() const override;

  virtual bool Set(const SEScalarQuantity<Unit>& s);
  void Copy(const SEScalarQuantity<Unit>& s);

  void SetValue(double d, const Unit& unit);
  virtual double GetValue(const Unit& unit) const;
  virtual double GetValue() const final;

  double GetValue(const char* unit) const override;
  double GetValue(const std::string& unit) const override;

  const Unit* GetUnit() const override;

  SEScalarQuantity& IncrementValue(double d, const char* unit) override;
  SEScalarQuantity& IncrementValue(double d, const std::string& unit) override;
  SEScalarQuantity& IncrementValue(double d, const Unit& unit);
  SEScalarQuantity& Increment(const SEScalarQuantity& s);
  SEScalarQuantity& DecrementValue(double d, const char* unit) override;
  SEScalarQuantity& DecrementValue(double d, const std::string& unit) override;
  SEScalarQuantity& DecrementValue(double d, const Unit& unit);
  SEScalarQuantity& Decrement(const SEScalarQuantity& s);
  SEScalarQuantity& MultiplyValue(double d);
  SEScalarQuantity& Multiply(const SEScalar& s);
  SEScalarQuantity& DivideValue(double d);
  SEScalarQuantity& Divide(const SEScalar& s);

  bool operator<(const SEScalarQuantity& rhs) const;
  bool operator<=(const SEScalarQuantity& rhs) const;
  bool operator>(const SEScalarQuantity& rhs) const;
  bool operator>=(const SEScalarQuantity& rhs) const;

  bool operator==(const SEScalarQuantity& rhs) const;
  bool operator!=(const SEScalarQuantity& rhs) const;

  SEScalarQuantity operator+(const SEScalarQuantity& rhs) const;
  SEScalarQuantity& operator+=(const SEScalarQuantity& rhs);
  SEScalarQuantity operator-(const SEScalarQuantity& rhs) const;
  SEScalarQuantity& operator-=(const SEScalarQuantity& rhs);
  SEScalarQuantity operator/(const SEScalar& rhs) const;
  SEScalarQuantity& operator/=(const SEScalar& rhs);
  SEScalarQuantity operator*(const SEScalar& rhs) const;
  SEScalarQuantity& operator*=(const SEScalar& rhs);

  virtual bool Equals(const SEScalarQuantity<Unit>& to) const;

  virtual void ToString(std::ostream& str) const override;

  const Unit* GetCompoundUnit(const char* unit) const override;
  const Unit* GetCompoundUnit(const std::string& unit) const override;
protected:
  virtual void Unload(CDM::ScalarData& s) const override;


protected:
  const Unit* m_unit;
};

//-------------------------------------------------------------------------------
template <class Unit>
inline void Override(const SEScalarQuantity<Unit>& from, SEScalarQuantity<Unit>& to)
{
  bool b = to.IsReadOnly();
  to.SetReadOnly(false);
  to.Set(from);
  to.SetReadOnly(b);
}
//-------------------------------------------------------------------------------
template <class Unit>
inline void ValueOverride(SEScalarQuantity<Unit>& s, double value, const Unit& unit)
{
  bool b = s.IsReadOnly();
  s.SetReadOnly(false);
  s.SetValue(value, unit);
  s.SetReadOnly(b);
}
//-------------------------------------------------------------------------------
template <class Unit>
inline void IncrementOverride(SEScalarQuantity<Unit>& s, double value, const Unit& unit)
{
  bool b = s.IsReadOnly();
  s.SetReadOnly(false);
  s.IncrementValue(value, unit);
  s.SetReadOnly(b);
}
//-------------------------------------------------------------------------------
}
#include <biogears/cdm/properties/SEScalarQuantity.inl>
