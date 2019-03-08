// The following tests will be exported and runnable
#include <string>
#include <sstream>
#include <vector>

#include <biogears/circuit_test_exports.h>
// #include <biogears/schema/biogears-cdm.hxx>
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitCalculator.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/compartment/thermal/SEThermalCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/utils/testing/SETestCase.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>

namespace biogears {
class SECompartmentManager;

enum enumCDMTestSourceType { DC,
  SIN,
  PULSE,
  ZEROCENTEREDSIN };

class BIOGEARS_CIRCUIT_TEST_API CommonDataModelTest : public Loggable {
public:
  CommonDataModelTest();
  CommonDataModelTest(Logger* logger);
  virtual ~CommonDataModelTest();

  bool RunTest(const std::string& testName,
    const std::string& sOutputDirectory);
  void FillFunctionMap();

protected:
  bool myLogger;
  double m_PercentTolerance;
  std::string m_OutDirectory;
  std::stringstream m_ss;

  typedef void (CommonDataModelTest::*testFunction)(const std::string&);
  typedef std::map<std::string, testFunction> testMap;
  testMap cdmMap;

  //////////////
  // Circuits //
  //////////////
public:
  // Basic Circuit Tests
  void BasicCircuitTest(const std::string& sOutputDirectory);

  void SeriesRCDCTest(const std::string& sOutputDirectory);
  void SeriesRCSINTest(const std::string& sOutputDirectory);
  void SeriesRCPULSETest(const std::string& sOutputDirectory);

  void SeriesRDCTest(const std::string& sOutputDirectory);
  void SeriesRSINTest(const std::string& sOutputDirectory);
  void SeriesRPULSETest(const std::string& sOutputDirectory);

  void SeriesRLDCTest(const std::string& sOutputDirectory);
  void SeriesRLSINTest(const std::string& sOutputDirectory);
  void SeriesRLPULSETest(const std::string& sOutputDirectory);

  void ParallelRDCTest(const std::string& sOutputDirectory);
  void ParallelRSINTest(const std::string& sOutputDirectory);
  void ParallelRPULSETest(const std::string& sOutputDirectory);

  void ParallelRCDCTest(const std::string& sOutputDirectory);
  void ParallelRCSINTest(const std::string& sOutputDirectory);
  void ParallelRCPULSETest(const std::string& sOutputDirectory);

  void ParallelRLDCTest(const std::string& sOutputDirectory);
  void ParallelRLSINTest(const std::string& sOutputDirectory);
  void ParallelRLPULSETest(const std::string& sOutputDirectory);
  void ParallelRLSINCenteredTest(const std::string& sOutputDirectory);

  void SeriesRLCDCTest(const std::string& sOutputDirectory);
  void SeriesRLCSINTest(const std::string& sOutputDirectory);
  void SeriesRLCPULSETest(const std::string& sOutputDirectory);

  void ParallelRLCDCTest(const std::string& sOutputDirectory);
  void ParallelRLCSINTest(const std::string& sOutputDirectory);
  void ParallelRLCPULSETest(const std::string& sOutputDirectory);

  void SwitchRCDCTest(const std::string& sOutputDirectory);
  void SwitchRCSINTest(const std::string& sOutputDirectory);
  void SwitchRCPULSETest(const std::string& sOutputDirectory);

  void Comprehensive1DCTest(const std::string& sOutputDirectory);
  void Comprehensive1SINTest(const std::string& sOutputDirectory);
  void Comprehensive1PULSETest(const std::string& sOutputDirectory);

  void SimpleDiodeDCTest(const std::string& sOutputDirectory);
  void SimpleDiodeSINTest(const std::string& sOutputDirectory);
  void SimpleDiodePULSETest(const std::string& sOutputDirectory);

  void SeriesRCDCCurrentTest(const std::string& sOutputDirectory);
  void SeriesRCSINCurrentTest(const std::string& sOutputDirectory);
  void SeriesRCPULSECurrentTest(const std::string& sOutputDirectory);

  void ParallelRCDCCurrentTest(const std::string& sOutputDirectory);
  void ParallelRCSINCurrentTest(const std::string& sOutputDirectory);
  void ParallelRCPULSECurrentTest(const std::string& sOutputDirectory);

  void SeriesCapDCTest(const std::string& sOutputDirectory);
  void SeriesCapSINTest(const std::string& sOutputDirectory);
  void SeriesCapPULSETest(const std::string& sOutputDirectory);

