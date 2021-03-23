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

#include <biogears/filesystem/path.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_io_Property_TEST
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

#define USING_TYPES(name)  \
  using namespace biogears::io; \
  using SEType = biogears::SE ## name; \
  using CDMType = CDM:: ## name ## Data;

//!
//!DecimalFormat;
//!//static void Marshall(const CDM::DecimalFormatData& in, SEDecimalFormat& out);
//!//static void UnMarshall(const SEDecimalFormat& in, CDM::DecimalFormatData& out);
TEST_F(TEST_FIXTURE_NAME, SEDecimalFormat)
{

  USING_TYPES(DecimalFormat)

  SEType source, sink;
  CDMType data;

  source.SetNotation(biogears::DecimalNotation::Fixed);
  source.SetPrecision(3);
  
  EXPECT_NE(source.GetPrecision(), sink.GetPrecision());
  EXPECT_NE(source.GetNotation(), sink.GetNotation());

  Property::UnMarshall(source, data);
  Property::Marshall(data, source);

  EXPECT_EQ(source.GetPrecision(), sink.GetPrecision());
  EXPECT_EQ(source.GetNotation(), sink.GetNotation());
}

//!
//! TYPE FunctionElectricPotentialVsTime
//!//static void Marshall(const CDM::FunctionElectricPotentialVsTimeData& in, SEFunctionElectricPotentialVsTime& out);
//!//static void UnMarshall(const SEFunctionElectricPotentialVsTime& in, CDM::FunctionElectricPotentialVsTimeData& out);
TEST_F(TEST_FIXTURE_NAME, SEFunctionElectricPotentialVsTime)
{
}

//!
//! TYPE Function;
//!//static void Marshall(const CDM::FunctionData& in, SEFunction& out);
//!//static void UnMarshall(const SEFunction& in, CDM::FunctionData& out);
TEST_F(TEST_FIXTURE_NAME, SEFunction)
{
}

//!
//! TYPE FunctionVolumeVsTime;
//!//static void Marshall(const CDM::FunctionVolumeVsTimeData& in, SEFunctionVolumeVsTime& out);
//!//static void UnMarshall(const SEFunctionVolumeVsTime& in, CDM::FunctionVolumeVsTimeData& out);
TEST_F(TEST_FIXTURE_NAME, SEFunctionVolumeVsTime)
{
}

//!
//! TYPE HistogramFractionVsLength;
//!//static void Marshall(const CDM::HistogramFractionVsLengthData& in, SEHistogramFractionVsLength& out);
//!//static void UnMarshall(const SEHistogramFractionVsLength& in, CDM::HistogramFractionVsLengthData& out);
TEST_F(TEST_FIXTURE_NAME, SEHistogramFractionVsLength)
{
}

//!
//! TYPE Histogram
//!//static void Marshall(const CDM::HistogramData& in, SEHistogram& out);
//!//static void UnMarshall(const SEHistogram& in, CDM::HistogramData& out);
TEST_F(TEST_FIXTURE_NAME, SEHistogram)
{
}

//!
//! TYPE Scalar0To1
//!//static void Marshall(const CDM::Scalar0To1Data& in, SEScalar0To1& out);
//!//static void UnMarshall(const SEScalar0To1& in, CDM::Scalar0To1Data& out);
TEST_F(TEST_FIXTURE_NAME, SEScalar0To1)
{
}

//!
//! TYPE ScalarAmount
//!//static void Marshall(const CDM::ScalarAmountData& in, SEScalarAmount& out);
//!//static void UnMarshall(const SEScalarAmount& in, CDM::ScalarAmountData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarAmount)
{
}

//!
//! TYPE ScalarAmountPerMass
//!//static void Marshall(const CDM::ScalarAmountPerMassData& in, SEScalarAmountPerMass& out);
//!//static void UnMarshall(const SEScalarAmountPerMass& in, CDM::ScalarAmountPerMassData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarAmountPerMass)
{
}

//!
//! TYPE ScalarAmountPerTime
//!//static void Marshall(const CDM::ScalarAmountPerTimeData& in, SEScalarAmountPerTime& out);
//!//static void UnMarshall(const SEScalarAmountPerTime& in, CDM::ScalarAmountPerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarAmountPerTime)
{
}

//!
//! TYPE ScalarAmountPerVolume
//!//static void Marshall(const CDM::ScalarAmountPerVolumeData& in, SEScalarAmountPerVolume& out);
//!//static void UnMarshall(const SEScalarAmountPerVolume& in, CDM::ScalarAmountPerVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarAmountPerVolume)
{
}

