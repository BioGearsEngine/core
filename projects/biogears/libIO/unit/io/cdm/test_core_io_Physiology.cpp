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

#include <io/cdm/Physiology.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_Physiology_TEST
#define TEST_FIXTURE_NAME DISABLED_PhysiologyFixture
#else
#define TEST_FIXTURE_NAME IO_PhysiologyFixture
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

// class SEPupillaryResponse
//! TYPE PupillaryResponse
//! static void Marshall(const CDM::PupillaryResponseData& in, SEPupillaryResponse& out);
//! static void UnMarshall(const SEPupillaryResponse& in, CDM::PupillaryResponseData& out);
#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
TEST_F(TEST_FIXTURE_NAME, PupillaryResponse)
{
  USING_TYPES(PupillaryResponse)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetReactivityModifier().SetValue(.4);
  source.GetShapeModifier().SetValue(.9);
  source.GetSizeModifier().SetValue(-.4);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEInflammatoryResponse
//! TYPE InflammatoryResponse
//! static void Marshall(const CDM::InflammatoryResponseData& in, SEInflammatoryResponse& out);
//! static void UnMarshall(const SEInflammatoryResponse& in, CDM::InflammatoryResponseData& out);
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
TEST_F(TEST_FIXTURE_NAME, InflammatoryResponse)
{

  USING_TYPES(InflammatoryResponse)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetLocalPathogen().SetValue(0.14);
  source.GetLocalMacrophage().SetValue(0.13);
  source.GetLocalNeutrophil().SetValue(0.12);
  source.GetLocalBarrier().SetValue(0.11);
  source.SetActiveTLR(CDM::enumOnOff::Off);
  source.GetAutonomicResponseLevel().SetValue(0.34);
  source.GetCatecholamines().SetValue(0.33);
  source.GetConstitutiveNOS().SetValue(0.32);
  source.GetInducibleNOSPre().SetValue(0.31);
  source.GetInducibleNOS().SetValue(0.30);
  source.GetInterleukin6().SetValue(0.29);
  source.GetInterleukin10().SetValue(0.28);
  source.GetInterleukin12().SetValue(0.27);
  source.GetMacrophageResting().SetValue(0.26);
  source.GetMacrophageActive().SetValue(0.25);
  source.GetNeutrophilResting().SetValue(0.24);
  source.GetNeutrophilActive().SetValue(0.23);
  source.GetNitrate().SetValue(0.22);
  source.GetNitricOxide().SetValue(0.21);
  source.GetBloodPathogen().SetValue(0.20);
  source.GetTissueIntegrity().SetValue(0.99);
  source.GetTrauma().SetValue(0.98);
  source.GetTumorNecrosisFactor().SetValue(0.97);
  source.GetInflammationTime().SetValue(0.9, biogears::TimeUnit::s);
  source.GetInflammationSources().push_back(CDM::enumInflammationSource::Hemorrhage);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEBloodChemistrySystem
//! TYPE BloodChemistrySystem
//! static void Marshall(const CDM::BloodChemistrySystemData& in, SEBloodChemistrySystem& out);
//! static void UnMarshall(const SEBloodChemistrySystem& in, CDM::BloodChemistrySystemData& out);
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
TEST_F(TEST_FIXTURE_NAME, BloodChemistrySystem)
{
  USING_TYPES(BloodChemistrySystem)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetArterialBloodPH().SetValue(0.1);
  source.GetArterialBloodPHBaseline().SetValue(0.1);
  source.GetVenousBloodPH().SetValue(0.1);
  source.GetCarbonDioxideSaturation().SetValue(0.1);
  source.GetCarbonMonoxideSaturation().SetValue(0.1);
  source.GetHematocrit().SetValue(0.1);
  source.GetOxygenSaturation().SetValue(0.1);
  source.GetOxygenVenousSaturation().SetValue(0.1);
  source.GetPulseOximetry().SetValue(0.1);
  source.GetShuntFraction().SetValue(0.1);
  source.GetVolumeFractionNeutralPhospholipidInPlasma().SetValue(0.1);
  source.GetVolumeFractionNeutralLipidInPlasma().SetValue(0.1);
  source.GetHemoglobinContent().SetValue(0.1, biogears::MassUnit::kg);
  source.GetHemoglobinLostToUrine().SetValue(0.1, biogears::MassUnit::kg);
  source.GetBloodDensity().SetValue(0.1, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetBloodUreaNitrogenConcentration().SetValue(0.1, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetTotalBilirubin().SetValue(0.1, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetTotalProteinConcentration().SetValue(0.1, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetPlasmaVolume().SetValue(0.1, biogears::VolumeUnit::L);
  source.GetRhTransfusionReactionVolume().SetValue(0.1, biogears::VolumeUnit::L);
  source.GetLymphocyteCellCount().SetValue(0.1, biogears::AmountPerVolumeUnit::ct_Per_L);
  source.GetNeutrophilCellCount().SetValue(0.1, biogears::AmountPerVolumeUnit::ct_Per_L);
  source.GetPhosphate().SetValue(0.1, biogears::AmountPerVolumeUnit::ct_Per_L);
  source.GetRedBloodCellAcetylcholinesterase().SetValue(0.1, biogears::AmountPerVolumeUnit::ct_Per_L);
  source.GetRedBloodCellCount().SetValue(0.1, biogears::AmountPerVolumeUnit::ct_Per_L);
  source.GetStrongIonDifference().SetValue(0.1, biogears::AmountPerVolumeUnit::ct_Per_L);
  source.GetViralLoad().SetValue(0.1, biogears::AmountPerVolumeUnit::ct_Per_L);
  source.GetWhiteBloodCellCount().SetValue(0.1, biogears::AmountPerVolumeUnit::ct_Per_L);
  source.GetBloodSpecificHeat().SetValue(0.1, biogears::HeatCapacitancePerMassUnit::kJ_Per_K_kg);
  source.GetArterialCarbonDioxidePressure().SetValue(0.1, biogears::PressureUnit::mmHg);
  source.GetArterialOxygenPressure().SetValue(0.1, biogears::PressureUnit::mmHg);
  source.GetPulmonaryArterialCarbonDioxidePressure().SetValue(0.1, biogears::PressureUnit::mmHg);
  source.GetPulmonaryArterialOxygenPressure().SetValue(0.1, biogears::PressureUnit::mmHg);
  source.GetPulmonaryVenousCarbonDioxidePressure().SetValue(0.1, biogears::PressureUnit::mmHg);
  source.GetPulmonaryVenousOxygenPressure().SetValue(0.1, biogears::PressureUnit::mmHg);
  source.GetVenousCarbonDioxidePressure().SetValue(0.1, biogears::PressureUnit::mmHg);
  source.GetVenousOxygenPressure().SetValue(0.1, biogears::PressureUnit::mmHg);

  source.GetInflammatoryResponse().GetLocalPathogen().SetValue(0.14);
  source.GetInflammatoryResponse().GetLocalMacrophage().SetValue(0.13);
  source.GetInflammatoryResponse().GetLocalNeutrophil().SetValue(0.12);
  source.GetInflammatoryResponse().GetLocalBarrier().SetValue(0.11);
  source.GetInflammatoryResponse().SetActiveTLR(CDM::enumOnOff::Off);
  source.GetInflammatoryResponse().GetAutonomicResponseLevel().SetValue(0.34);
  source.GetInflammatoryResponse().GetCatecholamines().SetValue(0.33);
  source.GetInflammatoryResponse().GetConstitutiveNOS().SetValue(0.32);
  source.GetInflammatoryResponse().GetInducibleNOSPre().SetValue(0.31);
  source.GetInflammatoryResponse().GetInducibleNOS().SetValue(0.30);
  source.GetInflammatoryResponse().GetInterleukin6().SetValue(0.29);
  source.GetInflammatoryResponse().GetInterleukin10().SetValue(0.28);
  source.GetInflammatoryResponse().GetInterleukin12().SetValue(0.27);
  source.GetInflammatoryResponse().GetMacrophageResting().SetValue(0.26);
  source.GetInflammatoryResponse().GetMacrophageActive().SetValue(0.25);
  source.GetInflammatoryResponse().GetNeutrophilResting().SetValue(0.24);
  source.GetInflammatoryResponse().GetNeutrophilActive().SetValue(0.23);
  source.GetInflammatoryResponse().GetNitrate().SetValue(0.22);
  source.GetInflammatoryResponse().GetNitricOxide().SetValue(0.21);
  source.GetInflammatoryResponse().GetBloodPathogen().SetValue(0.20);
  source.GetInflammatoryResponse().GetTissueIntegrity().SetValue(0.99);
  source.GetInflammatoryResponse().GetTrauma().SetValue(0.98);
  source.GetInflammatoryResponse().GetTumorNecrosisFactor().SetValue(0.97);
  source.GetInflammatoryResponse().GetInflammationTime().SetValue(0.9, biogears::TimeUnit::s);
  source.GetInflammatoryResponse().GetInflammationSources().push_back(CDM::enumInflammationSource::Hemorrhage);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SECardiovascularSystem
//! TYPE CardiovascularSystem
//! static void Marshall(const CDM::CardiovascularSystemData& in, SECardiovascularSystem& out);
//! static void UnMarshall(const SECardiovascularSystem& in, CDM::CardiovascularSystemData& out);
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
TEST_F(TEST_FIXTURE_NAME, CardiovascularSystem)
{
  USING_TYPES(CardiovascularSystem)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetHeartRhythm(CDM::enumHeartRhythm::NormalSinus);
  source.GetBloodVolume().SetValue(3.0, biogears::VolumeUnit::L);

  source.GetCardiacIndex().SetValue(4., biogears::VolumePerTimeAreaUnit::mL_Per_min_m2);

  source.GetCardiacOutput().SetValue(3.0,biogears::VolumePerTimeUnit::L_Per_day);

  source.GetHeartEjectionFraction().SetValue(1.0);

  source.GetHeartRate().SetValue(1.0, biogears::FrequencyUnit::Hz);

  source.GetHeartStrokeVolume().SetValue(1.0, biogears::VolumeUnit::mL);

  source.GetMeanSkinFlow().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetPulmonaryMeanCapillaryFlow().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetPulmonaryMeanShuntFlow().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetCerebralBloodFlow().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);

  source.GetPulmonaryVascularResistanceIndex().SetValue(1.0, biogears::PressureTimePerVolumeAreaUnit::dyn_s_Per_cm5_m2);

  source.GetArterialPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetCentralVenousPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetCerebralPerfusionPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetDiastolicArterialPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetExtremityPressureLeftArm().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetExtremityPressureLeftLeg().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetExtremityPressureRightArm().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetExtremityPressureRightLeg().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetIntracranialPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetMeanArterialPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetMeanArterialCarbonDioxidePartialPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetMeanArterialCarbonDioxidePartialPressureDelta().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetMeanCentralVenousPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetPulmonaryArterialPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetPulmonaryCapillariesWedgePressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetPulmonaryDiastolicArterialPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetPulmonaryMeanArterialPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetPulmonarySystolicArterialPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetPulsePressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetSystolicArterialPressure().SetValue(1.0, biogears::PressureUnit::mmHg);

  source.GetPulmonaryVascularResistance().SetValue(1.0, biogears::FlowResistanceUnit::cmH2O_s_Per_L);
  source.GetSystemicVascularResistance().SetValue(1.0, biogears::FlowResistanceUnit::cmH2O_s_Per_L);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEDrugSystem
//! TYPE DrugSystem
//! static void Marshall(const CDM::DrugSystemData& in, SEDrugSystem& out);
//! static void UnMarshall(const SEDrugSystem& in, CDM::DrugSystemData& out);
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
TEST_F(TEST_FIXTURE_NAME, DrugSystem)
{
  USING_TYPES(DrugSystem)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetAntibioticActivity().SetValue(1.0);

  source.GetHeartRateChange().SetValue(2.0, biogears::FrequencyUnit::Hz);
  source.GetRespirationRateChange().SetValue(3.0, biogears::FrequencyUnit::Hz);

  source.GetBronchodilationLevel().SetValue(0.5);
  source.GetHemorrhageChange().SetValue(0.6);
  source.GetNeuromuscularBlockLevel().SetValue(.4);
  source.GetPainToleranceChange().SetValue(0.3);
  source.GetSedationLevel().SetValue(0.2);
  source.GetTubularPermeabilityChange().SetValue(0.1);
  source.GetCentralNervousResponse().SetValue(0.7);

  source.GetMeanBloodPressureChange().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetPulsePressureChange().SetValue(1.0, biogears::PressureUnit::mmHg);

  source.GetFeverChange().SetValue(1.0, biogears::TemperatureUnit::C);

  source.GetTidalVolumeChange().SetValue(1.0, biogears::VolumeUnit::L);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEEndocrineSystem
//! TYPE EndocrineSystem
//! static void Marshall(const CDM::EndocrineSystemData& in, SEEndocrineSystem& out);
//! static void UnMarshall(const SEEndocrineSystem& in, CDM::EndocrineSystemData& out);
#include <biogears/cdm/system/physiology/SEEndocrineSystem.h>
TEST_F(TEST_FIXTURE_NAME, EndocrineSystem)
{
  USING_TYPES(EndocrineSystem)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetInsulinSynthesisRate().SetValue(1.0, biogears::AmountPerTimeUnit::mmol_Per_min);
  source.GetGlucagonSynthesisRate().SetValue(2.0, biogears::AmountPerTimeUnit::mmol_Per_min);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEEnergySystem
//! TYPE EnergySystem
//! static void Marshall(const CDM::EnergySystemData& in, SEEnergySystem& out);
//! static void UnMarshall(const SEEnergySystem& in, CDM::EnergySystemData& out);
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
TEST_F(TEST_FIXTURE_NAME, EnergySystem)
{
  USING_TYPES(EnergySystem)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetCreatinineProductionRate().SetValue(1.0, biogears::AmountPerTimeUnit::mol_Per_s);
  source.GetLactateProductionRate().SetValue(1.0, biogears::AmountPerTimeUnit::mol_Per_s);

  source.GetFatigueLevel().SetValue(1.0);
  source.GetTotalWorkRateLevel().SetValue(1.0);
  source.GetAchievedExerciseLevel().SetValue(1.0);

  source.GetChlorideLostToSweat().SetValue(1.0, biogears::MassUnit::g);
  source.GetPotassiumLostToSweat().SetValue(1.0, biogears::MassUnit::g);
  source.GetSodiumLostToSweat().SetValue(1.0, biogears::MassUnit::g);

  source.GetExerciseMeanArterialPressureDelta().SetValue(1.0, biogears::PressureUnit::mmHg);

  source.GetTotalMetabolicRate().SetValue(1.0, biogears::PowerUnit::W);
  source.GetEnergyDeficit().SetValue(1.0, biogears::PowerUnit::W);
  source.GetExerciseEnergyDemand().SetValue(1.0, biogears::PowerUnit::W);

  source.GetSkinTemperature().SetValue(1.0, biogears::TemperatureUnit::C);
  source.GetCoreTemperature().SetValue(1.0, biogears::TemperatureUnit::C);

  source.GetSweatRate().SetValue(1.0, biogears::MassPerTimeUnit::g_Per_day);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEGastrointestinalSystem
//! TYPE GastrointestinalSystem
//! static void Marshall(const CDM::GastrointestinalSystemData& in, SEGastrointestinalSystem& out);
//! static void UnMarshall(const SEGastrointestinalSystem& in, CDM::GastrointestinalSystemData& out);
#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>
TEST_F(TEST_FIXTURE_NAME, GastrointestinalSystem)
{
  USING_TYPES(GastrointestinalSystem)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetChymeAbsorptionRate().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_s);

  std::string m_Name;

  source.GetStomachContents().GetCarbohydrate().SetValue(500, biogears::MassUnit::mg);
  source.GetStomachContents().GetFat().SetValue(400, biogears::MassUnit::mg);
  source.GetStomachContents().GetProtein().SetValue(300, biogears::MassUnit::mg);
  source.GetStomachContents().GetCalcium().SetValue(200, biogears::MassUnit::mg);
  source.GetStomachContents().GetSodium().SetValue(100, biogears::MassUnit::mg);

  source.GetStomachContents().GetWater().SetValue(9., biogears::VolumeUnit::mL);

  auto state = std::make_unique<biogears::SEDrugTransitState>(*sarin);

  std::vector<double> lumenSolidMasses = { 1, 2, 3 };
  std::vector<double> lumenDissolvedMasses = { 4, 5, 6 };
  std::vector<double> enterocyteMasses = { 7, 8, 9 };

  state->SetLumenSolidMasses(lumenSolidMasses, biogears::MassUnit::mg);
  state->SetLumenDissolvedMasses(lumenDissolvedMasses, biogears::MassUnit::mg);
  state->SetEnterocyteMasses(enterocyteMasses, biogears::MassUnit::mg);

  state->GetTotalMassMetabolized().SetValue(3, biogears::MassUnit::mg);
  state->GetTotalMassExcreted().SetValue(4, biogears::MassUnit::mg);

  source.GetDrugTransitStates()[sarin] = state.get();

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEHepaticSystem
//! TYPE HepaticSystem
//! static void Marshall(const CDM::HepaticSystemData& in, SEHepaticSystem& out);
//! static void UnMarshall(const SEHepaticSystem& in, CDM::HepaticSystemData& out);
#include <biogears/cdm/system/physiology/SEHepaticSystem.h>
TEST_F(TEST_FIXTURE_NAME, HepaticSystem)
{
  USING_TYPES(HepaticSystem)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetKetoneProductionRate().SetValue(1.0, biogears::AmountPerTimeUnit::mol_Per_day);
  source.GetHepaticGluconeogenesisRate().SetValue(1.0, biogears::MassPerTimeUnit::g_Per_day);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SENervousSystem
//! TYPE NervousSystem
//! static void Marshall(const CDM::NervousSystemData& in, SENervousSystem& out);
//! static void UnMarshall(const SENervousSystem& in, CDM::NervousSystemData& out);
#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
#include <biogears/cdm/system/physiology/SENervousSystem.h>
TEST_F(TEST_FIXTURE_NAME, NervousSystem)
{
  USING_TYPES(NervousSystem)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetSleepState(CDM::enumSleepState::Asleep);

  source.GetComplianceScale().SetValue(1.0);
  source.GetHeartRateScale().SetValue(2.0);
  source.GetHeartElastanceScale().SetValue(3.0);
  source.GetMentalStatus().SetValue(4.0);
  source.GetPainVisualAnalogueScale().SetValue(5.0);
  source.GetResistanceScaleExtrasplanchnic().SetValue(6.0);
  source.GetResistanceScaleMuscle().SetValue(7.0);
  source.GetResistanceScaleMyocardium().SetValue(8.0);
  source.GetResistanceScaleSplanchnic().SetValue(9.0);
  source.GetBiologicalDebt().SetValue(10.0);
  source.GetAttentionLapses().SetValue(11.0);

  source.GetLeftEyePupillaryResponse().GetReactivityModifier().SetValue(.4);
  source.GetLeftEyePupillaryResponse().GetShapeModifier().SetValue(.9);
  source.GetLeftEyePupillaryResponse().GetSizeModifier().SetValue(-.4);

  source.GetRightEyePupillaryResponse().GetReactivityModifier().SetValue(-.4);
  source.GetRightEyePupillaryResponse().GetShapeModifier().SetValue(-.9);
  source.GetRightEyePupillaryResponse().GetSizeModifier().SetValue(.4);

  source.GetSleepTime().SetValue(1.0, biogears::TimeUnit::s);
  source.GetWakeTime().SetValue(10.0, biogears::TimeUnit::s);
  source.GetReactionTime().SetValue(5.0, biogears::TimeUnit::s);

  source.GetRichmondAgitationSedationScale().SetValue(1.0);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SERenalSystem
//! TYPE RenalSystem
//! static void Marshall(const CDM::RenalSystemData& in, SERenalSystem& out);
//! static void UnMarshall(const SERenalSystem& in, CDM::RenalSystemData& out);
#include <biogears/cdm/system/physiology/SERenalSystem.h>
TEST_F(TEST_FIXTURE_NAME, RenalSystem)
{
  USING_TYPES(RenalSystem)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetBladderPressure().SetValue(1.0, biogears::PressureUnit::mmHg);

  source.GetUrineSpecificGravity().SetValue(1.0);

  source.GetLeftGlomerularFiltrationSurfaceArea().SetValue(1.0, biogears::AreaUnit::m2);
  source.GetLeftTubularReabsorptionFiltrationSurfaceArea().SetValue(1.0, biogears::AreaUnit::m2);
  source.GetRightGlomerularFiltrationSurfaceArea().SetValue(1.0, biogears::AreaUnit::m2);
  source.GetRightTubularReabsorptionFiltrationSurfaceArea().SetValue(1.0, biogears::AreaUnit::m2);

  source.GetLeftAfferentArterioleResistance().SetValue(1.0, biogears::FlowResistanceUnit::mmHg_min_Per_mL);
  source.GetLeftEfferentArterioleResistance().SetValue(1.0, biogears::FlowResistanceUnit::mmHg_min_Per_mL);
  source.GetRenalVascularResistance().SetValue(1.0, biogears::FlowResistanceUnit::mmHg_min_Per_mL);
  source.GetRightAfferentArterioleResistance().SetValue(1.0, biogears::FlowResistanceUnit::mmHg_min_Per_mL);
  source.GetRightEfferentArterioleResistance().SetValue(1.0, biogears::FlowResistanceUnit::mmHg_min_Per_mL);

  source.GetFiltrationFraction().SetValue(0.4);
  source.GetLeftFiltrationFraction().SetValue(0.7);
  source.GetRightFiltrationFraction().SetValue(0.9);

  source.GetUrineUreaNitrogenConcentration().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_m3);

  source.GetUrineOsmolality().SetValue(1.0, biogears::OsmolalityUnit::mOsm_Per_kg);
  source.GetUrineOsmolarity().SetValue(1.0, biogears::OsmolarityUnit::mOsm_Per_L);

  source.GetLeftBowmansCapsulesHydrostaticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetLeftBowmansCapsulesOsmoticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetLeftGlomerularCapillariesHydrostaticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetLeftGlomerularCapillariesOsmoticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetLeftNetFiltrationPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetLeftNetReabsorptionPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetLeftPeritubularCapillariesHydrostaticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetLeftPeritubularCapillariesOsmoticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetLeftTubularHydrostaticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetLeftTubularOsmoticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRightBowmansCapsulesHydrostaticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRightBowmansCapsulesOsmoticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRightGlomerularCapillariesHydrostaticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRightGlomerularCapillariesOsmoticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRightNetFiltrationPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRightNetReabsorptionPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRightPeritubularCapillariesHydrostaticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRightPeritubularCapillariesOsmoticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRightTubularHydrostaticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRightTubularOsmoticPressure().SetValue(1.0, biogears::PressureUnit::mmHg);

  source.GetUrineVolume().SetValue(1.0, biogears::VolumeUnit::L);

  source.GetGlomerularFiltrationRate().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetLeftGlomerularFiltrationRate().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetLeftReabsorptionRate().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetMeanUrineOutput().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetRenalBloodFlow().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetRenalPlasmaFlow().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetRightGlomerularFiltrationRate().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetRightReabsorptionRate().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetUrinationRate().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetUrineProductionRate().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);

  source.GetLeftGlomerularFiltrationCoefficient().SetValue(1.0, biogears::VolumePerTimePressureUnit::mL_Per_s_mmHg);
  source.GetLeftReabsorptionFiltrationCoefficient().SetValue(1.0, biogears::VolumePerTimePressureUnit::mL_Per_s_mmHg);
  source.GetRightGlomerularFiltrationCoefficient().SetValue(1.0, biogears::VolumePerTimePressureUnit::mL_Per_s_mmHg);
  source.GetRightReabsorptionFiltrationCoefficient().SetValue(1.0, biogears::VolumePerTimePressureUnit::mL_Per_s_mmHg);

  source.GetLeftGlomerularFluidPermeability().SetValue(1.0, biogears::VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
  source.GetLeftTubularReabsorptionFluidPermeability().SetValue(1.0, biogears::VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
  source.GetRightGlomerularFluidPermeability().SetValue(1.0, biogears::VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
  source.GetRightTubularReabsorptionFluidPermeability().SetValue(1.0, biogears::VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SERenalSystem
//! TYPE RenalSystem
//! static void Marshall(const CDM::RespiratorySystemData& in, SERespiratorySystem& out);
//! static void UnMarshall(const SERespiratorySystem& in, CDM::RespiratorySystemData& out);
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
TEST_F(TEST_FIXTURE_NAME, RespiratorySystem)
{
  USING_TYPES(RespiratorySystem)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetInspiratoryExpiratoryRatio().SetValue(1.0);
  source.GetSpecificVentilation().SetValue(1.0);

  source.GetPulmonaryCompliance().SetValue(1.0, biogears::FlowComplianceUnit::L_Per_cmH2O);

  source.GetPulmonaryResistance().SetValue(1.0, biogears::FlowResistanceUnit::cmH2O_s_Per_L);

  source.GetEndTidalCarbonDioxideFraction().SetValue(1.0);

  source.GetRespirationDriverFrequency().SetValue(1.0, biogears::FrequencyUnit::Hz);
  source.GetRespirationRate().SetValue(1.0, biogears::FrequencyUnit::Hz);

  source.GetAlveolarArterialGradient().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetCarricoIndex().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetEndTidalCarbonDioxidePressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetMeanPleuralPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRespirationDriverPressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetRespirationMusclePressure().SetValue(1.0, biogears::PressureUnit::mmHg);
  source.GetTranspulmonaryPressure().SetValue(1.0, biogears::PressureUnit::mmHg);

  source.GetTidalVolume().SetValue(1.0, biogears::VolumeUnit::L);
  source.GetTotalLungVolume().SetValue(1.0, biogears::VolumeUnit::L);

  source.GetExpiratoryFlow().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_min);
  source.GetInspiratoryFlow().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_min);
  source.GetTargetPulmonaryVentilation().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_min);
  source.GetTotalAlveolarVentilation().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_min);
  source.GetTotalDeadSpaceVentilation().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_min);
  source.GetTotalPulmonaryVentilation().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_min);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SETissueSystem
//! TYPE TissueSystem
//! static void Marshall(const CDM::TissueSystemData& in, SETissueSystem& out);
//! static void UnMarshall(const SETissueSystem& in, CDM::TissueSystemData& out);

#include <biogears/cdm/system/physiology/SETissueSystem.h>
TEST_F(TEST_FIXTURE_NAME, TissueSystem)
{
  USING_TYPES(TissueSystem)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetIntracellularFluidPH().SetValue(1.0);
  source.GetRespiratoryExchangeRatio().SetValue(1.0);

  source.GetDehydrationFraction().SetValue(0.5);

  source.GetFatInsulinSetPoint().SetValue(1.0, biogears::AmountPerVolumeUnit::ct_Per_L);
  source.GetLiverInsulinSetPoint().SetValue(1.0, biogears::AmountPerVolumeUnit::ct_Per_L);
  source.GetMuscleInsulinSetPoint().SetValue(1.0, biogears::AmountPerVolumeUnit::ct_Per_L);

  source.GetLiverGlycogen().SetValue(1.0, biogears::MassUnit::g);
  source.GetMuscleGlycogen().SetValue(1.0, biogears::MassUnit::g);
  source.GetStoredFat().SetValue(1.0, biogears::MassUnit::g);
  source.GetStoredProtein().SetValue(1.0, biogears::MassUnit::g);

  source.GetFatGlucagonSetPoint().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetLiverGlucagonSetPoint().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_cm3);
  source.GetMuscleGlucagonSetPoint().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_cm3);

  source.GetExtracellularFluidVolume().SetValue(1.0, biogears::VolumeUnit::L);
  source.GetExtravascularFluidVolume().SetValue(1.0, biogears::VolumeUnit::L);
  source.GetIntracellularFluidVolume().SetValue(1.0, biogears::VolumeUnit::L);
  source.GetTotalBodyFluidVolume().SetValue(1.0, biogears::VolumeUnit::L);

  source.GetCarbonDioxideProductionRate().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);
  source.GetOxygenConsumptionRate().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_day);

  EXPECT_NE(source, sink);

  Physiology::Marshall(source, data);
  Physiology::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}