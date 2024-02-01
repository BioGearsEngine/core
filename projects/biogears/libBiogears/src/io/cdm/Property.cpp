#include "Property.h"

#include <biogears/schema/cdm/Properties.hxx>

#include <biogears/cdm/properties/SEDecimalFormat.h>
#include <biogears/cdm/properties/SEFunction.h>
#include <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.h>
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
#include <biogears/cdm/properties/SEHistogram.h>
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
#include <biogears/cdm/properties/SEScalarAmount.h>
#include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>
#include <biogears/cdm/properties/SEScalarElectricCapacitance.h>
#include <biogears/cdm/properties/SEScalarElectricCharge.h>
#include <biogears/cdm/properties/SEScalarElectricCurrent.h>
#include <biogears/cdm/properties/SEScalarElectricInductance.h>
#include <biogears/cdm/properties/SEScalarElectricPotential.h>
#include <biogears/cdm/properties/SEScalarElectricResistance.h>
#include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>
#include <biogears/cdm/properties/SEScalarEnergyPerMass.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerAmount.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>
#include <biogears/cdm/properties/SEScalarHeatConductance.h>
#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>
#include <biogears/cdm/properties/SEScalarHeatInductance.h>
#include <biogears/cdm/properties/SEScalarInversePressure.h>
#include <biogears/cdm/properties/SEScalarInverseVolume.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerMass.h>
#include <biogears/cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/properties/SEScalarVolumePerPressure.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>
#include <biogears/cdm/utils/RunningAverage.h>

#if defined(BIOGEARS_THROW_READONLY_EXCEPTIONS)
#define READ_ONLY_CHECK() \
  if (out.m_readOnly)     \
    throw CommonDataModelException("Scalar is marked read-only");
#else
#define READ_ONLY_CHECK() \
  if (out.m_readOnly)     \
    return;
#endif

