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

// The following tests will be exported and runnable
#include <math.h>

#include <biogears/circuit_test_exports.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
namespace biogears {
class SETestSuite;
class HowToTracker;

class BIOGEARS_CIRCUIT_TEST_API BioGearsEngineTest : Loggable {
public:
  BioGearsEngineTest();
  BioGearsEngineTest(Logger* logger);
  virtual ~BioGearsEngineTest();

  bool RunTest(const std::string& testName, const std::string& sOutputDirectory);
  void FillFunctionMap();

protected:
  bool myLogger;
  typedef void (BioGearsEngineTest::*testFunction)(const std::string&);
  typedef std::map<std::string, testFunction> testMap;
  testMap bgeMap;
  std::stringstream m_ss;

  //////////////////////////////////////////////////////////
  // Circuit Hemodynamics and Compartment Transport Tests //
  // System Specific Functions Tests as well              //
  //////////////////////////////////////////////////////////
public:
  ////////////////////
  // Cardiovascular //
  void CardiovascularCircuitAndTransportTest(const std::string& sTestDirectory);
  void CardiovascularAndRenalCircuitAndTransportTest(const std::string& sTestDirectory);
  void CardiovascularAndTissueCircuitAndTransportTest(const std::string& sTestDirectory);
  void CardiovascularTissueAndRenalCircuitAndTransportTest(const std::string& sTestDirectory);
  void CardiovascularBloodGasesTest(const std::string& sTestDirectory); // Characterize the cv circuit
  void TuneCardiovascularCircuitTest(const std::string& sTestDirectory);
  // Cardiovascular Utilities //
  void CardiovascularCircuitScaleTests(const std::string& sTestDirectory);

protected:
  enum CardiovascularDriver { Sinusoid = 0,
    Heart };
  void SinusoidHeartDriver(double time_s, double heartRate_s, double& lHeartElastance, double& rHeartElastance);
  void CardiovascularCircuitAndTransportTest(CardiovascularDriver driverType, double complianceScale, double resistanceScale, double volumeScale, double heartRate_bpm,
    double sysRes, double sysComp, double aortaRes, double aortaComp, double rightHeartRes, double venaCavaComp, bool connectTissue, bool connectRenal,
    bool balanceBloodgases, const std::string& sTestDirectory, const std::string& sTestName, bool breakOutResults);
  void TuneCardiovascularCircuitTest(SETestSuite& testSuite, const std::string& sTestDirectory, const std::string& sTestName, SEPatient& patient);

