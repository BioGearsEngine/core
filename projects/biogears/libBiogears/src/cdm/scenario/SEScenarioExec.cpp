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
#include <biogears/cdm/scenario/SEScenarioExec.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SEScenarioAutoSerialization.h>
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>
#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/cdm/utils/TimingProfile.h>
#include <biogears/schema/cdm/Scenario.hxx>

namespace biogears {
SEScenarioExec::SEScenarioExec(PhysiologyEngine& engine)
  : Loggable(engine.GetLogger())
  , m_Engine(engine)
{
  m_Cancel = false;
  m_CustomExec = nullptr;
}
//-----------------------------------------------------------------------------
SEScenarioExec::~SEScenarioExec()
{
}
//-----------------------------------------------------------------------------
bool SEScenarioExec::Execute(const char* scenarioFile, const char* resultsFile, SEScenarioCustomExec* cExec)
{
  return Execute(std::string{ scenarioFile }, std::string{ resultsFile }, cExec);
}
bool SEScenarioExec::Execute(SEScenario const& scenario, const char* resultsFile, SEScenarioCustomExec* cExec)
{
  Execute(scenario, std::string{ resultsFile }, cExec);
}
//-----------------------------------------------------------------------------
bool SEScenarioExec::Execute(SEScenario const& scenario, const std::string& resultsFile, SEScenarioCustomExec* cExec)
{
  auto scenarioData =  std::unique_ptr<CDM::ScenarioData>(scenario.Unload());
  auto memory_safe_scenario = std::make_unique<SEScenario>(m_Engine.GetSubstanceManager());
  memory_safe_scenario = std::make_unique<SEScenario>(m_Engine.GetSubstanceManager());
  if (!memory_safe_scenario->Load(*scenarioData)) {
    return false;
  }
  try {
    m_CustomExec = cExec;

    // Initialize the engine with a state or initial parameters
    if (scenarioData->EngineStateFile().present()) {
      m_Engine.LoadState(scenarioData->EngineStateFile().get());

      //SEScenario is a flawed design that requires the memory state of a
      //Physiology Engine to remain constant over the scenario and makes
      //State loading a nightmare. The class needs to be depricated this entire
      //Driver could just read from the Serial layer and standup Actions and Datarequiest
      //When we know the physiolgoy engine is good to go and not try to cashe state.

      memory_safe_scenario = std::make_unique<SEScenario>(m_Engine.GetSubstanceManager());
      if (!memory_safe_scenario->Load(*scenarioData)) {
        return false;
      }


      // WE ARE OVERWRITING ANY DATA REQUESTS IN THE STATE WITH WHATS IN THE SCENARIO!!!
      // Make a copy of the data requests, not this clears out data requests from the engine
      CDM::DataRequestsData* drData = memory_safe_scenario->GetDataRequestManager().Unload();
      m_Engine.GetEngineTrack()->GetDataRequestManager().Load(*drData, m_Engine.GetSubstanceManager());
      delete drData;
      //if (!m_Engine.GetEngineTrack()->GetDataRequestManager().HasResultsFilename())
      m_Engine.GetEngineTrack()->GetDataRequestManager().SetResultsFilename(resultsFile);
    } else if (scenarioData->InitialParameters().present()) {

      m_Engine.GetEngineTrack()->GetDataRequestManager().Load(scenarioData->DataRequests().get(), m_Engine.GetSubstanceManager());

      //if (!m_Engine.GetEngineTrack()->GetDataRequestManager().HasResultsFilename())
      m_Engine.GetEngineTrack()->GetDataRequestManager().SetResultsFilename(resultsFile);

      auto& params = memory_safe_scenario->GetInitialParameters();
      // Do we have any conditions
      std::vector<const SECondition*> conditions;
      for (SECondition* c : params.GetConditions()) {
        conditions.push_back(c);
      }

      if (params.HasPatientFile()) {
        // Set up the patient
        std::string pFile = "";
        pFile += params.GetPatientFile();
        if (!m_Engine.InitializeEngine(pFile, &conditions, &params.GetConfiguration())) {
          Error("Unable to initialize engine");
          return false;
        }
      } else if (params.HasPatient()) {
        if (!m_Engine.InitializeEngine(params.GetPatient(), &conditions, &params.GetConfiguration())) {
          Error("Unable to initialize engine");
          return false;
        }
      } else {
        Error("No patient provided");
        return false;
      }
    } else {
      Error("No initial engine parameters set");
      return false;
    }

    if (scenarioData->AutoSerialization().present()) {
      CreateFilePath(scenarioData->AutoSerialization()->Directory()); // Note method assumes you have a file and it ignores it
    }
    return ProcessActions(*memory_safe_scenario);
  } catch (CommonDataModelException& ex) {
    Error(ex.what());
  } catch (std::exception& ex) {
    Error(ex.what());
  } catch (...) {
    Error("Caught unknown exception, ending simulation");
  }
  return false;
};

//-----------------------------------------------------------------------------
bool SEScenarioExec::Execute(const std::string& scenarioFile, const std::string& resultsFile, SEScenarioCustomExec* cExec)
{

  try {
    std::string rFile = resultsFile;
    if (rFile.empty()) {
      rFile = scenarioFile;
      rFile += ".out";
    }

    m_ss << "Executing scenario file : " << scenarioFile << std::endl;
    Info(m_ss);
    m_Cancel = false;
    m_CustomExec = cExec;

    std::unique_ptr<CDM::ObjectData> bind = Serializer::ReadFile(scenarioFile, GetLogger());
    if (bind == nullptr) {
      m_ss << "Unable to load scenario file : " << scenarioFile << std::endl;
      Error(m_ss);
      return false;
    }
    CDM::ScenarioData* scenarioData = dynamic_cast<CDM::ScenarioData*>(bind.get());
    if (scenarioData == nullptr) {
      m_ss << "Unable to load scenario file : " << scenarioFile << std::endl;
      Error(m_ss);
      return false;
    }

    
    SEScenario scenario(m_Engine.GetSubstanceManager());
    if (!scenario.Load(*scenarioData)) {
      return false;
    }

    bool success = Execute(scenario, rFile,cExec);
    return success;
  } catch (CommonDataModelException& ex) {
    Error(ex.what());
  } catch (...) {
    Error("Caught unknown exception, ending simulation");
  }
  return false;
}

//-----------------------------------------------------------------------------
void SEScenarioExec::Cancel()
{
  m_Cancel = true;
}
//-----------------------------------------------------------------------------
bool SEScenarioExec::ProcessActions(SEScenario& scenario)
{
  Info("Executing Scenario");

  double dT_s = m_Engine.GetTimeStep(TimeUnit::s);
  double scenarioTime_s;
  double statusTime_s = 0; // Current time of this status cycle
  double statusStep_s = 60; //How long did it take to simulate this much time

  double sampleTime_s = scenario.GetDataRequestManager().GetSamplesPerSecond();
  if (sampleTime_s != 0) {
    sampleTime_s = 1 / sampleTime_s;
  }
  double currentSampleTime_s = sampleTime_s; //Sample the first step

  // Auto serialization
  bool serializeAction = false;
  double serializationTime_s = 0;
  double serializationPeriod_s = 0;
  std::string actionName;
  std::string serializationFileNameBase;
  std::stringstream serializationFileName;

  if (scenario.GetAutoSerialization().IsValid()) {
    serializationFileNameBase = scenario.GetAutoSerialization().GetFileName();
      // Strip off the xml if it's there
      size_t split = serializationFileNameBase.find(".xml");
      if (split != serializationFileNameBase.npos) {
        serializationFileNameBase = serializationFileNameBase.substr(0, split);
      }
      serializationPeriod_s = scenario.GetAutoSerialization().GetPeriod(TimeUnit::s);
      serializationFileName << scenario.GetAutoSerialization().GetDirectory() << "/" << serializationFileNameBase;
      serializationFileNameBase = serializationFileName.str();
  }

  TimingProfile profiler;
  profiler.Start("Total");
  profiler.Start("Status");

  m_Engine.GetEngineTrack()->SetupRequests();

  bool err = false;
  SEAdvanceTime* adv;
  for (SEAction* a : scenario.GetActions()) {
    if (m_Cancel) {
      break;
    }
    // We override advance time actions in order to advance and
    // pull requested data at each time step, all other actions
    // will be processed by the engine
    adv = dynamic_cast<SEAdvanceTime*>(a);
    if (adv != nullptr) {
      double time_s = adv->GetTime(TimeUnit::s);
      int count = (int)(time_s / dT_s);
      for (int i = 0; i <= count; i++) {
        if (m_Cancel) {
          break;
        }

        m_Engine.AdvanceModelTime();

        if (scenario.GetAutoSerialization().IsValid()) { // Auto Serialize
          if (serializationPeriod_s > 0) {
            serializationTime_s += dT_s;
            if (serializationTime_s > serializationPeriod_s) {
              serializationTime_s = 0;
              serializationFileName.str("");
              serializationFileName << serializationFileNameBase;
              if (scenario.GetAutoSerialization().GetPeriodTimeStamps() == CDM::enumOnOff::On)
                serializationFileName << "@" << m_Engine.GetSimulationTime(TimeUnit::s);
              serializationFileName << ".xml";
              std::unique_ptr<CDM::PhysiologyEngineStateData> state(m_Engine.SaveState(serializationFileName.str()));
              if (scenario.GetAutoSerialization().GetReloadState() == CDM::enumOnOff::On) {
                m_Engine.LoadState(*state);
                std::unique_ptr<CDM::PhysiologyEngineStateData> state(m_Engine.SaveState(serializationFileName.str() + ".Reloaded.xml"));
              }
            }
          }
          if (serializeAction) {
            serializeAction = false;
            serializationFileName.str("");
            serializationFileName << serializationFileNameBase << "-" << actionName << "-@" << m_Engine.GetSimulationTime(TimeUnit::s) << ".xml";
            std::unique_ptr<CDM::PhysiologyEngineStateData> state(m_Engine.SaveState(serializationFileName.str()));
            if (scenario.GetAutoSerialization().GetReloadState() == CDM::enumOnOff::On) {
              m_Engine.LoadState(*state);
              std::unique_ptr<CDM::PhysiologyEngineStateData> state(m_Engine.SaveState(serializationFileName.str() + ".Reloaded.xml"));
            }
          }
        }

        // Pull data from the engine
        scenarioTime_s = m_Engine.GetSimulationTime(TimeUnit::s);
        currentSampleTime_s += dT_s;
        if (currentSampleTime_s >= sampleTime_s) {
          currentSampleTime_s = 0;
          m_Engine.GetEngineTrack()->TrackData(scenarioTime_s);
        }
        // Call any custom callback provided
        if (m_CustomExec != nullptr) {
          m_CustomExec->CustomExec(scenarioTime_s, &m_Engine);
        }
        statusTime_s += dT_s;
        // How are we running?
        if (statusTime_s > statusStep_s) {
          statusTime_s = 0;
          m_ss << "Current Time is " << scenarioTime_s << "s, it took " << profiler.GetElapsedTime_s("Status") << "s to simulate the past " << statusStep_s << "s" << std::flush;
          profiler.Reset("Status");
          Info(m_ss);
        }
        if (m_Engine.GetPatient().IsEventActive(CDM::enumPatientEvent::IrreversibleState))
          return false; // Patient is for all intents and purposes dead, or out at least out of its methodology bounds, quit running
      }
      continue;
    }

    if (!ProcessAction(*a)) {
      err = true;
      break;
    }

    if (scenario.GetAutoSerialization().IsValid() && scenario.GetAutoSerialization().GetAfterActions() == CDM::enumOnOff::On) { // If we are testing force serialization after any action with this
      // Pull out the action type/name for file naming
      m_ss << *a;
      size_t start = m_ss.str().find(": ") + 2;
      size_t end = m_ss.str().find('\n');
      actionName = m_ss.str().substr(start, end - start);
      m_ss.str("");

      serializationFileName.str("");
      serializationFileName << serializationFileNameBase << "-" << actionName << "-@" << m_Engine.GetSimulationTime(TimeUnit::s) << ".xml";
      std::unique_ptr<CDM::PhysiologyEngineStateData> state(m_Engine.SaveState(serializationFileName.str()));
      if (scenario.GetAutoSerialization().GetReloadState() == CDM::enumOnOff::On) {
        m_Engine.LoadState(*state);
        std::unique_ptr<CDM::PhysiologyEngineStateData> state(m_Engine.SaveState(serializationFileName.str() + ".Reloaded.xml"));
      }
      serializeAction = true; // Serialize after the next time step
    }

    if (m_Engine.GetPatient().IsEventActive(CDM::enumPatientEvent::IrreversibleState))
      return false; // Patient is for all intents and purposes dead, or out at least out of its methodology bounds, quit running
  }
  m_ss << "It took " << profiler.GetElapsedTime_s("Total") << "s to run this simulation";
  profiler.Clear();
  Info(m_ss);

  return !err;
}
//-----------------------------------------------------------------------------
bool SEScenarioExec::ProcessAction(const SEAction& action)
{
  return m_Engine.ProcessAction(action);
}
//-----------------------------------------------------------------------------
}