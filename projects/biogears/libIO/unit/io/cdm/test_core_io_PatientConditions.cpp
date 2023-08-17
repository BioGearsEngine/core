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

#include <io/cdm/PatientConditions.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_PatientConditions_TEST
#define TEST_FIXTURE_NAME DISABLED_PatientConditionsFixture
#else
#define TEST_FIXTURE_NAME PatientConditionsFixture
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


//class SEPatientCondition
// TYPE PatientCondition
//static void Marshall(const CDM::PatientConditionData& in, SEPatientCondition& out);
//static void UnMarshall(const SEPatientCondition& in, CDM::PatientConditionData& out);
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>

//class SEChronicAnemia
// TYPE ChronicAnemia
//static void Marshall(const CDM::ChronicAnemiaData& in, SEChronicAnemia& out);
//static void UnMarshall(const SEChronicAnemia& in, CDM::ChronicAnemiaData& out);
#include <biogears/cdm/patient/conditions/SEChronicAnemia.h>

//class SEChronicVentricularSystolicDysfunction
// TYPE ChronicVentricularSystolicDysfunction
//static void Marshall(const CDM::ChronicVentricularSystolicDysfunctionData& in, SEChronicVentricularSystolicDysfunction& out);
//static void UnMarshall(const SEChronicVentricularSystolicDysfunction& in, CDM::ChronicVentricularSystolicDysfunctionData& out);
#include <biogears/cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.h>
TEST_F(TEST_FIXTURE_NAME, ChronicVentricularSystolicDysfunction)
{
  USING_TYPES(ChronicVentricularSystolicDysfunction)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetComment("ChronicVentricularSystolicDysfunction Serial Test.");

  EXPECT_NE(source, sink);

  PatientConditions::UnMarshall(source, data);
  PatientConditions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
//class SEChronicObstructivePulmonaryDisease
// TYPE ChronicObstructivePulmonaryDisease
//static void Marshall(const CDM::ChronicObstructivePulmonaryDiseaseData& in, SEChronicObstructivePulmonaryDisease& out);
//static void UnMarshall(const SEChronicObstructivePulmonaryDisease& in, CDM::ChronicObstructivePulmonaryDiseaseData& out);
#include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>
TEST_F(TEST_FIXTURE_NAME, ChronicObstructivePulmonaryDisease)
{
  USING_TYPES(ChronicObstructivePulmonaryDisease)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetBronchitisSeverity().SetValue(0.3);
  source.GetEmphysemaSeverity().SetValue(0.3);

  EXPECT_NE(source, sink);

  PatientConditions::UnMarshall(source, data);
  PatientConditions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
//class SEChronicPericardialEffusion
// TYPE ChronicPericardialEffusion
//static void Marshall(const CDM::ChronicPericardialEffusionData& in, SEChronicPericardialEffusion& out);
//static void UnMarshall(const SEChronicPericardialEffusion& in, CDM::ChronicPericardialEffusionData& out);
#include <biogears/cdm/patient/conditions/SEChronicPericardialEffusion.h>
TEST_F(TEST_FIXTURE_NAME, ChronicPericardialEffusion)
{
  USING_TYPES(ChronicPericardialEffusion)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetAccumulatedVolume().SetValue(0.3, biogears::VolumeUnit::L);


  EXPECT_NE(source, sink);

  PatientConditions::UnMarshall(source, data);
  PatientConditions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
//class SEChronicRenalStenosis
// TYPE ChronicRenalStenosis
//static void Marshall(const CDM::ChronicRenalStenosisData& in, SEChronicRenalStenosis& out);
//static void UnMarshall(const SEChronicRenalStenosis& in, CDM::ChronicRenalStenosisData& out);
#include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
TEST_F(TEST_FIXTURE_NAME, ChronicRenalStenosis)
{
  USING_TYPES(ChronicRenalStenosis)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetLeftKidneySeverity().SetValue(0.3);
  source.GetRightKidneySeverity().SetValue(0.3);

  EXPECT_NE(source, sink);

  PatientConditions::UnMarshall(source, data);
  PatientConditions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

//class SEDehydration
// TYPE Dehydration
//static void Marshall(const CDM::DehydrationData& in, SEDehydration& out);
//static void UnMarshall(const SEDehydration& in, CDM::DehydrationData& out);
#include <biogears/cdm/patient/conditions/SEDehydration.h>
TEST_F(TEST_FIXTURE_NAME, Dehydration)
{
  USING_TYPES(Dehydration)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetDehydrationFraction().SetValue(0.3);

  EXPECT_NE(source, sink);

  PatientConditions::UnMarshall(source, data);
  PatientConditions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
//class SEDiabetesType1
// TYPE DiabetesType1
//static void Marshall(const CDM::DiabetesType1Data& in, SEDiabetesType1& out);
//static void UnMarshall(const SEDiabetesType1& in, CDM::DiabetesType1Data& out);
#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
TEST_F(TEST_FIXTURE_NAME, DiabetesType1)
{
  USING_TYPES(DiabetesType1)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetInsulinProductionSeverity().SetValue(0.3);

  EXPECT_NE(source, sink);

  PatientConditions::UnMarshall(source, data);
  PatientConditions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
//class SEDiabetesType2
// TYPE DiabetesType2
//static void Marshall(const CDM::DiabetesType2Data& in, SEDiabetesType2& out);
//static void UnMarshall(const SEDiabetesType2& in, CDM::DiabetesType2Data& out);
#include <biogears/cdm/patient/conditions/SEDiabetesType2.h>
TEST_F(TEST_FIXTURE_NAME, DiabetesType2)
{
  USING_TYPES(DiabetesType2)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetInsulinProductionSeverity().SetValue(0.3);
  source.GetInsulinResistanceSeverity().SetValue(0.3);

  EXPECT_NE(source, sink);

  PatientConditions::UnMarshall(source, data);
  PatientConditions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
//class SEImpairedAlveolarExchange
// TYPE ImpairedAlveolarExchange
//static void Marshall(const CDM::ImpairedAlveolarExchangeData& in, SEImpairedAlveolarExchange& out);
//static void UnMarshall(const SEImpairedAlveolarExchange& in, CDM::ImpairedAlveolarExchangeData& out);
#include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
TEST_F(TEST_FIXTURE_NAME, ImpairedAlveolarExchange)
{
  USING_TYPES(ImpairedAlveolarExchange)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetImpairedFraction().SetValue( 0.3 );
  source.GetImpairedSurfaceArea().SetValue(5, biogears::AreaUnit::cm2);

  EXPECT_NE(source, sink);

  PatientConditions::UnMarshall(source, data);
  PatientConditions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
//class SELobarPneumonia
// TYPE LobarPneumonia
//static void Marshall(const CDM::LobarPneumoniaData& in, SELobarPneumonia& out);
//static void UnMarshall(const SELobarPneumonia& in, CDM::LobarPneumoniaData& out);
#include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
TEST_F(TEST_FIXTURE_NAME, LobarPneumonia)
{
  USING_TYPES(LobarPneumonia)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetSeverity().SetValue(0.7);
  source.GetLeftLungAffected().SetValue(0.2);
  source.GetRightLungAffected().SetValue(0.3);

  EXPECT_NE(source, sink);

  PatientConditions::UnMarshall(source, data);
  PatientConditions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
//class SEStarvation
// TYPE Starvation
//static void Marshall(const CDM::StarvationData& in, SEStarvation& out);
//static void UnMarshall(const SEStarvation& in, CDM::StarvationData& out);
#include <biogears/cdm/patient/conditions/SEStarvation.h>
TEST_F(TEST_FIXTURE_NAME, Starvation)
{
  USING_TYPES(Starvation)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetTimeSinceMeal().SetValue(5, biogears::TimeUnit::yr);

  EXPECT_NE(source, sink);

  PatientConditions::UnMarshall(source, data);
  PatientConditions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}