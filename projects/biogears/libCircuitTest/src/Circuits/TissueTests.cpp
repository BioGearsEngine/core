/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include <biogears/engine/Systems/Diffusion.h>
#include <biogears/engine/Systems/Tissue.h>
#include <biogears/engine/test/BioGearsEngineTest.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/tissue/SETissueCompartment.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/TimingProfile.h>
#include <biogears/cdm/utils/testing/SETestCase.h>
#include <biogears/cdm/utils/testing/SETestReport.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>

#include <biogears/cdm/utils/DataTrack.h>

namespace biogears {
void BioGearsEngineTest::DistributeMass(SETestSuite& testSuite)
{
  TimingProfile timer;
  BioGears bg(testSuite.GetLogger());
  Tissue& tsu = (Tissue&)bg.GetTissue();
  DiffusionCalculator& diffCalc = (DiffusionCalculator&)bg.GetDiffusionCalculator();
  // First test case
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("DistributeMassToHierarchy");
  timer.Start("Test");

  //                L0C0
  //               /    \
  //           L1C0      L1C1
  //           /  \      /  \
  //        L2C0  L2C1 L2C2 L2C3 <-- Only these cmpts have data

  SELiquidCompartment& L0C0 = bg.GetCompartments().CreateLiquidCompartment("L0C0");
  SELiquidCompartment& L1C0 = bg.GetCompartments().CreateLiquidCompartment("L1C0");
  SELiquidCompartment& L1C1 = bg.GetCompartments().CreateLiquidCompartment("L1C1");
  SELiquidCompartment& L2C0 = bg.GetCompartments().CreateLiquidCompartment("L2C0");
  SELiquidCompartment& L2C1 = bg.GetCompartments().CreateLiquidCompartment("L2C1");
  SELiquidCompartment& L2C2 = bg.GetCompartments().CreateLiquidCompartment("L2C2");
  SELiquidCompartment& L2C3 = bg.GetCompartments().CreateLiquidCompartment("L2C3");

  // Build up the hierarchy
  L0C0.AddChild(L1C0);
  L0C0.AddChild(L1C1);
  L1C0.AddChild(L2C0);
  L1C0.AddChild(L2C1);
  L1C1.AddChild(L2C2);
  L1C1.AddChild(L2C3);
  bg.GetCompartments().StateChange(); // Call this, AFTER YOU SET UP YOUR HIERARCHY, to ensure all parent compartments have their link data

  SESubstance* sub = bg.GetSubstances().GetSubstance("Oxygen");
  bg.GetSubstances().AddActiveSubstance(*sub);

  double L2C0_mL = 10;
  double L2C0_g = 10;
  double L2C1_mL = 20;
  double L2C1_g = 20;
  double L2C2_mL = 40;
  double L2C2_g = 40;
  double L2C3_mL = 80;
  double L2C3_g = 80;
  double total_mL = L2C0_mL + L2C1_mL + L2C2_mL + L2C3_mL;

  L2C0.GetVolume().SetValue(L2C0_mL, VolumeUnit::mL);
  L2C0.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C0_g, MassUnit::g);
  L2C1.GetVolume().SetValue(L2C1_mL, VolumeUnit::mL);
  L2C1.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C1_g, MassUnit::g);
  L2C2.GetVolume().SetValue(L2C2_mL, VolumeUnit::mL);
  L2C2.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C2_g, MassUnit::g);
  L2C3.GetVolume().SetValue(L2C3_mL, VolumeUnit::mL);
  L2C3.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C3_g, MassUnit::g);

  double delta_g = -20;

  double expected_L2C0_g = L2C0_g + (delta_g * L2C0_mL / total_mL);
  double expected_L2C1_g = L2C1_g + (delta_g * L2C1_mL / total_mL);
  double expected_L2C2_g = L2C2_g + (delta_g * L2C2_mL / total_mL);
  double expected_L2C3_g = L2C3_g + (delta_g * L2C3_mL / total_mL);

  diffCalc.DistributeMassbyVolumeWeighted(L0C0, *sub, delta_g, MassUnit::g);

  double L2C0_calc = L2C0.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C0_calc - expected_L2C0_g) > ZERO_APPROX)
    testCase.AddFailure("DistributeMassbyVolumeWeighted L2C0 mass is not expected mass");

  double L2C1_calc = L2C1.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C1_calc - expected_L2C1_g) > ZERO_APPROX)
    testCase.AddFailure("DistributeMassbyVolumeWeighted L2C1 mass is not expected mass");

  double L2C2_calc = L2C2.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C2_calc - expected_L2C2_g) > ZERO_APPROX)
    testCase.AddFailure("DistributeMassbyVolumeWeighted L2C2 mass is not expected mass");

  double L2C3_calc = L2C3.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C3_calc - expected_L2C3_g) > ZERO_APPROX)
    testCase.AddFailure("DistributeMassbyVolumeWeighted L2C3 mass is not expected mass");

  // Log how long our test took
  testCase.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);

  // Second test case
  SETestCase& testCase2 = testSuite.CreateTestCase();
  testCase2.SetName("DistributeMassbyMassToHierarchy");
  timer.Start("Test");

  L2C0_mL = 10;
  L2C0_g = 10;
  L2C1_mL = 20;
  L2C1_g = 20;
  L2C2_mL = 40;
  L2C2_g = 40;
  L2C3_mL = 80;
  L2C3_g = 80;
  double total_g = L2C0_g + L2C1_g + L2C2_g + L2C3_g;

  L2C0.GetVolume().SetValue(L2C0_mL, VolumeUnit::mL);
  L2C0.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C0_g, MassUnit::g);
  L2C1.GetVolume().SetValue(L2C1_mL, VolumeUnit::mL);
  L2C1.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C1_g, MassUnit::g);
  L2C2.GetVolume().SetValue(L2C2_mL, VolumeUnit::mL);
  L2C2.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C2_g, MassUnit::g);
  L2C3.GetVolume().SetValue(L2C3_mL, VolumeUnit::mL);
  L2C3.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C3_g, MassUnit::g);

  delta_g = -20;

  expected_L2C0_g = L2C0_g + (delta_g * L2C0_g / total_g);
  expected_L2C1_g = L2C1_g + (delta_g * L2C1_g / total_g);
  expected_L2C2_g = L2C2_g + (delta_g * L2C2_g / total_g);
  expected_L2C3_g = L2C3_g + (delta_g * L2C3_g / total_g);

  diffCalc.DistributeMassbyMassWeighted(L0C0, *sub, delta_g, MassUnit::g);

  L2C0_calc = L2C0.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C0_calc - expected_L2C0_g) > ZERO_APPROX)
    testCase2.AddFailure("DistributeMassbyMassWeighted L2C0 mass is not expected mass");

  L2C1_calc = L2C1.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C1_calc - expected_L2C1_g) > ZERO_APPROX)
    testCase2.AddFailure("DistributeMassbyMassWeighted L2C1 mass is not expected mass");

  L2C2_calc = L2C2.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C2_calc - expected_L2C2_g) > ZERO_APPROX)
    testCase2.AddFailure("DistributeMassbyMassWeighted L2C2 mass is not expected mass");

  L2C3_calc = L2C3.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C3_calc - expected_L2C3_g) > ZERO_APPROX)
    testCase2.AddFailure("DistributeMassbyMassWeighted L2C3 mass is not expected mass");

  // Third test case
  // Second test case
  SETestCase& testCase3 = testSuite.CreateTestCase();
  testCase3.SetName("DistributeMassbyMassToHierarchy_tooMuch");
  timer.Start("Test");

  L2C0_mL = 10;
  L2C0_g = 10;
  L2C1_mL = 20;
  L2C1_g = 20;
  L2C2_mL = 40;
  L2C2_g = 40;
  L2C3_mL = 80;
  L2C3_g = 80;
  total_g = L2C0_g + L2C1_g + L2C2_g + L2C3_g;

  L2C0.GetVolume().SetValue(L2C0_mL, VolumeUnit::mL);
  L2C0.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C0_g, MassUnit::g);
  L2C1.GetVolume().SetValue(L2C1_mL, VolumeUnit::mL);
  L2C1.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C1_g, MassUnit::g);
  L2C2.GetVolume().SetValue(L2C2_mL, VolumeUnit::mL);
  L2C2.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C2_g, MassUnit::g);
  L2C3.GetVolume().SetValue(L2C3_mL, VolumeUnit::mL);
  L2C3.GetSubstanceQuantity(*sub)->GetMass().SetValue(L2C3_g, MassUnit::g);

  delta_g = -2000;

  expected_L2C0_g = 0.;
  expected_L2C1_g = 0.;
  expected_L2C2_g = 0.;
  expected_L2C3_g = 0.;

  diffCalc.DistributeMassbyMassWeighted(L0C0, *sub, delta_g, MassUnit::g);

  L2C0_calc = L2C0.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C0_calc - expected_L2C0_g) > ZERO_APPROX)
    testCase3.AddFailure("L2C0 mass is not expected mass - Expecting zero");

  L2C1_calc = L2C1.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C1_calc - expected_L2C1_g) > ZERO_APPROX)
    testCase3.AddFailure("L2C1 mass is not expected mass - Expecting zero");

  L2C2_calc = L2C2.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C2_calc - expected_L2C2_g) > ZERO_APPROX)
    testCase3.AddFailure("L2C2 mass is not expected mass - Expecting zero");

  L2C3_calc = L2C3.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
  if (std::abs(L2C3_calc - expected_L2C3_g) > ZERO_APPROX)
    testCase3.AddFailure("L2C3 mass is not expected mass - Expecting zero");
}