//!
//! TYPE ScalarArea
//!//static void Marshall(const CDM::ScalarAreaData& in, SEScalarArea& out);
//!//static void UnMarshall(const SEScalarArea& in, CDM::ScalarAreaData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarArea)
{
}

//!
//! TYPE ScalarAreaPerTimePressure
//!//static void Marshall(const CDM::ScalarAreaPerTimePressureData& in, SEScalarAreaPerTimePressure& out);
//!//static void UnMarshall(const SEScalarAreaPerTimePressure& in, CDM::ScalarAreaPerTimePressureData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarAreaPerTimePressure)
{
}

//!
//! TYPE ScalarElectricCapacitance
//!//static void Marshall(const CDM::ScalarElectricCapacitanceData& in, SEScalarElectricCapacitance& out);
//!//static void UnMarshall(const SEScalarElectricCapacitance& in, CDM::ScalarElectricCapacitanceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarElectricCapacitance)
{
}

//!
//! TYPE ScalarElectricCharge
//!//static void Marshall(const CDM::ScalarElectricChargeData& in, SEScalarElectricCharge& out);
//!//static void UnMarshall(const SEScalarElectricCharge& in, CDM::ScalarElectricChargeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarElectricCharge)
{
}

//!
//! TYPE ScalarElectricCurrent
//!//static void Marshall(const CDM::ScalarElectricCurrentData& in, SEScalarElectricCurrent& out);
//!//static void UnMarshall(const SEScalarElectricCurrent& in, CDM::ScalarElectricCurrentData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarElectricCurrent)
{
}

//!
//! TYPE ScalarElectricInductance
//!//static void Marshall(const CDM::ScalarElectricInductanceData& in, SEScalarElectricInductance& out);
//!//static void UnMarshall(const SEScalarElectricInductance& in, CDM::ScalarElectricInductanceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarElectricInductance)
{
}

//!
//! TYPE ScalarElectricPotential
//!//static void Marshall(const CDM::ScalarElectricPotentialData& in, SEScalarElectricPotential& out);
//!//static void UnMarshall(const SEScalarElectricPotential& in, CDM::ScalarElectricPotentialData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarElectricPotential)
{
}

//!
//! TYPE ScalarElectricResistance
//!//static void Marshall(const CDM::ScalarElectricResistanceData& in, SEScalarElectricResistance& out);
//!//static void UnMarshall(const SEScalarElectricResistance& in, CDM::ScalarElectricResistanceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarElectricResistance)
{
}

//!
//! TYPE ScalarEnergy
//!//static void Marshall(const CDM::ScalarEnergyData& in, SEScalarEnergy& out);
//!//static void UnMarshall(const SEScalarEnergy& in, CDM::ScalarEnergyData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarEnergy)
{
}

//!
//! TYPE ScalarEnergyPerAmount
//!//static void Marshall(const CDM::ScalarEnergyPerAmountData& in, SEScalarEnergyPerAmount& out);
//!//static void UnMarshall(const SEScalarEnergyPerAmount& in, CDM::ScalarEnergyPerAmountData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarEnergyPerAmount)
{
}

//!
//! TYPE ScalarEnergyPerMass
//!//static void Marshall(const CDM::ScalarEnergyPerMassData& in, SEScalarEnergyPerMass& out);
//!//static void UnMarshall(const SEScalarEnergyPerMass& in, CDM::ScalarEnergyPerMassData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarEnergyPerMass)
{
}

//!
//! TYPE ScalarFlowCompliance
//!//static void Marshall(const CDM::ScalarFlowComplianceData& in, SEScalarFlowCompliance& out);
//!//static void UnMarshall(const SEScalarFlowCompliance& in, CDM::ScalarFlowComplianceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarFlowCompliance)
{
}

//!
//! TYPE ScalarFlowElastance
//!//static void Marshall(const CDM::ScalarFlowElastanceData& in, SEScalarFlowElastance& out);
//!//static void UnMarshall(const SEScalarFlowElastance& in, CDM::ScalarFlowElastanceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarFlowElastance)
{
}

//!
//! TYPE ScalarFlowInertance
//!//static void Marshall(const CDM::ScalarFlowInertanceData& in, SEScalarFlowInertance& out);
//!//static void UnMarshall(const SEScalarFlowInertance& in, CDM::ScalarFlowInertanceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarFlowInertance)
{
}

//!
//! TYPE ScalarFlowResistance
//!//static void Marshall(const CDM::ScalarFlowResistanceData& in, SEScalarFlowResistance& out);
//!//static void UnMarshall(const SEScalarFlowResistance& in, CDM::ScalarFlowResistanceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarFlowResistance)
{
}

