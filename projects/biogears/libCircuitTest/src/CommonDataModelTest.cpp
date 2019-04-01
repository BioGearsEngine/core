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
#include <biogears/cdm/test/CommonDataModelTest.h>

#include <fstream>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/schema/cdm/Compartment.hxx>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/utils/FileUtils.h>

namespace biogears {
CommonDataModelTest::CommonDataModelTest()
  : Loggable(new Logger())
  , m_Circuits(m_Logger)
{
  myLogger = true;
  FillFunctionMap();
}

CommonDataModelTest::CommonDataModelTest(Logger* logger)
  : Loggable(logger)
  , m_Circuits(logger)
{
  myLogger = false;
  FillFunctionMap();
}

CommonDataModelTest::~CommonDataModelTest()
{
  if (myLogger)
    SAFE_DELETE(m_Logger);
}

bool CommonDataModelTest::RunTest(const std::string& testName, const std::string& sOutputDirectory)
{
  const std::string variant_name = testName + "Test"; 
  try {
    //If you find the test name, run it
    if (cdmMap.find(testName) != cdmMap.end()) {
      testFunction func = cdmMap.at(testName);
      (this->*func)(sOutputDirectory);
      return true;
    } else if (cdmMap.find(variant_name) != cdmMap.end()) {
        testFunction func = cdmMap.at(variant_name);
        (this->*func)(sOutputDirectory);
        return true;
    } else {
      m_ss << "Could not find " << testName << " or " << testName << "Test in CommonDataModelTest." << std::endl;
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

void CommonDataModelTest::FillFunctionMap()
{
  //Fill a map that ties unit test names to their actual functions
  cdmMap.insert(std::make_pair("BasicCircuitTest", &CommonDataModelTest::BasicCircuitTest));

  cdmMap.insert(std::make_pair("SeriesRCDCTest", &CommonDataModelTest::SeriesRCDCTest));
  cdmMap.insert(std::make_pair("SeriesRCSINTest", &CommonDataModelTest::SeriesRCSINTest));
  cdmMap.insert(std::make_pair("SeriesRCPULSETest", &CommonDataModelTest::SeriesRCPULSETest));

  cdmMap.insert(std::make_pair("SeriesRDCTest", &CommonDataModelTest::SeriesRDCTest));
  cdmMap.insert(std::make_pair("SeriesRSINTest", &CommonDataModelTest::SeriesRSINTest));
  cdmMap.insert(std::make_pair("SeriesRPULSETest", &CommonDataModelTest::SeriesRPULSETest));

  cdmMap.insert(std::make_pair("SeriesRLDCTest", &CommonDataModelTest::SeriesRLDCTest));
  cdmMap.insert(std::make_pair("SeriesRLSINTest", &CommonDataModelTest::SeriesRLSINTest));
  cdmMap.insert(std::make_pair("SeriesRLPULSETest", &CommonDataModelTest::SeriesRLPULSETest));

  cdmMap.insert(std::make_pair("ParallelRDCTest", &CommonDataModelTest::ParallelRDCTest));
  cdmMap.insert(std::make_pair("ParallelRSINTest", &CommonDataModelTest::ParallelRSINTest));
  cdmMap.insert(std::make_pair("ParallelRPULSETest", &CommonDataModelTest::ParallelRPULSETest));

  cdmMap.insert(std::make_pair("ParallelRCDCTest", &CommonDataModelTest::ParallelRCDCTest));
  cdmMap.insert(std::make_pair("ParallelRCSINTest", &CommonDataModelTest::ParallelRCSINTest));
  cdmMap.insert(std::make_pair("ParallelRCPULSETest", &CommonDataModelTest::ParallelRCPULSETest));

  cdmMap.insert(std::make_pair("ParallelRLDCTest", &CommonDataModelTest::ParallelRLDCTest));
  cdmMap.insert(std::make_pair("ParallelRLSINTest", &CommonDataModelTest::ParallelRLSINTest));
  cdmMap.insert(std::make_pair("ParallelRLPULSETest", &CommonDataModelTest::ParallelRLPULSETest));
  cdmMap.insert(std::make_pair("ParallelRLSINCenteredTest", &CommonDataModelTest::ParallelRLSINCenteredTest));

  cdmMap.insert(std::make_pair("SeriesRLCDCTest", &CommonDataModelTest::SeriesRLCDCTest));
  cdmMap.insert(std::make_pair("SeriesRLCSINTest", &CommonDataModelTest::SeriesRLCSINTest));
  cdmMap.insert(std::make_pair("SeriesRLCPULSETest", &CommonDataModelTest::SeriesRLCPULSETest));

  cdmMap.insert(std::make_pair("ParallelRLCDCTest", &CommonDataModelTest::ParallelRLCDCTest));
  cdmMap.insert(std::make_pair("ParallelRLCSINTest", &CommonDataModelTest::ParallelRLCSINTest));
  cdmMap.insert(std::make_pair("ParallelRLCPULSETest", &CommonDataModelTest::ParallelRLCPULSETest));

  cdmMap.insert(std::make_pair("SwitchRCDCTest", &CommonDataModelTest::SwitchRCDCTest));
  cdmMap.insert(std::make_pair("SwitchRCSINTest", &CommonDataModelTest::SwitchRCSINTest));
  cdmMap.insert(std::make_pair("SwitchRCPULSETest", &CommonDataModelTest::SwitchRCPULSETest));

  cdmMap.insert(std::make_pair("Comprehensive1DCTest", &CommonDataModelTest::Comprehensive1DCTest));
  cdmMap.insert(std::make_pair("Comprehensive1SINTest", &CommonDataModelTest::Comprehensive1SINTest));
  cdmMap.insert(std::make_pair("Comprehensive1PULSETest", &CommonDataModelTest::Comprehensive1PULSETest));

  cdmMap.insert(std::make_pair("SimpleDiodeDCTest", &CommonDataModelTest::SimpleDiodeDCTest));
  cdmMap.insert(std::make_pair("SimpleDiodeSINTest", &CommonDataModelTest::SimpleDiodeSINTest));
  cdmMap.insert(std::make_pair("SimpleDiodePULSETest", &CommonDataModelTest::SimpleDiodePULSETest));

  cdmMap.insert(std::make_pair("SeriesRCDCCurrentTest", &CommonDataModelTest::SeriesRCDCCurrentTest));
  cdmMap.insert(std::make_pair("SeriesRCSINCurrentTest", &CommonDataModelTest::SeriesRCSINCurrentTest));
  cdmMap.insert(std::make_pair("SeriesRCPULSECurrentTest", &CommonDataModelTest::SeriesRCPULSECurrentTest));

  cdmMap.insert(std::make_pair("ParallelRCDCCurrentTest", &CommonDataModelTest::ParallelRCDCCurrentTest));
  cdmMap.insert(std::make_pair("ParallelRCSINCurrentTest", &CommonDataModelTest::ParallelRCSINCurrentTest));
  cdmMap.insert(std::make_pair("ParallelRCPULSECurrentTest", &CommonDataModelTest::ParallelRCPULSECurrentTest));

  cdmMap.insert(std::make_pair("SeriesCapDCTest", &CommonDataModelTest::SeriesCapDCTest));
  cdmMap.insert(std::make_pair("SeriesCapSINTest", &CommonDataModelTest::SeriesCapSINTest));
  cdmMap.insert(std::make_pair("SeriesCapPULSETest", &CommonDataModelTest::SeriesCapPULSETest));

  cdmMap.insert(std::make_pair("ParallelRDCCurrentTest", &CommonDataModelTest::ParallelRDCCurrentTest));
  cdmMap.insert(std::make_pair("ParallelRSINCurrentTest", &CommonDataModelTest::ParallelRSINCurrentTest));
  cdmMap.insert(std::make_pair("ParallelRPULSECurrentTest", &CommonDataModelTest::ParallelRPULSECurrentTest));

  cdmMap.insert(std::make_pair("CurrentCompDCTest", &CommonDataModelTest::CurrentCompDCTest));
  cdmMap.insert(std::make_pair("CurrentCompSINTest", &CommonDataModelTest::CurrentCompSINTest));
  cdmMap.insert(std::make_pair("CurrentCompPULSETest", &CommonDataModelTest::CurrentCompPULSETest));

  cdmMap.insert(std::make_pair("SeriesRLDCCurrentTest", &CommonDataModelTest::SeriesRLDCCurrentTest));
  cdmMap.insert(std::make_pair("SeriesRLSINCurrentTest", &CommonDataModelTest::SeriesRLSINCurrentTest));
  cdmMap.insert(std::make_pair("SeriesRLPULSECurrentTest", &CommonDataModelTest::SeriesRLPULSECurrentTest));

  cdmMap.insert(std::make_pair("ParallelRLDCCurrentTest", &CommonDataModelTest::ParallelRLDCCurrentTest));
  cdmMap.insert(std::make_pair("ParallelRLSINCurrentTest", &CommonDataModelTest::ParallelRLSINCurrentTest));
  cdmMap.insert(std::make_pair("ParallelRLPULSECurrentTest", &CommonDataModelTest::ParallelRLPULSECurrentTest));

  cdmMap.insert(std::make_pair("BasicDiodeDCCurrentTest", &CommonDataModelTest::BasicDiodeDCCurrentTest));
  cdmMap.insert(std::make_pair("BasicDiodeSINCurrentTest", &CommonDataModelTest::BasicDiodeSINCurrentTest));
  cdmMap.insert(std::make_pair("BasicDiodePULSECurrentTest", &CommonDataModelTest::BasicDiodePULSECurrentTest));

  cdmMap.insert(std::make_pair("SwitchRCDCCurrentTest", &CommonDataModelTest::SwitchRCDCCurrentTest));
  cdmMap.insert(std::make_pair("SwitchRCSINCurrentTest", &CommonDataModelTest::SwitchRCSINCurrentTest));
  cdmMap.insert(std::make_pair("SwitchRCPULSECurrentTest", &CommonDataModelTest::SwitchRCPULSECurrentTest));

  cdmMap.insert(std::make_pair("SeriesRLCDCCurrentTest", &CommonDataModelTest::SeriesRLCDCCurrentTest));
  cdmMap.insert(std::make_pair("SeriesRLCSINCurrentTest", &CommonDataModelTest::SeriesRLCSINCurrentTest));
  cdmMap.insert(std::make_pair("SeriesRLCPULSECurrentTest", &CommonDataModelTest::SeriesRLCPULSECurrentTest));

  cdmMap.insert(std::make_pair("ParallelRLCDCCurrentTest", &CommonDataModelTest::ParallelRLCDCCurrentTest));
  cdmMap.insert(std::make_pair("ParallelRLCSINCurrentTest", &CommonDataModelTest::ParallelRLCSINCurrentTest));
  cdmMap.insert(std::make_pair("ParallelRLCPULSECurrentTest", &CommonDataModelTest::ParallelRLCPULSECurrentTest));

  cdmMap.insert(std::make_pair("SeriesPressureSourceAdditionDCTest", &CommonDataModelTest::SeriesPressureSourceAdditionDCTest));
  cdmMap.insert(std::make_pair("SeriesPressureSourceAdditionSINTest", &CommonDataModelTest::SeriesPressureSourceAdditionSINTest));
  cdmMap.insert(std::make_pair("SeriesPressureSourceAdditionPULSETest", &CommonDataModelTest::SeriesPressureSourceAdditionPULSETest));

  cdmMap.insert(std::make_pair("SeriesCurrentSourceAdditionDCTest", &CommonDataModelTest::SeriesCurrentSourceAdditionDCTest));
  cdmMap.insert(std::make_pair("SeriesCurrentSourceAdditionSINTest", &CommonDataModelTest::SeriesCurrentSourceAdditionSINTest));
  cdmMap.insert(std::make_pair("SeriesCurrentSourceAdditionPULSETest", &CommonDataModelTest::SeriesCurrentSourceAdditionPULSETest));

  cdmMap.insert(std::make_pair("ParallelPressureSourceAdditionDCTest", &CommonDataModelTest::ParallelPressureSourceAdditionDCTest));
  cdmMap.insert(std::make_pair("ParallelPressureSourceAdditionSINTest", &CommonDataModelTest::ParallelPressureSourceAdditionSINTest));
  cdmMap.insert(std::make_pair("ParallelPressureSourceAdditionPULSETest", &CommonDataModelTest::ParallelPressureSourceAdditionPULSETest));

  cdmMap.insert(std::make_pair("ParallelCurrentSourceAdditionDCTest", &CommonDataModelTest::ParallelCurrentSourceAdditionDCTest));
  cdmMap.insert(std::make_pair("ParallelCurrentSourceAdditionSINTest", &CommonDataModelTest::ParallelCurrentSourceAdditionSINTest));
  cdmMap.insert(std::make_pair("ParallelCurrentSourceAdditionPULSETest", &CommonDataModelTest::ParallelCurrentSourceAdditionPULSETest));

  cdmMap.insert(std::make_pair("SeriesCapDCCurrentTest", &CommonDataModelTest::SeriesCapDCCurrentTest));
  cdmMap.insert(std::make_pair("SeriesCapSINCurrentTest", &CommonDataModelTest::SeriesCapSINCurrentTest));
  cdmMap.insert(std::make_pair("SeriesCapPULSECurrentTest", &CommonDataModelTest::SeriesCapPULSECurrentTest));

  cdmMap.insert(std::make_pair("ParallelCapDCTest", &CommonDataModelTest::ParallelCapDCTest));
  cdmMap.insert(std::make_pair("ParallelCapSINTest", &CommonDataModelTest::ParallelCapSINTest));
  cdmMap.insert(std::make_pair("ParallelCapPULSETest", &CommonDataModelTest::ParallelCapPULSETest));

  cdmMap.insert(std::make_pair("ParallelCapDCCurrentTest", &CommonDataModelTest::ParallelCapDCCurrentTest));
  cdmMap.insert(std::make_pair("ParallelCapSINCurrentTest", &CommonDataModelTest::ParallelCapSINCurrentTest));
  cdmMap.insert(std::make_pair("ParallelCapPULSECurrentTest", &CommonDataModelTest::ParallelCapPULSECurrentTest));

  cdmMap.insert(std::make_pair("SeriesIndDCTest", &CommonDataModelTest::SeriesIndDCTest));
  cdmMap.insert(std::make_pair("SeriesIndSINTest", &CommonDataModelTest::SeriesIndSINTest));
  cdmMap.insert(std::make_pair("SeriesIndPULSETest", &CommonDataModelTest::SeriesIndPULSETest));

  cdmMap.insert(std::make_pair("SeriesIndDCCurrentTest", &CommonDataModelTest::SeriesIndDCCurrentTest));
  cdmMap.insert(std::make_pair("SeriesIndSINCurrentTest", &CommonDataModelTest::SeriesIndSINCurrentTest));
  cdmMap.insert(std::make_pair("SeriesIndPULSECurrentTest", &CommonDataModelTest::SeriesIndPULSECurrentTest));

  cdmMap.insert(std::make_pair("ParallelIndDCTest", &CommonDataModelTest::ParallelIndDCTest));
  cdmMap.insert(std::make_pair("ParallelIndSINTest", &CommonDataModelTest::ParallelIndSINTest));
  cdmMap.insert(std::make_pair("ParallelIndPULSETest", &CommonDataModelTest::ParallelIndPULSETest));

  cdmMap.insert(std::make_pair("ParallelIndDCCurrentTest", &CommonDataModelTest::ParallelIndDCCurrentTest));
  cdmMap.insert(std::make_pair("ParallelIndSINCurrentTest", &CommonDataModelTest::ParallelIndSINCurrentTest));
  cdmMap.insert(std::make_pair("ParallelIndPULSECurrentTest", &CommonDataModelTest::ParallelIndPULSECurrentTest));

  cdmMap.insert(std::make_pair("BadDiodeDCTest", &CommonDataModelTest::BadDiodeDCTest));
  cdmMap.insert(std::make_pair("BadDiodeSINTest", &CommonDataModelTest::BadDiodeSINTest));
  cdmMap.insert(std::make_pair("BadDiodePULSETest", &CommonDataModelTest::BadDiodePULSETest));

  cdmMap.insert(std::make_pair("ValidationComprehensive1DCTest", &CommonDataModelTest::ValidationComprehensive1DCTest));
  cdmMap.insert(std::make_pair("ValidationComprehensive1SINTest", &CommonDataModelTest::ValidationComprehensive1SINTest));
  cdmMap.insert(std::make_pair("ValidationComprehensive1PULSETest", &CommonDataModelTest::ValidationComprehensive1PULSETest));
  cdmMap.insert(std::make_pair("ValidationComprehensive1SINCenteredTest", &CommonDataModelTest::ValidationComprehensive1SINCenteredTest));

  cdmMap.insert(std::make_pair("ValidationComprehensive2DCTest", &CommonDataModelTest::ValidationComprehensive2DCTest));
  cdmMap.insert(std::make_pair("ValidationComprehensive2SINTest", &CommonDataModelTest::ValidationComprehensive2SINTest));
  cdmMap.insert(std::make_pair("ValidationComprehensive2PULSETest", &CommonDataModelTest::ValidationComprehensive2PULSETest));
  cdmMap.insert(std::make_pair("ValidationComprehensive2SINCenteredTest", &CommonDataModelTest::ValidationComprehensive2SINCenteredTest));

  cdmMap.insert(std::make_pair("ElectricalCircuitTest", &CommonDataModelTest::ElectricalCircuitTest));
  cdmMap.insert(std::make_pair("FluidCircuitTest", &CommonDataModelTest::FluidCircuitTest));
  cdmMap.insert(std::make_pair("ThermalCircuitTest", &CommonDataModelTest::ThermalCircuitTest));
  cdmMap.insert(std::make_pair("CombinedCircuitTest", &CommonDataModelTest::CombinedCircuitTest));
  cdmMap.insert(std::make_pair("InterCircuitComparisonTest", &CommonDataModelTest::InterCircuitComparisonTest));
  cdmMap.insert(std::make_pair("InterCircuitIndividualTest", &CommonDataModelTest::InterCircuitIndividualTest));
  cdmMap.insert(std::make_pair("DynamicallyChangingCircuitTest", &CommonDataModelTest::DynamicallyChangingCircuitTest));
  cdmMap.insert(std::make_pair("NonZeroReferencePositive", &CommonDataModelTest::NonZeroReferencePositive));
  cdmMap.insert(std::make_pair("NonZeroReferenceNegative", &CommonDataModelTest::NonZeroReferenceNegative));
  cdmMap.insert(std::make_pair("PolarizedCapacitorTest", &CommonDataModelTest::PolarizedCapacitorTest));
  cdmMap.insert(std::make_pair("PreChargeComplianceZeroVolume", &CommonDataModelTest::PreChargeComplianceZeroVolume));
  cdmMap.insert(std::make_pair("PreChargeComplianceNonZeroVolume", &CommonDataModelTest::PreChargeComplianceNonZeroVolume));
  cdmMap.insert(std::make_pair("CircuitErrorTest", &CommonDataModelTest::CircuitErrorTest));
  cdmMap.insert(std::make_pair("CircuitLockingTest", &CommonDataModelTest::CircuitLockingTest));

  cdmMap.insert(std::make_pair("GasCompartmentTest", &CommonDataModelTest::GasCompartmentTest));
  cdmMap.insert(std::make_pair("LiquidCompartmentTest", &CommonDataModelTest::LiquidCompartmentTest));
  cdmMap.insert(std::make_pair("ThermalCompartmentTest", &CommonDataModelTest::ThermalCompartmentTest));
  cdmMap.insert(std::make_pair("TissueCompartmentTest", &CommonDataModelTest::TissueCompartmentTest));

  cdmMap.insert(std::make_pair("LargeFlowTransportTest", &CommonDataModelTest::LargeFlowTransportTest));
  cdmMap.insert(std::make_pair("LiquidTransportTest", &CommonDataModelTest::LiquidTransportTest));
  cdmMap.insert(std::make_pair("GasTransportTest", &CommonDataModelTest::GasTransportTest));

  cdmMap.insert(std::make_pair("ReadPatientDirectory", &CommonDataModelTest::ReadPatientDirectory));
  cdmMap.insert(std::make_pair("ReadScenarios", &CommonDataModelTest::ReadScenarios));
  cdmMap.insert(std::make_pair("ReadSubstanceDirectory", &CommonDataModelTest::ReadSubstanceDirectory));

  cdmMap.insert(std::make_pair("ScalarTest", &CommonDataModelTest::ScalarTest));
  cdmMap.insert(std::make_pair("UnitsTest", &CommonDataModelTest::UnitsTest));
}

void CommonDataModelTest::TestCompartmentSerialization(SECompartmentManager& mgr, const std::string& fileName)
{
  ScopedFileSystemLock lock;

  std::ofstream stream(ResolvePath(fileName));
  xml_schema::namespace_infomap map;
  map[""].name = "uri:/mil/tatrc/physiology/datamodel";

  CDM::CompartmentManager(stream, dynamic_cast<CDM::CompartmentManagerData&>(*mgr.Unload()), map);
  stream.close();
  std::unique_ptr<CDM::ObjectData> bind = Serializer::ReadFile(fileName, m_Logger);
  CDM::CompartmentManagerData* data = dynamic_cast<CDM::CompartmentManagerData*>(bind.get());
  if (data != nullptr) {
    if (!mgr.Load(*data, &m_Circuits))
      Error("Could not load Compartment Manager Data");
  } else {
    Error("Could not cast loaded Compartment Manager Data");
  }
}
}
