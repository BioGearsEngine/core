//**********************************************************************************
//Copyright 2015 Applied Research Associates, Inc.
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//this file except in compliance with the License.You may obtain a copy of the License
//at :
//http://www.apache.org/licenses/LICENSE-2.0
//Unless required by applicable law or agreed to in writing, software distributed under
//the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//CONDITIONS OF ANY KIND, either express or implied.See the License for the
//specific language governing permissions and limitations under the License.
//**************************************************************************************
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>

#include "PatientValidationGenerator.h"
#include <iostream>
#include <string>
#include "biogears/string/manipulation.h"

//
namespace biogears {
int runPatientScenario(const std::string patient, std::string&& XMLString);

//-------------------------------------------------------------------------------
PatientValidationGenerator::PatientValidationGenerator(size_t thread_count)
  : _pool(thread_count)
{
}
//-------------------------------------------------------------------------------
PatientValidationGenerator::~PatientValidationGenerator()
{
}
//-------------------------------------------------------------------------------
//!
//! \brief Iterates through patientFiles, creates a lambda function for each item, and passes those functions to a thread pool
//! 
void PatientValidationGenerator::GenPatientValidation()
{

  auto patients = ListFiles("patients", R"(\.xml)");
  for (auto& patient : patients) {
    std::function<void()> work = [=](){ biogears::runPatientScenario(patient, std::string("Scenarios/Validation/Patient-Validation.xml")); };
    _pool.queue_work(work);
  }
}
//-------------------------------------------------------------------------------

//!
//! \brief creates a BioGearsScenarioExec object and executes the scenario
//! \param patient : the name of the patient being used for the scenario
//! \param XMLString : a path to the xml scenario being used
//! \return int 0 if no exceptions were encountered, otherwise 1
//!
int runPatientScenario(const std::string patient, std::string&& XMLString)
{
  std::string patientXML(patient);

  std::string patientLog = "-" + patientXML;
  patientLog = findAndReplace(patientLog, ".xml", ".log");

  std::string patientResults = "-" + patientXML;
  patientResults = findAndReplace(patientResults, ".xml", "Results.csv");

  std::string logFile(patient);
  std::string outputFile(patient);
  logFile = findAndReplace(logFile, ".xml", "Results.log");
  outputFile = findAndReplace(outputFile, ".xml", "Results.csv");

  std::unique_ptr<PhysiologyEngine> eng;
  try {
    eng = CreateBioGearsEngine(logFile);
  } catch (std::exception e) {
    std::cout << e.what();
    return 1;
  }
  DataTrack* trk = &eng->GetEngineTrack()->GetDataTrack();
  BioGearsScenario sce(eng->GetSubstanceManager());
  sce.Load(XMLString);
  sce.GetInitialParameters().SetPatientFile(patientXML);

  BioGearsScenarioExec* exec = new BioGearsScenarioExec(*eng);
  exec->Execute(sce, outputFile, nullptr);
  delete exec;

  return 0;
}
//-------------------------------------------------------------------------------
//!
//! \brief thread pool begins execution of tasks in queue
//! 
void PatientValidationGenerator::run()
{
  _pool.start();
}
//-------------------------------------------------------------------------------
//!
//! \brief stops execution of tasks in queue
//! 
void PatientValidationGenerator::stop()
{
  _pool.stop();
}
//-------------------------------------------------------------------------------
//!
//! \brief stops the thread pool if the work queue is empty
//! \return true if the work queue is empty, false otherwise
//! 
bool PatientValidationGenerator::stop_if_empty()
{  
  return _pool.stop_if_empty();
}
//-------------------------------------------------------------------------------
//!
//! \brief joins threads in thread pool
//! 
void PatientValidationGenerator::join()
{
  _pool.join();
}
} // namespace biogears