//!
//! TYPE ScalarForce
//!//static void Marshall(const CDM::ScalarForceData& in, SEScalarForce& out);
//!//static void UnMarshall(const SEScalarForce& in, CDM::ScalarForceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarForce)
{
}

//!
//! TYPE ScalarFraction
//!//static void Marshall(const CDM::ScalarFractionData& in, SEScalarFraction& out);
//!//static void UnMarshall(const SEScalarFraction& in, CDM::ScalarFractionData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarFraction)
{
}

//!
//! TYPE ScalarFrequency
//!//static void Marshall(const CDM::ScalarFrequencyData& in, SEScalarFrequency& out);
//!//static void UnMarshall(const SEScalarFrequency& in, CDM::ScalarFrequencyData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarFrequency)
{
}

//!
//! TYPE Scalar
//!//static void Marshall(const CDM::ScalarData& in, SEScalar& out);
//!//static void UnMarshall(const SEScalar& in, CDM::ScalarData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalar)
{
}

//!
//! TYPE ScalarHeatCapacitance
//!//static void Marshall(const CDM::ScalarHeatCapacitanceData& in, SEScalarHeatCapacitance& out);
//!//static void UnMarshall(const SEScalarHeatCapacitance& in, CDM::ScalarHeatCapacitanceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarHeatCapacitance)
{
}

//!
//! TYPE ScalarHeatCapacitancePerAmount
//!//static void Marshall(const CDM::ScalarHeatCapacitancePerAmountData& in, SEScalarHeatCapacitancePerAmount& out);
//!//static void UnMarshall(const SEScalarHeatCapacitancePerAmount& in, CDM::ScalarHeatCapacitancePerAmountData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarHeatCapacitancePerAmount)
{
}

//!
//! TYPE ScalarHeatCapacitancePerMass
//!//static void Marshall(const CDM::ScalarHeatCapacitancePerMassData& in, SEScalarHeatCapacitancePerMass& out);
//!//static void UnMarshall(const SEScalarHeatCapacitancePerMass& in, CDM::ScalarHeatCapacitancePerMassData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarHeatCapacitancePerMass)
{
}

//!
//! TYPE ScalarHeatConductance
//!//static void Marshall(const CDM::ScalarHeatConductanceData& in, SEScalarHeatConductance& out);
//!//static void UnMarshall(const SEScalarHeatConductance& in, CDM::ScalarHeatConductanceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarHeatConductance)
{
}

//!
//! TYPE ScalarHeatConductancePerArea
//!//static void Marshall(const CDM::ScalarHeatConductancePerAreaData& in, SEScalarHeatConductancePerArea& out);
//!//static void UnMarshall(const SEScalarHeatConductancePerArea& in, CDM::ScalarHeatConductancePerAreaData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarHeatConductancePerArea)
{
}

//!
//! TYPE ScalarHeatInductance
//!//static void Marshall(const CDM::ScalarHeatInductanceData& in, SEScalarHeatInductance& out);
//!//static void UnMarshall(const SEScalarHeatInductance& in, CDM::ScalarHeatInductanceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarHeatInductance)
{
}

//!
//! TYPE ScalarHeatResistanceArea
//!//static void Marshall(const CDM::ScalarHeatResistanceAreaData& in, SEScalarHeatResistanceArea& out);
//!//static void UnMarshall(const SEScalarHeatResistanceArea& in, CDM::ScalarHeatResistanceAreaData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarHeatResistanceArea)
{
}

//!
//! TYPE ScalarHeatResistance
//!//static void Marshall(const CDM::ScalarHeatResistanceData& in, SEScalarHeatResistance& out);
//!//static void UnMarshall(const SEScalarHeatResistance& in, CDM::ScalarHeatResistanceData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarHeatResistance)
{
}

//!
//! TYPE ScalarInversePressure
//!//static void Marshall(const CDM::ScalarInversePressureData& in, SEScalarInversePressure& out);
//!//static void UnMarshall(const SEScalarInversePressure& in, CDM::ScalarInversePressureData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarInversePressure)
{
}

//!
//! TYPE ScalarInverseVolume
//!//static void Marshall(const CDM::ScalarInverseVolumeData& in, SEScalarInverseVolume& out);
//!//static void UnMarshall(const SEScalarInverseVolume& in, CDM::ScalarInverseVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarInverseVolume)
{
}

