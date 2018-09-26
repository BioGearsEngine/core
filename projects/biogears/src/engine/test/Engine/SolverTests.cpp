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
#include <numeric>
//Project Includes
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/utils/TimingProfile.h>
#include <biogears/cdm/utils/testing/SETestCase.h>
#include <biogears/cdm/utils/testing/SETestReport.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>
#include <biogears/engine/test/BioGearsEngineTest.h>

namespace biogears {
std::chrono::microseconds::rep vectorAverage(std::vector<std::chrono::microseconds::rep> const& v)
{
  if (v.size() > 0) {
    std::chrono::microseconds::rep sum = 0;
    for (auto f : v)
      sum += f;
    return sum / v.size();
  } else
    return INT_MAX;
}

void BioGearsEngineTest::SolverSpeedTest(const std::string& rptDirectory)
{
  m_Logger->ResetLogFile(rptDirectory + "/SolverSpeedTest.log");

  // Set up our test report
  SETestReport testReport = SETestReport(m_Logger);
  SETestSuite& tsSolverSpeed = testReport.CreateTestSuite();
  tsSolverSpeed.SetName("SolverSpeedTest");

  std::stringstream ss;
  TimingProfile timer;
  double deltaT_s = 1.0 / 90.0;
  bool showAllOutput = true; //toggle this to show all Info outputs for all circuits, which will show first-pass solve times and fail rates

  BioGears bg(tsSolverSpeed.GetLogger());
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::On);
  bg.m_Config->EnableTissue(CDM::enumOnOff::On);
  bg.CreateCircuitsAndCompartments();

  SEFluidCircuitCalculator fluidCalc(tsSolverSpeed.GetLogger());
  SEThermalCircuitCalculator thermalCalc(tsSolverSpeed.GetLogger());

  //Hold the average solving times for each solver
  std::vector<std::chrono::microseconds::rep> anesthesiaMachineTimes;
  std::vector<std::chrono::microseconds::rep> anesthesiaMachineWithRespTimes;
  std::vector<std::chrono::microseconds::rep> respiratoryTimes;
  std::vector<std::chrono::microseconds::rep> respiratoryWithInhalerTimes;
  std::vector<std::chrono::microseconds::rep> cardiovascularTimes;
  std::vector<std::chrono::microseconds::rep> renalTimes;
  std::vector<std::chrono::microseconds::rep> internalTempTimes;
  std::vector<std::chrono::microseconds::rep> externalTempTimes;
  std::vector<std::chrono::microseconds::rep> temperatureTimes;

  //Hold the solving time of Process the first time for each solver
  std::vector<std::chrono::microseconds::rep> anesthesiaMachineInitialTime;
  std::vector<std::chrono::microseconds::rep> anesthesiaMachineWithRespInitialTime;
  std::vector<std::chrono::microseconds::rep> respiratoryInitialTime;
  std::vector<std::chrono::microseconds::rep> respiratoryWithInhalerInitialTime;
  std::vector<std::chrono::microseconds::rep> cardiovascularInitialTime;
  std::vector<std::chrono::microseconds::rep> renalInitialTime;
  std::vector<std::chrono::microseconds::rep> internalTempInitialTime;
  std::vector<std::chrono::microseconds::rep> externalTempInitialTime;
  std::vector<std::chrono::microseconds::rep> temperatureInitialTime;

  //Hold the failure percentage for each solver
  std::vector<std::chrono::microseconds::rep> anesthesiaMachineFailureRate;
  std::vector<std::chrono::microseconds::rep> anesthesiaMachineWithRespFailureRate;
  std::vector<std::chrono::microseconds::rep> respiratoryFailureRate;
  std::vector<std::chrono::microseconds::rep> respiratoryWithInhalerFailureRate;
  std::vector<std::chrono::microseconds::rep> cardiovascularFailureRate;
  std::vector<std::chrono::microseconds::rep> renalFailureRate;
  std::vector<std::chrono::microseconds::rep> internalTempFailureRate;
  std::vector<std::chrono::microseconds::rep> externalTempFailureRate;
  std::vector<std::chrono::microseconds::rep> temperatureFailureRate;

  int numSolves = 50; //How many times we'll solve the circuit (not counting the initial time) to get our averages

