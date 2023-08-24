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
//! @date   2023/08/14
//!
//! Unit Test for BioGears Seralization
//!
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>

#include <io/cdm/Property.h>
#include <io/cdm/SubstanceQuantity.h>

#ifdef DISABLE_BIOGEARS_Substance_TEST
#define TEST_FIXTURE_NAME DISABLED_SubstanceQuantityFixture
#else
#define TEST_FIXTURE_NAME SubstanceQuantityFixture
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

  biogears::SEDecimalFormat FixtureDecimalFormat;
};

void TEST_FIXTURE_NAME::SetUp()
{
  FixtureDecimalFormat.SetNotation(biogears::DecimalNotation::Fixed);
  FixtureDecimalFormat.SetPrecision(2);
}

void TEST_FIXTURE_NAME::TearDown()
{
}

#define USING_TYPES(name)             \
  using namespace biogears::io;       \
  using SEType = biogears ::SE##name; \
  using CDMType = CDM ::name##Data;

// class  SEGasSubstanceQuantity
//    TYPE GasSubstanceQuantity
//   static void Marshall(const CDM::GasSubstanceQuantityData& in, SEGasSubstanceQuantity& out);
//   static void UnMarshall(const SEGasSubstanceQuantity& in, CDM::GasSubstanceQuantityData& out);
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
namespace BGE = mil::tatrc::physiology::biogears;

TEST_F(TEST_FIXTURE_NAME, GasSubstanceQuantity)
{

  USING_TYPES(GasSubstanceQuantity)

  biogears::Logger logger;
  biogears::SEDataRequestManager drMgr { &logger };
  biogears::SESubstanceManager subMgr { &logger };
  biogears::SECompartmentManager compMgr { subMgr };

  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());

  auto sarin = subMgr.GetSubstance("Sarin");

  auto& pRightChestLeak = compMgr.CreateGasCompartment(BGE::PulmonaryCompartment::RightChestLeak);
  auto& pLeftChestLeak = compMgr.CreateGasCompartment(BGE::PulmonaryCompartment::LeftChestLeak);

  SEType source { *sarin, pRightChestLeak }, sink { *sarin, pLeftChestLeak };
  CDMType data;

  source.GetPartialPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetVolume().SetValue(1.0, biogears::VolumeUnit::L);
  source.GetVolumeFraction().SetValue(1.0);

  EXPECT_NE(source, sink);

  SubstanceQuantity::UnMarshall(source, data);
  SubstanceQuantity::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class  SELiquidSubstanceQuantity
//    TYPE LiquidSubstanceQuantity
//   static void Marshall(const CDM::LiquidSubstanceQuantityData& in, SELiquidSubstanceQuantity& out);
//   static void UnMarshall(const SELiquidSubstanceQuantity& in, CDM::LiquidSubstanceQuantityData& out);
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>

TEST_F(TEST_FIXTURE_NAME, LiquidSubstanceQuantity)
{

  USING_TYPES(LiquidSubstanceQuantity)

  biogears::Logger logger;
  biogears::SEDataRequestManager drMgr { &logger };
  biogears::SESubstanceManager subMgr { &logger };
  biogears::SECompartmentManager compMgr { subMgr };

  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());

  auto sarin = subMgr.GetSubstance("Sarin");

  auto& RightRenalArtery = compMgr.CreateLiquidCompartment(BGE::VascularCompartment::RightRenalArtery);
  auto& vRightRenalVein = compMgr.CreateLiquidCompartment(BGE::VascularCompartment::RightRenalVein);

  SEType source { *sarin, RightRenalArtery }, sink { *sarin, RightRenalArtery };
  CDMType data;

  source.GetConcentration().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetMass().SetValue(1.0, biogears::MassUnit::g);
  source.GetMassCleared().SetValue(1.0, biogears::MassUnit::g);
  source.GetMassDeposited().SetValue(1.0, biogears::MassUnit::g);
  source.GetMassExcreted().SetValue(1.0, biogears::MassUnit::g); 
  source.GetMolarity().SetValue(1.0, biogears::AmountPerVolumeUnit::ct_Per_uL);
  source.GetSaturation().SetValue(0.22);

  //Don't get me started on tis
  EXPECT_THROW(source.GetPartialPressure().SetValue(1.0, biogears::PressureUnit::mmHg), biogears::CommonDataModelException);
  EXPECT_THROW(sink.GetPartialPressure().SetValue(1.0, biogears::PressureUnit::mmHg), biogears::CommonDataModelException);

  EXPECT_NE(source, sink);

  SubstanceQuantity::UnMarshall(source, data);
  SubstanceQuantity::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}