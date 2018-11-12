#ifndef BIOGEARS_MATH_ANGLES_H
#define BIOGEARS_MATH_ANGLES_H

//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.apache.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------

//! \file
//! \author Alex Pronschinske
//! \date 2017 Mar 20
//!
//! Mathematical utility code
//!

#include <array>
#include <cmath>
#include <utility>


namespace biogears {

inline namespace angle_literals {
// constants
//----------
constexpr long double PI_l = 3.1415926535897932384626433832795028l;
constexpr long double TWO_PI_l = 2.0l * PI_l;
constexpr long double HALF_PI_l = 0.5l * PI_l;
constexpr long double QUARTER_PI_l = 0.25l * PI_l;

constexpr double PI = 3.14159265358979323;
constexpr double TWO_PI = 2.0 * PI;
constexpr double HALF_PI = 0.5 * PI;
constexpr double QUARTER_PI = 0.25 * PI;

constexpr float PI_f = 3.1415926535f;
constexpr float TWO_PI_f = 2.0f * PI_f;
constexpr float HALF_PI_f = 0.5f * PI_f;
constexpr float QUARTER_PI_f = 0.25f * PI_f;

// literal suffixes for angular units
// *** these all perform convertions from the literal unit to radians ***
//-----------------------------------------------------------------------
inline constexpr long double operator""_l_deg(unsigned long long deg){ return static_cast<long double>(deg) * PI_l / 180.0l; }
inline constexpr double operator""_deg(unsigned long long deg) { return static_cast<double>(deg) * PI / 180.0; }
inline constexpr float operator""_f_deg(unsigned long long deg) { return static_cast<float>(deg) * PI_f / 180.0f; }
inline constexpr long double operator""_l_mil(unsigned long long mil) { return static_cast<long double>(mil) * PI_l / 3200.0l; }
inline constexpr double operator""_mil(unsigned long long mil) { return static_cast<double>(mil) * PI / 3200.0; }
inline constexpr float operator""_f_mil(unsigned long long mil) { return static_cast<float>(mil) * PI_f / 3200.0f; }
inline constexpr long double operator""_l_mrad(unsigned long long mrad) { return static_cast<long double>(mrad) / 1000.0l; }
inline constexpr double operator""_mrad(unsigned long long mrad) { return static_cast<double>(mrad) / 1000.0; }
inline constexpr float operator""_f_mrad(unsigned long long mrad) { return static_cast<float>(mrad) / 1000.0f; }
inline constexpr long double operator""_l_rad(unsigned long long rad) { return static_cast<long double>(rad); }
inline constexpr double operator""_rad(unsigned long long rad) { return static_cast<double>(rad); }
inline constexpr float operator""_f_rad(unsigned long long rad) { return static_cast<float>(rad); }

inline constexpr long double operator""_l_deg(long double deg) { return deg * PI_l / 180.0l; }
inline constexpr double operator""_deg(long double deg) { return static_cast<double>(deg) * PI / 180.0; }
inline constexpr float operator""_f_deg(long double deg) { return static_cast<float>(deg) * PI_f / 180.0f; }
inline constexpr long double operator""_l_mil(long double mil) { return mil * PI_l / 3200.0l; }
inline constexpr double operator""_mil(long double mil) { return static_cast<double>(mil) * PI / 3200.0; }
inline constexpr float operator""_f_mil(long double mil) { return static_cast<float>(mil) * PI_f / 3200.0f; }
inline constexpr long double operator""_l_mrad(long double mrad) { return mrad / 1000.0l; }
inline constexpr double operator""_mrad(long double mrad) { return static_cast<double>(mrad) / 1000.0; }
inline constexpr float operator""_f_mrad(long double mrad) { return static_cast<float>(mrad) / 1000.0f; }
inline constexpr long double operator""_l_rad(long double rad) { return rad; }
inline constexpr double operator""_rad(long double rad) { return static_cast<double>(rad); }
inline constexpr float operator""_f_rad(long double rad) { return static_cast<float>(rad); }
} //inline angle_literals


// constexpr versions of typical mathematical functions
//------------------------------------------------------------------------------
constexpr double const_sign(double x) { return (0. <= x) ? 1.0 : -1.0; }
constexpr double const_abs(double x) { return (0. <= x) ? x : -x; }
constexpr float const_sign(float x) { return (0.f <= x) ? 1.0f : -1.0f; }
constexpr float const_abs(float x) { return (0.f <= x) ? x : -x; }
//!
//! Composes a floating point value with the magnitude of x and the sign of y.
//!
constexpr double const_copysign(double x, double y) {
  return const_sign(y) * const_abs(x);
}
constexpr float const_copysign(float x, float y) {
  return const_sign(y) * const_abs(x);
}

//------------------------------------------------------------------------------
constexpr double radians_from(double x) { return x * PI / 180.0; }
constexpr float radians_from(float x) { return x * PI_f / 180.0f; }
constexpr double degrees_from(double x) { return x * 180.0 / PI; }
constexpr float degrees_from(float x) { return x * 180.0f / PI_f; }

// angle units utilities
//----------------------
enum class AngleUnit { RAD, DEG, MIL, MRAD };

class Angle {
private:
  // angular value in radians
  double _rads = 0.0f;

public:

