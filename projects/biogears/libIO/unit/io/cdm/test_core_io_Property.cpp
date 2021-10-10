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
#define TEST_FIXTURE_NAME PropertiesFixture
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

#include <biogears/cdm/properties/SEScalarTypes.h>

#include <biogears/cdm/properties/SEDecimalFormat.h>
//!
//!DecimalFormat;
//! static void Marshall(const CDM::DecimalFormatData& in, SEDecimalFormat& out);
//! static void UnMarshall(const SEDecimalFormat& in, CDM::DecimalFormatData& out);
TEST_F(TEST_FIXTURE_NAME, DecimalFormat)
{

  USING_TYPES(DecimalFormat)

  SEType source, sink;
  CDMType data;

  source.SetNotation(biogears::DecimalNotation::Fixed);
  source.SetPrecision(3);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.h>
#include <biogears/cdm/properties/SEScalarElectricPotential.h>
//! TYPE FunctionElectricPotentialVsTime
//! static void Marshall(const CDM::FunctionElectricPotentialVsTimeData& in, SEFunctionElectricPotentialVsTime& out);
//! static void UnMarshall(const SEFunctionElectricPotentialVsTime& in, CDM::FunctionElectricPotentialVsTimeData& out);
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

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.h>
//!
//! TYPE Function;
//! static void Marshall(const CDM::FunctionData& in, SEFunction& out);
//! static void UnMarshall(const SEFunction& in, CDM::FunctionData& out);
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

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
//!
//! TYPE FunctionVolumeVsTime;
//! static void Marshall(const CDM::FunctionVolumeVsTimeData& in, SEFunctionVolumeVsTime& out);
//! static void UnMarshall(const SEFunctionVolumeVsTime& in, CDM::FunctionVolumeVsTimeData& out);
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

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
//!
//! TYPE HistogramFractionVsLength;
//! static void Marshall(const CDM::HistogramFractionVsLengthData& in, SEHistogramFractionVsLength& out);
//! static void UnMarshall(const SEHistogramFractionVsLength& in, CDM::HistogramFractionVsLengthData& out);
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

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEHistogram.h>
//!
//! TYPE Histogram
//! static void Marshall(const CDM::HistogramData& in, SEHistogram& out);
//! static void UnMarshall(const SEHistogram& in, CDM::HistogramData& out);
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

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalar0To1.h>
//!
//! TYPE Scalar0To1
//! static void Marshall(const CDM::Scalar0To1Data& in, SEScalar0To1& out);
//! static void UnMarshall(const SEScalar0To1& in, CDM::Scalar0To1Data& out);
TEST_F(TEST_FIXTURE_NAME, 0To1)
{
  USING_TYPES(Scalar0To1)

  SEType source, sink;
  CDMType data;

  source.SetValue(0.14159);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarAmount.h>
//!
//! TYPE ScalarAmount
//! static void Marshall(const CDM::ScalarAmountData& in, SEScalarAmount& out);
//! static void UnMarshall(const SEScalarAmount& in, CDM::ScalarAmountData& out);
TEST_F(TEST_FIXTURE_NAME, Amount)
{
  USING_TYPES(ScalarAmount)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AmountUnit::pmol);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/properties/SEScalarAmountPerMass.h>
//!
//! TYPE ScalarAmountPerMass
//! static void Marshall(const CDM::ScalarAmountPerMassData& in, SEScalarAmountPerMass& out);
//! static void UnMarshall(const SEScalarAmountPerMass& in, CDM::ScalarAmountPerMassData& out);
TEST_F(TEST_FIXTURE_NAME, AmountPerMass)
{
  USING_TYPES(ScalarAmountPerMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AmountPerMassUnit::ct_Per_ug);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
//!
//! TYPE ScalarAmountPerTime
//! static void Marshall(const CDM::ScalarAmountPerTimeData& in, SEScalarAmountPerTime& out);
//! static void UnMarshall(const SEScalarAmountPerTime& in, CDM::ScalarAmountPerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, AmountPerTime)
{
  USING_TYPES(ScalarAmountPerTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AmountPerTimeUnit::pmol_Per_min);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
//!
//! TYPE ScalarAmountPerVolume
//! static void Marshall(const CDM::ScalarAmountPerVolumeData& in, SEScalarAmountPerVolume& out);
//! static void UnMarshall(const SEScalarAmountPerVolume& in, CDM::ScalarAmountPerVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, AmountPerVolume)
{
  USING_TYPES(ScalarAmountPerVolume)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AmountPerVolumeUnit::ct_Per_uL);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarArea.h>
//!
//! TYPE ScalarArea
//! static void Marshall(const CDM::ScalarAreaData& in, SEScalarArea& out);
//! static void UnMarshall(const SEScalarArea& in, CDM::ScalarAreaData& out);
TEST_F(TEST_FIXTURE_NAME, Area)
{
  USING_TYPES(ScalarArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AreaUnit::m2);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>
//!
//! TYPE ScalarAreaPerTimePressure
//! static void Marshall(const CDM::ScalarAreaPerTimePressureData& in, SEScalarAreaPerTimePressure& out);
//! static void UnMarshall(const SEScalarAreaPerTimePressure& in, CDM::ScalarAreaPerTimePressureData& out);
TEST_F(TEST_FIXTURE_NAME, AreaPerTimePressure)
{
  USING_TYPES(ScalarAreaPerTimePressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::AreaPerTimePressureUnit::m2_Per_min_mmHg);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricCapacitance.h>

//!
//! TYPE ScalarElectricCapacitance
//! static void Marshall(const CDM::ScalarElectricCapacitanceData& in, SEScalarElectricCapacitance& out);
//! static void UnMarshall(const SEScalarElectricCapacitance& in, CDM::ScalarElectricCapacitanceData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricCapacitance)
{ 
  USING_TYPES(ScalarElectricCapacitance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricCapacitanceUnit::F);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricCharge.h>

//!
//! TYPE ScalarElectricCharge
//! static void Marshall(const CDM::ScalarElectricChargeData& in, SEScalarElectricCharge& out);
//! static void UnMarshall(const SEScalarElectricCharge& in, CDM::ScalarElectricChargeData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricCharge)
{   
  USING_TYPES(ScalarElectricCharge)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricChargeUnit::C);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricCurrent.h>

//!
//! TYPE ScalarElectricCurrent
//! static void Marshall(const CDM::ScalarElectricCurrentData& in, SEScalarElectricCurrent& out);
//! static void UnMarshall(const SEScalarElectricCurrent& in, CDM::ScalarElectricCurrentData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricCurrent)
{      
  USING_TYPES(ScalarElectricCurrent)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricCurrentUnit::A);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricInductance.h>

//!
//! TYPE ScalarElectricInductance
//! static void Marshall(const CDM::ScalarElectricInductanceData& in, SEScalarElectricInductance& out);
//! static void UnMarshall(const SEScalarElectricInductance& in, CDM::ScalarElectricInductanceData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricInductance)
{       
  USING_TYPES(ScalarElectricInductance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricInductanceUnit::H);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricPotential.h>

//!
//! TYPE ScalarElectricPotential
//! static void Marshall(const CDM::ScalarElectricPotentialData& in, SEScalarElectricPotential& out);
//! static void UnMarshall(const SEScalarElectricPotential& in, CDM::ScalarElectricPotentialData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricPotential)
{     
  USING_TYPES(ScalarElectricPotential)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricPotentialUnit::mV);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarElectricResistance.h>

//!
//! TYPE ScalarElectricResistance
//! static void Marshall(const CDM::ScalarElectricResistanceData& in, SEScalarElectricResistance& out);
//! static void UnMarshall(const SEScalarElectricResistance& in, CDM::ScalarElectricResistanceData& out);
TEST_F(TEST_FIXTURE_NAME, ElectricResistance)
{    
  USING_TYPES(ScalarElectricResistance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ElectricResistanceUnit::Ohm);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarEnergy.h>

//!
//! TYPE ScalarEnergy
//! static void Marshall(const CDM::ScalarEnergyData& in, SEScalarEnergy& out);
//! static void UnMarshall(const SEScalarEnergy& in, CDM::ScalarEnergyData& out);
TEST_F(TEST_FIXTURE_NAME, Energy)
{     
  USING_TYPES(ScalarEnergy)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::EnergyUnit::kJ);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>

//!
//! TYPE ScalarEnergyPerAmount
//! static void Marshall(const CDM::ScalarEnergyPerAmountData& in, SEScalarEnergyPerAmount& out);
//! static void UnMarshall(const SEScalarEnergyPerAmount& in, CDM::ScalarEnergyPerAmountData& out);
TEST_F(TEST_FIXTURE_NAME, EnergyPerAmount)
{     
  USING_TYPES(ScalarEnergyPerAmount)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::EnergyPerAmountUnit::kJ_Per_mol);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarEnergyPerMass.h>

//!
//! TYPE ScalarEnergyPerMass
//! static void Marshall(const CDM::ScalarEnergyPerMassData& in, SEScalarEnergyPerMass& out);
//! static void UnMarshall(const SEScalarEnergyPerMass& in, CDM::ScalarEnergyPerMassData& out);
TEST_F(TEST_FIXTURE_NAME, EnergyPerMass)
{   
  USING_TYPES(ScalarEnergyPerMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::EnergyPerMassUnit::kcal_Per_kg);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>

//!
//! TYPE ScalarFlowCompliance
//! static void Marshall(const CDM::ScalarFlowComplianceData& in, SEScalarFlowCompliance& out);
//! static void UnMarshall(const SEScalarFlowCompliance& in, CDM::ScalarFlowComplianceData& out);
TEST_F(TEST_FIXTURE_NAME, FlowCompliance)
{   
  USING_TYPES(ScalarFlowCompliance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::FlowComplianceUnit::mL_Per_cmH2O);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFlowElastance.h>

//!
//! TYPE ScalarFlowElastance
//! static void Marshall(const CDM::ScalarFlowElastanceData& in, SEScalarFlowElastance& out);
//! static void UnMarshall(const SEScalarFlowElastance& in, CDM::ScalarFlowElastanceData& out);
TEST_F(TEST_FIXTURE_NAME, FlowElastance)
{   
  USING_TYPES(ScalarFlowElastance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::FlowElastanceUnit::mmHg_Per_mL);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFlowInertance.h>

//!
//! TYPE ScalarFlowInertance
//! static void Marshall(const CDM::ScalarFlowInertanceData& in, SEScalarFlowInertance& out);
//! static void UnMarshall(const SEScalarFlowInertance& in, CDM::ScalarFlowInertanceData& out);
TEST_F(TEST_FIXTURE_NAME, FlowInertance)
{    
  USING_TYPES(ScalarFlowInertance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::FlowInertanceUnit::cmH2O_s2_Per_mL);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFlowResistance.h>

//!
//! TYPE ScalarFlowResistance
//! static void Marshall(const CDM::ScalarFlowResistanceData& in, SEScalarFlowResistance& out);
//! static void UnMarshall(const SEScalarFlowResistance& in, CDM::ScalarFlowResistanceData& out);
TEST_F(TEST_FIXTURE_NAME, FlowResistance)
{    
  USING_TYPES(ScalarFlowResistance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::FlowResistanceUnit::cmH2O_s_Per_L);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarForce.h>

//!
//! TYPE ScalarForce
//! static void Marshall(const CDM::ScalarForceData& in, SEScalarForce& out);
//! static void UnMarshall(const SEScalarForce& in, CDM::ScalarForceData& out);
TEST_F(TEST_FIXTURE_NAME, Force)
{    
  USING_TYPES(ScalarForce)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::ForceUnit::dyn);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFraction.h>

//!
//! TYPE ScalarFraction
//! static void Marshall(const CDM::ScalarFractionData& in, SEScalarFraction& out);
//! static void UnMarshall(const SEScalarFraction& in, CDM::ScalarFractionData& out);
TEST_F(TEST_FIXTURE_NAME, Fraction)
{   
  USING_TYPES(ScalarFraction)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarFrequency.h>

//!
//! TYPE ScalarFrequency
//! static void Marshall(const CDM::ScalarFrequencyData& in, SEScalarFrequency& out);
//! static void UnMarshall(const SEScalarFrequency& in, CDM::ScalarFrequencyData& out);
TEST_F(TEST_FIXTURE_NAME, Frequency)
{  
  USING_TYPES(ScalarFrequency)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::FrequencyUnit::Per_hr);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalar.h>

//!
//! TYPE Scalar
//! static void Marshall(const CDM::ScalarData& in, SEScalar& out);
//! static void UnMarshall(const SEScalar& in, CDM::ScalarData& out);
TEST_F(TEST_FIXTURE_NAME, Scalar)
{   
  USING_TYPES(Scalar)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>

//!
//! TYPE ScalarHeatCapacitance
//! static void Marshall(const CDM::ScalarHeatCapacitanceData& in, SEScalarHeatCapacitance& out);
//! static void UnMarshall(const SEScalarHeatCapacitance& in, CDM::ScalarHeatCapacitanceData& out);
TEST_F(TEST_FIXTURE_NAME, HeatCapacitance)
{   
  USING_TYPES(ScalarHeatCapacitance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatCapacitanceUnit::kcal_Per_C);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerAmount.h>

//!
//! TYPE ScalarHeatCapacitancePerAmount
//! static void Marshall(const CDM::ScalarHeatCapacitancePerAmountData& in, SEScalarHeatCapacitancePerAmount& out);
//! static void UnMarshall(const SEScalarHeatCapacitancePerAmount& in, CDM::ScalarHeatCapacitancePerAmountData& out);
TEST_F(TEST_FIXTURE_NAME, HeatCapacitancePerAmount)
{  
  USING_TYPES(ScalarHeatCapacitancePerAmount)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatCapacitancePerAmountUnit::J_Per_K_mol);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>

//!
//! TYPE ScalarHeatCapacitancePerMass
//! static void Marshall(const CDM::ScalarHeatCapacitancePerMassData& in, SEScalarHeatCapacitancePerMass& out);
//! static void UnMarshall(const SEScalarHeatCapacitancePerMass& in, CDM::ScalarHeatCapacitancePerMassData& out);
TEST_F(TEST_FIXTURE_NAME, HeatCapacitancePerMass)
{  
  USING_TYPES(ScalarHeatCapacitancePerMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatCapacitancePerMassUnit::kcal_Per_C_kg);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatConductance.h>

//!
//! TYPE ScalarHeatConductance
//! static void Marshall(const CDM::ScalarHeatConductanceData& in, SEScalarHeatConductance& out);
//! static void UnMarshall(const SEScalarHeatConductance& in, CDM::ScalarHeatConductanceData& out);
TEST_F(TEST_FIXTURE_NAME, HeatConductance)
{   
  USING_TYPES(ScalarHeatConductance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatConductanceUnit::kcal_Per_K_s);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>

//!
//! TYPE ScalarHeatConductancePerArea
//! static void Marshall(const CDM::ScalarHeatConductancePerAreaData& in, SEScalarHeatConductancePerArea& out);
//! static void UnMarshall(const SEScalarHeatConductancePerArea& in, CDM::ScalarHeatConductancePerAreaData& out);
TEST_F(TEST_FIXTURE_NAME, HeatConductancePerArea)
{  
  USING_TYPES(ScalarHeatConductancePerArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatConductancePerAreaUnit::BTU_Per_hr_ft2_C);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatInductance.h>

//!
//! TYPE ScalarHeatInductance
//! static void Marshall(const CDM::ScalarHeatInductanceData& in, SEScalarHeatInductance& out);
//! static void UnMarshall(const SEScalarHeatInductance& in, CDM::ScalarHeatInductanceData& out);
TEST_F(TEST_FIXTURE_NAME, HeatInductance)
{   
  USING_TYPES(ScalarHeatInductance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatInductanceUnit::K_s_Per_W);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>

//!
//! TYPE ScalarHeatResistanceArea
//! static void Marshall(const CDM::ScalarHeatResistanceAreaData& in, SEScalarHeatResistanceArea& out);
//! static void UnMarshall(const SEScalarHeatResistanceArea& in, CDM::ScalarHeatResistanceAreaData& out);
TEST_F(TEST_FIXTURE_NAME, HeatResistanceArea)
{ 
  USING_TYPES(ScalarHeatResistanceArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatResistanceAreaUnit::tog);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarHeatResistance.h>

//!
//! TYPE ScalarHeatResistance
//! static void Marshall(const CDM::ScalarHeatResistanceData& in, SEScalarHeatResistance& out);
//! static void UnMarshall(const SEScalarHeatResistance& in, CDM::ScalarHeatResistanceData& out);
TEST_F(TEST_FIXTURE_NAME, HeatResistance)
{     
  USING_TYPES(ScalarHeatResistance)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::HeatResistanceUnit::C_s_Per_kcal);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarInversePressure.h>

//!
//! TYPE ScalarInversePressure
//! static void Marshall(const CDM::ScalarInversePressureData& in, SEScalarInversePressure& out);
//! static void UnMarshall(const SEScalarInversePressure& in, CDM::ScalarInversePressureData& out);
TEST_F(TEST_FIXTURE_NAME, InversePressure)
{   
  USING_TYPES(ScalarInversePressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::InversePressureUnit::Inverse_mmHg);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarInverseVolume.h>

//!
//! TYPE ScalarInverseVolume
//! static void Marshall(const CDM::ScalarInverseVolumeData& in, SEScalarInverseVolume& out);
//! static void UnMarshall(const SEScalarInverseVolume& in, CDM::ScalarInverseVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, InverseVolume)
{ 
  USING_TYPES(ScalarInverseVolume)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::InverseVolumeUnit::Inverse_L);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarLength.h>

//!
//! TYPE ScalarLength
//! static void Marshall(const CDM::ScalarLengthData& in, SEScalarLength& out);
//! static void UnMarshall(const SEScalarLength& in, CDM::ScalarLengthData& out);
TEST_F(TEST_FIXTURE_NAME, Length)
{    
  USING_TYPES(ScalarLength)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::LengthUnit::ft);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>

//!
//! TYPE ScalarLengthPerTime
//! static void Marshall(const CDM::ScalarLengthPerTimeData& in, SEScalarLengthPerTime& out);
//! static void UnMarshall(const SEScalarLengthPerTime& in, CDM::ScalarLengthPerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, LengthPerTime)
{  
  USING_TYPES(ScalarLengthPerTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::LengthPerTimeUnit::cm_Per_s);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarLengthPerTimePressure.h>

//!
//! TYPE ScalarLengthPerTimePressure
//! static void Marshall(const CDM::ScalarLengthPerTimePressureData& in, SEScalarLengthPerTimePressure& out);
//! static void UnMarshall(const SEScalarLengthPerTimePressure& in, CDM::ScalarLengthPerTimePressureData& out);
TEST_F(TEST_FIXTURE_NAME, LengthPerTimePressure)
{  
  USING_TYPES(ScalarLengthPerTimePressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::LengthPerTimePressureUnit::cm_Per_s_mmHg);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarMass.h>

//!
//! TYPE ScalarMass
//! static void Marshall(const CDM::ScalarMassData& in, SEScalarMass& out);
//! static void UnMarshall(const SEScalarMass& in, CDM::ScalarMassData& out);
TEST_F(TEST_FIXTURE_NAME, Mass)
{  
  USING_TYPES(ScalarMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassUnit::kg);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>

//!
//! TYPE ScalarMassPerAmount
//! static void Marshall(const CDM::ScalarMassPerAmountData& in, SEScalarMassPerAmount& out);
//! static void UnMarshall(const SEScalarMassPerAmount& in, CDM::ScalarMassPerAmountData& out);
TEST_F(TEST_FIXTURE_NAME, MassPerAmount)
{  
  USING_TYPES(ScalarMassPerAmount)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassPerAmountUnit::g_Per_mol);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarArea.h>

//!
//! TYPE ScalarMassPerAreaTime
//! static void Marshall(const CDM::ScalarMassPerAreaTimeData& in, SEScalarMassPerAreaTime& out);
//! static void UnMarshall(const SEScalarMassPerAreaTime& in, CDM::ScalarMassPerAreaTimeData& out);
TEST_F(TEST_FIXTURE_NAME, MassPerAreaTime)
{   
  USING_TYPES(ScalarMassPerAreaTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassPerAreaTimeUnit::g_Per_cm2_s);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarMassPerMass.h>

//!
//! TYPE ScalarMassPerMass
//! static void Marshall(const CDM::ScalarMassPerMassData& in, SEScalarMassPerMass& out);
//! static void UnMarshall(const SEScalarMassPerMass& in, CDM::ScalarMassPerMassData& out);
TEST_F(TEST_FIXTURE_NAME, MassPerMass)
{  
  USING_TYPES(ScalarMassPerMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassPerMassUnit::mg_Per_g);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarMassPerTime.h>

//!
//! TYPE ScalarMassPerTime
//! static void Marshall(const CDM::ScalarMassPerTimeData& in, SEScalarMassPerTime& out);
//! static void UnMarshall(const SEScalarMassPerTime& in, CDM::ScalarMassPerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, MassPerTime)
{   
  USING_TYPES(ScalarMassPerTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassPerTimeUnit::g_Per_min);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>

//!
//! TYPE ScalarMassPerVolume
//! static void Marshall(const CDM::ScalarMassPerVolumeData& in, SEScalarMassPerVolume& out);
//! static void UnMarshall(const SEScalarMassPerVolume& in, CDM::ScalarMassPerVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, MassPerVolume)
{  
  USING_TYPES(ScalarMassPerVolume)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::MassPerVolumeUnit::g_Per_m3);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarNeg1To1.h>

//!
//! TYPE ScalarNeg1To1
//! static void Marshall(const CDM::ScalarNeg1To1Data& in, SEScalarNeg1To1& out);
//! static void UnMarshall(const SEScalarNeg1To1& in, CDM::ScalarNeg1To1Data& out);
TEST_F(TEST_FIXTURE_NAME, Neg1To1)
{  
  USING_TYPES(ScalarNeg1To1)

  SEType source, sink;
  CDMType data;

  source.SetValue(0.14159);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarOsmolality.h>

//!
//! TYPE ScalarOsmolality
//! static void Marshall(const CDM::ScalarOsmolalityData& in, SEScalarOsmolality& out);
//! static void UnMarshall(const SEScalarOsmolality& in, CDM::ScalarOsmolalityData& out);
TEST_F(TEST_FIXTURE_NAME, Osmolality)
{ 
  USING_TYPES(ScalarOsmolality)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::OsmolalityUnit::Osm_Per_kg);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarOsmolarity.h>

//!
//! TYPE ScalarOsmolarity
//! static void Marshall(const CDM::ScalarOsmolarityData& in, SEScalarOsmolarity& out);
//! static void UnMarshall(const SEScalarOsmolarity& in, CDM::ScalarOsmolarityData& out);
TEST_F(TEST_FIXTURE_NAME, Osmolarity)
{ 
  USING_TYPES(ScalarOsmolarity)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::OsmolarityUnit::Osm_Per_L);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPower.h>

//!
//! TYPE ScalarPower
//! static void Marshall(const CDM::ScalarPowerData& in, SEScalarPower& out);
//! static void UnMarshall(const SEScalarPower& in, CDM::ScalarPowerData& out);
TEST_F(TEST_FIXTURE_NAME, Power)
{  
  USING_TYPES(ScalarPower)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PowerUnit::BTU_Per_hr);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h>

//!
//! TYPE ScalarPowerPerAreaTemperatureToTheFourth
//! static void Marshall(const CDM::ScalarPowerPerAreaTemperatureToTheFourthData& in, SEScalarPowerPerAreaTemperatureToTheFourth& out);
//! static void UnMarshall(const SEScalarPowerPerAreaTemperatureToTheFourth& in, CDM::ScalarPowerPerAreaTemperatureToTheFourthData& out);
TEST_F(TEST_FIXTURE_NAME, PowerPerAreaTemperatureToTheFourth)
{   
  USING_TYPES(ScalarPowerPerAreaTemperatureToTheFourth)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PowerPerAreaTemperatureToTheFourthUnit::W_Per_m2_K4);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPressure.h>

//!
//! TYPE ScalarPressure
//! static void Marshall(const CDM::ScalarPressureData& in, SEScalarPressure& out);
//! static void UnMarshall(const SEScalarPressure& in, CDM::ScalarPressureData& out);
TEST_F(TEST_FIXTURE_NAME, Pressure)
{  
  USING_TYPES(ScalarPressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PressureUnit::cmH2O);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPressurePerVolume.h>

//!
//! TYPE ScalarPressurePerVolume
//! static void Marshall(const CDM::ScalarPressurePerVolumeData& in, SEScalarPressurePerVolume& out);
//! static void UnMarshall(const SEScalarPressurePerVolume& in, CDM::ScalarPressurePerVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, PressurePerVolume)
{  
  USING_TYPES(ScalarPressurePerVolume)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PressurePerVolumeUnit::cmH2O_Per_mL);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPressureTimePerArea.h>

//!
//! TYPE ScaMlarPressureTimePerArea
//! static void Marshall(const CDM::ScalarPressureTimePerAreaData& in, SEScalarPressureTimePerArea& out);
//! static void UnMarshall(const SEScalarPressureTimePerArea& in, CDM::ScalarPressureTimePerAreaData& out);
TEST_F(TEST_FIXTURE_NAME, PressureTimePerArea)
{   
  USING_TYPES(ScalarPressureTimePerArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PressureTimePerAreaUnit::cmH2O_Per_mL_m2);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarPressureTimePerVolumeArea.h>

//!
//! TYPE ScalarPressureTimePerVolumeArea
//! static void Marshall(const CDM::ScalarPressureTimePerVolumeAreaData& in, SEScalarPressureTimePerVolumeArea& out);
//! static void UnMarshall(const SEScalarPressureTimePerVolumeArea& in, CDM::ScalarPressureTimePerVolumeAreaData& out);
TEST_F(TEST_FIXTURE_NAME, PressureTimePerVolumeArea)
{     
  USING_TYPES(ScalarPressureTimePerVolumeArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::PressureTimePerVolumeAreaUnit::dyn_s_Per_cm5_m2);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarTemperature.h>

//!
//! TYPE ScalarTemperature
//! static void Marshall(const CDM::ScalarTemperatureData& in, SEScalarTemperature& out);
//! static void UnMarshall(const SEScalarTemperature& in, CDM::ScalarTemperatureData& out);
TEST_F(TEST_FIXTURE_NAME, Temperature)
{  
  USING_TYPES(ScalarTemperature)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::TemperatureUnit::F);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarTime.h>

//!
//! TYPE ScalarTime
//! static void Marshall(const CDM::ScalarTimeData& in, SEScalarTime& out);
//! static void UnMarshall(const SEScalarTime& in, CDM::ScalarTimeData& out);
TEST_F(TEST_FIXTURE_NAME, Time)
{    
  USING_TYPES(ScalarTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::TimeUnit::yr);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolume.h>

//!
//! TYPE ScalarVolume
//! static void Marshall(const CDM::ScalarVolumeData& in, SEScalarVolume& out);
//! static void UnMarshall(const SEScalarVolume& in, CDM::ScalarVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, Volume)
{  
  USING_TYPES(ScalarVolume)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumeUnit::m3);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerPressure.h>

//!
//! TYPE ScalarVolumePerPressure
//! static void Marshall(const CDM::ScalarVolumePerPressureData& in, SEScalarVolumePerPressure& out);
//! static void UnMarshall(const SEScalarVolumePerPressure& in, CDM::ScalarVolumePerPressureData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerPressure)
{   
  USING_TYPES(ScalarVolumePerPressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerPressureUnit::L_Per_Pa);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerTimeArea.h>

//!
//! TYPE ScalarVolumePerTimeArea
//! static void Marshall(const CDM::ScalarVolumePerTimeAreaData& in, SEScalarVolumePerTimeArea& out);
//! static void UnMarshall(const SEScalarVolumePerTimeArea& in, CDM::ScalarVolumePerTimeAreaData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerTimeArea)
{   
  USING_TYPES(ScalarVolumePerTimeArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerTimeAreaUnit::mL_Per_min_m2);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

//!
//! TYPE ScalarVolumePerTime
//! static void Marshall(const CDM::ScalarVolumePerTimeData& in, SEScalarVolumePerTime& out);
//! static void UnMarshall(const SEScalarVolumePerTime& in, CDM::ScalarVolumePerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerTime)
{    
  USING_TYPES(ScalarVolumePerTime)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerTimeUnit::L_Per_min);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>

//!
//! TYPE ScalarVolumePerTimeMass
//! static void Marshall(const CDM::ScalarVolumePerTimeMassData& in, SEScalarVolumePerTimeMass& out);
//! static void UnMarshall(const SEScalarVolumePerTimeMass& in, CDM::ScalarVolumePerTimeMassData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerTimeMass)
{    
  USING_TYPES(ScalarVolumePerTimeMass)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerTimeMassUnit::mL_Per_s_kg);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>

//!
//! TYPE ScalarVolumePerTimePressureArea
//! static void Marshall(const CDM::ScalarVolumePerTimePressureAreaData& in, SEScalarVolumePerTimePressureArea& out);
//! static void UnMarshall(const SEScalarVolumePerTimePressureArea& in, CDM::ScalarVolumePerTimePressureAreaData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerTimePressureArea)
{    
  USING_TYPES(ScalarVolumePerTimePressureArea)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>

//!
//! TYPE ScalarVolumePerTimePressure
//! static void Marshall(const CDM::ScalarVolumePerTimePressureData& in, SEScalarVolumePerTimePressure& out);
//! static void UnMarshall(const SEScalarVolumePerTimePressure& in, CDM::ScalarVolumePerTimePressureData& out);
TEST_F(TEST_FIXTURE_NAME, VolumePerTimePressure)
{     
  USING_TYPES(ScalarVolumePerTimePressure)

  SEType source, sink;
  CDMType data;

  source.SetValue(3.14159, biogears::VolumePerTimePressureUnit::mL_Per_min_mmHg);

  EXPECT_NE(source, sink);

  Property::UnMarshall(source, data);
  Property::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