  //For each of the solver types
  //Direct, PartialPivLu, FullPivLu, JacobiSvd, HouseholderQr, Ldlt, Llt, SparseLU, SparseQR, BiCGSTAB, ConjugateGradient
  //Test each circuit and record solving times
  EigenCircuitSolver solver;
  for (int i = 0; i < EigenCircuitSolver::_size; i++, solver++) {
    bool failed = false;

    //Anesthesia Machine alone
    SEFluidCircuit* fCircuit = &bg.GetCircuits().GetAnesthesiaMachineCircuit();

    //Set the solver type
    fluidCalc.SetEigenSolver(solver);

    int numFails = 0;

    timer.Start("AnesthesiaMachineSolo");
    try {
      fluidCalc.Process(*fCircuit, deltaT_s);
    } catch (CommonDataModelException& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (...) {
      std::cerr << "Exception!!" << std::endl;
      failed = true;
    }
    timer.Stop("AnesthesiaMachineSolo");
    if (!failed) {
      ss << "Anesthesia circuit initially solved in " << timer.GetElapsedTime<std::chrono::microseconds>("AnesthesiaMachineSolo") << " microseconds, ";
      anesthesiaMachineInitialTime.push_back(timer.GetElapsedTime<std::chrono::microseconds>("AnesthesiaMachineSolo"));
    } else {
      ss << "Anesthesia circuit failed initially, ";
      anesthesiaMachineInitialTime.push_back(INT_MAX);
    }

    std::vector<std::chrono::microseconds::rep> times;
    for (int i = 0; i < numSolves; i++) {
      failed = false;

      timer.Start("AnesthesiaMachineSolo");
      try {
        fluidCalc.Process(*fCircuit, deltaT_s);
      } catch (CommonDataModelException& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (...) {
        std::cerr << "Exception!!" << std::endl;
        failed = true;
      }
      timer.Stop("AnesthesiaMachineSolo");

      if (failed)
        numFails++;
      else
        times.push_back(timer.GetElapsedTime<std::chrono::microseconds>("AnesthesiaMachineSolo"));
    }

    anesthesiaMachineTimes.push_back(vectorAverage(times));
    anesthesiaMachineFailureRate.push_back(100 * numFails / numSolves);
    ss << "and over " << numSolves - numFails << "/" << numSolves << " successful solves, took an average of " << anesthesiaMachineTimes.back() << " microseconds with " << solver.string() << " solver.";
    if (showAllOutput)
      Info(ss);

    //Anesthesia Machine with Respiratory
    fCircuit = &bg.GetCircuits().GetRespiratoryAndAnesthesiaMachineCircuit();
    numFails = 0;

    timer.Start("AnesthesiaMachineWithRespiratory");
    try {
      fluidCalc.Process(*fCircuit, deltaT_s);
    } catch (CommonDataModelException& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (...) {
      std::cerr << "Exception!!" << std::endl;
      failed = true;
    }
    timer.Stop("AnesthesiaMachineWithRespiratory");
    if (!failed) {
      ss << "Anesthesia circuit with respiratory initially solved in " << timer.GetElapsedTime<std::chrono::microseconds>("AnesthesiaMachineWithRespiratory") << " microseconds, ";
      anesthesiaMachineWithRespInitialTime.push_back(timer.GetElapsedTime<std::chrono::microseconds>("AnesthesiaMachineWithRespiratory"));
    } else {
      ss << "Anesthesia circuit with respiratory failed initially, ";
      anesthesiaMachineWithRespInitialTime.push_back(INT_MAX);
    }

    times.clear();
    for (int i = 0; i < numSolves; i++) {
      failed = false;

      timer.Start("AnesthesiaMachineWithRespiratory");
      try {
        fluidCalc.Process(*fCircuit, deltaT_s);
      } catch (CommonDataModelException& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (...) {
        std::cerr << "Exception!!" << std::endl;
        failed = true;
      }
      timer.Stop("AnesthesiaMachineWithRespiratory");

      if (failed)
        numFails++;
      else
        times.push_back(timer.GetElapsedTime<std::chrono::microseconds>("AnesthesiaMachineWithRespiratory"));
    }

    anesthesiaMachineWithRespTimes.push_back(vectorAverage(times));
    anesthesiaMachineWithRespFailureRate.push_back(100 * numFails / numSolves);
    ss << "and over " << numSolves - numFails << "/" << numSolves << " successful solves, took an average of " << anesthesiaMachineWithRespTimes.back() << " microseconds with " << solver.string() << " solver.";
    if (showAllOutput)
      Info(ss);

    //Respiratory Only
    fCircuit = &bg.GetCircuits().GetRespiratoryCircuit();

    numFails = 0;

    timer.Start("Respiratory");
    try {
      fluidCalc.Process(*fCircuit, deltaT_s);
    } catch (CommonDataModelException& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (...) {
      std::cerr << "Exception!!" << std::endl;
      failed = true;
    }
    timer.Stop("Respiratory");
    if (!failed) {
      ss << "Respiratory circuit initially solved in " << timer.GetElapsedTime<std::chrono::microseconds>("Respiratory") << " microseconds, ";
      respiratoryInitialTime.push_back(timer.GetElapsedTime<std::chrono::microseconds>("Respiratory"));
    } else {
      ss << "Respiratory circuit failed initially, ";
      respiratoryInitialTime.push_back(INT_MAX);
    }

    times.clear();
    for (int i = 0; i < numSolves; i++) {
      failed = false;

      timer.Start("Respiratory");
      try {
        fluidCalc.Process(*fCircuit, deltaT_s);
      } catch (CommonDataModelException& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (...) {
        std::cerr << "Exception!!" << std::endl;
        failed = true;
      }
      timer.Stop("Respiratory");

      if (failed)
        numFails++;
      else
        times.push_back(timer.GetElapsedTime<std::chrono::microseconds>("Respiratory"));
    }

    respiratoryTimes.push_back(vectorAverage(times));
    respiratoryFailureRate.push_back(100 * numFails / numSolves);
    ss << "and over " << numSolves - numFails << "/" << numSolves << " successful solves, took an average of " << respiratoryTimes.back() << " microseconds with " << solver.string() << " solver.";
    if (showAllOutput)
      Info(ss);

    //Respiratory with Inhaler
    fCircuit = &bg.GetCircuits().GetRespiratoryAndInhalerCircuit();

    numFails = 0;

    timer.Start("RespiratoryWithInhaler");
    try {
      fluidCalc.Process(*fCircuit, deltaT_s);
    } catch (CommonDataModelException& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (...) {
      std::cerr << "Exception!!" << std::endl;
      failed = true;
    }
    timer.Stop("RespiratoryWithInhaler");
    if (!failed) {
      ss << "Respiratory circuit with inhaler initially solved in " << timer.GetElapsedTime<std::chrono::microseconds>("RespiratoryWithInhaler") << " microseconds, ";
      respiratoryWithInhalerInitialTime.push_back(timer.GetElapsedTime<std::chrono::microseconds>("RespiratoryWithInhaler"));
    } else {
      ss << "Respiratory circuit with inhaler failed initially, ";
      respiratoryWithInhalerInitialTime.push_back(INT_MAX);
    }

    times.clear();
    for (int i = 0; i < numSolves; i++) {
      failed = false;

      timer.Start("RespiratoryWithInhaler");
      try {
        fluidCalc.Process(*fCircuit, deltaT_s);
      } catch (CommonDataModelException& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (...) {
        std::cerr << "Exception!!" << std::endl;
        failed = true;
      }
      timer.Stop("RespiratoryWithInhaler");

      if (failed)
        numFails++;
      else
        times.push_back(timer.GetElapsedTime<std::chrono::microseconds>("RespiratoryWithInhaler"));
    }

    respiratoryWithInhalerTimes.push_back(vectorAverage(times));
    respiratoryWithInhalerFailureRate.push_back(100 * numFails / numSolves);
    ss << "and over " << numSolves - numFails << "/" << numSolves << " successful solves, took an average of " << respiratoryWithInhalerTimes.back() << " microseconds with " << solver.string() << " solver.";
    if (showAllOutput)
      Info(ss);

    //Cardiovascular
    fCircuit = &bg.GetCircuits().GetActiveCardiovascularCircuit();
    numFails = 0;

    timer.Start("Cardiovascular");
    try {
      fluidCalc.Process(*fCircuit, deltaT_s);
    } catch (CommonDataModelException& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (...) {
      std::cerr << "Exception!!" << std::endl;
      failed = true;
    }
    timer.Stop("Cardiovascular");
    if (!failed) {
      ss << "Active CV circuit initially solved in " << timer.GetElapsedTime<std::chrono::microseconds>("Cardiovascular") << " microseconds, ";
      cardiovascularInitialTime.push_back(timer.GetElapsedTime<std::chrono::microseconds>("Cardiovascular"));
    } else {
      ss << "Active CV circuit failed initially, ";
      cardiovascularInitialTime.push_back(INT_MAX);
    }

    times.clear();
    for (int i = 0; i < numSolves; i++) {
      failed = false;

      timer.Start("Cardiovascular");
      try {
        fluidCalc.Process(*fCircuit, deltaT_s);
      } catch (CommonDataModelException& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (...) {
        std::cerr << "Exception!!" << std::endl;
        failed = true;
      }
      timer.Stop("Cardiovascular");

      if (failed)
        numFails++;
      else
        times.push_back(timer.GetElapsedTime<std::chrono::microseconds>("Cardiovascular"));
    }

    cardiovascularTimes.push_back(vectorAverage(times));
    cardiovascularFailureRate.push_back(100 * numFails / numSolves);
    ss << "and over " << numSolves - numFails << "/" << numSolves << " successful solves, took an average of " << cardiovascularTimes.back() << " microseconds with " << solver.string() << " solver.";
    if (showAllOutput)
      Info(ss);

    //Renal
    fCircuit = &bg.GetCircuits().GetRenalCircuit();
    numFails = 0;

    timer.Start("Renal");
    try {
      fluidCalc.Process(*fCircuit, deltaT_s);
    } catch (CommonDataModelException& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (...) {
      std::cerr << "Exception!!" << std::endl;
      failed = true;
    }
    timer.Stop("Renal");
    if (!failed) {
      ss << "Renal circuit initially solved in " << timer.GetElapsedTime<std::chrono::microseconds>("Renal") << " microseconds, ";
      renalInitialTime.push_back(timer.GetElapsedTime<std::chrono::microseconds>("Renal"));
    } else {
      ss << "Renal circuit failed initially, ";
      renalInitialTime.push_back(INT_MAX);
    }

    times.clear();
    for (int i = 0; i < numSolves; i++) {
      failed = false;

      timer.Start("Renal");
      try {
        fluidCalc.Process(*fCircuit, deltaT_s);
      } catch (CommonDataModelException& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (...) {
        std::cerr << "Exception!!" << std::endl;
        failed = true;
      }
      timer.Stop("Renal");

      if (failed)
        numFails++;
      else
        times.push_back(timer.GetElapsedTime<std::chrono::microseconds>("Renal"));
    }

    renalTimes.push_back(vectorAverage(times));
    renalFailureRate.push_back(100 * numFails / numSolves);
    ss << "and over " << numSolves - numFails << "/" << numSolves << " successful solves, took an average of " << renalTimes.back() << " microseconds with " << solver.string() << " solver.";
    if (showAllOutput)
      Info(ss);

    //Internal Temp
    SEThermalCircuit* tCircuit = &bg.GetCircuits().GetInternalTemperatureCircuit();
    thermalCalc.SetEigenSolver(solver);
    numFails = 0;

    timer.Start("InternalTemperature");
    try {
      thermalCalc.Process(*tCircuit, deltaT_s);
    } catch (CommonDataModelException& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (...) {
      std::cerr << "Exception!!" << std::endl;
      failed = true;
    }
    timer.Stop("InternalTemperature");
    if (!failed) {
      ss << "Internal Temperature circuit initially solved in " << timer.GetElapsedTime<std::chrono::microseconds>("InternalTemperature") << " microseconds, ";
      internalTempInitialTime.push_back(timer.GetElapsedTime<std::chrono::microseconds>("InternalTemperature"));
    } else {
      ss << "Internal Temperature circuit failed initially, ";
      internalTempInitialTime.push_back(INT_MAX);
    }

    times.clear();
    for (int i = 0; i < numSolves; i++) {
      failed = false;

      timer.Start("InternalTemperature");
      try {
        thermalCalc.Process(*tCircuit, deltaT_s);
      } catch (CommonDataModelException& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (...) {
        std::cerr << "Exception!!" << std::endl;
        failed = true;
      }
      timer.Stop("InternalTemperature");

      if (failed)
        numFails++;
      else
        times.push_back(timer.GetElapsedTime<std::chrono::microseconds>("InternalTemperature"));
    }

    internalTempTimes.push_back(vectorAverage(times));
    internalTempFailureRate.push_back(100 * numFails / numSolves);
    ss << "and over " << numSolves - numFails << "/" << numSolves << " successful solves, took an average of " << internalTempTimes.back() << " microseconds with " << solver.string() << " solver.";
    if (showAllOutput)
      Info(ss);

    //External Temp
    tCircuit = &bg.GetCircuits().GetExternalTemperatureCircuit();
    numFails = 0;

    timer.Start("ExternalTemperature");
    try {
      thermalCalc.Process(*tCircuit, deltaT_s);
    } catch (CommonDataModelException& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (...) {
      std::cerr << "Exception!!" << std::endl;
      failed = true;
    }
    timer.Stop("ExternalTemperature");
    if (!failed) {
      ss << "External Temperature circuit initially solved in " << timer.GetElapsedTime<std::chrono::microseconds>("ExternalTemperature") << " microseconds, ";
      externalTempInitialTime.push_back(timer.GetElapsedTime<std::chrono::microseconds>("ExternalTemperature"));
    } else {
      ss << "External Temperature circuit failed initially, ";
      externalTempInitialTime.push_back(INT_MAX);
    }

    times.clear();
    for (int i = 0; i < numSolves; i++) {
      failed = false;

      timer.Start("ExternalTemperature");
      try {
        thermalCalc.Process(*tCircuit, deltaT_s);
      } catch (CommonDataModelException& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (...) {
        std::cerr << "Exception!!" << std::endl;
        failed = true;
      }
      timer.Stop("ExternalTemperature");

      if (failed)
        numFails++;
      else
        times.push_back(timer.GetElapsedTime<std::chrono::microseconds>("ExternalTemperature"));
    }

    externalTempTimes.push_back(vectorAverage(times));
    externalTempFailureRate.push_back(100 * numFails / numSolves);
    ss << "and over " << numSolves - numFails << "/" << numSolves << " successful solves, took an average of " << externalTempTimes.back() << " microseconds with " << solver.string() << " solver.";
    if (showAllOutput)
      Info(ss);

    //Temperature
    tCircuit = &bg.GetCircuits().GetTemperatureCircuit();
    numFails = 0;

    timer.Start("Temperature");
    try {
      thermalCalc.Process(*tCircuit, deltaT_s);
    } catch (CommonDataModelException& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      failed = true;
    } catch (...) {
      std::cerr << "Exception!!" << std::endl;
      failed = true;
    }
    timer.Stop("Temperature");
    if (!failed) {
      ss << "Temperature circuit initially solved in " << timer.GetElapsedTime<std::chrono::microseconds>("Temperature") << " microseconds, ";
      temperatureInitialTime.push_back(timer.GetElapsedTime<std::chrono::microseconds>("Temperature"));
    } else {
      ss << "Temperature circuit failed initially, ";
      temperatureInitialTime.push_back(INT_MAX);
    }

    times.clear();
    for (int i = 0; i < numSolves; i++) {
      failed = false;

      timer.Start("Temperature");
      try {
        thermalCalc.Process(*tCircuit, deltaT_s);
      } catch (CommonDataModelException& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        failed = true;
      } catch (...) {
        std::cerr << "Exception!!" << std::endl;
        failed = true;
      }
      timer.Stop("Temperature");

      if (failed)
        numFails++;
      else
        times.push_back(timer.GetElapsedTime<std::chrono::microseconds>("Temperature"));
    }

    temperatureTimes.push_back(vectorAverage(times));
    temperatureFailureRate.push_back(100 * numFails / numSolves);
    ss << "and over " << numSolves - numFails << "/" << numSolves << " successful solves, took an average of " << temperatureTimes.back() << " microseconds with " << solver.string() << " solver.";
    if (showAllOutput)
      Info(ss);
  }

  //Now find the quickest time for each circuit
  int minAnesthesiaMachineIndex, minAMWithRespIndex, minRespiratoryIndex, minRespWithInhalerIndex, minCVIndex, minRenalIndex, minInternalIndex, minExternalIndex, minTempIndex;
  minAnesthesiaMachineIndex = minAMWithRespIndex = minRespiratoryIndex = minRespWithInhalerIndex = minCVIndex = minRenalIndex = minInternalIndex = minExternalIndex = minTempIndex = 0;

  for (int i = 0; i < EigenCircuitSolver::_size; i++) {
    minAnesthesiaMachineIndex = anesthesiaMachineTimes.at(i) < anesthesiaMachineTimes.at(minAnesthesiaMachineIndex) ? i : minAnesthesiaMachineIndex;
    minAMWithRespIndex = anesthesiaMachineWithRespTimes.at(i) < anesthesiaMachineWithRespTimes.at(minAMWithRespIndex) ? i : minAMWithRespIndex;
    minRespiratoryIndex = respiratoryTimes.at(i) < respiratoryTimes.at(minRespiratoryIndex) ? i : minRespiratoryIndex;
    minRespWithInhalerIndex = respiratoryWithInhalerTimes.at(i) < respiratoryWithInhalerTimes.at(minRespWithInhalerIndex) ? i : minRespWithInhalerIndex;
    minCVIndex = cardiovascularTimes.at(i) < cardiovascularTimes.at(minCVIndex) ? i : minCVIndex;
    minRenalIndex = renalTimes.at(i) < renalTimes.at(minRenalIndex) ? i : minRenalIndex;
    minInternalIndex = internalTempTimes.at(i) < internalTempTimes.at(minInternalIndex) ? i : minInternalIndex;
    minExternalIndex = externalTempTimes.at(i) < externalTempTimes.at(minExternalIndex) ? i : minExternalIndex;
    minTempIndex = temperatureTimes.at(i) < temperatureTimes.at(minTempIndex) ? i : minTempIndex;
  }

  //Output
  //This just goes to the log for now
  ss.str("");
  ss.clear();
  ss << "The quickest solver for the Anesthesia Machine circuit was " << EigenCircuitSolver::Value(minAnesthesiaMachineIndex) << " at an average of " << anesthesiaMachineTimes.at(minAnesthesiaMachineIndex) << " microseconds and a failure rate of " << anesthesiaMachineFailureRate.at(minAnesthesiaMachineIndex) << "%.";
  Info(ss);
  ss << "The quickest solver for the Anesthesia Machine plus Respiratory circuit was " << EigenCircuitSolver::Value(minAMWithRespIndex) << " at an average of " << anesthesiaMachineWithRespTimes.at(minAMWithRespIndex) << " microseconds and a failure rate of " << anesthesiaMachineWithRespFailureRate.at(minAMWithRespIndex) << "%.";
  Info(ss);
  ss << "The quickest solver for the Respiratory circuit was " << EigenCircuitSolver::Value(minRespiratoryIndex) << " at an average of " << respiratoryTimes.at(minRespiratoryIndex) << " microseconds and a failure rate of " << respiratoryFailureRate.at(minRespiratoryIndex) << "%.";
  Info(ss);
  ss << "The quickest solver for the Respiratory plus Inhaler circuit was " << EigenCircuitSolver::Value(minRespWithInhalerIndex) << " at an average of " << respiratoryWithInhalerTimes.at(minRespWithInhalerIndex) << " microseconds and a failure rate of " << respiratoryWithInhalerFailureRate.at(minRespWithInhalerIndex) << "%.";
  Info(ss);
  ss << "The quickest solver for the active CV circuit was " << EigenCircuitSolver::Value(minCVIndex) << " at an average of " << cardiovascularTimes.at(minCVIndex) << " microseconds and a failure rate of " << cardiovascularFailureRate.at(minCVIndex) << "%.";
  Info(ss);
  ss << "The quickest solver for the Renal circuit was " << EigenCircuitSolver::Value(minRenalIndex) << " at an average of " << renalTimes.at(minRenalIndex) << " microseconds and a failure rate of " << renalFailureRate.at(minRenalIndex) << "%.";
  Info(ss);
  ss << "The quickest solver for the Internal Temperature circuit was " << EigenCircuitSolver::Value(minInternalIndex) << " at an average of " << internalTempTimes.at(minInternalIndex) << " microseconds and a failure rate of " << internalTempFailureRate.at(minInternalIndex) << "%.";
  Info(ss);
  ss << "The quickest solver for the External Temperature circuit was " << EigenCircuitSolver::Value(minExternalIndex) << " at an average of " << externalTempTimes.at(minExternalIndex) << " microseconds and a failure rate of " << externalTempFailureRate.at(minExternalIndex) << "%.";
  Info(ss);
  ss << "The quickest solver for the combined Temperature circuit was " << EigenCircuitSolver::Value(minTempIndex) << " at an average of " << temperatureTimes.at(minTempIndex) << " microseconds and a failure rate of " << temperatureFailureRate.at(minTempIndex) << "%.";
  Info(ss);

  //What should we write out in the report and/or Track?
  //testReport.WriteFile(rptDirectory + "/SolverSpeedTest.xml");
}
}