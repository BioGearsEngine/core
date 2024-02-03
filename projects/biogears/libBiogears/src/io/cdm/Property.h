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

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/properties/PropertyEnum.h>
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
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // template <typename SE, typename XSD>  option
    static void UnMarshall(xsd::cxx::tree::optional<CDM::DecimalFormatData> const& option_in, SEDecimalFormat& out);
    static void Marshall(const SEDecimalFormat& in, xsd::cxx::tree::optional<CDM::DecimalFormatData>& option_out);
    // template <typename T> class SEScalarQuantity;
    template <typename Unit>
    static void UnMarshall(const CDM::ScalarData& in, SEScalarQuantity<Unit>& out);
    template <typename Unit>
    static void Marshall(const SEScalarQuantity<Unit>& in, CDM::ScalarData& out);
    // class SEDecimalFormat;
    static void UnMarshall(const CDM::DecimalFormatData& in, SEDecimalFormat& out);
    static void Marshall(const SEDecimalFormat& in, CDM::DecimalFormatData& out);
    // class SEFunctionElectricPotentialVsTime;
    static void UnMarshall(const CDM::FunctionElectricPotentialVsTimeData& in, SEFunctionElectricPotentialVsTime& out);
    static void Marshall(const SEFunctionElectricPotentialVsTime& in, CDM::FunctionElectricPotentialVsTimeData& out);
    // class SEFunction;
    static void UnMarshall(const CDM::FunctionData& in, SEFunction& out);
    static void Marshall(const SEFunction& in, CDM::FunctionData& out);
    // class SEFunctionVolumeVsTime;
    static void UnMarshall(const CDM::FunctionVolumeVsTimeData& in, SEFunctionVolumeVsTime& out);
    static void Marshall(const SEFunctionVolumeVsTime& in, CDM::FunctionVolumeVsTimeData& out);
    // class SEHistogramFractionVsLength;
    static void UnMarshall(const CDM::HistogramFractionVsLengthData& in, SEHistogramFractionVsLength& out);
    static void Marshall(const SEHistogramFractionVsLength& in, CDM::HistogramFractionVsLengthData& out);
    // class SEHistogram;
    static void UnMarshall(const CDM::HistogramData& in, SEHistogram& out);
    static void Marshall(const SEHistogram& in, CDM::HistogramData& out);
    // class SEScalar0To1;
    static void UnMarshall(const CDM::Scalar0To1Data& in, SEScalar0To1& out);
    static void Marshall(const SEScalar0To1& in, CDM::Scalar0To1Data& out);
    // class SEScalarAmount;
    static void UnMarshall(const CDM::ScalarAmountData& in, SEScalarAmount& out);
    static void Marshall(const SEScalarAmount& in, CDM::ScalarAmountData& out);
    // class SEScalarAmountPerMass;
    static void UnMarshall(const CDM::ScalarAmountPerMassData& in, SEScalarAmountPerMass& out);
    static void Marshall(const SEScalarAmountPerMass& in, CDM::ScalarAmountPerMassData& out);
    // class SEScalarAmountPerTime;
    static void UnMarshall(const CDM::ScalarAmountPerTimeData& in, SEScalarAmountPerTime& out);
    static void Marshall(const SEScalarAmountPerTime& in, CDM::ScalarAmountPerTimeData& out);
    // class SEScalarAmountPerVolume;
    static void UnMarshall(const CDM::ScalarAmountPerVolumeData& in, SEScalarAmountPerVolume& out);
    static void Marshall(const SEScalarAmountPerVolume& in, CDM::ScalarAmountPerVolumeData& out);
    // class SEScalarArea;
    static void UnMarshall(const CDM::ScalarAreaData& in, SEScalarArea& out);
    static void Marshall(const SEScalarArea& in, CDM::ScalarAreaData& out);
    // class SEScalarAreaPerTimePressure;
    static void UnMarshall(const CDM::ScalarAreaPerTimePressureData& in, SEScalarAreaPerTimePressure& out);
    static void Marshall(const SEScalarAreaPerTimePressure& in, CDM::ScalarAreaPerTimePressureData& out);
    // class SEScalarElectricCapacitance;
    static void UnMarshall(const CDM::ScalarElectricCapacitanceData& in, SEScalarElectricCapacitance& out);
    static void Marshall(const SEScalarElectricCapacitance& in, CDM::ScalarElectricCapacitanceData& out);
    // class SEScalarElectricCharge;
    static void UnMarshall(const CDM::ScalarElectricChargeData& in, SEScalarElectricCharge& out);
    static void Marshall(const SEScalarElectricCharge& in, CDM::ScalarElectricChargeData& out);
    // class SEScalarElectricCurrent;
    static void UnMarshall(const CDM::ScalarElectricCurrentData& in, SEScalarElectricCurrent& out);
    static void Marshall(const SEScalarElectricCurrent& in, CDM::ScalarElectricCurrentData& out);
    // class SEScalarElectricInductance;
    static void UnMarshall(const CDM::ScalarElectricInductanceData& in, SEScalarElectricInductance& out);
    static void Marshall(const SEScalarElectricInductance& in, CDM::ScalarElectricInductanceData& out);
    // class SEScalarElectricPotential;
    static void UnMarshall(const CDM::ScalarElectricPotentialData& in, SEScalarElectricPotential& out);
    static void Marshall(const SEScalarElectricPotential& in, CDM::ScalarElectricPotentialData& out);
    // class SEScalarElectricResistance;
    static void UnMarshall(const CDM::ScalarElectricResistanceData& in, SEScalarElectricResistance& out);
    static void Marshall(const SEScalarElectricResistance& in, CDM::ScalarElectricResistanceData& out);
    // class SEScalarEnergy;
    static void UnMarshall(const CDM::ScalarEnergyData& in, SEScalarEnergy& out);
    static void Marshall(const SEScalarEnergy& in, CDM::ScalarEnergyData& out);
    // class SEScalarEnergyPerAmount;
    static void UnMarshall(const CDM::ScalarEnergyPerAmountData& in, SEScalarEnergyPerAmount& out);
    static void Marshall(const SEScalarEnergyPerAmount& in, CDM::ScalarEnergyPerAmountData& out);
    // class SEScalarEnergyPerMass;
    static void UnMarshall(const CDM::ScalarEnergyPerMassData& in, SEScalarEnergyPerMass& out);
    static void Marshall(const SEScalarEnergyPerMass& in, CDM::ScalarEnergyPerMassData& out);
    // class SEScalarFlowCompliance;
    static void UnMarshall(const CDM::ScalarFlowComplianceData& in, SEScalarFlowCompliance& out);
    static void Marshall(const SEScalarFlowCompliance& in, CDM::ScalarFlowComplianceData& out);
    // class SEScalarFlowElastance;
    static void UnMarshall(const CDM::ScalarFlowElastanceData& in, SEScalarFlowElastance& out);
    static void Marshall(const SEScalarFlowElastance& in, CDM::ScalarFlowElastanceData& out);
    // class SEScalarFlowInertance;
    static void UnMarshall(const CDM::ScalarFlowInertanceData& in, SEScalarFlowInertance& out);
    static void Marshall(const SEScalarFlowInertance& in, CDM::ScalarFlowInertanceData& out);
    // class SEScalarFlowResistance;
    static void UnMarshall(const CDM::ScalarFlowResistanceData& in, SEScalarFlowResistance& out);
    static void Marshall(const SEScalarFlowResistance& in, CDM::ScalarFlowResistanceData& out);
    // class SEScalarForce;
    static void UnMarshall(const CDM::ScalarForceData& in, SEScalarForce& out);
    static void Marshall(const SEScalarForce& in, CDM::ScalarForceData& out);
    // class SEScalarFraction;
    static void UnMarshall(const CDM::ScalarFractionData& in, SEScalarFraction& out);
    static void Marshall(const SEScalarFraction& in, CDM::ScalarFractionData& out);
    // class SEScalarFrequency;
    static void UnMarshall(const CDM::ScalarFrequencyData& in, SEScalarFrequency& out);
    static void Marshall(const SEScalarFrequency& in, CDM::ScalarFrequencyData& out);
    // class SEScalar;
    static void UnMarshall(const CDM::ScalarData& in, SEScalar& out);
    static void Marshall(const SEScalar& in, CDM::ScalarData& out);
    // class SEScalarHeatCapacitance;
    static void UnMarshall(const CDM::ScalarHeatCapacitanceData& in, SEScalarHeatCapacitance& out);
    static void Marshall(const SEScalarHeatCapacitance& in, CDM::ScalarHeatCapacitanceData& out);
    // class SEScalarHeatCapacitancePerAmount;
    static void UnMarshall(const CDM::ScalarHeatCapacitancePerAmountData& in, SEScalarHeatCapacitancePerAmount& out);
    static void Marshall(const SEScalarHeatCapacitancePerAmount& in, CDM::ScalarHeatCapacitancePerAmountData& out);
    // class SEScalarHeatCapacitancePerMass;
    static void UnMarshall(const CDM::ScalarHeatCapacitancePerMassData& in, SEScalarHeatCapacitancePerMass& out);
    static void Marshall(const SEScalarHeatCapacitancePerMass& in, CDM::ScalarHeatCapacitancePerMassData& out);
    // class SEScalarHeatConductance;
    static void UnMarshall(const CDM::ScalarHeatConductanceData& in, SEScalarHeatConductance& out);
    static void Marshall(const SEScalarHeatConductance& in, CDM::ScalarHeatConductanceData& out);
    // class SEScalarHeatConductancePerArea;
    static void UnMarshall(const CDM::ScalarHeatConductancePerAreaData& in, SEScalarHeatConductancePerArea& out);
    static void Marshall(const SEScalarHeatConductancePerArea& in, CDM::ScalarHeatConductancePerAreaData& out);
    // class SEScalarHeatInductance;
    static void UnMarshall(const CDM::ScalarHeatInductanceData& in, SEScalarHeatInductance& out);
    static void Marshall(const SEScalarHeatInductance& in, CDM::ScalarHeatInductanceData& out);
    // class SEScalarHeatResistanceArea;
    static void UnMarshall(const CDM::ScalarHeatResistanceAreaData& in, SEScalarHeatResistanceArea& out);
    static void Marshall(const SEScalarHeatResistanceArea& in, CDM::ScalarHeatResistanceAreaData& out);
    // class SEScalarHeatResistance;
    static void UnMarshall(const CDM::ScalarHeatResistanceData& in, SEScalarHeatResistance& out);
    static void Marshall(const SEScalarHeatResistance& in, CDM::ScalarHeatResistanceData& out);
    // class SEScalarInversePressure;
    static void UnMarshall(const CDM::ScalarInversePressureData& in, SEScalarInversePressure& out);
    static void Marshall(const SEScalarInversePressure& in, CDM::ScalarInversePressureData& out);
    // class SEScalarInverseVolume;
    static void UnMarshall(const CDM::ScalarInverseVolumeData& in, SEScalarInverseVolume& out);
    static void Marshall(const SEScalarInverseVolume& in, CDM::ScalarInverseVolumeData& out);
    // class SEScalarLength;
    static void UnMarshall(const CDM::ScalarLengthData& in, SEScalarLength& out);
    static void Marshall(const SEScalarLength& in, CDM::ScalarLengthData& out);
    // class SEScalarLengthPerTime;
    static void UnMarshall(const CDM::ScalarLengthPerTimeData& in, SEScalarLengthPerTime& out);
    static void Marshall(const SEScalarLengthPerTime& in, CDM::ScalarLengthPerTimeData& out);
    // class SEScalarLengthPerTimePressure;
    static void UnMarshall(const CDM::ScalarLengthPerTimePressureData& in, SEScalarLengthPerTimePressure& out);
    static void Marshall(const SEScalarLengthPerTimePressure& in, CDM::ScalarLengthPerTimePressureData& out);
    // class SEScalarMass;
    static void UnMarshall(const CDM::ScalarMassData& in, SEScalarMass& out);
    static void Marshall(const SEScalarMass& in, CDM::ScalarMassData& out);
    // class SEScalarMassPerAmount;
    static void UnMarshall(const CDM::ScalarMassPerAmountData& in, SEScalarMassPerAmount& out);
    static void Marshall(const SEScalarMassPerAmount& in, CDM::ScalarMassPerAmountData& out);
    // class SEScalarMassPerAreaTime;
    static void UnMarshall(const CDM::ScalarMassPerAreaTimeData& in, SEScalarMassPerAreaTime& out);
    static void Marshall(const SEScalarMassPerAreaTime& in, CDM::ScalarMassPerAreaTimeData& out);
    // class SEScalarMassPerMass;
    static void UnMarshall(const CDM::ScalarMassPerMassData& in, SEScalarMassPerMass& out);
    static void Marshall(const SEScalarMassPerMass& in, CDM::ScalarMassPerMassData& out);
    // class SEScalarMassPerTime;
    static void UnMarshall(const CDM::ScalarMassPerTimeData& in, SEScalarMassPerTime& out);
    static void Marshall(const SEScalarMassPerTime& in, CDM::ScalarMassPerTimeData& out);
    // class SEScalarMassPerVolume;
    static void UnMarshall(const CDM::ScalarMassPerVolumeData& in, SEScalarMassPerVolume& out);
    static void Marshall(const SEScalarMassPerVolume& in, CDM::ScalarMassPerVolumeData& out);
    // class SEScalarNeg1To1;
    static void UnMarshall(const CDM::ScalarNeg1To1Data& in, SEScalarNeg1To1& out);
    static void Marshall(const SEScalarNeg1To1& in, CDM::ScalarNeg1To1Data& out);
    // class SEScalarOsmolality;
    static void UnMarshall(const CDM::ScalarOsmolalityData& in, SEScalarOsmolality& out);
    static void Marshall(const SEScalarOsmolality& in, CDM::ScalarOsmolalityData& out);
    // class SEScalarOsmolarity;
    static void UnMarshall(const CDM::ScalarOsmolarityData& in, SEScalarOsmolarity& out);
    static void Marshall(const SEScalarOsmolarity& in, CDM::ScalarOsmolarityData& out);
    // class SEScalarPower;
    static void UnMarshall(const CDM::ScalarPowerData& in, SEScalarPower& out);
    static void Marshall(const SEScalarPower& in, CDM::ScalarPowerData& out);
    // class SEScalarPowerPerAreaTemperatureToTheFourth;
    static void UnMarshall(const CDM::ScalarPowerPerAreaTemperatureToTheFourthData& in, SEScalarPowerPerAreaTemperatureToTheFourth& out);
    static void Marshall(const SEScalarPowerPerAreaTemperatureToTheFourth& in, CDM::ScalarPowerPerAreaTemperatureToTheFourthData& out);
    // class SEScalarPressure;
    static void UnMarshall(const CDM::ScalarPressureData& in, SEScalarPressure& out);
    static void Marshall(const SEScalarPressure& in, CDM::ScalarPressureData& out);
    // class SEScalarPressurePerVolume;
    static void UnMarshall(const CDM::ScalarPressurePerVolumeData& in, SEScalarPressurePerVolume& out);
    static void Marshall(const SEScalarPressurePerVolume& in, CDM::ScalarPressurePerVolumeData& out);
    // class SEScaMlarPressureTimePerArea;
    static void UnMarshall(const CDM::ScalarPressureTimePerAreaData& in, SEScalarPressureTimePerArea& out);
    static void Marshall(const SEScalarPressureTimePerArea& in, CDM::ScalarPressureTimePerAreaData& out);
    // class SEScalarPressureTimePerVolumeArea;
    static void UnMarshall(const CDM::ScalarPressureTimePerVolumeAreaData& in, SEScalarPressureTimePerVolumeArea& out);
    static void Marshall(const SEScalarPressureTimePerVolumeArea& in, CDM::ScalarPressureTimePerVolumeAreaData& out);
    // class SEScalarTemperature;
    static void UnMarshall(const CDM::ScalarTemperatureData& in, SEScalarTemperature& out);
    static void Marshall(const SEScalarTemperature& in, CDM::ScalarTemperatureData& out);
    // class SEScalarTime;
    static void UnMarshall(const CDM::ScalarTimeData& in, SEScalarTime& out);
    static void Marshall(const SEScalarTime& in, CDM::ScalarTimeData& out);
    // class SEScalarVolume;
    static void UnMarshall(const CDM::ScalarVolumeData& in, SEScalarVolume& out);
    static void Marshall(const SEScalarVolume& in, CDM::ScalarVolumeData& out);
    // class SEScalarVolumePerPressure;
    static void UnMarshall(const CDM::ScalarVolumePerPressureData& in, SEScalarVolumePerPressure& out);
    static void Marshall(const SEScalarVolumePerPressure& in, CDM::ScalarVolumePerPressureData& out);
    // class SEScalarVolumePerTimeArea;
    static void UnMarshall(const CDM::ScalarVolumePerTimeAreaData& in, SEScalarVolumePerTimeArea& out);
    static void Marshall(const SEScalarVolumePerTimeArea& in, CDM::ScalarVolumePerTimeAreaData& out);
    // class SEScalarVolumePerTime;
    static void UnMarshall(const CDM::ScalarVolumePerTimeData& in, SEScalarVolumePerTime& out);
    static void Marshall(const SEScalarVolumePerTime& in, CDM::ScalarVolumePerTimeData& out);
    // class SEScalarVolumePerTimeMass;
    static void UnMarshall(const CDM::ScalarVolumePerTimeMassData& in, SEScalarVolumePerTimeMass& out);
    static void Marshall(const SEScalarVolumePerTimeMass& in, CDM::ScalarVolumePerTimeMassData& out);
    // class SEScalarVolumePerTimePressureArea;
    static void UnMarshall(const CDM::ScalarVolumePerTimePressureAreaData& in, SEScalarVolumePerTimePressureArea& out);
    static void Marshall(const SEScalarVolumePerTimePressureArea& in, CDM::ScalarVolumePerTimePressureAreaData& out);
    // class SEScalarVolumePerTimePressure;
    static void UnMarshall(const CDM::ScalarVolumePerTimePressureData& in, SEScalarVolumePerTimePressure& out);
    static void Marshall(const SEScalarVolumePerTimePressure& in, CDM::ScalarVolumePerTimePressureData& out);
    // class SERunningAverage
    static void UnMarshall(const CDM::RunningAverageData& in, RunningAverage& out);
    static void Marshall(const RunningAverage& in, CDM::RunningAverageData& out);
  };

  //-------------------------------------------------------------------------------
  template <typename Unit>
  void Property::UnMarshall(const CDM::ScalarData& in, SEScalarQuantity<Unit>& out)
  {
    out.Clear();
    if (in.unit().present()) {
      out.SetValue(in.value(), Unit::GetCompoundUnit(in.unit().get()));
    } else {
      throw CommonDataModelException("ScalarQuantity attempted to load a ScalarData with no unit, must have a unit.");
    }
    out.m_readOnly = in.readOnly();
  }
  //-------------------------------------------------------------------------------
  template <typename Unit>
  void Property::Marshall(const SEScalarQuantity<Unit>& in, CDM::ScalarData& out)
  {
    out.value(in.m_value);
    out.unit(in.m_unit->GetString());
    out.readOnly(in.m_readOnly);
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Property::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Property::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
}

}