  double m_MeanAortaPressure;
  double m_MeanCVPressure;
  double m_Systolic;
  double m_Diastolic;
  double m_Pulse;
  double m_CardiacOutput;

public:
  ///////////
  // Renal //
  void RenalCircuitAndTransportTest(const std::string& sTestDirectory);
  void RenalTGFFeedbackTest(const std::string& sTestDirectory);
  void RenalTGFandUPRFeedbackTest(const std::string& sTestDirectory);
  void RenalSecretionTest(const std::string& sTestDirectory);
  void RenalUrinateTest(const std::string& sTestDirectory);

protected:
  enum RenalFeedback { TGF,
    TGFandUPR };
  void RenalFeedbackTest(RenalFeedback feedback, const std::string& sTestDirectory, const std::string& sTestName);
  enum RenalSystems { Secretion,
    Urinating };
  void RenalSystemTest(RenalSystems systemtest, const std::string& sTestDirectory, const std::string& sTestName);

public:
  /////////////////
  // Respiratory //
  void RespiratoryCircuitAndTransportTest(const std::string& sTestDirectory);
  void RespiratoryDriverTest(const std::string& sTestDirectory);

protected:
  enum RespiratoryConfiguration { RespiratorySolo,
    AnesthesiaMachineSolo,
    RespiratoryWithAnesthesiaMachine,
    RespiratoryWithInhaler,
    RespiratoryWithMechanicalVentilator };
  void RespiratoryCircuitAndTransportTest(RespiratoryConfiguration config, const std::string& sTestDirectory);

public:
  ////////////////////////
  // Anesthesia Machine //
  void AnesthesiaMachineCircuitAndTransportTest(const std::string& sTestDirectory);
  void RespiratoryWithAnesthesiaMachineCircuitAndTransportTest(const std::string& sTestDirectory);

protected:
  void AnesthesiaMachineCircuitAndTransportTest(RespiratoryConfiguration config, const std::string& sTestDirectory);

public:
  /////////////
  // Inhaler //
  void RespiratoryWithInhalerCircuitAndTransportTest(const std::string& sTestDirectory);
  void RespiratoryWithMechanicalVentilatorCircuitAndTransportTest(const std::string& sTestDirectory);

protected:
public:
  ////////
  // GI //
  enum MealType { CarbsOnly,
    ProteinOnly,
    FatOnly,
    Balanced,
    None };
  void CustomNutrientKineticsTest(const std::string& sOutputDirectory);
  void ProteinGluconeogenesisTest(const std::string& sOutputDirectory);
  void StarvingKetogenesisTest(const std::string& sOutputDirectory);
  void AnaerobicExerciseTest(const std::string& sOutputDirectory);
  void HormoneStabilityTest(const std::string& sOutputDirectory);
  void StorageTest(const std::string& sOutputDirectory);
  void FullStoresFastingTest(const std::string& sOutputDirectory);
  void LipogenesisTest(const std::string& sOutputDirectory);
  void DigestionProtein(const std::string& sOutputDirectory);
  void DigestionFat(const std::string& sOutputDirectory);
  void DigestionMixed(const std::string& sOutputDirectory);
  void DigestionCarbs(const std::string& sOutputDirectory);
  void AbsorptionCarbs(const std::string& sOutputDirectory);
  void AbsorptionProtein(const std::string& sOutputDirectory);
  void AbsorptionFat(const std::string& sOutputDirectory);

protected:
  void NutrientKineticsTest(bool usingAbsorption, bool usingDynamicHormones, bool usingGlycogen, bool usingProteinStorage, bool usingFatStorage, bool fullStores, bool useDiffusion, bool useConsumption, bool usingLipogenesis, bool usingGluconeogenesis, bool isAnaerobic, double exerciseWork_W, const std::string& sOutputDirectory, double testDuration_hr, MealType mealType = None, std::string testName = "Custom");
  void ProduceAndConsume(double baseEnergyRequested_kcal, double exerciseEnergyRequested_kcal, bool isAnaerobic, BioGears& bg, double deltaT_s, double brainFlowFraction, double& muscleGlycogen_g, double& CO2Produced_mol, double& O2Consumed_mol, double& brainEnergyDeficit_kcal, double& muscleEnergyDeficit_kcal, double& lactateFromGlucose_g, DataTrack& trk);
  void DigestionTest(const std::string& sOutputDirectory, MealType = None);
  void AbsorptionTest(const std::string& sOutputDirectory, MealType = None);

public:
  ////////////////////////////
  // Environment and Energy //
  void InternalTemperatureVariableBMRCircuitTest(const std::string& sTestDirectory);
  void InternalTemperatureVariableSkinCircuitTest(const std::string& sTestDirectory);
  void InternalTemperatureVariableCoreCircuitTest(const std::string& sTestDirectory);
  void EnvironmentVariableTemperatureCircuitTest(const std::string& sTestDirectory);
  void CombinedInternalAndEnvironmentVariableBMRandTemperatureCircuitTest(const std::string& sTestDirectory);
  void CombinedInternalAndEnvironmentSkinTempDropCircuitTest(const std::string& sTestDirectory);
  void EnvironmentISO7730ComparisonTest(const std::string& sTestDirectory);

protected:
public:
  //////////////////
  // Tissue Tests //
  void DiffusionClearanceExcretionTests(const std::string& rptDirectory);
  void AlveolarCarbonDioxideDiffusionTest(const std::string& rptDirectory);
  void AlveolarOxygenDiffusionTest(const std::string& rptDirectory);
  void EigenDiffusionTest(const std::string& rptDirectory);
  void FacilitatedDiffusionTest(const std::string& rptDirectory);
  void TissueCombinedTransportTest(const std::string& rptDirectory);

protected:
  void DistributeMass(SETestSuite& testSuite);
  void PerfusionLimitedDiffusionTest(SETestSuite& testSuite);

