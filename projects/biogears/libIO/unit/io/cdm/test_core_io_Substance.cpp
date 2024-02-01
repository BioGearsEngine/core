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

#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/Logger.h>

#include <io/cdm/Property.h>
#include <io/cdm/Substance.h>

#ifdef DISABLE_BIOGEARS_Substance_TEST
#define TEST_FIXTURE_NAME DISABLED_SubstanceFixture
#else
#define TEST_FIXTURE_NAME IO_SubstanceFixture
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

// class SESubstanceManager;
//    TYPE SubstanceManager
//   static void Marshall(const CDM::SubstanceManagerData& in, SESubstanceManager& out);
//   static void UnMarshall(const SESubstanceManager& in, CDM::SubstanceManagerData& out);
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
TEST_F(TEST_FIXTURE_NAME, SubstanceManager)
{

  biogears::Logger logger;

  biogears::SESubstanceManager source { &logger };

  // NOTE: SESUbstanceManager can not Unload
  ASSERT_TRUE(source.LoadSubstanceDirectory());
}
// class SESubstanceAerosolization;
//   TYPE SubstanceAerosolization
//  static void Marshall(const CDM::SubstanceAerosolizationData& in, SESubstanceAerosolization& out);
//  static void UnMarshall(const SESubstanceAerosolization& in, CDM::SubstanceAerosolizationData& out);
#include <biogears/cdm/substance/SESubstanceAerosolization.h>

