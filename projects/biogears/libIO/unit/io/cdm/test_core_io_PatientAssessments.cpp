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

#include <io/cdm/PatientAssessments.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_PatientAssessments_TEST
#define TEST_FIXTURE_NAME DISABLED_PatientAssessmentsFixture
#else
#define TEST_FIXTURE_NAME IO_PatientAssessmentsFixture
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

// class SEArterialBloodGasAnalysis;
//!
//! TYPE ArterialBloodGasAnalysis
//! static void UnMarshall(const CDM::ArterialBloodGasAnalysis& in, SEArterialBloodGasAnalysis& out);
//! static void Marshall(const SEArterialBloodGasAnalysis& in, CDM::ArterialBloodGasAnalysis& out);
#include <biogears/cdm/patient/assessments/SEArterialBloodGasAnalysis.h>
TEST_F(TEST_FIXTURE_NAME, ArterialBloodGasAnalysis)
{
  USING_TYPES(ArterialBloodGasAnalysis)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetpH().SetValue(5);
  source.GetPartialPressureOxygen().SetValue(6, biogears::PressureUnit::mmHg);
  source.GetPartialPressureCarbonDioxide().SetValue(6, biogears::PressureUnit::mmHg);
  source.GetBaseExcess().SetValue(9, biogears::AmountPerVolumeUnit::mol_Per_L);
  source.GetStandardBicarbonate().SetValue(20, biogears::AmountPerVolumeUnit::mol_Per_mL);
  source.GetOxygenSaturation().SetValue(7);

  EXPECT_NE(source, sink);

  PatientAssessments::Marshall(source, data);
  PatientAssessments::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SEPulmonaryFunctionTest;
//!
//! TYPE PulmonaryFunctionTest
//! static void UnMarshall(const CDM::PulmonaryFunctionTest& in, SEPulmonaryFunctionTest& out);
//! static void Marshall(const SEPulmonaryFunctionTest& in, CDM::PulmonaryFunctionTest& out);
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
TEST_F(TEST_FIXTURE_NAME, PulmonaryFunctionTest)
{
  USING_TYPES(PulmonaryFunctionTest)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetExpiratoryReserveVolume().SetValue(4, biogears::VolumeUnit::L);
  source.GetForcedVitalCapacity().SetValue(5, biogears::VolumeUnit::L);
  source.GetForcedExpiratoryVolume().SetValue(6, biogears::VolumeUnit::L);
  source.GetForcedExpiratoryFlow().SetValue(4, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetFunctionalResidualCapacity().SetValue(7, biogears::VolumeUnit::L);
  source.GetInspiratoryReserveVolume().SetValue(8, biogears::VolumeUnit::L);
  source.GetMaximumVoluntaryVentilation().SetValue(9, biogears::VolumeUnit::L);
  source.GetPeakExpiratoryFlow().SetValue(0.5, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetResidualVolume().SetValue(11, biogears::VolumeUnit::L);
  source.GetSlowVitalCapacity().SetValue(22, biogears::VolumeUnit::L);
  source.GetTotalLungCapacity().SetValue(33, biogears::VolumeUnit::L);
  source.GetVitalCapacity().SetValue(44, biogears::VolumeUnit::L);

  source.GetLungVolumePlot().SetTimeUnit(biogears::TimeUnit::s);
  source.GetLungVolumePlot().SetVolumeUnit(biogears::VolumeUnit::L);

  EXPECT_NE(source, sink);

  PatientAssessments::Marshall(source, data);
  PatientAssessments::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SECompleteBloodCount;
//!
//! TYPE CompleteBloodCoun
//! static void UnMarshall(const CDM::CompleteBloodCount& in, SECompleteBloodCount& out);
//! static void Marshall(const SECompleteBloodCount& in, CDM::CompleteBloodCount& out);
#include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>
TEST_F(TEST_FIXTURE_NAME, CompleteBloodCount)
{
  USING_TYPES(CompleteBloodCount)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetHematocrit().SetValue(.5);
  source.GetHemoglobin().SetValue(5, biogears::MassPerVolumeUnit::g_Per_L);
  source.GetLymphocyteCellCount().SetValue(6, biogears::AmountPerVolumeUnit::ct_Per_uL);
  source.GetMeanCorpuscularHemoglobin().SetValue(55, biogears::MassPerAmountUnit::g_Per_mmol);
  source.GetMeanCorpuscularHemoglobinConcentration().SetValue(75, biogears::MassPerVolumeUnit::g_Per_L);
  source.GetMeanCorpuscularVolume().SetValue(33, biogears::VolumeUnit::dL);
  source.GetPlateletCount().SetValue(45, biogears::AmountPerVolumeUnit::mmol_Per_L);
  source.GetNeutrophilCount().SetValue(35, biogears::AmountPerVolumeUnit::mmol_Per_L);
  source.GetRedBloodCellCount().SetValue(25, biogears::AmountPerVolumeUnit::mmol_Per_L);
  source.GetWhiteBloodCellCount().SetValue(15, biogears::AmountPerVolumeUnit::mmol_Per_L);

  EXPECT_NE(source, sink);

  PatientAssessments::Marshall(source, data);
  PatientAssessments::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SEComprehensiveMetabolicPanel;
//!
//! TYPE ComprehensiveMetabolicPanel
//! static void UnMarshall(const CDM::ComprehensiveMetabolicPanel& in, SEComprehensiveMetabolicPanel& out);
//! static void Marshall(const SEComprehensiveMetabolicPanel& in, CDM::ComprehensiveMetabolicPanel& out);
#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
TEST_F(TEST_FIXTURE_NAME, ComprehensiveMetabolicPanel)
{
  USING_TYPES(ComprehensiveMetabolicPanel)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetAlbumin().SetValue(1, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetALP().SetValue(2, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetALT().SetValue(31, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetAST().SetValue(14, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetBUN().SetValue(15, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetCalcium().SetValue(61, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetCO2().SetValue(71, biogears::AmountPerVolumeUnit::ct_Per_uL);
  source.GetCreatinine().SetValue(18, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetGlucose().SetValue(19, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetPotassium().SetValue(11, biogears::AmountPerVolumeUnit::ct_Per_uL);
  source.GetSodium().SetValue(122, biogears::AmountPerVolumeUnit::ct_Per_uL);
  source.GetTotalBilirubin().SetValue(331, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetTotalProtein().SetValue(441, biogears::MassPerVolumeUnit::g_Per_cm3);

  EXPECT_NE(source, sink);

  PatientAssessments::Marshall(source, data);
  PatientAssessments::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SEUrinalysis;
//!
//! TYPE Urinalysis
//! static void UnMarshall(const CDM::Urinalysis& in, SEUrinalysis& out);
//! static void Marshall(const SEUrinalysis& in, CDM::Urinalysis& out);
#include <biogears/cdm/patient/assessments/SEUrinalysis.h>
#include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>
TEST_F(TEST_FIXTURE_NAME, Urinalysis)
{
  USING_TYPES(Urinalysis)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetColorResult(CDM::enumUrineColor::Pink);
  source.SetAppearanceResult(CDM::enumClarityIndicator::Cloudy);
  source.SetGlucoseResult(CDM::enumPresenceIndicator::Positive);
  source.SetKetoneResult(CDM::enumPresenceIndicator::Negative);
  source.GetBilirubinResult().SetValue(5);
  source.GetSpecificGravityResult().SetValue(9);
  source.SetBloodResult(CDM::enumPresenceIndicator::Positive);
  source.GetPHResult().SetValue(2);
  source.SetProteinResult(CDM::enumPresenceIndicator::Negative);
  source.GetUrobilinogenResult().SetValue(88, biogears::MassPerVolumeUnit::g_Per_mL);
  source.SetNitriteResult(CDM::enumPresenceIndicator::Positive);
  source.SetLeukocyteEsteraseResult(CDM::enumPresenceIndicator::Negative);

  auto microscopic = new biogears::SEUrinalysisMicroscopic();

  source.GetMicroscopicResult().SetObservationType(CDM::enumMicroscopicObservationType::LowPowerField);
  source.GetMicroscopicResult().GetRedBloodCellsResult().SetValue(1, biogears::AmountUnit::mol);
  source.GetMicroscopicResult().GetWhiteBloodCellsResult().SetValue(2, biogears::AmountUnit::pmol);

  source.GetMicroscopicResult().SetEpithelialCellsResult(CDM::enumMicroscopicObservationAmount::Many);
  source.GetMicroscopicResult().GetCastsResult().SetValue(2, biogears::AmountUnit::pmol);

  source.GetMicroscopicResult().SetCrystalsResult(CDM::enumMicroscopicObservationAmount::Few);
  source.GetMicroscopicResult().SetBacteriaResult(CDM::enumMicroscopicObservationAmount::Moderate);
  source.GetMicroscopicResult().SetTrichomonadsResult(CDM::enumMicroscopicObservationAmount::Many);
  source.GetMicroscopicResult().SetYeastResult(CDM::enumMicroscopicObservationAmount::Few);

  EXPECT_NE(source, sink);

  PatientAssessments::Marshall(source, data);
  PatientAssessments::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SEUrinalysisMicroscopic;
//!
//! TYPE UrinalysisMicroscopic
//! static void UnMarshall(const CDM::UrinalysisMicroscopic& in, SEUrinalysisMicroscopic& out);
//! static void Marshall(const SEUrinalysisMicroscopic& in, CDM::UrinalysisMicroscopic& out);
#include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>
TEST_F(TEST_FIXTURE_NAME, UrinalysisMicroscopic)
{
  USING_TYPES(UrinalysisMicroscopic)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetObservationType(CDM::enumMicroscopicObservationType::LowPowerField);
  source.GetRedBloodCellsResult().SetValue(1, biogears::AmountUnit::mol);
  source.GetWhiteBloodCellsResult().SetValue(2, biogears::AmountUnit::pmol);

  source.SetEpithelialCellsResult(CDM::enumMicroscopicObservationAmount::Many);
  source.GetCastsResult().SetValue(2, biogears::AmountUnit::pmol);

  source.SetCrystalsResult(CDM::enumMicroscopicObservationAmount::Few);
  source.SetBacteriaResult(CDM::enumMicroscopicObservationAmount::Moderate);
  source.SetTrichomonadsResult(CDM::enumMicroscopicObservationAmount::Many);
  source.SetYeastResult(CDM::enumMicroscopicObservationAmount::Few);

  EXPECT_NE(source, sink);

  PatientAssessments::Marshall(source, data);
  PatientAssessments::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEProthrombinTime
//!
//! TYPE ProthrombinTime
//! static void UnMarshall(const CDM::ProthrombinTime& in, SEProthrombinTime& out);
//! static void Marshall(const SEProthrombinTime& in, CDM::ProthrombinTime& out);
#include <biogears/cdm/patient/assessments/SEProthrombinTime.h>
TEST_F(TEST_FIXTURE_NAME, ProthrombinTime)
{
  USING_TYPES(ProthrombinTime)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetInternationalNormalizedRatio().SetValue(.1);
 

  EXPECT_NE(source, sink);

  PatientAssessments::Marshall(source, data);
  PatientAssessments::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SEPsychomotorVigilanceTask
//!
//! TYPE PsychomotorVigilanceTask
//! static void UnMarshall(const CDM::PsychomotorVigilanceTask& in, SEPsychomotorVigilanceTask& out);
//! static void Marshall(const SEPsychomotorVigilanceTask& in, CDM::PsychomotorVigilanceTask& out);
#include <biogears/cdm/patient/assessments/SEPsychomotorVigilanceTask.h>
TEST_F(TEST_FIXTURE_NAME, PsychomotorVigilanceTask)
{
  USING_TYPES(PsychomotorVigilanceTask)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetAttentionLapses().SetValue(.1);
  source.GetReactionTime().SetValue(.2, biogears::TimeUnit::s);

  EXPECT_NE(source, sink);

  PatientAssessments::Marshall(source, data);
  PatientAssessments::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SESequentialOrganFailureAssessment
//!
//! TYPE SequentialOrganFailureAssessment
//! static void UnMarshall(const CDM::SequentialOrganFailureAssessment& in, SESequentialOrganFailureAssessment& out);
//! static void Marshall(const SESequentialOrganFailureAssessment& in, CDM::SequentialOrganFailureAssessment& out);
#include <biogears/cdm/patient/assessments/SESequentialOrganFailureAssessment.h>
TEST_F(TEST_FIXTURE_NAME, SequentialOrganFailureAssessment)
{
  USING_TYPES(SequentialOrganFailureAssessment)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetRespirationSOFA().SetValue(.1);
  source.GetCoagulationSOFA().SetValue(.2);
  source.GetLiverSOFA().SetValue(.3);
  source.GetCardiovascularSOFA().SetValue(.4);
  source.GetCentralNervousSOFA().SetValue(.5);
  source.GetRenalSOFA().SetValue(.6);

  EXPECT_NE(source, sink);

  PatientAssessments::Marshall(source, data);
  PatientAssessments::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}