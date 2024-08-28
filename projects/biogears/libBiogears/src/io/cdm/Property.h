/**************************************************************************************
Copyright 2019 Applied Research Associates, Inc.
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
#include <biogears/exports.h>

#include <random>
#include <type_traits>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/enums/SEPropertyEnums.h>
#include <biogears/cdm/properties/SEDecimalFormat.h>
#include <biogears/schema/cdm/Properties.hxx>

// Question: To Serialize Invalid units or not to Serialize?
//           TO Throw an exception when a member is invalid?
#define CDM_PROPERTY_MARSHALL_HELPER(in, out, func)                                          \
  if (in.m_##func && in.m_##func->IsValid()) {                                               \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>());         \
    io::Property::Marshall(*in.m_##func, out.func());                                        \
  } /* else if (in.m_##func) {                                                               \
     throw biogears::CommonDataModelException("func is InValid and cannot be Unmarshalled"); \
   }*/

#define CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, func)                                \
  if (in.m_##func && in.m_##func->IsValid()) {                                              \
    io::Property::Marshall(*in.m_##func, out.func());                                       \
  } /*else if (in.m_##func) {                                                               \
    throw biogears::CommonDataModelException("func is InValid and cannot be Unmarshalled"); \
  }*/

#define CDM_ENUM_MARSHALL_HELPER(in, out, func) \
  if (in.Has##func()) {                         \
    out.func(in.m_##func);                      \
  }

#define SE_PROPERTY_ENUM_MARSHALL_HELPER(in, out, func)                              \
  if (in.Has##func()) {                                                              \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Property::Marshall(in.m_##func, out.func());                                 \
  }

#define SE_OPTIONAL_PROPERTY_ENUM_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func != decltype(in.m_##func)::Invalid) {           \
    io::Property::Marshall(in.m_##func, out.func());             \
  }

namespace biogears {
class RunningAverage;

class SEDecimalFormat;
template <typename T>
class SEScalarQuantity;
class SEFunctionElectricPotentialVsTime;
class SEFunction;
class SEFunctionVolumeVsTime;
class SEHistogramFractionVsLength;
class SEHistogram;
class SEProperty;
class SEScalar0To1;
class SEScalarAmount;
class SEScalarAmountPerMass;
class SEScalarAmountPerTime;
class SEScalarAmountPerVolume;
class SEScalarArea;
class SEScalarAreaPerTimePressure;
class SEScalarElectricCapacitance;
class SEScalarElectricCharge;
class SEScalarElectricCurrent;
class SEScalarElectricInductance;
class SEScalarElectricPotential;
class SEScalarElectricResistance;
class SEScalarEnergy;
class SEScalarEnergyPerAmount;
class SEScalarEnergyPerMass;
class SEScalarFlowCompliance;
class SEScalarFlowElastance;
class SEScalarFlowInertance;
class SEScalarFlowResistance;
class SEScalarForce;
class SEScalarFraction;
class SEScalarFrequency;
class SEScalar;
class SEScalarHeatCapacitance;
class SEScalarHeatCapacitancePerAmount;
class SEScalarHeatCapacitancePerMass;
class SEScalarHeatConductance;
class SEScalarHeatConductancePerArea;
class SEScalarHeatInductance;
class SEScalarHeatResistanceArea;
class SEScalarHeatResistance;
class SEScalarInversePressure;
class SEScalarInverseVolume;
class SEScalarLength;
class SEScalarLengthPerTime;
class SEScalarLengthPerTimePressure;
class SEScalarMass;
class SEScalarMassPerAmount;
class SEScalarMassPerAreaTime;
class SEScalarMassPerMass;
class SEScalarMassPerTime;
class SEScalarMassPerVolume;
class SEScalarNeg1To1;
class SEScalarOsmolality;
class SEScalarOsmolarity;
class SEScalarPower;
class SEScalarPowerPerAreaTemperatureToTheFourth;
class SEScalarPressure;
class SEScalarPressurePerVolume;
class SEScalarPressureTimePerArea;
class SEScalarPressureTimePerVolumeArea;
class SEScalarTemperature;
class SEScalarTime;
class SEScalarVolume;
class SEScalarVolumePerPressure;
class SEScalarVolumePerTimeArea;
class SEScalarVolumePerTime;
class SEScalarVolumePerTimeMass;
class SEScalarVolumePerTimePressureArea;
class SEScalarVolumePerTimePressure;
class SEUnitScalar;

namespace io {
  class BIOGEARS_PRIVATE_API Property {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd = nullptr);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);

    // template <typename T> class SEScalarQuantity;
    template <typename Unit>
    static void UnMarshall(const CDM::ScalarData& in, SEScalarQuantity<Unit>& out, std::default_random_engine* rd = nullptr);
    template <typename Unit>
    static void Marshall(const SEScalarQuantity<Unit>& in, CDM::ScalarData& out);
    // SEDecimalFormat
    static void UnMarshall(xsd::cxx::tree::optional<CDM::DecimalFormatData> const& option_in, SEDecimalFormat& out);
    static void Marshall(const SEDecimalFormat& in, xsd::cxx::tree::optional<CDM::DecimalFormatData>& option_out);
    // class SEDecimalFormat;
    static void UnMarshall(const CDM::DecimalFormatData& in, SEDecimalFormat& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEDecimalFormat& in, CDM::DecimalFormatData& out);
    // class SEFunctionElectricPotentialVsTime;
    static void UnMarshall(const CDM::FunctionElectricPotentialVsTimeData& in, SEFunctionElectricPotentialVsTime& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEFunctionElectricPotentialVsTime& in, CDM::FunctionElectricPotentialVsTimeData& out);
    // class SEFunction;
    static void UnMarshall(const CDM::FunctionData& in, SEFunction& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEFunction& in, CDM::FunctionData& out);
    // class SEFunctionVolumeVsTime;
    static void UnMarshall(const CDM::FunctionVolumeVsTimeData& in, SEFunctionVolumeVsTime& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEFunctionVolumeVsTime& in, CDM::FunctionVolumeVsTimeData& out);
    // class SEHistogramFractionVsLength;
    static void UnMarshall(const CDM::HistogramFractionVsLengthData& in, SEHistogramFractionVsLength& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEHistogramFractionVsLength& in, CDM::HistogramFractionVsLengthData& out);
    // class SEHistogram;
    static void UnMarshall(const CDM::HistogramData& in, SEHistogram& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEHistogram& in, CDM::HistogramData& out);
    // class SERunningAverage
    static void UnMarshall(const CDM::RunningAverageData& in, RunningAverage& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const RunningAverage& in, CDM::RunningAverageData& out);
    // class Scalar0To1Data
    static void UnMarshall(const CDM::Scalar0To1Data& in, SEScalar0To1& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEScalar0To1& in, CDM::Scalar0To1Data& out);
    // class SEScalarNeg1To1
    static void UnMarshall(const CDM::ScalarNeg1To1Data& in, SEScalarNeg1To1& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEScalarNeg1To1& in, CDM::ScalarNeg1To1Data& out);
    // class  SEScalarFractionData
    static void UnMarshall(const CDM::ScalarFractionData& in, SEScalarFraction& out, std::default_random_engine* rd);
    static void Marshall(const SEScalarFraction& in, CDM::ScalarFractionData& out);

    // class SEScalar
    static void UnMarshall(const CDM::ScalarData& in, SEScalar& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEScalar& in, CDM::ScalarData& out);

    // SESide
    static void UnMarshall(const CDM::enumSide& in, SESide& out);
    static void Marshall(const SESide& in, CDM::enumSide& out);
    // SEOpenClosed
    static void UnMarshall(const CDM::enumOpenClosed& in, SEOpenClosed& out);
    static void Marshall(const SEOpenClosed& in, CDM::enumOpenClosed& out);
    // SEOnOff
    static void UnMarshall(const CDM::enumOnOff& in, SEOnOff& out);
    static void Marshall(const SEOnOff& in, CDM::enumOnOff& out);
    //  SEErrorType
    static void UnMarshall(const CDM::enumErrorType& in, SEErrorType& out);
    static void Marshall(const SEErrorType& in, CDM::enumErrorType& out);
  };

  //-------------------------------------------------------------------------------
  template <typename Unit>
  void Property::UnMarshall(const CDM::ScalarData& in, SEScalarQuantity<Unit>& out, std::default_random_engine* rd)
  {
    out.Clear();
    if (in.unit().present()) {
      if (in.deviation().present() && rd) {
        auto nd = std::normal_distribution(in.value(), in.deviation().get());
        out.SetValue(nd(*rd), Unit::GetCompoundUnit(in.unit().get()));
      } else {
        out.SetValue(in.value(), Unit::GetCompoundUnit(in.unit().get()));
      }
    } else {
      throw CommonDataModelException("ScalarQuantity attempted to load a ScalarData with no unit, must have a unit.");
    }
    out.m_readOnly = in.readOnly();
  }
  template <typename Unit>
  void Property::Marshall(const SEScalarQuantity<Unit>& in, CDM::ScalarData& out)
  {
    out.value(in.m_value);
    out.unit(in.m_unit->GetString());
    out.readOnly(in.m_readOnly);

    //    out.value(std::make_unique<std::remove_reference<decltype(out.value())>::type>(in.m_value));
    //    out.unit(std::make_unique<std::remove_reference<decltype(out.unit())>::type>(in.m_unit->GetString()));
    //    out.readOnly(std::make_unique<std::remove_reference<decltype(out.readOnly())>::type>(in.m_readOnly));
  }
  //----------------------------------------------------------------------------------

  template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>*>
  void Property::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present() || option_in->empty()) {
      out = SE::Invalid;
    } else {
      UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>*>
  void Property::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      UnMarshall(option_in.get(), out, rd);
    }
  }

  template <typename SE, typename XSD>
  void Property::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(std::move(item));
  }
} // namespace io

// Operators
bool operator==(CDM::enumSide const& lhs, SESide const& rhs);
bool operator==(CDM::enumOpenClosed const& lhs, SEOpenClosed const& rhs);
bool operator==(CDM::enumOnOff const& lhs, SEOnOff const& rhs);
bool operator==(CDM::enumErrorType const& lhs, SEErrorType const& rhs);

inline bool operator==(SESide const& lhs, CDM::enumSide const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEOpenClosed const& lhs, CDM::enumOpenClosed const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEOnOff const& lhs, CDM::enumOnOff const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEErrorType const& lhs, CDM::enumErrorType const& rhs)
{
  return rhs == lhs;
}

inline bool operator!=(CDM::enumSide const& lhs, SESide const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumOpenClosed const& lhs, SEOpenClosed const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumOnOff const& lhs, SEOnOff const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumErrorType const& lhs, SEErrorType const& rhs)
{
  return !(lhs == rhs);
}

inline bool operator!=(SESide const& lhs, CDM::enumSide const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEOpenClosed const& lhs, CDM::enumOpenClosed const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEOnOff const& lhs, CDM::enumOnOff const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEErrorType const& lhs, CDM::enumErrorType const& rhs)
{
  return !(rhs == lhs);
}

} // namespace biogears
