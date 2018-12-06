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
//! @author David Lee
//! @date   2017 Aug 3rd
//!
//! Unit Test for Biogears-common Config
//!
#include <limits>
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#ifdef DISABLE_BIOGEARS_SEBloodChemistrySystem_TEST
#define TEST_FIXTURE_NAME DISABLED_SEBloodChemistrySystem_Fixture
#else
#define TEST_FIXTURE_NAME SEBloodChemistrySystem_Fixture
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
  virtual void SetUp();

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown();

  biogears::Logger* logger;
  biogears::SEBloodChemistrySystem* bcs;
  biogears::CCompoundUnit* ccunit;
};

void TEST_FIXTURE_NAME::SetUp()
{
  logger = new biogears::Logger;
  bcs = new SEBloodChemistrySystem(logger);
}

void TEST_FIXTURE_NAME::TearDown()
{
  delete bcs;
  delete logger;
}
TEST_F(TEST_FIXTURE_NAME, ArterialBloodPH)
{
  EXPECT_EQ(false, bcs->HasArterialBloodPH());
  bcs->GetArterialBloodPH().SetValue(1.0);
  EXPECT_EQ(true, bcs->HasArterialBloodPH());
  EXPECT_EQ(1.0, bcs->GetArterialBloodPH().GetValue());

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasArterialBloodPH());
  auto dvalue = cbcs.GetArterialBloodPH();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, BloodDensity)
{
  EXPECT_EQ(false, bcs->HasBloodDensity());
  bcs->GetBloodDensity().SetValue(1.0,MassPerVolumeUnit::kg_Per_m3);
  EXPECT_EQ(true, bcs->HasBloodDensity());
  EXPECT_EQ(1.0,bcs->GetBloodDensity().GetValue(MassPerVolumeUnit::kg_Per_m3));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasBloodDensity());
  auto dvalue = cbcs.GetBloodDensity(MassPerVolumeUnit::kg_Per_m3);
  EXPECT_NE(dvalue,dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ArterialBloodPHBaseline)
{
  EXPECT_EQ(false, bcs->HasArterialBloodPHBaseline());
  bcs->GetArterialBloodPHBaseline().SetValue(1.0);
  EXPECT_EQ(true, bcs->HasArterialBloodPHBaseline());
  EXPECT_EQ(1.0, bcs->GetArterialBloodPHBaseline().GetValue());

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasArterialBloodPHBaseline());
  auto dvalue = cbcs.GetArterialBloodPHBaseline();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, VenousBloodPH)
{
  EXPECT_EQ(false, bcs->HasVenousBloodPH());
  bcs->GetVenousBloodPH().SetValue(1.0);
  EXPECT_EQ(true, bcs->HasVenousBloodPH());
  EXPECT_EQ(1.0, bcs->GetVenousBloodPH().GetValue());

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasVenousBloodPH());
  auto dvalue = cbcs.GetVenousBloodPH();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, BloodSpecificHeat)
{
  EXPECT_EQ(false, bcs->HasBloodSpecificHeat());
  bcs->GetBloodSpecificHeat().SetValue(1.0,HeatCapacitancePerMassUnit::J_Per_K_kg);
  EXPECT_EQ(true, bcs->HasBloodSpecificHeat());
  EXPECT_EQ(1.0, bcs->GetBloodSpecificHeat().GetValue(HeatCapacitancePerMassUnit::J_Per_K_kg));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasBloodSpecificHeat());
  auto dvalue = cbcs.GetBloodSpecificHeat(HeatCapacitancePerMassUnit::J_Per_K_kg);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, BloodUreaNitrogenConcentration)
{
  EXPECT_EQ(false, bcs->HasBloodUreaNitrogenConcentration());
  bcs->GetBloodUreaNitrogenConcentration().SetValue(1.0, MassPerVolumeUnit::kg_Per_m3);
  EXPECT_EQ(true, bcs->HasBloodUreaNitrogenConcentration());
  EXPECT_EQ(1.0, bcs->GetBloodUreaNitrogenConcentration().GetValue(MassPerVolumeUnit::kg_Per_m3));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasBloodUreaNitrogenConcentration());
  auto dvalue = cbcs.GetBloodUreaNitrogenConcentration(MassPerVolumeUnit::kg_Per_m3);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CarbonDioxideSaturation)
{
  EXPECT_EQ(false, bcs->HasCarbonDioxideSaturation());
  bcs->GetCarbonDioxideSaturation().SetValue(1.0);
  EXPECT_EQ(true, bcs->HasCarbonDioxideSaturation());
  EXPECT_EQ(1.0, bcs->GetCarbonDioxideSaturation().GetValue());

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasCarbonDioxideSaturation());
  auto dvalue = cbcs.GetCarbonDioxideSaturation();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, CarbonMonoxideSaturation)
{
  EXPECT_EQ(false, bcs->HasCarbonMonoxideSaturation());
  bcs->GetCarbonMonoxideSaturation().SetValue(1.0);
  EXPECT_EQ(true, bcs->HasCarbonMonoxideSaturation());
  EXPECT_EQ(1.0, bcs->GetCarbonMonoxideSaturation().GetValue());

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasCarbonMonoxideSaturation());
  auto dvalue = cbcs.GetCarbonMonoxideSaturation();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, Hematocrit)
{
  EXPECT_EQ(false, bcs->HasHematocrit());
  bcs->GetHematocrit().SetValue(1.0);
  EXPECT_EQ(true, bcs->HasHematocrit());
  EXPECT_EQ(1.0, bcs->GetHematocrit().GetValue());

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasHematocrit());
  auto dvalue = cbcs.GetHematocrit();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, HemoglobinContent)
{
  EXPECT_EQ(false, bcs->HasHemoglobinContent());
  bcs->GetHemoglobinContent().SetValue(1.0, MassUnit::g);
  EXPECT_EQ(true, bcs->HasHemoglobinContent());
  EXPECT_EQ(1.0, bcs->GetHemoglobinContent().GetValue(MassUnit::g));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasHemoglobinContent());
  auto dvalue = cbcs.GetHemoglobinContent(MassUnit::g);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, OxygenSaturation)
{
  EXPECT_EQ(false, bcs->HasOxygenSaturation());
  bcs->GetOxygenSaturation().SetValue(1.0);
  EXPECT_EQ(true, bcs->HasOxygenSaturation());
  EXPECT_EQ(1.0, bcs->GetOxygenSaturation().GetValue());

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasOxygenSaturation());
  auto dvalue = cbcs.GetOxygenSaturation();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, Phosphate)
{
  EXPECT_EQ(false, bcs->HasPhosphate());
  bcs->GetPhosphate().SetValue(1.0, AmountPerVolumeUnit::ct_Per_L);
  EXPECT_EQ(true, bcs->HasPhosphate());
  EXPECT_EQ(1.0, bcs->GetPhosphate().GetValue(AmountPerVolumeUnit::ct_Per_L));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasPhosphate());
  auto dvalue = cbcs.GetPhosphate(AmountPerVolumeUnit::ct_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PlasmaVolume)
{
  EXPECT_EQ(false, bcs->HasPlasmaVolume());
  bcs->GetPlasmaVolume().SetValue(1.0, VolumeUnit::L);
  EXPECT_EQ(true, bcs->HasPlasmaVolume());
  EXPECT_EQ(1.0, bcs->GetPlasmaVolume().GetValue(VolumeUnit::L));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasPlasmaVolume());
  auto dvalue = cbcs.GetPlasmaVolume(VolumeUnit::L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulseOximetry)
{
  EXPECT_EQ(false, bcs->HasPulseOximetry());
  bcs->GetPulseOximetry().SetValue(1.0);
  EXPECT_EQ(true, bcs->HasPulseOximetry());
  EXPECT_EQ(1.0, bcs->GetPulseOximetry().GetValue());

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasPulseOximetry());
  auto dvalue = cbcs.GetPulseOximetry();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RedBloodAcetylcholinesterase)
{
  EXPECT_EQ(false, bcs->HasRedBloodCellAcetylcholinesterase());
  bcs->GetRedBloodCellAcetylcholinesterase().SetValue(1.0,AmountPerVolumeUnit::ct_Per_L);
  EXPECT_EQ(true, bcs->HasRedBloodCellAcetylcholinesterase());
  EXPECT_EQ(1.0, bcs->GetRedBloodCellAcetylcholinesterase().GetValue(AmountPerVolumeUnit::ct_Per_L));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasRedBloodCellAcetylcholinesterase());
  auto dvalue = cbcs.GetRedBloodCellAcetylcholinesterase(AmountPerVolumeUnit::ct_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, RedBloodCellCount)
{
  EXPECT_EQ(false, bcs->HasRedBloodCellCount());
  bcs->GetRedBloodCellCount().SetValue(1.0,AmountPerVolumeUnit::ct_Per_L);
  EXPECT_EQ(true, bcs->HasRedBloodCellCount());
  EXPECT_EQ(1.0, bcs->GetRedBloodCellCount().GetValue(AmountPerVolumeUnit::ct_Per_L));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasRedBloodCellCount());
  auto dvalue = cbcs.GetRedBloodCellCount(AmountPerVolumeUnit::ct_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ShuntFraction)
{
  EXPECT_EQ(false, bcs->HasShuntFraction());
  bcs->GetShuntFraction().SetValue(1.0);
  EXPECT_EQ(true, bcs->HasShuntFraction());
  EXPECT_EQ(1.0, bcs->GetShuntFraction().GetValue());

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasShuntFraction());
  auto dvalue = cbcs.GetShuntFraction();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, StrongIonDifference)
{
  EXPECT_EQ(false, bcs->HasStrongIonDifference());
  bcs->GetStrongIonDifference().SetValue(1.0,AmountPerVolumeUnit::ct_Per_L);
  EXPECT_EQ(true, bcs->HasStrongIonDifference());
  EXPECT_EQ(1.0, bcs->GetStrongIonDifference().GetValue(AmountPerVolumeUnit::ct_Per_L));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasStrongIonDifference());
  auto dvalue = cbcs.GetStrongIonDifference(AmountPerVolumeUnit::ct_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TotalBilirubin)
{
  EXPECT_EQ(false, bcs->HasTotalBilirubin());
  bcs->GetTotalBilirubin().SetValue(1.0,MassPerVolumeUnit::g_Per_L);
  EXPECT_EQ(true, bcs->HasTotalBilirubin());
  EXPECT_EQ(1.0, bcs->GetTotalBilirubin().GetValue(MassPerVolumeUnit::g_Per_L));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasTotalBilirubin());
  auto dvalue = cbcs.GetTotalBilirubin(MassPerVolumeUnit::g_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, TotalProteinConcentration)
{
  EXPECT_EQ(false, bcs->HasTotalProteinConcentration());
  bcs->GetTotalProteinConcentration().SetValue(1.0,MassPerVolumeUnit::g_Per_L);
  EXPECT_EQ(true, bcs->HasTotalProteinConcentration());
  EXPECT_EQ(1.0, bcs->GetTotalProteinConcentration().GetValue(MassPerVolumeUnit::g_Per_L));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasTotalProteinConcentration());
  auto dvalue = cbcs.GetTotalProteinConcentration(MassPerVolumeUnit::g_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, FractionNeutralLipidInPlasma)
{
  EXPECT_EQ(false, bcs->HasVolumeFractionNeutralLipidInPlasma());
  bcs->GetVolumeFractionNeutralLipidInPlasma().SetValue(1.0);
  EXPECT_EQ(true, bcs->HasVolumeFractionNeutralLipidInPlasma());
  EXPECT_EQ(1.0, bcs->GetVolumeFractionNeutralLipidInPlasma().GetValue());

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasVolumeFractionNeutralLipidInPlasma());
  auto dvalue = cbcs.GetVolumeFractionNeutralLipidInPlasma();
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, WhiteBloodCellCount)
{
  EXPECT_EQ(false, bcs->HasWhiteBloodCellCount());
  bcs->GetWhiteBloodCellCount().SetValue(1.0,AmountPerVolumeUnit::ct_Per_L);
  EXPECT_EQ(true, bcs->HasWhiteBloodCellCount());
  EXPECT_EQ(1.0, bcs->GetWhiteBloodCellCount().GetValue(AmountPerVolumeUnit::ct_Per_L));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasWhiteBloodCellCount());
  auto dvalue = cbcs.GetWhiteBloodCellCount(AmountPerVolumeUnit::ct_Per_L);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ArterialCarbonDioxidePressure)
{
  EXPECT_EQ(false, bcs->HasArterialCarbonDioxidePressure());
  bcs->GetArterialCarbonDioxidePressure().SetValue(1.0,PressureUnit::Pa);
  EXPECT_EQ(true, bcs->HasArterialCarbonDioxidePressure());
  EXPECT_EQ(1.0, bcs->GetArterialCarbonDioxidePressure().GetValue(PressureUnit::Pa));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasArterialCarbonDioxidePressure());
  auto dvalue = cbcs.GetArterialCarbonDioxidePressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, ArterialOxygenPressure)
{
  EXPECT_EQ(false, bcs->HasArterialOxygenPressure());
  bcs->GetArterialOxygenPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, bcs->HasArterialOxygenPressure());
  EXPECT_EQ(1.0, bcs->GetArterialOxygenPressure().GetValue(PressureUnit::Pa));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasArterialOxygenPressure());
  auto dvalue = cbcs.GetArterialOxygenPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryArterialOxygenPressure)
{
  EXPECT_EQ(false, bcs->HasPulmonaryArterialOxygenPressure());
  bcs->GetPulmonaryArterialOxygenPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, bcs->HasPulmonaryArterialOxygenPressure());
  EXPECT_EQ(1.0, bcs->GetPulmonaryArterialOxygenPressure().GetValue(PressureUnit::Pa));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasPulmonaryArterialOxygenPressure());
  auto dvalue = cbcs.GetPulmonaryArterialOxygenPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryVenousCarbonDioxidePressure)
{
  EXPECT_EQ(false, bcs->HasPulmonaryVenousCarbonDioxidePressure());
  bcs->GetPulmonaryVenousCarbonDioxidePressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, bcs->HasPulmonaryVenousCarbonDioxidePressure());
  EXPECT_EQ(1.0, bcs->GetPulmonaryVenousCarbonDioxidePressure().GetValue(PressureUnit::Pa));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasPulmonaryVenousCarbonDioxidePressure());
  auto dvalue = cbcs.GetPulmonaryVenousCarbonDioxidePressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, PulmonaryVenousOxygenPressure)
{
  EXPECT_EQ(false, bcs->HasPulmonaryVenousOxygenPressure());
  bcs->GetPulmonaryVenousOxygenPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, bcs->HasPulmonaryVenousOxygenPressure());
  EXPECT_EQ(1.0, bcs->GetPulmonaryVenousOxygenPressure().GetValue(PressureUnit::Pa));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasPulmonaryVenousOxygenPressure());
  auto dvalue = cbcs.GetPulmonaryVenousOxygenPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, VenousOxygenPressure)
{
  EXPECT_EQ(false, bcs->HasVenousOxygenPressure());
  bcs->GetVenousOxygenPressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, bcs->HasVenousOxygenPressure());
  EXPECT_EQ(1.0, bcs->GetVenousOxygenPressure().GetValue(PressureUnit::Pa));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasVenousOxygenPressure());
  auto dvalue = cbcs.GetVenousOxygenPressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, VenousCarbonDioxidePressure)
{
  EXPECT_EQ(false, bcs->HasVenousCarbonDioxidePressure());
  bcs->GetVenousCarbonDioxidePressure().SetValue(1.0, PressureUnit::Pa);
  EXPECT_EQ(true, bcs->HasVenousCarbonDioxidePressure());
  EXPECT_EQ(1.0, bcs->GetVenousCarbonDioxidePressure().GetValue(PressureUnit::Pa));

  const biogears::SEBloodChemistrySystem cbcs(logger);
  EXPECT_EQ(false, cbcs.HasVenousCarbonDioxidePressure());
  auto dvalue = cbcs.GetVenousCarbonDioxidePressure(PressureUnit::Pa);
  EXPECT_NE(dvalue, dvalue);
}
TEST_F(TEST_FIXTURE_NAME, AcuteInflammatoryResponse)
{
  //todo:lmarin: Spruce this up after Matt's changes are fully in
  EXPECT_EQ(false, bcs->HasAcuteInflammatoryResponse());
//  bcs->GetAcuteInflammatoryResponse().InitializeState();
//  EXPECT_EQ(true, bcs->HasAcuteInflammatoryResponse());
}
TEST_F(TEST_FIXTURE_NAME, placeholder)
{
  //bool h30 = bcs.HasAcuteInflammatoryResponse();
}
