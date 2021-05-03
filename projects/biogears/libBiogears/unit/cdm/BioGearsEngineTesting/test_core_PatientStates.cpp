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

#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>

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
TEST_F(TEST_FIXTURE_NAME, Bradycardic)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("BradycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Bradycard Patient Unit Test");
  if (!bg->LoadState("./states/Bradycardic@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Carol)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("CarolPatientUnitTest.log");
  bg->GetLogger()->Info("Carol Patient Unit Test");
  if (!bg->LoadState("./states/Carol@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Cynthia)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("CynthiaPatientUnitTest.log");
  bg->GetLogger()->Info("Cynthia Patient Unit Test");
  if (!bg->LoadState("./states/Cynthia@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, DefaultFemale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("DefaultFemalePatientUnitTest.log");
  bg->GetLogger()->Info("Default Female Patient Unit Test");
  if (!bg->LoadState("./states/DefaultFemale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, DefaultMale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("DefaultMalePatientUnitTest.log");
  bg->GetLogger()->Info("Default Male Patient Unit Test");
  if (!bg->LoadState("./states/DefaultMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, DefaultTemplateFemale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("DefaultTemplateFemalePatientUnitTest.log");
  bg->GetLogger()->Info("Default Template Female Patient Unit Test");
  if (!bg->LoadState("./states/DefaultTemplateFemale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, DefaultTemplateMale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("DefaultTemplateMalePatientUnitTest.log");
  bg->GetLogger()->Info("Default Template Male Patient Unit Test");
  if (!bg->LoadState("./states/DefaultTemplateMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, ExtremeFemale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("ExtremeFemalePatientUnitTest.log");
  bg->GetLogger()->Info("Extreme Female Patient Unit Test");
  if (!bg->LoadState("./states/ExtremeFemale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, ExtremeMale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("ExtremeMalePatientUnitTest.log");
  bg->GetLogger()->Info("Extreme Male Patient Unit Test");
  if (!bg->LoadState("./states/ExtremeMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Gus)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("GusPatientUnitTest.log");
  bg->GetLogger()->Info("Gus Patient Unit Test");
  if (!bg->LoadState("./states/Gus@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Hassan)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HassanPatientUnitTest.log");
  bg->GetLogger()->Info("Hassan Patient Unit Test");
  if (!bg->LoadState("./states/Hassan@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Jane)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("JanePatientUnitTest.log");
  bg->GetLogger()->Info("Jane Patient Unit Test");
  if (!bg->LoadState("./states/Jane@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Jeff)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("JeffPatientUnitTest.log");
  bg->GetLogger()->Info("Jeff Patient Unit Test");
  if (!bg->LoadState("./states/Jeff@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Joel)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("JoelPatientUnitTest.log");
  bg->GetLogger()->Info("Joel Patient Unit Test");
  if (!bg->LoadState("./states/Joel@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Nathan)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("NathanPatientUnitTest.log");
  bg->GetLogger()->Info("Nathan Patient Unit Test");
  if (!bg->LoadState("./states/Nathan@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Rick)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("RickPatientUnitTest.log");
  bg->GetLogger()->Info("Rick Patient Unit Test");
  if (!bg->LoadState("./states/Rick@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Soldier)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("SoldierPatientUnitTest.log");
  bg->GetLogger()->Info("Soldier Patient Unit Test");
  if (!bg->LoadState("./states/Soldier@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, StandardFemale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("StandardFemalePatientUnitTest.log");
  bg->GetLogger()->Info("Standard Female Patient Unit Test");
  if (!bg->LoadState("./states/StandardFemale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, StandardMale)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("StandardMalePatientUnitTest.log");
  bg->GetLogger()->Info("Standard Male Patient Unit Test");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Tachycardic)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("TachycardicPatientUnitTest.log");
  bg->GetLogger()->Info("Tachycardic Patient Unit Test");
  if (!bg->LoadState("./states/Tachycardic@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, ToughGirl)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("ToughGirlPatientUnitTest.log");
  bg->GetLogger()->Info("HowToAsthmaAttack");
  if (!bg->LoadState("./states/ToughGirl@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, ToughGuy)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("ToughGuyPatientUnitTest.log");
  bg->GetLogger()->Info("Tough Guy Patient Unit Test");
  if (!bg->LoadState("./states/ToughGuy@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Tristan)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("TristanPatientUnitTest.log");
  bg->GetLogger()->Info("HowToAsthmaAttack");
  if (!bg->LoadState("./states/Tristan@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}
TEST_F(TEST_FIXTURE_NAME, Underweight)
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("UnderweightPatientUnitTest.log");
  bg->GetLogger()->Info("HowToAsthmaAttack");
  if (!bg->LoadState("./states/Underweight@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
  EXPECT_NO_THROW(bg->AdvanceModelTime(1.0, TimeUnit::s));
}