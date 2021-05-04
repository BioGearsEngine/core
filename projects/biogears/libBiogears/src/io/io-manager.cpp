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
#include <biogears/filesystem/path.h>

#include <biogears/io/constants.h>

#include <biogears/io/directories/config.h>
#include <biogears/io/directories/ecg.h>
#include <biogears/io/directories/environments.h>
#include <biogears/io/directories/nutrition.h>
#include <biogears/io/directories/override.h>
#include <biogears/io/directories/patients.h>
#include <biogears/io/directories/xsd.h>
#include <biogears/io/io-manager.h>

#include <biogears/io/sha1.h>

#ifdef IO_EMBED_STATES
#include <biogears/io/directories/states.h>
#endif

#include <biogears/io/directories/substances.h>

#include <fstream>

namespace biogears {
namespace io {

  IOManager::IOManager(char const* dataroot, char const* schemaroot)
    : _biogears_data_root(dataroot)
  {
    _biogears_schema_root = (schemaroot) ? schemaroot : _biogears_data_root;
  }
  std::string IOManager::GetConfigDirectory() const
  {
    return _dirs.config;
  }
  //---------------------------------------------------------------------------
  void IOManager::SetConfigDirectory(std::string const& s)
  {
    _dirs.config = s;
  }
  //---------------------------------------------------------------------------
  std::string IOManager::GetEcgDirectory() const
  {
    return _dirs.ecg;
  }
  //---------------------------------------------------------------------------
  void IOManager::SetEcgDirectory(std::string const& s)
  {
    _dirs.ecg = s;
  }
  //---------------------------------------------------------------------------
  std::string IOManager::GetEnvironmentsDirectory() const
  {
    return _dirs.environments;
  }
  //---------------------------------------------------------------------------
  void IOManager::SetEnvironmentsDirectory(std::string const& s)
  {
    _dirs.environments = s;
  }
  //---------------------------------------------------------------------------
  std::string IOManager::GetNutritionDirectory() const
  {
    return _dirs.nutrition;
  }
  //---------------------------------------------------------------------------
  void IOManager::SetNutritionDirectory(std::string const& s)
  {
    _dirs.nutrition = s;
  }
  //---------------------------------------------------------------------------
  std::string IOManager::GetOverrideDirectory() const
  {
    return _dirs.override;
  }
  //---------------------------------------------------------------------------
  void IOManager::SetOverrideDirectory(std::string const& s)
  {
    _dirs.override = s;
  }
  //---------------------------------------------------------------------------
  std::string IOManager::GetPatientsDirectory() const
  {
    return _dirs.patients;
  }
  //---------------------------------------------------------------------------
  void IOManager::SetPatientsDirectory(std::string const& s)
  {
    _dirs.patients = s;
  }
  //---------------------------------------------------------------------------
  std::string IOManager::GetStatesDirectory() const
  {
    return _dirs.states;
  }
  //---------------------------------------------------------------------------
  void IOManager::SetStatesDirectory(std::string const& s)
  {
    _dirs.states = s;
  }
  //---------------------------------------------------------------------------
  std::string IOManager::GetSubstancesDirectory() const
  {
    return _dirs.substances;
  }
  //---------------------------------------------------------------------------
  void IOManager::SetSubstancesDirectory(std::string const& s)
  {
    _dirs.substances = s;
  }
  //---------------------------------------------------------------------------
  std::string IOManager::GetScenariosDirectory() const
  {
    return _dirs.scenarios;
  }
  //---------------------------------------------------------------------------
  void IOManager::SetScenariosDirectory(std::string const& s)
  {
    _dirs.scenarios = s;
  }
  //---------------------------------------------------------------------------
  std::string IOManager::GetBioGearsDataRootDirectory() const
  {
    return _biogears_data_root;
  }
  //---------------------------------------------------------------------------
  void IOManager::SetBioGearsDataRootDirectory(std::string const& s)
  {
    _biogears_data_root = s;
  }
  //---------------------------------------------------------------------------
  std::string IOManager::GetBioGearsSchemaDirectory() const
  {
    return _biogears_schema_root;
  }
  //---------------------------------------------------------------------------
  void IOManager::SetBioGearsSchemaDirectory(std::string const& s)
  {
    _biogears_schema_root = s;
  }
  //---------------------------------------------------------------------------
  //!
  //!  Writes the contents to the embded runtime_directory to disk
  //!
  //!  //TODO: Optional overwrite protection
  //!  //TODO: Optional temporary directory construction to prevent partial write
  //!  //TODO: Optional empty dir check with cleanup