  void GenericClearanceTest(SETestSuite& testSuite);
  void GenericExcretionTest(SETestSuite& testSuite);

public:
  ///////////////////
  // Nervous Tests //
  void BrainInjuryTest(const std::string& rptDirectory);

protected:

public:
  ///////////////////
  // Blood Chemistry Tests //
  void AcuteInflammationTest(const std::string& rptDirectory);

protected:
  double Up1(double y, double x, double n);
  double Up2(double y, double x, double n);
  double Down(double y, double x, double n);
  double AlbuminTransport(SELiquidCompartment& from, SELiquidCompartment& to, SEFluidCircuitPath& flowPath, SESubstance& alb, double dt_min, double tissueIntegrity);
  void UpdateOncoticPressure(SEScalarMassPerVolume& albVas, SEScalarMassPerVolume& albExtra, SEFluidCircuitPath& plasma, SEFluidCircuitPath& inter);

public:
  ////////////////////////////
  // Acid Base Engine Tests //
  void FourCompartmentTestSimple(const std::string& sOutputDirectory);
  void AcidBaseFourCompartmentTest(const std::string& sOutputDirectory);
  void FiveCompartmentTestWithDiffusion(const std::string& sOutputDirectory);
  void FiveCompartmentTestWithActiveDiffusion(const std::string& sOutputDirectory);
  void AcidBaseFourCompartmentTestWithProductionConsumption(const std::string& sOutputDirectory);
  void AcidBaseFiveCompartmentTestWithDiffusion(const std::string& sOutputDirectory);
  void AcidBaseFiveCompartmentTestWithProductionConsumptionAndDiffusion(const std::string& sOutputDirectory);

protected:
  void FourCompartmentTest(bool usingAcidBase, bool usingProductionConsumption, bool usingDiffusion, bool activeDiffusion, const std::string& sOutputDirectory);
  bool usingAcidBase;
  bool usingProductionConsumption;
  bool usingDiffusion;
  bool activeDiffusion; //Flag to determine whether we are testing active diffusion of ionic species (needs to be used in conjunction with usingDiffusion)

  /////////////////////
  // Acid Base Tests //
  /////////////////////
public:
  void AcidBaseMathTest(const std::string& sOutputDirectory);
  void AcidBaseFeedbackTest(const std::string& sOutputDirectory);
  void AcidBaseLimitsTest(const std::string& sOutputDirectory);
  void AcidBaseExtremeTest(const std::string& rptDirectory);
  void AcidBaseBloodGasTests(const std::string& sOutputDirectory);

protected:
  enum bloodType { ARTERIAL,
    VENOUS,
    CAPILLARY,
    RESPIRATORY_ACIDOSIS,
    METABOLIC_ALKALOSIS,
    METABOLIC_ACIDOSIS,
    RESPIRATORY_ALKALOSIS,
    CUSTOM };
  void AcidBaseBloodGasTest(BioGears& bg, bloodType compartment, SETestSuite& testSuite);

  /////////////////////
  // Aerosol Tests //
  /////////////////////
public:
  void AerosolTest(const std::string& sOutputDirectory);

protected:
  void SizeIndependentDepositionEfficencyCoefficientsTest(SETestSuite& suite, SESubstance& substance, double expectedMouthCoeff, double expectedCarinaCoeff, double expectedDeadSpaceCoeff, double expectedAlveoliCoeff);
  void DepositionFractionTest(SETestSuite& suite, SESubstance& substance, double expectedMouthDepFrac, double expectedCarinaDepFrac, double expectedDeadSpaceDepFrac, double expectedAlveoliDepFrac);

public:
  //////////////////////////
  // General Engine Tests //
  void ConditionCombinations(const std::string& rptDirectory);
  void MultiEngineTest(const std::string& sTestDirectory);
  void SerializationTest(const std::string& sTestDirectory);
  void SolverSpeedTest(const std::string& sTestDirectory);

protected:
  void InhalerState(PhysiologyEngine* bg, HowToTracker& tracker);
  void InjectSuccsState(PhysiologyEngine* bg, HowToTracker& tracker, const SESubstance& succs);
};
}