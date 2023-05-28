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
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#include <biogears/io/io-manager.h>

namespace BGE = mil::tatrc::physiology::biogears;

OPEN_BIOGEARS_NAMESPACE
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
    std::unique_ptr<CDM::ObjectData> bind;

    auto io = m_Logger->GetIoManager().lock();
    auto possible_path = io->FindScenarioFile(scenarioFile.c_str());
    if (possible_path.empty()) {
#ifdef BIOGEARS_IO_PRESENT
      size_t content_size;
      auto content = io->get_embedded_resource_file(scenarioFile.c_str(), content_size);
      bind = Serializer::ReadBuffer((XMLByte*)content, content_size, m_Logger);
#endif
    } else {
      bind = Serializer::ReadFile(possible_path, m_Logger);
    }

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
CLOSE_BIOGEARS_NAMESPACE