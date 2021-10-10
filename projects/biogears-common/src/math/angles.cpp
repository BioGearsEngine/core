
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
//! \data 2017 Mar 29
//!

#include <biogears/math/angles.h>

namespace biogears {

  #ifndef ANDROID
  // these are checks to ensure that the Angle methods marked as `constexpr`
  // are in-fact able to evaluate at compile-time
  static_assert(Angle::from_units(AngleUnit::MIL, 0.0).in_rad() == 0.0, "");
  static_assert(Angle(1.) + Angle(2.) == 3., "");

  static_assert(0. + 1.0_i + 2.0_j + 3.0_k == Quaternion(0., 1., 2., 3.), "");
  static_assert(Quaternion(1.,2.,3.,4.).real() + Quaternion(1.,2.,3.,4.).pure() == Quaternion(1.,2.,3.,4.), "");
  static_assert((Quaternion(1.,1.,1.,1.) * Quaternion(1.,1.,1.,1.).conj()).w() == 4.0, "");
  #endif

//------------------------------------------------------------------------------
auto azimuth_uncert_from_quat( const Quaternion& q
                             , const Quaternion& sigq
                             ) -> Angle
{
  auto sqr = [](double x) { return x * x; };
  const double u1 = 2 * (q.w() * q.z() + q.x() * q.y());
  const double u2 = 1 - 2 * (sqr(q.y()) + sqr(q.z()));

  const double t1 = 2 / (sqr(u1) + sqr(u2));

  const double t2 = sqr(q.z() * u2 * sigq.w())
                  + sqr(q.y() * u2 * sigq.x())
                  + sqr((q.x() * u2 + 2 * q.y() * u1) * sigq.y())
                  + sqr((q.w() * u2 + 2 * q.z() * u1) * sigq.z());

  return t1 * sqrt(t2);
}

//------------------------------------------------------------------------------
EulerAng EulerAng::from_quat(const Quaternion& q) {
  return from_quat(q.w(), q.x(), q.y(), q.z());
}
EulerAng EulerAng::from_quat(double qw, double qx, double qy, double qz) {
  const double q01 = qw * qx;
  const double q02 = qw * qy;
  const double q03 = qw * qz;
  const double q11 = qx * qx;
  const double q12 = qx * qy;
  const double q13 = qx * qz;
  const double q22 = qy * qy;
  const double q23 = qy * qz;
  const double q33 = qz * qz;

  const double t2 = std::min(std::max(-1.0, 2.0*(q02 - q13)), 1.0);

  const Angle az { std::atan2(2.0*(q12 + q03), 1.0 - 2.0*(q22 + q33)) };
  const Angle pt { std::asin(t2) };
  const auto pt_az_ = norm_to_sphere_360(pt, az);
  const Angle rl { std::atan2(2.0*(q01 + q23), 1.0 - 2.0*(q11 + q22)) };
  return EulerAng().yaw(pt_az_.second)
                   .pitch(pt_az_.first)
                   .roll(norm_angle(rl));
}

//------------------------------------------------------------------------------
Quaternion EulerAng::to_quat() const {
  return Quaternion::from_euler(*this);
}

//------------------------------------------------------------------------------
Angle norm_angle(Angle theta) {
  const auto x = theta.cos();
  const auto y = theta.sin();
  const auto theta_ = std::atan2(y, x);
  return Angle(theta_);
}
//------------------------------------------------------------------------------
Angle norm_angle_360(Angle theta) {
  const auto x = theta.cos();
  const auto y = theta.sin();
  const auto theta_ = std::atan2(y, x);
  return Angle(theta_ + PI);
}
//------------------------------------------------------------------------------
//!
//! Normalizes lat/lon angles such that
//! lat in [-pi/2, pi/2] and lon in (-pi, pi]
//!
//! @return (latitude, longitude)
//!
std::pair<Angle, Angle> norm_to_sphere(Angle lat, Angle lon) {
	const Angle lat_ = std::asin(lat.sin());
	const Angle lon_ = norm_angle(lon);
	return std::make_pair(lat_, lon_);

}

//------------------------------------------------------------------------------
//!
//! Normalizes lat/lon angles such that
//! lat in [-pi/2, pi/2] and lon in [0, 2*pi)
//!
//! @return (latitude, longitude)
//!
std::pair<Angle, Angle> norm_to_sphere_360(Angle lat, Angle lon) {
	auto latlon = norm_to_sphere(lat, lon);
	const Angle lon_ = latlon.second + ((latlon.second < 0.0) ? TWO_PI : 0.0);
	return std::make_pair(latlon.first, lon_);

}

#ifndef ANDROID
//------------------------------------------------------------------------------
// these are checks to ensure that the Anglef methods marked as `constexpr`
// are in-fact able to evaluate at compile-time
static_assert(Anglef::from_units(AngleUnit::MIL, 0.0f).in_rad() == 0.0f, "");
static_assert(Anglef(1.f) + Anglef(2.f) == 3.f, "");

static_assert(0.f + 1.0_f_i + 2.0_f_j + 3.0_f_k == Quaternionf(0.f, 1.f, 2.f, 3.f), "");
static_assert(Quaternionf(1.f, 2.f, 3.f, 4.f).real() + Quaternionf(1.f, 2.f, 3.f, 4.f).pure() == Quaternionf(1.f, 2.f, 3.f, 4.f), "");
static_assert((Quaternionf(1.f, 1.f, 1.f, 1.f) * Quaternionf(1.f, 1.f, 1.f, 1.f).conj()).w() == 4.0f, "");
#endif()

//------------------------------------------------------------------------------
auto azimuth_uncert_from_quat(const Quaternionf& q
	, const Quaternionf& sigq
) -> Anglef
{
	auto sqr = [](float x) { return x * x; };
	const float u1 = 2.f * (q.w() * q.z() + q.x() * q.y());
	const float u2 = 1.f - 2.f * (sqr(q.y()) + sqr(q.z()));

	const float t1 = 2.f / (sqr(u1) + sqr(u2));

	const float t2 = sqr(q.z() * u2 * sigq.w())
		+ sqr(q.y() * u2 * sigq.x())
		+ sqr((q.x() * u2 + 2.f * q.y() * u1) * sigq.y())
		+ sqr((q.w() * u2 + 2.f * q.z() * u1) * sigq.z());

	return t1 * sqrt(t2);
}

//------------------------------------------------------------------------------
EulerAngf EulerAngf::from_quat(const Quaternionf& q) {
	return from_quat(q.w(), q.x(), q.y(), q.z());
}
EulerAngf EulerAngf::from_quat(float qw, float qx, float qy, float qz) {
	const float q01 = qw * qx;
	const float q02 = qw * qy;
	const float q03 = qw * qz;
	const float q11 = qx * qx;
	const float q12 = qx * qy;
	const float q13 = qx * qz;
	const float q22 = qy * qy;
	const float q23 = qy * qz;
	const float q33 = qz * qz;

	const float t2 = std::min(std::max(-1.0f, 2.0f*(q02 - q13)), 1.0f);

	const Anglef az { std::atan2(2.0f*(q12 + q03), 1.0f - 2.0f*(q22 + q33)) };
	const Anglef pt { std::asin(t2) };
	const auto pt_az_ = norm_to_sphere_360(pt, az);
	const Anglef rl { std::atan2(2.0f*(q01 + q23), 1.0f - 2.0f*(q11 + q22)) };
	return EulerAngf().yaw(pt_az_.second)
		.pitch(pt_az_.first)
		.roll(norm_angle(rl));
}

//------------------------------------------------------------------------------
Quaternionf EulerAngf::to_quat() const {
	return Quaternionf::from_euler(*this);
}

//------------------------------------------------------------------------------
Anglef norm_angle(Anglef theta) {
	const auto x = theta.cos();
	const auto y = theta.sin();
	const auto theta_ = std::atan2(y, x);
	return Anglef(theta_);
}
//------------------------------------------------------------------------------
Anglef norm_angle_360(Anglef theta) {
	const auto x = theta.cos();
	const auto y = theta.sin();
	const auto theta_ = std::atan2(y, x);
	return Anglef(theta_ + PI_f);
}
//------------------------------------------------------------------------------
//!
//! Normalizes lat/lon angles such that
//! lat in [-pi/2, pi/2] and lon in (-pi, pi]
//!
//! @return (latitude, longitude)
//!
std::pair<Anglef, Anglef> norm_to_sphere(Anglef lat, Anglef lon) {
  const Anglef lat_ = std::asin(lat.sin());
  const Anglef lon_ = norm_angle(lon);
  return std::make_pair(lat_, lon_);

}

//------------------------------------------------------------------------------
//!
//! Normalizes lat/lon angles such that
//! lat in [-pi/2, pi/2] and lon in [0, 2*pi)
//!
//! @return (latitude, longitude)
//!
std::pair<Anglef, Anglef> norm_to_sphere_360(Anglef lat, Anglef lon) {
   auto latlon = norm_to_sphere(lat, lon);
   const Anglef lon_ = latlon.second + ((latlon.second < 0.0f) ? TWO_PI_f : 0.0f);
   return std::make_pair(latlon.first, lon_);

}

} // end namespace biogears
