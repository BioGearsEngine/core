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
#include <biogears/engine/test/BioGearsEngineTest.h>

#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarInverseVolume.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/utils/testing/SETestCase.h>
#include <biogears/cdm/utils/testing/SETestReport.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>

namespace biogears {
void BioGearsEngineTest::AerosolTest(const std::string& sOutputDirectory)
{
  m_Logger->ResetLogFile(sOutputDirectory + "/AerosolTest.log");

  SETestReport testReport = SETestReport(m_Logger);

  //// Create a suite
  //SETestSuite& albuteroluite = testReport.CreateTestSuite();
  //albuteroluite.SetName("Albuterol");
  //SESubstanceManager subMgr(m_Logger);
  //subMgr.LoadSubstanceDirectory();
  //SESubstance* albuterol = subMgr.GetSubstance("Albuterol");
  //SizeIndependentDepositionEfficencyCoefficientsTest(albuteroluite, *albuterol, 0.043737, 0.045737, 0.090432, 0.3115);
  //DepositionFractionTest(albuteroluite, *albuterol, 0.0805701, 0.0843696, 0.0701852, 0.111879);

  // Create a suite
  SETestSuite& normalDistributionSuite = testReport.CreateTestSuite();
  normalDistributionSuite.SetName("NormalDistribution");
  SESubstance normalDistributedSubstance(m_Logger);
  normalDistributedSubstance.GetAerosolization().GetBronchioleModifier().SetValue(0); //Need something here...
  normalDistributedSubstance.GetAerosolization().GetInflammationCoefficient().SetValue(0.5); //Need something here...
  SEHistogramFractionVsLength& concentrations = normalDistributedSubstance.GetAerosolization().GetParticulateSizeDistribution();
  // Boundary -1 is 0, i.e. no diameter
  concentrations.GetLength().push_back(1.0e-4); // Boundary 0
  concentrations.GetLength().push_back(1.0e-3); // Boundary 1
  concentrations.GetLength().push_back(1.0e-2); // Boundary 2
  concentrations.GetLength().push_back(1.0e-1); // Boundary 3
  concentrations.GetLength().push_back(1.0); // Boundary 4
  concentrations.GetLength().push_back(1.0e1); // Boundary 5
  concentrations.GetLength().push_back(1.0e2); // Boundary 6
    // Boundary n is infinity
  concentrations.SetLengthUnit(LengthUnit::um);

  // Bin -1 contains all of the particles that are less than 1.0e-4 um aerodynamic diameter
  concentrations.GetFraction().push_back(0.015); // Bin 0 (Bounded by Boundary 0 and Boundary 1)
  concentrations.GetFraction().push_back(0.035); // Bin 1 (Bounded by Boundary 1 and Boundary 2)
  concentrations.GetFraction().push_back(0.9); // Bin 2 (Bounded by Boundary 2 and Boundary 3)
  concentrations.GetFraction().push_back(0.035); // Bin 3 (Bounded by Boundary 3 and Boundary 4)
  concentrations.GetFraction().push_back(0.015); // Bin 4 (Bounded by Boundary 4 and Boundary 5)
  concentrations.GetFraction().push_back(0); // Bin 5 (Bounded by Boundary 5 and Boundary 6)
    // Bin n contains

  SizeIndependentDepositionEfficencyCoefficientsTest(normalDistributionSuite, normalDistributedSubstance, 0.043737, 0.045737, 0.090432, 0.3115);
  DepositionFractionTest(normalDistributionSuite, normalDistributedSubstance, 0.0805701, 0.0843696, 0.0701852, 0.111879);

  // Create another suite...
  // Create a suite
  SETestSuite& monodispersedSuite = testReport.CreateTestSuite();
  monodispersedSuite.SetName("Monodispersed");
  SESubstance monodispersedSubstance(m_Logger);
  monodispersedSubstance.GetAerosolization().GetBronchioleModifier().SetValue(0); //Need something here...
  monodispersedSubstance.GetAerosolization().GetInflammationCoefficient().SetValue(0.5); //Need something here...
  SEHistogramFractionVsLength& monoConcentrations = monodispersedSubstance.GetAerosolization().GetParticulateSizeDistribution();
  // Boundary -1 is 0, i.e. no diameter
  monoConcentrations.GetLength().push_back(1.0e-4); // Boundary 0
  monoConcentrations.GetLength().push_back(1.0e-3); // Boundary 1
  monoConcentrations.GetLength().push_back(1.0e-2); // Boundary 2
  monoConcentrations.GetLength().push_back(1.0e-1); // Boundary 3
  monoConcentrations.GetLength().push_back(1.0); // Boundary 4
  monoConcentrations.GetLength().push_back(1.0e1); // Boundary 5
  monoConcentrations.GetLength().push_back(1.0e2); // Boundary 6
    // Boundary n is infinity
  monoConcentrations.SetLengthUnit(LengthUnit::um);

  monoConcentrations.GetFraction().push_back(0); // Bin 0 (Bounded by Boundary 0 and Boundary 1)
  monoConcentrations.GetFraction().push_back(0); // Bin 1 (Bounded by Boundary 1 and Boundary 2)
  monoConcentrations.GetFraction().push_back(1.0); // Bin 2 (Bounded by Boundary 2 and Boundary 3)
  monoConcentrations.GetFraction().push_back(0); // Bin 3 (Bounded by Boundary 3 and Boundary 4)
  monoConcentrations.GetFraction().push_back(0); // Bin 4 (Bounded by Boundary 4 and Boundary 5)
  monoConcentrations.GetFraction().push_back(0); // Bin 5 (Bounded by Boundary 5 and Boundary 6)

  SizeIndependentDepositionEfficencyCoefficientsTest(monodispersedSuite, monodispersedSubstance, 0.024864, 0.025498, 0.083839, 0.32856);
  DepositionFractionTest(monodispersedSuite, monodispersedSubstance, 0.0471459, 0.0482577, 0.0660969, 0.117988);

  // Create a suite
  SETestSuite& mono2Suite = testReport.CreateTestSuite();
  mono2Suite.SetName("Monodispersed_2");
  SESubstance mono2Substance(m_Logger);
  mono2Substance.GetAerosolization().GetBronchioleModifier().SetValue(0); //Need something here...
  mono2Substance.GetAerosolization().GetInflammationCoefficient().SetValue(0.5); //Need something here...
  SEHistogramFractionVsLength& mono2Concentrations = mono2Substance.GetAerosolization().GetParticulateSizeDistribution();
  // Boundary -1 is 0, i.e. no diameter
  mono2Concentrations.GetLength().push_back(1.0e-4); // Boundary 0
  mono2Concentrations.GetLength().push_back(1.0e-3); // Boundary 1
  mono2Concentrations.GetLength().push_back(1.0e-2); // Boundary 2
  mono2Concentrations.GetLength().push_back(1.0e-1); // Boundary 3
  mono2Concentrations.GetLength().push_back(1.0); // Boundary 4
  mono2Concentrations.GetLength().push_back(1.0e1); // Boundary 5
  mono2Concentrations.GetLength().push_back(1.0e2); // Boundary 6
    // Boundary n is infinity
  mono2Concentrations.SetLengthUnit(LengthUnit::um);

  mono2Concentrations.GetFraction().push_back(0); // Bin 0 (Bounded by Boundary 0 and Boundary 1)
  mono2Concentrations.GetFraction().push_back(0); // Bin 1 (Bounded by Boundary 1 and Boundary 2)
  mono2Concentrations.GetFraction().push_back(0); // Bin 2 (Bounded by Boundary 2 and Boundary 3)
  mono2Concentrations.GetFraction().push_back(0); // Bin 3 (Bounded by Boundary 3 and Boundary 4)
  mono2Concentrations.GetFraction().push_back(0); // Bin 4 (Bounded by Boundary 4 and Boundary 5)
  mono2Concentrations.GetFraction().push_back(1.0); // Bin 5 (Bounded by Boundary 5 and Boundary 6)

  SizeIndependentDepositionEfficencyCoefficientsTest(mono2Suite, mono2Substance, 0.26841, 0.36689, 0.0012658, 0.0014995);
  DepositionFractionTest(mono2Suite, mono2Substance, 0.366641, 0.504666, 0.000931153, 0.000542435);

  // Create a suite
  SETestSuite& mono3Suite = testReport.CreateTestSuite();
  mono3Suite.SetName("Monodispersed_3");
  SESubstance mono3Substance(m_Logger);
  mono3Substance.GetAerosolization().GetBronchioleModifier().SetValue(0); //Need something here...
  mono3Substance.GetAerosolization().GetInflammationCoefficient().SetValue(0.5); //Need something here...
  SEHistogramFractionVsLength& mono3Concentrations = mono3Substance.GetAerosolization().GetParticulateSizeDistribution();
  // Boundary -1 is 0, i.e. no diameter
  mono3Concentrations.GetLength().push_back(1.0e-4); // Boundary 0
  mono3Concentrations.GetLength().push_back(1.0e-3); // Boundary 1
  mono3Concentrations.GetLength().push_back(1.0e-2); // Boundary 2
  mono3Concentrations.GetLength().push_back(1.0e-1); // Boundary 3
  mono3Concentrations.GetLength().push_back(1.0); // Boundary 4
  mono3Concentrations.GetLength().push_back(1.0e1); // Boundary 5
  mono3Concentrations.GetLength().push_back(1.0e2); // Boundary 6
    // Boundary n is infinity
  mono3Concentrations.SetLengthUnit(LengthUnit::um);

  mono3Concentrations.GetFraction().push_back(1.0); // Bin 0 (Bounded by Boundary 0 and Boundary 1)
  mono3Concentrations.GetFraction().push_back(0); // Bin 1 (Bounded by Boundary 1 and Boundary 2)
  mono3Concentrations.GetFraction().push_back(0); // Bin 2 (Bounded by Boundary 2 and Boundary 3)
  mono3Concentrations.GetFraction().push_back(0); // Bin 3 (Bounded by Boundary 3 and Boundary 4)
  mono3Concentrations.GetFraction().push_back(0); // Bin 4 (Bounded by Boundary 4 and Boundary 5)
  mono3Concentrations.GetFraction().push_back(0); // Bin 5 (Bounded by Boundary 5 and Boundary 6)

  SizeIndependentDepositionEfficencyCoefficientsTest(mono3Suite, mono3Substance, 0.43617, 0.77358, 0.21788, 0.005489);
  DepositionFractionTest(mono3Suite, mono3Substance, 0.468578, 0.753432, 0.1188, 0.00197536);

  // Create a suite
  SETestSuite& zhangDispersion = testReport.CreateTestSuite();
  zhangDispersion.SetName("ZhangDispersion");
  SESubstance zhangSubstance(m_Logger);
  zhangSubstance.GetAerosolization().GetBronchioleModifier().SetValue(0); //Need something here...
  zhangSubstance.GetAerosolization().GetInflammationCoefficient().SetValue(0.5); //Need something here...
  SEHistogramFractionVsLength& zhangConcentrations = zhangSubstance.GetAerosolization().GetParticulateSizeDistribution();
  // This histogram based on a digitization of Figure 4b in @cite zhang2012chemical
  zhangConcentrations.GetLength().push_back(10.308639); // Boundary 0
  zhangConcentrations.GetLength().push_back(16.263765); // Boundary 1
  zhangConcentrations.GetLength().push_back(25.854805); // Boundary 2
  zhangConcentrations.GetLength().push_back(41.101852); // Boundary 3
  zhangConcentrations.GetLength().push_back(65.83879); // Boundary 4
  zhangConcentrations.GetLength().push_back(103.87276); // Boundary 5
  zhangConcentrations.GetLength().push_back(165.1284); // Boundary 6
  zhangConcentrations.GetLength().push_back(260.5203); // Boundary 7
  zhangConcentrations.GetLength().push_back(417.31314); // Boundary 8
  zhangConcentrations.GetLength().push_back(663.4103); // Boundary 9
  zhangConcentrations.GetLength().push_back(1062.6803); // Boundary 10
    // Boundary n is infinity
  zhangConcentrations.SetLengthUnit(LengthUnit::um);

  zhangConcentrations.GetFraction().push_back(0.0028992); // Bin 0 (Bounded by Boundary 0 and Boundary 1)
  zhangConcentrations.GetFraction().push_back(0.018143616); // Bin 1 (Bounded by Boundary 1 and Boundary 2)
  zhangConcentrations.GetFraction().push_back(0.069563083); // Bin 2 (Bounded by Boundary 2 and Boundary 3)
  zhangConcentrations.GetFraction().push_back(0.170665942); // Bin 3 (Bounded by Boundary 3 and Boundary 4)
  zhangConcentrations.GetFraction().push_back(0.251295383); // Bin 4 (Bounded by Boundary 4 and Boundary 5)
  zhangConcentrations.GetFraction().push_back(0.245390533); // Bin 5 (Bounded by Boundary 5 and Boundary 6)
  zhangConcentrations.GetFraction().push_back(0.145956494); // Bin 6 (Bounded by Boundary 6 and Boundary 7)
  zhangConcentrations.GetFraction().push_back(0.058885618); // Bin 7 (Bounded by Boundary 7 and Boundary 8)
  zhangConcentrations.GetFraction().push_back(0.020538896); // Bin 8 (Bounded by Boundary 8 and Boundary 9)
  zhangConcentrations.GetFraction().push_back(0.016661235); // Bin 9 (Bounded by Boundary 9 and Boundary 10)

  SizeIndependentDepositionEfficencyCoefficientsTest(zhangDispersion, zhangSubstance, 0.25368, 0.3399, 0.00013825, 0.00022882);
  DepositionFractionTest(zhangDispersion, zhangSubstance, 0.353503, 0.478626, 0.000102784, 8.27909e-05);

  testReport.WriteFile(sOutputDirectory + "/AerosolTestReport.xml");
}

void BioGearsEngineTest::SizeIndependentDepositionEfficencyCoefficientsTest(SETestSuite& suite, SESubstance& substance,
  double expectedMouthCoeff, double expectedTracheaCoeff, double expectedDeadSpaceCoeff, double expectedAlveoliCoeff)
{
  double PercentTolerance = 0.1;
  // Set up our test report
  TimingProfile pTimer;
  pTimer.Start("Test");
  SETestCase& tc1 = suite.CreateTestCase();
  tc1.SetName(std::string{ substance.GetName() } +"SIDECo");

  BioGears bg(m_Logger);
  const SizeIndependentDepositionEfficencyCoefficient& SIDECoeff = bg.GetSubstances().GetSizeIndependentDepositionEfficencyCoefficient(substance);
  m_ss << "Mouth: " << SIDECoeff.GetMouth();
  Info(m_ss);
  if (GeneralMath::PercentTolerance(SIDECoeff.GetMouth(), expectedMouthCoeff) > PercentTolerance) {
    m_ss << " Trachea SIDE Coefficient is not correct : " << SIDECoeff.GetMouth() << " expected " << expectedMouthCoeff;
    tc1.AddFailure(m_ss);
  }
  m_ss << "Trachea: " << SIDECoeff.GetTrachea();
  Info(m_ss);
  if (GeneralMath::PercentTolerance(SIDECoeff.GetTrachea(), expectedTracheaCoeff) > PercentTolerance) {
    m_ss << " Deadspace SIDE Coefficient is not correct : " << SIDECoeff.GetTrachea() << " expected " << expectedTracheaCoeff;
    tc1.AddFailure(m_ss);
  }
  m_ss << "Deadspace: " << SIDECoeff.GetBronchi();
  Info(m_ss);
  if (GeneralMath::PercentTolerance(SIDECoeff.GetBronchi(), expectedDeadSpaceCoeff) > PercentTolerance) {
    m_ss << " Mouth SIDE Coefficient is not correct : " << SIDECoeff.GetBronchi() << " expected " << expectedDeadSpaceCoeff;
    tc1.AddFailure(m_ss);
  }
  m_ss << "Alveoli: " << SIDECoeff.GetAlveoli();
  Info(m_ss);
  if (GeneralMath::PercentTolerance(SIDECoeff.GetAlveoli(), expectedAlveoliCoeff) > PercentTolerance) {
    m_ss << " Alveoli SIDE Coefficient is not correct : " << SIDECoeff.GetAlveoli() << " expected " << expectedAlveoliCoeff;
    tc1.AddFailure(m_ss);
  }
  tc1.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void BioGearsEngineTest::DepositionFractionTest(SETestSuite& suite, SESubstance& substance,
  double expectedMouthDepFrac, double expectedTracheaDepFrac, double expectedDeadSpaceDepFrac, double expectedAlveoliDepFrac)
{
  TimingProfile pTimer;
  pTimer.Start("Test");
  double PercentTolerance = 0.1;
  SETestCase& tc = suite.CreateTestCase();
  tc.SetName(std::string{substance.GetName()} + "DepositionFraction");

  BioGears bg(m_Logger);
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();
  bg.GetSubstances().InitializeGasCompartments();
  SEEnvironmentalConditions& env = bg.GetEnvironment().GetConditions();
  SELiquidCompartment* ambient = bg.GetCompartments().GetLiquidCompartment(BGE::EnvironmentCompartment::Ambient);

  SEFluidCircuit* rCircuit = &bg.GetCircuits().GetRespiratoryCircuit();
  SEGasCompartmentGraph* rGraph = &bg.GetCompartments().GetRespiratoryGraph();
  SELiquidCompartmentGraph* aGraph = &bg.GetCompartments().GetAerosolGraph();

  const SizeIndependentDepositionEfficencyCoefficient& SIDECoeff = bg.GetSubstances().GetSizeIndependentDepositionEfficencyCoefficient(substance);

  SEFluidCircuitPath* driverPath = rCircuit->GetPath(BGE::RespiratoryPath::EnvironmentToRespiratoryMuscle);
  SEGasTransporter gtxpt(VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, NoUnit::unitless, bg.GetLogger());
  SELiquidTransporter ltxpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, bg.GetLogger());
  SEFluidCircuitCalculator calc(FlowComplianceUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s, FlowInertanceUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O, VolumeUnit::L, FlowResistanceUnit::cmH2O_s_Per_L, bg.GetLogger());

  //Set the reference not pressure to the standard environment
  //This is needed because we're not setting the Environment during initialization in this unit test
  rCircuit->GetNode(BGE::EnvironmentNode::Ambient)->GetNextPressure().Set(env.GetAtmosphericPressure());
  rCircuit->GetNode(BGE::EnvironmentNode::Ambient)->GetPressure().Set(env.GetAtmosphericPressure());
  //Precharge the stomach to prevent negative volume
  rCircuit->GetNode(BGE::RespiratoryNode::Stomach)->GetNextPressure().Set(env.GetAtmosphericPressure());
  rCircuit->GetNode(BGE::RespiratoryNode::Stomach)->GetPressure().Set(env.GetAtmosphericPressure());

  SEFluidCircuitPath* Env2Mouth = rCircuit->GetPath(BGE::RespiratoryPath::EnvironmentToMouth);

  SELiquidSubstanceQuantity* mouthParticulate = nullptr;
  SELiquidSubstanceQuantity* TracheaParticulate = nullptr;
  SELiquidSubstanceQuantity* leftDeadSpaceParticulate = nullptr;
  SELiquidSubstanceQuantity* leftAlveoliParticulate = nullptr;
  SELiquidSubstanceQuantity* rightDeadSpaceParticulate = nullptr;
  SELiquidSubstanceQuantity* rightAlveoliParticulate = nullptr;

  //Circuit Analysis Test --------------------------------------------------
  //Execution parameters
  double time = 0;
  double deltaT_s = 1.0 / 50.0;

  //Drive waveform parameters
  double period = 5.0;
  double runTime_s = period * 2;
  double alpha = (2 * 3.1415) / (period);
  double driverPressure_cmH2O = 0.0;
  double amplitude_cmH2O = 6.0;
  double yOffset = -12.0;
  double sinusoid = 0;

  DataTrack trk;
  std::ofstream file;
  double totalInspiredAir_mL = 0;
  double totalExspiredAir_mL = 0;
  double totalInspiredParticulate_ug = 0;

  double totalParticulateDeposited_ug = 0;
  double depositedMouthParticulate_ug = 0;
  double depositedTracheaParticulate_ug = 0;
  double depositedLeftDeadSpaceParticulate_ug = 0;
  double depositedLeftAlveoliParticulate_ug = 0;
  double depositedRightDeadSpaceParticulate_ug = 0;
  double depositedRightAlveoliParticulate_ug = 0;

  for (unsigned int i = 0; i < runTime_s / deltaT_s; i++) {
    sinusoid = cos(alpha * time);
    //PreProcess - Push driver pressure and  variable compliance data into the Circuit
    driverPressure_cmH2O = yOffset + amplitude_cmH2O * sinusoid; //compute new pressure
    driverPath->GetNextPressureSource().SetValue(driverPressure_cmH2O, PressureUnit::cmH2O);

    //Process - Execute the circuit
    calc.Process(*rCircuit, deltaT_s);
    //Execute the substance transport function
    gtxpt.Transport(*rGraph, deltaT_s);
    // Do it again for aerosols
    if (aGraph != nullptr)
      ltxpt.Transport(*aGraph, deltaT_s);
    //convert 'Next' values to current
    calc.PostProcess(*rCircuit);

    trk.Track("DriverPressure_cmH2O", time, driverPressure_cmH2O);
    trk.Track("Env2Mouth_mL/s", time, Env2Mouth->GetFlow(VolumePerTimeUnit::mL_Per_s));

    trk.Track("TotalMouthPreParticulate_ug", time, mouthParticulate == nullptr ? 0 : mouthParticulate->GetMass(MassUnit::ug));
    trk.Track("MouthAerosol_mL", time, aGraph->GetCompartment(BGE::PulmonaryCompartment::Mouth)->GetVolume(VolumeUnit::mL));
    trk.Track("MouthAir_mL", time, rGraph->GetCompartment(BGE::PulmonaryCompartment::Mouth)->GetVolume(VolumeUnit::mL));

    if (i > runTime_s * 0.5 / deltaT_s) { // Completed a cycle now start tracking Deposition and total inspired Air

      if (mouthParticulate == nullptr) {
        double aerosolConcentration_g_Per_m3 = 2.5;
        bg.GetSubstances().AddActiveSubstance(substance);
        ambient->GetSubstanceQuantity(substance)->GetConcentration().SetValue(aerosolConcentration_g_Per_m3, MassPerVolumeUnit::g_Per_m3);
        ambient->Balance(BalanceLiquidBy::Concentration);

        mouthParticulate = aGraph->GetCompartment(BGE::PulmonaryCompartment::Mouth)->GetSubstanceQuantity(substance);
        TracheaParticulate = aGraph->GetCompartment(BGE::PulmonaryCompartment::Trachea)->GetSubstanceQuantity(substance);
        leftDeadSpaceParticulate = aGraph->GetCompartment(BGE::PulmonaryCompartment::LeftBronchi)->GetSubstanceQuantity(substance);
        leftAlveoliParticulate = aGraph->GetCompartment(BGE::PulmonaryCompartment::LeftAlveoli)->GetSubstanceQuantity(substance);
        rightDeadSpaceParticulate = aGraph->GetCompartment(BGE::PulmonaryCompartment::RightBronchi)->GetSubstanceQuantity(substance);
        rightAlveoliParticulate = aGraph->GetCompartment(BGE::PulmonaryCompartment::RightAlveoli)->GetSubstanceQuantity(substance);

        // Initialize to the env concentrations
        // aerosolConcentration_g_Per_m3 = 0.; // Or initialize to zero if you want.
        mouthParticulate->GetConcentration().SetValue(aerosolConcentration_g_Per_m3, MassPerVolumeUnit::g_Per_m3);
        mouthParticulate->Balance(BalanceLiquidBy::Concentration);
        TracheaParticulate->GetConcentration().SetValue(aerosolConcentration_g_Per_m3, MassPerVolumeUnit::g_Per_m3);
        TracheaParticulate->Balance(BalanceLiquidBy::Concentration);
        leftDeadSpaceParticulate->GetConcentration().SetValue(aerosolConcentration_g_Per_m3, MassPerVolumeUnit::g_Per_m3);
        leftDeadSpaceParticulate->Balance(BalanceLiquidBy::Concentration);
        leftAlveoliParticulate->GetConcentration().SetValue(aerosolConcentration_g_Per_m3, MassPerVolumeUnit::g_Per_m3);
        leftAlveoliParticulate->Balance(BalanceLiquidBy::Concentration);
        rightDeadSpaceParticulate->GetConcentration().SetValue(aerosolConcentration_g_Per_m3, MassPerVolumeUnit::g_Per_m3);
        rightDeadSpaceParticulate->Balance(BalanceLiquidBy::Concentration);
        rightAlveoliParticulate->GetConcentration().SetValue(aerosolConcentration_g_Per_m3, MassPerVolumeUnit::g_Per_m3);
        rightAlveoliParticulate->Balance(BalanceLiquidBy::Concentration);
      }

      if (Env2Mouth->GetFlow().IsPositive()) {
        // Grab instantaneous
        double inspiredAir_mL = Env2Mouth->GetFlow(VolumePerTimeUnit::mL_Per_s) * deltaT_s;
        double inspiredParticulate_ug = Env2Mouth->GetFlow(VolumePerTimeUnit::mL_Per_s) * ambient->GetSubstanceQuantity(substance)->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * deltaT_s;
        // Totals from instantaneous
        totalInspiredAir_mL += inspiredAir_mL;
        totalInspiredParticulate_ug += inspiredParticulate_ug;
      } else {
        double exspiredAir_mL = Env2Mouth->GetFlow(VolumePerTimeUnit::mL_Per_s) * deltaT_s;
        totalExspiredAir_mL += exspiredAir_mL;
      }
      if (true) {
        // Calculate the deposited mass by concentration and flow
        depositedMouthParticulate_ug = mouthParticulate->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * aGraph->GetCompartment(BGE::PulmonaryCompartment::Mouth)->GetInFlow(VolumePerTimeUnit::mL_Per_s) * deltaT_s * SIDECoeff.GetMouth();
        depositedTracheaParticulate_ug = TracheaParticulate->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * aGraph->GetCompartment(BGE::PulmonaryCompartment::Trachea)->GetInFlow(VolumePerTimeUnit::mL_Per_s) * deltaT_s * SIDECoeff.GetTrachea();
        depositedLeftDeadSpaceParticulate_ug = leftDeadSpaceParticulate->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * aGraph->GetCompartment(BGE::PulmonaryCompartment::LeftBronchi)->GetInFlow(VolumePerTimeUnit::mL_Per_s) * deltaT_s * SIDECoeff.GetBronchi();
        depositedLeftAlveoliParticulate_ug = leftAlveoliParticulate->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * aGraph->GetCompartment(BGE::PulmonaryCompartment::LeftAlveoli)->GetInFlow(VolumePerTimeUnit::mL_Per_s) * deltaT_s * SIDECoeff.GetAlveoli();
        depositedRightDeadSpaceParticulate_ug = rightDeadSpaceParticulate->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * aGraph->GetCompartment(BGE::PulmonaryCompartment::RightBronchi)->GetInFlow(VolumePerTimeUnit::mL_Per_s) * deltaT_s * SIDECoeff.GetBronchi();
        depositedRightAlveoliParticulate_ug = rightAlveoliParticulate->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * aGraph->GetCompartment(BGE::PulmonaryCompartment::RightAlveoli)->GetInFlow(VolumePerTimeUnit::mL_Per_s) * deltaT_s * SIDECoeff.GetAlveoli();

        // Total deposited
        totalParticulateDeposited_ug += depositedMouthParticulate_ug + depositedTracheaParticulate_ug + depositedLeftDeadSpaceParticulate_ug + depositedLeftAlveoliParticulate_ug + depositedRightDeadSpaceParticulate_ug + depositedRightAlveoliParticulate_ug;

        // Remove the delta from the compartment substance quantity mass
        mouthParticulate->GetMass().IncrementValue(-depositedMouthParticulate_ug, MassUnit::ug);
        mouthParticulate->Balance(BalanceLiquidBy::Mass);
        TracheaParticulate->GetMass().IncrementValue(-depositedTracheaParticulate_ug, MassUnit::ug);
        TracheaParticulate->Balance(BalanceLiquidBy::Mass);
        leftDeadSpaceParticulate->GetMass().IncrementValue(-depositedLeftDeadSpaceParticulate_ug, MassUnit::ug);
        leftDeadSpaceParticulate->Balance(BalanceLiquidBy::Mass);
        leftAlveoliParticulate->GetMass().IncrementValue(-depositedLeftAlveoliParticulate_ug, MassUnit::ug);
        leftAlveoliParticulate->Balance(BalanceLiquidBy::Mass);
        rightDeadSpaceParticulate->GetMass().IncrementValue(-depositedRightDeadSpaceParticulate_ug, MassUnit::ug);
        rightDeadSpaceParticulate->Balance(BalanceLiquidBy::Mass);
        rightAlveoliParticulate->GetMass().IncrementValue(-depositedRightAlveoliParticulate_ug, MassUnit::ug);
        rightAlveoliParticulate->Balance(BalanceLiquidBy::Mass);
        // Add the delta to the compartment substance quantity mass deposited
        mouthParticulate->GetMassDeposited().IncrementValue(depositedMouthParticulate_ug, MassUnit::ug);
        TracheaParticulate->GetMassDeposited().IncrementValue(depositedTracheaParticulate_ug, MassUnit::ug);
        leftDeadSpaceParticulate->GetMassDeposited().IncrementValue(depositedLeftDeadSpaceParticulate_ug, MassUnit::ug);
        leftAlveoliParticulate->GetMassDeposited().IncrementValue(depositedLeftAlveoliParticulate_ug, MassUnit::ug);
        rightDeadSpaceParticulate->GetMassDeposited().IncrementValue(depositedRightDeadSpaceParticulate_ug, MassUnit::ug);
        rightAlveoliParticulate->GetMassDeposited().IncrementValue(depositedRightAlveoliParticulate_ug, MassUnit::ug);
      }
    }

    trk.Track("MouthConcentration_ug_Per_mL", time, mouthParticulate == nullptr ? 0 : mouthParticulate->GetConcentration(MassPerVolumeUnit::ug_Per_mL));
    trk.Track("TracheaConcentration_ug_Per_mL", time, TracheaParticulate == nullptr ? 0 : TracheaParticulate->GetConcentration(MassPerVolumeUnit::ug_Per_mL));

    trk.Track("MouthInFlow_mL_Per_s", time, aGraph->GetCompartment(BGE::PulmonaryCompartment::Mouth)->GetInFlow(VolumePerTimeUnit::mL_Per_s));
    trk.Track("TracheaInFlow_mL_Per_s", time, aGraph->GetCompartment(BGE::PulmonaryCompartment::Trachea)->GetInFlow(VolumePerTimeUnit::mL_Per_s));

    trk.Track("TotalInspiredAir_mL", time, totalInspiredAir_mL);
    trk.Track("TotalExspiredAir_mL", time, totalExspiredAir_mL);
    trk.Track("TotalInspiredParticulate_ug", time, totalInspiredParticulate_ug);
    trk.Track("TotalDepositedParticulate_ug", time, totalParticulateDeposited_ug);
    trk.Track("TotalMouthPostParticulate_ug", time, mouthParticulate == nullptr ? 0 : mouthParticulate->GetMass(MassUnit::ug));
    trk.Track("MouthParticulateDeposited_ug", time, depositedMouthParticulate_ug);
    trk.Track("TotalMouthParticulateDeposited_ug", time, mouthParticulate == nullptr ? 0 : mouthParticulate->GetMassDeposited(MassUnit::ug));
    trk.Track("TracheaParticulate_ug", time, TracheaParticulate == nullptr ? 0 : TracheaParticulate->GetMass(MassUnit::ug));
    trk.Track("TracheaParticulateDeposited_ug", time, TracheaParticulate == nullptr ? 0 : TracheaParticulate->GetMassDeposited(MassUnit::ug));
    trk.Track("LeftDeadSpaceParticulate_ug", time, leftDeadSpaceParticulate == nullptr ? 0 : leftDeadSpaceParticulate->GetMass(MassUnit::ug));
    trk.Track("LeftDeadSpaceParticulateDeposited_ug", time, leftDeadSpaceParticulate == nullptr ? 0 : leftDeadSpaceParticulate->GetMassDeposited(MassUnit::ug));
    trk.Track("LeftAlveoliParticulate_ug", time, leftAlveoliParticulate == nullptr ? 0 : leftAlveoliParticulate->GetMass(MassUnit::ug));
    trk.Track("LeftAlveoliParticulateDeposited_ug", time, leftAlveoliParticulate == nullptr ? 0 : leftAlveoliParticulate->GetMassDeposited(MassUnit::ug));
    trk.Track("RightDeadSpaceParticulate_ug", time, rightDeadSpaceParticulate == nullptr ? 0 : rightDeadSpaceParticulate->GetMass(MassUnit::ug));
    trk.Track("RightDeadSpaceParticulateDeposited_ug", time, rightDeadSpaceParticulate == nullptr ? 0 : rightDeadSpaceParticulate->GetMassDeposited(MassUnit::ug));
    trk.Track("RightAlveoliParticulate_ug", time, rightAlveoliParticulate == nullptr ? 0 : rightAlveoliParticulate->GetMass(MassUnit::ug));
    trk.Track("RightAlveoliParticulateDeposited_ug", time, rightAlveoliParticulate == nullptr ? 0 : rightAlveoliParticulate->GetMassDeposited(MassUnit::ug));

    if (i == 0)
      trk.CreateFile((std::string{ "./UnitTests/BioGearsTests/" } +substance.GetName() + "DepositionFraction.csv").c_str(), file);
    trk.StreamTrackToFile(file);

    time += deltaT_s;
  }

  // Do all the deltas match?
  double expectedInspiredParticulate_ug = totalInspiredAir_mL * ambient->GetSubstanceQuantity(substance)->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
  if (GeneralMath::PercentTolerance(totalInspiredParticulate_ug, expectedInspiredParticulate_ug) > PercentTolerance) {
    m_ss << " Compartment particulate mass does not expected particulate mass : " << totalInspiredParticulate_ug << " expected " << expectedInspiredParticulate_ug;
    tc.AddFailure(m_ss);
  }

  m_ss << "Total Particulate Inhaled : " << totalInspiredParticulate_ug;
  Info(m_ss);
  m_ss << "Total Particulate Deposited : " << totalParticulateDeposited_ug;
  Info(m_ss);

  m_ss << "Particulate Deposited in Mouth: " << mouthParticulate->GetMassDeposited(MassUnit::ug);
  Info(m_ss);
  if (GeneralMath::PercentTolerance(expectedMouthDepFrac, mouthParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug) > PercentTolerance) {
    m_ss << " Mouth Particle Deposition is not correct : " << mouthParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug << " expected " << expectedMouthDepFrac;
    tc.AddFailure(m_ss);
  }

  m_ss << "Particulate Deposited in Trachea: " << TracheaParticulate->GetMassDeposited(MassUnit::ug);
  Info(m_ss);
  if (GeneralMath::PercentTolerance(expectedTracheaDepFrac, TracheaParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug) > PercentTolerance) {
    m_ss << " Trachea Particle Deposition is not correct : " << TracheaParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug << " expected " << expectedTracheaDepFrac;
    tc.AddFailure(m_ss);
  }

  m_ss << "Particulate Deposited in Left Dead Space : " << leftDeadSpaceParticulate->GetMassDeposited(MassUnit::ug);
  Info(m_ss);
  if (GeneralMath::PercentTolerance(expectedDeadSpaceDepFrac, leftDeadSpaceParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug) > PercentTolerance) {
    m_ss << " Left Dead Space Particle Deposition is not correct : " << leftDeadSpaceParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug << " expected " << expectedDeadSpaceDepFrac;
    tc.AddFailure(m_ss);
  }

  m_ss << "Particulate Deposited in Left Alveoli : " << leftAlveoliParticulate->GetMassDeposited(MassUnit::ug);
  Info(m_ss);
  if (GeneralMath::PercentTolerance(expectedAlveoliDepFrac, leftAlveoliParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug) > PercentTolerance) {
    m_ss << " Left Alveoli Particle Deposition is not correct : " << leftAlveoliParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug << " expected " << expectedAlveoliDepFrac;
    tc.AddFailure(m_ss);
  }

  m_ss << "Particulate Deposited in Right Dead Space : " << rightDeadSpaceParticulate->GetMassDeposited(MassUnit::ug);
  Info(m_ss);
  if (GeneralMath::PercentTolerance(expectedDeadSpaceDepFrac, rightDeadSpaceParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug) > PercentTolerance) {
    m_ss << " Right Dead Space Particle Deposition is not correct : " << rightDeadSpaceParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug << " expected " << expectedDeadSpaceDepFrac;
    tc.AddFailure(m_ss);
  }

  m_ss << "Particulate Deposited in Right Alveoli : " << rightAlveoliParticulate->GetMassDeposited(MassUnit::ug);
  Info(m_ss);
  if (GeneralMath::PercentTolerance(expectedAlveoliDepFrac, rightAlveoliParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug) > PercentTolerance) {
    m_ss << " Right Alveoli Particle Deposition is not correct : " << rightAlveoliParticulate->GetMassDeposited(MassUnit::ug) / totalInspiredParticulate_ug << " expected " << expectedAlveoliDepFrac;
    tc.AddFailure(m_ss);
  }

  tc.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}
}