  void ParallelRDCCurrentTest(const std::string& sOutputDirectory);
  void ParallelRSINCurrentTest(const std::string& sOutputDirectory);
  void ParallelRPULSECurrentTest(const std::string& sOutputDirectory);

  void CurrentCompDCTest(const std::string& sOutputDirectory);
  void CurrentCompSINTest(const std::string& sOutputDirectory);
  void CurrentCompPULSETest(const std::string& sOutputDirectory);

  void SeriesRLDCCurrentTest(const std::string& sOutputDirectory);
  void SeriesRLSINCurrentTest(const std::string& sOutputDirectory);
  void SeriesRLPULSECurrentTest(const std::string& sOutputDirectory);

  void ParallelRLDCCurrentTest(const std::string& sOutputDirectory);
  void ParallelRLSINCurrentTest(const std::string& sOutputDirectory);
  void ParallelRLPULSECurrentTest(const std::string& sOutputDirectory);

  void BasicDiodeDCCurrentTest(const std::string& sOutputDirectory);
  void BasicDiodeSINCurrentTest(const std::string& sOutputDirectory);
  void BasicDiodePULSECurrentTest(const std::string& sOutputDirectory);

  // NOTE: These next three tests seem to give errors while running, but they
  // produce output...
  void SwitchRCDCCurrentTest(const std::string& sOutputDirectory);
  void SwitchRCSINCurrentTest(const std::string& sOutputDirectory);
  void SwitchRCPULSECurrentTest(const std::string& sOutputDirectory);

  void SeriesRLCDCCurrentTest(const std::string& sOutputDirectory);
  void SeriesRLCSINCurrentTest(const std::string& sOutputDirectory);
  void SeriesRLCPULSECurrentTest(const std::string& sOutputDirectory);

  void ParallelRLCDCCurrentTest(const std::string& sOutputDirectory);
  void ParallelRLCSINCurrentTest(const std::string& sOutputDirectory);
  void ParallelRLCPULSECurrentTest(const std::string& sOutputDirectory);

  void SeriesPressureSourceAdditionDCTest(const std::string& sOutputDirectory);
  void SeriesPressureSourceAdditionSINTest(const std::string& sOutputDirectory);
  void
  SeriesPressureSourceAdditionPULSETest(const std::string& sOutputDirectory);

  // NOTE: These next six tests seem to give errors while running, but it's
  // intentional
  void SeriesCurrentSourceAdditionDCTest(const std::string& sOutputDirectory);
  void SeriesCurrentSourceAdditionSINTest(const std::string& sOutputDirectory);
  void
  SeriesCurrentSourceAdditionPULSETest(const std::string& sOutputDirectory);

  void
  ParallelPressureSourceAdditionDCTest(const std::string& sOutputDirectory);
  void
  ParallelPressureSourceAdditionSINTest(const std::string& sOutputDirectory);
  void
  ParallelPressureSourceAdditionPULSETest(const std::string& sOutputDirectory);

  void ParallelCurrentSourceAdditionDCTest(const std::string& sOutputDirectory);
  void
  ParallelCurrentSourceAdditionSINTest(const std::string& sOutputDirectory);
  void
  ParallelCurrentSourceAdditionPULSETest(const std::string& sOutputDirectory);

  void SeriesCapDCCurrentTest(const std::string& sOutputDirectory);
  void SeriesCapSINCurrentTest(const std::string& sOutputDirectory);
  void SeriesCapPULSECurrentTest(const std::string& sOutputDirectory);

  void ParallelCapDCTest(const std::string& sOutputDirectory);
  void ParallelCapSINTest(const std::string& sOutputDirectory);
  void ParallelCapPULSETest(const std::string& sOutputDirectory);

  void ParallelCapDCCurrentTest(const std::string& sOutputDirectory);
  void ParallelCapSINCurrentTest(const std::string& sOutputDirectory);
  void ParallelCapPULSECurrentTest(const std::string& sOutputDirectory);

  void SeriesIndDCTest(const std::string& sOutputDirectory);
  void SeriesIndSINTest(const std::string& sOutputDirectory);
  void SeriesIndPULSETest(const std::string& sOutputDirectory);

  void SeriesIndDCCurrentTest(const std::string& sOutputDirectory);
  void SeriesIndSINCurrentTest(const std::string& sOutputDirectory);
  void SeriesIndPULSECurrentTest(const std::string& sOutputDirectory);

  void ParallelIndDCTest(const std::string& sOutputDirectory);
  void ParallelIndSINTest(const std::string& sOutputDirectory);
  void ParallelIndPULSETest(const std::string& sOutputDirectory);

