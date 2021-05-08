/**************************************************************************************
Copyright 2021 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#include <biogears/io/io-manager.h>

#include <biogears/filesystem/path.h>

#include <biogears/version.h>

#ifdef BIOGEARS_IO_PRESENT
#include <biogears/io/constants.h>
#include <biogears/io/directories/config.h>
#include <biogears/io/directories/ecg.h>
#include <biogears/io/directories/environments.h>
#include <biogears/io/directories/nutrition.h>
#include <biogears/io/directories/override.h>
#include <biogears/io/directories/patients.h>
#include <biogears/io/directories/xsd.h>
#endif

#include <biogears/io/sha1.h>

#include <map>
#include <regex>

#ifndef _WIN32
#include <dirent.h>
#endif

#ifdef BIOGEARS_IO_EMBED_STATES
#include <biogears/io/directories/states.h>
#endif

#include <biogears/io/directories/substances.h>

#include <fstream>

namespace biogears {

IOManager::IOManager()
  : IOManager("", nullptr)
{
}
//---------------------------------------------------------------------------
IOManager::IOManager(char const* dataroot, char const* schemaroot)
  : m_biogears_data_root(dataroot)
  , m_biogears_install_root("")
  , m_biogears_working_directory("")
{
  m_biogears_schema_root = (schemaroot) ? schemaroot : m_biogears_data_root;
}
//---------------------------------------------------------------------------
IOManager::~IOManager()
{
}
//---------------------------------------------------------------------------

//!
//! The following files are designed to generate new file locations.
//! Each function take in path. If the path is absolute it returns the path
//! If the path is relative it will append the working_directory and the appropriate folder to the
//! path provided
//!
//! If looking for an existing file call find_resource_file or read_resource_file
//!

inline std::string ResolveFileLocation(std::string const& given, std::string const& prefix, std::string const& working_dir)
{
  filesystem::path given_path { given };
  if (given_path.is_absolute()) {
    return given_path.string();
  }
  filesystem::path config_path = { prefix };
  config_path /= given_path;
  if (config_path.is_absolute()) {
    return config_path.string();
  }
  filesystem::path working_path = { working_dir };
  working_path /= config_path;
  return working_path.string();
}

std::string IOManager::ResolveConfigFileLocation(std::string const& filename) { return ResolveFileLocation(filename, m_dirs.config, m_biogears_working_directory); }
std::string IOManager::ResolveEcgFileLocation(std::string const& filename) { return ResolveFileLocation(filename, m_dirs.ecg, m_biogears_working_directory); }
std::string IOManager::ResolveEnvironmentFileLocation(std::string const& filename) { return ResolveFileLocation(filename, m_dirs.environments, m_biogears_working_directory); }
std::string IOManager::ResolveNutritionFileLocation(std::string const& filename) { return ResolveFileLocation(filename, m_dirs.nutrition, m_biogears_working_directory); }
std::string IOManager::ResolveOverrideFileLocation(std::string const& filename) { return ResolveFileLocation(filename, m_dirs.override, m_biogears_working_directory); }
std::string IOManager::ResolvePatientFileLocation(std::string const& filename) { return ResolveFileLocation(filename, m_dirs.patients, m_biogears_working_directory); }
std::string IOManager::ResolveStateFileLocation(std::string const& filename)
{
  return ResolveFileLocation(filename, m_dirs.states, m_biogears_working_directory);
}
std::string IOManager::ResolveSubstanceFileLocation(std::string const& filename)
{
  return ResolveFileLocation(filename, m_dirs.substances, m_biogears_working_directory);
}
std::string IOManager::ResolveScenarioFileLocation(std::string const& filename) { return ResolveFileLocation(filename, m_dirs.scenarios, m_biogears_working_directory); }
std::string IOManager::ResolveLogFileLocation(std::string const& filename) { return ResolveFileLocation(filename, m_dirs.log, m_biogears_working_directory); }
std::string IOManager::ResolveResultsFileLocation(std::string const& filename) { return ResolveFileLocation(filename, m_dirs.results, m_biogears_working_directory); }

//------------------------------------------------------------------------------
std::vector<filesystem::path> ListFiles(std::string const& dir, std::string const& regex, bool recurse)
{
  std::string filename;
  std::vector<filesystem::path> results;
  std::regex filter { regex };
#ifdef _WIN32
  WIN32_FIND_DATAA data;
  filesystem::path pattern = dir + "\\*";
  HANDLE hFind = FindFirstFileA(pattern.string().c_str(), &data);

  if (INVALID_HANDLE_VALUE != hFind) {
    do {
      if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && recurse) {
        auto recurseResults = ListFiles(dir + "\\" + std::string(data.cFileName), regex, recurse);
        results.insert(results.end(),
                       std::make_move_iterator(recurseResults.begin()),
                       std::make_move_iterator(recurseResults.end()));
      } else {
        if (std::regex_match(data.cFileName, filter)) {
          results.emplace_back(pattern.parent_path() / data.cFileName);
        }
      }
    } while (FindNextFile(hFind, &data));
    FindClose(hFind);
    return results;
  }

#else
  DIR* d;
  dirent* ent;
  if ((d = opendir(dir.c_str())) != nullptr) {
    while ((ent = readdir(d)) != nullptr) {
      size_t nameLength = strlen(ent->d_name);
      if (ent->d_name[0] == '.' && ((nameLength == 1) || (nameLength == 2 && ent->d_name[1] == '.')))
        continue;
      filename = dir;
      filename += "/";
      filename += ent->d_name;

      if (!filesystem::is_directory(filename)) {
        if (std::regex_match(ent->d_name, filter))
          results.push_back(filename);
      } else {
        if (recurse) {
          auto recurseResults = ListFiles(filename, regex, recurse);
          results.insert(results.end(),
                         std::make_move_iterator(recurseResults.begin()),
                         std::make_move_iterator(recurseResults.end()));
        }
      }
    }
  }
#endif
  return {};
}

//!
//! Finds files across all possible fallback locations and returns a list of the files
//! With highest priority. So the return list could be a mix of files from multiple directories if
//! Step 1 contains only one file and Step 2 contains all the required files.
//!
//! Possible alternative behavior would be to only fallback to the first directory which contains
//! A file, but the current behavior is supposed to encourage all files be embedded or in an install location
//! Except the ones modified
std::vector<filesystem::path> IOManager::find_files(std::string suffix, std::string regex, bool recurse) const
{
  std::map<std::string, filesystem::path> known_files;
  size_t content_size = 0;
  filesystem::path test_location;

  //Step 0 :
  test_location = suffix;
  if (test_location.is_absolute() && test_location.exists() && test_location.is_directory()) {
    auto results = ListFiles(test_location.string(), regex, recurse);
    for (auto& result : results) {
      filesystem::path p { result };
      if (known_files.find(p.basename().string()) == known_files.end()) {
        known_files[p.basename().string()] = std::move(p);
      }
    }
  }

  //Step 1 : Current Working Directory
  test_location = m_biogears_working_directory;
  test_location /= suffix;
  if (test_location.exists() && test_location.is_directory()) {
    auto results = ListFiles(test_location.string(), regex, recurse);
    for (auto& result : results) {
      filesystem::path p { result };
      if (known_files.find(p.basename().string()) == known_files.end()) {
        known_files[p.basename().string()] = std::move(p);
      }
    }
  }

  //Step 2 : Alternative CWD
  if (!m_biogears_data_root.empty()) {
    test_location = m_biogears_data_root;
    test_location /= suffix;
    if (test_location.exists() && test_location.is_directory()) {
      auto results = ListFiles(test_location.string(), regex, recurse);
      for (auto& result : results) {
        filesystem::path p { result };
        if (known_files.find(p.basename().string()) == known_files.end()) {
          known_files[p.basename().string()] = std::move(p);
        }
      }
    }
  }

  if (!m_biogears_schema_root.empty()) {
    test_location = m_biogears_schema_root;
    test_location /= suffix;
    if (test_location.exists() && test_location.is_directory()) {
      auto results = ListFiles(test_location.string(), regex, recurse);
      for (auto& result : results) {
        filesystem::path p { result };
        if (known_files.find(p.basename().string()) == known_files.end()) {
          known_files[p.basename().string()] = std::move(p);
        }
      }
    }
  }

  //Step 3: ENV{BIOGEARS_DATA_ROOT} ENV{BIOGEARS_SCHEMA_ROOT}
  char* data_root = getenv("BIOGEARS_DATA_ROOT");
  if (data_root != nullptr) {
    test_location = data_root;
    test_location /= suffix;
    if (test_location.exists() && test_location.is_directory()) {
      auto results = ListFiles(test_location.string(), regex, recurse);
      for (auto& result : results) {
        filesystem::path p { result };
        if (known_files.find(p.basename().string()) == known_files.end()) {
          known_files[p.basename().string()] = std::move(p);
        }
      }
    }
  }
  char* schema_root = getenv("BIOGEARS_SCHEMA_ROOT");
  if (schema_root != nullptr) {
    test_location = schema_root;
    test_location /= suffix;
    if (test_location.exists() && test_location.is_directory()) {
      auto results = ListFiles(test_location.string(), regex, recurse);
      for (auto& result : results) {
        filesystem::path p { result };
        if (known_files.find(p.basename().string()) == known_files.end()) {
          known_files[p.basename().string()] = std::move(p);
        }
      }
    }
  }
  //Step 3: BIOGEARS_INSTALL_ROOT
  std::string install_root;
#ifdef _WIN32
  //TODO: ALSO CHECK CMAKE_INSTALL_PREFIX as generated by CMAKE
  install_root = getenv("APPDATA");
  install_root += "/biogears/";
#else
  install_root = "~/.biogears/";
#endif
  install_root += rev_tag();
  if (install_root != "") {
    test_location = install_root;
    test_location /= suffix;
    if (test_location.exists() && test_location.is_directory()) {
      auto results = ListFiles(test_location.string(), regex, recurse);
      for (auto& result : results) {
        filesystem::path p { result };
        if (known_files.find(p.basename().string()) == known_files.end()) {
          known_files[p.basename().string()] = std::move(p);
        }
      }
    }
  }

  std::vector<filesystem::path> results;
  for (auto& pair : known_files) {
    results.emplace_back(std::move(pair.second.string()));
  }
  return results;
}

filesystem::path IOManager::FindConfigFile(const char* file) const
{
  auto possible_path = find_resource_file(file);
  if (possible_path.empty()) {
    std::string prefix { "config/" };
    if (possible_path.compare(0, prefix.size(), prefix) == 0) {
      possible_path = possible_path.substr(prefix.size());
    }
    filesystem::path implied_path = m_dirs.config;
    implied_path /= file;
    possible_path = find_resource_file(implied_path.string().c_str());
  }
  return possible_path;
}

filesystem::path IOManager::FindEcgFile(const char* file) const
{
  auto possible_path = find_resource_file(file);
  if (possible_path.empty()) {
    std::string prefix { "ecg/" };
    if (possible_path.compare(0, prefix.size(), prefix) == 0) {
      possible_path = possible_path.substr(prefix.size());
    }
    filesystem::path implied_path = m_dirs.ecg;
    implied_path /= file;
    possible_path = find_resource_file(implied_path.string().c_str());
  }
  return possible_path;
}

filesystem::path IOManager::FindEnvironmentFile(const char* file) const
{
  auto possible_path = find_resource_file(file);
  if (possible_path.empty()) {
    std::string prefix { "environments/" };
    if (possible_path.compare(0, prefix.size(), prefix) == 0) {
      possible_path = possible_path.substr(prefix.size());
    }
    filesystem::path implied_path = m_dirs.environments;
    implied_path /= file;
    possible_path = find_resource_file(implied_path.string().c_str());
  }
  return possible_path;
}

filesystem::path IOManager::FindNutritionFile(const char* file) const
{
  auto possible_path = find_resource_file(file);
  if (possible_path.empty()) {
    std::string prefix { "nutrition/" };
    if (possible_path.compare(0, prefix.size(), prefix) == 0) {
      possible_path = possible_path.substr(prefix.size());
    }
    filesystem::path implied_path = m_dirs.nutrition;
    implied_path /= file;
    possible_path = find_resource_file(implied_path.string().c_str());
  }
  return possible_path;
}

filesystem::path IOManager::FindOverrideFile(const char* file) const
{
  auto possible_path = find_resource_file(file);
  if (possible_path.empty()) {
    std::string prefix { "override/" };
    if (possible_path.compare(0, prefix.size(), prefix) == 0) {
      possible_path = possible_path.substr(prefix.size());
    }
    filesystem::path implied_path = m_dirs.override;
    implied_path /= file;
    possible_path = find_resource_file(implied_path.string().c_str());
  }
  return possible_path;
}

filesystem::path IOManager::FindPatientFile(const char* file) const
{
  auto possible_path = find_resource_file(file);
  if (possible_path.empty()) {
    std::string prefix { "patients/" };
    if (possible_path.compare(0, prefix.size(), prefix) == 0) {
      possible_path = possible_path.substr(prefix.size());
    }
    filesystem::path implied_path = m_dirs.patients;
    implied_path /= file;
    possible_path = find_resource_file(implied_path.string().c_str());
  }
  return possible_path;
}
filesystem::path IOManager::FindStateFile(const char* file) const
{
  auto possible_path = find_resource_file(file);
  if (possible_path.empty()) {
    std::string prefix { "states/" };
    if (possible_path.compare(0, prefix.size(), prefix) == 0) {
      possible_path = possible_path.substr(prefix.size());
    }
    filesystem::path implied_path = m_dirs.states;
    implied_path /= file;
    possible_path = find_resource_file(implied_path.string().c_str());
  }
  return possible_path;
}
filesystem::path IOManager::FindSubstanceFile(const char* file) const
{
  auto possible_path = find_resource_file(file);
  if (possible_path.empty()) {
    std::string prefix { "substances/" };
    if (possible_path.compare(0, prefix.size(), prefix) == 0) {
      possible_path = possible_path.substr(prefix.size());
    }
    filesystem::path implied_path = m_dirs.substances;
    implied_path /= file;
    possible_path = find_resource_file(implied_path.string().c_str());
  }
  return possible_path;
}

filesystem::path IOManager::FindScenarioFile(const char* file) const
{
  auto possible_path = find_resource_file(file);
  if (possible_path.empty()) {
    std::string prefix { "Scenarios/" };
    if (possible_path.compare(0, prefix.size(), prefix) == 0) {
      possible_path = possible_path.substr(prefix.size());
    }
    filesystem::path implied_path = m_dirs.scenarios;
    implied_path /= file;
    possible_path = find_resource_file(implied_path.string().c_str());
  }
  return possible_path;
}

std::vector<filesystem::path> IOManager::FindAllSubstanceFiles() const
{
  return find_files(m_dirs.substances, R"(.*\.xml)", false);
}
//!
//! Configuration Accessors
//!
std::string IOManager::GetConfigDirectory() const
{
  return m_dirs.config;
}
//---------------------------------------------------------------------------
void IOManager::SetConfigDirectory(std::string const& s)
{
  m_dirs.config = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetEcgDirectory() const
{
  return m_dirs.ecg;
}
//---------------------------------------------------------------------------
void IOManager::SetEcgDirectory(std::string const& s)
{
  m_dirs.ecg = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetEnvironmentsDirectory() const
{
  return m_dirs.environments;
}
//---------------------------------------------------------------------------
void IOManager::SetEnvironmentsDirectory(std::string const& s)
{
  m_dirs.environments = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetNutritionDirectory() const
{
  return m_dirs.nutrition;
}
//---------------------------------------------------------------------------
void IOManager::SetNutritionDirectory(std::string const& s)
{
  m_dirs.nutrition = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetOverrideDirectory() const
{
  return m_dirs.override;
}
//---------------------------------------------------------------------------
void IOManager::SetOverrideDirectory(std::string const& s)
{
  m_dirs.override = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetPatientsDirectory() const
{
  return m_dirs.patients;
}
//---------------------------------------------------------------------------
void IOManager::SetPatientsDirectory(std::string const& s)
{
  m_dirs.patients = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetStatesDirectory() const
{
  return m_dirs.states;
}
//---------------------------------------------------------------------------
void IOManager::SetStatesDirectory(std::string const& s)
{
  m_dirs.states = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetSubstancesDirectory() const
{
  return m_dirs.substances;
}
//---------------------------------------------------------------------------
void IOManager::SetSubstancesDirectory(std::string const& s)
{
  m_dirs.substances = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetScenariosDirectory() const
{
  return m_dirs.scenarios;
}
//---------------------------------------------------------------------------
void IOManager::SetScenariosDirectory(std::string const& s)
{
  m_dirs.scenarios = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetBioGearsDataRootDirectory() const
{
  return m_biogears_data_root;
}
//---------------------------------------------------------------------------
void IOManager::SetBioGearsDataRootDirectory(std::string const& s)
{
  m_biogears_data_root = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetBioGearsSchemaDirectory() const
{
  return m_biogears_schema_root;
}
//---------------------------------------------------------------------------
void IOManager::SetBioGearsSchemaDirectory(std::string const& s)
{
  m_biogears_schema_root = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetBioGearsWorkingDirectory() const
{
  return m_biogears_working_directory;
}
//---------------------------------------------------------------------------
void IOManager::SetBioGearsWorkingDirectory(std::string const& s)
{
  m_biogears_working_directory = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetLogDirectory() const
{
  return m_dirs.log;
}
//---------------------------------------------------------------------------
void IOManager::SetLogDirectory(std::string const& s)
{
  m_dirs.log = s;
}
//---------------------------------------------------------------------------
std::string IOManager::GetResultsDirectory() const
{
  return m_dirs.results;
}
//---------------------------------------------------------------------------
void IOManager::SetResultsDirectory(std::string const& s)
{
  m_dirs.results = s;
}
//---------------------------------------------------------------------------

#ifdef BIOGEARS_IO_PRESENT

//---------------------------------------------------------------------------
//!
//!  Writes the contents to the embded runtime_directory to disk
//!
//!  //TODO: Optional overwrite protection
//!  //TODO: Optional temporary directory construction to prevent partial write
//!  //TODO: Optional empty dir check with cleanup

bool IOManager::generate_runtime_directory(const char* runtime_directory) const
{
  biogears::filesystem::create_directories(runtime_directory);

  bool result = io::generate_xsd_directory(runtime_directory);
  result &= io::generate_config_directory(runtime_directory);
  result &= io::generate_ecg_directory(runtime_directory);
  result &= io::generate_environments_directory(runtime_directory);
  result &= io::generate_nutrition_directory(runtime_directory);
  result &= io::generate_override_directory(runtime_directory);
  result &= io::generate_patients_directory(runtime_directory);
#ifdef BIOGEARS_IO_EMBED_STATES
  result &= generate_states_directory(runtime_directory);
#endif
  result &= io::generate_substances_directory(runtime_directory);

  return result;
}
//---------------------------------------------------------------------------
bool IOManager::does_embedded_file_exist(const char* embeded_path) const
{
  return io::does_embedded_xsd_file_exist(embeded_path)
    || io::does_embedded_config_file_exist(embeded_path)
    || io::does_embedded_ecg_file_exist(embeded_path)
    || io::does_embedded_environments_file_exist(embeded_path)
    || io::does_embedded_nutrition_file_exist(embeded_path)
    || io::does_embedded_override_file_exist(embeded_path)
    || io::does_embedded_patients_file_exist(embeded_path)
#ifdef BIOGEARS_IO_EMBED_STATES
    || does_embedded_states_file_exist(embeded_path)
#endif
    || io::does_embedded_substances_file_exist(embeded_path);
}
//---------------------------------------------------------------------------
char const* IOManager::get_expected_sha1(const char* file) const
{
  char const* result = nullptr;
  result = io::get_xsd_file_sha1(file);
  if (*result) {
    return result;
  }
  result = io::get_config_file_sha1(file);
  if (*result) {
    return result;
  }
  result = io::get_ecg_file_sha1(file);
  if (*result) {
    return result;
  }
  result = io::get_environments_file_sha1(file);
  if (*result) {
    return result;
  }
  result = io::get_nutrition_file_sha1(file);
  if (*result) {
    return result;
  }
  result = io::get_override_file_sha1(file);
  if (*result) {
    return result;
  }
  result = io::get_patients_file_sha1(file);
  if (*result) {
    return result;
  }
#ifdef BIOGEARS_IO_EMBED_STATES
  result = get_states_file_sha1(file);
  if (*result) {
    return result;
  }
#endif
  result = io::get_substances_file_sha1(file);
  if (*result) {
    return result;
  }

  return "";
}
//---------------------------------------------------------------------------
size_t IOManager::get_directory_count() const
{
  size_t sum = biogears::io::config_file_count()
    + biogears::io::ecg_file_count()
    + biogears::io::environments_file_count()
    + biogears::io::nutrition_file_count()
    + biogears::io::override_file_count()
    + biogears::io::patients_file_count()
    + biogears::io::substances_file_count()
    + biogears::io::xsd_file_count();
#ifdef BIOGEARS_IO_EMBED_STATES
  sum += states_file_count();
#endif
  return sum;
}
//---------------------------------------------------------------------------
//!
//!  This function does not provide fall back mechanics. This is intentional
//!  It should be used to compare two file locations exsactly
bool IOManager::validate_file(const char* path, const char* embeded_path) const
{
  if (embeded_path) {
    std::string embeded_hash = get_expected_sha1(embeded_path);
    std::string file_hash = calculate_sha1(path);

    return embeded_hash == file_hash;
  } else {
    if (does_embedded_file_exist(path)) {
      std::string embeded_hash = get_expected_sha1(path);
      std::string file_hash = calculate_sha1(path);

      return embeded_hash == file_hash;
    }
    return false;
  }
}
//---------------------------------------------------------------------------
//!  Returns a char const * of the embeded conents if it exists
//!
//!  \param const char* file  -- Resource path relative to runtime/ matching the embeded file path
//!  \param size_t [OUT]      -- Size of the const char* file return. Zero if nothing was found. (why embedded an empty file?)
//!  \return  char const *    -- Contents of the file "" if nothing was found

char const* IOManager::get_embedded_resource_file(const char* file, std::size_t& size) const
{
  char const* result = nullptr;
  size = 0;
  result = io::get_embedded_xsd_file(file, size);
  if (size > 0) {
    return result;
  }
  result = io::get_embedded_config_file(file, size);
  if (size > 0) {
    return result;
  }
  result = io::get_embedded_ecg_file(file, size);
  if (size > 0) {
    return result;
  }
  result = io::get_embedded_environments_file(file, size);
  if (size > 0) {
    return result;
  }
  result = io::get_embedded_nutrition_file(file, size);
  if (size > 0) {
    return result;
  }
  result = io::get_embedded_override_file(file, size);
  if (size > 0) {
    return result;
  }
  result = io::get_embedded_patients_file(file, size);
  if (size > 0) {
    return result;
  }
#ifdef BIOGEARS_IO_EMBED_STATES
  result = get_embedded_states_file(file, size);
  if (size > 0) {
    return result;
  }
#endif
  result = io::get_embedded_substances_file(file, size);
  if (size > 0) {
    return result;
  }
  return nullptr;
}
//---------------------------------------------------------------------------
size_t IOManager::get_embedded_resource_file_size(const char* file) const
{
  size_t size = 0;
  size = io::get_embedded_xsd_file_size(file);
  if (size > 0) {
    return size;
  }
  size = io::get_embedded_config_file_size(file);
  if (size > 0) {
    return size;
  }
  size = io::get_embedded_ecg_file_size(file);
  if (size > 0) {
    return size;
  }
  size = io::get_embedded_environments_file_size(file);
  if (size > 0) {
    return size;
  }
  size = io::get_embedded_nutrition_file_size(file);
  if (size > 0) {
    return size;
  }
  size = io::get_embedded_override_file_size(file);
  if (size > 0) {
    return size;
  }
  size = io::get_embedded_patients_file_size(file);
  if (size > 0) {
    return size;
  }
#ifdef BIOGEARS_IO_EMBED_STATES
  size = get_embedded_states_file_size(file);
  if (size > 0) {
    return size;
  }
#endif
  size = io::get_embedded_substances_file_size(file);
  if (size > 0) {
    return size;
  }
  return size;
}
#endif
//---------------------------------------------------------------------------
std::string IOManager::find_resource_file(char const* file) const
{
  //!  Look Second in CWD  FILE
  //!  Look First in BIOGEARS_DATA_ROOT
  //!  Look in BIOGEARS_INSTALL_ROOT <determined by cmake>
  //!  Look in the LIBRARY

  //Step 1 : Current Working Directory
  size_t content_size = 0;
  filesystem::path test_location { m_biogears_working_directory };
  test_location /= file;
  if (test_location.exists() && test_location.is_file()) {
    return test_location.string();
  }

  //Step 2 : Alternative CWD
  if (!m_biogears_data_root.empty()) {
    test_location = m_biogears_data_root;
    test_location /= file;
    if (test_location.exists() && test_location.is_file()) {
      return test_location.string();
    }
  }

  if (!m_biogears_schema_root.empty()) {
    test_location = m_biogears_schema_root;
    test_location /= file;
    if (test_location.exists() && test_location.is_file()) {
      return test_location.string();
    }
  }

  //Step 3: ENV{BIOGEARS_DATA_ROOT} ENV{BIOGEARS_SCHEMA_ROOT}
  char* data_root = getenv("BIOGEARS_DATA_ROOT");
  if (data_root != nullptr) {
    test_location = data_root;
    test_location /= file;
    if (test_location.exists() && test_location.is_file()) {
      return test_location.string();
    }
  }
  char* schema_root = getenv("BIOGEARS_SCHEMA_ROOT");
  if (schema_root != nullptr) {
    test_location = schema_root;
    test_location /= file;
    if (test_location.exists() && test_location.is_file()) {
      return test_location.string();
    }
  }
  //Step 3: BIOGEARS_INSTALL_ROOT
  std::string install_root;
#ifdef _WIN32
  //TODO: ALSO CHECK CMAKE_INSTALL_PREFIX as generated by CMAKE
  install_root = getenv("APPDATA");
  install_root += "/biogears/";
#else
  install_root = "~/.biogears/";
#endif
  install_root += rev_tag();
  if (install_root != "") {
    test_location = install_root;
    test_location /= file;
    if (test_location.exists() && test_location.is_file()) {
      return test_location.string();
    }
  }

  return "";
}
size_t IOManager::read_resource_file(char const*
                                       file,
                                     char* buffer, size_t buffer_size) const
{
  //!  Look Second in CWD  FILE
  //!  Look First in BIOGEARS_DATA_ROOT
  //!  Look in BIOGEARS_INSTALL_ROOT <determined by cmake>
  //!  Look in the LIBRARY

  //Step 1 : Current Working Directory
  size_t content_size = 0;
  biogears::filesystem::path test_location { find_resource_file(file) };
  if (test_location.exists() && test_location.is_file()) {
    std::ifstream resource_file { test_location.str(), std::ios::in };
    if (resource_file.is_open()) {
      resource_file.seekg(0, resource_file.end);
      content_size = resource_file.tellg();
      resource_file.seekg(0, resource_file.beg);
    }
    if (content_size < buffer_size) {
      resource_file.read(buffer, content_size);
      return content_size;
    }
  }

  //#ifdef BIOGEARS_IO_PRESENT
  //  //Step 4: Embeded Content
  //  auto embeded_content = get_embedded_resource_file(file, content_size);
  //  if (content_size > 0 && content_size < buffer_size) {
  //    memcpy(buffer, embeded_content, content_size);
  //  }
  //#endif

  return content_size;
}
//---------------------------------------------------------------------------
std::string IOManager::calculate_sha1(const char* path) const
{
  std::string content;
  std::string result = std::string(40, '\0');
  io::sha1 hash;

  std::ifstream file { path, std::ios::in | std::ios::binary };
  if (file) {

    auto start = file.tellg();
    file.seekg(0, file.end);
    auto end = file.tellg();
    auto length = end - start;
    file.seekg(0, file.beg);

    content.resize(length);
    file.read(&content[0], length);
    file.close();

    hash.add(&content[0]);
    hash.finalize();
    hash.print_hex(&result[0]);
  }

  return result;
}
//---------------------------------------------------------------------------
std::string IOManager::calculate_sha1(const char* buffer, size_t buffer_size) const
{
  std::string result = std::string(40, '\0');
  io::sha1 hash;

  hash.add(buffer, (uint32_t)buffer_size);
  hash.finalize();
  hash.print_hex(&result[0]);

  return result;
}
//-----------------------------------------------------------------------------

} // namespace biogears