//!
//! TYPE ScalarLength
//!//static void Marshall(const CDM::ScalarLengthData& in, SEScalarLength& out);
//!//static void UnMarshall(const SEScalarLength& in, CDM::ScalarLengthData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarLength)
{
}

//!
//! TYPE ScalarLengthPerTime
//!//static void Marshall(const CDM::ScalarLengthPerTimeData& in, SEScalarLengthPerTime& out);
//!//static void UnMarshall(const SEScalarLengthPerTime& in, CDM::ScalarLengthPerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarLengthPerTime)
{
}

//!
//! TYPE ScalarLengthPerTimePressure
//!//static void Marshall(const CDM::ScalarLengthPerTimePressureData& in, SEScalarLengthPerTimePressure& out);
//!//static void UnMarshall(const SEScalarLengthPerTimePressure& in, CDM::ScalarLengthPerTimePressureData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarLengthPerTimePressure)
{
}

//!
//! TYPE ScalarMass
//!//static void Marshall(const CDM::ScalarMassData& in, SEScalarMass& out);
//!//static void UnMarshall(const SEScalarMass& in, CDM::ScalarMassData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarMass)
{
}

//!
//! TYPE ScalarMassPerAmount
//!//static void Marshall(const CDM::ScalarMassPerAmountData& in, SEScalarMassPerAmount& out);
//!//static void UnMarshall(const SEScalarMassPerAmount& in, CDM::ScalarMassPerAmountData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarMassPerAmount)
{
}

//!
//! TYPE ScalarMassPerAreaTime
//!//static void Marshall(const CDM::ScalarMassPerAreaTimeData& in, SEScalarMassPerAreaTime& out);
//!//static void UnMarshall(const SEScalarMassPerAreaTime& in, CDM::ScalarMassPerAreaTimeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarMassPerAreaTime)
{
}

//!
//! TYPE ScalarMassPerMass
//!//static void Marshall(const CDM::ScalarMassPerMassData& in, SEScalarMassPerMass& out);
//!//static void UnMarshall(const SEScalarMassPerMass& in, CDM::ScalarMassPerMassData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarMassPerMass)
{
}

//!
//! TYPE ScalarMassPerTime
//!//static void Marshall(const CDM::ScalarMassPerTimeData& in, SEScalarMassPerTime& out);
//!//static void UnMarshall(const SEScalarMassPerTime& in, CDM::ScalarMassPerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarMassPerTime)
{
}

//!
//! TYPE ScalarMassPerVolume
//!//static void Marshall(const CDM::ScalarMassPerVolumeData& in, SEScalarMassPerVolume& out);
//!//static void UnMarshall(const SEScalarMassPerVolume& in, CDM::ScalarMassPerVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarMassPerVolume)
{
}

//!
//! TYPE ScalarNeg1To1
//!//static void Marshall(const CDM::ScalarNeg1To1Data& in, SEScalarNeg1To1& out);
//!//static void UnMarshall(const SEScalarNeg1To1& in, CDM::ScalarNeg1To1Data& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarNeg1To1)
{
}

//!
//! TYPE ScalarOsmolality
//!//static void Marshall(const CDM::ScalarOsmolalityData& in, SEScalarOsmolality& out);
//!//static void UnMarshall(const SEScalarOsmolality& in, CDM::ScalarOsmolalityData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarOsmolality)
{
}

//!
//! TYPE ScalarOsmolarity
//!//static void Marshall(const CDM::ScalarOsmolarityData& in, SEScalarOsmolarity& out);
//!//static void UnMarshall(const SEScalarOsmolarity& in, CDM::ScalarOsmolarityData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarOsmolarity)
{
}

//!
//! TYPE ScalarPower
//!//static void Marshall(const CDM::ScalarPowerData& in, SEScalarPower& out);
//!//static void UnMarshall(const SEScalarPower& in, CDM::ScalarPowerData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarPower)
{
}

//!
//! TYPE ScalarPowerPerAreaTemperatureToTheFourth
//!//static void Marshall(const CDM::ScalarPowerPerAreaTemperatureToTheFourthData& in, SEScalarPowerPerAreaTemperatureToTheFourth& out);
//!//static void UnMarshall(const SEScalarPowerPerAreaTemperatureToTheFourth& in, CDM::ScalarPowerPerAreaTemperatureToTheFourthData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarPowerPerAreaTemperatureToTheFourth)
{
}

//!
//! TYPE ScalarPressure
//!//static void Marshall(const CDM::ScalarPressureData& in, SEScalarPressure& out);
//!//static void UnMarshall(const SEScalarPressure& in, CDM::ScalarPressureData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarPressure)
{
}

