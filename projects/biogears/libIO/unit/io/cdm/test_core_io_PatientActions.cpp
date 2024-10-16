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

#include <biogears/cdm/enums/SEPropertyEnums.h>
#include <biogears/cdm/utils/Logger.h>

#include <io/cdm/PatientActions.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_PatientActions_TEST
#define TEST_FIXTURE_NAME DISABLED_PatientActionsFixture
#else
#define TEST_FIXTURE_NAME IO_PatientActionsFixture
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

#include <biogears/cdm/patient/actions/SEPatientAction.h>

#include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
// class SEPatientAssessmentRequest;
//!
//! TYPE PatientAssessmentRequest
//! static void UnMarshall(const CDM::PatientAssessmentRequestData& in, SEPatientAssessmentRequest& out);
//! static void Marshall(const SEPatientAssessmentRequest& in, CDM::PatientAssessmentRequestData& out);
TEST_F(TEST_FIXTURE_NAME, PatientAssessmentRequest)
{
  USING_TYPES(PatientAssessmentRequest)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetType(biogears::SEPatientAssessmentType::CompleteBloodCount);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEAcuteStress.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
// class SEAcuteStress;
//!
//! TYPE AcuteStress
//! static void UnMarshall(const CDM::AcuteStressData& in, SEAcuteStress& out);
//! static void Marshall(const SEAcuteStress& in, CDM::AcuteStressData& out);
TEST_F(TEST_FIXTURE_NAME, AcuteStress)
{
  USING_TYPES(AcuteStress)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEAirwayObstruction.h>
// class SEAirwayObstruction;
//!
//! TYPE AirwayObstruction
//! static void UnMarshall(const CDM::AirwayObstructionData& in, SEAirwayObstruction& out);
//! static void Marshall(const SEAirwayObstruction& in, CDM::AirwayObstructionData& out);
TEST_F(TEST_FIXTURE_NAME, AirwayObstruction)
{
  USING_TYPES(AirwayObstruction)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEApnea.h>
// class SEApnea;
//!
//! TYPE Apnea
//! static void UnMarshall(const CDM::ApneaData& in, SEApnea& out);
//! static void Marshall(const SEApnea& in, CDM::ApneaData& out);
TEST_F(TEST_FIXTURE_NAME, Apnea)
{
  USING_TYPES(Apnea)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
// class SEAsthmaAttack;
//!
//! TYPE AsthmaAttack
//! static void UnMarshall(const CDM::AsthmaAttackData& in, SEAsthmaAttack& out);
//! static void Marshall(const SEAsthmaAttack& in, CDM::AsthmaAttackData& out);
TEST_F(TEST_FIXTURE_NAME, AsthmaAttack)
{
  USING_TYPES(AsthmaAttack)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEBrainInjury.h>
// class SEBrainInjury;
//!
//! TYPE BrainInjury
//! static void UnMarshall(const CDM::BrainInjuryData& in, SEBrainInjury& out);
//! static void Marshall(const SEBrainInjury& in, CDM::BrainInjuryData& out);
TEST_F(TEST_FIXTURE_NAME, BrainInjury)
{
  USING_TYPES(BrainInjury)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);
  source.SetType(biogears::SEBrainInjuryType::LeftFocal);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEBronchoconstriction.h>
// class SEBronchoconstriction;
//!
//! TYPE Bronchoconstriction
//! static void UnMarshall(const CDM::BronchoconstrictionData& in, SEBronchoconstriction& out);
//! static void Marshall(const SEBronchoconstriction& in, CDM::BronchoconstrictionData& out);
TEST_F(TEST_FIXTURE_NAME, Bronchoconstriction)
{
  USING_TYPES(Bronchoconstriction)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEBurnWound.h>
// class SEBurnWound;
//!
//! TYPE BurnWound
//! static void UnMarshall(const CDM::BurnWoundData& in, SEBurnWound& out);
//! static void Marshall(const SEBurnWound& in, CDM::BurnWoundData& out);
TEST_F(TEST_FIXTURE_NAME, BurnWound)
{
  USING_TYPES(BurnWound)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetTotalBodySurfaceArea(.79);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SECardiacArrest.h>
// class SECardiacArrest;
//!
//! TYPE CardiacArrest
//! static void UnMarshall(const CDM::CardiacArrestData& in, SECardiacArrest& out);
//! static void Marshall(const SECardiacArrest& in, CDM::CardiacArrestData& out);
TEST_F(TEST_FIXTURE_NAME, CardiacArrest)
{
  USING_TYPES(CardiacArrest)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEChestCompression.h>
// class SEChestCompression;
//!
//! TYPE ChestCompression
//! static void UnMarshall(const CDM::ChestCompressionData& in, SEChestCompression& out);
//! static void Marshall(const SEChestCompression& in, CDM::ChestCompressionData& out);
TEST_F(TEST_FIXTURE_NAME, ChestCompression)
{
  USING_TYPES(ChestCompression)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
#include <biogears/cdm/properties/SEScalarForce.h>
// class SEChestCompressionForce;
//!
//! TYPE ChestCompressionForce
//! static void UnMarshall(const CDM::ChestCompressionForceData& in, SEChestCompressionForce& out);
//! static void Marshall(const SEChestCompressionForce& in, CDM::ChestCompressionForceData& out);
TEST_F(TEST_FIXTURE_NAME, ChestCompressionForce)
{
  USING_TYPES(ChestCompressionForce)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetForce().SetValue(.79, biogears::ForceUnit::dyn);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEChestCompressionForceScale.h>
#include <biogears/cdm/properties/SEScalarTime.h>
// class SEChestCompressionForceScale;
//!
//! TYPE ChestCompressionForceScale
//! static void UnMarshall(const CDM::ChestCompressionForceScaleData& in, SEChestCompressionForceScale& out);
//! static void Marshall(const SEChestCompressionForceScale& in, CDM::ChestCompressionForceScaleData& out);
TEST_F(TEST_FIXTURE_NAME, ChestCompressionForceScale)
{
  USING_TYPES(ChestCompressionForceScale)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetForcePeriod().SetValue(.79, biogears::TimeUnit::hr);
  source.GetForceScale().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEChestOcclusiveDressing.h>
// class SEChestOcclusiveDressing;
//!
//! TYPE ChestOcclusiveDressing
//! static void UnMarshall(const CDM::ChestOcclusiveDressingData& in, SEChestOcclusiveDressing& out);
//! static void Marshall(const SEChestOcclusiveDressing& in, CDM::ChestOcclusiveDressingData& out);
TEST_F(TEST_FIXTURE_NAME, ChestOcclusiveDressing)
{
  USING_TYPES(ChestOcclusiveDressing)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetSide(biogears::SESide::Right);
  source.SetActive(true);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEEbola.h>
// class SEEbola;
// static void UnMarshall(const CDM::EbolaData& in, SEEbola& out);
// static void Marshall(const SEEbola& in, CDM::EbolaData& out);

TEST_F(TEST_FIXTURE_NAME, Ebola)
{
  USING_TYPES(Ebola)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetSeverity(biogears::SEInfectionSeverity::Moderate);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEForcedInhale.h>
// class SEForcedInhale;
//!
//! TYPE ForcedInhale
//! static void UnMarshall(const CDM::ForcedInhaleData& in, SEForcedInhale& out);
//! static void Marshall(const SEForcedInhale& in, CDM::ForcedInhaleData& out);
TEST_F(TEST_FIXTURE_NAME, ForcedInhale)
{
  USING_TYPES(ForcedInhale)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetPeriod().SetValue(1, biogears::TimeUnit::s);
  source.GetInspiratoryCapacityFraction().Set(0.72);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEForcedExhale.h>
// class SEForcedExhale;
//!
//! TYPE ForcedExhale
//! static void UnMarshall(const CDM::ForcedExhaleData& in, SEForcedExhale& out);
//! static void Marshall(const SEForcedExhale& in, CDM::ForcedExhaleData& out);
TEST_F(TEST_FIXTURE_NAME, ForcedExhale)
{
  USING_TYPES(ForcedExhale)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetPeriod().SetValue(1, biogears::TimeUnit::s);
  source.GetExpiratoryReserveVolumeFraction().SetValue(.72);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEBreathHold.h>
// class SEBreathHold;
//!
//! TYPE BreathHold
//! static void UnMarshall(const CDM::BreathHoldData& in, SEBreathHold& out);
//! static void Marshall(const SEBreathHold& in, CDM::BreathHoldData& out);
TEST_F(TEST_FIXTURE_NAME, BreathHold)
{
  USING_TYPES(BreathHold)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetPeriod().SetValue(1, biogears::TimeUnit::s);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEUseInhaler.h>
// class SEUseInhaler;
//!
//! TYPE UseInhaler
//! static void UnMarshall(const CDM::UseInhalerData& in, SEUseInhaler& out);
//! static void Marshall(const SEUseInhaler& in, CDM::UseInhalerData& out);
TEST_F(TEST_FIXTURE_NAME, UseInhaler)
{
  USING_TYPES(UseInhaler)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEConsciousRespiration.h>
// class SEConsciousRespiration;
//!
//! TYPE ConsciousRespiration
//! static void UnMarshall(const CDM::ConsciousRespirationData& in, SEConsciousRespiration& out);
//! static void Marshall(const SEConsciousRespiration& in, CDM::ConsciousRespirationData& out);
TEST_F(TEST_FIXTURE_NAME, ConsciousRespiration)
{
  USING_TYPES(ConsciousRespiration)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
// class SEConsumeNutrients;
//!
//! TYPE ConsumeNutrients
//! static void UnMarshall(const CDM::ConsumeNutrientsData& in, SEConsumeNutrients& out);
//! static void Marshall(const SEConsumeNutrients& in, CDM::ConsumeNutrientsData& out);
TEST_F(TEST_FIXTURE_NAME, ConsumeNutrients)
{
  USING_TYPES(ConsumeNutrients)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  auto& nutrition = source.GetNutrition();
  nutrition.SetName("Not Real nutrition");
  nutrition.GetCarbohydrate().SetValue(0.7777, biogears::MassUnit::g);
  nutrition.GetFat().SetValue(0.6666, biogears::MassUnit::g);
  nutrition.GetProtein().SetValue(0.5555, biogears::MassUnit::g);
  nutrition.GetCalcium().SetValue(0.4444, biogears::MassUnit::g);
  nutrition.GetSodium().SetValue(0.2222, biogears::MassUnit::g);
  nutrition.GetWater().SetValue(0.1111, biogears::VolumeUnit::dL);
  source.SetNutritionFile("NotARealFile.txt");

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEExercise.h>
// class SEExercise;
//!
//! TYPE Exercise
//! static void UnMarshall(const CDM::ExerciseData& in, SEExercise& out);
//! static void Marshall(const SEExercise& in, CDM::ExerciseData& out);
TEST_F(TEST_FIXTURE_NAME, Exercise)
{
  USING_TYPES(Exercise)

  SEType source, sink;
  CDMType data;

  source.SetComment("SECycling");
  biogears::SEExercise::SECycling cycling;

  cycling.AddedWeight.SetValue(2.5, biogears::MassUnit::g);
  cycling.PowerCycle.SetValue(2.5, biogears::PowerUnit::W);
  cycling.CadenceCycle.SetValue(2.5, biogears::FrequencyUnit::Hz);
  source.SetCyclingExercise(cycling);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);

  /*   Generic Exercise */
  data = CDMType {};
  source.Clear();
  source.SetComment("SEGeneric");
  biogears::SEExercise::SEGeneric generic;

  generic.DesiredWorkRate.SetValue(2.5, biogears::PowerUnit::W);
  generic.Intensity.SetValue(0.5, biogears::NoUnit::unitless);
  source.SetGenericExercise(generic);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);

  /*   Strength  Training*/
  data = CDMType {};
  source.Clear();
  source.SetComment("SEStrengthTraining");
  biogears::SEExercise::SEStrengthTraining strength;

  strength.RepsStrength.SetValue(2.5);
  strength.WeightStrength.SetValue(2.5, biogears::MassUnit::kg);
  source.SetStrengthExercise(strength);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);

  /*   Running  Training*/
  data = CDMType {};
  source.Clear();
  source.SetComment("SERunning");
  biogears::SEExercise::SERunning running;

  running.InclineRun.SetValue(0.1);
  running.AddedWeight.SetValue(2.5, biogears::MassUnit::kg);
  running.SpeedRun.SetValue(2.5, biogears::LengthPerTimeUnit::m_Per_s);
  source.SetRunningExercise(running);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);

  /*   Running  ALL*/
  data = CDMType {};
  source.Clear();
  source.SetComment("SERunning");
  source.SetCyclingExercise(cycling);
  source.SetRunningExercise(running);
  source.SetGenericExercise(generic);
  source.SetStrengthExercise(strength);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
// class SEHemorrhage;
//!
//! TYPE Hemorrhage
//! static void UnMarshall(const CDM::HemorrhageData& in, SEHemorrhage& out);
//! static void Marshall(const SEHemorrhage& in, CDM::HemorrhageData& out);
TEST_F(TEST_FIXTURE_NAME, Hemorrhage)
{
  USING_TYPES(Hemorrhage)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetInitialRate().SetValue(2.5, biogears::VolumePerTimeUnit::mL_Per_s);
  source.GetBleedResistance().SetValue(.5, biogears::FlowResistanceUnit::cmH2O_s_Per_L);
  source.SetCompartment("Fake Compartment");

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
// class SEInfection;
//!
//! TYPE Infection
//! static void UnMarshall(const CDM::InfectionData& in, SEInfection& out);
//! static void Marshall(const SEInfection& in, CDM::InfectionData& out);
TEST_F(TEST_FIXTURE_NAME, Infection)
{
  USING_TYPES(Infection)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetLocation("Fake Compartment");
  source.SetSeverity(biogears::SEInfectionSeverity::Moderate);
  source.GetMinimumInhibitoryConcentration().SetValue(.5, biogears::MassPerVolumeUnit::g_Per_mL);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEIntubation.h>
// class SEIntubation;
//!
//! TYPE Intubation
//! static void UnMarshall(const CDM::IntubationData& in, SEIntubation& out);
//! static void Marshall(const SEIntubation& in, CDM::IntubationData& out);
TEST_F(TEST_FIXTURE_NAME, Intubation)
{
  USING_TYPES(Intubation)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetType(biogears::SEIntubationType::LeftMainstem);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
// class SEMechanicalVentilation;
//!
//! TYPE MechanicalVentilation
//!     static void UnMarshall(const CDM::MechanicalVentilationData& in, const SESubstanceManager& substances, SEMechanicalVentilation& out);
//!     static void Marshall(const SEMechanicalVentilation& in, CDM::MechanicalVentilationData& out);
TEST_F(TEST_FIXTURE_NAME, MechanicalVentilation)
{
  USING_TYPES(MechanicalVentilation)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetState(biogears::SEOnOff::Off);
  source.GetFlow().SetValue(1.23, biogears::VolumePerTimeUnit::L_Per_s);
  source.GetPressure().SetValue(1.23, biogears::PressureUnit::cmH2O);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, mgr, sink);

  EXPECT_EQ(source, sink);
}
#include <biogears/cdm/patient/actions/SENasalCannula.h>
// class SENasalCannula;
//!
//! TYPE NasalCannula
//! static void UnMarshall(const CDM::NasalCannulaData& in, SENasalCannula& out);
//! static void Marshall(const SENasalCannula& in, CDM::NasalCannulaData& out);
TEST_F(TEST_FIXTURE_NAME, NasalCannula)
{
  USING_TYPES(NasalCannula)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetFlowRate().SetValue(1.23, biogears::VolumePerTimeUnit::L_Per_s);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SENeedleDecompression.h>
// class SENeedleDecompression;
//!
//! TYPE NeedleDecompression
//! static void UnMarshall(const CDM::NeedleDecompressionData& in, SENeedleDecompression& out);
//! static void Marshall(const SENeedleDecompression& in, CDM::NeedleDecompressionData& out);
//!
TEST_F(TEST_FIXTURE_NAME, NeedleDecompression)
{
  USING_TYPES(NeedleDecompression)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetSide(biogears::SESide::Left);
  source.SetActive(true);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEPainStimulus.h>
// class SEPainStimulus;
//!
//! TYPE PainStimulus
//! static void UnMarshall(const CDM::PainStimulusData& in, SEPainStimulus& out);
//! static void Marshall(const SEPainStimulus& in, CDM::PainStimulusData& out);
TEST_F(TEST_FIXTURE_NAME, PainStimulus)
{
  USING_TYPES(PainStimulus)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetLocation("Fake Compartment");
  source.GetSeverity().SetValue(0.5);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEPericardialEffusion.h>
// class SEPericardialEffusion;
//!
//! TYPE PericardialEffusion
//! static void UnMarshall(const CDM::PericardialEffusionData& in, SEPericardialEffusion& out);
//! static void Marshall(const SEPericardialEffusion& in, CDM::PericardialEffusionData& out);
TEST_F(TEST_FIXTURE_NAME, PericardialEffusion)
{
  USING_TYPES(PericardialEffusion)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetEffusionRate().SetValue(0.5, biogears::VolumePerTimeUnit::L_Per_s);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEPulmonaryShunt.h>
// class SEPulmonaryShunt;
//!
//! TYPE PulmonaryShunt
//! static void UnMarshall(const CDM::PulmonaryShuntData& in, SEPulmonaryShunt& out);
//! static void Marshall(const SEPulmonaryShunt& in, CDM::PulmonaryShuntData& out);
TEST_F(TEST_FIXTURE_NAME, PulmonaryShunt)
{
  USING_TYPES(PulmonaryShunt)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetFlowRateScale().SetValue(0.6);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
// class SESubstanceBolus;
//!
//! TYPE SubstanceBolus
//! static void UnMarshall(const CDM::SubstanceBolusData& in, SESubstanceBolus& out);
//! static void Marshall(const SESubstanceBolus& in, CDM::SubstanceBolusData& out);
TEST_F(TEST_FIXTURE_NAME, SubstanceBolus)
{
  USING_TYPES(SubstanceBolus)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  auto oxygen = mgr.GetSubstance("Oxygen");

  SEType source { *oxygen }, sink { *oxygen };
  CDMType data;

  source.SetComment("Test Comment");
  source.SetAdminRoute(biogears::SEBolusAdministration::Intramuscular);
  source.GetAdminTime().SetValue(2.9, biogears::TimeUnit::s);
  source.GetDose().SetValue(1.9, biogears::VolumeUnit::dL);
  source.GetConcentration().SetValue(2.0, biogears::MassPerVolumeUnit::mg_Per_m3);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/substance/SESubstanceManager.h>
// class SESubstanceBolusState;
//!
//! TYPE SubstanceBolusState
//! static void UnMarshall(const CDM::SubstanceBolusStateData& in, SESubstanceBolusState& out);
//! static void Marshall(const SESubstanceBolusState& in, CDM::SubstanceBolusStateData& out);
TEST_F(TEST_FIXTURE_NAME, SubstanceBolusState)
{
  USING_TYPES(SubstanceBolusState)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  auto oxygen = mgr.GetSubstance("Oxygen");

  SEType source { *oxygen }, sink { *oxygen };
  CDMType data;

  source.GetAdministeredDose().SetValue(0.314, biogears::VolumeUnit::dL);
  source.GetElapsedTime().SetValue(0.314, biogears::TimeUnit::min);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
// class SESubstanceCompoundInfusion;
//!
//! TYPE SubstanceCompoundInfusion
//! static void UnMarshall(const CDM::SubstanceCompoundInfusionData& in, SESubstanceCompoundInfusion& out);
//! static void Marshall(const SESubstanceCompoundInfusion& in, CDM::SubstanceCompoundInfusionData& out);
TEST_F(TEST_FIXTURE_NAME, SubstanceCompoundInfusion)
{
  USING_TYPES(SubstanceCompoundInfusion)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  auto saline = mgr.GetCompound("Saline");
  ASSERT_TRUE(saline != nullptr);
  SEType source { *saline }, sink { *saline };
  CDMType data;

  source.SetComment("Test Comment");
  source.GetBagVolume().SetValue(0.314, biogears::VolumeUnit::dL);
  source.GetRate().SetValue(0.314, biogears::VolumePerTimeUnit::mL_Per_s);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
// class SESubstanceInfusion;
//!
//! TYPE SubstanceInfusion
//! static void UnMarshall(const CDM::SubstanceInfusionData& in, SESubstanceInfusion& out);
//! static void Marshall(const SESubstanceInfusion& in, CDM::SubstanceInfusionData& out);
TEST_F(TEST_FIXTURE_NAME, SubstanceInfusion)
{
  USING_TYPES(SubstanceInfusion)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  auto oxygen = mgr.GetSubstance("Oxygen");
  SEType source { *oxygen }, sink { *oxygen };
  CDMType data;

  source.SetComment("Test Comment");
  source.GetConcentration().SetValue(0.314, biogears::MassPerVolumeUnit::g_Per_m3);
  source.GetRate().SetValue(0.314, biogears::VolumePerTimeUnit::mL_Per_s);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SESleep.h>
// class SESleep;
//!
//! TYPE Sleep
//! static void UnMarshall(const CDM::SleepData& in, SESleep& out);
//! static void Marshall(const SESleep& in, CDM::SleepData& out);
TEST_F(TEST_FIXTURE_NAME, Sleep)
{
  USING_TYPES(Sleep)

  biogears::Logger logger;
  SEType source {}, sink {};
  CDMType data;

  source.SetComment("Test Comment");
  source.SetSleepState(biogears::SEOnOff::On);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SESubstanceNasalDose.h>
// class SESubstanceNasalDose;
//!
//! TYPE SubstanceNasalDose
//! static void UnMarshall(const CDM::SubstanceNasalDoseData& in, SESubstanceNasalDose& out);
//! static void Marshall(const SESubstanceNasalDose& in, CDM::SubstanceNasalDoseData& out);
TEST_F(TEST_FIXTURE_NAME, SubstanceNasalDose)
{
  USING_TYPES(SubstanceNasalDose)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  auto naloxone = mgr.GetSubstance("Naloxone");
  SEType source { *naloxone }, sink { *naloxone };
  CDMType data;

  source.SetComment("Test Comment");
  source.GetDose().SetValue(2.0, biogears::MassUnit::kg);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class NasalState;
//!
//! TYPE SENasalState
//! static void UnMarshall(const CDM::NasalStateData& in, SENasalState& out);
//! static void Marshall(const SENasalState& in, CDM::NasalStateData& out);
TEST_F(TEST_FIXTURE_NAME, NasalState)
{
  USING_TYPES(NasalState)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  auto naloxone = mgr.GetSubstance("Naloxone");
  SEType source { *naloxone }, sink { *naloxone };
  CDMType data;

  source.GetTotalNasalDose().SetValue(1.1, biogears::MassUnit::kg);
  // source.GetVenaCavaConcentration().SetValue(2.2, biogears::MassPerVolumeUnit::g_Per_L);
  source.GetUnreleasedNasalMasses().emplace_back(3.2, biogears::MassUnit::g);
  source.GetReleasedNasalMasses().emplace_back(4.3, biogears::MassUnit::g);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
// class SESubstanceOralDose;
//!
//! TYPE SubstanceOralDose
//! static void UnMarshall(const CDM::SubstanceOralDoseData& in, SESubstanceOralDose& out);
//! static void Marshall(const SESubstanceOralDose& in, CDM::SubstanceOralDoseData& out);
TEST_F(TEST_FIXTURE_NAME, SubstanceOralDose)
{
  USING_TYPES(SubstanceOralDose)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  auto naloxone = mgr.GetSubstance("Naloxone");
  SEType source { *naloxone }, sink { *naloxone };
  CDMType data;

  source.SetComment("Test Comment");
  source.GetDose().SetValue(2.0, biogears::MassUnit::kg);
  source.SetAdminRoute(biogears::SEOralAdministrationType::Transmucosal);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SETransmucosalState;
//!
//! TYPE TransmucosalState
//! static void UnMarshall(const CDM::TransmucosalStateData& in, SETransmucosalState& out);
//! static void Marshall(const SETransmucosalState& in, CDM::TransmucosalStateData& out);
TEST_F(TEST_FIXTURE_NAME, TransmucosalState)
{
  USING_TYPES(TransmucosalState)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  auto naloxone = mgr.GetSubstance("Naloxone");
  SEType source { *naloxone }, sink { *naloxone };
  CDMType data;

  source.GetMouthSolidMass().SetValue(1.1, biogears::MassUnit::kg);
  source.GetSalivaConcentration().SetValue(2.2, biogears::MassPerVolumeUnit::g_Per_L);
  source.GetBuccalConcentrations().emplace_back(3.2, biogears::MassPerVolumeUnit::g_Per_L);
  source.GetSublingualConcentrations().emplace_back(4.3, biogears::MassPerVolumeUnit::g_Per_L);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SETourniquet.h>
// class SETourniquet;
//!
//! TYPE Tourniquet
//! static void UnMarshall(const CDM::TourniquetData& in, SETourniquet& out);
//! static void Marshall(const SETourniquet& in, CDM::TourniquetData& out);
//!
TEST_F(TEST_FIXTURE_NAME, Tourniquet)
{
  USING_TYPES(Tourniquet)

  biogears::Logger logger;
  SEType source {}, sink {};
  CDMType data;

  source.SetComment("Test Comment");
  source.SetCompartment("InferiorAnteriorLateralDistalLimb");
  source.SetTourniquetLevel(biogears::SETourniquetApplicationType::Misapplied);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
// class SETensionPneumothorax;
//!
//! TYPE TensionPneumothorax
//!
//! static void UnMarshall(const CDM::TensionPneumothoraxData& in, SETensionPneumothorax& out);
//! static void Marshall(const SETensionPneumothorax& in, CDM::TensionPneumothoraxData& out);
TEST_F(TEST_FIXTURE_NAME, TensionPneumothorax)
{
  USING_TYPES(TensionPneumothorax)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().SetValue(0.5);
  source.SetSide(biogears::SESide::Left);
  source.SetType(biogears::SEPneumothoraxType::Closed);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEUrinate.h>
// class SEUrinate;
//!
//! TYPE Urinate
//! static void UnMarshall(const CDM::UrinateData& in, SEUrinate& out);
//! static void Marshall(const SEUrinate& in, CDM::UrinateData& out);
TEST_F(TEST_FIXTURE_NAME, Urinate)
{
  USING_TYPES(Urinate)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEOverride.h>
// class SEOverride;
//!
//! TYPE Override
//! static void UnMarshall(const CDM::OverrideData& in, SEOverride& out);
//! static void Marshall(const SEOverride& in, CDM::OverrideData& out);
TEST_F(TEST_FIXTURE_NAME, Override)
{
  USING_TYPES(Override)

  SEType source, sink;
  CDMType data;

  source.GetBloodVolumeOverride().SetValue(3.14159, biogears::VolumeUnit::mL);

  EXPECT_NE(source, sink);

  PatientActions::Marshall(source, data);
  PatientActions::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
