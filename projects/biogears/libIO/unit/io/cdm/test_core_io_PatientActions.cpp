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

#include <biogears/cdm/utils/Logger.h>

#include <io/cdm/PatientActions.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_io_Property_TEST
#define TEST_FIXTURE_NAME DISABLED_PatientActionsFixture
#else
#define TEST_FIXTURE_NAME PatientActionsFixture
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

//class SEPatientAction;
//!
//! TYPE PatientAction
//! static void Marshall(const CDM::PatientActionData& in, SEPatientAction& out);
//! static void UnMarshall(const SEPatientAction& in, CDM::PatientActionData& out);
TEST_F(TEST_FIXTURE_NAME, PatientAction)
{
  //USING_TYPES(PatientAction)

  //SEType source, sink;
  //CDMType data;

  ////source.SetValue(3.14159, biogears::VolumePerTimeMassUnit::mL_Per_s_kg);

  //EXPECT_NE(source, sink);

  //PatientActions::UnMarshall(source, data);
  //PatientActions::Marshall(data, sink);

  //EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
//class SEPatientAssessmentRequest;
//!
//! TYPE PatientAssessmentRequest
//! static void Marshall(const CDM::PatientAssessmentRequestData& in, SEPatientAssessmentRequest& out);
//! static void UnMarshall(const SEPatientAssessmentRequest& in, CDM::PatientAssessmentRequestData& out);
TEST_F(TEST_FIXTURE_NAME, PatientAssessmentRequest)
{
  USING_TYPES(PatientAssessmentRequest)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetType(CDM::enumPatientAssessment::CompleteBloodCount);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEAcuteStress.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
//class SEAcuteStress;
//!
//! TYPE AcuteStress
//! static void Marshall(const CDM::AcuteStressData& in, SEAcuteStress& out);
//! static void UnMarshall(const SEAcuteStress& in, CDM::AcuteStressData& out);
TEST_F(TEST_FIXTURE_NAME, AcuteStress)
{
  USING_TYPES(AcuteStress)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEAirwayObstruction.h>
//class SEAirwayObstruction;
//!
//! TYPE AirwayObstruction
//! static void Marshall(const CDM::AirwayObstructionData& in, SEAirwayObstruction& out);
//! static void UnMarshall(const SEAirwayObstruction& in, CDM::AirwayObstructionData& out);
TEST_F(TEST_FIXTURE_NAME, AirwayObstruction)
{
  USING_TYPES(AirwayObstruction)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEApnea.h>
//class SEApnea;
//!
//! TYPE Apnea
//! static void Marshall(const CDM::ApneaData& in, SEApnea& out);
//! static void UnMarshall(const SEApnea& in, CDM::ApneaData& out);
TEST_F(TEST_FIXTURE_NAME, Apnea)
{
  USING_TYPES(Apnea)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
//class SEAsthmaAttack;
//!
//! TYPE AsthmaAttack
//! static void Marshall(const CDM::AsthmaAttackData& in, SEAsthmaAttack& out);
//! static void UnMarshall(const SEAsthmaAttack& in, CDM::AsthmaAttackData& out);
TEST_F(TEST_FIXTURE_NAME, AsthmaAttack)
{
  USING_TYPES(AsthmaAttack)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEBrainInjury.h>
//class SEBrainInjury;
//!
//! TYPE BrainInjury
//! static void Marshall(const CDM::BrainInjuryData& in, SEBrainInjury& out);
//! static void UnMarshall(const SEBrainInjury& in, CDM::BrainInjuryData& out);
TEST_F(TEST_FIXTURE_NAME, BrainInjury)
{
  USING_TYPES(BrainInjury)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);
  source.SetType(CDM::enumBrainInjuryType::LeftFocal);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEBronchoconstriction.h>
//class SEBronchoconstriction;
//!
//! TYPE Bronchoconstriction
//! static void Marshall(const CDM::BronchoconstrictionData& in, SEBronchoconstriction& out);
//! static void UnMarshall(const SEBronchoconstriction& in, CDM::BronchoconstrictionData& out);
TEST_F(TEST_FIXTURE_NAME, Bronchoconstriction)
{
  USING_TYPES(Bronchoconstriction)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEBurnWound.h>
//class SEBurnWound;
//!
//! TYPE BurnWound
//! static void Marshall(const CDM::BurnWoundData& in, SEBurnWound& out);
//! static void UnMarshall(const SEBurnWound& in, CDM::BurnWoundData& out);
TEST_F(TEST_FIXTURE_NAME, BurnWound)
{
  USING_TYPES(BurnWound)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetTotalBodySurfaceArea().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SECardiacArrest.h>
//class SECardiacArrest;
//!
//! TYPE CardiacArrest
//! static void Marshall(const CDM::CardiacArrestData& in, SECardiacArrest& out);
//! static void UnMarshall(const SECardiacArrest& in, CDM::CardiacArrestData& out);
TEST_F(TEST_FIXTURE_NAME, CardiacArrest)
{
  USING_TYPES(CardiacArrest)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEChestCompression.h>
//class SEChestCompression;
//!
//! TYPE ChestCompression
//! static void Marshall(const CDM::ChestCompressionData& in, SEChestCompression& out);
//! static void UnMarshall(const SEChestCompression& in, CDM::ChestCompressionData& out);
TEST_F(TEST_FIXTURE_NAME, ChestCompression)
{
  USING_TYPES(ChestCompression)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
#include <biogears/cdm/properties/SEScalarForce.h>
//class SEChestCompressionForce;
//!
//! TYPE ChestCompressionForce
//! static void Marshall(const CDM::ChestCompressionForceData& in, SEChestCompressionForce& out);
//! static void UnMarshall(const SEChestCompressionForce& in, CDM::ChestCompressionForceData& out);
TEST_F(TEST_FIXTURE_NAME, ChestCompressionForce)
{
  USING_TYPES(ChestCompressionForce)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetForce().SetValue(.79, biogears::ForceUnit::dyn);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEChestCompressionForceScale.h>
#include <biogears/cdm/properties/SEScalarTime.h>
//class SEChestCompressionForceScale;
//!
//! TYPE ChestCompressionForceScale
//! static void Marshall(const CDM::ChestCompressionForceScaleData& in, SEChestCompressionForceScale& out);
//! static void UnMarshall(const SEChestCompressionForceScale& in, CDM::ChestCompressionForceScaleData& out);
TEST_F(TEST_FIXTURE_NAME, ChestCompressionForceScale)
{
  USING_TYPES(ChestCompressionForceScale)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetForcePeriod().SetValue(.79, biogears::TimeUnit::hr);
  source.GetForceScale().Set(.79);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEChestOcclusiveDressing.h>
//class SEChestOcclusiveDressing;
//!
//! TYPE ChestOcclusiveDressing
//! static void Marshall(const CDM::ChestOcclusiveDressingData& in, SEChestOcclusiveDressing& out);
//! static void UnMarshall(const SEChestOcclusiveDressing& in, CDM::ChestOcclusiveDressingData& out);
TEST_F(TEST_FIXTURE_NAME, ChestOcclusiveDressing)
{
  USING_TYPES(ChestOcclusiveDressing)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetSide(CDM::enumSide::Right);
  source.SetActive(true);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEForcedInhale.h>
//class SEForcedInhale;
//!
//! TYPE ForcedInhale
//! static void Marshall(const CDM::ForcedInhaleData& in, SEForcedInhale& out);
//! static void UnMarshall(const SEForcedInhale& in, CDM::ForcedInhaleData& out);
TEST_F(TEST_FIXTURE_NAME, ForcedInhale)
{
  USING_TYPES(ForcedInhale)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetPeriod().SetValue(1, biogears::TimeUnit::s);
  source.GetInspiratoryCapacityFraction().Set(0.72);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEForcedExhale.h>
//class SEForcedExhale;
//!
//! TYPE ForcedExhale
//! static void Marshall(const CDM::ForcedExhaleData& in, SEForcedExhale& out);
//! static void UnMarshall(const SEForcedExhale& in, CDM::ForcedExhaleData& out);
TEST_F(TEST_FIXTURE_NAME, ForcedExhale)
{
  USING_TYPES(ForcedExhale)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetPeriod().SetValue(1, biogears::TimeUnit::s);
  source.GetExpiratoryReserveVolumeFraction().SetValue(.72);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEBreathHold.h>
//class SEBreathHold;
//!
//! TYPE BreathHold
//! static void Marshall(const CDM::BreathHoldData& in, SEBreathHold& out);
//! static void UnMarshall(const SEBreathHold& in, CDM::BreathHoldData& out);
TEST_F(TEST_FIXTURE_NAME, BreathHold)
{
  USING_TYPES(BreathHold)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetPeriod().SetValue(1, biogears::TimeUnit::s);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEUseInhaler.h>
//class SEUseInhaler;
//!
//! TYPE UseInhaler
//! static void Marshall(const CDM::UseInhalerData& in, SEUseInhaler& out);
//! static void UnMarshall(const SEUseInhaler& in, CDM::UseInhalerData& out);
TEST_F(TEST_FIXTURE_NAME, UseInhaler)
{
  USING_TYPES(UseInhaler)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEConsciousRespiration.h>
//class SEConsciousRespiration;
//!
//! TYPE ConsciousRespiration
//! static void Marshall(const CDM::ConsciousRespirationData& in, SEConsciousRespiration& out);
//! static void UnMarshall(const SEConsciousRespiration& in, CDM::ConsciousRespirationData& out);
TEST_F(TEST_FIXTURE_NAME, ConsciousRespiration)
{
  USING_TYPES(ConsciousRespiration)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
//class SEConsumeNutrients;
//!
//! TYPE ConsumeNutrients
//! static void Marshall(const CDM::ConsumeNutrientsData& in, SEConsumeNutrients& out);
//! static void UnMarshall(const SEConsumeNutrients& in, CDM::ConsumeNutrientsData& out);
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

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEExercise.h>
//class SEExercise;
//!
//! TYPE Exercise
//! static void Marshall(const CDM::ExerciseData& in, SEExercise& out);
//! static void UnMarshall(const SEExercise& in, CDM::ExerciseData& out);
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

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

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

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

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

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

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

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

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

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
//class SEHemorrhage;
//!
//! TYPE Hemorrhage
//! static void Marshall(const CDM::HemorrhageData& in, SEHemorrhage& out);
//! static void UnMarshall(const SEHemorrhage& in, CDM::HemorrhageData& out);
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

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
//class SEInfection;
//!
//! TYPE Infection
//! static void Marshall(const CDM::InfectionData& in, SEInfection& out);
//! static void UnMarshall(const SEInfection& in, CDM::InfectionData& out);
TEST_F(TEST_FIXTURE_NAME, Infection)
{
  USING_TYPES(Infection)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetLocation("Fake Compartment");
  source.SetSeverity(CDM::enumInfectionSeverity::Moderate);
  source.GetMinimumInhibitoryConcentration().SetValue(.5, biogears::MassPerVolumeUnit::g_Per_mL);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEIntubation.h>
//class SEIntubation;
//!
//! TYPE Intubation
//! static void Marshall(const CDM::IntubationData& in, SEIntubation& out);
//! static void UnMarshall(const SEIntubation& in, CDM::IntubationData& out);
TEST_F(TEST_FIXTURE_NAME, Intubation)
{
  USING_TYPES(Intubation)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetType(CDM::enumIntubationType::LeftMainstem);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
//class SEMechanicalVentilation;
//!
//! TYPE MechanicalVentilation
//!     static void Marshall(const CDM::MechanicalVentilationData& in, const SESubstanceManager& substances, SEMechanicalVentilation& out);
//!     static void UnMarshall(const SEMechanicalVentilation& in, CDM::MechanicalVentilationData& out);
TEST_F(TEST_FIXTURE_NAME, MechanicalVentilation)
{
  USING_TYPES(MechanicalVentilation)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetState(CDM::enumOnOff::Off);
  source.GetFlow().SetValue(1.23, biogears::VolumePerTimeUnit::L_Per_s);
  source.GetPressure().SetValue(1.23, biogears::PressureUnit::cmH2O);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, mgr, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SENeedleDecompression.h>
//class SENeedleDecompression;
//!
//! TYPE NeedleDecompression
//! static void Marshall(const CDM::NeedleDecompressionData& in, SENeedleDecompression& out);
//! static void UnMarshall(const SENeedleDecompression& in, CDM::NeedleDecompressionData& out);
TEST_F(TEST_FIXTURE_NAME, NeedleDecompression)
{
  USING_TYPES(NeedleDecompression)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetSide(CDM::enumSide::Left);
  source.SetActive(true);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEPainStimulus.h>
//class SEPainStimulus;
//!
//! TYPE PainStimulus
//! static void Marshall(const CDM::PainStimulusData& in, SEPainStimulus& out);
//! static void UnMarshall(const SEPainStimulus& in, CDM::PainStimulusData& out);
TEST_F(TEST_FIXTURE_NAME, PainStimulus)
{
  USING_TYPES(PainStimulus)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.SetLocation("Fake Compartment");
  source.GetSeverity().SetValue(0.5);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEPericardialEffusion.h>
//class SEPericardialEffusion;
//!
//! TYPE PericardialEffusion
//! static void Marshall(const CDM::PericardialEffusionData& in, SEPericardialEffusion& out);
//! static void UnMarshall(const SEPericardialEffusion& in, CDM::PericardialEffusionData& out);
TEST_F(TEST_FIXTURE_NAME, PericardialEffusion)
{
  USING_TYPES(PericardialEffusion)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetEffusionRate().SetValue(0.5, biogears::VolumePerTimeUnit::L_Per_s);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
//class SETensionPneumothorax;
//!
//! TYPE TensionPneumothorax
//!
//! static void Marshall(const CDM::TensionPneumothoraxData& in, SETensionPneumothorax& out);
//! static void UnMarshall(const SETensionPneumothorax& in, CDM::TensionPneumothoraxData& out);
TEST_F(TEST_FIXTURE_NAME, TensionPneumothorax)
{
  USING_TYPES(TensionPneumothorax)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");
  source.GetSeverity().SetValue(0.5);
  source.SetSide(CDM::enumSide::Left);
  source.SetType(CDM::enumPneumothoraxType::Closed);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
//class SESubstanceBolus;
//!
//! TYPE SubstanceBolus
//! static void Marshall(const CDM::SubstanceBolusData& in, SESubstanceBolus& out);
//! static void UnMarshall(const SESubstanceBolus& in, CDM::SubstanceBolusData& out);
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
  source.SetAdminRoute(CDM::enumBolusAdministration::Intramuscular);
  source.GetAdminTime().SetValue(2.9, biogears::TimeUnit::s);
  source.GetDose().SetValue(1.9, biogears::VolumeUnit::dL);
  source.GetConcentration().SetValue(2.0, biogears::MassPerVolumeUnit::mg_Per_m3);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/substance/SESubstanceManager.h>
//class SESubstanceBolusState;
//!
//! TYPE SubstanceBolusState
//! static void Marshall(const CDM::SubstanceBolusStateData& in, SESubstanceBolusState& out);
//! static void UnMarshall(const SESubstanceBolusState& in, CDM::SubstanceBolusStateData& out);
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

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
//class SESubstanceCompoundInfusion;
//!
//! TYPE SubstanceCompoundInfusion
//! static void Marshall(const CDM::SubstanceCompoundInfusionData& in, SESubstanceCompoundInfusion& out);
//! static void UnMarshall(const SESubstanceCompoundInfusion& in, CDM::SubstanceCompoundInfusionData& out);
TEST_F(TEST_FIXTURE_NAME, SubstanceCompoundInfusion)
{
  USING_TYPES(SubstanceCompoundInfusion)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  auto oxygen = mgr.GetCompound("Saline");
  SEType source { *oxygen }, sink { *oxygen };
  CDMType data;

  source.SetComment("Test Comment");
  source.GetBagVolume().SetValue(0.314, biogears::VolumeUnit::dL);
  source.GetRate().SetValue(0.314, biogears::VolumePerTimeUnit::mL_Per_s);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
//class SESubstanceInfusion;
//!
//! TYPE SubstanceInfusion
//! static void Marshall(const CDM::SubstanceInfusionData& in, SESubstanceInfusion& out);
//! static void UnMarshall(const SESubstanceInfusion& in, CDM::SubstanceInfusionData& out);
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

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEUrinate.h>
//class SEUrinate;
//!
//! TYPE Urinate
//! static void Marshall(const CDM::UrinateData& in, SEUrinate& out);
//! static void UnMarshall(const SEUrinate& in, CDM::UrinateData& out);
TEST_F(TEST_FIXTURE_NAME, Urinate)
{
  USING_TYPES(Urinate)

  SEType source, sink;
  CDMType data;

  source.SetComment("Test Comment");

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}

#include <biogears/cdm/patient/actions/SEOverride.h>
//class SEOverride;
//!
//! TYPE Override
//! static void Marshall(const CDM::OverrideData& in, SEOverride& out);
//! static void UnMarshall(const SEOverride& in, CDM::OverrideData& out);
TEST_F(TEST_FIXTURE_NAME, Override)
{
  USING_TYPES(Override)

  SEType source, sink;
  CDMType data;

  source.GetBloodVolumeOverride().SetValue(3.14159, biogears::VolumeUnit::mL);

  EXPECT_NE(source, sink);

  PatientActions::UnMarshall(source, data);
  PatientActions::Marshall(data, sink);

  EXPECT_EQ(source, sink);
}
