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
#include <biogears/cdm/utils/Logger.h>

#include <io/cdm/Property.h>
#include <io/cdm/Scenario.h>

#ifdef DISABLE_BIOGEARS_Scenario_TEST
#define TEST_FIXTURE_NAME DISABLED_ScenarioFixture
#else
#define TEST_FIXTURE_NAME IO_ScenarioFixture
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

// class SECondition;
//  TYPE Condition
// static void Marshall(const CDM::ConditionData& in, SECondition& out);
// static void UnMarshall(const SECondition& in, CDM::ConditionData& out);
#include <biogears/cdm/scenario/SECondition.h>

// class SEAction;
//  TYPE Action
// static void Marshall(const CDM::ActionData& in, SEAction& out);
// static void UnMarshall(const SEAction& in, CDM::ActionData& out);
#include <biogears/cdm/scenario/SEAction.h>

// class SEDataRequest;
//  TYPE DataRequest
// static void Marshall(const CDM::DataRequestData& in, SEDataRequest& out);
// static void UnMarshall(const SEDataRequest& in, CDM::DataRequestData& out);
#include <biogears/cdm/scenario/requests/SEDataRequest.h>

// class SEPatientDataRequest;
//  TYPE PatientDataRequest
// static void Marshall(const CDM::PatientDataRequestData& in, SEPatientDataRequest& out);
// static void UnMarshall(const SEPatientDataRequest& in, CDM::PatientDataRequestData& out);
#include <biogears/cdm/scenario/requests/SEPatientDataRequest.h>
TEST_F(TEST_FIXTURE_NAME, PatientDataRequest)
{
  USING_TYPES(PatientDataRequest)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetNotation(biogears::DecimalNotation::Fixed);
  source.SetPrecision(3);

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SEPhysiologyDataRequest;
//  TYPE PhysiologyDataRequest
// static void Marshall(const CDM::PhysiologyDataRequestData& in, SEPhysiologyDataRequest& out);
// static void UnMarshall(const SEPhysiologyDataRequest& in, CDM::PhysiologyDataRequestData& out);
#include <biogears/cdm/scenario/requests/SEPhysiologyDataRequest.h>
TEST_F(TEST_FIXTURE_NAME, PhysiologyDataRequest)
{
  USING_TYPES(PhysiologyDataRequest)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetNotation(biogears::DecimalNotation::Fixed);
  source.SetPrecision(3);

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SEEnvironmentDataRequest;
//  TYPE EnvironmentDataRequest
// static void Marshall(const CDM::EnvironmentDataRequestData& in, SEEnvironmentDataRequest& out);
// static void UnMarshall(const SEEnvironmentDataRequest& in, CDM::EnvironmentDataRequestData& out);
#include <biogears/cdm/scenario/requests/SEEnvironmentDataRequest.h>
TEST_F(TEST_FIXTURE_NAME, EnvironmentDataRequest)
{
  USING_TYPES(EnvironmentDataRequest)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetName("Leg");
  source.SetNotation(biogears::DecimalNotation::Fixed);
  source.SetPrecision(3);

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SEEquipmentDataRequest;
//  TYPE EquipmentDataRequest
// static void Marshall(const CDM::EquipmentDataRequestData& in, SEEquipmentDataRequest& out);
// static void UnMarshall(const SEEquipmentDataRequest& in, CDM::EquipmentDataRequestData& out);
#include <biogears/cdm/scenario/requests/SEEquipmentDataRequest.h>
TEST_F(TEST_FIXTURE_NAME, EquipmentDataRequest)
{
  USING_TYPES(EquipmentDataRequest)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetType("A TYPE");

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SECompartmentDataRequest;
//  TYPE CompartmentDataRequest
// static void Marshall(const CDM::CompartmentDataRequestData& in, SECompartmentDataRequest& out);
// static void UnMarshall(const SECompartmentDataRequest& in, CDM::CompartmentDataRequestData& out);
#include <biogears/cdm/scenario/requests/SECompartmentDataRequest.h>

// class SECompartmentSubstanceDataRequest;
//  TYPE CompartmentSubstanceDataRequest
// static void Marshall(const CDM::CompartmentSubstanceDataRequestData& in, SECompartmentSubstanceDataRequest& out);
// static void UnMarshall(const SECompartmentSubstanceDataRequest& in, CDM::CompartmentSubstanceDataRequestData& out);
#include <biogears/cdm/scenario/requests/SECompartmentSubstanceDataRequest.h>

// class SEGasCompartmentDataRequest;
//  TYPE GasCompartmentDataRequest
// static void Marshall(const CDM::GasCompartmentDataRequestData& in, SEGasCompartmentDataRequest& out);
// static void UnMarshall(const SEGasCompartmentDataRequest& in, CDM::GasCompartmentDataRequestData& out);
#include <biogears/cdm/scenario/requests/SEGasCompartmentDataRequest.h>
TEST_F(TEST_FIXTURE_NAME, GasCompartmentDataRequest)
{
  USING_TYPES(GasCompartmentDataRequest)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetCompartment("Leg");

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SELiquidCompartmentDataRequest;
//  TYPE LiquidCompartmentDataRequest
// static void Marshall(const CDM::LiquidCompartmentDataRequestData& in, SELiquidCompartmentDataRequest& out);
// static void UnMarshall(const SELiquidCompartmentDataRequest& in, CDM::LiquidCompartmentDataRequestData& out);
#include <biogears/cdm/scenario/requests/SELiquidCompartmentDataRequest.h>
TEST_F(TEST_FIXTURE_NAME, LiquidCompartmentDataRequest)
{
  USING_TYPES(LiquidCompartmentDataRequest)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source {}, sink {};
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetCompartment("Leg");

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SEThermalCompartmentDataRequest;
//  TYPE ThermalCompartmentDataRequest
// static void Marshall(const CDM::ThermalCompartmentDataRequestData& in, SEThermalCompartmentDataRequest& out);
// static void UnMarshall(const SEThermalCompartmentDataRequest& in, CDM::ThermalCompartmentDataRequestData& out);
#include <biogears/cdm/scenario/requests/SEThermalCompartmentDataRequest.h>
TEST_F(TEST_FIXTURE_NAME, ThermalCompartmentDataRequest)
{
  USING_TYPES(ThermalCompartmentDataRequest)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetCompartment("Leg");

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SETissueCompartmentDataRequest;
//  TYPE TissueCompartmentDataRequest
// static void Marshall(const CDM::TissueCompartmentDataRequestData& in, SETissueCompartmentDataRequest& out);
// static void UnMarshall(const SETissueCompartmentDataRequest& in, CDM::TissueCompartmentDataRequestData& out);
#include <biogears/cdm/scenario/requests/SETissueCompartmentDataRequest.h>
TEST_F(TEST_FIXTURE_NAME, TissueCompartmentDataRequest)
{
  USING_TYPES(TissueCompartmentDataRequest)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetCompartment("Leg");

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SESubstanceDataRequest;
//  TYPE SubstanceDataRequest
// static void Marshall(const CDM::SubstanceDataRequestData& in, SESubstanceDataRequest& out);
// static void UnMarshall(const SESubstanceDataRequest& in, CDM::SubstanceDataRequestData& out);
#include <biogears/cdm/scenario/requests/SESubstanceDataRequest.h>
TEST_F(TEST_FIXTURE_NAME, SubstanceDataRequest)
{
  USING_TYPES(SubstanceDataRequest)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetCompartment("Leg");
  source.SetSubstance(sarin);

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, mgr, sink);

  EXPECT_EQ(source, sink);
}
// class SEDataRequestManager;
//  TYPE DataRequestManager
// static void Marshall(const CDM::DataRequestManagerData& in, SEDataRequestManager& out);
// static void UnMarshall(const SEDataRequestManager& in, CDM::DataRequestManagerData& out);
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>
TEST_F(TEST_FIXTURE_NAME, DataRequestManager)
{
  USING_TYPES(DataRequestManager)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetSamplesPerSecond(60);

  source.SetResultsFilename("REsultsFile");
  source.SetWorkingDir("WorkingDir");

  source.GetDefaultDecimalFormatting().Set(FixtureDecimalFormat);
  source.GetOverrideDecimalFormatting().Set(FixtureDecimalFormat);

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, mgr, sink);

  EXPECT_EQ(source, sink);
}
// class SEScenario;
//  TYPE Scenario
// static void Marshall(const CDM::ScenarioData& in, SEScenario& out);
// static void UnMarshall(const SEScenario& in, CDM::ScenarioData& out);
#include <biogears/cdm/engine/PhysiologyEngineDynamicStabilization.h>
#include <biogears/cdm/engine/PhysiologyEngineStabilization.h>
#include <biogears/cdm/engine/PhysiologyEngineTimedStabilization.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolator.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolationWaveform.h>
TEST_F(TEST_FIXTURE_NAME, Scenario)
{
  USING_TYPES(Scenario)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { mgr }, sink { mgr };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetName("SetName");
  source.SetDescription("SetDescription");
  source.SetEngineStateFile("SetEngineStateFile");

  source.GetInitialParameters().GetConfiguration().GetECGInterpolator();

  source.GetInitialParameters().GetConfiguration().GetStabilizationCriteria()->GetStabilizationDuration().SetValue(5.0, biogears::TimeUnit::s);

  source.GetInitialParameters().GetConfiguration().GetTimedStabilizationCriteria();
  source.GetInitialParameters().GetConfiguration().GetTimedStabilizationCriteria().GetFeedbackStabilizationTime().SetValue(5.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetTimedStabilizationCriteria().GetRestingStabilizationTime().SetValue(5.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetTimedStabilizationCriteria().GetStabilizationDuration().SetValue(5.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetTimedStabilizationCriteria().GetConditionCriteria();

  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().SetName("Resting Critiera)");
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetConvergenceTime().SetValue(5.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetMinimumReactionTime().SetValue(7.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetMaximumAllowedStabilizationTime().SetValue(9.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetPropertyConvergence();

  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetConditionCriteria();

  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria();
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria().SetName("Feedback Criteria");
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria().GetConvergenceTime().SetValue(5.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria().GetMinimumReactionTime().SetValue(7.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria().GetMaximumAllowedStabilizationTime().SetValue(9.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria().GetPropertyConvergence();

  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().SetName("Resting Critiera)");
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetConvergenceTime().SetValue(5.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetMinimumReactionTime().SetValue(7.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetMaximumAllowedStabilizationTime().SetValue(9.0, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetPropertyConvergence();

  source.GetInitialParameters().GetConfiguration().GetDynamicStabilizationCriteria().GetStabilizationDuration().SetValue(10, biogears::TimeUnit::s);

  source.GetInitialParameters().GetConfiguration().GetTimeStep().SetValue(0.02, biogears::TimeUnit::s);
  source.GetInitialParameters().GetConfiguration().SetWritePatientBaselineFile(CDM::enumOnOff::Off);

  source.GetInitialParameters().GetPatient().SetName("unit_test");
  source.GetInitialParameters().GetPatient().SetAnnotation("Seerailization Test User");
  source.GetInitialParameters().GetPatient().SetGender(CDM::enumSex::Female);
  source.GetInitialParameters().GetPatient().SetSex(CDM::enumSex::Female);
  source.GetInitialParameters().GetPatient().GetAge().SetValue(55.0, biogears::TimeUnit::yr);
  source.GetInitialParameters().GetPatient().GetWeight().SetValue(80.0, biogears::MassUnit::kg);
  source.GetInitialParameters().GetPatient().GetHeight().SetValue(175.0, biogears::LengthUnit::cm);
  source.GetInitialParameters().GetPatient().GetAlveoliSurfaceArea().SetValue(16.0, biogears::AreaUnit::cm2);
  source.GetInitialParameters().GetPatient().GetBasalMetabolicRate().SetValue(15.0, biogears::PowerUnit::J_Per_s);
  source.GetInitialParameters().GetPatient().SetBloodType(CDM::enumBloodType::AB);
  source.GetInitialParameters().GetPatient().SetBloodRh(true);
  source.GetInitialParameters().GetPatient().GetBloodVolumeBaseline().SetValue(14.0, biogears::VolumeUnit::L);
  source.GetInitialParameters().GetPatient().GetBodyDensity().SetValue(13.0, biogears::MassPerVolumeUnit::g_Per_L);
  source.GetInitialParameters().GetPatient().GetBodyFatFraction().SetValue(12.0);
  source.GetInitialParameters().GetPatient().GetDiastolicArterialPressureBaseline().SetValue(11.0, biogears::PressureUnit::cmH2O);
  source.GetInitialParameters().GetPatient().GetExpiratoryReserveVolume().SetValue(10.0, biogears::VolumeUnit::mL);
  source.GetInitialParameters().GetPatient().GetFunctionalResidualCapacity().SetValue(9.0, biogears::VolumeUnit::mL);
  source.GetInitialParameters().GetPatient().GetHeartRateBaseline().SetValue(2.0, biogears::FrequencyUnit::Hz);
  source.GetInitialParameters().GetPatient().GetHeartRateMaximum().SetValue(1.0, biogears::FrequencyUnit::Hz);
  source.GetInitialParameters().GetPatient().GetHeartRateMinimum().SetValue(3.0, biogears::FrequencyUnit::Hz);
  source.GetInitialParameters().GetPatient().GetHyperhidrosis().SetValue(-0.3);
  source.GetInitialParameters().GetPatient().GetInspiratoryCapacity().SetValue(4.0, biogears::VolumeUnit::L);
  source.GetInitialParameters().GetPatient().GetInspiratoryReserveVolume().SetValue(5.0, biogears::VolumeUnit::L);
  source.GetInitialParameters().GetPatient().GetLeanBodyMass().SetValue(6.0, biogears::MassUnit::g);
  source.GetInitialParameters().GetPatient().GetMaxWorkRate().SetValue(7.0, biogears::PowerUnit::W);
  source.GetInitialParameters().GetPatient().GetMuscleMass().SetValue(8.0, biogears::MassUnit::g);
  source.GetInitialParameters().GetPatient().GetMeanArterialPressureBaseline().SetValue(3.0, biogears::PressureUnit::mmHg);
  source.GetInitialParameters().GetPatient().GetPainSusceptibility().SetValue(0.5);
  source.GetInitialParameters().GetPatient().GetResidualVolume().SetValue(9.0, biogears::VolumeUnit::L);
  source.GetInitialParameters().GetPatient().GetRespirationRateBaseline().SetValue(3.0, biogears::FrequencyUnit::Hz);
  source.GetInitialParameters().GetPatient().GetRespiratoryDriverAmplitudeBaseline().SetValue(3.0, biogears::PressureUnit::mmHg);
  source.GetInitialParameters().GetPatient().GetRightLungRatio().SetValue(3.20);
  source.GetInitialParameters().GetPatient().GetSkinSurfaceArea().SetValue(3.10, biogears::AreaUnit::cm2);
  source.GetInitialParameters().GetPatient().GetSleepAmount().SetValue(300.0, biogears::TimeUnit::min);
  source.GetInitialParameters().GetPatient().GetSystolicArterialPressureBaseline().SetValue(34.0, biogears::PressureUnit::mmHg);
  source.GetInitialParameters().GetPatient().GetTotalVentilationBaseline().SetValue(33.0, biogears::VolumePerTimeUnit::mL_Per_hr);
  source.GetInitialParameters().GetPatient().GetTidalVolumeBaseline().SetValue(32.0, biogears::VolumeUnit::L);
  source.GetInitialParameters().GetPatient().GetTotalLungCapacity().SetValue(31.0, biogears::VolumeUnit::L);
  source.GetInitialParameters().GetPatient().GetVitalCapacity().SetValue(30.0, biogears::VolumeUnit::L);

  source.GetAutoSerialization().GetPeriod().SetValue(10, biogears::TimeUnit::s);
  source.GetAutoSerialization().SetPeriodTimeStamps(CDM::enumOnOff::On);
  source.GetAutoSerialization().SetAfterActions(CDM::enumOnOff::On);
  source.GetAutoSerialization().SetReloadState(CDM::enumOnOff::On);
  source.GetAutoSerialization().SetDirectory("SetDirectory:");
  source.GetAutoSerialization().SetFileName("SetFileName:");
  source.GetDataRequestManager();

  biogears::SEPainStimulus action;
  action.GetSeverity().SetValue(.5);
  action.SetLocation("LEG");
  source.AddAction(action);

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SEScenarioInitialParameters;
//  TYPE ScenarioInitialParameters
// static void Marshall(const CDM::ScenarioInitialParametersData& in, SEScenarioInitialParameters& out);
// static void UnMarshall(const SEScenarioInitialParameters& in, CDM::ScenarioInitialParametersData& out);
#include <biogears/cdm/patient/conditions/SEChronicAnemia.h>
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>

TEST_F(TEST_FIXTURE_NAME, ScenarioInitialParameters)
{
  USING_TYPES(ScenarioInitialParameters)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { mgr }, sink { mgr };

  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetConfiguration().GetECGInterpolator();

  source.GetConfiguration().GetStabilizationCriteria()->GetStabilizationDuration().SetValue(5.0, biogears::TimeUnit::s);

  source.GetConfiguration().GetTimedStabilizationCriteria();
  source.GetConfiguration().GetTimedStabilizationCriteria().GetFeedbackStabilizationTime().SetValue(5.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetTimedStabilizationCriteria().GetRestingStabilizationTime().SetValue(5.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetTimedStabilizationCriteria().GetStabilizationDuration().SetValue(5.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetTimedStabilizationCriteria().GetConditionCriteria();

  source.GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().SetName("Resting Critiera)");
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetConvergenceTime().SetValue(5.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetMinimumReactionTime().SetValue(7.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetMaximumAllowedStabilizationTime().SetValue(9.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetPropertyConvergence();

  source.GetConfiguration().GetDynamicStabilizationCriteria().GetConditionCriteria();

  source.GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria();
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria().SetName("Feedback Criteria");
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria().GetConvergenceTime().SetValue(5.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria().GetMinimumReactionTime().SetValue(7.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria().GetMaximumAllowedStabilizationTime().SetValue(9.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetFeedbackCriteria().GetPropertyConvergence();

  source.GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().SetName("Resting Critiera)");
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetConvergenceTime().SetValue(5.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetMinimumReactionTime().SetValue(7.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetMaximumAllowedStabilizationTime().SetValue(9.0, biogears::TimeUnit::s);
  source.GetConfiguration().GetDynamicStabilizationCriteria().GetRestingCriteria().GetPropertyConvergence();

  source.GetConfiguration().GetTimeStep().SetValue(0.02, biogears::TimeUnit::s);

  source.GetConfiguration().SetWritePatientBaselineFile(CDM::enumOnOff::Off);

  source.GetPatient().SetName("unit_test");
  source.GetPatient().SetAnnotation("Seerailization Test User");
  source.GetPatient().SetGender(CDM::enumSex::Female);
  source.GetPatient().SetSex(CDM::enumSex::Female);
  source.GetPatient().GetAge().SetValue(55.0, biogears::TimeUnit::yr);
  source.GetPatient().GetWeight().SetValue(80.0, biogears::MassUnit::kg);
  source.GetPatient().GetHeight().SetValue(175.0, biogears::LengthUnit::cm);
  source.GetPatient().GetAlveoliSurfaceArea().SetValue(16.0, biogears::AreaUnit::cm2);
  source.GetPatient().GetBasalMetabolicRate().SetValue(15.0, biogears::PowerUnit::J_Per_s);
  source.GetPatient().SetBloodType(CDM::enumBloodType::AB);
  source.GetPatient().SetBloodRh(true);
  source.GetPatient().GetBloodVolumeBaseline().SetValue(14.0, biogears::VolumeUnit::L);
  source.GetPatient().GetBodyDensity().SetValue(13.0, biogears::MassPerVolumeUnit::g_Per_L);
  source.GetPatient().GetBodyFatFraction().SetValue(12.0);
  source.GetPatient().GetDiastolicArterialPressureBaseline().SetValue(11.0, biogears::PressureUnit::cmH2O);
  source.GetPatient().GetExpiratoryReserveVolume().SetValue(10.0, biogears::VolumeUnit::mL);
  source.GetPatient().GetFunctionalResidualCapacity().SetValue(9.0, biogears::VolumeUnit::mL);
  source.GetPatient().GetHeartRateBaseline().SetValue(2.0, biogears::FrequencyUnit::Hz);
  source.GetPatient().GetHeartRateMaximum().SetValue(1.0, biogears::FrequencyUnit::Hz);
  source.GetPatient().GetHeartRateMinimum().SetValue(3.0, biogears::FrequencyUnit::Hz);
  source.GetPatient().GetHyperhidrosis().SetValue(-0.3);
  source.GetPatient().GetInspiratoryCapacity().SetValue(4.0, biogears::VolumeUnit::L);
  source.GetPatient().GetInspiratoryReserveVolume().SetValue(5.0, biogears::VolumeUnit::L);
  source.GetPatient().GetLeanBodyMass().SetValue(6.0, biogears::MassUnit::g);
  source.GetPatient().GetMaxWorkRate().SetValue(7.0, biogears::PowerUnit::W);
  source.GetPatient().GetMuscleMass().SetValue(8.0, biogears::MassUnit::g);
  source.GetPatient().GetMeanArterialPressureBaseline().SetValue(3.0, biogears::PressureUnit::mmHg);
  source.GetPatient().GetPainSusceptibility().SetValue(0.5);
  source.GetPatient().GetResidualVolume().SetValue(9.0, biogears::VolumeUnit::L);
  source.GetPatient().GetRespirationRateBaseline().SetValue(3.0, biogears::FrequencyUnit::Hz);
  source.GetPatient().GetRespiratoryDriverAmplitudeBaseline().SetValue(3.0, biogears::PressureUnit::mmHg);
  source.GetPatient().GetRightLungRatio().SetValue(3.20);
  source.GetPatient().GetSkinSurfaceArea().SetValue(3.10, biogears::AreaUnit::cm2);
  source.GetPatient().GetSleepAmount().SetValue(300.0, biogears::TimeUnit::min);
  source.GetPatient().GetSystolicArterialPressureBaseline().SetValue(34.0, biogears::PressureUnit::mmHg);
  source.GetPatient().GetTotalVentilationBaseline().SetValue(33.0, biogears::VolumePerTimeUnit::mL_Per_hr);
  source.GetPatient().GetTidalVolumeBaseline().SetValue(32.0, biogears::VolumeUnit::L);
  source.GetPatient().GetTotalLungCapacity().SetValue(31.0, biogears::VolumeUnit::L);
  source.GetPatient().GetVitalCapacity().SetValue(30.0, biogears::VolumeUnit::L);

  source.SetPatientFile("Male.xml");

  biogears::SEChronicAnemia condition;
  condition.SetComment("SEChronicAnemia");
  condition.GetReductionFactor().SetValue(0.4);
   
  source.AddCondition(condition);
  source.SetTrackStabilization(false);

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEScenarioAutoSerialization;
//  TYPE ScenarioAutoSerialization
// static void Marshall(const CDM::ScenarioAutoSerializationData& in, SEScenarioAutoSerialization& out);
// static void UnMarshall(const SEScenarioAutoSerialization& in, CDM::ScenarioAutoSerializationData& out);
#include <biogears/cdm/scenario/SEScenarioAutoSerialization.h>
TEST_F(TEST_FIXTURE_NAME, ScenarioAutoSerialization)
{
  USING_TYPES(ScenarioAutoSerialization)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetPeriod().SetValue(1.0, biogears::TimeUnit::hr);

  source.SetPeriodTimeStamps(CDM::enumOnOff::Off);
  source.SetAfterActions(CDM::enumOnOff::On);
  source.SetReloadState(CDM::enumOnOff::Off);

  source.SetDirectory("Something Something");
  source.SetFileName("Nothing Nothing");

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEAdvanceTime
//  TYPE AdvanceTime
// static void Marshall(const CDM::AdvanceTimeData& in, SEAdvanceTime& out);
// static void UnMarshall(const SEAdvanceTime& in, CDM::AdvanceTimeData& out);
#include <biogears/cdm/scenario/SEAdvanceTime.h>
TEST_F(TEST_FIXTURE_NAME, AdvanceTime)
{
  USING_TYPES(AdvanceTime)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetTime().SetValue(1.0, biogears::TimeUnit::hr);

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SESerializeState
//  TYPE SerializeState
// static void Marshall(const CDM::SerializeStateData& in, SESerializeState& out);
// static void UnMarshall(const SESerializeState& in, CDM::SerializeStateData& out);
#include <biogears/cdm/scenario/SESerializeState.h>
TEST_F(TEST_FIXTURE_NAME, SerializeState)
{
  USING_TYPES(SerializeState)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetComment("SerializeState Test");
  source.SetFilename("State.txt");
  source.SetType(CDM::enumSerializationType::Save);

  EXPECT_NE(source, sink);

  Scenario::UnMarshall(source, data);
  Scenario::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}