namespace biogears {
namespace io {
  //class SEDecimalFormat
  //template <typename SE, typename XSD>  option
  void Property::UnMarshall(xsd::cxx::tree::optional<CDM::DecimalFormatData> const& option_in, SEDecimalFormat& out)
  {
    if (!option_in.present()) {
      out.Reset();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  void Property::Marshall(SEDecimalFormat const& in, xsd::cxx::tree::optional<CDM::DecimalFormatData>& option_out)
  {
    auto item = std::make_unique<CDM::DecimalFormatData>();
    Marshall(in, *item);
    option_out.set(*item);
  }

  void Property::UnMarshall(const CDM::DecimalFormatData& in, SEDecimalFormat& out)
  {
    out.Reset();
    if (in.DecimalFormat().present()) {
      if (in.DecimalFormat().get() == CDM::enumDecimalFormat::FixedMantissa)
        out.m_Notation = DecimalNotation::Fixed;
      else if (in.DecimalFormat().get() == CDM::enumDecimalFormat::SignificantDigits)
        out.m_Notation = DecimalNotation::Scientific;
    }
    if (in.Precision().present())
      out.m_Precision = in.Precision().get();
  }
  void Property::Marshall(const SEDecimalFormat& in, CDM::DecimalFormatData& out)
  {
    out.Precision(static_cast<CDM::DecimalFormatData::Precision_type>(in.m_Precision));
    switch (in.m_Notation) {
    case DecimalNotation::Default:
    case DecimalNotation::Fixed: {
      out.DecimalFormat(CDM::enumDecimalFormat::FixedMantissa);
      break;
    }
    case DecimalNotation::Scientific: {
      out.DecimalFormat(CDM::enumDecimalFormat::SignificantDigits);
      break;
    }
    }
  }
  //class SEFunctionElectricPotentialVsTime-------------------------------------------------
  void Property::UnMarshall(const CDM::FunctionElectricPotentialVsTimeData& in, SEFunctionElectricPotentialVsTime& out)
  {
    Property::UnMarshall(static_cast<CDM::FunctionData const&>(in), static_cast<SEFunction&>(out));
    out.m_TimeUnit = &TimeUnit::GetCompoundUnit(in.IndependentUnit().get());
    out.m_ElectricPotentialUnit = &ElectricPotentialUnit::GetCompoundUnit(in.DependentUnit().get());
  }
  void Property::Marshall(const SEFunctionElectricPotentialVsTime& in, CDM::FunctionElectricPotentialVsTimeData& out)
  {
    Property::Marshall(static_cast<SEFunction const&>(in), static_cast<CDM::FunctionData&>(out));
    out.IndependentUnit(in.m_TimeUnit->GetString());
    out.DependentUnit(in.m_ElectricPotentialUnit->GetString());
  }
  //class SEFunction-------------------------------------------------
  void Property::UnMarshall(const CDM::FunctionData& in, SEFunction& out)
  {
    out.Clear();
    for (auto& dv : in.Dependent().DoubleList()) {
      out.m_Dependent.push_back(dv);
    }
    for (auto& iv : in.Independent().DoubleList()) {
      out.m_Independent.push_back(iv);
    }
  }
  void Property::Marshall(const SEFunction& in, CDM::FunctionData& out)
  {
    out.Dependent(std::make_unique<CDM::DoubleArray>());
    out.Dependent().DoubleList(std::make_unique<CDM::DoubleList>());
    out.Independent(std::make_unique<CDM::DoubleArray>());
    out.Independent().DoubleList(std::make_unique<CDM::DoubleList>());
    for (auto& dv : in.m_Dependent) {
      out.Dependent().DoubleList().push_back(dv);
    }
    for (auto& iv : in.m_Independent) {
      out.Independent().DoubleList().push_back(iv);
    }
  }
  //class SEFunctionVolumeVsTime-------------------------------------------------
  void Property::UnMarshall(const CDM::FunctionVolumeVsTimeData& in, SEFunctionVolumeVsTime& out)
  {
    Property::UnMarshall(static_cast<CDM::FunctionData const&>(in), static_cast<SEFunction&>(out));
    out.m_TimeUnit = &TimeUnit::GetCompoundUnit(in.IndependentUnit().get());
    out.m_VolumeUnit = &VolumeUnit::GetCompoundUnit(in.DependentUnit().get());
  }
  void Property::Marshall(const SEFunctionVolumeVsTime& in, CDM::FunctionVolumeVsTimeData& out)
  {
    Property::Marshall(static_cast<SEFunction const&>(in), static_cast<CDM::FunctionData&>(out));
    out.IndependentUnit(in.m_TimeUnit->GetString());
    out.DependentUnit(in.m_VolumeUnit->GetString());
  }
  //class SEHistogramFractionVsLength-------------------------------------------------
  void Property::UnMarshall(const CDM::HistogramFractionVsLengthData& in, SEHistogramFractionVsLength& out)
  {
    Property::UnMarshall(static_cast<CDM::HistogramData const&>(in), static_cast<SEHistogram&>(out));
    out.m_LengthUnit = &LengthUnit::GetCompoundUnit(in.IndependentUnit().get());
  }
  void Property::Marshall(const SEHistogramFractionVsLength& in, CDM::HistogramFractionVsLengthData& out)
  {
    Property::Marshall(static_cast<SEHistogram const&>(in), static_cast<CDM::HistogramData&>(out));
    out.IndependentUnit(in.m_LengthUnit->GetString());
  }
  //class SEHistogram-------------------------------------------------
  void Property::UnMarshall(const CDM::HistogramData& in, SEHistogram& out)
  {
    out.Clear();
    for (auto& dv : in.Dependent().DoubleList())
      out.m_Dependent.push_back(dv);
    for (auto& iv : in.Independent().DoubleList())
      out.m_Independent.push_back(iv);
  }
  void Property::Marshall(const SEHistogram& in, CDM::HistogramData& out)
  {
    out.Dependent(std::unique_ptr<CDM::DoubleList>(new CDM::DoubleList()));
    out.Dependent().DoubleList(std::unique_ptr<CDM::DoubleList>(new CDM::DoubleList()));
    out.Independent(std::unique_ptr<CDM::DoubleList>(new CDM::DoubleList()));
    out.Independent().DoubleList(std::unique_ptr<CDM::DoubleList>(new CDM::DoubleList()));
    for (auto& dv : in.m_Dependent)
      out.Dependent().DoubleList().push_back(dv);
    for (auto& iv : in.m_Independent)
      out.Independent().DoubleList().push_back(iv);
  }
  //class SEScalar0To1-------------------------------------------------
  void Property::UnMarshall(const CDM::Scalar0To1Data& in, SEScalar0To1& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalar&>(out));
  }
  void Property::Marshall(const SEScalar0To1& in, CDM::Scalar0To1Data& out)
  {
    Property::Marshall(static_cast<SEScalar const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarAmount-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarAmountData& in, SEScalarAmount& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<AmountUnit>&>(out));
  }
  void Property::Marshall(const SEScalarAmount& in, CDM::ScalarAmountData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<AmountUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarAmountPerMass-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarAmountPerMassData& in, SEScalarAmountPerMass& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<AmountPerMassUnit>&>(out));
  }
  void Property::Marshall(const SEScalarAmountPerMass& in, CDM::ScalarAmountPerMassData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<AmountPerMassUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarAmountPerTime-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarAmountPerTimeData& in, SEScalarAmountPerTime& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<AmountPerTimeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarAmountPerTime& in, CDM::ScalarAmountPerTimeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<AmountPerTimeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarAmountPerVolume-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarAmountPerVolumeData& in, SEScalarAmountPerVolume& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<AmountPerVolumeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarAmountPerVolume& in, CDM::ScalarAmountPerVolumeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<AmountPerVolumeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarArea-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarAreaData& in, SEScalarArea& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<AreaUnit>&>(out));
  }
  void Property::Marshall(const SEScalarArea& in, CDM::ScalarAreaData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<AreaUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarAreaPerTimePressure-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarAreaPerTimePressureData& in, SEScalarAreaPerTimePressure& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<AreaPerTimePressureUnit>&>(out));
  }
  void Property::Marshall(const SEScalarAreaPerTimePressure& in, CDM::ScalarAreaPerTimePressureData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<AreaPerTimePressureUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarElectricCapacitance-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarElectricCapacitanceData& in, SEScalarElectricCapacitance& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<ElectricCapacitanceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarElectricCapacitance& in, CDM::ScalarElectricCapacitanceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<ElectricCapacitanceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarElectricCharge-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarElectricChargeData& in, SEScalarElectricCharge& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<ElectricChargeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarElectricCharge& in, CDM::ScalarElectricChargeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<ElectricChargeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarElectricCurrent-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarElectricCurrentData& in, SEScalarElectricCurrent& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<ElectricCurrentUnit>&>(out));
  }
  void Property::Marshall(const SEScalarElectricCurrent& in, CDM::ScalarElectricCurrentData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<ElectricCurrentUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarElectricInductance-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarElectricInductanceData& in, SEScalarElectricInductance& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<ElectricInductanceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarElectricInductance& in, CDM::ScalarElectricInductanceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<ElectricInductanceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarElectricPotential-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarElectricPotentialData& in, SEScalarElectricPotential& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<ElectricPotentialUnit>&>(out));
  }
  void Property::Marshall(const SEScalarElectricPotential& in, CDM::ScalarElectricPotentialData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<ElectricPotentialUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarElectricResistance-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarElectricResistanceData& in, SEScalarElectricResistance& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<ElectricResistanceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarElectricResistance& in, CDM::ScalarElectricResistanceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<ElectricResistanceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarEnergy-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarEnergyData& in, SEScalarEnergy& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<EnergyUnit>&>(out));
  }
  void Property::Marshall(const SEScalarEnergy& in, CDM::ScalarEnergyData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<EnergyUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarEnergyPerAmount-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarEnergyPerAmountData& in, SEScalarEnergyPerAmount& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<EnergyPerAmountUnit>&>(out));
  }
  void Property::Marshall(const SEScalarEnergyPerAmount& in, CDM::ScalarEnergyPerAmountData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<EnergyPerAmountUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarEnergyPerMass-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarEnergyPerMassData& in, SEScalarEnergyPerMass& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<EnergyPerMassUnit>&>(out));
  }
  void Property::Marshall(const SEScalarEnergyPerMass& in, CDM::ScalarEnergyPerMassData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<EnergyPerMassUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarFlowCompliance-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarFlowComplianceData& in, SEScalarFlowCompliance& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<FlowComplianceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarFlowCompliance& in, CDM::ScalarFlowComplianceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<FlowComplianceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarFlowElastance-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarFlowElastanceData& in, SEScalarFlowElastance& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<FlowElastanceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarFlowElastance& in, CDM::ScalarFlowElastanceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<FlowElastanceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarFlowInertance-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarFlowInertanceData& in, SEScalarFlowInertance& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<FlowInertanceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarFlowInertance& in, CDM::ScalarFlowInertanceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<FlowInertanceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarFlowResistance-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarFlowResistanceData& in, SEScalarFlowResistance& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<FlowResistanceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarFlowResistance& in, CDM::ScalarFlowResistanceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<FlowResistanceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarForce-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarForceData& in, SEScalarForce& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<ForceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarForce& in, CDM::ScalarForceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<ForceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarFraction-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarFractionData& in, SEScalarFraction& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalar&>(out));
  }
  void Property::Marshall(const SEScalarFraction& in, CDM::ScalarFractionData& out)
  {
    Property::Marshall(static_cast<SEScalar const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarFrequency-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarFrequencyData& in, SEScalarFrequency& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<FrequencyUnit>&>(out));
  }
  void Property::Marshall(const SEScalarFrequency& in, CDM::ScalarFrequencyData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<FrequencyUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalar-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarData& in, SEScalar& out)
  {
    READ_ONLY_CHECK()
    out.Clear();
    ;
    out.m_value = in.value();
    if (in.unit().present()) {
      std::string u = in.unit().get();
      if (!("unitless" == u || "" == u || u.empty())) {
        throw CommonDataModelException("CDM::Scalar API is intended to be unitless, You are trying to load a ScalarData with a unit defined");
      }
    }
    out.m_readOnly = in.readOnly();
  }
  void Property::Marshall(const SEScalar& in, CDM::ScalarData& out)
  {
    out.value(in.m_value);
    out.readOnly(in.m_readOnly);
  }
  //class SEScalarHeatCapacitance-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarHeatCapacitanceData& in, SEScalarHeatCapacitance& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<HeatCapacitanceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarHeatCapacitance& in, CDM::ScalarHeatCapacitanceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<HeatCapacitanceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarHeatCapacitancePerAmount-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarHeatCapacitancePerAmountData& in, SEScalarHeatCapacitancePerAmount& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<HeatCapacitancePerAmountUnit>&>(out));
  }
  void Property::Marshall(const SEScalarHeatCapacitancePerAmount& in, CDM::ScalarHeatCapacitancePerAmountData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<HeatCapacitancePerAmountUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarHeatCapacitancePerMass-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarHeatCapacitancePerMassData& in, SEScalarHeatCapacitancePerMass& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<HeatCapacitancePerMassUnit>&>(out));
  }
  void Property::Marshall(const SEScalarHeatCapacitancePerMass& in, CDM::ScalarHeatCapacitancePerMassData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<HeatCapacitancePerMassUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarHeatConductance-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarHeatConductanceData& in, SEScalarHeatConductance& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<HeatConductanceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarHeatConductance& in, CDM::ScalarHeatConductanceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<HeatConductanceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarHeatConductancePerArea-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarHeatConductancePerAreaData& in, SEScalarHeatConductancePerArea& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<HeatConductancePerAreaUnit>&>(out));
  }
  void Property::Marshall(const SEScalarHeatConductancePerArea& in, CDM::ScalarHeatConductancePerAreaData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<HeatConductancePerAreaUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarHeatInductance-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarHeatInductanceData& in, SEScalarHeatInductance& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<HeatInductanceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarHeatInductance& in, CDM::ScalarHeatInductanceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<HeatInductanceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarHeatResistanceArea-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarHeatResistanceAreaData& in, SEScalarHeatResistanceArea& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<HeatResistanceAreaUnit>&>(out));
  }
  void Property::Marshall(const SEScalarHeatResistanceArea& in, CDM::ScalarHeatResistanceAreaData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<HeatResistanceAreaUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarHeatResistance-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarHeatResistanceData& in, SEScalarHeatResistance& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<HeatResistanceUnit>&>(out));
  }
  void Property::Marshall(const SEScalarHeatResistance& in, CDM::ScalarHeatResistanceData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<HeatResistanceUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarInversePressure-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarInversePressureData& in, SEScalarInversePressure& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<InversePressureUnit>&>(out));
  }
  void Property::Marshall(const SEScalarInversePressure& in, CDM::ScalarInversePressureData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<InversePressureUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarInverseVolume-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarInverseVolumeData& in, SEScalarInverseVolume& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<InverseVolumeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarInverseVolume& in, CDM::ScalarInverseVolumeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<InverseVolumeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarLength-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarLengthData& in, SEScalarLength& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<LengthUnit>&>(out));
  }
  void Property::Marshall(const SEScalarLength& in, CDM::ScalarLengthData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<LengthUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarLengthPerTime-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarLengthPerTimeData& in, SEScalarLengthPerTime& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<LengthPerTimeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarLengthPerTime& in, CDM::ScalarLengthPerTimeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<LengthPerTimeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarLengthPerTimePressure-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarLengthPerTimePressureData& in, SEScalarLengthPerTimePressure& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<LengthPerTimePressureUnit>&>(out));
  }
  void Property::Marshall(const SEScalarLengthPerTimePressure& in, CDM::ScalarLengthPerTimePressureData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<LengthPerTimePressureUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarMass-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarMassData& in, SEScalarMass& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<MassUnit>&>(out));
  }
  void Property::Marshall(const SEScalarMass& in, CDM::ScalarMassData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<MassUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarMassPerAmount-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarMassPerAmountData& in, SEScalarMassPerAmount& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<MassPerAmountUnit>&>(out));
  }
  void Property::Marshall(const SEScalarMassPerAmount& in, CDM::ScalarMassPerAmountData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<MassPerAmountUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarMassPerAreaTime-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarMassPerAreaTimeData& in, SEScalarMassPerAreaTime& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<MassPerAreaTimeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarMassPerAreaTime& in, CDM::ScalarMassPerAreaTimeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<MassPerAreaTimeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarMassPerMass-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarMassPerMassData& in, SEScalarMassPerMass& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<MassPerMassUnit>&>(out));
  }
  void Property::Marshall(const SEScalarMassPerMass& in, CDM::ScalarMassPerMassData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<MassPerMassUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarMassPerTime-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarMassPerTimeData& in, SEScalarMassPerTime& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<MassPerTimeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarMassPerTime& in, CDM::ScalarMassPerTimeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<MassPerTimeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarMassPerVolume-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarMassPerVolumeData& in, SEScalarMassPerVolume& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<MassPerVolumeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarMassPerVolume& in, CDM::ScalarMassPerVolumeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<MassPerVolumeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarNeg1To1-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarNeg1To1Data& in, SEScalarNeg1To1& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalar&>(out));
  }
  void Property::Marshall(const SEScalarNeg1To1& in, CDM::ScalarNeg1To1Data& out)
  {
    Property::Marshall(static_cast<SEScalar const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarOsmolality-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarOsmolalityData& in, SEScalarOsmolality& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<OsmolalityUnit>&>(out));
  }
  void Property::Marshall(const SEScalarOsmolality& in, CDM::ScalarOsmolalityData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<OsmolalityUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarOsmolarity-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarOsmolarityData& in, SEScalarOsmolarity& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<OsmolarityUnit>&>(out));
  }
  void Property::Marshall(const SEScalarOsmolarity& in, CDM::ScalarOsmolarityData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<OsmolarityUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarPower-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarPowerData& in, SEScalarPower& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<PowerUnit>&>(out));
  }
  void Property::Marshall(const SEScalarPower& in, CDM::ScalarPowerData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<PowerUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarPowerPerAreaTemperatureToTheFourth-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarPowerPerAreaTemperatureToTheFourthData& in, SEScalarPowerPerAreaTemperatureToTheFourth& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>&>(out));
  }
  void Property::Marshall(const SEScalarPowerPerAreaTemperatureToTheFourth& in, CDM::ScalarPowerPerAreaTemperatureToTheFourthData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarPressure-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarPressureData& in, SEScalarPressure& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<PressureUnit>&>(out));
  }
  void Property::Marshall(const SEScalarPressure& in, CDM::ScalarPressureData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<PressureUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarPressurePerVolume-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarPressurePerVolumeData& in, SEScalarPressurePerVolume& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<PressurePerVolumeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarPressurePerVolume& in, CDM::ScalarPressurePerVolumeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<PressurePerVolumeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScaMlarPressureTimePerArea-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarPressureTimePerAreaData& in, SEScalarPressureTimePerArea& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<PressureTimePerAreaUnit>&>(out));
  }
  void Property::Marshall(const SEScalarPressureTimePerArea& in, CDM::ScalarPressureTimePerAreaData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<PressureTimePerAreaUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarPressureTimePerVolumeArea-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarPressureTimePerVolumeAreaData& in, SEScalarPressureTimePerVolumeArea& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<PressureTimePerVolumeAreaUnit>&>(out));
  }
  void Property::Marshall(const SEScalarPressureTimePerVolumeArea& in, CDM::ScalarPressureTimePerVolumeAreaData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<PressureTimePerVolumeAreaUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarTemperature-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarTemperatureData& in, SEScalarTemperature& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<TemperatureUnit>&>(out));
  }
  void Property::Marshall(const SEScalarTemperature& in, CDM::ScalarTemperatureData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<TemperatureUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarTime-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarTimeData& in, SEScalarTime& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<TimeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarTime& in, CDM::ScalarTimeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<TimeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarVolume-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarVolumeData& in, SEScalarVolume& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<VolumeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarVolume& in, CDM::ScalarVolumeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<VolumeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarVolumePerPressure-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarVolumePerPressureData& in, SEScalarVolumePerPressure& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<VolumePerPressureUnit>&>(out));
  }
  void Property::Marshall(const SEScalarVolumePerPressure& in, CDM::ScalarVolumePerPressureData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<VolumePerPressureUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarVolumePerTimeArea-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarVolumePerTimeAreaData& in, SEScalarVolumePerTimeArea& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<VolumePerTimeAreaUnit>&>(out));
  }
  void Property::Marshall(const SEScalarVolumePerTimeArea& in, CDM::ScalarVolumePerTimeAreaData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<VolumePerTimeAreaUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarVolumePerTime-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarVolumePerTimeData& in, SEScalarVolumePerTime& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<VolumePerTimeUnit>&>(out));
  }
  void Property::Marshall(const SEScalarVolumePerTime& in, CDM::ScalarVolumePerTimeData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<VolumePerTimeUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarVolumePerTimeMass-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarVolumePerTimeMassData& in, SEScalarVolumePerTimeMass& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<VolumePerTimeMassUnit>&>(out));
  }
  void Property::Marshall(const SEScalarVolumePerTimeMass& in, CDM::ScalarVolumePerTimeMassData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<VolumePerTimeMassUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarVolumePerTimePressureArea-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarVolumePerTimePressureAreaData& in, SEScalarVolumePerTimePressureArea& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<VolumePerTimePressureAreaUnit>&>(out));
  }
  void Property::Marshall(const SEScalarVolumePerTimePressureArea& in, CDM::ScalarVolumePerTimePressureAreaData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<VolumePerTimePressureAreaUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SEScalarVolumePerTimePressure-------------------------------------------------
  void Property::UnMarshall(const CDM::ScalarVolumePerTimePressureData& in, SEScalarVolumePerTimePressure& out)
  {
    Property::UnMarshall(static_cast<CDM::ScalarData const&>(in), static_cast<SEScalarQuantity<VolumePerTimePressureUnit>&>(out));
  }
  void Property::Marshall(const SEScalarVolumePerTimePressure& in, CDM::ScalarVolumePerTimePressureData& out)
  {
    Property::Marshall(static_cast<SEScalarQuantity<VolumePerTimePressureUnit> const&>(in), static_cast<CDM::ScalarData&>(out));
  }
  //class SERunningAverage-------------------------------------------------
  void Property::UnMarshall(const CDM::RunningAverageData& in, RunningAverage& out)
  {
    out.m_Sum = in.sum();
    out.m_NumSamples = in.numSamples();
  }
  void Property::Marshall(const RunningAverage& in, CDM::RunningAverageData& out)
  {
    out.sum(in.m_Sum);
    out.numSamples(in.m_NumSamples);
  }
}
}
