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

#include <biogears/io/io-manager.h>
#include <biogears/schema/cdm/Generators.hxx>
#include <biogears/schema/cdm/Patient.hxx>
#include <biogears/schema/cdm/Scenario.hxx>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <regex>
#include <set>
#include <strstream>
#include <vector>

#include <biogears/cdm/Serializer.h>
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

namespace biogears {
//-------------------------------------------------------------------------------
// Class Scenario Template
struct ScenarioTemplate {
public:
  std::string name;
  std::string timeline_file;

  std::map<std::string, std::normal_distribution<double>> normals;
  std::map<std::string, std::pair<double, double>> limits_of_normals;

  std::map<std::string, std::uniform_int_distribution<int>> discrete_uniforms;
  std::map<std::string, std::uniform_real_distribution<double>> continuous_uniforms;

  std::map<std::string, std::discrete_distribution<int>> weighted_discretes;
  std::map<std::string, CDM::WeightedDistributionData> weighted_descrete_values;
};
//-------------------------------------------------------------------------------
static ScenarioTemplate process_template(CDM::ScenarioProfileData data)
{

  ScenarioTemplate result;

  result.name = data.name();
  result.timeline_file = data.templateFile();

  for (auto nDistribuition : data.NormalDistribution()) {
    result.normals[nDistribuition] = std::normal_distribution<double>(nDistribuition.mean(), nDistribuition.deviation());

    auto a = nDistribuition.min().present() ? nDistribuition.min().get() : std::numeric_limits<double>::min();
    auto b = nDistribuition.max().present() ? nDistribuition.max().get() : std::numeric_limits<double>::max();
    result.limits_of_normals[nDistribuition] = std::pair<double, double> { a, b };
  }

  for (auto nDistribuition : data.DiscreteUniformDistribution()) {
    result.discrete_uniforms[nDistribuition] = std::uniform_int_distribution<int>(nDistribuition.a(), nDistribuition.b());
  }

  for (auto nDistribuition : data.ContinuousUniformDistribution()) {
    result.continuous_uniforms[nDistribuition] = std::uniform_real_distribution<double>(nDistribuition.a(), nDistribuition.b());
  }

  for (auto nDistribuition : data.WeightedDistribution()) {
    std::vector<double> weights;
    for (auto& weight : nDistribuition.Value()) {
      weights.push_back(weight.weight());
    }
    result.weighted_discretes[nDistribuition.name()] = std::discrete_distribution<int>(weights.begin(), weights.end());
    result.weighted_descrete_values[nDistribuition.name()] = nDistribuition;
  }

  return result;
}
//-------------------------------------------------------------------------------
static void WriteScenarioFile(std::string const directory, std::string const name, std::string contents)
{
  xml_schema::namespace_infomap info;
  info[""].name = "uri:/mil/tatrc/physiology/datamodel";
  info[""].schema = "BioGearsDataModel.xsd";

  constexpr auto PREFIX_DIR = "Scenarios";
#if _WIN32
  _mkdir(PREFIX_DIR);
  _mkdir((std::string(PREFIX_DIR) + "/" + directory).c_str());
#else
  mkdir(PREFIX_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  mkdir((std::string(PREFIX_DIR) + "/" + directory).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

  try {
    std::string out_file = std::string(PREFIX_DIR) + "/" + directory.c_str() + "/" + name;
    std::ofstream oFileStream;
    oFileStream.open(out_file.c_str());
    if (oFileStream.is_open()) {

      biogears::Logger logger;

      auto result = Serializer::ReadBuffer((XMLByte*)contents.data(), contents.size(), &logger);

      CDM::Scenario(oFileStream, *((CDM::ScenarioData*)result.get()), info);
      std::cout << std::format("Saved {}/", PREFIX_DIR) + directory.c_str() + "/" + name
                << "\n";
    }
    oFileStream.close();

  } catch (::xsd::cxx::tree::parsing<char> e) {
    std::cout << e.what() << std::endl;
  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }
}
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

  auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
  std::string template_directory = std::format("Templates_{:%Y%m%dT%H%M}", time);

  // STEP 1 : Iterate over command arguments of CDM::ScenarioGeneratorData
  for (auto& run : _runs) {
    auto const& config_file = run;

    try {

      biogears::IOManager ioManager;

      auto content_size = ioManager.read_resource_file(run.c_str(), reinterpret_cast<char*>(&_buffer[0]), _buffer.size());
      _buffer[content_size] = 0;
      _buffer[content_size + 1] = std::ifstream::traits_type::eof();

      std::istrstream content_stream(reinterpret_cast<const char*>(_buffer.data()), content_size);

      auto scenarioGenerator = CDM::ScenarioGenerator(content_stream);

      // STEP 2: For Each ScenarioGenerator Data iterate over the ScenarioTemplateData elements
      for (auto& scenarioTemplate : scenarioGenerator->ScenarioProfile()) {

        auto processed_template = process_template(scenarioTemplate);
        ;

        // STEP 3: Each ScenarioTempalteData contains an attribute to either a CDM::ScenarioData or a CDM::ActionListData
        //         We must open each and replace any occurance of a NormalDistribution or WeightedDistribution name with a generated value.
        //         Regex assumes place hodler variables are in the folowing format @NAME@
        //
        //        Search order if timeline
        //        Absolute -> Resolve
        //        Relative -> [ Folder(run) + timeline(), timeline(), timeline/ + timeline(), scenario/ + timeline()

        size_t master_timeline_content_size = 0;
        auto path_to_timeline = std::filesystem::path(scenarioTemplate.templateFile().c_str());
        if (!path_to_timeline.is_absolute()) {
          path_to_timeline = std::filesystem::path(run.c_str()).parent_path() / std::filesystem::path(scenarioTemplate.templateFile().c_str());
        }
        if (!std::filesystem::exists(path_to_timeline)) {
          path_to_timeline = std::filesystem::path(scenarioTemplate.templateFile().c_str());
          if (!std::filesystem::exists(path_to_timeline)) {
            path_to_timeline = std::filesystem::path("timeline/") / std::filesystem::path(scenarioTemplate.templateFile().c_str());
            if (!std::filesystem::exists(path_to_timeline)) {
              path_to_timeline = std::filesystem::path("scenario/") / std::filesystem::path(scenarioTemplate.templateFile().c_str());
            }
          }
        }

        if (!std::filesystem::exists(path_to_timeline)) {
          std::cerr << std::format("Unable to find {}", scenarioTemplate.templateFile().c_str());
          continue;
        }

        // STEP 4: We have resolved the relative path to the timeline. Lets read it in and modify it.
        master_timeline_content_size = ioManager.read_resource_file(path_to_timeline.string().c_str(), reinterpret_cast<char*>(&_buffer[0]), _buffer.size());

        if (master_timeline_content_size == 0) {
          std::cerr << std::format("Unable to read {}", path_to_timeline.string());
          continue;
        }

        // TODO:Bounds Check
        _buffer[master_timeline_content_size] = 0;
        _buffer[master_timeline_content_size + 1] = std::ifstream::traits_type::eof();

        std::istrstream master_tlineline_content_stream(reinterpret_cast<const char*>(_buffer.data()), master_timeline_content_size);
        auto master_timeline_contents = master_tlineline_content_stream.str();

        for (auto template_count = 0; template_count < scenarioTemplate.count(); ++template_count) {
          std::string modified_scenario_contents = master_timeline_contents;

          for (auto& [key, distribution] : processed_template.normals) {
            std::string regex = std::format("@{}@", key);
            const std::regex re(regex);
            auto clmaped_value = std::max(processed_template.limits_of_normals[key].first,
                                          std::min(processed_template.limits_of_normals[key].second, distribution(gen)));
            modified_scenario_contents = std::regex_replace(modified_scenario_contents, re, std::format("{}", clmaped_value));
          }

          for (auto& [key, distribution] : processed_template.discrete_uniforms) {
            std::string regex = std::format("@{}@", key);
            const std::regex re(regex);
            modified_scenario_contents = std::regex_replace(modified_scenario_contents, re, std::format("{}", distribution(gen)));
          }

          for (auto& [key, distribution] : processed_template.continuous_uniforms) {
            std::string regex = std::format("@{}@", key);
            const std::regex re(regex);
            modified_scenario_contents = std::regex_replace(modified_scenario_contents, re, std::format("{}", distribution(gen)));
          }

          for (auto& [key, distribution] : processed_template.weighted_discretes) {
            const std::regex re(std::format("@{}@", key));

            auto intermediate_value = distribution(gen);
            auto intended_values = processed_template.weighted_descrete_values[key].Value();
            std::string value = intended_values[intermediate_value];
            modified_scenario_contents = std::regex_replace(modified_scenario_contents, re, std::format("{}", value));
          }
          std::string output_name = std::format("{}_{:02}.xml", processed_template.name, template_count + 1);
          WriteScenarioFile(template_directory, output_name, modified_scenario_contents);
        }
      }

    } catch (std::runtime_error /*e*/) {
      std::cout << "Exception caught." << std::endl;
    } catch (::xsd::cxx::tree::parsing<char> e) {
      std::cout << "Exception caught.\n"
                << e << std::endl;
    }
  }
}

//-------------------------------------------------------------------------------
} // namespace biogears