  constexpr Angle() = default;
  constexpr Angle(const Angle&) = default;
  Angle& operator=(const Angle&) = default;

  // input value must be in radians (use custom literal syntax for conversions)
  constexpr Angle(double x) : _rads(x) {}

  Angle& operator=(double x) {
    _rads = x;
    return *this;
  }

  constexpr static Angle from_units(AngleUnit units, double x) {
    return (units == AngleUnit::DEG)  ? Angle::deg(x)
         : (units == AngleUnit::MIL)  ? Angle::mil(x)
         : (units == AngleUnit::MRAD) ? Angle::mrad(x)
         :                             Angle(x);
  }
  constexpr static Angle mrad(double x) { return Angle(x / 1000.0); }
  constexpr static Angle deg(double x) { return Angle(x * PI / 180.0); }
  constexpr static Angle mil(double x) { return Angle(x * PI / 3200.0); }
  constexpr static Angle arc(double d, double m, double s) {
    return Angle::deg(const_copysign(const_abs(d) + (m / 60.0) + (s / 3600.0), d));
  }

  inline double& rad() { return _rads; }
  inline const double& rad() const { return _rads; }

  constexpr double in_rad() const { return _rads; }
  constexpr double in_mrad() const { return _rads * 1000.0; }
  constexpr double in_deg() const { return _rads * 180.0 / PI; }
  constexpr double in_mil() const { return _rads * 3200.0 / PI; }