  void ParallelIndDCCurrentTest(const std::string& sOutputDirectory);
  void ParallelIndSINCurrentTest(const std::string& sOutputDirectory);
  void ParallelIndPULSECurrentTest(const std::string& sOutputDirectory);

  // These next three tests intentionally fail
  void BadDiodeDCTest(const std::string& sOutputDirectory);
  void BadDiodeSINTest(const std::string& sOutputDirectory);
  void BadDiodePULSETest(const std::string& sOutputDirectory);

  void ValidationComprehensive1DCTest(const std::string& sOutputDirectory);
  void ValidationComprehensive1SINTest(const std::string& sOutputDirectory);
  void ValidationComprehensive1PULSETest(const std::string& sOutputDirectory);
  void
  ValidationComprehensive1SINCenteredTest(const std::string& sOutputDirectory);

  void ValidationComprehensive2DCTest(const std::string& sOutputDirectory);
  void ValidationComprehensive2SINTest(const std::string& sOutputDirectory);
  void ValidationComprehensive2PULSETest(const std::string& sOutputDirectory);
  void
  ValidationComprehensive2SINCenteredTest(const std::string& sOutputDirectory);

  // Advanced Circuit Tests
  void ElectricalCircuitTest(const std::string& sOutputDirectory);
  void FluidCircuitTest(const std::string& sOutputDirectory);
  void ThermalCircuitTest(const std::string& sOutputDirectory);
  void CombinedCircuitTest(const std::string& sOutputDirectory);
  void InterCircuitComparisonTest(const std::string& sOutputDirectory);
  void InterCircuitIndividualTest(
    const std::string& sOutputDirectory); // This one outputs two files
  void CircuitErrorTest(
    const std::string& sOutputDirectory); // This one doesn't output anything right now
  void DynamicallyChangingCircuitTest(
    const std::string& sOutputDirectory); // This one has some really strange output (-1.$)
  void NonZeroReferencePositive(const std::string& sOutputDirectory);
  void NonZeroReferenceNegative(const std::string& sOutputDirectory);
  void PolarizedCapacitorTest(const std::string& sOutputDirectory);
  void PreChargeComplianceZeroVolume(const std::string& sOutputDirectory);
  void PreChargeComplianceNonZeroVolume(const std::string& sOutputDirectory);
  void CircuitLockingTest(const std::string& sOutputDirectory);

protected:
  // Used to generalize Basic Tests
  void RunTest(const std::string& sOutputDirectory, const std::string& testName,
    enumCDMTestSourceType sourceType, int testIndex);