//!
//! TYPE ScalarPressurePerVolume
//!//static void Marshall(const CDM::ScalarPressurePerVolumeData& in, SEScalarPressurePerVolume& out);
//!//static void UnMarshall(const SEScalarPressurePerVolume& in, CDM::ScalarPressurePerVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarPressurePerVolume)
{
}

//!
//! TYPE ScaMlarPressureTimePerArea
//!//static void Marshall(const CDM::ScalarPressureTimePerAreaData& in, SEScalarPressureTimePerArea& out);
//!//static void UnMarshall(const SEScalarPressureTimePerArea& in, CDM::ScalarPressureTimePerAreaData& out);
TEST_F(TEST_FIXTURE_NAME, SEScaMlarPressureTimePerArea)
{
}

//!
//! TYPE ScalarPressureTimePerVolumeArea
//!//static void Marshall(const CDM::ScalarPressureTimePerVolumeAreaData& in, SEScalarPressureTimePerVolumeArea& out);
//!//static void UnMarshall(const SEScalarPressureTimePerVolumeArea& in, CDM::ScalarPressureTimePerVolumeAreaData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarPressureTimePerVolumeArea)
{
}

//!
//! TYPE ScalarTemperature
//!//static void Marshall(const CDM::ScalarTemperatureData& in, SEScalarTemperature& out);
//!//static void UnMarshall(const SEScalarTemperature& in, CDM::ScalarTemperatureData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarTemperature)
{
}

//!
//! TYPE ScalarTime
//!//static void Marshall(const CDM::ScalarTimeData& in, SEScalarTime& out);
//!//static void UnMarshall(const SEScalarTime& in, CDM::ScalarTimeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarTime)
{
}

//!
//! TYPE ScalarVolume
//!//static void Marshall(const CDM::ScalarVolumeData& in, SEScalarVolume& out);
//!//static void UnMarshall(const SEScalarVolume& in, CDM::ScalarVolumeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarVolume)
{
}

//!
//! TYPE ScalarVolumePerPressure
//!//static void Marshall(const CDM::ScalarVolumePerPressureData& in, SEScalarVolumePerPressure& out);
//!//static void UnMarshall(const SEScalarVolumePerPressure& in, CDM::ScalarVolumePerPressureData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarVolumePerPressure)
{
}

//!
//! TYPE ScalarVolumePerTimeArea
//!//static void Marshall(const CDM::ScalarVolumePerTimeAreaData& in, SEScalarVolumePerTimeArea& out);
//!//static void UnMarshall(const SEScalarVolumePerTimeArea& in, CDM::ScalarVolumePerTimeAreaData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarVolumePerTimeArea)
{
}

//!
//! TYPE ScalarVolumePerTime
//!//static void Marshall(const CDM::ScalarVolumePerTimeData& in, SEScalarVolumePerTime& out);
//!//static void UnMarshall(const SEScalarVolumePerTime& in, CDM::ScalarVolumePerTimeData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarVolumePerTime)
{
}

//!
//! TYPE ScalarVolumePerTimeMass
//!//static void Marshall(const CDM::ScalarVolumePerTimeMassData& in, SEScalarVolumePerTimeMass& out);
//!//static void UnMarshall(const SEScalarVolumePerTimeMass& in, CDM::ScalarVolumePerTimeMassData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarVolumePerTimeMass)
{
}

//!
//! TYPE ScalarVolumePerTimePressureArea
//!//static void Marshall(const CDM::ScalarVolumePerTimePressureAreaData& in, SEScalarVolumePerTimePressureArea& out);
//!//static void UnMarshall(const SEScalarVolumePerTimePressureArea& in, CDM::ScalarVolumePerTimePressureAreaData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarVolumePerTimePressureArea)
{
}

//!
//! TYPE ScalarVolumePerTimePressure
//!//static void Marshall(const CDM::ScalarVolumePerTimePressureData& in, SEScalarVolumePerTimePressure& out);
//!//static void UnMarshall(const SEScalarVolumePerTimePressure& in, CDM::ScalarVolumePerTimePressureData& out);
TEST_F(TEST_FIXTURE_NAME, SEScalarVolumePerTimePressure)
{
}

//!
//!class SERunningAverage
//!//static void Marshall(const CDM::RunningAverageData& in, RunningAverage& out);
//!//static void UnMarshall(const RunningAverage& in, CDM::RunningAverageData& out);
TEST_F(TEST_FIXTURE_NAME, SERunningAverage)
{
}
