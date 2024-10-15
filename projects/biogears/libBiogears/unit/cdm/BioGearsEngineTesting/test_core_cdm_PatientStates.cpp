//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the ALche License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.aLche.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------
//!
//! @author David Lee
//! @date   2017 Aug 3rd
//!
//! Unit Test for Biogears-common Config
//!
#include <limits>
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#include <biogears/string/manipulation.h>

#include <iostream>
#include <string>

#ifdef DISABLE_BIOGEARS_PatientStates_TEST
#define TEST_FIXTURE_NAME DISABLED_PatientStates_Fixture
#else
#define TEST_FIXTURE_NAME PatientStates_Fixture
#endif

using namespace biogears;
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
  virtual void SetUp() override;

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown() override;
  biogears::Logger* logger;
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{
}

TEST_F(TEST_FIXTURE_NAME, StateGeneration)
{
  using namespace biogears;

  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("StateGeneration.log");
  SEPatient patient { bg->GetLogger() };

  patient.SetName("StateGeneration");

  patient.SetName("StandardMale");
  patient.SetSex(biogears::SESex::Male);
  patient.GetAge().SetValue(44, biogears::TimeUnit::yr);
  patient.GetWeight().SetValue(170, biogears::MassUnit::lb);
  patient.GetHeight().SetValue(71, biogears::LengthUnit::inch);
  patient.GetBodyFatFraction().SetValue(0.21);
  patient.SetBloodType(SEBloodType::AB);
  patient.SetBloodRh(true);
  patient.GetHeartRateBaseline().SetValue(72, biogears::FrequencyUnit::Per_min);
  patient.GetRespirationRateBaseline().SetValue(16, biogears::FrequencyUnit::Per_min);
  patient.GetDiastolicArterialPressureBaseline().SetValue(73.5, biogears::PressureUnit::mmHg);
  patient.GetSystolicArterialPressureBaseline().SetValue(114, biogears::PressureUnit::mmHg);
  
  bg->InitializeEngine(patient);
  
  std::string stateFile = asprintf("%s%s@%.0fs.xml", "UnitTest", patient.GetName().c_str());
  bg->SaveStateToFile(stateFile);

  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState(stateFile));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}


TEST_F(TEST_FIXTURE_NAME, StandardFemale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/StandardFemale@0s.xml")); 
  
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}

TEST_F(TEST_FIXTURE_NAME, DefaultTemplateFemale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/DefaultTemplateFemale@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}

TEST_F(TEST_FIXTURE_NAME, DefaultTemplateMale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/DefaultTemplateMale@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}

TEST_F(TEST_FIXTURE_NAME, StandardMale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/StandardMale@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}


//Female@0s.xml
TEST_F(TEST_FIXTURE_NAME, Female)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Female@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}

//Female_18_Normal@0s.xml
//Female_30_Normal@0s.xml
TEST_F(TEST_FIXTURE_NAME, Female_18_Normal)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Female_18_Normal@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}

TEST_F(TEST_FIXTURE_NAME, Female_30_Normal)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Female_30_Normal@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
//Female_40_Overweight@0s.xml
//Male@0s.xml
TEST_F(TEST_FIXTURE_NAME, Female_40_Overweight)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Female_40_Overweight@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}

TEST_F(TEST_FIXTURE_NAME, Male)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Male@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
//Male_22_Fit_Soldier@0s.xml
//Male_24_Normal_hidrosis2@0s.xml
TEST_F(TEST_FIXTURE_NAME, Male_22_Fit_Soldier)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Male_22_Fit_Soldier@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}

TEST_F(TEST_FIXTURE_NAME, Male_24_Normal_hidrosis2)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Male_24_Normal_hidrosis2@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
//Male_25_Normal@0s.xml
//Male_28_Normal_hr109_rr18@0s.xml
TEST_F(TEST_FIXTURE_NAME, Male_25_Normal)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Male_25_Normal@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}

TEST_F(TEST_FIXTURE_NAME, Male_28_Normal_hr109_rr18)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Male_28_Normal_hr109_rr18@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
//Male_32_Normal_hr93_rr14@0s.xml
//Male_44_Bradycardic@0s.xml
TEST_F(TEST_FIXTURE_NAME, Male_32_Normal_hr93_rr14)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Male_32_Normal_hr93_rr14@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}

TEST_F(TEST_FIXTURE_NAME, Male_44_Bradycardic)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Male_44_Bradycardic@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
//Male_44_Normal_hr109_rr15@0s.xml
//Male_44_Normal_rr12@0s.xml
TEST_F(TEST_FIXTURE_NAME, Male_44_Normal_hr109_rr15)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Male_44_Normal_hr109_rr15@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}

TEST_F(TEST_FIXTURE_NAME, Male_44_Normal_rr12)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Male_44_Normal_rr12@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
//Male_44_SleepDeprived@0s.xml
//Male_44_Tachycardic@0s.xml
TEST_F(TEST_FIXTURE_NAME, Male_44_SleepDeprived)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Male_44_SleepDeprived@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}

TEST_F(TEST_FIXTURE_NAME, Male_44_Tachycardic)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  ASSERT_TRUE(bg->LoadState("./states/Male_44_Tachycardic@0s.xml"));

  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
