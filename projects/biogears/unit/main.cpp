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
#include "CommonDataModelTest.h"

#define ENGINE_TESTS
#ifdef ENGINE_TESTS
#include "BioGearsEngineTest.h"
#endif
#include "utils/FileUtils.h"

int main(int argc, char* argv[])
{
  std::string cdmDir = ".\\UnitTests\\CDMTests";
  Logger cdmLogger(cdmDir + "\\CDMTests.log");
  CommonDataModelTest cdmTest(&cdmLogger);

  std::string bgDir = ".\\UnitTests\\BioGearsTests";
  Logger bgeLogger(bgDir + "\\BioGearsTests.log");
  BioGearsEngineTest bgeTest(&bgeLogger);

  if (argc == 1)
  {
    try
    {
      // Easily run various tests of your choosing

      ////////////////////
      // BioGears Tests //
      ////////////////////

      //bgeTest.CardiovascularCircuitAndTransportTest(bgDir);
      //bgeTest.CardiovascularAndRenalCircuitAndTransportTest(bgDir);
      //bgeTest.CardiovascularAndTissueCircuitAndTransportTest(bgDir);
      //bgeTest.CardiovascularTissueAndRenalCircuitAndTransportTest(bgDir);
      //bgeTest.CardiovascularBloodGasesTest(bgDir);
      //bgeTest.TuneCardiovascularCircuitTest(bgDir);

      //bgeTest.RenalCircuitAndTransportTest(bgDir);
      //bgeTest.RenalTGFFeedbackTest(bgDir);
      //bgeTest.RenalTGFandUPRFeedbackTest(bgDir);
      //bgeTest.RenalSecretionTest(bgDir);
      //bgeTest.RenalUrinateTest(bgDir);

      //bgeTest.RespiratoryCircuitAndTransportTest(bgDir);
      //bgeTest.RespiratoryDriverTest(bgDir);
      //bgeTest.AnesthesiaMachineCircuitAndTransportTest(bgDir);
      //bgeTest.RespiratoryWithAnesthesiaMachineCircuitAndTransportTest(bgDir);	
      //bgeTest.RespiratoryWithInhalerCircuitAndTransportTest(bgDir);

      //bgeTest.InternalTemperatureVariableBMRCircuitTest(bgDir);
      //bgeTest.InternalTemperatureVariableSkinCircuitTest(bgDir);
      //bgeTest.InternalTemperatureVariableCoreCircuitTest(bgDir);
      //bgeTest.EnvironmentVariableTemperatureCircuitTest(bgDir);
      //bgeTest.CombinedInternalAndEnvironmentVariableBMRandTemperatureCircuitTest(bgDir);
      //bgeTest.CombinedInternalAndEnvironmentSkinTempDropCircuitTest(bgDir);
      //bgeTest.EnvironmentISO7730ComparisonTest(bgDir);

      //bgeTest.DiffusionClearanceExcretionTests(bgDir);
      //bgeTest.AlveolarOxygenDiffusionTest(bgDir);
      //bgeTest.AlveolarCarbonDioxideDiffusionTest(bgDir);
      //bgeTest.InstantPlusSimpleDiffusionTest(bgDir);
      //bgeTest.SimpleDiffusionTwoCompartmentTest(bgDir);
      //bgeTest.SimpleDiffusionFourCompartmentTest(bgDir);
	  //bgeTest.ActiveTransportTest(bgDir);
      //bgeTest.SimpleDiffusionHierarchyTest(bgDir);
      // bgeTest.FacilitatedDiffusionTest(bgDir);
	  //bgeTest.ActiveIonTransportTest(bgDir);

      //bgeTest.BrainInjuryTest(bgDir);

      //bgeTest.ConditionCombinations(bgDir);
      //bgeTest.MultiEngineTest(bgDir);
      //bgeTest.SerializationTest(bgDir);
      //bgeTest.SolverSpeedTest(bgDir);

      //bgeTest.AcidBaseMathTest(bgDir);
      //bgeTest.AcidBaseFeedbackTest(bgDir);
      //bgeTest.AcidBaseLimitsTest(bgDir);
      //bgeTest.AcidBaseExtremeTest(bgDir);
      //bgeTest.AcidBaseBloodGasTests(bgDir);

      //bgeTest.AerosolTest(bgDir);

      //bgeTest.DigestionCarbs(bgDir);
      //bgeTest.DigestionProtein(bgDir);
      //bgeTest.DigestionFat(bgDir);
      //bgeTest.DigestionMixed(bgDir);
      //bgeTest.AbsorptionCarbs(bgDir);
      //bgeTest.AbsorptionFat(bgDir);
      //bgeTest.AbsorptionProtein(bgDir);
      //bgeTest.NutrientKineticsTestSimple(bgDir);
      //bgeTest.FourCompartmentTestSimple(bgDir);
      //bgeTest.AcidBaseFourCompartmentTest(bgDir);
      //bgeTest.FiveCompartmentTestWithDiffusion(bgDir);
	  //bgeTest.FiveCompartmentTestWithActiveDiffusion(bgDir);
      //bgeTest.AcidBaseFourCompartmentTestWithProductionConsumption(bgDir);
      //bgeTest.AcidBaseFiveCompartmentTestWithDiffusion(bgDir);
      //bgeTest.AcidBaseFiveCompartmentTestWithProductionConsumptionAndDiffusion(bgDir);

      // Our CV Characterization Driver  (Not part of the test suite)
      //std::string cvDir = ".\\UnitTests\\CVTuningTests";
      //MKDIR(cvDir.c_str());
      //bgeTest.CardiovascularCircuitScaleTests(cvDir);

      ///////////////
      // CDM Tests //
      ///////////////

      //Compartment Tests
      //cdmTest.GasCompartmentTest(cdmDir);
      //cdmTest.LiquidCompartmentTest(cdmDir);
      //cdmTest.ThermalCompartmentTest(cdmDir);
      //cdmTest.TissueCompartmentTest(cdmDir);

      //Transport Tests
      //cdmTest.LargeFlowTransportTest(cdmDir);
      //cdmTest.LiquidTransportTest(cdmDir);
      //cdmTest.GasTransportTest(cdmDir);

      //I/O Tests
      //cdmTest.ReadPatientDirectory(cdmDir);
      //cdmTest.ReadScenarios(cdmDir);
      //cdmTest.ReadSubstanceDirectory(cdmDir);

      //Property Tests
      //cdmTest.ScalarTest(cdmDir);
      //cdmTest.UnitsTest(cdmDir);

      //Advanced Circuit Tests
      //cdmTest.ElectricalCircuitTest(cdmDir);
      //cdmTest.FluidCircuitTest(cdmDir);
      //cdmTest.ThermalCircuitTest(cdmDir);
      //cdmTest.CombinedCircuitTest(cdmDir);
      //cdmTest.InterCircuitComparisonTest(cdmDir);
      //cdmTest.InterCircuitIndividualTest(cdmDir);
      //cdmTest.CircuitErrorTest(cdmDir);
      //cdmTest.DynamicallyChangingCircuitTest(cdmDir);
      //cdmTest.NonZeroReferencePositive(cdmDir);
      //cdmTest.NonZeroReferenceNegative(cdmDir);
      //cdmTest.PolarizedCapacitorTest(cdmDir);
      //cdmTest.PreChargeComplianceZeroVolume(cdmDir);
      //cdmTest.PreChargeComplianceNonZeroVolume(cdmDir);
      //cdmTest.CircuitLockingTest(cdmDir);

      ////Basic Circuit Tests
      //cdmTest.BasicCircuitTest(cdmDir);

      //cdmTest.SeriesRCDCTest(cdmDir);
      //cdmTest.SeriesRCSINTest(cdmDir);
      //cdmTest.SeriesRCPULSETest(cdmDir);

      //cdmTest.SeriesRDCTest(cdmDir);
      //cdmTest.SeriesRSINTest(cdmDir);
      //cdmTest.SeriesRPULSETest(cdmDir);

      //cdmTest.SeriesRLDCTest(cdmDir);
      //cdmTest.SeriesRLSINTest(cdmDir);
      //cdmTest.SeriesRLPULSETest(cdmDir);

      //cdmTest.ParallelRDCTest(cdmDir);
      //cdmTest.ParallelRSINTest(cdmDir);
      //cdmTest.ParallelRPULSETest(cdmDir);

      //cdmTest.ParallelRCDCTest(cdmDir);
      //cdmTest.ParallelRCSINTest(cdmDir);
      //cdmTest.ParallelRCPULSETest(cdmDir);

      //cdmTest.ParallelRLDCTest(cdmDir);
      //cdmTest.ParallelRLSINTest(cdmDir);
      //cdmTest.ParallelRLPULSETest(cdmDir);
      //cdmTest.ParallelRLSINCenteredTest(cdmDir);

      //cdmTest.SeriesRLCDCTest(cdmDir);
      //cdmTest.SeriesRLCSINTest(cdmDir);
      //cdmTest.SeriesRLCPULSETest(cdmDir);

      //cdmTest.ParallelRLCDCTest(cdmDir);
      //cdmTest.ParallelRLCSINTest(cdmDir);
      //cdmTest.ParallelRLCPULSETest(cdmDir);

      //cdmTest.SwitchRCDCTest(cdmDir);
      //cdmTest.SwitchRCSINTest(cdmDir);
      //cdmTest.SwitchRCPULSETest(cdmDir);

      //cdmTest.Comprehensive1DCTest(cdmDir);
      //cdmTest.Comprehensive1SINTest(cdmDir);
      //cdmTest.Comprehensive1PULSETest(cdmDir);

      //cdmTest.SimpleDiodeDCTest(cdmDir);
      //cdmTest.SimpleDiodeSINTest(cdmDir);
      //cdmTest.SimpleDiodePULSETest(cdmDir);

      //cdmTest.SeriesRCDCCurrentTest(cdmDir);
      //cdmTest.SeriesRCSINCurrentTest(cdmDir);
      //cdmTest.SeriesRCPULSECurrentTest(cdmDir);

      //cdmTest.ParallelRCDCCurrentTest(cdmDir);
      //cdmTest.ParallelRCSINCurrentTest(cdmDir);
      //cdmTest.ParallelRCPULSECurrentTest(cdmDir);

      //cdmTest.SeriesCapDCTest(cdmDir);
      //cdmTest.SeriesCapSINTest(cdmDir);
      //cdmTest.SeriesCapPULSETest(cdmDir);

      //cdmTest.ParallelRDCCurrentTest(cdmDir);
      //cdmTest.ParallelRSINCurrentTest(cdmDir);
      //cdmTest.ParallelRPULSECurrentTest(cdmDir);

      //cdmTest.CurrentCompDCTest(cdmDir);
      //cdmTest.CurrentCompSINTest(cdmDir);
      //cdmTest.CurrentCompPULSETest(cdmDir);

      //cdmTest.SeriesRLDCCurrentTest(cdmDir);
      //cdmTest.SeriesRLSINCurrentTest(cdmDir);
      //cdmTest.SeriesRLPULSECurrentTest(cdmDir);

      //cdmTest.ParallelRLDCCurrentTest(cdmDir);
      //cdmTest.ParallelRLSINCurrentTest(cdmDir);
      //cdmTest.ParallelRLPULSECurrentTest(cdmDir);

      //cdmTest.BasicDiodeDCCurrentTest(cdmDir);
      //cdmTest.BasicDiodeSINCurrentTest(cdmDir);
      //cdmTest.BasicDiodePULSECurrentTest(cdmDir);

      ////NOTE: These next three tests seem to give errors while running, but they produce output...
      //cdmTest.SwitchRCDCCurrentTest(cdmDir);
      //cdmTest.SwitchRCSINCurrentTest(cdmDir);
      //cdmTest.SwitchRCPULSECurrentTest(cdmDir);

      //cdmTest.SeriesRLCDCCurrentTest(cdmDir);
      //cdmTest.SeriesRLCSINCurrentTest(cdmDir);
      //cdmTest.SeriesRLCPULSECurrentTest(cdmDir);

      //cdmTest.ParallelRLCDCCurrentTest(cdmDir);
      //cdmTest.ParallelRLCSINCurrentTest(cdmDir);
      //cdmTest.ParallelRLCPULSECurrentTest(cdmDir);

      //cdmTest.SeriesPressureSourceAdditionDCTest(cdmDir);
      //cdmTest.SeriesPressureSourceAdditionSINTest(cdmDir);
      //cdmTest.SeriesPressureSourceAdditionPULSETest(cdmDir);

      ////NOTE: These next six tests seem to give errors while running, but it's intentional
      //cdmTest.SeriesCurrentSourceAdditionDCTest(cdmDir);
      //cdmTest.SeriesCurrentSourceAdditionSINTest(cdmDir);
      //cdmTest.SeriesCurrentSourceAdditionPULSETest(cdmDir);

      //cdmTest.ParallelPressureSourceAdditionDCTest(cdmDir);
      //cdmTest.ParallelPressureSourceAdditionSINTest(cdmDir);
      //cdmTest.ParallelPressureSourceAdditionPULSETest(cdmDir);

      //cdmTest.ParallelCurrentSourceAdditionDCTest(cdmDir);
      //cdmTest.ParallelCurrentSourceAdditionSINTest(cdmDir);
      //cdmTest.ParallelCurrentSourceAdditionPULSETest(cdmDir);

      //cdmTest.SeriesCapDCCurrentTest(cdmDir);
      //cdmTest.SeriesCapSINCurrentTest(cdmDir);
      //cdmTest.SeriesCapPULSECurrentTest(cdmDir);

      //cdmTest.ParallelCapDCTest(cdmDir);
      //cdmTest.ParallelCapSINTest(cdmDir);
      //cdmTest.ParallelCapPULSETest(cdmDir);

      //cdmTest.ParallelCapDCCurrentTest(cdmDir);
      //cdmTest.ParallelCapSINCurrentTest(cdmDir);
      //cdmTest.ParallelCapPULSECurrentTest(cdmDir);

      //cdmTest.SeriesIndDCTest(cdmDir);
      //cdmTest.SeriesIndSINTest(cdmDir);
      //cdmTest.SeriesIndPULSETest(cdmDir);

      //cdmTest.SeriesIndDCCurrentTest(cdmDir);
      //cdmTest.SeriesIndSINCurrentTest(cdmDir);
      //cdmTest.SeriesIndPULSECurrentTest(cdmDir);

      //cdmTest.ParallelIndDCTest(cdmDir);
      //cdmTest.ParallelIndSINTest(cdmDir);
      //cdmTest.ParallelIndPULSETest(cdmDir);

      //cdmTest.ParallelIndDCCurrentTest(cdmDir);
      //cdmTest.ParallelIndSINCurrentTest(cdmDir);
      //cdmTest.ParallelIndPULSECurrentTest(cdmDir);

      ////These next three tests intentionally fail
      //cdmTest.BadDiodeDCTest(cdmDir);
      //cdmTest.BadDiodeSINTest(cdmDir);
      //cdmTest.BadDiodePULSETest(cdmDir);

      //cdmTest.ValidationComprehensive1DCTest(cdmDir);
      //cdmTest.ValidationComprehensive1SINTest(cdmDir);
      //cdmTest.ValidationComprehensive1PULSETest(cdmDir);
      //cdmTest.ValidationComprehensive1SINCenteredTest(cdmDir);

      //cdmTest.ValidationComprehensive2DCTest(cdmDir);
      //cdmTest.ValidationComprehensive2SINTest(cdmDir);
      //cdmTest.ValidationComprehensive2PULSETest(cdmDir);
      //cdmTest.ValidationComprehensive2SINCenteredTest(cdmDir);
    }
    catch (CommonDataModelException& ex)
    {
      std::cerr << ex.what() << std::endl;
    }
    catch (std::exception& ex)
    {
      std::cerr << ex.what() << std::endl;
    }
    catch (...)
    {
      std::cerr << "Exception!!" << std::endl;
    }
    return 0;
  }
  else
  {
    if (cdmTest.RunTest(argv[1], ".\\UnitTests\\CDMTests"))
      return 0;
    if (bgeTest.RunTest(argv[1], ".\\UnitTests\\BioGearsTests"))
      return 0;
  }
  return 1;
}

