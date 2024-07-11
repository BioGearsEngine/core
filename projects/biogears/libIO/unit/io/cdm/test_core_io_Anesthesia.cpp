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

#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/enums/SEPropertyEnums.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/Logger.h>

#include <io/cdm/Anesthesia.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_Anesthesia_TEST
#define TEST_FIXTURE_NAME DISABLED_AnesthesiaFixture
#else
#define TEST_FIXTURE_NAME IO_AnesthesiaFixture
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

#define USING_TYPES(name)             \
  using namespace biogears::io;       \
  using SEType = biogears ::SE##name; \
  using CDMType = CDM ::name##Data;

#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
// class SEAnesthesiaMachine;
//!
//! TYPE AnesthesiaMachine
//! static void UnMarshall(const CDM::AnesthesiaMachine& in, SEAnesthesiaMachine& out);
//! static void Marshall(const SEAnesthesiaMachine& in, CDM::AnesthesiaMachine& out);
TEST_F(TEST_FIXTURE_NAME, AnesthesiaMachine)
{
  USING_TYPES(AnesthesiaMachine)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source(mgr), sink(mgr);
  CDMType data;

  auto oxygen = mgr.GetSubstance("Oxygen");
  auto nitrogen = mgr.GetSubstance("Nitrogen");

  source.SetConnection(CDM::enumAnesthesiaMachineConnection::Mask);
  source.GetInletFlow().SetValue(33, biogears::VolumePerTimeUnit::mL_Per_day);
  source.GetInspiratoryExpiratoryRatio().SetValue(0.5);
  source.GetOxygenFraction().SetValue(.04);
  source.SetOxygenSource(CDM::enumAnesthesiaMachineOxygenSource::BottleTwo);
  source.GetPositiveEndExpiredPressure().SetValue(55, biogears::PressureUnit::mmHg);
  source.SetPrimaryGas(CDM::enumAnesthesiaMachinePrimaryGas::Nitrogen);
  source.GetRespiratoryRate().SetValue(66, biogears::FrequencyUnit::Per_hr);
  source.GetReliefValvePressure().SetValue(77, biogears::PressureUnit::mmHg);
  source.GetVentilatorPressure().SetValue(88, biogears::PressureUnit::mmHg);

  biogears::SEAnesthesiaMachineChamber lChamber(mgr), rChamber(mgr);
  biogears::SEAnesthesiaMachineOxygenBottle lBootle(&logger), rBottle(&logger);

  source.GetRightChamber().SetState(CDM::enumOnOff::On);
  source.GetRightChamber().GetSubstanceFraction().SetValue(.05);
  source.GetRightChamber().SetSubstance(*oxygen);

  source.GetLeftChamber().SetState(CDM::enumOnOff::On);
  source.GetLeftChamber().GetSubstanceFraction().SetValue(.05);
  source.GetLeftChamber().SetSubstance(*nitrogen);

  source.GetOxygenBottleOne().GetVolume().SetValue(2, biogears::VolumeUnit::L);
  source.GetOxygenBottleTwo().GetVolume().SetValue(3, biogears::VolumeUnit::L);

  EXPECT_NE(source, sink);

  Anesthesia::Marshall(source, data);
  Anesthesia::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEAnesthesiaMachineChamber;
//!
//! TYPE AnesthesiaMachineChamber
//! static void UnMarshall(const CDM::AnesthesiaMachineChamber& in, SEAnesthesiaMachineChamber& out);
//! static void Marshall(const SEAnesthesiaMachineChamber& in, CDM::AnesthesiaMachineChamber& out);
TEST_F(TEST_FIXTURE_NAME, AnesthesiaMachineChamber)
{
  USING_TYPES(AnesthesiaMachineChamber)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source(mgr), sink(mgr);
  CDMType data;

  auto oxygen = mgr.GetSubstance("Oxygen");
  auto nitrogen = mgr.GetSubstance("Nitrogen");

  source.SetState(CDM::enumOnOff::On);
  source.GetSubstanceFraction().SetValue(.05);
  source.SetSubstance(*oxygen);

  EXPECT_NE(source, sink);

  Anesthesia::Marshall(source, data);
  Anesthesia::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEAnesthesiaMachineOxygenBottle;
//!
//! TYPE AnesthesiaMachineOxygenBottle
//! static void UnMarshall(const CDM::AnesthesiaMachineOxygenBottle& in, SEAnesthesiaMachineOxygenBottle& out);
//! static void Marshall(const SEAnesthesiaMachineOxygenBottle& in, CDM::AnesthesiaMachineOxygenBottle& out);
TEST_F(TEST_FIXTURE_NAME, AnesthesiaMachineOxygenBottle)
{
  USING_TYPES(AnesthesiaMachineOxygenBottle)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source(&logger), sink(&logger);
  CDMType data;

  auto oxygen = mgr.GetSubstance("Oxygen");
  auto nitrogen = mgr.GetSubstance("Nitrogen");

  source.GetVolume().SetValue(2, biogears::VolumeUnit::L);

  EXPECT_NE(source, sink);

  Anesthesia::Marshall(source, data);
  Anesthesia::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