  inline double sin() const { return std::sin(_rads); }
  inline double cos() const { return std::cos(_rads); }
  inline double tan() const { return std::tan(_rads); }
};

constexpr Angle operator-(const Angle& a) { return Angle(-a.in_rad()); }

constexpr double operator+(const Angle&  a, const Angle&  b)  { return a.in_rad() + b.in_rad(); }
constexpr double operator-(const Angle&  a, const Angle&  b)  { return a.in_rad() - b.in_rad(); }
constexpr double operator*(const Angle&  a, const Angle& b)   { return a.in_rad() * b.in_rad(); }
constexpr double operator/(const Angle&  a, const Angle& b)   { return a.in_rad() / b.in_rad(); }
constexpr double operator+(const Angle&  a, const double& b)  { return a.in_rad() + b;          }
constexpr double operator-(const Angle&  a, const double&  b) { return a.in_rad() - b;          }
constexpr double operator*(const Angle&  a, const double& b)  { return a.in_rad() * b;          }
constexpr double operator/(const Angle&  a, const double& b)  { return a.in_rad() / b;          }
constexpr double operator+(const double& a, const Angle&  b)  { return a          + b.in_rad(); }
constexpr double operator-(const double& a, const Angle&  b)  { return a          - b.in_rad(); }
constexpr double operator*(const double& a, const Angle&  b)  { return a          * b.in_rad(); }
constexpr double operator/(const double& a, const Angle&  b)  { return a          / b.in_rad(); }

inline Angle& operator+=(Angle& a, const Angle&  b)  { a.rad() += b.in_rad(); return a; }
inline Angle& operator-=(Angle& a, const Angle&  b)  { a.rad() -= b.in_rad(); return a; }
inline Angle& operator*=(Angle& a, const Angle& b)   { a.rad() *= b.in_rad(); return a; }
inline Angle& operator/=(Angle& a, const Angle& b)   { a.rad() /= b.in_rad(); return a; }
inline Angle& operator+=(Angle& a, const double&  b) { a.rad() += b;          return a; }
inline Angle& operator-=(Angle& a, const double&  b) { a.rad() -= b;          return a; }
inline Angle& operator*=(Angle& a, const double& b)  { a.rad() *= b;          return a; }
inline Angle& operator/=(Angle& a, const double& b)  { a.rad() /= b;          return a; }

constexpr bool operator==(const Angle&  a, const Angle& b)  { return a.in_rad() == b.in_rad(); }
constexpr bool operator!=(const Angle&  a, const Angle& b)  { return a.in_rad() != b.in_rad(); }
constexpr bool operator< (const Angle&  a, const Angle& b)  { return a.in_rad() <  b.in_rad(); }
constexpr bool operator> (const Angle&  a, const Angle& b)  { return a.in_rad() >  b.in_rad(); }
constexpr bool operator<=(const Angle&  a, const Angle& b)  { return a.in_rad() <= b.in_rad(); }
constexpr bool operator>=(const Angle&  a, const Angle& b)  { return a.in_rad() >= b.in_rad(); }
constexpr bool operator==(const Angle&  a, const double& b) { return a.in_rad() == b;          }
constexpr bool operator!=(const Angle&  a, const double& b) { return a.in_rad() != b;          }
constexpr bool operator< (const Angle&  a, const double& b) { return a.in_rad() <  b;          }
constexpr bool operator> (const Angle&  a, const double& b) { return a.in_rad() >  b;          }
constexpr bool operator<=(const Angle&  a, const double& b) { return a.in_rad() <= b;          }
constexpr bool operator>=(const Angle&  a, const double& b) { return a.in_rad() >= b;          }
constexpr bool operator==(const double& a, const Angle& b)  { return a          == b.in_rad(); }
constexpr bool operator!=(const double& a, const Angle& b)  { return a          != b.in_rad(); }
constexpr bool operator< (const double& a, const Angle& b)  { return a          <  b.in_rad(); }
constexpr bool operator> (const double& a, const Angle& b)  { return a          <  b.in_rad(); }
constexpr bool operator<=(const double& a, const Angle& b)  { return a          <= b.in_rad(); }
constexpr bool operator>=(const double& a, const Angle& b)  { return a          >= b.in_rad(); }

//-------------------------------------------------------------------------------
class Anglef {
private:
  // angular value in radians
  float _rads = 0.0f;

public:

  constexpr Anglef() = default;
  constexpr Anglef(const Anglef&) = default;
  Anglef& operator=(const Anglef&) = default;

  // input value must be in radians (use custom literal syntax for conversions)
  constexpr Anglef(float x) : _rads(x) {}

  Anglef& operator=(float x) {
    _rads = x;
    return *this;
  }

  constexpr static Anglef from_units(AngleUnit units, float x) {
    return (units == AngleUnit::DEG) ? Anglef::deg(x)
      : (units == AngleUnit::MIL) ? Anglef::mil(x)
      : (units == AngleUnit::MRAD) ? Anglef::mrad(x)
      : Anglef(x);
  }
  constexpr static Anglef mrad(float x) { return Anglef(x / 1000.0f); }
  constexpr static Anglef deg(float x) { return Anglef(x * PI_f / 180.0f); }
  constexpr static Anglef mil(float x) { return Anglef(x * PI_f / 3200.0f); }
  constexpr static Anglef arc(float d, float m, float s) {
    return Anglef::deg(const_copysign(const_abs(d) + (m / 60.0f) + (s / 3600.0f), d));
  }

  inline float& rad() { return _rads; }
  inline const float& rad() const { return _rads; }

  constexpr float in_rad() const { return _rads; }
  constexpr float in_mrad() const { return _rads * 1000.0f; }
  constexpr float in_deg() const { return _rads * 180.0f / PI_f; }
  constexpr float in_mil() const { return _rads * 3200.0f / PI_f; }

