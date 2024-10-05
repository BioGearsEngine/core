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

//!
//! @author
//! @date   2021/03/22
//!
//! Unit Test for BioGears Seralization
//!
#include <thread>

#include <gtest/gtest.h>

#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_Property_TEST
#define TEST_FIXTURE_NAME DISABLED_PropertiesFixture
#else
#define TEST_FIXTURE_NAME IO_PropertiesFixture
#endif

// The fixture for testing class Foo.
class TEST_FIXTURE_NAME : public ::testing::Test {
protected:
  // You can do set-up work for each test here.
  TEST_FIXTURE_NAME() = default;

  // You can do clean-up work that doesn't throw exceptions here.
  virtual ~TEST_FIXTURE_NAME() = default;

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  // Code here will be called immediately after the constructor (right
  // before each test).
  virtual void SetUp();

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown();
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{
}

#define USING_TYPES(name)            \
  using namespace biogears::io;      \
  using SEType = biogears :: SE ## name; \
  using CDMType = CDM :: name ## Data;

#include <biogears/cdm/properties/SEProperties.h>

#include <biogears/cdm/properties/SEDecimalFormat.h>
//!
//!DecimalFormat;
//! static void UnMarshall(const CDM::DecimalFormatData& in, SEDecimalFormat& out);
//! static void Marshall(const SEDecimalFormat& in, CDM::DecimalFormatData& out);
TEST_F(TEST_FIXTURE_NAME, DecimalFormat)
{

  USING_TYPES(DecimalFormat)

  SEType source, sink;
  CDMType data;

  source.SetNotation(biogears::DecimalNotation::Fixed);
  source.SetPrecision(3);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.h>
#include <biogears/cdm/properties/SEScalarElectricPotential.h>
//! TYPE FunctionElectricPotentialVsTime
//! static void UnMarshall(const CDM::FunctionElectricPotentialVsTimeData& in, SEFunctionElectricPotentialVsTime& out);
//! static void Marshall(const SEFunctionElectricPotentialVsTime& in, CDM::FunctionElectricPotentialVsTimeData& out);
TEST_F(TEST_FIXTURE_NAME, FunctionElectricPotentialVsTime)
{
  USING_TYPES(FunctionElectricPotentialVsTime)

  SEType source, sink;
  CDMType data;

  source.GetTime().push_back(1.0);
  source.GetTime().push_back(2.0);
  source.GetTime().push_back(3.0);
  source.GetTime().push_back(4.0);

  source.SetTimeUnit(biogears::TimeUnit::hr);

  source.GetElectricPotential().push_back(2);
  source.GetElectricPotential().push_back(3);
  source.GetElectricPotential().push_back(4);
  source.GetElectricPotential().push_back(5);

  source.SetElectricPotentialUnit(biogears::ElectricPotentialUnit::mV);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.h>
//!
//! TYPE Function;
//! static void UnMarshall(const CDM::FunctionData& in, SEFunction& out);
//! static void Marshall(const SEFunction& in, CDM::FunctionData& out);
TEST_F(TEST_FIXTURE_NAME, Function)
{
  USING_TYPES(Function)

  SEType source, sink;
  CDMType data;

  source.GetDependent().push_back(1.0);
  source.GetDependent().push_back(2.0);
  source.GetDependent().push_back(3.0);
  source.GetDependent().push_back(4.0);

  source.GetIndependent().push_back(2);
  source.GetIndependent().push_back(3);
  source.GetIndependent().push_back(4);
  source.GetIndependent().push_back(5);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
//!
//! TYPE FunctionVolumeVsTime;
//! static void UnMarshall(const CDM::FunctionVolumeVsTimeData& in, SEFunctionVolumeVsTime& out);
//! static void Marshall(const SEFunctionVolumeVsTime& in, CDM::FunctionVolumeVsTimeData& out);
TEST_F(TEST_FIXTURE_NAME, FunctionVolumeVsTime)
{
  USING_TYPES(FunctionVolumeVsTime)

  SEType source, sink;
  CDMType data;

  source.GetDependent().push_back(1.0);
  source.GetDependent().push_back(2.0);
  source.GetDependent().push_back(3.0);
  source.GetDependent().push_back(4.0);

  source.GetIndependent().push_back(2);
  source.GetIndependent().push_back(3);
  source.GetIndependent().push_back(4);
  source.GetIndependent().push_back(5);

  source.SetTimeUnit(biogears::TimeUnit::hr);
  source.SetVolumeUnit(biogears::VolumeUnit::dL);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
//!
//! TYPE HistogramFractionVsLength;
//! static void UnMarshall(const CDM::HistogramFractionVsLengthData& in, SEHistogramFractionVsLength& out);
//! static void Marshall(const SEHistogramFractionVsLength& in, CDM::HistogramFractionVsLengthData& out);
TEST_F(TEST_FIXTURE_NAME, HistogramFractionVsLength)
{
  USING_TYPES(HistogramFractionVsLength)

  SEType source, sink;
  CDMType data;

  source.GetDependent().push_back(1.0);
  source.GetDependent().push_back(2.0);
  source.GetDependent().push_back(3.0);
  source.GetDependent().push_back(4.0);

  source.GetIndependent().push_back(2);
  source.GetIndependent().push_back(3);
  source.GetIndependent().push_back(4);
  source.GetIndependent().push_back(5);

  source.SetLengthUnit(biogears::LengthUnit::ft);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEHistogram.h>
//!
//! TYPE Histogram
//! static void UnMarshall(const CDM::HistogramData& in, SEHistogram& out);
//! static void Marshall(const SEHistogram& in, CDM::HistogramData& out);
TEST_F(TEST_FIXTURE_NAME, Histogram)
{
  USING_TYPES(Histogram)

  SEType source, sink;
  CDMType data;

  source.GetDependent().push_back(1.0);
  source.GetDependent().push_back(2.0);
  source.GetDependent().push_back(3.0);
  source.GetDependent().push_back(4.0);

  source.GetIndependent().push_back(2);
  source.GetIndependent().push_back(3);
  source.GetIndependent().push_back(4);
  source.GetIndependent().push_back(5);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalar0To1.h>
//!
//! TYPE Scalar0To1
//! static void UnMarshall(const CDM::Scalar0To1Data& in, SEScalar0To1& out);
//! static void Marshall(const SEScalar0To1& in, CDM::Scalar0To1Data& out);
TEST_F(TEST_FIXTURE_NAME, 0To1)
{
  USING_TYPES(Scalar0To1)

  SEType source, sink;
  CDMType data;

  source.SetValue(0.14159);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarAmount.h>
//!
//! TYPE ScalarAmount
//! static void UnMarshall(const CDM::ScalarAmountData& in, SEScalarAmount& out);
//! static void Marshall(const SEScalarAmount& in, CDM::ScalarAmountData& out);
TEST_F(TEST_FIXTURE_NAME, Amount)
{
  USING_TYPES(ScalarAmount)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AmountUnit::pmol);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/properties/SEScalarAmountPerMass.h>
//!
//! TYPE ScalarAmountPerMass
//! static void UnMarshall(const CDM::ScalarAmountPerMassData& in, SEScalarAmountPerMass& out);
//! static void Marshall(const SEScalarAmountPerMass& in, CDM::ScalarAmountPerMassData& out);
TEST_F(TEST_FIXTURE_NAME, AmountPerMass)
{
  USING_TYPES(ScalarAmountPerMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AmountPerMassUnit::ct_Per_ug);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
//!
//! TYPE ScalarAmountPerTime
//! static void UnMarshall(const CDM::ScalarAmountPerTimeData& in, SEScalarAmountPerTime& out);
//! static void Marshall(const SEScalarAmountPerTime& in, CDM::ScalarAmountPerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, AmountPerTime)
{
  USING_TYPES(ScalarAmountPerTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AmountPerTimeUnit::pmol_Per_min);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
//!
//! TYPE ScalarAmountPerVolume
//! static void UnMarshall(const CDM::ScalarAmountPerVolumeData& in, SEScalarAmountPerVolume& out);
//! static void Marshall(const SEScalarAmountPerVolume& in, CDM::ScalarAmountPerVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, AmountPerVolume)
{
  USING_TYPES(ScalarAmountPerVolume)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AmountPerVolumeUnit::ct_Per_uL);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarArea.h>
//!
//! TYPE ScalarArea
//! static void UnMarshall(const CDM::ScalarAreaData& in, SEScalarArea& out);
//! static void Marshall(const SEScalarArea& in, CDM::ScalarAreaData& out);
TEST_F(TEST_FIXTURE_NAME, Area)
{
  USING_TYPES(ScalarArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AreaUnit::m2);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>
//!
//! TYPE ScalarAreaPerTimePressure
//! static void UnMarshall(const CDM::ScalarAreaPerTimePressureData& in, SEScalarAreaPerTimePressure& out);
//! static void Marshall(const SEScalarAreaPerTimePressure& in, CDM::ScalarAreaPerTimePressureData& out);
TEST_F(TEST_FIXTURE_NAME, AreaPerTimePressure)
{
  USING_TYPES(ScalarAreaPerTimePressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AreaPerTimePressureUnit::m2_Per_min_mmHg);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricCapacitance.h>

//!
//! TYPE ScalarElectricCapacitance
//! static void UnMarshall(const CDM::ScalarElectricCapacitanceData& in, SEScalarElectricCapacitance& out);
//! static void Marshall(const SEScalarElectricCapacitance& in, CDM::ScalarElectricCapacitanceData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricCapacitance)
{ 
  USING_TYPES(ScalarElectricCapacitance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricCapacitanceUnit::F);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricCharge.h>

//!
//! TYPE ScalarElectricCharge
//! static void UnMarshall(const CDM::ScalarElectricChargeData& in, SEScalarElectricCharge& out);
//! static void Marshall(const SEScalarElectricCharge& in, CDM::ScalarElectricChargeData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricCharge)
{   
  USING_TYPES(ScalarElectricCharge)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricChargeUnit::C);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricCurrent.h>

//!
//! TYPE ScalarElectricCurrent
//! static void UnMarshall(const CDM::ScalarElectricCurrentData& in, SEScalarElectricCurrent& out);
//! static void Marshall(const SEScalarElectricCurrent& in, CDM::ScalarElectricCurrentData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricCurrent)
{      
  USING_TYPES(ScalarElectricCurrent)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricCurrentUnit::A);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricInductance.h>

//!
//! TYPE ScalarElectricInductance
//! static void UnMarshall(const CDM::ScalarElectricInductanceData& in, SEScalarElectricInductance& out);
//! static void Marshall(const SEScalarElectricInductance& in, CDM::ScalarElectricInductanceData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricInductance)
{       
  USING_TYPES(ScalarElectricInductance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricInductanceUnit::H);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricPotential.h>

//!
//! TYPE ScalarElectricPotential
//! static void UnMarshall(const CDM::ScalarElectricPotentialData& in, SEScalarElectricPotential& out);
//! static void Marshall(const SEScalarElectricPotential& in, CDM::ScalarElectricPotentialData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricPotential)
{     
  USING_TYPES(ScalarElectricPotential)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricPotentialUnit::mV);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricResistance.h>

//!
//! TYPE ScalarElectricResistance
//! static void UnMarshall(const CDM::ScalarElectricResistanceData& in, SEScalarElectricResistance& out);
//! static void Marshall(const SEScalarElectricResistance& in, CDM::ScalarElectricResistanceData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricResistance)
{    
  USING_TYPES(ScalarElectricResistance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricResistanceUnit::Ohm);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarEnergy.h>

//!
//! TYPE ScalarEnergy
//! static void UnMarshall(const CDM::ScalarEnergyData& in, SEScalarEnergy& out);
//! static void Marshall(const SEScalarEnergy& in, CDM::ScalarEnergyData& out);
TEST_F(TEST_FIXTURE_NAME, Energy)
{     
  USING_TYPES(ScalarEnergy)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::EnergyUnit::kJ);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>

//!
//! TYPE ScalarEnergyPerAmount
//! static void UnMarshall(const CDM::ScalarEnergyPerAmountData& in, SEScalarEnergyPerAmount& out);
//! static void Marshall(const SEScalarEnergyPerAmount& in, CDM::ScalarEnergyPerAmountData& out);
TEST_F(TEST_FIXTURE_NAME, EnergyPerAmount)
{     
  USING_TYPES(ScalarEnergyPerAmount)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::EnergyPerAmountUnit::kJ_Per_mol);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarEnergyPerMass.h>

//!
//! TYPE ScalarEnergyPerMass
//! static void UnMarshall(const CDM::ScalarEnergyPerMassData& in, SEScalarEnergyPerMass& out);
//! static void Marshall(const SEScalarEnergyPerMass& in, CDM::ScalarEnergyPerMassData& out);
TEST_F(TEST_FIXTURE_NAME, EnergyPerMass)
{   
  USING_TYPES(ScalarEnergyPerMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::EnergyPerMassUnit::kcal_Per_kg);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>

//!
//! TYPE ScalarFlowCompliance
//! static void UnMarshall(const CDM::ScalarFlowComplianceData& in, SEScalarFlowCompliance& out);
//! static void Marshall(const SEScalarFlowCompliance& in, CDM::ScalarFlowComplianceData& out);
TEST_F(TEST_FIXTURE_NAME, FlowCompliance)
{   
  USING_TYPES(ScalarFlowCompliance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::FlowComplianceUnit::mL_Per_cmH2O);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFlowElastance.h>

//!
//! TYPE ScalarFlowElastance
//! static void UnMarshall(const CDM::ScalarFlowElastanceData& in, SEScalarFlowElastance& out);
//! static void Marshall(const SEScalarFlowElastance& in, CDM::ScalarFlowElastanceData& out);
TEST_F(TEST_FIXTURE_NAME, FlowElastance)
{   
  USING_TYPES(ScalarFlowElastance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::FlowElastanceUnit::mmHg_Per_mL);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFlowInertance.h>

//!
//! TYPE ScalarFlowInertance
//! static void UnMarshall(const CDM::ScalarFlowInertanceData& in, SEScalarFlowInertance& out);
//! static void Marshall(const SEScalarFlowInertance& in, CDM::ScalarFlowInertanceData& out);
TEST_F(TEST_FIXTURE_NAME, FlowInertance)
{    
  USING_TYPES(ScalarFlowInertance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::FlowInertanceUnit::cmH2O_s2_Per_mL);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFlowResistance.h>

//!
//! TYPE ScalarFlowResistance
//! static void UnMarshall(const CDM::ScalarFlowResistanceData& in, SEScalarFlowResistance& out);
//! static void Marshall(const SEScalarFlowResistance& in, CDM::ScalarFlowResistanceData& out);
TEST_F(TEST_FIXTURE_NAME, FlowResistance)
{    
  USING_TYPES(ScalarFlowResistance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::FlowResistanceUnit::cmH2O_s_Per_L);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarForce.h>

//!
//! TYPE ScalarForce
//! static void UnMarshall(const CDM::ScalarForceData& in, SEScalarForce& out);
//! static void Marshall(const SEScalarForce& in, CDM::ScalarForceData& out);
TEST_F(TEST_FIXTURE_NAME, Force)
{    
  USING_TYPES(ScalarForce)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ForceUnit::dyn);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFraction.h>

//!
//! TYPE ScalarFraction
//! static void UnMarshall(const CDM::ScalarFractionData& in, SEScalarFraction& out);
//! static void Marshall(const SEScalarFraction& in, CDM::ScalarFractionData& out);
TEST_F(TEST_FIXTURE_NAME, Fraction)
{   
  USING_TYPES(ScalarFraction)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFrequency.h>

//!
//! TYPE ScalarFrequency
//! static void UnMarshall(const CDM::ScalarFrequencyData& in, SEScalarFrequency& out);
//! static void Marshall(const SEScalarFrequency& in, CDM::ScalarFrequencyData& out);
TEST_F(TEST_FIXTURE_NAME, Frequency)
{  
  USING_TYPES(ScalarFrequency)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::FrequencyUnit::Per_hr);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalar.h>

//!
//! TYPE Scalar
//! static void UnMarshall(const CDM::ScalarData& in, SEScalar& out);
//! static void Marshall(const SEScalar& in, CDM::ScalarData& out);
TEST_F(TEST_FIXTURE_NAME, Scalar)
{   
  USING_TYPES(Scalar)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>

//!
//! TYPE ScalarHeatCapacitance
//! static void UnMarshall(const CDM::ScalarHeatCapacitanceData& in, SEScalarHeatCapacitance& out);
//! static void Marshall(const SEScalarHeatCapacitance& in, CDM::ScalarHeatCapacitanceData& out);
TEST_F(TEST_FIXTURE_NAME, HeatCapacitance)
{   
  USING_TYPES(ScalarHeatCapacitance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatCapacitanceUnit::kcal_Per_C);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerAmount.h>

//!
//! TYPE ScalarHeatCapacitancePerAmount
//! static void UnMarshall(const CDM::ScalarHeatCapacitancePerAmountData& in, SEScalarHeatCapacitancePerAmount& out);
//! static void Marshall(const SEScalarHeatCapacitancePerAmount& in, CDM::ScalarHeatCapacitancePerAmountData& out);
TEST_F(TEST_FIXTURE_NAME, HeatCapacitancePerAmount)
{  
  USING_TYPES(ScalarHeatCapacitancePerAmount)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatCapacitancePerAmountUnit::J_Per_K_mol);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>

//!
//! TYPE ScalarHeatCapacitancePerMass
//! static void UnMarshall(const CDM::ScalarHeatCapacitancePerMassData& in, SEScalarHeatCapacitancePerMass& out);
//! static void Marshall(const SEScalarHeatCapacitancePerMass& in, CDM::ScalarHeatCapacitancePerMassData& out);
TEST_F(TEST_FIXTURE_NAME, HeatCapacitancePerMass)
{  
  USING_TYPES(ScalarHeatCapacitancePerMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatCapacitancePerMassUnit::kcal_Per_C_kg);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatConductance.h>

//!
//! TYPE ScalarHeatConductance
//! static void UnMarshall(const CDM::ScalarHeatConductanceData& in, SEScalarHeatConductance& out);
//! static void Marshall(const SEScalarHeatConductance& in, CDM::ScalarHeatConductanceData& out);
TEST_F(TEST_FIXTURE_NAME, HeatConductance)
{   
  USING_TYPES(ScalarHeatConductance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatConductanceUnit::kcal_Per_K_s);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>

//!
//! TYPE ScalarHeatConductancePerArea
//! static void UnMarshall(const CDM::ScalarHeatConductancePerAreaData& in, SEScalarHeatConductancePerArea& out);
//! static void Marshall(const SEScalarHeatConductancePerArea& in, CDM::ScalarHeatConductancePerAreaData& out);
TEST_F(TEST_FIXTURE_NAME, HeatConductancePerArea)
{  
  USING_TYPES(ScalarHeatConductancePerArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatConductancePerAreaUnit::BTU_Per_hr_ft2_C);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatInductance.h>

//!
//! TYPE ScalarHeatInductance
//! static void UnMarshall(const CDM::ScalarHeatInductanceData& in, SEScalarHeatInductance& out);
//! static void Marshall(const SEScalarHeatInductance& in, CDM::ScalarHeatInductanceData& out);
TEST_F(TEST_FIXTURE_NAME, HeatInductance)
{   
  USING_TYPES(ScalarHeatInductance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatInductanceUnit::K_s_Per_W);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>

//!
//! TYPE ScalarHeatResistanceArea
//! static void UnMarshall(const CDM::ScalarHeatResistanceAreaData& in, SEScalarHeatResistanceArea& out);
//! static void Marshall(const SEScalarHeatResistanceArea& in, CDM::ScalarHeatResistanceAreaData& out);
TEST_F(TEST_FIXTURE_NAME, HeatResistanceArea)
{ 
  USING_TYPES(ScalarHeatResistanceArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatResistanceAreaUnit::tog);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatResistance.h>

//!
//! TYPE ScalarHeatResistance
//! static void UnMarshall(const CDM::ScalarHeatResistanceData& in, SEScalarHeatResistance& out);
//! static void Marshall(const SEScalarHeatResistance& in, CDM::ScalarHeatResistanceData& out);
TEST_F(TEST_FIXTURE_NAME, HeatResistance)
{     
  USING_TYPES(ScalarHeatResistance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatResistanceUnit::C_s_Per_kcal);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarInversePressure.h>

//!
//! TYPE ScalarInversePressure
//! static void UnMarshall(const CDM::ScalarInversePressureData& in, SEScalarInversePressure& out);
//! static void Marshall(const SEScalarInversePressure& in, CDM::ScalarInversePressureData& out);
TEST_F(TEST_FIXTURE_NAME, InversePressure)
{   
  USING_TYPES(ScalarInversePressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::InversePressureUnit::Inverse_mmHg);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarInverseVolume.h>

//!
//! TYPE ScalarInverseVolume
//! static void UnMarshall(const CDM::ScalarInverseVolumeData& in, SEScalarInverseVolume& out);
//! static void Marshall(const SEScalarInverseVolume& in, CDM::ScalarInverseVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, InverseVolume)
{ 
  USING_TYPES(ScalarInverseVolume)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::InverseVolumeUnit::Inverse_L);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarLength.h>

//!
//! TYPE ScalarLength
//! static void UnMarshall(const CDM::ScalarLengthData& in, SEScalarLength& out);
//! static void Marshall(const SEScalarLength& in, CDM::ScalarLengthData& out);
TEST_F(TEST_FIXTURE_NAME, Length)
{    
  USING_TYPES(ScalarLength)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::LengthUnit::ft);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>

//!
//! TYPE ScalarLengthPerTime
//! static void UnMarshall(const CDM::ScalarLengthPerTimeData& in, SEScalarLengthPerTime& out);
//! static void Marshall(const SEScalarLengthPerTime& in, CDM::ScalarLengthPerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, LengthPerTime)
{  
  USING_TYPES(ScalarLengthPerTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::LengthPerTimeUnit::cm_Per_s);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarLengthPerTimePressure.h>

//!
//! TYPE ScalarLengthPerTimePressure
//! static void UnMarshall(const CDM::ScalarLengthPerTimePressureData& in, SEScalarLengthPerTimePressure& out);
//! static void Marshall(const SEScalarLengthPerTimePressure& in, CDM::ScalarLengthPerTimePressureData& out);
TEST_F(TEST_FIXTURE_NAME, LengthPerTimePressure)
{  
  USING_TYPES(ScalarLengthPerTimePressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::LengthPerTimePressureUnit::cm_Per_s_mmHg);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarMass.h>

//!
//! TYPE ScalarMass
//! static void UnMarshall(const CDM::ScalarMassData& in, SEScalarMass& out);
//! static void Marshall(const SEScalarMass& in, CDM::ScalarMassData& out);
TEST_F(TEST_FIXTURE_NAME, Mass)
{  
  USING_TYPES(ScalarMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassUnit::kg);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>

//!
//! TYPE ScalarMassPerAmount
//! static void UnMarshall(const CDM::ScalarMassPerAmountData& in, SEScalarMassPerAmount& out);
//! static void Marshall(const SEScalarMassPerAmount& in, CDM::ScalarMassPerAmountData& out);
TEST_F(TEST_FIXTURE_NAME, MassPerAmount)
{  
  USING_TYPES(ScalarMassPerAmount)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassPerAmountUnit::g_Per_mol);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarArea.h>

//!
//! TYPE ScalarMassPerAreaTime
//! static void UnMarshall(const CDM::ScalarMassPerAreaTimeData& in, SEScalarMassPerAreaTime& out);
//! static void Marshall(const SEScalarMassPerAreaTime& in, CDM::ScalarMassPerAreaTimeData& out);
TEST_F(TEST_FIXTURE_NAME, MassPerAreaTime)
{   
  USING_TYPES(ScalarMassPerAreaTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassPerAreaTimeUnit::g_Per_cm2_s);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarMassPerMass.h>

//!
//! TYPE ScalarMassPerMass
//! static void UnMarshall(const CDM::ScalarMassPerMassData& in, SEScalarMassPerMass& out);
//! static void Marshall(const SEScalarMassPerMass& in, CDM::ScalarMassPerMassData& out);
TEST_F(TEST_FIXTURE_NAME, MassPerMass)
{  
  USING_TYPES(ScalarMassPerMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassPerMassUnit::mg_Per_g);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarMassPerTime.h>

//!
//! TYPE ScalarMassPerTime
//! static void UnMarshall(const CDM::ScalarMassPerTimeData& in, SEScalarMassPerTime& out);
//! static void Marshall(const SEScalarMassPerTime& in, CDM::ScalarMassPerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, MassPerTime)
{   
  USING_TYPES(ScalarMassPerTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassPerTimeUnit::g_Per_min);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>

//!
//! TYPE ScalarMassPerVolume
//! static void UnMarshall(const CDM::ScalarMassPerVolumeData& in, SEScalarMassPerVolume& out);
//! static void Marshall(const SEScalarMassPerVolume& in, CDM::ScalarMassPerVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, MassPerVolume)
{  
  USING_TYPES(ScalarMassPerVolume)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassPerVolumeUnit::g_Per_m3);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarNeg1To1.h>

//!
//! TYPE ScalarNeg1To1
//! static void UnMarshall(const CDM::ScalarNeg1To1Data& in, SEScalarNeg1To1& out);
//! static void Marshall(const SEScalarNeg1To1& in, CDM::ScalarNeg1To1Data& out);
TEST_F(TEST_FIXTURE_NAME, Neg1To1)
{  
  USING_TYPES(ScalarNeg1To1)

  SEType source, sink;
  CDMType data;

  source.SetValue(0.14159);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarOsmolality.h>

//!
//! TYPE ScalarOsmolality
//! static void UnMarshall(const CDM::ScalarOsmolalityData& in, SEScalarOsmolality& out);
//! static void Marshall(const SEScalarOsmolality& in, CDM::ScalarOsmolalityData& out);
TEST_F(TEST_FIXTURE_NAME, Osmolality)
{ 
  USING_TYPES(ScalarOsmolality)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::OsmolalityUnit::Osm_Per_kg);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarOsmolarity.h>

//!
//! TYPE ScalarOsmolarity
//! static void UnMarshall(const CDM::ScalarOsmolarityData& in, SEScalarOsmolarity& out);
//! static void Marshall(const SEScalarOsmolarity& in, CDM::ScalarOsmolarityData& out);
TEST_F(TEST_FIXTURE_NAME, Osmolarity)
{ 
  USING_TYPES(ScalarOsmolarity)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::OsmolarityUnit::Osm_Per_L);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPower.h>

//!
//! TYPE ScalarPower
//! static void UnMarshall(const CDM::ScalarPowerData& in, SEScalarPower& out);
//! static void Marshall(const SEScalarPower& in, CDM::ScalarPowerData& out);
TEST_F(TEST_FIXTURE_NAME, Power)
{  
  USING_TYPES(ScalarPower)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PowerUnit::BTU_Per_hr);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h>

//!
//! TYPE ScalarPowerPerAreaTemperatureToTheFourth
//! static void UnMarshall(const CDM::ScalarPowerPerAreaTemperatureToTheFourthData& in, SEScalarPowerPerAreaTemperatureToTheFourth& out);
//! static void Marshall(const SEScalarPowerPerAreaTemperatureToTheFourth& in, CDM::ScalarPowerPerAreaTemperatureToTheFourthData& out);
TEST_F(TEST_FIXTURE_NAME, PowerPerAreaTemperatureToTheFourth)
{   
  USING_TYPES(ScalarPowerPerAreaTemperatureToTheFourth)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PowerPerAreaTemperatureToTheFourthUnit::W_Per_m2_K4);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPressure.h>

//!
//! TYPE ScalarPressure
//! static void UnMarshall(const CDM::ScalarPressureData& in, SEScalarPressure& out);
//! static void Marshall(const SEScalarPressure& in, CDM::ScalarPressureData& out);
TEST_F(TEST_FIXTURE_NAME, Pressure)
{  
  USING_TYPES(ScalarPressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PressureUnit::cmH2O);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPressurePerVolume.h>

//!
//! TYPE ScalarPressurePerVolume
//! static void UnMarshall(const CDM::ScalarPressurePerVolumeData& in, SEScalarPressurePerVolume& out);
//! static void Marshall(const SEScalarPressurePerVolume& in, CDM::ScalarPressurePerVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, PressurePerVolume)
{  
  USING_TYPES(ScalarPressurePerVolume)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PressurePerVolumeUnit::cmH2O_Per_mL);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPressureTimePerArea.h>

//!
//! TYPE ScaMlarPressureTimePerArea
//! static void UnMarshall(const CDM::ScalarPressureTimePerAreaData& in, SEScalarPressureTimePerArea& out);
//! static void Marshall(const SEScalarPressureTimePerArea& in, CDM::ScalarPressureTimePerAreaData& out);
TEST_F(TEST_FIXTURE_NAME, PressureTimePerArea)
{   
  USING_TYPES(ScalarPressureTimePerArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PressureTimePerAreaUnit::cmH2O_Per_mL_m2);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPressureTimePerVolumeArea.h>

//!
//! TYPE ScalarPressureTimePerVolumeArea
//! static void UnMarshall(const CDM::ScalarPressureTimePerVolumeAreaData& in, SEScalarPressureTimePerVolumeArea& out);
//! static void Marshall(const SEScalarPressureTimePerVolumeArea& in, CDM::ScalarPressureTimePerVolumeAreaData& out);
TEST_F(TEST_FIXTURE_NAME, PressureTimePerVolumeArea)
{     
  USING_TYPES(ScalarPressureTimePerVolumeArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PressureTimePerVolumeAreaUnit::dyn_s_Per_cm5_m2);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarTemperature.h>

//!
//! TYPE ScalarTemperature
//! static void UnMarshall(const CDM::ScalarTemperatureData& in, SEScalarTemperature& out);
//! static void Marshall(const SEScalarTemperature& in, CDM::ScalarTemperatureData& out);
TEST_F(TEST_FIXTURE_NAME, Temperature)
{  
  USING_TYPES(ScalarTemperature)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::TemperatureUnit::F);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarTime.h>

//!
//! TYPE ScalarTime
//! static void UnMarshall(const CDM::ScalarTimeData& in, SEScalarTime& out);
//! static void Marshall(const SEScalarTime& in, CDM::ScalarTimeData& out);
TEST_F(TEST_FIXTURE_NAME, Time)
{    
  USING_TYPES(ScalarTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::TimeUnit::yr);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolume.h>

//!
//! TYPE ScalarVolume
//! static void UnMarshall(const CDM::ScalarVolumeData& in, SEScalarVolume& out);
//! static void Marshall(const SEScalarVolume& in, CDM::ScalarVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, Volume)
{  
  USING_TYPES(ScalarVolume)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumeUnit::m3);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerPressure.h>

//!
//! TYPE ScalarVolumePerPressure
//! static void UnMarshall(const CDM::ScalarVolumePerPressureData& in, SEScalarVolumePerPressure& out);
//! static void Marshall(const SEScalarVolumePerPressure& in, CDM::ScalarVolumePerPressureData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerPressure)
{   
  USING_TYPES(ScalarVolumePerPressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerPressureUnit::L_Per_Pa);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerTimeArea.h>

//!
//! TYPE ScalarVolumePerTimeArea
//! static void UnMarshall(const CDM::ScalarVolumePerTimeAreaData& in, SEScalarVolumePerTimeArea& out);
//! static void Marshall(const SEScalarVolumePerTimeArea& in, CDM::ScalarVolumePerTimeAreaData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerTimeArea)
{   
  USING_TYPES(ScalarVolumePerTimeArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerTimeAreaUnit::mL_Per_min_m2);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

//!
//! TYPE ScalarVolumePerTime
//! static void UnMarshall(const CDM::ScalarVolumePerTimeData& in, SEScalarVolumePerTime& out);
//! static void Marshall(const SEScalarVolumePerTime& in, CDM::ScalarVolumePerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerTime)
{    
  USING_TYPES(ScalarVolumePerTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerTimeUnit::L_Per_min);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>

//!
//! TYPE ScalarVolumePerTimeMass
//! static void UnMarshall(const CDM::ScalarVolumePerTimeMassData& in, SEScalarVolumePerTimeMass& out);
//! static void Marshall(const SEScalarVolumePerTimeMass& in, CDM::ScalarVolumePerTimeMassData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerTimeMass)
{    
  USING_TYPES(ScalarVolumePerTimeMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerTimeMassUnit::mL_Per_s_kg);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>

//!
//! TYPE ScalarVolumePerTimePressureArea
//! static void UnMarshall(const CDM::ScalarVolumePerTimePressureAreaData& in, SEScalarVolumePerTimePressureArea& out);
//! static void Marshall(const SEScalarVolumePerTimePressureArea& in, CDM::ScalarVolumePerTimePressureAreaData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerTimePressureArea)
{    
  USING_TYPES(ScalarVolumePerTimePressureArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>

//!
//! TYPE ScalarVolumePerTimePressure
//! static void UnMarshall(const CDM::ScalarVolumePerTimePressureData& in, SEScalarVolumePerTimePressure& out);
//! static void Marshall(const SEScalarVolumePerTimePressure& in, CDM::ScalarVolumePerTimePressureData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerTimePressure)
{     
  USING_TYPES(ScalarVolumePerTimePressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerTimePressureUnit::mL_Per_min_mmHg);

  EXPECT_NE(source, sink);

  Property::Marshall(source, data);
  Property::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}