  // Setup and preprocess methods for Basic Tests
  void TestSetup7SeriesRCDC();
  void TestSetup7SeriesRCSIN();
  void TestSetup7SeriesRCPULSE();
  void TestSetup8SeriesRDC();
  void TestSetup8SeriesRSIN();
  void TestSetup8SeriesRPULSE();
  void TestSetup9SeriesRLDC();
  void TestSetup9SeriesRLSIN();
  void TestSetup9SeriesRLPULSE();
  void TestSetup10ParallelRDC();
  void TestSetup10ParallelRSIN();
  void TestSetup10ParallelRPULSE();
  void TestSetup11ParallelRCDC();
  void TestSetup11ParallelRCSIN();
  void TestSetup11ParallelRCPULSE();
  void TestSetup12ParallelRLDC();
  void TestSetup12ParallelRLSIN();
  void TestSetup12ParallelRLPULSE();
  void TestSetup12ParallelRLSINCentered();
  void TestSetup13SeriesRLCDC();
  void TestSetup13SeriesRLCSIN();
  void TestSetup13SeriesRLCPULSE();
  void TestSetup14ParallelRLCDC();
  void TestSetup14ParallelRLCSIN();
  void TestSetup14ParallelRLCPULSE();
  void TestSetup15SwitchRCDC();
  void TestSetup15SwitchRCSIN();
  void TestSetup15SwitchRCPULSE();
  void TestSetup16Comprehensive1DC();
  void TestSetup16Comprehensive1SIN();
  void TestSetup16Comprehensive1PULSE();
  void TestSetup17BasicDiodeDC();
  void TestSetup17BasicDiodeSIN();
  void TestSetup17BasicDiodePULSE();
  void TestSetup18RCSeriesDCCurrent();
  void TestSetup18RCSeriesSINCurrent();
  void TestSetup18RCSeriesPulseCurrent();
  void TestSetup19RCParallelDCCurrent();
  void TestSetup19RCParallelSINCurrent();
  void TestSetup19RCParallelPulseCurrent();
  void TestSetup20SeriesCapDC();
  void TestSetup20SeriesCapSIN();
  void TestSetup20SeriesCapPulse();
  void TestSetup21ParallelRDCCurrent();
  void TestSetup21ParallelRSINCurrent();
  void TestSetup21ParallelRPulseCurrent();
  void TestSetup22CurrentCompDC();
  void TestSetup22CurrentCompSIN();
  void TestSetup22CurrentCompPulse();
  void TestSetup23SeriesRLDCCurrent();
  void TestSetup23SeriesRLSINCurrent();
  void TestSetup23SeriesRLPULSECurrent();
  void TestSetup24ParallelRLDCCurrent();
  void TestSetup24ParallelRLSINCurrent();
  void TestSetup24ParallelRLPULSECurrent();
  void TestSetup25BasicDiodeDCCurrent();
  void TestSetup25BasicDiodeSINCurrent();
  void TestSetup25BasicDiodePULSECurrent();
  void TestSetup26SwitchRCDCCurrent();
  void TestSetup26SwitchRCSINCurrent();
  void TestSetup26SwitchRCPULSECurrent();
  void TestSetup27SeriesRLCDCCurrent();
  void TestSetup27SeriesRLCSINCurrent();
  void TestSetup27SeriesRLCPULSECurrent();
  void TestSetup28ParallelRLCDCCurrent();
  void TestSetup28ParallelRLCSINCurrent();
  void TestSetup28ParallelRLCPULSECurrent();
  void TestSetup29SeriesPressureSourceAdditionDC();
  void TestSetup29SeriesPressureSourceAdditionSIN();
  void TestSetup29SeriesPressureSourceAdditionPULSE();
  void TestSetup30SeriesCurrentSourceAdditionDC();
  void TestSetup30SeriesCurrentSourceAdditionSIN();
  void TestSetup30SeriesCurrentSourceAdditionPULSE();
  void TestSetup31ParallelPressureSourceAdditionDC();
  void TestSetup31ParallelPressureSourceAdditionSIN();
  void TestSetup31ParallelPressureSourceAdditionPULSE();
  void TestSetup32ParallelCurrentSourceAdditionDC();
  void TestSetup32ParallelCurrentSourceAdditionSIN();
  void TestSetup32ParallelCurrrentSourceAdditionPULSE();
  void TestSetup33SeriesCapDCCurrent();
  void TestSetup33SeriesCapSINCurrent();
  void TestSetup33SeriesCapPulseCurrent();
  void TestSetup34ParallelCapDC();
  void TestSetup34ParallelCapSIN();
  void TestSetup34ParallelCapPulse();
  void TestSetup35ParallelCapDCCurrent();
  void TestSetup35ParallelCapSINCurrent();
  void TestSetup35ParallelCapPulseCurrent();
  void TestSetup36SeriesIndDC();
  void TestSetup36SeriesIndSIN();
  void TestSetup36SeriesIndPulse();
  void TestSetup37SeriesIndDCCurrent();
  void TestSetup37SeriesIndSINCurrent();
  void TestSetup37SeriesIndPulseCurrent();
  void TestSetup38ParallelIndDC();
  void TestSetup38ParallelIndSIN();
  void TestSetup38ParallelIndPulse();
  void TestSetup39ParallelIndDCCurrent();
  void TestSetup39ParallelIndSINCurrent();
  void TestSetup39ParallelIndPulseCurrent();
  void TestSetup40BadDiodeDC();
  void TestSetup40BadDiodeSIN();
  void TestSetup40BadDiodePulse();
  void TestSetup41ValidationComprehensive1DC();
  void TestSetup41ValidationComprehensive1SIN();
  void TestSetup41ValidationComprehensive1Pulse();
  void TestSetup41ValidationComprehensive1SINCentered();
  void TestSetup42ValidationComprehensive2DC();
  void TestSetup42ValidationComprehensive2SIN();
  void TestSetup42ValidationComprehensive2Pulse();
  void TestSetup42ValidationComprehensive2SINCentered();

  void TestPreProcess1(double dT, int i);
  void TestPreProcess2(double dT, int i);
  void TestPreProcess3(double dT, int i);
  void TestPreProcess4(double dT, int i);
  void BasicCircuitPreProcess(double dT);
  // Advanced pre-process
  void FluidPreProcess(SEFluidCircuit& c, double dT);
  void TestCircuitSerialization(const std::string& fileName);

