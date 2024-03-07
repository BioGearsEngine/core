//**********************************************************************************
// Copyright 2015 Applied Research Associates, Inc.
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use
// this file except in compliance with the License.You may obtain a copy of the License
// at :
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software distributed under
// the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.See the License for the
// specific language governing permissions and limitations under the License.
//**************************************************************************************

#include "ScenarioGenerator.h"

#include <filesystem>
#include <iostream>
#include <strstream>

#include <biogears/io/io-manager.h>
#include <biogears/schema/cdm/Scenario.hxx>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include <biogears/schema/cdm/Patient.hxx>
#include <biogears/string/manipulation.h>

namespace mil {
namespace tatrc {
  namespace physiology {
    namespace datamodel {
    }
  }
}
}
namespace CDM = mil::tatrc::physiology::datamodel;

static std::string dateString()
{
  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  std::string s(30, '\0');
  std::strftime(&s[0], s.size(), "%Y-%m-%dT%H_%M_%S", std::localtime(&now));
  return s;
}

namespace biogears {
std::vector<std::string> generate_boy_names();
std::vector<std::string> generate_girl_names();
//-------------------------------------------------------------------------------
ScenarioGenerator::ScenarioGenerator(std::vector<std::string> params)
{
  _buffer.resize(_buffer_size);
  _runs = params;

  // TODO: STRIP NON VALID FILES
}

//-------------------------------------------------------------------------------
//!
//! \brief Iterates through patientFiles, creates a lambda function for each item, and passes those functions to a thread pool
//!
void ScenarioGenerator::Generate()
{

  std::random_device rd {};
  std::mt19937 gen { rd() };
  std::string unit_str = "";
  std::string scenario_pool_dir = std::string("scenario_") + dateString();

  for (auto& run : _runs) {
    auto const& config_file = run;

    try {

      biogears::IOManager ioManager;

      auto content_size = ioManager.read_resource_file(run.c_str(), reinterpret_cast<char*>(&_buffer[0]), _buffer.size());
      _buffer[content_size] = std::ifstream::traits_type::eof(); 
     

      std::istrstream content_stream(reinterpret_cast<const char*>(_buffer.data()), content_size);

      auto scenarioTemplate = CDM::ScenarioTemplate(content_stream);

      for (auto& scenario : scenarioTemplate->Template()) {
        auto timelineName = scenario.name();

        std::filesystem::path timelinePath = scenario.file().c_str();
        if (std::filesystem::exists(timelinePath)) {

          std::ifstream ifs { timelinePath };
          if (ifs.is_open()) {
            std::string timelineContents;
            std::ostringstream ss;
            ss << ifs.rdbuf();
            timelineContents = ss.str();
          }
        }
      }
    } catch (std::runtime_error /*e*/) {
      std::cout << "Exception caught." << std::endl;
    } catch ( ::xsd::cxx::tree::parsing< char > e) {
      std::cout << "Exception caught.\n"
                << e << std::endl;
    }
  }
}

//-------------------------------------------------------------------------------
} // namespace biogears