TEST_F(TEST_FIXTURE_NAME, SubstanceAerosolization)
{
  USING_TYPES(SubstanceAerosolization)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetBronchioleModifier().SetValue(1.0);
  source.GetInflammationCoefficient().SetValue(1.0);
  source.GetParticulateSizeDistribution().GetFraction().push_back(1.0);
  source.GetParticulateSizeDistribution().GetFraction().push_back(2.0);

  source.GetParticulateSizeDistribution().GetLength().push_back(3.0);
  source.GetParticulateSizeDistribution().GetLength().push_back(4.0);
  source.GetParticulateSizeDistribution().GetLength().push_back(5.0);
  source.GetParticulateSizeDistribution().SetLengthUnit(biogears::LengthUnit::m);
  EXPECT_NE(source, sink);

  Substance::Marshall(source, data);
  Substance::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SESubstancePharmacokinetics;
//   TYPE SubstancePharmacokinetics
//  static void Marshall(const CDM::SubstancePharmacokineticsData& in, SESubstancePharmacokinetics& out);
//  static void UnMarshall(const v& in, CDM::SubstancePharmacokineticsData& out);
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>

TEST_F(TEST_FIXTURE_NAME, SubstancePharmacokinetics)
{
  USING_TYPES(SubstancePharmacokinetics)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());

  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  auto& chemicals = source.GetPhysicochemicals();
  chemicals.SetBindingProtein(CDM::enumSubstanceBindingProtein::AAG);
  chemicals.GetPrimaryPKA().SetValue(9);
  chemicals.GetSecondaryPKA().SetValue(4);
  chemicals.GetBloodPlasmaRatio().SetValue(1.0);
  chemicals.GetFractionUnboundInPlasma().SetValue(1.0);
  chemicals.SetIonicState(CDM::enumSubstanceIonicState::Acid);
  chemicals.GetLogP().SetValue(1.0);
  chemicals.GetHydrogenBondCount().SetValue(1.0);
  chemicals.GetPolarSurfaceArea().SetValue(1.0);

  auto& kenetics = source.GetTissueKinetics("sarin") ;

  kenetics.GetPartitionCoefficient().SetValue(2);

  EXPECT_NE(source, sink);

  Substance::Marshall(source, data);
  Substance::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SESubstancePhysicochemical;
//   TYPE SubstancePhysicochemicals
//  static void Marshall(const CDM::SubstancePhysicochemicalsData& in, SESubstancePhysicochemical& out);
//  static void UnMarshall(const SESubstancePhysicochemical& in, CDM::SubstancePhysicochemicalsData& out);
#include <biogears/cdm/substance/SESubstancePhysicochemical.h>

TEST_F(TEST_FIXTURE_NAME, SubstancePhysicochemical)
{
  USING_TYPES(SubstancePhysicochemical)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetBindingProtein(CDM::enumSubstanceBindingProtein::AAG);
  source.GetPrimaryPKA().SetValue(9);
  source.GetSecondaryPKA().SetValue(4);
  source.GetBloodPlasmaRatio().SetValue(1.0);
  source.GetFractionUnboundInPlasma().SetValue(1.0);
  source.SetIonicState(CDM::enumSubstanceIonicState::Acid);
  source.GetLogP().SetValue(1.0);
  source.GetHydrogenBondCount().SetValue(1.0);
  source.GetPolarSurfaceArea().SetValue(1.0);

  EXPECT_NE(source, sink);

  Substance::Marshall(source, data);
  Substance::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SESubstanceTissuePharmacokinetics;
//   TYPE SubstanceTissuePharmacokinetics
//  static void Marshall(const CDM::SubstanceTissuePharmacokineticsData& in, SESubstanceTissuePharmacokinetics& out);
//  static void UnMarshall(const SESubstanceTissuePharmacokinetics& in, CDM::SubstanceTissuePharmacokineticsData& out);
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
TEST_F(TEST_FIXTURE_NAME, SubstanceTissuePharmacokinetics)
{
  USING_TYPES(SubstanceTissuePharmacokinetics)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { "SubstanceTissuePharmcoKineticsTest", &logger }, sink { "SINK", &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetPartitionCoefficient().SetValue(100.0);

  EXPECT_NE(source, sink);

  Substance::Marshall(source, data);
  Substance::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SESubstancePharmacodynamics;
//   TYPE SubstancePharmacodynamics
//  static void Marshall(const CDM::SubstancePharmacodynamicsData& in, SESubstancePharmacodynamics& out);
//  static void UnMarshall(const SESubstancePharmacodynamics& in, CDM::SubstancePharmacodynamicsData& out);
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>

TEST_F(TEST_FIXTURE_NAME, SubstancePharmacodynamics)
{
  USING_TYPES(SubstancePharmacodynamics)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto modifier = std::make_unique<biogears::SEPharmacodynamicModifier>(biogears::SEScalarMassPerVolume { 1.0, biogears::MassPerVolumeUnit::g_Per_dL }, 1.0);
  source.GetPharmacodynamicModifiers()["TEST"] = modifier.release();

  source.GetBronchodilation().Set({ 1.0 }, { 1.0, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetDiastolicPressureModifier().Set({ 2.0 }, { 3.0, biogears::MassPerVolumeUnit::g_Per_dL });

  source.GetFeverModifier().Set({ 5 }, { 10, biogears::MassPerVolumeUnit::g_Per_dL });

  source.GetHeartRateModifier().Set({ 5 }, { 110, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetHemorrhageModifier().Set({ 6 }, { 210, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetNeuromuscularBlock().Set({ 7 }, { 310, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetPainModifier().Set({ 8 }, { 410, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetPupilReactivityModifier().Set({ 5 }, { 510, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetPupilSizeModifier().Set({ 9 }, { 610, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetRespirationRateModifier().Set({ 10 }, { 710, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetSystolicPressureModifier().Set({ 11 }, { 810, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetTidalVolumeModifier().Set({ 12 }, { 910, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetCentralNervousModifier().Set({ 13 }, { 190, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetTubularPermeabilityModifier().Set({ 14 }, { 180, biogears::MassPerVolumeUnit::g_Per_dL });
  source.GetSedation().Set({ 15 }, { 10, biogears::MassPerVolumeUnit::g_Per_dL });

  source.GetEffectSiteRateConstant().SetValue(1.0, biogears::FrequencyUnit::Per_min);
  source.GetAntibacterialEffect().SetValue(1.0, biogears::FrequencyUnit::Per_min);
  source.GetEMaxShapeParameter().SetValue(1.0);

  EXPECT_NE(source, sink);

  Substance::Marshall(source, data);
  Substance::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SESubstanceClearance;
//   TYPE SubstanceClearance
//  static void Marshall(const CDM::SubstanceClearanceData& in, SESubstanceClearance& out);
//  static void UnMarshall(const SESubstanceClearance& in, CDM::SubstanceClearanceData& out);
#include <biogears/cdm/substance/SESubstanceClearance.h>

TEST_F(TEST_FIXTURE_NAME, SubstanceClearance)
{
  USING_TYPES(SubstanceClearance)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetSystemic(true);
  source.SetCellular(false);
  source.SetChargeInBlood(CDM::enumCharge::Negative);

  source.GetCellBirthRate().SetValue(1.0, biogears::FrequencyUnit::Hz);
  source.GetCellDeathRate().SetValue(1.0, biogears::FrequencyUnit::Hz);

  source.GetFractionExcretedInFeces().SetValue(1.0);
  source.GetFractionExcretedInUrine().SetValue(0.5);
  source.GetFractionMetabolizedInGut().SetValue(0.6);
  source.GetFractionUnboundInPlasma().SetValue(0.8);

  source.SetRenalDynamic(biogears::RenalDynamic::Regulation);

  source.GetIntrinsicClearance().SetValue(1.0, biogears::VolumePerTimeMassUnit::mL_Per_s_g);
  source.GetRenalClearance().SetValue(1.0, biogears::VolumePerTimeMassUnit::mL_Per_s_g);
  source.GetSystemicClearance().SetValue(1.0, biogears::VolumePerTimeMassUnit::mL_Per_s_g);

  source.GetRenalReabsorptionRatio().SetValue(0.11);
  source.GetGlomerularFilterability().SetValue(0.33);

  source.GetRenalTransportMaximum().SetValue(1.0, biogears::MassPerTimeUnit::g_Per_s);
  source.GetRenalFiltrationRate().SetValue(1.0, biogears::MassPerTimeUnit::g_Per_s);
  source.GetRenalReabsorptionRate().SetValue(1.0, biogears::MassPerTimeUnit::g_Per_s);
  source.GetRenalExcretionRate().SetValue(1.0, biogears::MassPerTimeUnit::g_Per_s);

  EXPECT_NE(source, sink);

  Substance::Marshall(source, data);
  Substance::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SESubstance;
//   TYPE Substance
//  static void Marshall(const CDM::SubstanceData& in, SESubstance& out);
//  static void UnMarshall(const SESubstance& in, CDM::SubstanceData& out);
#include <biogears/cdm/substance/SESubstance.h>
TEST_F(TEST_FIXTURE_NAME, Substance)
{
  USING_TYPES(Substance)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  source.SetName("SubstanceTest");
  source.SetClassification(CDM::enumSubstanceClass::Anesthetic);
  source.SetState(CDM::enumSubstanceState::Solid);

  source.GetMichaelisCoefficient().SetValue(1.0);
  source.GetRelativeDiffusionCoefficient().SetValue(1.0);

  source.GetMembraneResistance().SetValue(1.0, biogears::ElectricResistanceUnit::Ohm);

  source.GetEndTidalFraction().SetValue(1.0);

  source.GetSolubilityCoefficient().SetValue(1.0, biogears::InversePressureUnit::Inverse_cmH2O);

  source.GetMassInBlood().SetValue(1.0, biogears::MassUnit::g);
  source.GetMassInBody().SetValue(1.0, biogears::MassUnit::g);
  source.GetMassInTissue().SetValue(1.0, biogears::MassUnit::g);
  source.GetSystemicMassCleared().SetValue(1.0, biogears::MassUnit::g);

  source.GetMolarMass().SetValue(1.0, biogears::MassPerAmountUnit::g_Per_ct);

  source.GetMaximumDiffusionFlux().SetValue(1.0, biogears::MassPerAreaTimeUnit::g_Per_cm2_s);

  source.GetBloodConcentration().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_dL);
  source.GetDensity().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_dL);
  source.GetEffectSiteConcentration().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_dL);
  source.GetPlasmaConcentration().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_dL);
  source.GetTissueConcentration().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_dL);

  source.GetEndTidalPressure().SetValue(1.0, biogears::PressureUnit::mmHg);

  source.GetAreaUnderCurve().SetValue(1.0, biogears::TimeMassPerVolumeUnit::hr_g_Per_L);

  source.GetAlveolarTransfer().SetValue(1.0, biogears::VolumePerTimeUnit::L_Per_s);

  source.GetDiffusingCapacity().SetValue(1.0, biogears::VolumePerTimePressureUnit::L_Per_s_mmHg);

  source.GetAerosolization().GetBronchioleModifier().SetValue(0.1);
  source.GetAerosolization().GetInflammationCoefficient().SetValue(0.5);
  source.GetAerosolization().GetParticulateSizeDistribution();
  source.GetAerosolization().GetParticulateSizeDistribution().GetFraction().push_back(1.0);
  source.GetAerosolization().GetParticulateSizeDistribution().GetFraction().push_back(2.0);

  source.GetAerosolization().GetParticulateSizeDistribution().GetLength().push_back(3.0);
  source.GetAerosolization().GetParticulateSizeDistribution().GetLength().push_back(4.0);
  source.GetAerosolization().GetParticulateSizeDistribution().GetLength().push_back(5.0);
  source.GetAerosolization().GetParticulateSizeDistribution().SetLengthUnit(biogears::LengthUnit::m);
  source.GetClearance().SetSystemic(true);
  source.GetClearance().SetCellular(false);
  source.GetClearance().SetChargeInBlood(CDM::enumCharge::Negative);

  source.GetClearance().GetCellBirthRate().SetValue(1.0, biogears::FrequencyUnit::Hz);
  source.GetClearance().GetCellDeathRate().SetValue(1.0, biogears::FrequencyUnit::Hz);

  source.GetClearance().GetFractionExcretedInFeces().SetValue(1.0);
  source.GetClearance().GetFractionExcretedInUrine().SetValue(0.5);
  source.GetClearance().GetFractionMetabolizedInGut().SetValue(0.6);
  source.GetClearance().GetFractionUnboundInPlasma().SetValue(0.8);

  source.GetClearance().SetRenalDynamic(biogears::RenalDynamic::Regulation);

  source.GetClearance().GetIntrinsicClearance().SetValue(1.0, biogears::VolumePerTimeMassUnit::mL_Per_s_g);
  source.GetClearance().GetRenalClearance().SetValue(1.0, biogears::VolumePerTimeMassUnit::mL_Per_s_g);
  source.GetClearance().GetSystemicClearance().SetValue(1.0, biogears::VolumePerTimeMassUnit::mL_Per_s_g);

  source.GetClearance().GetRenalReabsorptionRatio().SetValue(0.11);
  source.GetClearance().GetGlomerularFilterability().SetValue(0.33);

  source.GetClearance().GetRenalTransportMaximum().SetValue(1.0, biogears::MassPerTimeUnit::g_Per_s);
  source.GetClearance().GetRenalFiltrationRate().SetValue(1.0, biogears::MassPerTimeUnit::g_Per_s);
  source.GetClearance().GetRenalReabsorptionRate().SetValue(1.0, biogears::MassPerTimeUnit::g_Per_s);
  source.GetClearance().GetRenalExcretionRate().SetValue(1.0, biogears::MassPerTimeUnit::g_Per_s);

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetPK();
  source.GetPD();

  EXPECT_NE(source, sink);

  Substance::Marshall(source, data);
  Substance::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SESubstanceCompound;
//   TYPE SubstanceCompound
//  static void Marshall(const CDM::SubstanceCompoundData& in, SESubstanceCompound& out);
//  static void UnMarshall(const SESubstanceCompound& in, CDM::SubstanceCompoundData& out);
#include <biogears/cdm/substance/SESubstanceCompound.h>

TEST_F(TEST_FIXTURE_NAME, SubstanceCompound)
{
  USING_TYPES(SubstanceCompound)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.SetName("SubstanceCompoundTest");
  source.SetClassification(CDM::enumSubstanceClass::Anesthetic);
  source.SetRhFactor(true);

  biogears::SESubstanceConcentration sub { *sarin };
  sub.GetConcentration().SetValue(30, biogears::MassPerVolumeUnit::g_Per_dL);

  source.GetComponents().push_back(sub);

  EXPECT_NE(source, sink);

  Substance::Marshall(source, data);
  Substance::UnMarshall(data, mgr, sink);

  EXPECT_EQ(source, sink);
}
// class SESubstanceFraction;
//   TYPE SubstanceFraction
//  static void Marshall(const CDM::SubstanceFractionData& in, SESubstanceFraction& out);
//  static void UnMarshall(const SESubstanceFraction& in, CDM::SubstanceFractionData& out);
#include <biogears/cdm/substance/SESubstanceFraction.h>

TEST_F(TEST_FIXTURE_NAME, SubstanceFraction)
{
  USING_TYPES(SubstanceFraction)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());

  auto sarin = mgr.GetSubstance("Sarin");

  SEType source { *sarin }, sink { *sarin };
  CDMType data;

source.GetFractionAmount().SetValue( 0.99);

  EXPECT_NE(source, sink);

  Substance::Marshall(source, data);
  Substance::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}
// class SESubstanceConcentration;
//   TYPE SubstanceConcentration
//  static void Marshall(const CDM::SubstanceConcentrationData& in, SESubstanceConcentration& out);
//  static void UnMarshall(const SESubstanceConcentration& in, CDM::SubstanceConcentrationData& out);
#include <biogears/cdm/substance/SESubstanceConcentration.h>

TEST_F(TEST_FIXTURE_NAME, SubstanceConcentration)
{
  USING_TYPES(SubstanceConcentration)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());

  auto sarin = mgr.GetSubstance("Sarin");

  SEType source { *sarin }, sink { *sarin };
  CDMType data;

  source.GetConcentration().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_mL);

  EXPECT_NE(source, sink);

  Substance::Marshall(source, data);
  Substance::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEPharmacodynamicModifier;
//    TYPE PharmacodynamicModifier
//   static void Marshall(const CDM::PharmacodynamicModifierData& in, SEPharmacodynamicModifier& out);
//   static void UnMarshall(const SEPharmacodynamicModifier& in, CDM::PharmacodynamicModifierData& out);
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>

TEST_F(TEST_FIXTURE_NAME, PharmacodynamicModifier)
{
  USING_TYPES(PharmacodynamicModifier)

  biogears::Logger logger;
  biogears::SESubstanceManager mgr { &logger };
  ASSERT_TRUE(mgr.LoadSubstanceDirectory());
  SEType source, sink;
  CDMType data;

  auto sarin = mgr.GetSubstance("Sarin");

  source.GetEC50().SetValue(1.0, biogears::MassPerVolumeUnit::g_Per_dL);
  source.GetEMax().SetValue(1.0);

  EXPECT_NE(source, sink);

  Substance::Marshall(source, data);
  Substance::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}