void BioGearsEngineTest::PerfusionLimitedDiffusionTest(SETestSuite& testSuite)
{
  BioGears bg(testSuite.GetLogger());
  Tissue& tsu = (Tissue&)bg.GetTissue();
  DiffusionCalculator& diffCalc = (DiffusionCalculator&)bg.GetDiffusionCalculator();
  TimingProfile timer;
  double timestep_s = 1. / 90.;
  SESubstance* sub = bg.GetSubstances().GetSubstance("Ketamine");
  double bFlow_mL_Per_s = 2.0;
  double PartitionCoeff = 1.52201;
  double matrixVolume_mL = 2.0;
  double tissueMass_ug = 3.;
  double bVol_mL = 10.;
  double bConc_ug_Per_mL = 10.;

  bg.GetSubstances().AddActiveSubstance(*sub);

  // First test case
  SETestCase& testCase1 = testSuite.CreateTestCase();
  testCase1.SetName("PerfusionLimitedDiffusionTest");
  timer.Start("Test");

  SETissueCompartment& tissue = bg.GetCompartments().CreateTissueCompartment(BGE::TissueCompartment::Bone);
  SELiquidCompartment& extracellular = bg.GetCompartments().CreateLiquidCompartment(BGE::ExtravascularCompartment::BoneExtracellular);
  SELiquidCompartment& intracellular = bg.GetCompartments().CreateLiquidCompartment(BGE::ExtravascularCompartment::BoneIntracellular);
  tissue.GetMatrixVolume().SetValue(matrixVolume_mL, VolumeUnit::mL);
  intracellular.GetSubstanceQuantity(*sub)->GetMass().SetValue(tissueMass_ug, MassUnit::ug);
  intracellular.GetVolume().SetValue(1.0, VolumeUnit::mL); //Need fluid volume to balance.
  extracellular.GetVolume().SetValue(1.0, VolumeUnit::mL); //Should be fine, right? We don't validate and tissue concentrations.

  SELiquidCompartment& vascular = bg.GetCompartments().CreateLiquidCompartment(BGE::VascularCompartment::Bone);
  vascular.GetVolume().SetValue(bVol_mL, VolumeUnit::mL);
  vascular.GetSubstanceQuantity(*sub)->GetConcentration().SetValue(bConc_ug_Per_mL, MassPerVolumeUnit::ug_Per_mL);
  vascular.GetSubstanceQuantity(*sub)->Balance(BalanceLiquidBy::Concentration);
  double vascularStartMass = vascular.GetSubstanceQuantity(*sub)->GetMass(MassUnit::ug);
  intracellular.GetSubstanceQuantity(*sub)->Balance(BalanceLiquidBy::Mass);

  SELiquidCompartmentLink& flow = bg.GetCompartments().CreateLiquidLink(intracellular, vascular, "ExtravascularExchange");
  flow.GetFlow().SetValue(bFlow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  extracellular.AddLink(flow);
  vascular.AddLink(flow);

  bg.GetCompartments().StateChange();

  DiffusionCalculator::DiffusionCompartmentSet diffSet = { &tissue, &vascular, &extracellular, &intracellular };

  diffCalc.CalculatePerfusionLimitedDiffusion(diffSet, *sub, PartitionCoeff);

  // Check
  double MassIncrement_ug = bFlow_mL_Per_s * timestep_s * ((bConc_ug_Per_mL) - ((tissueMass_ug / matrixVolume_mL) / PartitionCoeff));
  double vascularEndMass = diffSet.vascular->GetSubstanceQuantity(*sub)->GetMass(MassUnit::ug);
  double rtnMassInc_ug = vascularStartMass - vascularEndMass;
  if (std::abs(rtnMassInc_ug - MassIncrement_ug) > 1.e-10) {
    testCase1.AddFailure("PerfusionLimitedDiffusion returned unexpected mass increment.");
  }

  // Log how long our test took
  testCase1.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);

  // Subsequent test cases
  //timer.Start("Test");
}

void BioGearsEngineTest::AlveolarOxygenDiffusionTest(const std::string& rptDirectory)
{
  //This test examines diffusion of O2 from a gas compartment (like lungs) to a liquid compartment
  //The gas compartment is set to resemble atmospheric conditions, and the liquid compartment is oxygen-poor
  //We should expect to see oxygen diffuse into the liquid compartment
  BioGears bg(m_Logger);
  Tissue& tsu = (Tissue&)bg.GetTissue();

  double timestep_s = 1.0 / 90;
  SESubstance* O2 = bg.GetSubstances().GetSubstance("Oxygen");
  SESubstance* N2 = bg.GetSubstances().GetSubstance("Nitrogen");
  bg.GetSubstances().AddActiveSubstance(*O2);
  bg.GetSubstances().AddActiveSubstance(*N2);

  SELiquidCompartment& lcmpt = bg.GetCompartments().CreateLiquidCompartment("lcmpt");
  SEGasCompartment& gcmpt = bg.GetCompartments().CreateGasCompartment("gcmpt");

  DataTrack trk1;
  std::string rptFile = rptDirectory + "/AlveolarOxygenDiffusionTest.csv";
  double time = 0.0;

  double liquidVol_mL = 1000.0;
  double lMass_g = .001; //oxygen-poor

  gcmpt.GetVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  lcmpt.GetVolume().SetValue(liquidVol_mL, VolumeUnit::mL);

  gcmpt.GetSubstanceQuantity(*O2)->GetVolumeFraction().SetValue(.2);
  gcmpt.GetSubstanceQuantity(*N2)->GetVolumeFraction().SetValue(.8);
  gcmpt.GetPressure().SetValue(760, PressureUnit::mmHg);
  lcmpt.GetSubstanceQuantity(*O2)->GetMass().SetValue(lMass_g, MassUnit::g);
  lcmpt.GetSubstanceQuantity(*N2)->GetMass().SetValue(lMass_g, MassUnit::g);
  gcmpt.Balance(BalanceGasBy::VolumeFraction);
  lcmpt.Balance(BalanceLiquidBy::Mass);

  double diffusingCapacityO2_mL_Per_s_mmHg = .33;

  double l_O2_ug = lcmpt.GetSubstanceQuantity(*O2)->GetMass().GetValue(MassUnit::ug);
  double lconc = lcmpt.GetSubstanceQuantity(*O2)->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_L);
  double lppO2_mmHg = lcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg);
  double gppO2_mmHg = gcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg);

  trk1.Track("LiquidCompartmentOxygenMass_ug", time, l_O2_ug);
  trk1.Track("LiquidCompartmentOxygenConc_ug_Per_L", time, lconc);
  trk1.Track("LiquidCompartmentPO2_mmHg", time, lcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg));
  trk1.Track("GasCompartmentPO2_mmHg", time, gcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg));

  for (int i = 0; i < 180; i++) {
    tsu.AlveolarPartialPressureGradientDiffusion(gcmpt, lcmpt, *O2, diffusingCapacityO2_mL_Per_s_mmHg, timestep_s);
    gcmpt.Balance(BalanceGasBy::VolumeFraction);
    time += timestep_s;
    l_O2_ug = lcmpt.GetSubstanceQuantity(*O2)->GetMass().GetValue(MassUnit::ug);
    lconc = lcmpt.GetSubstanceQuantity(*O2)->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_L);
    lppO2_mmHg = lcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg);
    gppO2_mmHg = gcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg);
    trk1.Track("LiquidCompartmentOxygenMass_ug", time, l_O2_ug);
    trk1.Track("LiquidCompartmentOxygenConc_ug_Per_L", time, lconc);
    trk1.Track("LiquidCompartmentPO2_mmHg", time, lcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg));
    trk1.Track("GasCompartmentPO2_mmHg", time, gcmpt.GetSubstanceQuantity(*O2)->GetPartialPressure(PressureUnit::mmHg));
  }

  // Write data to file
  trk1.WriteTrackToFile(rptFile.c_str());
}

void BioGearsEngineTest::AlveolarCarbonDioxideDiffusionTest(const std::string& rptDirectory)
{
  //Second test case: High liquid compartment CO2 concentration with a lower gas compartment CO2 concentration
  //We should expect CO2 to diffuse out of the liquid compartment into the gas compartment
  BioGears bg(m_Logger);
  Tissue& tsu = (Tissue&)bg.GetTissue();

  DataTrack trk2;
  std::string rptFile = rptDirectory + "/AlveolarCarbonDioxideDiffusionTest.csv";

  double timestep_s = 1.0 / 90;

  SESubstance* CO2 = bg.GetSubstances().GetSubstance("CarbonDioxide");
  SESubstance* N2 = bg.GetSubstances().GetSubstance("Nitrogen");
  bg.GetSubstances().AddActiveSubstance(*CO2);
  bg.GetSubstances().AddActiveSubstance(*N2);

  SELiquidCompartment& lcmpt2 = bg.GetCompartments().CreateLiquidCompartment("lcmpt2");
  SEGasCompartment& gcmpt2 = bg.GetCompartments().CreateGasCompartment("gcmpt2");

  double time = 0.0;

  double liquidVol_mL = 1000.0;
  double lMass_g = .04; //high amount of CO2

  gcmpt2.GetVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  lcmpt2.GetVolume().SetValue(liquidVol_mL, VolumeUnit::mL);

  gcmpt2.GetSubstanceQuantity(*CO2)->GetVolumeFraction().SetValue(.0004); //atmospheric CO2 is .04%
  gcmpt2.GetSubstanceQuantity(*N2)->GetVolumeFraction().SetValue(1 - .0004);
  gcmpt2.GetPressure().SetValue(760, PressureUnit::mmHg);
  lcmpt2.GetSubstanceQuantity(*CO2)->GetMass().SetValue(lMass_g, MassUnit::g);
  lcmpt2.GetSubstanceQuantity(*N2)->GetMass().SetValue(lMass_g, MassUnit::g);
  gcmpt2.Balance(BalanceGasBy::VolumeFraction);
  lcmpt2.Balance(BalanceLiquidBy::Mass);

  double l_CO2_ug = lcmpt2.GetSubstanceQuantity(*CO2)->GetMass().GetValue(MassUnit::ug);
  double lconc = lcmpt2.GetSubstanceQuantity(*CO2)->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_L);
  double lppO2_mmHg = lcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg);
  double gppO2_mmHg = gcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg);
  double diffusingCapacityO2_mL_Per_s_mmHg = .33;

  trk2.Track("LiquidCompartmentCO2Mass_ug", time, l_CO2_ug);
  trk2.Track("LiquidCompartmentCO2Conc_ug_Per_L", time, lconc);
  trk2.Track("LiquidCompartmentPCO2_mmHg", time, lcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg));
  trk2.Track("GasCompartmentPCO2_mmHg", time, gcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg));

  for (int i = 0; i < 180; i++) {
    tsu.AlveolarPartialPressureGradientDiffusion(gcmpt2, lcmpt2, *CO2, diffusingCapacityO2_mL_Per_s_mmHg, timestep_s);
    gcmpt2.Balance(BalanceGasBy::VolumeFraction);
    time += timestep_s;
    l_CO2_ug = lcmpt2.GetSubstanceQuantity(*CO2)->GetMass().GetValue(MassUnit::ug);
    lconc = lcmpt2.GetSubstanceQuantity(*CO2)->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_L);
    lppO2_mmHg = lcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg);
    gppO2_mmHg = gcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg);
    trk2.Track("LiquidCompartmentCO2Mass_ug", time, l_CO2_ug);
    trk2.Track("LiquidCompartmentCO2Conc_ug_Per_L", time, lconc);
    trk2.Track("LiquidCompartmentPCO2_mmHg", time, lcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg));
    trk2.Track("GasCompartmentPCO2_mmHg", time, gcmpt2.GetSubstanceQuantity(*CO2)->GetPartialPressure(PressureUnit::mmHg));
  }

  // Write data to file
  trk2.WriteTrackToFile(rptFile.c_str());
}

