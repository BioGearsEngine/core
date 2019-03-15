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
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
BioGearsScenarioExec::BioGearsScenarioExec(PhysiologyEngine& engine)
  : SEScenarioExec(engine)
{
}

BioGearsScenarioExec::~BioGearsScenarioExec()
{
}

bool BioGearsScenarioExec::Execute(const SEScenario& scenario, const std::string& resultsFile, SEScenarioCustomExec* cExec)
{
  try {
    bool success = SEScenarioExec::Execute(scenario, resultsFile, cExec);
    return success;
  } catch (CommonDataModelException& ex) {
    Error(ex.what());
  } catch (...) {
    Error("Caught unknown exception, ending simulation");
  }
  return false;
}

bool BioGearsScenarioExec::Execute(const std::string& scenarioFile, const std::string& resultsFile, SEScenarioCustomExec* cExec)
{
  try {
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
    CDM::ScenarioData* sceData = dynamic_cast<CDM::ScenarioData*>(bind.get());
    if (sceData == nullptr) {
      m_ss << "Unable to load scenario file : " << scenarioFile << std::endl;
      Error(m_ss);
      return false;
    }
    BioGearsScenario scenario(m_Engine.GetSubstanceManager());
    scenario.Load(*sceData);
    std::string rFile = resultsFile;
 
    bool success = SEScenarioExec::Execute(scenario, rFile, cExec);
    return success;
  } catch (CommonDataModelException& ex) {
    Error(ex.what());
  } catch (...) {
    Error("Caught unknown exception, ending simulation");
  }
  return false;
}
}