  inline float sin() const { return std::sin(_rads); }
  inline float cos() const { return std::cos(_rads); }
  inline float tan() const { return std::tan(_rads); }
};

constexpr Anglef operator-(const Anglef& a) { return Anglef(-a.in_rad()); }

constexpr float operator+(const Anglef&  a, const Anglef&  b) { return a.in_rad() + b.in_rad(); }
constexpr float operator-(const Anglef&  a, const Anglef&  b) { return a.in_rad() - b.in_rad(); }
constexpr float operator*(const Anglef&  a, const Anglef& b) { return a.in_rad() * b.in_rad(); }
constexpr float operator/(const Anglef&  a, const Anglef& b) { return a.in_rad() / b.in_rad(); }
constexpr float operator+(const Anglef&  a, const float& b) { return a.in_rad() + b; }
constexpr float operator-(const Anglef&  a, const float&  b) { return a.in_rad() - b; }
constexpr float operator*(const Anglef&  a, const float& b) { return a.in_rad() * b; }
constexpr float operator/(const Anglef&  a, const float& b) { return a.in_rad() / b; }
constexpr float operator+(const float& a, const Anglef&  b) { return a + b.in_rad(); }
constexpr float operator-(const float& a, const Anglef&  b) { return a - b.in_rad(); }
constexpr float operator*(const float& a, const Anglef&  b) { return a          * b.in_rad(); }
constexpr float operator/(const float& a, const Anglef&  b) { return a / b.in_rad(); }

inline Anglef& operator+=(Anglef& a, const Anglef&  b) { a.rad() += b.in_rad(); return a; }
inline Anglef& operator-=(Anglef& a, const Anglef&  b) { a.rad() -= b.in_rad(); return a; }
inline Anglef& operator*=(Anglef& a, const Anglef& b) { a.rad() *= b.in_rad(); return a; }
inline Anglef& operator/=(Anglef& a, const Anglef& b) { a.rad() /= b.in_rad(); return a; }
inline Anglef& operator+=(Anglef& a, const float&  b) { a.rad() += b;          return a; }
inline Anglef& operator-=(Anglef& a, const float&  b) { a.rad() -= b;          return a; }
inline Anglef& operator*=(Anglef& a, const float& b) { a.rad() *= b;          return a; }
inline Anglef& operator/=(Anglef& a, const float& b) { a.rad() /= b;          return a; }

constexpr bool operator==(const Anglef&  a, const Anglef& b) { return a.in_rad() == b.in_rad(); }
constexpr bool operator!=(const Anglef&  a, const Anglef& b) { return a.in_rad() != b.in_rad(); }
constexpr bool operator< (const Anglef&  a, const Anglef& b) { return a.in_rad() < b.in_rad(); }
constexpr bool operator> (const Anglef&  a, const Anglef& b) { return a.in_rad() > b.in_rad(); }
constexpr bool operator<=(const Anglef&  a, const Anglef& b) { return a.in_rad() <= b.in_rad(); }
constexpr bool operator>=(const Anglef&  a, const Anglef& b) { return a.in_rad() >= b.in_rad(); }
constexpr bool operator==(const Anglef&  a, const float& b) { return a.in_rad() == b; }
constexpr bool operator!=(const Anglef&  a, const float& b) { return a.in_rad() != b; }
constexpr bool operator< (const Anglef&  a, const float& b) { return a.in_rad() < b; }
constexpr bool operator> (const Anglef&  a, const float& b) { return a.in_rad() > b; }
constexpr bool operator<=(const Anglef&  a, const float& b) { return a.in_rad() <= b; }
constexpr bool operator>=(const Anglef&  a, const float& b) { return a.in_rad() >= b; }
constexpr bool operator==(const float& a, const Anglef& b) { return a == b.in_rad(); }
constexpr bool operator!=(const float& a, const Anglef& b) { return a != b.in_rad(); }
constexpr bool operator< (const float& a, const Anglef& b) { return a < b.in_rad(); }
constexpr bool operator> (const float& a, const Anglef& b) { return a < b.in_rad(); }
constexpr bool operator<=(const float& a, const Anglef& b) { return a <= b.in_rad(); }
constexpr bool operator>=(const float& a, const Anglef& b) { return a >= b.in_rad(); }

//------------------------------------------------------------------------------
template <class T>
struct MatchingAngle;

template <>
struct MatchingAngle<double> { using type = Angle; };

template <>
struct MatchingAngle<float> { using type = Anglef; };

template <class T>
using AngleBase = typename MatchingAngle<T>::type;

//------------------------------------------------------------------------------
//struct Quaternion;
class EulerAng;
class EulerAngf;

template <class T>
struct MatchingEulerAng;

template <>
struct MatchingEulerAng<double> { using type = EulerAng; };

template <>
struct MatchingEulerAng<float> { using type = EulerAngf; };

template <class T>
using EulerAngBase = typename MatchingEulerAng<T>::type;

//------------------------------------------------------------------------------
template <class T>
struct QuaternionBase {
  static constexpr auto zero = static_cast<T>(0);
  static constexpr auto one = static_cast<T>(1);