void BioGearsEngineTest::EigenDiffusionTest(const std::string& rptDirectory)
{
  m_Logger->ResetLogFile(rptDirectory + "/EigenDiffusionTest.log");
  BioGears bg(m_Logger);
  Tissue& tsu = (Tissue&)bg.GetTissue();
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::On);
  bg.CreateCircuitsAndCompartments();

  std::string matrixFile = rptDirectory + "/EigenDiffusionTest.csv";
  DataTrack matrixTrk;

  //Use sodium, potassium, and chloride as model substances
  SESubstance* Na = &bg.GetSubstances().GetSodium();
  SESubstance* K = &bg.GetSubstances().GetPotassium();
  SESubstance* Cl = &bg.GetSubstances().GetChloride();

  bg.GetSubstances().AddActiveSubstance(*Na);
  bg.GetSubstances().AddActiveSubstance(*K);
  bg.GetSubstances().AddActiveSubstance(*Cl);

  std::vector<SESubstance*> subs = { Na, K, Cl };

  //Grab some existing compartments--notice that this test was done using BioGears Lite
  SELiquidCompartment* liverVas = bg.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Liver);
  SELiquidCompartment* liverTis = bg.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular);
  SELiquidCompartment* skinVas = bg.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Skin);
  SELiquidCompartment* skinTis = bg.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::SkinExtracellular);
  SELiquidCompartment* muscleVas = bg.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle);
  SELiquidCompartment* muscleTis = bg.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::MuscleExtracellular);

  //Set concentrations.  Liver uses vascular and intra values in engine and other cmpts mix the values so that we can see
  //different progressions towards equilibrium
  liverVas->GetSubstanceQuantity(*Na)->GetConcentration().SetValue(145.0, MassPerVolumeUnit::mg_Per_L);
  liverVas->GetSubstanceQuantity(*K)->GetConcentration().SetValue(4.5, MassPerVolumeUnit::mg_Per_L);
  liverVas->GetSubstanceQuantity(*Cl)->GetConcentration().SetValue(105.0, MassPerVolumeUnit::mg_Per_L);
  liverTis->GetSubstanceQuantity(*Na)->GetConcentration().SetValue(20.0, MassPerVolumeUnit::mg_Per_L);
  liverTis->GetSubstanceQuantity(*K)->GetConcentration().SetValue(120.0, MassPerVolumeUnit::mg_Per_L);
  liverTis->GetSubstanceQuantity(*Cl)->GetConcentration().SetValue(15.0, MassPerVolumeUnit::mg_Per_L);

  skinVas->GetSubstanceQuantity(*Na)->GetConcentration().SetValue(50.0, MassPerVolumeUnit::mg_Per_L);
  skinVas->GetSubstanceQuantity(*K)->GetConcentration().SetValue(40.0, MassPerVolumeUnit::mg_Per_L);
  skinVas->GetSubstanceQuantity(*Cl)->GetConcentration().SetValue(10.0, MassPerVolumeUnit::mg_Per_L);
  skinTis->GetSubstanceQuantity(*Na)->GetConcentration().SetValue(100.0, MassPerVolumeUnit::mg_Per_L);
  skinTis->GetSubstanceQuantity(*K)->GetConcentration().SetValue(20.0, MassPerVolumeUnit::mg_Per_L);
  skinTis->GetSubstanceQuantity(*Cl)->GetConcentration().SetValue(200.0, MassPerVolumeUnit::mg_Per_L);

  muscleVas->GetSubstanceQuantity(*Na)->GetConcentration().SetValue(5.0, MassPerVolumeUnit::mg_Per_L);
  muscleVas->GetSubstanceQuantity(*K)->GetConcentration().SetValue(80.0, MassPerVolumeUnit::mg_Per_L);
  muscleVas->GetSubstanceQuantity(*Cl)->GetConcentration().SetValue(500.0, MassPerVolumeUnit::mg_Per_L);
  muscleTis->GetSubstanceQuantity(*Na)->GetConcentration().SetValue(75.0, MassPerVolumeUnit::mg_Per_L);
  muscleTis->GetSubstanceQuantity(*K)->GetConcentration().SetValue(70.0, MassPerVolumeUnit::mg_Per_L);
  muscleTis->GetSubstanceQuantity(*Cl)->GetConcentration().SetValue(1.0, MassPerVolumeUnit::mg_Per_L);

  liverVas->Balance(BalanceLiquidBy::Concentration);
  liverTis->Balance(BalanceLiquidBy::Concentration);
  skinVas->Balance(BalanceLiquidBy::Concentration);
  skinTis->Balance(BalanceLiquidBy::Concentration);
  muscleVas->Balance(BalanceLiquidBy::Concentration);
  muscleTis->Balance(BalanceLiquidBy::Concentration);

  //Let's simulate 3- minutes
  double simTime_s = 60.0 * 60.0;
  double currentTime_s = 0.0;
  double timestep_s = 1.0 / 50.0;

  //Make matrices to hold concentrations--3x3 structered [Na, K, Cl] x [Liver, Skin, Muscle]
  //This probably is not the most efficient way to initialize these matrices but we'll worry about that later
  Eigen::Matrix3d vascular_mM;
  Eigen::Matrix3d tissue_mM;

  vascular_mM << 145.0, 50.0, 5.0, 4.5, 40.0, 80.0, 105.0, 10.0, 500.0;
  tissue_mM << 20.0, 100.0, 75.0, 120.0, 20.0, 70.0, 15.0, 200.0, 1.0;

  //Make a matrix to cache permeability coefficients and populate it so that pCoeff for Na, K, Cl are on diagonal in that order
  Eigen::Matrix3d pCoeff = Eigen::Matrix3d::Zero();
  for (int i = 0; i < subs.size(); i++) {
    double molarMass_g_Per_mol = subs[i]->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    double molecularRadius_nm = 0.0348 * std::pow(molarMass_g_Per_mol, 0.4175);
    double vToECpermeabilityCoefficient_mL_Per_s_g = 0.0287 * std::pow(molecularRadius_nm, -2.920) / 100.0; // This is only valid if the molecular radius is > 1.0 nm.
    if (molecularRadius_nm < 1.0)
      vToECpermeabilityCoefficient_mL_Per_s_g = 0.0184 * std::pow(molecularRadius_nm, -1.223) / 100.0;

    // Multiply by tissue mass to get the tissue-dependent coefficient.
    double tissueMass = 0.1; //Just assume this is the same for all of them--1000x less than assumed in standard loop above to account for mg to ug conversion
    double vToECpermeabilityCoefficient_mL_Per_s = vToECpermeabilityCoefficient_mL_Per_s_g * tissueMass;

    pCoeff(i, i) = vToECpermeabilityCoefficient_mL_Per_s;
  }

  std::vector<std::pair<SELiquidCompartment*, SELiquidCompartment*>> vascularTissuePair; //Need to store like this to preserve order
  vascularTissuePair.emplace_back(std::make_pair(liverVas, liverTis));
  vascularTissuePair.emplace_back(std::make_pair(skinVas, skinTis));
  vascularTissuePair.emplace_back(std::make_pair(muscleVas, muscleTis));

  Eigen::Matrix3d deltaM;
  while (currentTime_s < simTime_s) {
    deltaM = timestep_s * pCoeff * (vascular_mM - tissue_mM);
    for (int i = 0; i < vascularTissuePair.size(); i++) {
      for (int j = 0; j < subs.size(); j++) {
        vascularTissuePair[i].first->GetSubstanceQuantity(*subs[j])->GetMass().IncrementValue(-deltaM(j, i), MassUnit::mg);
        vascularTissuePair[i].first->GetSubstanceQuantity(*subs[j])->Balance(BalanceLiquidBy::Mass);
        vascular_mM(j, i) = vascularTissuePair[i].first->GetSubstanceQuantity(*subs[j])->GetConcentration(MassPerVolumeUnit::mg_Per_L);
        vascularTissuePair[i].second->GetSubstanceQuantity(*subs[j])->GetMass().IncrementValue(deltaM(j, i), MassUnit::mg);
        vascularTissuePair[i].second->GetSubstanceQuantity(*subs[j])->Balance(BalanceLiquidBy::Mass);
        tissue_mM(j, i) = vascularTissuePair[i].second->GetSubstanceQuantity(*subs[j])->GetConcentration(MassPerVolumeUnit::mg_Per_L);
      }
    }

    matrixTrk.Track("LiverVascular_Na", currentTime_s, liverVas->GetSubstanceQuantity(*Na)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("LiverVascular_K", currentTime_s, liverVas->GetSubstanceQuantity(*K)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("LiverVascular_Cl", currentTime_s, liverVas->GetSubstanceQuantity(*Cl)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("LiverTissue_Na", currentTime_s, liverTis->GetSubstanceQuantity(*Na)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("LiverTissue_K", currentTime_s, liverTis->GetSubstanceQuantity(*K)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("LiverTissue_Cl", currentTime_s, liverTis->GetSubstanceQuantity(*Cl)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("MuscleVascular_Na", currentTime_s, muscleVas->GetSubstanceQuantity(*Na)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("MuscleVascular_K", currentTime_s, muscleVas->GetSubstanceQuantity(*K)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("MuscleVascular_Cl", currentTime_s, muscleVas->GetSubstanceQuantity(*Cl)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("MuscleTissue_Na", currentTime_s, muscleTis->GetSubstanceQuantity(*Na)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("MuscleTissue_K", currentTime_s, muscleTis->GetSubstanceQuantity(*K)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("MuscleTissue_Cl", currentTime_s, muscleTis->GetSubstanceQuantity(*Cl)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("SkinVascular_Na", currentTime_s, skinVas->GetSubstanceQuantity(*Na)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("SkinVascular_K", currentTime_s, skinVas->GetSubstanceQuantity(*K)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("SkinVascular_Cl", currentTime_s, skinVas->GetSubstanceQuantity(*Cl)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("SkinTissue_Na", currentTime_s, skinTis->GetSubstanceQuantity(*Na)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("SkinTissue_K", currentTime_s, skinTis->GetSubstanceQuantity(*K)->GetConcentration(MassPerVolumeUnit::mg_Per_L));
    matrixTrk.Track("SkinTissue_Cl", currentTime_s, skinTis->GetSubstanceQuantity(*Cl)->GetConcentration(MassPerVolumeUnit::mg_Per_L));

    currentTime_s += timestep_s;
  }

  matrixTrk.WriteTrackToFile(matrixFile.c_str());

}

void BioGearsEngineTest::FacilitatedDiffusionTest(const std::string& rptDirectory)
{
  BioGears bg(m_Logger);
  Tissue& tsu = (Tissue&)bg.GetTissue();
  DiffusionCalculator& diffCalc = (DiffusionCalculator&)bg.GetDiffusionCalculator();
  SESubstance* sub = bg.GetSubstances().GetSubstance("Glucose");
  bg.GetSubstances().AddActiveSubstance(*sub);
  double timestep_s = 0.02;
  SETissueCompartment& tissue = bg.GetCompartments().CreateTissueCompartment("Tissue");
  SELiquidCompartment& tissueExtracellular = bg.GetCompartments().CreateLiquidCompartment("Extracellular");
  SELiquidCompartment& tissueIntracellular = bg.GetCompartments().CreateLiquidCompartment("Intracellular");
  SELiquidCompartment& vascular = bg.GetCompartments().CreateLiquidCompartment("Vascular");

  DataTrack trk1;
  std::string rptFile = rptDirectory + "/FacilitatedDiffusionTest.csv";
  double time = 0.0;

  tissueExtracellular.GetVolume().SetValue(45.0, VolumeUnit::mL);
  tissueIntracellular.GetVolume().SetValue(200.0, VolumeUnit::mL);
  vascular.GetVolume().SetValue(100.0, VolumeUnit::mL);
  tissueExtracellular.GetSubstanceQuantity(*sub)->GetMass().SetValue(2.9, MassUnit::g);
  tissueIntracellular.GetSubstanceQuantity(*sub)->GetMass().SetValue(1.0, MassUnit::g);
  vascular.GetSubstanceQuantity(*sub)->GetMass().SetValue(5.0, MassUnit::g);
  tissueExtracellular.Balance(BalanceLiquidBy::Mass);
  tissueIntracellular.Balance(BalanceLiquidBy::Mass);
  vascular.Balance(BalanceLiquidBy::Mass);
  
  DiffusionCalculator::DiffusionCompartmentSet diffSet = { &tissue, &vascular, &tissueExtracellular, &tissueIntracellular };

  double capCoverage_cm2 = 90.0; /// \todo Relate capillary coverage to tissue mass.
  double maximumMassFlux = sub->GetMaximumDiffusionFlux(MassPerAreaTimeUnit::g_Per_cm2_s);
  double combinedCoefficient_g_Per_s = maximumMassFlux * capCoverage_cm2;

  double vascularConc_g_Per_mL = vascular.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL);
  double ECconc_g_Per_mL = tissueExtracellular.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL);
  trk1.Track("VascularConc_g_Per_mL", time, vascularConc_g_Per_mL);
  trk1.Track("ExtracellularConc_g_Per_mL", time, ECconc_g_Per_mL);

  for (int i = 0; i < 180; i++) {
    diffCalc.CalculateFacilitatedDiffusion(diffSet, *sub, combinedCoefficient_g_Per_s);
    vascular.Balance(BalanceLiquidBy::Mass);
    tissueExtracellular.Balance(BalanceLiquidBy::Mass);
    time += timestep_s;
    vascularConc_g_Per_mL = vascular.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL);
    ECconc_g_Per_mL = tissueExtracellular.GetSubstanceQuantity(*sub)->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_mL);
    trk1.Track("VascularConc_g_Per_mL", time, vascularConc_g_Per_mL);
    trk1.Track("ExtracellularConc_g_Per_mL", time, ECconc_g_Per_mL);
  }

  trk1.WriteTrackToFile(rptFile.c_str());
}

void BioGearsEngineTest::GenericClearanceTest(SETestSuite& testSuite)
{
  TimingProfile timer;
  BioGears bg(testSuite.GetLogger());
  Tissue& tsu = (Tissue&)bg.GetTissue();

  // First test case
  SETestCase& testCase1 = testSuite.CreateTestCase();
  testCase1.SetName("GenericClearanceTestLiquid");
  // You at least need 2 test cases here (liquid and tissue)
  SETestCase& testCase = testSuite.CreateTestCase();
  timer.Start("Test");
  // Test Logic
  //testCase.GetFailures().push_back("Report any errors like this");
  // Log how long our test took
  testCase1.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);

  // Second test case
  SETestCase& testCase2 = testSuite.CreateTestCase();
  testCase2.SetName("GenericClearanceTestTissue");
  timer.Start("Test");

  testCase2.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);
  // Subsequent test cases
}

void BioGearsEngineTest::GenericExcretionTest(SETestSuite& testSuite)
{
  TimingProfile timer;
  BioGears bg(testSuite.GetLogger());
  Tissue& tsu = (Tissue&)bg.GetTissue();

  // First test case
  SETestCase& testCase1 = testSuite.CreateTestCase();
  testCase1.SetName("GenericExcretionTest");
  timer.Start("Test");
  // Test Logic
  //testCase.GetFailures().push_back("Report any errors like this");
  // Log how long our test took
  testCase1.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);
}

//Set-up the test suite
void BioGearsEngineTest::DiffusionClearanceExcretionTests(const std::string& rptDirectory)
{

  m_Logger->ResetLogFile(rptDirectory + "/DiffusionClearanceExcretionTests.log");
  // Set up our test report

  SETestReport testReport = SETestReport(m_Logger);

  SETestSuite& tsDistMass = testReport.CreateTestSuite();
  tsDistMass.SetName("DistributeMass");
  DistributeMass(tsDistMass);

  SETestSuite& ts1 = testReport.CreateTestSuite();
  ts1.SetName("PerfusionLimitedDiffusionTest");
  PerfusionLimitedDiffusionTest(ts1);

  //SETestSuite& ts7 = testReport.CreateTestSuite();
  //ts6.SetName("GenericClearanceTest");
  //GenericClearanceTest(ts7);

  //SETestSuite& ts8 = testReport.CreateTestSuite();
  //ts7.SetName("GenericExcretionTest");
  //GenericExcretionTest(ts8);

  //testReport.WriteFile(rptDirectory + "/GasCompartmentTest.xml");
}

void BioGearsEngineTest::TissueCombinedTransportTest(const std::string& rptDirectory)
{
  enum testCases { ExtraIntra = 0,
                   VascExtra = 1,
                   Combined = 2 };
  int testRun = VascExtra;
  m_Logger->ResetLogFile(rptDirectory + "/TissueCombinedTransportTest.log");
  std::string circuitFile = rptDirectory + "/TissueTransportCircuit.csv";
  std::string testFile = rptDirectory + "/TissueCombinedTransportTest.csv";
  std::string stabilizationFile = rptDirectory + "/TissueCombinedStabilization.csv";
  BioGears bg(m_Logger);
  SECircuitManager circuits(m_Logger);
  SELiquidTransporter txpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, m_Logger);
  SEFluidCircuitCalculator calc(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, m_Logger);
  DataTrack circuitTrk;
  DataTrack stabilization;
  DataTrack testTrk;
  DataTrack trk;

  Tissue& tsu = (Tissue&)bg.GetTissue();
  DiffusionCalculator& diffCalc = (DiffusionCalculator&)bg.GetDiffusionCalculator();
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();

  SESubstance& Na = bg.GetSubstances().GetSodium();
  SESubstance& K = bg.GetSubstances().GetPotassium();
  SESubstance& Cl = bg.GetSubstances().GetChloride();
  SESubstance& Alb = bg.GetSubstances().GetAlbumin();

  bg.GetSubstances().AddActiveSubstance(Na);
  bg.GetSubstances().AddActiveSubstance(K);
  bg.GetSubstances().AddActiveSubstance(Cl);
  bg.GetSubstances().AddActiveSubstance(Alb);

  std::vector<SESubstance*> subs = { &Na, &K, &Cl, &Alb };

  //General sim parameters
  double dt = 1.0; //s
  double time = 0.0; //s
  double perturb = 0.0;
  double stabilizationTime = 30; //min
  double simTime = 360; //Total sim length
  double startInfuse = 30.0;
  double endInfuse = -1.0; //Means we won't do infusion
  bool testSaline = true;
  //Substance concentrations
  double Na_Extra = 145.0;
  double Na_Intra = 15.0;
  double K_Extra = 4.5;
  double K_Intra = 120.0;
  double Cl_Extra = 102.0;
  double Cl_Intra = 4.5;
  double Na_Vasc = Na_Extra;
  double K_Vasc = K_Extra;
  double Cl_Vasc = Cl_Extra;
  //Albumin parameters
  double Alb_Vasc_g_Per_dL = 4.5;
  double Alb_Extra_g_Per_dL = 2.0;
  double albReflection = 0.94;
  double kAlbumin_mL_Per_s = 0.01339; //Empirically determined to get net albumin flux = 0 at steady state flows
  double massVascularToInterstitial_ug = 0.0;
  double massInterstitialToLymph_ug = 0.0;

  double Potential = -84.8;
  double flowRate_mL_Per_s = 0.0; //for tracking filtration in test 1

  //--Set up estimated circuit values--//
  double ECF_L = 18.5;
  double ICF_L = 25.0;

  double aortaPressure_mmHg = 90.0; //Heart drive pressure
  double veinsPressure_mmHg = 4.0; //Vein pressure
  double muscleVascularPressure_mmHg = 38.0; //Pressure stored on muscle vascular node in BioGears setup
  double cardiacOutput_mL_Per_s = 98.0;
  double muscleFlow_mL_Per_s = 0.17 * cardiacOutput_mL_Per_s; //Target blood flow rate since we are just testing the muscle compartment as proxy

  double totalVolume_mL = 5170; //Use this to get target compartment volumes
  double aortaVolume_mL = 0.05 * totalVolume_mL;
  double veinsVolume_mL = 0.247 * totalVolume_mL;
  double muscleVascularVolume_mL = 0.14 * totalVolume_mL; //This is the target muscle volume stored on vascular node in BioGears setup
  double vascularVolumeBaseline_mL = aortaVolume_mL + veinsVolume_mL + muscleVascularVolume_mL; //We're not using other compartments
  double muscleTissueVolume_L = 29.0; //This is the tissue volume and mass (1 kg/L density assumed for muscle in BioGears)
  double volumeTrk = 0.0;
  double muscleEWFraction = 0.118;
  double MuscleIWFraction = 0.63;
  double muscleExtracellularVolume_mL = muscleEWFraction * muscleTissueVolume_L * 1000;
  double muscleIntracellularVolume_mL = MuscleIWFraction * muscleTissueVolume_L * 1000;
  double lymphFlowBaseline_mL_Per_s = 0.0412 * muscleTissueVolume_L / 60; //Convert mL/min-kg tissue mass from Carlson1996Impairment to mL/s

  double heartToMuscleResistance = (aortaPressure_mmHg - muscleVascularPressure_mmHg) / muscleFlow_mL_Per_s; //mmHg-s/mL
  double muscleToVeinResistance = (muscleVascularPressure_mmHg - veinsPressure_mmHg) / (muscleFlow_mL_Per_s - lymphFlowBaseline_mL_Per_s); //mmHg-s/mL
  double veinToHeartResistance = (veinsPressure_mmHg) / muscleFlow_mL_Per_s; //mmHg-s/mL --> assumes heart entry node is reference pressure
  double vascularToExtraResistance_mmHg_min_Per_mL = 1.0 / 5.82;

  double capillaryCOP_mmHg = 2.1 * Alb_Vasc_g_Per_dL + 0.16 * std::pow(Alb_Vasc_g_Per_dL, 2) + 0.009 * std::pow(Alb_Vasc_g_Per_dL, 3);
  double interstitialCOP_mmHg = 2.1 * Alb_Extra_g_Per_dL + 0.16 * std::pow(Alb_Extra_g_Per_dL, 2) + 0.009 * std::pow(Alb_Extra_g_Per_dL, 3);
  double targetGradient_mmHg = 3.0;
  double hydrostaticGradient_mmHg = targetGradient_mmHg + albReflection * (capillaryCOP_mmHg - interstitialCOP_mmHg);
  double e1NodePressure = muscleVascularPressure_mmHg - capillaryCOP_mmHg; //We're assuming the pressure drop across the resistor between Ex1 and Ex2 is very small
  double e3NodePressure = muscleVascularPressure_mmHg - hydrostaticGradient_mmHg;
  double e2NodePressure = e3NodePressure - interstitialCOP_mmHg;
  double targetFlow_mL_Per_min = targetGradient_mmHg / vascularToExtraResistance_mmHg_min_Per_mL;
  double ivRate_mL_Per_s = (0.05 + 0.247 + 0.14) * 1000.0 / 3600.0; //1 L per hour to mL/s, scaled down to available volume in this circuit
  double salineNa_mg_Per_mL = 3.54;
  double salineCl_mg_Per_mL = 5.46;
  double ivAlbumin_mg_Per_mL = 74.50;
  double vascularVolume_mL = 0.0;
  double pumpCompliance_mL_Per_mmHg = 100.0 / 1.5; //From Chaim2016Haemodynamic--need this to adjust pump rate when IV fluid is given since volume expansion occurs
  double diffusivityCoefficient_mL_Per_min = 0.97;
  double albuminDiffusive_ug_Per_min;
  double albuminConvective_ug_Per_min;
  double lymphSensitivity = 0.4 * targetFlow_mL_Per_min;
  double lymphFlow_mL_Per_s = 0.0;
  double interstitialPressureBaseline_mmHg;

  //Extra<->Intra Test Circuit
  SEFluidCircuit* IEcircuit = &circuits.CreateFluidCircuit("CircuitDeSoleil");

  //Nodes
  SEFluidCircuitNode& E1 = IEcircuit->CreateNode("nExtra1");
  E1.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  E1.GetNextPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  E1.GetVolumeBaseline().SetValue(ECF_L, VolumeUnit::L);
  SEFluidCircuitNode& I1 = IEcircuit->CreateNode("nIntra1");
  I1.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  I1.GetNextPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  I1.GetVolumeBaseline().SetValue(ICF_L, VolumeUnit::L);
  SEFluidCircuitNode& r = IEcircuit->CreateNode("nRef");
  r.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  IEcircuit->AddReferenceNode(r);
  //Paths
  SEFluidCircuitPath& E1ToI1 = IEcircuit->CreatePath(E1, I1, "pE1ToI1");
  E1ToI1.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& refToE1 = IEcircuit->CreatePath(r, E1, "refToE1");
  refToE1.GetComplianceBaseline().SetValue(ECF_L * 1000.0 / e3NodePressure, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& I1Toref = IEcircuit->CreatePath(I1, r, "I1Toref");
  I1Toref.GetComplianceBaseline().SetValue(ICF_L * 1000.0 / e3NodePressure, FlowComplianceUnit::mL_Per_mmHg);

  IEcircuit->SetNextAndCurrentFromBaselines();
  IEcircuit->StateChange();

  //Compartments
  SELiquidCompartment& Extra = bg.GetCompartments().CreateLiquidCompartment("cExtra");
  Extra.MapNode(E1);
  SELiquidCompartment& Intra = bg.GetCompartments().CreateLiquidCompartment("cIntra");
  Intra.MapNode(I1);
  SETissueCompartment& MuscleTissue = bg.GetCompartments().CreateTissueCompartment("cTissue");
  MuscleTissue.GetMembranePotential().SetValue(Potential, ElectricPotentialUnit::mV);
  MuscleTissue.GetTotalMass().SetValue(29.0, MassUnit::kg);
  SELiquidCompartment& Bloodstream = bg.GetCompartments().CreateLiquidCompartment("cBlood");
  Bloodstream.GetVolume().SetValue(1.15, VolumeUnit::L); //assuming volume assigned to arteries in four compartment test

  //Constants and circuit elements for VascExtra test
  SELiquidCompartmentGraph* Graph = &bg.GetCompartments().CreateLiquidGraph("Graphite");
  Graph->Clear(); //Empty graph so we just add what we want
  Graph->StateChange();
  SEFluidCircuit* EVcircuit = &circuits.CreateFluidCircuit("FlyingCircuit");

  //Nodes
  SEFluidCircuitNode& nVeins = EVcircuit->CreateNode("VeinNode");
  nVeins.GetVolumeBaseline().SetValue(veinsVolume_mL, VolumeUnit::mL);
  nVeins.GetPressure().SetValue(veinsPressure_mmHg, PressureUnit::mmHg);
  nVeins.GetNextPressure().SetValue(veinsPressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitNode& nMuscleVascular = EVcircuit->CreateNode("MuscleNode");
  nMuscleVascular.GetVolumeBaseline().SetValue(muscleVascularVolume_mL, VolumeUnit::mL);
  nMuscleVascular.GetPressure().SetValue(muscleVascularPressure_mmHg, PressureUnit::mmHg);
  nMuscleVascular.GetNextPressure().SetValue(muscleVascularPressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitNode& nHeartIn = EVcircuit->CreateNode("HeartInNode");
  nHeartIn.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  nHeartIn.GetNextPressure().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitNode& nHeartOut = EVcircuit->CreateNode("HeartOutNode");
  nHeartOut.GetVolumeBaseline().SetValue(aortaVolume_mL, VolumeUnit::mL); //Not necessary to put a compliance on this path
  SEFluidCircuitNode& nEx1 = EVcircuit->CreateNode("ExtraNode1");
  nEx1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  nEx1.GetNextPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  SEFluidCircuitNode& nEx2 = EVcircuit->CreateNode("ExtraNode2");
  nEx2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  nEx2.GetNextPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  SEFluidCircuitNode& nEx3 = EVcircuit->CreateNode("ExtraNode3");
  nEx3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  nEx3.GetNextPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  nEx3.GetVolumeBaseline().SetValue(muscleExtracellularVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nLymph = EVcircuit->CreateNode("LymphNode1");
  nLymph.GetVolumeBaseline().SetValue(5.0, VolumeUnit::L);
  SEFluidCircuitNode& nGround = EVcircuit->CreateNode("GroundNode");
  nGround.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  EVcircuit->AddReferenceNode(nGround);

  //Paths
  SEFluidCircuitPath& pHeartToMuscle = EVcircuit->CreatePath(nHeartOut, nMuscleVascular, "HeartToMusclePath");
  pHeartToMuscle.GetResistanceBaseline().SetValue(heartToMuscleResistance, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& pMuscleToVeins = EVcircuit->CreatePath(nMuscleVascular, nVeins, "MuscleToVeinsPath");
  pMuscleToVeins.GetResistanceBaseline().SetValue(muscleToVeinResistance, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& pVeinsToHeart = EVcircuit->CreatePath(nVeins, nHeartIn, "VeinsToHeartPath");
  pVeinsToHeart.GetResistanceBaseline().SetValue(veinToHeartResistance, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& pHeartPump = EVcircuit->CreatePath(nHeartIn, nHeartOut, "HeartPumpPath");
  pHeartPump.GetPressureSourceBaseline().SetValue(90, PressureUnit::mmHg); //From BioGears CV circuit setup
  SEFluidCircuitPath& pPlasmaCOP = EVcircuit->CreatePath(nEx1, nMuscleVascular, "PlasmaCOP"); //Note direction interstitial to vascular
  pPlasmaCOP.GetPressureSourceBaseline().SetValue(albReflection * capillaryCOP_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pEx1ToEx2 = EVcircuit->CreatePath(nEx1, nEx2, "ExtraResistancePath");
  pEx1ToEx2.GetResistanceBaseline().SetValue(vascularToExtraResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pInterstitialCOP = EVcircuit->CreatePath(nEx2, nEx3, "InterstitialCOP");
  pInterstitialCOP.GetPressureSourceBaseline().SetValue(albReflection * interstitialCOP_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pEx3ToLymph = EVcircuit->CreatePath(nEx3, nLymph, "ExtracellularToLymph1Path");
  pEx3ToLymph.GetFlowSourceBaseline().SetValue(targetFlow_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
  SEFluidCircuitPath& pLymphToVeins = EVcircuit->CreatePath(nLymph, nVeins, "LymphReturnPath");
  pLymphToVeins.GetFlowSourceBaseline().SetValue(targetFlow_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
  SEFluidCircuitPath& pHeartInToGround = EVcircuit->CreatePath(nHeartIn, nGround, "HeartToGroundPath"); //Need this to get reference value for circuit
  pHeartInToGround.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& pMuscleToGround = EVcircuit->CreatePath(nMuscleVascular, nGround, "GroundToMusclePath");
  pMuscleToGround.GetComplianceBaseline().SetValue(muscleVascularVolume_mL / muscleVascularPressure_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& pVeinsToGround = EVcircuit->CreatePath(nVeins, nGround, "GroundToVeinsPath");
  pVeinsToGround.GetComplianceBaseline().SetValue(0.17 * veinsVolume_mL / veinsPressure_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& pEx3ToGround = EVcircuit->CreatePath(nEx3, nGround, "ExtraToGroundPath");
  pEx3ToGround.GetComplianceBaseline().SetValue(200.0, FlowComplianceUnit::mL_Per_mmHg); //This value comes from Reisner2012Computational
  EVcircuit->SetNextAndCurrentFromBaselines();
  EVcircuit->StateChange();

  //Compartments
  SELiquidCompartment& cVeins = bg.GetCompartments().CreateLiquidCompartment("VeinCompartment");
  cVeins.MapNode(nVeins);
  SELiquidCompartment& cHeart = bg.GetCompartments().CreateLiquidCompartment("HeartCompartment");
  cHeart.MapNode(nHeartIn);
  cHeart.MapNode(nHeartOut);
  SELiquidCompartment& cMuscle = bg.GetCompartments().CreateLiquidCompartment("MuscleVascularCompartment");
  cMuscle.MapNode(nMuscleVascular);
  SELiquidCompartment& cExtraCell = bg.GetCompartments().CreateLiquidCompartment("MuscleExtracellularCompartment");
  cExtraCell.MapNode(nEx1);
  cExtraCell.MapNode(nEx2);
  cExtraCell.MapNode(nEx3);
  SELiquidCompartment& cLymph = bg.GetCompartments().CreateLiquidCompartment("MuscleLymphCompartment");
  cLymph.MapNode(nLymph);
  SETissueCompartment& cTissue = bg.GetCompartments().CreateTissueCompartment("MuscleTissueCompartment");
  cTissue.GetTotalMass().SetValue(29.0, MassUnit::kg);
  cTissue.GetMembranePotential().SetValue(Potential, ElectricPotentialUnit::mV);

  //Links
  SELiquidCompartmentLink& lHeartToMuscle = bg.GetCompartments().CreateLiquidLink(cHeart, cMuscle, "HeartToMuscleLink");
  lHeartToMuscle.MapPath(pHeartToMuscle);
  SELiquidCompartmentLink& lMuscleToVeins = bg.GetCompartments().CreateLiquidLink(cMuscle, cVeins, "MuscleToVeinsLink");
  lMuscleToVeins.MapPath(pMuscleToVeins);
  SELiquidCompartmentLink& lVeinsToHeart = bg.GetCompartments().CreateLiquidLink(cVeins, cHeart, "VeinsToHeartLink");
  lVeinsToHeart.MapPath(pVeinsToHeart);
  SELiquidCompartmentLink& lVascularToExtra = bg.GetCompartments().CreateLiquidLink(cMuscle, cExtraCell, "VascularToExtraLink");
  lVascularToExtra.MapPath(pPlasmaCOP);
  SELiquidCompartmentLink& lExtraToLymph = bg.GetCompartments().CreateLiquidLink(cExtraCell, cLymph, "ExtracellToLymphLink");
  lExtraToLymph.MapPath(pEx3ToLymph);
  SELiquidCompartmentLink& lLymphToVeins = bg.GetCompartments().CreateLiquidLink(cLymph, cVeins, "LymphToVeinsLink");
  lLymphToVeins.MapPath(pLymphToVeins);

  Graph->AddCompartment(cVeins);
  Graph->AddCompartment(cHeart);
  Graph->AddCompartment(cMuscle);
  Graph->AddCompartment(cExtraCell);
  Graph->AddCompartment(cLymph);
  Graph->AddLink(lHeartToMuscle);
  Graph->AddLink(lMuscleToVeins);
  Graph->AddLink(lVeinsToHeart);
  //Graph->AddLink(lExtraToLymph);
  //Graph->AddLink(lLymphToVeins);
  Graph->StateChange();

  std::vector<SELiquidCompartment*> Comps = Graph->GetCompartments();

  //Combined circuit
  SEFluidCircuit* EVIcircuit = &circuits.CreateFluidCircuit("CircuitFreaks");
  SELiquidCompartmentGraph* CombinedGraph = &bg.GetCompartments().CreateLiquidGraph("CombinedGraph");
  EVIcircuit->AddCircuit(*EVcircuit); //Combined circuit is the exact same as the EV test circuit, we are just going to add a couple of elements for intercellular space
  SEFluidCircuitNode& nI1 = EVIcircuit->CreateNode("IntracellularNode");
  nI1.GetVolumeBaseline().SetValue(muscleIntracellularVolume_mL, VolumeUnit::mL);
  nI1.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //Intracellular pressure should be the same as extracellular (no pressure gradient)
  nI1.GetNextPressure().SetValue(e3NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& pCellMembrane = EVIcircuit->CreatePath(nEx3, nI1, "ExtraToIntraPath");
  pCellMembrane.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& pIntraToGround = EVIcircuit->CreatePath(nI1, nGround, "IntraToGround");
  pIntraToGround.GetComplianceBaseline().SetValue(muscleIntracellularVolume_mL / e3NodePressure, FlowComplianceUnit::mL_Per_mmHg);
  EVIcircuit->SetNextAndCurrentFromBaselines();
  EVIcircuit->StateChange();

  SELiquidCompartment& cIntraCell = bg.GetCompartments().CreateLiquidCompartment("MuscleIntracellularCompartment");
  cIntraCell.MapNode(nI1);

  CombinedGraph->AddGraph(*Graph); //Add in all of the extracellular and vascular compartments
  CombinedGraph->AddCompartment(cIntraCell);
  CombinedGraph->StateChange();

  std::vector<SELiquidCompartment*> combinedComps = CombinedGraph->GetCompartments();
  double lymphTest = 0.0;

  switch (testRun) {
  case ExtraIntra:
    //Initialize substance concentrations--since there is no link between compartments, the only way to move them is by explicitly calling a Diffusion method
    Extra.GetSubstanceQuantity(Na)->GetMolarity().SetValue(Na_Extra, AmountPerVolumeUnit::mmol_Per_L);
    Intra.GetSubstanceQuantity(Na)->GetMolarity().SetValue(Na_Intra, AmountPerVolumeUnit::mmol_Per_L);
    Bloodstream.GetSubstanceQuantity(Na)->GetMolarity().SetValue(Na_Extra, AmountPerVolumeUnit::mmol_Per_L);
    Extra.GetSubstanceQuantity(K)->GetMolarity().SetValue(K_Extra, AmountPerVolumeUnit::mmol_Per_L);
    Intra.GetSubstanceQuantity(K)->GetMolarity().SetValue(K_Intra, AmountPerVolumeUnit::mmol_Per_L);
    Bloodstream.GetSubstanceQuantity(K)->GetMolarity().SetValue(K_Extra, AmountPerVolumeUnit::mmol_Per_L);
    Extra.GetSubstanceQuantity(Cl)->GetMolarity().SetValue(Cl_Extra, AmountPerVolumeUnit::mmol_Per_L);
    Intra.GetSubstanceQuantity(Cl)->GetMolarity().SetValue(Cl_Intra, AmountPerVolumeUnit::mmol_Per_L);
    Bloodstream.GetSubstanceQuantity(Cl)->GetMolarity().SetValue(Cl_Extra, AmountPerVolumeUnit::mmol_Per_L);

    Extra.Balance(BalanceLiquidBy::Molarity);
    Intra.Balance(BalanceLiquidBy::Molarity);
    Bloodstream.Balance(BalanceLiquidBy::Molarity);

    ICF_L = Intra.GetVolume(VolumeUnit::L);
    Na_Intra = Intra.GetSubstanceQuantity(Na)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    K_Intra = Intra.GetSubstanceQuantity(K)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    Cl_Intra = Intra.GetSubstanceQuantity(Cl)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    ECF_L = Extra.GetVolume(VolumeUnit::L);
    Na_Extra = Extra.GetSubstanceQuantity(Na)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    K_Extra = Extra.GetSubstanceQuantity(K)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    Cl_Extra = Extra.GetSubstanceQuantity(Cl)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    flowRate_mL_Per_s = E1ToI1.GetFlow(VolumePerTimeUnit::mL_Per_s);

    trk.Track("Intra_Vol", time, ICF_L);
    trk.Track("Intra_Na", time, Na_Intra);
    trk.Track("Intra_K", time, K_Intra);
    trk.Track("Intra_Cl", time, Cl_Intra);
    trk.Track("Extra_Vol", time, ECF_L);
    trk.Track("Extra_Na", time, Na_Extra);
    trk.Track("Extra_K", time, K_Extra);
    trk.Track("Extra_Cl", time, Cl_Extra);
    trk.Track("Vascular_Na", time, Na_Vasc);
    trk.Track("Vascular_K", time, K_Vasc);
    trk.Track("Vascular_Cl", time, Cl_Vasc);
    trk.Track("FlowRate_mL_Per_s", time, flowRate_mL_Per_s);
    trk.Track("Potential", time, Potential);
    trk.Track(time, *IEcircuit);

    dt = 0.2;

    for (int i = 0; i < simTime * 60 / dt; i++) {
      if (i == 90 * 60 / dt) //Change conditions at 90 minutes
      {
        Extra.GetVolume().IncrementValue(1.5, VolumeUnit::L);
        Extra.GetSubstanceQuantity(Na)->GetMass().IncrementValue(0.145 * 1.5 * Na.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        Extra.GetSubstanceQuantity(Cl)->GetMass().IncrementValue(0.145 * 1.5 * Cl.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        Extra.Balance(BalanceLiquidBy::Mass);
      }

      calc.Process(*IEcircuit, dt);

      ICF_L = Intra.GetVolume(VolumeUnit::L);
      Na_Intra = Intra.GetSubstanceQuantity(Na)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
      K_Intra = Intra.GetSubstanceQuantity(K)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
      Cl_Intra = Intra.GetSubstanceQuantity(Cl)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
      ECF_L = Extra.GetVolume(VolumeUnit::L);
      Na_Extra = Extra.GetSubstanceQuantity(Na)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
      K_Extra = Extra.GetSubstanceQuantity(K)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
      Cl_Extra = Extra.GetSubstanceQuantity(Cl)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
      Na_Vasc = Bloodstream.GetSubstanceQuantity(Na)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
      K_Vasc = Bloodstream.GetSubstanceQuantity(K)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
      Cl_Vasc = Bloodstream.GetSubstanceQuantity(Cl)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
      flowRate_mL_Per_s = E1ToI1.GetNextFlowSource(VolumePerTimeUnit::mL_Per_s);
      Potential = MuscleTissue.GetMembranePotential(ElectricPotentialUnit::mV);

      calc.PostProcess(*IEcircuit);

      trk.Track(time, *IEcircuit);
      trk.Track("Intra_Vol", time, ICF_L);
      trk.Track("Intra_Na", time, Na_Intra);
      trk.Track("Intra_K", time, K_Intra);
      trk.Track("Intra_Cl", time, Cl_Intra);
      trk.Track("Extra_Vol", time, ECF_L);
      trk.Track("Extra_Na", time, Na_Extra);
      trk.Track("Extra_K", time, K_Extra);
      trk.Track("Extra_Cl", time, Cl_Extra);
      trk.Track("Vascular_Na", time, Na_Vasc);
      trk.Track("Vascular_K", time, K_Vasc);
      trk.Track("Vascular_Cl", time, Cl_Vasc);
      trk.Track("FlowRate_mL_Per_s", time, flowRate_mL_Per_s);
      trk.Track("Potential", time, Potential);
      time += dt;
    }

    trk.WriteTrackToFile(testFile.c_str());
    break;

  case VascExtra:
    for (int i = 0; i < stabilizationTime * 60 / dt; i++) {
      calc.Process(*EVcircuit, dt);
      calc.PostProcess(*EVcircuit);
      volumeTrk = 0.0;
      for (auto comp : Comps) {
        volumeTrk += comp->GetVolume(VolumeUnit::mL);
      }
      stabilization.Track(time, *EVcircuit);
      stabilization.Track("TotalVolume_mL", time, volumeTrk);
      time += dt;
    }
    Info("Finished stabilization");
    stabilization.WriteTrackToFile(stabilizationFile.c_str());
    time = 0.0;
    dt = 0.2;
    vascularVolumeBaseline_mL = 0.0; //Reset this since compliances might have caused them to shift
    lymphFlowBaseline_mL_Per_s = pEx3ToLymph.GetFlow(VolumePerTimeUnit::mL_Per_s);
    muscleExtracellularVolume_mL = cExtraCell.GetVolume(VolumeUnit::mL); //Get stabilized volume to use for lymph relationship
    interstitialPressureBaseline_mmHg = nEx3.GetPressure(PressureUnit::mmHg);
    for (auto comp : Comps) {
      if ((comp->GetName() == "MuscleExtracellularCompartment") || (comp->GetName() == "MuscleLymphCompartment"))
        continue;
      vascularVolumeBaseline_mL += comp->GetVolume(VolumeUnit::mL);
    }
    //Set albumin concentrations first
    cHeart.GetSubstanceQuantity(Alb)->GetConcentration().SetValue(Alb_Vasc_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
    cVeins.GetSubstanceQuantity(Alb)->GetConcentration().SetValue(Alb_Vasc_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
    cMuscle.GetSubstanceQuantity(Alb)->GetConcentration().SetValue(Alb_Vasc_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
    cExtraCell.GetSubstanceQuantity(Alb)->GetConcentration().SetValue(Alb_Extra_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
    cLymph.GetSubstanceQuantity(Alb)->GetConcentration().SetValue(Alb_Extra_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
    for (auto comp : Comps) {
      comp->Balance(BalanceLiquidBy::Concentration); //Balance the albumins, do it here to reduce copy/paste
      comp->GetSubstanceQuantity(Na)->GetMolarity().SetValue(Na_Vasc / 1000.0, AmountPerVolumeUnit::mol_Per_L);
      comp->GetSubstanceQuantity(K)->GetMolarity().SetValue(K_Vasc / 1000.0, AmountPerVolumeUnit::mol_Per_L);
      comp->GetSubstanceQuantity(Cl)->GetMolarity().SetValue(Cl_Vasc / 1000.0, AmountPerVolumeUnit::mol_Per_L);
      comp->Balance(BalanceLiquidBy::Molarity); //Now balance the ions
    }

    for (int j = 0; j < simTime * 60 / dt; j++) {

      //Need to move the albumin manually--by definition a positive value is from capillary to interstitial
      //Don't worry about "DistributeMass" functions because we don't have any child compartments to worry about here
      albuminDiffusive_ug_Per_min = diffusivityCoefficient_mL_Per_min * (cMuscle.GetSubstanceQuantity(Alb)->GetConcentration(MassPerVolumeUnit::ug_Per_mL) - cExtraCell.GetSubstanceQuantity(Alb)->GetConcentration(MassPerVolumeUnit::ug_Per_mL));
      albuminConvective_ug_Per_min = pEx1ToEx2.GetFlow(VolumePerTimeUnit::mL_Per_min) * (1.0 - albReflection) * (cMuscle.GetSubstanceQuantity(Alb)->GetConcentration(MassPerVolumeUnit::ug_Per_mL) + cExtraCell.GetSubstanceQuantity(Alb)->GetConcentration(MassPerVolumeUnit::ug_Per_mL)) / 2.0;
      massVascularToInterstitial_ug = (albuminDiffusive_ug_Per_min + albuminConvective_ug_Per_min) / 60.0 * dt;

      massInterstitialToLymph_ug = lymphFlow_mL_Per_s * cExtraCell.GetSubstanceQuantity(Alb)->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * dt;

      cMuscle.GetSubstanceQuantity(Alb)->GetMass().IncrementValue(-massVascularToInterstitial_ug, MassUnit::ug);
      cExtraCell.GetSubstanceQuantity(Alb)->GetMass().IncrementValue(massVascularToInterstitial_ug - massInterstitialToLymph_ug, MassUnit::ug);
      cLymph.GetSubstanceQuantity(Alb)->GetMass().IncrementValue(massInterstitialToLymph_ug, MassUnit::ug);
      cMuscle.GetSubstanceQuantity(Alb)->Balance(BalanceLiquidBy::Mass);
      cExtraCell.GetSubstanceQuantity(Alb)->Balance(BalanceLiquidBy::Mass);
      cLymph.GetSubstanceQuantity(Alb)->Balance(BalanceLiquidBy::Mass);

      //Calculate next osmotic pressure gradient
      Alb_Vasc_g_Per_dL = cMuscle.GetSubstanceQuantity(Alb)->GetConcentration(MassPerVolumeUnit::g_Per_dL);
      Alb_Extra_g_Per_dL = cExtraCell.GetSubstanceQuantity(Alb)->GetConcentration(MassPerVolumeUnit::g_Per_dL);
      capillaryCOP_mmHg = 2.1 * Alb_Vasc_g_Per_dL + 0.16 * std::pow(Alb_Vasc_g_Per_dL, 2) + 0.009 * std::pow(Alb_Vasc_g_Per_dL, 3);
      interstitialCOP_mmHg = 2.1 * Alb_Extra_g_Per_dL + 0.16 * std::pow(Alb_Extra_g_Per_dL, 2) + 0.009 * std::pow(Alb_Extra_g_Per_dL, 3);
      pPlasmaCOP.GetNextPressureSource().SetValue(albReflection * capillaryCOP_mmHg, PressureUnit::mmHg);
      pInterstitialCOP.GetNextPressureSource().SetValue(albReflection * interstitialCOP_mmHg, PressureUnit::mmHg);

      //Adjust lymph flow
      //pEx3ToLymph.GetNextPressureSource().SetValue(nVeins.GetPressure(PressureUnit::mmHg) - veinsPressure_mmHg, PressureUnit::mmHg);
      lymphFlow_mL_Per_s = lymphFlowBaseline_mL_Per_s + lymphSensitivity * (nEx3.GetPressure(PressureUnit::mmHg) - interstitialPressureBaseline_mmHg);
      BLIM(lymphFlow_mL_Per_s, 0.5 * lymphFlowBaseline_mL_Per_s, 22.0 * lymphFlowBaseline_mL_Per_s); //UB comes from Mazzoni and Himeno
      pEx3ToLymph.GetNextFlowSource().SetValue(lymphFlow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
      pLymphToVeins.GetNextFlowSource().SetValue(lymphFlow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
      vascularVolume_mL = 0.0;
      volumeTrk = 0.0;

      for (auto comp : Comps) {
        volumeTrk += comp->GetVolume(VolumeUnit::mL);
        if ((comp->GetName() == "MuscleExtracellularCompartment") || (comp->GetName() == "MuscleLymphCompartment"))
          continue;
        vascularVolume_mL += comp->GetVolume(VolumeUnit::mL);
      }
      aortaPressure_mmHg = 90.0 + (1 / pumpCompliance_mL_Per_mmHg) * (vascularVolume_mL - vascularVolumeBaseline_mL);
      pHeartPump.GetNextPressureSource().SetValue(aortaPressure_mmHg, PressureUnit::mmHg);
      if ((time >= startInfuse * 60) && (time < endInfuse * 60.0)) {
        pHeartInToGround.GetNextFlowSource().SetValue(-ivRate_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
        if (testSaline) {
          cHeart.GetSubstanceQuantity(Na)->GetMass().IncrementValue(ivRate_mL_Per_s * salineNa_mg_Per_mL * dt, MassUnit::mg);
          cHeart.GetSubstanceQuantity(Cl)->GetMass().IncrementValue(ivRate_mL_Per_s * salineCl_mg_Per_mL * dt, MassUnit::mg);
          cHeart.GetSubstanceQuantity(Na)->Balance(BalanceLiquidBy::Mass);
          cHeart.GetSubstanceQuantity(Cl)->Balance(BalanceLiquidBy::Mass);
        } else {
          //Must be testing albumin
          cHeart.GetSubstanceQuantity(Alb)->GetMass().IncrementValue(ivRate_mL_Per_s * ivAlbumin_mg_Per_mL * dt, MassUnit::mg);
          cHeart.GetSubstanceQuantity(Alb)->Balance(BalanceLiquidBy::Mass);
        }
      }

      calc.Process(*EVcircuit, dt);
      nHeartIn.GetNextPressure().SetValue(0.0, PressureUnit::mmHg);
      txpt.Transport(*Graph, dt);
      calc.PostProcess(*EVcircuit);

      circuitTrk.Track(time, *EVcircuit);
      testTrk.Track(time, *Graph, &subs);
      testTrk.Track("TotalVolume_mL", time, volumeTrk);
      testTrk.Track("VascularVolume_mL", time, vascularVolume_mL);
      testTrk.Track("MembranePotential_mV", time, Potential);
      testTrk.Track("AlbuminVToI", time, massVascularToInterstitial_ug);
      testTrk.Track("AlbuminLToV", time, massInterstitialToLymph_ug);

      time += dt;
    }

    Info("Made it this far");
    circuitTrk.WriteTrackToFile(circuitFile.c_str());
    testTrk.WriteTrackToFile(testFile.c_str());
    break;
  case Combined:
    for (int i = 0; i < stabilizationTime * 60 / dt; i++) {
      calc.Process(*EVIcircuit, dt);
      calc.PostProcess(*EVIcircuit);
      volumeTrk = 0.0;
      for (auto comp : combinedComps) {
        volumeTrk += comp->GetVolume(VolumeUnit::mL);
      }
      circuitTrk.Track(time, *EVIcircuit);
      circuitTrk.Track("TotalVolume_mL", time, volumeTrk);
      time += dt;
    }
    Info("Finished stabilization");
    time = 0.0;
    dt = 0.2;

    vascularVolumeBaseline_mL = 0.0; //Reset this since compliances might have caused them to shift
    muscleExtracellularVolume_mL = cExtraCell.GetVolume(VolumeUnit::mL); //Get stabilized volume to use for lymph relationship
    for (auto comp : Comps) {
      if ((comp->GetName() == "MuscleExtracellularCompartment") || (comp->GetName() == "MuscleLymphCompartment"))
        continue;
      vascularVolumeBaseline_mL += comp->GetVolume(VolumeUnit::mL);
    }
    //Set albumin concentrations first
    cHeart.GetSubstanceQuantity(Alb)->GetConcentration().SetValue(Alb_Vasc_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
    cVeins.GetSubstanceQuantity(Alb)->GetConcentration().SetValue(Alb_Vasc_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
    cMuscle.GetSubstanceQuantity(Alb)->GetConcentration().SetValue(Alb_Vasc_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
    cExtraCell.GetSubstanceQuantity(Alb)->GetConcentration().SetValue(Alb_Extra_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
    cLymph.GetSubstanceQuantity(Alb)->GetConcentration().SetValue(Alb_Extra_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
    for (auto comp : combinedComps) {
      if (comp->GetName() == "MuscleIntracellularCompartment") {
        //Don't set albumin in intracellular space
        comp->GetSubstanceQuantity(Na)->GetMolarity().SetValue(Na_Intra / 1000.0, AmountPerVolumeUnit::mol_Per_L);
        comp->GetSubstanceQuantity(K)->GetMolarity().SetValue(K_Intra / 1000.0, AmountPerVolumeUnit::mol_Per_L);
        comp->GetSubstanceQuantity(Cl)->GetMolarity().SetValue(Cl_Intra / 1000.0, AmountPerVolumeUnit::mol_Per_L);
        comp->Balance(BalanceLiquidBy::Molarity);
      } else {
        comp->Balance(BalanceLiquidBy::Concentration); //Balance the albumins, do it here to reduce copy/paste
        comp->GetSubstanceQuantity(Na)->GetMolarity().SetValue(Na_Vasc / 1000.0, AmountPerVolumeUnit::mol_Per_L);
        comp->GetSubstanceQuantity(K)->GetMolarity().SetValue(K_Vasc / 1000.0, AmountPerVolumeUnit::mol_Per_L);
        comp->GetSubstanceQuantity(Cl)->GetMolarity().SetValue(Cl_Vasc / 1000.0, AmountPerVolumeUnit::mol_Per_L);
        comp->Balance(BalanceLiquidBy::Molarity); //Now balance the ions
      }
    }

    for (int j = 0; j < simTime * 60 / dt; j++) {
      //Handle the intracellular to extracellular water and ion transport
      //tsu.CoupledIonTransport(cTissue, cMuscle, cExtraCell, cIntraCell, pCellMembrane, dt, &Na, &K, &Cl);
      Potential = cTissue.GetMembranePotential(ElectricPotentialUnit::mV);

      //Need to move the albumin manually--by definition a positive value is from capillary to interstitial
      //Don't worry about "DistributeMass" functions because we don't have any child compartments to worry about here

      cMuscle.GetSubstanceQuantity(Alb)->GetMass().IncrementValue(-massVascularToInterstitial_ug, MassUnit::ug);
      cExtraCell.GetSubstanceQuantity(Alb)->GetMass().IncrementValue(massVascularToInterstitial_ug, MassUnit::ug);
      cMuscle.GetSubstanceQuantity(Alb)->Balance(BalanceLiquidBy::Mass);
      cExtraCell.GetSubstanceQuantity(Alb)->Balance(BalanceLiquidBy::Mass);

      //Calculate next osmotic pressure gradient
      Alb_Vasc_g_Per_dL = cMuscle.GetSubstanceQuantity(Alb)->GetConcentration(MassPerVolumeUnit::g_Per_dL);
      Alb_Extra_g_Per_dL = cExtraCell.GetSubstanceQuantity(Alb)->GetConcentration(MassPerVolumeUnit::g_Per_dL);
      capillaryCOP_mmHg = 2.8 * Alb_Vasc_g_Per_dL + 0.18 * std::pow(Alb_Vasc_g_Per_dL, 2) + 0.012 * std::pow(Alb_Vasc_g_Per_dL, 3); //Mazzoni1988Dynamic
      interstitialCOP_mmHg = 1.5 * Alb_Extra_g_Per_dL; //Mazzoni1988Dynamic
      pPlasmaCOP.GetNextPressureSource().SetValue(capillaryCOP_mmHg, PressureUnit::mmHg);
      pInterstitialCOP.GetNextPressureSource().SetValue(interstitialCOP_mmHg, PressureUnit::mmHg);

      //Adjust lymph flow
      //pEx3ToLymph.GetNextPressureSource().SetValue(nVeins.GetPressure(PressureUnit::mmHg) - veinsPressure_mmHg, PressureUnit::mmHg);
      lymphFlowBaseline_mL_Per_s = pEx3ToLymph.GetFlowSourceBaseline(VolumePerTimeUnit::mL_Per_s);
      lymphFlow_mL_Per_s = lymphFlowBaseline_mL_Per_s + (0.0581 / 60.0) * (nEx3.GetPressure(PressureUnit::mmHg) - e3NodePressure) * MuscleTissue.GetTotalMass(MassUnit::kg);
      BLIM(lymphFlow_mL_Per_s, 0.5 * lymphFlowBaseline_mL_Per_s, 22.0 * lymphFlowBaseline_mL_Per_s); //UB comes from Mazzoni and Himeno
      pEx3ToLymph.GetNextFlowSource().SetValue(lymphFlow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

      lymphTest = lymphFlowBaseline_mL_Per_s + (0.0581 / 60.0) * MuscleTissue.GetTotalMass(MassUnit::kg) * ((cExtraCell.GetVolume(VolumeUnit::mL) - nEx3.GetVolumeBaseline(VolumeUnit::mL)) / pEx3ToGround.GetComplianceBaseline(FlowComplianceUnit::mL_Per_mmHg));

      //Update volumes and overall driving pressure
      vascularVolume_mL = 0.0;
      volumeTrk = 0.0;
      for (auto comp : combinedComps) {
        volumeTrk += comp->GetVolume(VolumeUnit::mL);
        if ((comp->GetName() == "MuscleExtracellularCompartment") || (comp->GetName() == "MuscleLymphCompartment") || (comp->GetName() == "MuscleIntracellularCompartment"))
          continue;
        vascularVolume_mL += comp->GetVolume(VolumeUnit::mL);
      }

      aortaPressure_mmHg = 90.0 + (1 / pumpCompliance_mL_Per_mmHg) * (vascularVolume_mL - vascularVolumeBaseline_mL);
      pHeartPump.GetNextPressureSource().SetValue(aortaPressure_mmHg, PressureUnit::mmHg);

      //Apply infusion
      //if ((time>=startInfuse*60)&&(time < endInfuse*60.0))
      //{
      //  pHeartInToGround.GetNextFlowSource().SetValue(-ivRate_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
      //  if (testSaline)
      //  {
      //    cHeart.GetSubstanceQuantity(Na)->GetMass().IncrementValue(ivRate_mL_Per_s*salineNa_mg_Per_mL*dt, MassUnit::mg);
      //    cHeart.GetSubstanceQuantity(Cl)->GetMass().IncrementValue(ivRate_mL_Per_s*salineCl_mg_Per_mL*dt, MassUnit::mg);
      //    cHeart.Balance(BalanceLiquidBy::Mass);
      //  }
      //  else
      //  {
      //    //Must be testing albumin
      //    cHeart.GetSubstanceQuantity(Alb)->GetMass().IncrementValue(ivRate_mL_Per_s*ivAlbumin_mg_Per_mL*dt, MassUnit::mg);
      //    cHeart.GetSubstanceQuantity(Alb)->Balance(BalanceLiquidBy::Mass);
      //  }
      //}

      calc.Process(*EVIcircuit, dt);
      nHeartIn.GetNextPressure().SetValue(0.0, PressureUnit::mmHg);
      txpt.Transport(*CombinedGraph, dt);
      calc.PostProcess(*EVIcircuit);

      testTrk.Track(time, *EVIcircuit);
      testTrk.Track(time, *CombinedGraph, &subs);
      testTrk.Track("TotalVolume_mL", time, volumeTrk);
      testTrk.Track("VascularVolume_mL", time, vascularVolume_mL);
      testTrk.Track("MembranePotential_mV", time, Potential);
      testTrk.Track("LymphTest", time, lymphTest);
      time += dt;
    }

    Info("Made it this far");
    circuitTrk.WriteTrackToFile(circuitFile.c_str());
    testTrk.WriteTrackToFile(testFile.c_str());
    break;
  }
}
}