  SECircuitManager m_Circuits;

  //////////////////
  // Compartments //
  //////////////////
public:
  void GasCompartmentTest(const std::string& sOutputDirectory);
  void LiquidCompartmentTest(const std::string& sOutputDirectory);
  void ThermalCompartmentTest(const std::string& sOutputDirectory);
  void TissueCompartmentTest(
    const std::string& sOutputDirectory); // there is nothing being tested here

protected:
  void TestCompartmentSerialization(SECompartmentManager& mgr,
    const std::string& fileName);
  // Gas Compartments
  void TestGasFlows(SETestSuite& testSuite, SESubstanceManager& subMgr);
  void TestGasHierarchyFlows(SETestSuite& testSuite,
    SESubstanceManager& subMgr);
  void TestFlow(SETestCase& testCase, SEGasCompartment& cmpt,
    double inflow_mL_Per_s, double outflow_mL_Per_s);
  void TestGasHierarchy(SETestSuite& testSuite, SESubstanceManager& subMgr);
  void TestGasCircuitVolumesPressuresAndFlows(SETestSuite& testSuite,
    SESubstanceManager& subMgr);
  void CheckGasPressureAndVolume(SETestCase& testCase, SEGasCompartment& cmpt);
  void TestGasSubstanceQuantity(SETestCase& testCase, SEGasCompartment& cmpt,
    SEGasSubstanceQuantity& subQ,
    double totalSubVolume_mL,
    double totalVolume_mL);
  void TestUpdateGasLinks(SETestSuite& testSuite, SESubstanceManager& subMgr);
  // Liquid Compartments
  void TestLiquidFlows(SETestSuite& testSuite, SESubstanceManager& subMgr);
  void TestLiquidHierarchyFlows(SETestSuite& testSuite,
    SESubstanceManager& subMgr);
  void TestFlow(SETestCase& testCase, SELiquidCompartment& cmpt,
    double inflow_mL_Per_s, double outflow_mL_Per_s);
  void TestLiquidHierarchy(SETestSuite& testSuite, SESubstanceManager& subMgr);
  void TestLiquidCircuitVolumesPressuresAndFlows(SETestSuite& testSuite,
    SESubstanceManager& subMgr);
  void TestLiquidHierarchySaturation(SETestSuite& testSuite,
    SESubstanceManager& subMgr);
  void CheckLiquidPressureAndVolume(SETestCase& testCase,
    SELiquidCompartment& cmpt);
  void TestLiquidSubstanceQuantity(SETestCase& testCase,
    SELiquidCompartment& cmpt,
    SELiquidSubstanceQuantity& subQ,
    double totalMass_mg, double totalVolume_mL);
  void TestUpdateLiquidLinks(SETestSuite& testSuite,
    SESubstanceManager& subMgr);
  // Thermal Compartments
  void TestThermalFlows(SETestSuite& testSuite, SESubstanceManager& subMgr);
  void TestThermalFlowHierarchy(SETestSuite& testSuite,
    SESubstanceManager& subMgr);
  void TestFlow(SETestCase& testCase, SEThermalCompartment& cmpt,
    double inflow_kcal_Per_s, double outflow_kcal_Per_s);
  void TestThermalHierarchy(SETestSuite& testSuite, SESubstanceManager& subMgr);
  void TestCircuitHeatTemperatureAndFlows(SETestSuite& testSuite,
    SESubstanceManager& subMgr);
  void CheckTemperatureAndHeat(SETestCase& testCase,
    SEThermalCompartment& cmpt);

  ///////////////
  // Transport //
  ///////////////
public:
  void LargeFlowTransportTest(const std::string& sOutputDirectory);
  void LiquidTransportTest(const std::string& sOutputDirectory);
  void GasTransportTest(const std::string& sOutputDirectory);

  ///////////////
  // I/O Tests //
  ///////////////
public:
  void ReadPatientDirectory(const std::string& sOutputDirectory);
  void ReadScenarios(const std::string& sOutputDirectory);
  void ReadSubstanceDirectory(const std::string& sOutputDirectory);

  //////////////////////
  // Properties Tests //
  //////////////////////
public:
  void ScalarTest(const std::string& sOutputDirectory);
  void UnitsTest(const std::string& sOutputDirectory);

protected:
  void ConvertList(std::vector<std::string> stringList);
  // These property tests are not outputting any reports or anything, kind of
  // just a sandbox at this point Maybe one day I will set something up, but
  // these are generally not a problem (no jinx!)
};
}