  // Data members
  //-------------
  std::array<T, 4> wxyz = { zero, zero, zero, zero };
  //-------------

  using Self = QuaternionBase<T>;

  ~QuaternionBase() = default;

  QuaternionBase(Self&&) = default;
  auto operator=(Self&&) -> Self& = default;

  constexpr QuaternionBase(const Self&) = default;
  auto operator=(const Self&) -> Self& = default;
  
  constexpr QuaternionBase() : wxyz{ zero, zero, zero, zero } {}

  constexpr QuaternionBase(T w, T x = zero, T y = zero, T z = zero)
    : wxyz{w, x, y, z} {}

  static constexpr auto unit() -> Self { return { one, zero, zero, zero }; }

  //!
  //! Create a unit quaternion from a set of Tait-Bryan angles.
  //!
  static auto from_euler(const EulerAngBase<T>&) -> Self;
  static auto from_euler(AngleBase<T> yaw, AngleBase<T> pitch, AngleBase<T> roll) -> Self;
  static auto from_euler(T yaw, T pitch, T roll) -> Self;

  //!
  //! Create a unit quaternion from a directional cosine matrix (rotation matrix).
  //!
  EulerAngBase<T> to_euler() const;

  constexpr T w() const { return wxyz[0]; }
  constexpr T x() const { return wxyz[1]; }
  constexpr T y() const { return wxyz[2]; }
  constexpr T z() const { return wxyz[3]; }

  T& w_mut() { return wxyz[0]; }
  T& x_mut() { return wxyz[1]; }
  T& y_mut() { return wxyz[2]; }
  T& z_mut() { return wxyz[3]; }

  const std::array<T, 4>& as_array() const { return wxyz; }

  constexpr auto real() const -> Self { return { w(), zero, zero, zero }; }
  constexpr auto pure() const -> Self { return { zero, x(), y(), z() }; }
  constexpr auto conj() const -> Self { return { w(), -x(), -y(), -z() }; }

  T norm() const {
    return std::sqrt(w()*w() + x()*x() + y()*y() + z()*z());
  }
  auto inv() const -> Self {
    const auto n = norm();
    return { w()/n, x()/n, y()/n, z()/n };
  }

  constexpr T inner_prod(const Self& other) const {
    return w() * other.w() + x() * other.x() + y() * other.y() + z() * other.z();
  }
  constexpr auto scale(const T c) const -> Self {
    return { c*w(), c*x(), c*y(), c*z() };
  }

  constexpr auto operator+() const -> Self { return *this; }
  constexpr auto operator-() const -> Self { return { -w(), -x(), -y(), -z() }; }

  constexpr auto operator*(const Self& rhs) const -> Self {
    return
      { w() * rhs.w() - x() * rhs.x() - y() * rhs.y() - z() * rhs.z()
      , w() * rhs.x() + x() * rhs.w() + y() * rhs.z() - z() * rhs.y()
      , w() * rhs.y() - x() * rhs.z() + y() * rhs.w() + z() * rhs.x()
      , w() * rhs.z() + x() * rhs.y() - y() * rhs.x() + z() * rhs.w()
      };
  }

