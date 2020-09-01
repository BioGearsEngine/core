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



// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SEScenarioExec.h>
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// A class used to handle any specific logic you may want to do each time step
///
/// \details
/// This method will be called at the end of EACH time step of the engine
/// The SEScenarioExecutor will process the advance time actions in a scenario and
/// step the engine, calling this method each time step
//--------------------------------------------------------------------------------------------------
class MyCustomExec : public SEScenarioCustomExec {
public:
  void CustomExec(double time_s, PhysiologyEngine* engine)
  {
    // you are given the current scenairo time and the engine, so you can do what ever you want
  }
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage of creating and running a scenario
///
/// \details
//--------------------------------------------------------------------------------------------------
void HowToRunScenario()
{
  // Create an engine object
  // BioGearsEngines will always output log messages to stdout and a log file
  // If you want this engine to write a log file, include the name
  // of the log file. If nullptr is given, the engine will only output to the console
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToRunScenario.log");
  bg->GetLogger()->Info("HowToRunScenario");

  // This BioGearsEngine logger is based on log4cpp (which is based on log4j)
  // BioGearsEngine logs to several distinct, ordered
  // category levels: DEBUG, INFO, WARN, ERROR, FATAL
  // These categories are orders, if your level is set to DEBUG you will recieve ALL messages.
  // If set to INFO, you will not recieve DEBUG, but everything else
  // If set to WARN, you will not recieve DEBUG and INFO, but everything else
  // You can specify which level you would like the engine to log
  bg->GetLogger()->SetLogLevel(log4cpp::Priority::INFO);

  // You can forward logs as demonstrated in HowTo-EngineUse

  // Create a Scenario Executor
  SEScenarioExec executor(*bg);
  // Let's make a scenario (you could just point the executor to a scenario xml file on disk as well)
  SEScenario sce(bg->GetSubstanceManager());
  sce.SetName("HowToRunScenario");
  sce.SetDescription("Simple Scenario to demonstraight building a scenario by the CDM API");
  sce.GetInitialParameters().SetPatientFile("StandardMale.xml");
  // Note you can set an Engine state, or create your own SEPatient object (see HowTo-CreateAPatient)
  // When filling out a data request, units are optional
  // The units will be set to whatever units the engine uses.
  // NOTE: The scenario makes it's own copy of these requests
  // Once you set it, any changes will not be reflected in the scenario
  // You can reuse this object for future actions
  sce.GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  sce.GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", FrequencyUnit::Per_min);
  sce.GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalLungVolume", VolumeUnit::mL);
  // you can specify where the file goes
  sce.GetDataRequestManager().SetResultsFilename("./HowTo-RunScenarioResults.csv");
  // If you don't set the file name it will try to make a place for the results in a bin/Scenarios/ folder
  // Let's just run for 2 minutes
  // NOTE: the scenario will make it's own copy of this action
  // Once you set it, any changes will not be reflected in the scenario
  // You can reuse this object for future actions
  SEAdvanceTime adv;
  adv.GetTime().SetValue(2, TimeUnit::min);
  sce.AddAction(adv);

  executor.Execute(sce, "./HowTo-RunScenarioResults.csv", new MyCustomExec());
}