  bool IOManager::generate_runtime_directory(const char* file) const
  {
    biogears::filesystem::create_directories(file);

    biogears::filesystem::path runtime_directory { file };

    bool result = generate_xsd_directory(runtime_directory);
    result &= generate_config_directory(runtime_directory);
    result &= generate_ecg_directory(runtime_directory);
    result &= generate_environments_directory(runtime_directory);
    result &= generate_nutrition_directory(runtime_directory);
    result &= generate_override_directory(runtime_directory);
    result &= generate_patients_directory(runtime_directory);
#ifdef IO_EMBED_STATES
    result &= generate_states_directory(runtime_directory);
#endif
    result &= generate_substances_directory(runtime_directory);

    return result;
  }
  //---------------------------------------------------------------------------
  bool IOManager::does_embedded_file_exist(const char* file) const
  {

    biogears::filesystem::path embeded_path { file };

    return does_embedded_xsd_file_exist(embeded_path)
      || does_embedded_config_file_exist(embeded_path)
      || does_embedded_ecg_file_exist(embeded_path)
      || does_embedded_environments_file_exist(embeded_path)
      || does_embedded_nutrition_file_exist(embeded_path)
      || does_embedded_override_file_exist(embeded_path)
      || does_embedded_patients_file_exist(embeded_path)
#ifdef IO_EMBED_STATES
      || does_embedded_states_file_exist(embeded_path)
#endif
      || does_embedded_substances_file_exist(embeded_path);
  }
  //---------------------------------------------------------------------------
  size_t IOManager::find_resource_file(char const* file, char* buffer, size_t buffer_size) const
  {

    //!  Look Second in CWD  FILE
    //!  Look First in BIOGEARS_DATA_ROOT
    //!  Look in BIOGEARS_INSTALL_ROOT <determined by cmake>
    //!  Look in the LIBRARY

    //Step 1 : Current Working Directory
    size_t content_size = 0;
    biogears::filesystem::path test_location { file };
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

    //Step 2 : Alternative CWD
    if (!_biogears_data_root.empty()) {
      test_location = _biogears_data_root;
      test_location /= file;
      if (test_location.exists() && test_location.is_file()) {
        std::ifstream resource_file { test_location.str(), std::ios::in };
        if (resource_file.is_open()) {
          resource_file.seekg(0, resource_file.end);
          content_size = resource_file.tellg();
          resource_file.seekg(0, resource_file.beg);
          if (content_size < buffer_size) {
            resource_file.read(buffer, content_size);
            return content_size;
          }
        }
      }
    }

    if (!_biogears_schema_root.empty()) {
      test_location = _biogears_schema_root;
      test_location /= file;
      if (test_location.exists() && test_location.is_file()) {
        std::ifstream resource_file { test_location.str(), std::ios::in };
        if (resource_file.is_open()) {
          resource_file.seekg(0, resource_file.end);
          content_size = resource_file.tellg();
          resource_file.seekg(0, resource_file.beg);
          if (content_size < buffer_size) {
            resource_file.read(buffer, content_size);
            return content_size;
          }
        }
      }
    }

    //Step 3: ENV{BIOGEARS_DATA_ROOT} ENV{BIOGEARS_SCHEMA_ROOT}
    char* data_root = getenv("BIOGEARS_DATA_ROOT");
    if (data_root != nullptr) {
      test_location = data_root;
      test_location /= file;
      if (test_location.exists() && test_location.is_file()) {
        std::ifstream resource_file { test_location.str(), std::ios::in };
        if (resource_file.is_open()) {
          resource_file.seekg(0, resource_file.end);
          content_size = resource_file.tellg();
          resource_file.seekg(0, resource_file.beg);
          if (content_size < buffer_size) {
            resource_file.read(buffer, content_size);
            return content_size;
          }
        }
      }
    }
    char* schema_root = getenv("BIOGEARS_SCHEMA_ROOT");
    if (schema_root != nullptr) {
      test_location = schema_root;
      test_location /= file;
      if (test_location.exists() && test_location.is_file()) {
        std::ifstream resource_file { test_location.str(), std::ios::in };
        if (resource_file.is_open()) {
          resource_file.seekg(0, resource_file.end);
          content_size = resource_file.tellg();
          resource_file.seekg(0, resource_file.beg);
          if (content_size < buffer_size) {
            resource_file.read(buffer, content_size);
            return content_size;
          }
        }
      }
    }
    //Step 3: BIOGEARS_INSTALL_ROOT
    std::string install_root;
#ifdef _WIN32
    install_root = getenv("APPDATA");
    install_root += "/biogears/";
#else
    install_root = "~/.biogears/";
#endif
    install_root += _REV_TAG_;
    if (install_root != "") {
      test_location = install_root;
      test_location /= file;
      if (test_location.exists() && test_location.is_file()) {
        std::ifstream resource_file { test_location.str(), std::ios::in };
        if (resource_file.is_open()) {
          resource_file.seekg(0, resource_file.end);
          content_size = resource_file.tellg();
          resource_file.seekg(0, resource_file.beg);
          if (content_size < buffer_size) {
            resource_file.read(buffer, content_size);
            return content_size;
          }
        }
      }
    }
    //Step 4: Embeded Content
    auto embeded_content = get_embedded_resource_file(file, content_size);
    if (content_size > 0 && content_size < buffer_size) {
      memcpy(buffer, embeded_content, content_size);
    }

    return content_size;
  }
  //---------------------------------------------------------------------------
  char const* IOManager::get_expected_sha1(const char* file) const
  {
    char const* result = nullptr;
    result = get_xsd_file_sha1(file);
    if (*result) {
      return result;
    }
    result = get_config_file_sha1(file);
    if (*result) {
      return result;
    }
    result = get_ecg_file_sha1(file);
    if (*result) {
      return result;
    }
    result = get_environments_file_sha1(file);
    if (*result) {
      return result;
    }
    result = get_nutrition_file_sha1(file);
    if (*result) {
      return result;
    }
    result = get_override_file_sha1(file);
    if (*result) {
      return result;
    }
    result = get_patients_file_sha1(file);
    if (*result) {
      return result;
    }
#ifdef IO_EMBED_STATES
    result = get_states_file_sha1(file);
    if (*result) {
      return result;
    }
#endif
    result = get_substances_file_sha1(file);
    if (*result) {
      return result;
    }

    return "";
  }
  //---------------------------------------------------------------------------
  std::string IOManager::calculate_sha1(const char* path) const
  {
    std::string content;
    std::string result = std::string(40, '\0');
    sha1 hash;

    std::ifstream file { path, std::ios::in | std::ios::binary };
    if (file) {

      auto start = file.tellg();
      file.seekg(0, file.end);
      auto end = file.tellg();
      auto length = end  - start;
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
    sha1 hash;

    hash.add(buffer, buffer_size);
    hash.finalize();
    hash.print_hex(&result[0]);

    return result;
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
    result = get_embedded_xsd_file(file, size);
    if (size > 0) {
      return result;
    }
    result = get_embedded_config_file(file, size);
    if (size > 0) {
      return result;
    }
    result = get_embedded_ecg_file(file, size);
    if (size > 0) {
      return result;
    }
    result = get_embedded_environments_file(file, size);
    if (size > 0) {
      return result;
    }
    result = get_embedded_nutrition_file(file, size);
    if (size > 0) {
      return result;
    }
    result = get_embedded_override_file(file, size);
    if (size > 0) {
      return result;
    }
    result = get_embedded_patients_file(file, size);
    if (size > 0) {
      return result;
    }
#ifdef IO_EMBED_STATES
    result = get_embedded_states_file(file, size);
    if (size > 0) {
      return result;
    }
#endif
    result = get_embedded_substances_file(file, size);
    if (size > 0) {
      return result;
    }
    return nullptr;
  }

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
#ifdef IO_EMBED_STATES
    sum += states_file_count();
#endif
    return sum;
  }

  //-----------------------------------------------------------------------------

} //namespace io
} // namespace biogears