  constexpr auto operator+(const Self& rhs) const -> Self {
    return { w() + rhs.w(), x() + rhs.x(), y() + rhs.y(), z() + rhs.z() };
  }
  constexpr auto operator-(const Self& rhs) const -> Self {
    return { w() - rhs.w(), x() - rhs.x(), y() - rhs.y(), z() - rhs.z() };
  }
};

template <class T>
constexpr QuaternionBase<T> operator+(const QuaternionBase<T>& a, const T& b) {
  return { a.w() + b, a.x(), a.y(), a.z() };
}

template <class T>
constexpr QuaternionBase<T> operator+(const T& b, const QuaternionBase<T>& a) {
  return { a.w() + b, a.x(), a.y(), a.z() };
}

template <class T>
constexpr bool operator==(const QuaternionBase<T>&  a, const QuaternionBase<T>& b) {
  return a.w() == b.w()
      && a.x() == b.x()
      && a.y() == b.y()
      && a.z() == b.z()
       ;
}

template <class T>
constexpr bool operator!=(const QuaternionBase<T>&  a, const QuaternionBase<T>& b) { return !(a == b); }

//------------------------------------------------------------------------------
using Quaternion = QuaternionBase<double>;
using Quaternionf = QuaternionBase<float>;

//------------------------------------------------------------------------------
class EulerAng {
public:
  EulerAng() = default;

  EulerAng(Angle yaw,Angle pitch,Angle roll)
  :_yaw(yaw)
  ,_pitch(pitch)
  ,_roll(roll)
  {}

  static EulerAng from_quat(const Quaternion&);
  static EulerAng from_quat(double qw, double qx, double qy, double qz);

  Quaternion to_quat() const;

  EulerAng azimuth_rad(double x) && { _yaw = x;   return *this; }
  EulerAng yaw_rad(double x) &&     { _yaw = x;   return *this; }
  EulerAng pitch_rad(double x) &&   { _pitch = x; return *this; }
  EulerAng roll_rad(double x) &&    { _roll = x;  return *this; }

  EulerAng azimuth(Angle x) && { _yaw = x;   return *this; }
  EulerAng yaw(Angle x) && { _yaw = x;   return *this; }
  EulerAng pitch(Angle x) && { _pitch = x; return *this; }
  EulerAng roll(Angle x) && { _roll = x;  return *this; }

  EulerAng azimuth_deg(double x) && { _yaw = Angle::deg(x);   return *this; }
  EulerAng yaw_deg(double x) &&     { _yaw = Angle::deg(x);   return *this; }
  EulerAng pitch_deg(double x) &&   { _pitch = Angle::deg(x); return *this; }
  EulerAng roll_deg(double x) &&    { _roll = Angle::deg(x);  return *this; }

  Angle azimuth() const { return _yaw; }
  Angle yaw() const { return _yaw; }
  Angle pitch() const { return _pitch; }
  Angle roll() const { return _roll; }

