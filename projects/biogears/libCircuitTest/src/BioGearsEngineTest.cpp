/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at: http://www.apache.org/licenses/LICENSE-2.0 Unless required by
applicable law or agreed to in writing, software distributed under the License
is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied. See the License for the specific language
governing permissions and limitations under the License.
**************************************************************************************/

#include <biogears/engine/test/BioGearsEngineTest.h>

namespace biogears {
BioGearsEngineTest::BioGearsEngineTest()
  : Loggable(new Logger())
{
  myLogger = true;
  FillFunctionMap();
}

BioGearsEngineTest::BioGearsEngineTest(Logger* logger)
  : Loggable(logger)
{
  myLogger = false;
  FillFunctionMap();
}

BioGearsEngineTest::~BioGearsEngineTest()
{
  if (myLogger)
    SAFE_DELETE(m_Logger);
}

bool BioGearsEngineTest::RunTest(const std::string& testName,
  const std::string& sOutputDirectory)
{
  try {
    // If you find the test name, run it
    if (bgeMap.find(testName) != bgeMap.end()) {
      testFunction func = bgeMap.at(testName);
      (this->*func)(sOutputDirectory);
      return true;
    } else {
      // Try to find the function with "Test" appended; run it if found
      if (bgeMap.find(testName + "Test") != bgeMap.end()) {
        testFunction func = bgeMap.at(testName + "Test");
        (this->*func)(sOutputDirectory);
        return true;
      }

      m_ss << "Could not find " << testName << " or " << testName
           << "Test in BioGearsEngineTest." << std::endl;
      Error(m_ss);
      return false;
    }
  } catch (CommonDataModelException& ex) {
    Error(ex.what());
  } catch (std::exception& ex) {
    Error(ex.what());
  } catch (...) {
    Error("Exception!!");
  }
  return false;
}

void BioGearsEngineTest::FillFunctionMap()
{
  // A few functions have overloads, so we have to use the pointer clarify which
  // one we want
  testFunction cardioCTTest = &BioGearsEngineTest::CardiovascularCircuitAndTransportTest;
  testFunction cardioBGTest = &BioGearsEngineTest::CardiovascularBloodGasesTest;
  testFunction cardioTuneCircuitTest = &BioGearsEngineTest::TuneCardiovascularCircuitTest;
  testFunction respCTTest = &BioGearsEngineTest::RespiratoryCircuitAndTransportTest;
  testFunction anesthCTTest = &BioGearsEngineTest::AnesthesiaMachineCircuitAndTransportTest;

  // Fill a map that ties unit test names to their actual functions
  bgeMap.insert(
    std::make_pair("CardiovascularCircuitAndTransportTest", cardioCTTest));
  bgeMap.insert(std::make_pair(
    "CardiovascularAndRenalCircuitAndTransportTest",
    &BioGearsEngineTest::CardiovascularAndRenalCircuitAndTransportTest));
  bgeMap.insert(std::make_pair(
    "CardiovascularAndTissueCircuitAndTransportTest",
    &BioGearsEngineTest::CardiovascularAndTissueCircuitAndTransportTest));
  bgeMap.insert(
    std::make_pair("CardiovascularTissueAndRenalCircuitAndTransportTest",
      &BioGearsEngineTest::
        CardiovascularTissueAndRenalCircuitAndTransportTest));
  bgeMap.insert(std::make_pair("CardiovascularBloodGasesTest", cardioBGTest));
  bgeMap.insert(
    std::make_pair("TuneCardiovascularCircuitTest", cardioTuneCircuitTest));
  bgeMap.insert(
    std::make_pair("CardiovascularCircuitScaleTests",
      &BioGearsEngineTest::CardiovascularCircuitScaleTests));

  bgeMap.insert(
    std::make_pair("RenalCircuitAndTransportTest",
      &BioGearsEngineTest::RenalCircuitAndTransportTest));
  bgeMap.insert(std::make_pair("RenalTGFFeedbackTest",
    &BioGearsEngineTest::RenalTGFFeedbackTest));
  bgeMap.insert(
    std::make_pair("RenalTGFandUPRFeedbackTest",
      &BioGearsEngineTest::RenalTGFandUPRFeedbackTest));
  bgeMap.insert(std::make_pair("RenalSecretionTest",
    &BioGearsEngineTest::RenalSecretionTest));
  bgeMap.insert(std::make_pair("RenalUrinateTest",
    &BioGearsEngineTest::RenalUrinateTest));

  bgeMap.insert(
    std::make_pair("RespiratoryCircuitAndTransportTest", respCTTest));
  bgeMap.insert(std::make_pair("RespiratoryDriverTest",
    &BioGearsEngineTest::RespiratoryDriverTest));

  bgeMap.insert(
    std::make_pair("AnesthesiaMachineCircuitAndTransportTest", anesthCTTest));
  bgeMap.insert(std::make_pair(
    "RespiratoryWithAnesthesiaMachineCircuitAndTransportTest",
    &BioGearsEngineTest::
      RespiratoryWithAnesthesiaMachineCircuitAndTransportTest));

  bgeMap.insert(std::make_pair(
    "RespiratoryWithInhalerCircuitAndTransportTest",
    &BioGearsEngineTest::RespiratoryWithInhalerCircuitAndTransportTest));

  bgeMap.insert(std::make_pair(
    "RespiratoryWithMechanicalVentilatorCircuitAndTransportTest",
    &BioGearsEngineTest::
      RespiratoryWithMechanicalVentilatorCircuitAndTransportTest));

  bgeMap.insert(std::make_pair(
    "InternalTemperatureVariableBMRCircuitTest",
    &BioGearsEngineTest::InternalTemperatureVariableBMRCircuitTest));
  bgeMap.insert(std::make_pair(
    "InternalTemperatureVariableSkinCircuitTest",
    &BioGearsEngineTest::InternalTemperatureVariableSkinCircuitTest));
  bgeMap.insert(std::make_pair(
    "InternalTemperatureVariableCoreCircuitTest",
    &BioGearsEngineTest::InternalTemperatureVariableCoreCircuitTest));
  bgeMap.insert(std::make_pair(
    "EnvironmentVariableTemperatureCircuitTest",
    &BioGearsEngineTest::EnvironmentVariableTemperatureCircuitTest));
  bgeMap.insert(std::make_pair(
    "CombinedInternalAndEnvironmentVariableBMRandTemperatureCircuitTest",
    &BioGearsEngineTest::
      CombinedInternalAndEnvironmentVariableBMRandTemperatureCircuitTest));
  bgeMap.insert(std::make_pair(
    "CombinedInternalAndEnvironmentSkinTempDropCircuitTest",
    &BioGearsEngineTest::
      CombinedInternalAndEnvironmentSkinTempDropCircuitTest));
  bgeMap.insert(
    std::make_pair("EnvironmentISO7730ComparisonTest",
      &BioGearsEngineTest::EnvironmentISO7730ComparisonTest));

  bgeMap.insert(
    std::make_pair("DiffusionClearanceExcretionTests",
      &BioGearsEngineTest::DiffusionClearanceExcretionTests));
  bgeMap.insert(
    std::make_pair("AlveolarCarbonDioxideDiffusionTest",
      &BioGearsEngineTest::AlveolarCarbonDioxideDiffusionTest));
  bgeMap.insert(
    std::make_pair("AlveolarOxygenDiffusionTest",
      &BioGearsEngineTest::AlveolarOxygenDiffusionTest));
  bgeMap.insert(
    std::make_pair("EigenDiffusionTest",
      &BioGearsEngineTest::EigenDiffusionTest));
  bgeMap.insert(std::make_pair("FacilitatedDiffusionTest",
    &BioGearsEngineTest::FacilitatedDiffusionTest));
  bgeMap.insert(
    std::make_pair("TissueCombinedTransportTest",
      &BioGearsEngineTest::TissueCombinedTransportTest));

  bgeMap.insert(
    std::make_pair("BrainInjuryTest", &BioGearsEngineTest::BrainInjuryTest));

  bgeMap.insert(std::make_pair("ConditionCombinations",
    &BioGearsEngineTest::ConditionCombinations));
  bgeMap.insert(
    std::make_pair("MultiEngineTest", &BioGearsEngineTest::MultiEngineTest));
  bgeMap.insert(std::make_pair("SerializationTest",
    &BioGearsEngineTest::SerializationTest));
  bgeMap.insert(
    std::make_pair("SolverSpeedTest", &BioGearsEngineTest::SolverSpeedTest));

  bgeMap.insert(std::make_pair("AcidBaseMathTest",
    &BioGearsEngineTest::AcidBaseMathTest));
  bgeMap.insert(std::make_pair("AcidBaseFeedbackTest",
    &BioGearsEngineTest::AcidBaseFeedbackTest));
  bgeMap.insert(std::make_pair("AcidBaseLimitsTest",
    &BioGearsEngineTest::AcidBaseLimitsTest));
  bgeMap.insert(std::make_pair("AcidBaseExtremeTest",
    &BioGearsEngineTest::AcidBaseExtremeTest));
  bgeMap.insert(std::make_pair("AcidBaseBloodGasTests",
    &BioGearsEngineTest::AcidBaseBloodGasTests));

  bgeMap.insert(
    std::make_pair("AerosolTest", &BioGearsEngineTest::AerosolTest));

  bgeMap.insert(std::make_pair("FourCompartmentTestSimple",
    &BioGearsEngineTest::FourCompartmentTestSimple));
  bgeMap.insert(
    std::make_pair("AcidBaseFourCompartmentTest",
      &BioGearsEngineTest::AcidBaseFourCompartmentTest));
  bgeMap.insert(
    std::make_pair("AcidBaseFourCompartmentTestWithProductionConsumption",
      &BioGearsEngineTest::
        AcidBaseFourCompartmentTestWithProductionConsumption));

  bgeMap.insert(
    std::make_pair("CustomNutrientKineticsTest",
      &BioGearsEngineTest::CustomNutrientKineticsTest));
  bgeMap.insert(
    std::make_pair("ProteinGluconeogenesisTest",
      &BioGearsEngineTest::ProteinGluconeogenesisTest));
  bgeMap.insert(std::make_pair("StarvingKetogenesisTest",
    &BioGearsEngineTest::StarvingKetogenesisTest));
  bgeMap.insert(std::make_pair("AnaerobicExerciseTest",
    &BioGearsEngineTest::AnaerobicExerciseTest));
  bgeMap.insert(std::make_pair("HormoneStabilityTest",
    &BioGearsEngineTest::HormoneStabilityTest));
  bgeMap.insert(
    std::make_pair("StorageTest", &BioGearsEngineTest::StorageTest));
  bgeMap.insert(std::make_pair("FullStoresFastingTest",
    &BioGearsEngineTest::FullStoresFastingTest));
  bgeMap.insert(
    std::make_pair("LipogenesisTest", &BioGearsEngineTest::LipogenesisTest));

  bgeMap.insert(
    std::make_pair("AcuteInflammationTest",
      &BioGearsEngineTest::AcuteInflammationTest));
}
}