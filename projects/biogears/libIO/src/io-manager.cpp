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

#include <biogears/io/directories/config.h>
#include <biogears/io/directories/ecg.h>
#include <biogears/io/directories/environments.h>
#include <biogears/io/directories/nutrition.h>
#include <biogears/io/directories/override.h>
#include <biogears/io/directories/patients.h>
#include <biogears/io/directories/xsd.h>
#include <biogears/io/io-manager.h>

#ifdef IO_EMBED_STATES
#include <biogears/io/directories/states.h>
#endif

#include <biogears/io/directories/substances.h>

namespace biogears {
namespace io {
  //!
  //!  Writes the contents to the embded runtime_directory to disk
  //!
  //!  //TODO: Optional overwrite protection
  //!  //TODO: Optional temporary directory construction to prevent partial write
  //!  //TODO: Optional empty dir check with cleanup

  bool IOManager::generate_runtime_directory(const char* file)
  {
    biogears::filesystem::create_directories(file);

    biogears::filesystem::path runtime_directory { file };

    bool result = generate_xsd_directory(runtime_directory / "xsd");
    result &= generate_config_directory(runtime_directory / "config");
    result &= generate_ecg_directory(runtime_directory / "directory");
    result &= generate_environments_directory(runtime_directory / "environments");
    result &= generate_nutrition_directory(runtime_directory / "nutrition");
    result &= generate_override_directory(runtime_directory / "override");
    result &= generate_patients_directory(runtime_directory / "patients");
#ifdef IO_EMBED_STATES
    result &= generate_states_directory(runtime_directory / "states");
#endif
    result &= generate_substances_directory(runtime_directory / "substances");

    return result;
  }
  //---------------------------------------------------------------------------
  bool IOManager::does_embedded_file_exist(const char* file)
  {

    biogears::filesystem::path runtime_directory { file };

    return does_embedded_xsd_file_exist(runtime_directory / "xsd")
      || does_embedded_config_file_exist(runtime_directory / "config")
      || does_embedded_ecg_file_exist(runtime_directory / "directory")
      || does_embedded_environments_file_exist(runtime_directory / "environments")
      || does_embedded_nutrition_file_exist(runtime_directory / "nutrition")
      || does_embedded_override_file_exist(runtime_directory / "override")
      || does_embedded_patients_file_exist(runtime_directory / "patients")
#ifdef IO_EMBED_STATES
      || does_embedded_states_file_exist(runtime_directory / "states")
#endif
      || does_embedded_substances_file_exist(runtime_directory / "substances");
  }
  //---------------------------------------------------------------------------
  size_t IOManager::find_resource_file(char const* file, char* buffer, size_t buffer_size)
  {

    size_t bytes_written = 0;
    if (bytes_written = find_xsd_file(file, _biogears_schema_root, buffer, buffer_size)) {
      return bytes_written;
    }

    if (bytes_written = find_config_file(file, _biogears_data_root, buffer, buffer_size)) {
      return bytes_written;
    }

    if (bytes_written = find_ecg_file(file, _biogears_data_root, buffer, buffer_size)) {
      return bytes_written;
    }

    if (bytes_written = find_environment_file(file, _biogears_data_root, buffer, buffer_size)) {
      return bytes_written;
    }

    if (bytes_written = find_nutrition_file(file, _biogears_data_root, buffer, buffer_size)) {
      return bytes_written;
    }

    if (bytes_written = find_override_file(file, _biogears_data_root, buffer, buffer_size)) {
      return bytes_written;
    }

    if (bytes_written = find_patients_file(file, _biogears_data_root, buffer, buffer_size)) {
      return bytes_written;
    }
#ifdef IO_EMBED_STATES
    if (bytes_written = find_states_file(file, _biogears_data_root, buffer, buffer_size)) {
      return bytes_written;
    }
#endif
    if (bytes_written = find_substances_file(file, _biogears_data_root, buffer, buffer_size)) {
      return bytes_written;
    }
    return 0;
  }
  //---------------------------------------------------------------------------
  char const* IOManager::get_sha1(const char* file)
  {
    char const* result = nullptr;
    if (result = get_xsd_file_sha1(file)) {
      return result;
    }

    if (result = get_config_file_sha1(file)) {
      return result;
    }

    if (result = get_ecg_file_sha1(file)) {
      return result;
    }

    if (result = get_environments_file_sha1(file)) {
      return result;
    }

    if (result = get_nutrition_file_sha1(file)) {
      return result;
    }

    if (result = get_override_file_sha1(file)) {
      return result;
    }

    if (result = get_patients_file_sha1(file)) {
      return result;
    }
#ifdef IO_EMBED_STATES
    if (result = get_states_file_sha1(file)) {
      return result;
    }
#endif
    if (result = get_substances_file_sha1(file)) {
      return result;
    }

    return nullptr;
  }
  //---------------------------------------------------------------------------
  char const* IOManager::get_embedded_resource_file(const char* file)
  {
    char const* result = nullptr;
    if (result = get_embedded_xsd_file(file)) {
      return result;
    }

    if (result = get_embedded_config_file(file)) {
      return result;
    }

    if (result = get_embedded_ecg_file(file)) {
      return result;
    }

    if (result = get_embedded_environments_file(file)) {
      return result;
    }

    if (result = get_embedded_nutrition_file(file)) {
      return result;
    }

    if (result = get_embedded_override_file(file)) {
      return result;
    }

    if (result = get_embedded_patients_file(file)) {
      return result;
    }
#ifdef IO_EMBED_STATES
    if (result = get_embedded_states_file(file)) {
      return result;
    }
#endif
    if (result = get_embedded_substances_file(file)) {
      return result;
    }
    return nullptr;
  }

}
}