  Angle& azimuth() { return _yaw; }
  Angle& yaw() { return _yaw; }
  Angle& pitch() { return _pitch; }
  Angle& roll() { return _roll; }

private:
  // Data members
  //-------------
  Angle _yaw = 0.;
  Angle _pitch = 0.;
  Angle _roll = 0.;
};

//------------------------------------------------------------------------------
inline bool operator==(const EulerAng&  lhs, const EulerAng& rhs) {
  return (lhs.yaw() == rhs.yaw())
      && (lhs.pitch() == rhs.pitch())
      && (lhs.roll() == rhs.roll());
}
inline bool operator!=(const EulerAng&  rhs, const EulerAng& lhs) { return !(rhs == lhs); }

//------------------------------------------------------------------------------
auto azimuth_uncert_from_quat( const Quaternion& q
                             , const Quaternion& sigq
                             ) -> Angle;

//------------------------------------------------------------------------------
//!
//! Normalizes angular value to be (-pi, pi]
//!
Angle norm_angle(Angle theta);

//------------------------------------------------------------------------------
//!
//! Normalizes angular value to be [0, 2*pi)
//!
Angle norm_angle_360(Angle theta);
//------------------------------------------------------------------------------
//!
//! Normalizes lat/lon angles such that
//! lat in [-pi/2, pi/2] and lon in (-pi, pi]
//!
//! @return (latitude, longitude)
//!
std::pair<Angle, Angle> norm_to_sphere(Angle lat, Angle lon);

//------------------------------------------------------------------------------
//!
//! Normalizes lat/lon angles such that
//! lat in [-pi/2, pi/2] and lon in [0, 2*pi)
//!
//! @return (latitude, longitude)
//!
std::pair<Angle, Angle> norm_to_sphere_360(Angle lat, Angle lon);

inline namespace quaternion_literals {
  inline constexpr biogears::Quaternion operator""_q(unsigned long long xw) { return { static_cast<double>(xw), 0., 0., 0. }; }
  inline constexpr biogears::Quaternion operator""_i(unsigned long long xi) { return { 0., static_cast<double>(xi), 0., 0. }; }
  inline constexpr biogears::Quaternion operator""_j(unsigned long long xj) { return { 0., 0., static_cast<double>(xj), 0. }; }
  inline constexpr biogears::Quaternion operator""_k(unsigned long long xk) { return { 0., 0., 0., static_cast<double>(xk) }; }

  inline constexpr biogears::Quaternion operator""_q(long double xw) { return { static_cast<double>(xw), 0., 0., 0. }; }
  inline constexpr biogears::Quaternion operator""_i(long double xi) { return { 0., static_cast<double>(xi), 0., 0. }; }
  inline constexpr biogears::Quaternion operator""_j(long double xj) { return { 0., 0., static_cast<double>(xj), 0. }; }
  inline constexpr biogears::Quaternion operator""_k(long double xk) { return { 0., 0., 0., static_cast<double>(xk) }; }
} // inline quaternain_literals

//------------------------------------------------------------------------------
class EulerAngf {
public:
  EulerAngf() = default;
  EulerAngf(Anglef yaw, Anglef pitch, Anglef roll)
    :_yaw(yaw)
    , _pitch(pitch)
    , _roll(roll)
  {}
  static EulerAngf from_quat(const Quaternionf&);
  static EulerAngf from_quat(float qw, float qx, float qy, float qz);

  Quaternionf to_quat() const;

  EulerAngf azimuth_rad(float x) && { _yaw = x;   return *this; }
  EulerAngf yaw_rad(float x) && { _yaw = x;   return *this; }
  EulerAngf pitch_rad(float x) && { _pitch = x; return *this; }
  EulerAngf roll_rad(float x) && { _roll = x;  return *this; }

  EulerAngf azimuth(Anglef x) && { _yaw = x;   return *this; }
  EulerAngf yaw(Anglef x) && { _yaw = x;   return *this; }
  EulerAngf pitch(Anglef x) && { _pitch = x; return *this; }
  EulerAngf roll(Anglef x) && { _roll = x;  return *this; }

  EulerAngf azimuth_deg(float x) && { _yaw = Anglef::deg(x);   return *this; }
  EulerAngf yaw_deg(float x) && { _yaw = Anglef::deg(x);   return *this; }
  EulerAngf pitch_deg(float x) && { _pitch = Anglef::deg(x); return *this; }
  EulerAngf roll_deg(float x) && { _roll = Anglef::deg(x);  return *this; }

  Anglef azimuth() const { return _yaw; }
  Anglef yaw() const { return _yaw; }
  Anglef pitch() const { return _pitch; }
  Anglef roll() const { return _roll; }

