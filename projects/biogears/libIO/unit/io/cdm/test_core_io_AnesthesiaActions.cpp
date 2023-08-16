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
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
#include <biogears/cdm/utils/Logger.h>

#include <io/cdm/AnesthesiaActions.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_Anesthesia_Actions_TEST
#define TEST_FIXTURE_NAME DISABLED_Anesthesia_ActionsFixture
#else
#define TEST_FIXTURE_NAME AnesthesiaActionsFixture
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

  biogears::Logger logger;
  biogears::SESubstanceManager* mgr = nullptr;
  biogears::SEAnesthesiaMachine* machine = nullptr;
};

void TEST_FIXTURE_NAME::SetUp()
{
  if (!mgr) {
    mgr = new biogears::SESubstanceManager(&logger);
    mgr->LoadSubstanceDirectory();
  }
  if (!machine) {
    machine = new biogears::SEAnesthesiaMachine(*mgr);
  }

  auto oxygen = mgr->GetSubstance("Oxygen");
  auto nitrogen = mgr->GetSubstance("Nitrogen");

  machine->SetConnection(CDM::enumAnesthesiaMachineConnection::Mask);
  machine->GetInletFlow().SetValue(33, biogears::VolumePerTimeUnit::mL_Per_day);
  machine->GetInspiratoryExpiratoryRatio().SetValue(0.5);
  machine->GetOxygenFraction().SetValue(.04);
  machine->SetOxygenSource(CDM::enumAnesthesiaMachineOxygenSource::BottleTwo);
  machine->GetPositiveEndExpiredPressure().SetValue(55, biogears::PressureUnit::mmHg);
  machine->SetPrimaryGas(CDM::enumAnesthesiaMachinePrimaryGas::Nitrogen);
  machine->GetRespiratoryRate().SetValue(66, biogears::FrequencyUnit::Per_hr);
  machine->GetReliefValvePressure().SetValue(77, biogears::PressureUnit::mmHg);
  machine->GetVentilatorPressure().SetValue(88, biogears::PressureUnit::mmHg);

  biogears::SEAnesthesiaMachineChamber lChamber(*mgr), rChamber(*mgr);
  biogears::SEAnesthesiaMachineOxygenBottle lBootle(&logger), rBottle(&logger);

  machine->GetRightChamber().SetState(CDM::enumOnOff::On);
  machine->GetRightChamber().GetSubstanceFraction().SetValue(.05);
  machine->GetRightChamber().SetSubstance(*oxygen);

  machine->GetLeftChamber().SetState(CDM::enumOnOff::On);
  machine->GetLeftChamber().GetSubstanceFraction().SetValue(.05);
  machine->GetLeftChamber().SetSubstance(*nitrogen);

  machine->GetOxygenBottleOne().GetVolume().SetValue(2, biogears::VolumeUnit::L);
  machine->GetOxygenBottleTwo().GetVolume().SetValue(3, biogears::VolumeUnit::L);
}

void TEST_FIXTURE_NAME::TearDown()
{
  machine->Clear();
}

#define USING_TYPES(name)             \
  using namespace biogears::io;       \
  using SEType = biogears ::SE##name; \
  using CDMType = CDM ::name##Data;

#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>

