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

//#include <memory>

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

#define CDM_PROPERTY_UNMARSHAL_HELPER(xsd, func)                                     \
  if (in.m_##func) {                                                                 \
    xsd.func(std::make_unique<std::remove_reference<decltype(xsd.func())>::type>()); \
    io::Property::UnMarshall(*in.m_##func, xsd.func());                              \
  }

namespace io {
  class BIOGEARS_PRIVATE_API Property {
  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //template <typename SE, typename XSD>  option
    static void Marshall(xsd::cxx::tree::optional<CDM::DecimalFormatData> const& option_in, SEDecimalFormat& out);
    static void UnMarshall(const SEDecimalFormat& in, xsd::cxx::tree::optional<CDM::DecimalFormatData>& option_out);
    //template <typename T> class SEScalarQuantity;
    template <typename Unit>
    static void Marshall(const CDM::ScalarData& in, SEScalarQuantity<Unit>& out);
    template <typename Unit>
    static void UnMarshall(const SEScalarQuantity<Unit>& in, CDM::ScalarData& out);
    //class SEDecimalFormat;
    static void Marshall(const CDM::DecimalFormatData& in, SEDecimalFormat& out);
    static void UnMarshall(const SEDecimalFormat& in, CDM::DecimalFormatData& out);
    //class SEFunctionElectricPotentialVsTime;
    static void Marshall(const CDM::FunctionElectricPotentialVsTimeData& in, SEFunctionElectricPotentialVsTime& out);
    static void UnMarshall(const SEFunctionElectricPotentialVsTime& in, CDM::FunctionElectricPotentialVsTimeData& out);
    //class SEFunction;
    static void Marshall(const CDM::FunctionData& in, SEFunction& out);
    static void UnMarshall(const SEFunction& in, CDM::FunctionData& out);
    //class SEFunctionVolumeVsTime;
    static void Marshall(const CDM::FunctionVolumeVsTimeData& in, SEFunctionVolumeVsTime& out);
    static void UnMarshall(const SEFunctionVolumeVsTime& in, CDM::FunctionVolumeVsTimeData& out);
    //class SEHistogramFractionVsLength;
    static void Marshall(const CDM::HistogramFractionVsLengthData& in, SEHistogramFractionVsLength& out);
    static void UnMarshall(const SEHistogramFractionVsLength& in, CDM::HistogramFractionVsLengthData& out);
    //class SEHistogram;
    static void Marshall(const CDM::HistogramData& in, SEHistogram& out);
    static void UnMarshall(const SEHistogram& in, CDM::HistogramData& out);
    //class SEScalar0To1;
    static void Marshall(const CDM::Scalar0To1Data& in, SEScalar0To1& out);
    static void UnMarshall(const SEScalar0To1& in, CDM::Scalar0To1Data& out);
    //class SEScalarAmount;
    static void Marshall(const CDM::ScalarAmountData& in, SEScalarAmount& out);
    static void UnMarshall(const SEScalarAmount& in, CDM::ScalarAmountData& out);
    //class SEScalarAmountPerMass;
    static void Marshall(const CDM::ScalarAmountPerMassData& in, SEScalarAmountPerMass& out);
    static void UnMarshall(const SEScalarAmountPerMass& in, CDM::ScalarAmountPerMassData& out);
    //class SEScalarAmountPerTime;
    static void Marshall(const CDM::ScalarAmountPerTimeData& in, SEScalarAmountPerTime& out);
    static void UnMarshall(const SEScalarAmountPerTime& in, CDM::ScalarAmountPerTimeData& out);
    //class SEScalarAmountPerVolume;
    static void Marshall(const CDM::ScalarAmountPerVolumeData& in, SEScalarAmountPerVolume& out);
    static void UnMarshall(const SEScalarAmountPerVolume& in, CDM::ScalarAmountPerVolumeData& out);
    //class SEScalarArea;
    static void Marshall(const CDM::ScalarAreaData& in, SEScalarArea& out);
    static void UnMarshall(const SEScalarArea& in, CDM::ScalarAreaData& out);
    //class SEScalarAreaPerTimePressure;
    static void Marshall(const CDM::ScalarAreaPerTimePressureData& in, SEScalarAreaPerTimePressure& out);
    static void UnMarshall(const SEScalarAreaPerTimePressure& in, CDM::ScalarAreaPerTimePressureData& out);
    //class SEScalarElectricCapacitance;
    static void Marshall(const CDM::ScalarElectricCapacitanceData& in, SEScalarElectricCapacitance& out);
    static void UnMarshall(const SEScalarElectricCapacitance& in, CDM::ScalarElectricCapacitanceData& out);
    //class SEScalarElectricCharge;
    static void Marshall(const CDM::ScalarElectricChargeData& in, SEScalarElectricCharge& out);
    static void UnMarshall(const SEScalarElectricCharge& in, CDM::ScalarElectricChargeData& out);
    //class SEScalarElectricCurrent;
    static void Marshall(const CDM::ScalarElectricCurrentData& in, SEScalarElectricCurrent& out);
    static void UnMarshall(const SEScalarElectricCurrent& in, CDM::ScalarElectricCurrentData& out);
    //class SEScalarElectricInductance;
    static void Marshall(const CDM::ScalarElectricInductanceData& in, SEScalarElectricInductance& out);
    static void UnMarshall(const SEScalarElectricInductance& in, CDM::ScalarElectricInductanceData& out);
    //class SEScalarElectricPotential;
    static void Marshall(const CDM::ScalarElectricPotentialData& in, SEScalarElectricPotential& out);
    static void UnMarshall(const SEScalarElectricPotential& in, CDM::ScalarElectricPotentialData& out);
    //class SEScalarElectricResistance;
    static void Marshall(const CDM::ScalarElectricResistanceData& in, SEScalarElectricResistance& out);
    static void UnMarshall(const SEScalarElectricResistance& in, CDM::ScalarElectricResistanceData& out);
    //class SEScalarEnergy;
    static void Marshall(const CDM::ScalarEnergyData& in, SEScalarEnergy& out);
    static void UnMarshall(const SEScalarEnergy& in, CDM::ScalarEnergyData& out);
    //class SEScalarEnergyPerAmount;
    static void Marshall(const CDM::ScalarEnergyPerAmountData& in, SEScalarEnergyPerAmount& out);
    static void UnMarshall(const SEScalarEnergyPerAmount& in, CDM::ScalarEnergyPerAmountData& out);
    //class SEScalarEnergyPerMass;
    static void Marshall(const CDM::ScalarEnergyPerMassData& in, SEScalarEnergyPerMass& out);
    static void UnMarshall(const SEScalarEnergyPerMass& in, CDM::ScalarEnergyPerMassData& out);
    //class SEScalarFlowCompliance;
    static void Marshall(const CDM::ScalarFlowComplianceData& in, SEScalarFlowCompliance& out);
    static void UnMarshall(const SEScalarFlowCompliance& in, CDM::ScalarFlowComplianceData& out);
    //class SEScalarFlowElastance;
    static void Marshall(const CDM::ScalarFlowElastanceData& in, SEScalarFlowElastance& out);
    static void UnMarshall(const SEScalarFlowElastance& in, CDM::ScalarFlowElastanceData& out);
    //class SEScalarFlowInertance;
    static void Marshall(const CDM::ScalarFlowInertanceData& in, SEScalarFlowInertance& out);
    static void UnMarshall(const SEScalarFlowInertance& in, CDM::ScalarFlowInertanceData& out);
    //class SEScalarFlowResistance;
    static void Marshall(const CDM::ScalarFlowResistanceData& in, SEScalarFlowResistance& out);
    static void UnMarshall(const SEScalarFlowResistance& in, CDM::ScalarFlowResistanceData& out);
    //class SEScalarForce;
    static void Marshall(const CDM::ScalarForceData& in, SEScalarForce& out);
    static void UnMarshall(const SEScalarForce& in, CDM::ScalarForceData& out);
    //class SEScalarFraction;
    static void Marshall(const CDM::ScalarFractionData& in, SEScalarFraction& out);
    static void UnMarshall(const SEScalarFraction& in, CDM::ScalarFractionData& out);
    //class SEScalarFrequency;
    static void Marshall(const CDM::ScalarFrequencyData& in, SEScalarFrequency& out);
    static void UnMarshall(const SEScalarFrequency& in, CDM::ScalarFrequencyData& out);
    //class SEScalar;
    static void Marshall(const CDM::ScalarData& in, SEScalar& out);
    static void UnMarshall(const SEScalar& in, CDM::ScalarData& out);
    //class SEScalarHeatCapacitance;
    static void Marshall(const CDM::ScalarHeatCapacitanceData& in, SEScalarHeatCapacitance& out);
    static void UnMarshall(const SEScalarHeatCapacitance& in, CDM::ScalarHeatCapacitanceData& out);
    //class SEScalarHeatCapacitancePerAmount;
    static void Marshall(const CDM::ScalarHeatCapacitancePerAmountData& in, SEScalarHeatCapacitancePerAmount& out);
    static void UnMarshall(const SEScalarHeatCapacitancePerAmount& in, CDM::ScalarHeatCapacitancePerAmountData& out);
    //class SEScalarHeatCapacitancePerMass;
    static void Marshall(const CDM::ScalarHeatCapacitancePerMassData& in, SEScalarHeatCapacitancePerMass& out);
    static void UnMarshall(const SEScalarHeatCapacitancePerMass& in, CDM::ScalarHeatCapacitancePerMassData& out);
    //class SEScalarHeatConductance;
    static void Marshall(const CDM::ScalarHeatConductanceData& in, SEScalarHeatConductance& out);
    static void UnMarshall(const SEScalarHeatConductance& in, CDM::ScalarHeatConductanceData& out);
    //class SEScalarHeatConductancePerArea;
    static void Marshall(const CDM::ScalarHeatConductancePerAreaData& in, SEScalarHeatConductancePerArea& out);
    static void UnMarshall(const SEScalarHeatConductancePerArea& in, CDM::ScalarHeatConductancePerAreaData& out);
    //class SEScalarHeatInductance;
    static void Marshall(const CDM::ScalarHeatInductanceData& in, SEScalarHeatInductance& out);
    static void UnMarshall(const SEScalarHeatInductance& in, CDM::ScalarHeatInductanceData& out);
    //class SEScalarHeatResistanceArea;
    static void Marshall(const CDM::ScalarHeatResistanceAreaData& in, SEScalarHeatResistanceArea& out);
    static void UnMarshall(const SEScalarHeatResistanceArea& in, CDM::ScalarHeatResistanceAreaData& out);
    //class SEScalarHeatResistance;
    static void Marshall(const CDM::ScalarHeatResistanceData& in, SEScalarHeatResistance& out);
    static void UnMarshall(const SEScalarHeatResistance& in, CDM::ScalarHeatResistanceData& out);
    //class SEScalarInversePressure;
    static void Marshall(const CDM::ScalarInversePressureData& in, SEScalarInversePressure& out);
    static void UnMarshall(const SEScalarInversePressure& in, CDM::ScalarInversePressureData& out);
    //class SEScalarInverseVolume;
    static void Marshall(const CDM::ScalarInverseVolumeData& in, SEScalarInverseVolume& out);
    static void UnMarshall(const SEScalarInverseVolume& in, CDM::ScalarInverseVolumeData& out);
    //class SEScalarLength;
    static void Marshall(const CDM::ScalarLengthData& in, SEScalarLength& out);
    static void UnMarshall(const SEScalarLength& in, CDM::ScalarLengthData& out);
    //class SEScalarLengthPerTime;
    static void Marshall(const CDM::ScalarLengthPerTimeData& in, SEScalarLengthPerTime& out);
    static void UnMarshall(const SEScalarLengthPerTime& in, CDM::ScalarLengthPerTimeData& out);
    //class SEScalarLengthPerTimePressure;
    static void Marshall(const CDM::ScalarLengthPerTimePressureData& in, SEScalarLengthPerTimePressure& out);
    static void UnMarshall(const SEScalarLengthPerTimePressure& in, CDM::ScalarLengthPerTimePressureData& out);
    //class SEScalarMass;
    static void Marshall(const CDM::ScalarMassData& in, SEScalarMass& out);
    static void UnMarshall(const SEScalarMass& in, CDM::ScalarMassData& out);
    //class SEScalarMassPerAmount;
    static void Marshall(const CDM::ScalarMassPerAmountData& in, SEScalarMassPerAmount& out);
    static void UnMarshall(const SEScalarMassPerAmount& in, CDM::ScalarMassPerAmountData& out);
    //class SEScalarMassPerAreaTime;
    static void Marshall(const CDM::ScalarMassPerAreaTimeData& in, SEScalarMassPerAreaTime& out);
    static void UnMarshall(const SEScalarMassPerAreaTime& in, CDM::ScalarMassPerAreaTimeData& out);
    //class SEScalarMassPerMass;
    static void Marshall(const CDM::ScalarMassPerMassData& in, SEScalarMassPerMass& out);
    static void UnMarshall(const SEScalarMassPerMass& in, CDM::ScalarMassPerMassData& out);
    //class SEScalarMassPerTime;
    static void Marshall(const CDM::ScalarMassPerTimeData& in, SEScalarMassPerTime& out);
    static void UnMarshall(const SEScalarMassPerTime& in, CDM::ScalarMassPerTimeData& out);
    //class SEScalarMassPerVolume;
    static void Marshall(const CDM::ScalarMassPerVolumeData& in, SEScalarMassPerVolume& out);
    static void UnMarshall(const SEScalarMassPerVolume& in, CDM::ScalarMassPerVolumeData& out);
    //class SEScalarNeg1To1;
    static void Marshall(const CDM::ScalarNeg1To1Data& in, SEScalarNeg1To1& out);
    static void UnMarshall(const SEScalarNeg1To1& in, CDM::ScalarNeg1To1Data& out);
    //class SEScalarOsmolality;
    static void Marshall(const CDM::ScalarOsmolalityData& in, SEScalarOsmolality& out);
    static void UnMarshall(const SEScalarOsmolality& in, CDM::ScalarOsmolalityData& out);
    //class SEScalarOsmolarity;
    static void Marshall(const CDM::ScalarOsmolarityData& in, SEScalarOsmolarity& out);
    static void UnMarshall(const SEScalarOsmolarity& in, CDM::ScalarOsmolarityData& out);
    //class SEScalarPower;
    static void Marshall(const CDM::ScalarPowerData& in, SEScalarPower& out);
    static void UnMarshall(const SEScalarPower& in, CDM::ScalarPowerData& out);
    //class SEScalarPowerPerAreaTemperatureToTheFourth;
    static void Marshall(const CDM::ScalarPowerPerAreaTemperatureToTheFourthData& in, SEScalarPowerPerAreaTemperatureToTheFourth& out);
    static void UnMarshall(const SEScalarPowerPerAreaTemperatureToTheFourth& in, CDM::ScalarPowerPerAreaTemperatureToTheFourthData& out);
    //class SEScalarPressure;
    static void Marshall(const CDM::ScalarPressureData& in, SEScalarPressure& out);
    static void UnMarshall(const SEScalarPressure& in, CDM::ScalarPressureData& out);
    //class SEScalarPressurePerVolume;
    static void Marshall(const CDM::ScalarPressurePerVolumeData& in, SEScalarPressurePerVolume& out);
    static void UnMarshall(const SEScalarPressurePerVolume& in, CDM::ScalarPressurePerVolumeData& out);
    //class SEScaMlarPressureTimePerArea;
    static void Marshall(const CDM::ScalarPressureTimePerAreaData& in, SEScalarPressureTimePerArea& out);
    static void UnMarshall(const SEScalarPressureTimePerArea& in, CDM::ScalarPressureTimePerAreaData& out);
    //class SEScalarPressureTimePerVolumeArea;
    static void Marshall(const CDM::ScalarPressureTimePerVolumeAreaData& in, SEScalarPressureTimePerVolumeArea& out);
    static void UnMarshall(const SEScalarPressureTimePerVolumeArea& in, CDM::ScalarPressureTimePerVolumeAreaData& out);
    //class SEScalarTemperature;
    static void Marshall(const CDM::ScalarTemperatureData& in, SEScalarTemperature& out);
    static void UnMarshall(const SEScalarTemperature& in, CDM::ScalarTemperatureData& out);
    //class SEScalarTime;
    static void Marshall(const CDM::ScalarTimeData& in, SEScalarTime& out);
    static void UnMarshall(const SEScalarTime& in, CDM::ScalarTimeData& out);
    //class SEScalarVolume;
    static void Marshall(const CDM::ScalarVolumeData& in, SEScalarVolume& out);
    static void UnMarshall(const SEScalarVolume& in, CDM::ScalarVolumeData& out);
    //class SEScalarVolumePerPressure;
    static void Marshall(const CDM::ScalarVolumePerPressureData& in, SEScalarVolumePerPressure& out);
    static void UnMarshall(const SEScalarVolumePerPressure& in, CDM::ScalarVolumePerPressureData& out);
    //class SEScalarVolumePerTimeArea;
    static void Marshall(const CDM::ScalarVolumePerTimeAreaData& in, SEScalarVolumePerTimeArea& out);
    static void UnMarshall(const SEScalarVolumePerTimeArea& in, CDM::ScalarVolumePerTimeAreaData& out);
    //class SEScalarVolumePerTime;
    static void Marshall(const CDM::ScalarVolumePerTimeData& in, SEScalarVolumePerTime& out);
    static void UnMarshall(const SEScalarVolumePerTime& in, CDM::ScalarVolumePerTimeData& out);
    //class SEScalarVolumePerTimeMass;
    static void Marshall(const CDM::ScalarVolumePerTimeMassData& in, SEScalarVolumePerTimeMass& out);
    static void UnMarshall(const SEScalarVolumePerTimeMass& in, CDM::ScalarVolumePerTimeMassData& out);
    //class SEScalarVolumePerTimePressureArea;
    static void Marshall(const CDM::ScalarVolumePerTimePressureAreaData& in, SEScalarVolumePerTimePressureArea& out);
    static void UnMarshall(const SEScalarVolumePerTimePressureArea& in, CDM::ScalarVolumePerTimePressureAreaData& out);
    //class SEScalarVolumePerTimePressure;
    static void Marshall(const CDM::ScalarVolumePerTimePressureData& in, SEScalarVolumePerTimePressure& out);
    static void UnMarshall(const SEScalarVolumePerTimePressure& in, CDM::ScalarVolumePerTimePressureData& out);
    //class SERunningAverage
    static void Marshall(const CDM::RunningAverageData& in, RunningAverage& out);
    static void UnMarshall(const RunningAverage& in, CDM::RunningAverageData& out);
  };

  //-------------------------------------------------------------------------------
  template <typename Unit>
  void Property::Marshall(const CDM::ScalarData& in, SEScalarQuantity<Unit>& out)
  {
    if (out.m_readOnly) {
#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
      throw CommonDataModelException("Scalar is marked read-only");
#else
      return;
#endif
    }
    out.Clear();
    if (in.unit().present()) {
      out.SetValue(in.value(), Unit::GetCompoundUnit(in.unit().get()));
    } else
      throw CommonDataModelException("ScalarQuantity attempted to load a ScalarData with no unit, must have a unit.");
    out.m_readOnly = in.readOnly();
  }
  //-------------------------------------------------------------------------------
  template <typename Unit>
  void Property::UnMarshall(const SEScalarQuantity<Unit>& in, CDM::ScalarData& out)
  {
    out.value(in.m_value);
    out.unit(in.m_unit->GetString());
    out.readOnly(in.m_readOnly);
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Property::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Property::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
}
}