  Anglef& azimuth() { return _yaw; }
  Anglef& yaw() { return _yaw; }
  Anglef& pitch() { return _pitch; }
  Anglef& roll() { return _roll; }

private:
  // Data members
  //-------------
  Anglef _yaw = 0;
  Anglef _pitch = 0;
  Anglef _roll = 0;
};

//------------------------------------------------------------------------------
inline bool operator==(const EulerAngf&  lhs, const EulerAngf& rhs) {
  return (lhs.yaw() == rhs.yaw())
    && (lhs.pitch() == rhs.pitch())
    && (lhs.roll() == rhs.roll());
}
inline bool operator!=(const EulerAngf&  rhs, const EulerAngf& lhs) { return !(rhs == lhs); }

//------------------------------------------------------------------------------
auto azimuth_uncert_from_quat(const Quaternionf& q
  , const Quaternionf& sigq
)->Anglef;

//------------------------------------------------------------------------------
//!
//! Normalizes angular value to be (-pi, pi]
//!
Anglef norm_angle(Anglef theta);

//------------------------------------------------------------------------------
//!
//! Normalizes angular value to be [0, 2*pi)
//!
Anglef norm_angle_360(Anglef theta);
//------------------------------------------------------------------------------
//!
//! Normalizes lat/lon angles such that
//! lat in [-pi/2, pi/2] and lon in (-pi, pi]
//!
//! @return (latitude, longitude)
//!
std::pair<Anglef, Anglef> norm_to_sphere(Anglef lat, Anglef lon);

//------------------------------------------------------------------------------
//!
//! Normalizes lat/lon angles such that
//! lat in [-pi/2, pi/2] and lon in [0, 2*pi)
//!
//! @return (latitude, longitude)
//!
std::pair<Anglef, Anglef> norm_to_sphere_360(Anglef lat, Anglef lon);

inline namespace Quaternionf_literals {
inline constexpr biogears::Quaternionf operator""_f_q(unsigned long long xw) { return biogears::Quaternionf(static_cast<float>(xw)); }
inline constexpr biogears::Quaternionf operator""_f_i(unsigned long long xi) { return biogears::Quaternionf(0.f, static_cast<float>(xi), 0.f, 0.f); }
inline constexpr biogears::Quaternionf operator""_f_j(unsigned long long xj) { return biogears::Quaternionf(0.f, 0.f, static_cast<float>(xj), 0.f); }
inline constexpr biogears::Quaternionf operator""_f_k(unsigned long long xk) { return biogears::Quaternionf(0.f, 0.f, 0.f, static_cast<float>(xk)); }

inline constexpr biogears::Quaternionf operator""_f_q(long double xw) { return biogears::Quaternionf(static_cast<float>(xw)); }
inline constexpr biogears::Quaternionf operator""_f_i(long double xi) { return biogears::Quaternionf(0.f, static_cast<float>(xi), 0.f, 0.f); }
inline constexpr biogears::Quaternionf operator""_f_j(long double xj) { return biogears::Quaternionf(0.f, 0.f, static_cast<float>(xj), 0.f); }
inline constexpr biogears::Quaternionf operator""_f_k(long double xk) { return biogears::Quaternionf(0.f, 0.f, 0.f, static_cast<float>(xk)); }
} // inline quaternain_literals

//==============================================================================
// Templated definitions
//------------------------------------------------------------------------------
template <class T>
auto QuaternionBase<T>::from_euler(const EulerAngBase<T>& ea) -> Self {
  return Self::from_euler(ea.azimuth(), ea.pitch(), ea.roll());
}

//------------------------------------------------------------------------------
template <class T>
auto QuaternionBase<T>::from_euler(AngleBase<T> yaw, AngleBase<T> pitch, AngleBase<T> roll) -> Self {
  return from_euler(yaw.rad(), pitch.rad(), roll.rad());
}

//------------------------------------------------------------------------------
template <class T>
auto QuaternionBase<T>::from_euler(const T yaw, const T pitch, const T roll) -> Self {
  constexpr auto half = static_cast<T>(0.5f);

  const auto t0 = std::cos(yaw * half);
  const auto t1 = std::sin(yaw * half);
  const auto t2 = std::cos(roll * half);
  const auto t3 = std::sin(roll * half);
  const auto t4 = std::cos(pitch * half);
  const auto t5 = std::sin(pitch * half);

  return { t0 * t2 * t4 + t1 * t3 * t5
         , t0 * t3 * t4 - t1 * t2 * t5
         , t0 * t2 * t5 + t1 * t3 * t4
         , t1 * t2 * t4 - t0 * t3 * t5
         };
}

//------------------------------------------------------------------------------
template <class T>
EulerAngBase<T> QuaternionBase<T>::to_euler() const {
  return EulerAngBase<T>::from_quat(*this);
}

} //end namespace biogears



#endif // BIOGEARS_MATH_ANGLES_H