// class SEAnesthesiaMachineConfiguration;
//!
//! TYPE AnesthesiaMachineConfiguration
//! static void Marshall(const CDM::AnesthesiaMachineConfigurationData& in, SEAnesthesiaMachineConfiguration& out);
//! static void UnMarshall(const SEAnesthesiaMachineConfiguration& in, CDM::AnesthesiaMachineConfigurationData& out);
TEST_F(TEST_FIXTURE_NAME, AnesthesiaMachineConfiguration)
{
  USING_TYPES(AnesthesiaMachineConfiguration)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source(mgr), sink(mgr);
  CDMType data;

  auto oxygen = mgr.GetSubstance("Oxygen");
  auto nitrogen = mgr.GetSubstance("Nitrogen");

  source.GetConfiguration().SetConnection(CDM::enumAnesthesiaMachineConnection::Mask);
  source.GetConfiguration().GetInletFlow().SetValue(33, biogears::VolumePerTimeUnit::mL_Per_day);
  source.GetConfiguration().GetInspiratoryExpiratoryRatio().SetValue(0.5);
  source.GetConfiguration().GetOxygenFraction().SetValue(0.5);
  source.GetConfiguration().SetOxygenSource(CDM::enumAnesthesiaMachineOxygenSource::BottleTwo);
  source.GetConfiguration().GetPositiveEndExpiredPressure().SetValue(55, biogears::PressureUnit::mmHg);
  source.GetConfiguration().SetPrimaryGas(CDM::enumAnesthesiaMachinePrimaryGas::Nitrogen);
  source.GetConfiguration().GetRespiratoryRate().SetValue(66, biogears::FrequencyUnit::Per_hr);
  source.GetConfiguration().GetReliefValvePressure().SetValue(77, biogears::PressureUnit::mmHg);
  source.GetConfiguration().GetVentilatorPressure().SetValue(88, biogears::PressureUnit::mmHg);
  source.GetConfiguration().GetRightChamber().SetState(CDM::enumOnOff::On);
  source.GetConfiguration().GetRightChamber().GetSubstanceFraction().SetValue(0.5);
  source.GetConfiguration().GetRightChamber().SetSubstance(*oxygen);
  source.GetConfiguration().GetLeftChamber().SetState(CDM::enumOnOff::On);
  source.GetConfiguration().GetLeftChamber().GetSubstanceFraction().SetValue(0.5);
  source.GetConfiguration().GetLeftChamber().SetSubstance(*nitrogen);
  source.GetConfiguration().GetOxygenBottleOne().GetVolume().SetValue(3, biogears::VolumeUnit::L);
  source.GetConfiguration().GetOxygenBottleTwo().GetVolume().SetValue(3, biogears::VolumeUnit::L);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/system/equipment/Anesthesia/actions/SEExpiratoryValveLeak.h>
// class SEExpiratoryValveLeak;
//!
//! TYPE ExpiratoryValveLeak
//! static void Marshall(const CDM::ExpiratoryValveLeakData& in, SEExpiratoryValveLeak& out);
//! static void UnMarshall(const SEExpiratoryValveLeak& in, CDM::ExpiratoryValveLeakData& out);
TEST_F(TEST_FIXTURE_NAME, ExpiratoryValveLeak)
{
  USING_TYPES(ExpiratoryValveLeak)

  SEType source, sink;
  CDMType data;

  source.SetComment("ExpiratoryValveLeak");
  source.GetSeverity().SetValue(.8);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/system/equipment/Anesthesia/actions/SEExpiratoryValveObstruction.h>
// class SEExpiratoryValveObstruction;
//!
//! TYPE ExpiratoryValveObstruction
//! static void Marshall(const CDM::ExpiratoryValveObstructionData& in, SEExpiratoryValveObstruction& out);
//! static void UnMarshall(const SEExpiratoryValveObstruction& in, CDM::ExpiratoryValveObstructionData& out);
TEST_F(TEST_FIXTURE_NAME, ExpiratoryValveObstruction)
{
  USING_TYPES(ExpiratoryValveObstruction)

  SEType source, sink;
  CDMType data;

  source.SetComment("ExpiratoryValveObstruction");
  source.GetSeverity().SetValue(.8);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEInspiratoryValveLeak.h>
// class SEInspiratoryValveLeak;
//!
//! TYPE InspiratoryValveLeak
//! static void Marshall(const CDM::InspiratoryValveLeakData& in, SEInspiratoryValveLeak& out);
//! static void UnMarshall(const SEInspiratoryValveLeak& in, CDM::InspiratoryValveLeakData& out);
TEST_F(TEST_FIXTURE_NAME, InspiratoryValveLeak)
{
  USING_TYPES(InspiratoryValveLeak)

  SEType source, sink;
  CDMType data;

  source.SetComment("InspiratoryValveLeak");
  source.GetSeverity().SetValue(.8);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEInspiratoryValveObstruction.h>
// class SEInspiratoryValveObstruction;
//!
//! TYPE InspiratoryValveObstruction
//! static void Marshall(const CDM::InspiratoryValveObstructionData& in, SEInspiratoryValveObstruction& out);
//! static void UnMarshall(const SEInspiratoryValveObstruction& in, CDM::InspiratoryValveObstructionData& out);
TEST_F(TEST_FIXTURE_NAME, InspiratoryValveObstruction)
{
  USING_TYPES(InspiratoryValveObstruction)

  SEType source, sink;
  CDMType data;

  source.SetComment("InspiratoryValveObstruction");
  source.GetSeverity().SetValue(.8);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEMaskLeak.h>
// class SEMaskLeak;
//!
//! TYPE MaskLeak
//! static void Marshall(const CDM::MaskLeakData& in, SEMaskLeak& out);
//! static void UnMarshall(const SEMaskLeak& in, CDM::MaskLeakData& out);
TEST_F(TEST_FIXTURE_NAME, MaskLeak)
{
  USING_TYPES(MaskLeak)

  SEType source, sink;
  CDMType data;

  source.SetComment("MaskLeak");
  source.GetSeverity().SetValue(.8);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/system/equipment/Anesthesia/actions/SESodaLimeFailure.h>
// class SESodaLimeFailure;
//!
//! TYPE SodaLimeFailure
//! static void Marshall(const CDM::SodaLimeFailureData& in, SESodaLimeFailure& out);
//! static void UnMarshall(const SESodaLimeFailure& in, CDM::SodaLimeFailureData& out);
TEST_F(TEST_FIXTURE_NAME, SodaLimeFailure)
{
  USING_TYPES(SodaLimeFailure)

  SEType source, sink;
  CDMType data;

  source.SetComment("SodaLimeFailure");
  source.GetSeverity().SetValue(.8);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/system/equipment/Anesthesia/actions/SETubeCuffLeak.h>
// class SETubeCuffLeak;
//!
//! TYPE TubeCuffLeak
//! static void Marshall(const CDM::TubeCuffLeakData& in, SETubeCuffLeak& out);
//! static void UnMarshall(const SETubeCuffLeak& in, CDM::TubeCuffLeakData& out);
TEST_F(TEST_FIXTURE_NAME, TubeCuffLeak)
{
  USING_TYPES(TubeCuffLeak)

  SEType source, sink;
  CDMType data;

  source.SetComment("TubeCuffLeak");
  source.GetSeverity().SetValue(.8);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEVaporizerFailure.h>
// class SEVaporizerFailure;
//!
//! TYPE VaporizerFailure
//! static void Marshall(const CDM::VaporizerFailureData& in, SEVaporizerFailure& out);
//! static void UnMarshall(const SEVaporizerFailure& in, CDM::VaporizerFailureData& out);
TEST_F(TEST_FIXTURE_NAME, VaporizerFailure)
{
  USING_TYPES(VaporizerFailure)

  SEType source, sink;
  CDMType data;

  source.SetComment("VaporizerFailure");
  source.GetSeverity().SetValue(.8);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEVentilatorPressureLoss.h>
// class SEVentilatorPressureLoss;
//!
//! TYPE VentilatorPressureLoss
//! static void Marshall(const CDM::VentilatorPressureLossData& in, SEVentilatorPressureLoss& out);
//! static void UnMarshall(const SEVentilatorPressureLoss& in, CDM::VentilatorPressureLossData& out);
TEST_F(TEST_FIXTURE_NAME, VentilatorPressureLoss)
{
  USING_TYPES(VentilatorPressureLoss)

  SEType source, sink;
  CDMType data;

  source.SetComment("VentilatorPressureLoss");
  source.GetSeverity().SetValue(.8);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEYPieceDisconnect.h>
// class SEYPieceDisconnect;
//!
//! TYPE YPieceDisconnect
//! static void Marshall(const CDM::YPieceDisconnectData& in, SEYPieceDisconnect& out);
//! static void UnMarshall(const SEYPieceDisconnect& in, CDM::YPieceDisconnectData& out);
TEST_F(TEST_FIXTURE_NAME, YPieceDisconnect)
{
  USING_TYPES(YPieceDisconnect)

  SEType source, sink;
  CDMType data;

  source.SetComment("YPieceDisconnect");
  source.GetSeverity().SetValue(.8);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenWallPortPressureLoss.h>
// class SEOxygenWallPortPressureLoss;
//!
//! TYPE OxygenWallPortPressureLoss
//! static void Marshall(const CDM::OxygenWallPortPressureLossData& in, SEOxygenWallPortPressureLoss& out);
//! static void UnMarshall(const SEOxygenWallPortPressureLoss& in, CDM::OxygenWallPortPressureLossData& out);
TEST_F(TEST_FIXTURE_NAME, OxygenWallPortPressureLoss)
{
  USING_TYPES(OxygenWallPortPressureLoss)

  SEType source, sink;
  CDMType data;

  source.SetComment("OxygenWallPortPressureLoss");
  source.SetActive(true);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenTankPressureLoss.h>
// class SEOxygenTankPressureLoss;
//!
//! TYPE OxygenTankPressureLoss
//! static void Marshall(const CDM::OxygenTankPressureLossData& in, SEOxygenTankPressureLoss& out);
//! static void UnMarshall(const SEOxygenTankPressureLoss& in, CDM::OxygenTankPressureLossData& out);
TEST_F(TEST_FIXTURE_NAME, OxygenTankPressureLoss)
{
  USING_TYPES(OxygenTankPressureLoss)

  SEType source, sink;
  CDMType data;

  source.SetComment("OxygenTankPressureLoss");
  source.SetActive(true);

  EXPECT_NE(source, sink);

  AnesthesiaActions::UnMarshall(source, data);
  AnesthesiaActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}