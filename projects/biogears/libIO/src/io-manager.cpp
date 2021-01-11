#pragma once

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

#include <direct.h>

#include <biogears/filesystem/path.h>

#include <biogears/io/io-manager.h>
#include <biogears/io/directories/xsd.h>
#include <biogears/io/directories/config.h>
#include <biogears/io/directories/ecg.h>
#include <biogears/io/directories/environments.h>
#include <biogears/io/directories/nutrition.h>
#include <biogears/io/directories/override.h>
#include <biogears/io/directories/patients.h>
#include <biogears/io/directories/states.h>
#include <biogears/io/directories/substances.h>

namespace biogears {
  namespace io {
    //!
    //!  Writes the contents to the embded runtime_directory to disk
    //!  
    //!  //TODO: Optional overwrite protection
    //!  //TODO: Optional temporary directory construction to prevent partial write
    //!  //TODO: Optional empty dir check with cleanup 


    bool  IOManager::generate_runtime_directory(const char* file){
        biogears::filesystem::create_directories(file);

        biogears::filesystem::path runtime_directory { file };

        bool result = generate_xsd_directory(runtime_directory / "xsd");
        result &= generate_config_directory(runtime_directory / "config");
        result &= generate_ecg_directory(runtime_directory / "directory");
        result &= generate_environments_directory(runtime_directory / "environments");
        result &= generate_nutrition_directory(runtime_directory / "nutrition");
        result &= generate_override_directory(runtime_directory / "override");
        result &= generate_patients_directory(runtime_directory / "patients");
        result &= generate_states_directory(runtime_directory / "states");
        result &= generate_substances_directory(runtime_directory / "substances");

        return result;
    }
    //---------------------------------------------------------------------------
    bool  IOManager::does_embeded_file_exists(const char* file){

      
      biogears::filesystem::path runtime_directory { file };


      return does_embeded_xsd_file_exists(runtime_directory / "xsd")
      || does_embeded_config_file_exists(runtime_directory / "config")
      || does_embeded_ecg_file_exists(runtime_directory / "directory")
      || does_embeded_environments_file_exists(runtime_directory / "environments")
      || does_embeded_nutrition_file_exists(runtime_directory / "nutrition")
      || does_embeded_override_file_exists(runtime_directory / "override")
      || does_embeded_patients_file_exists(runtime_directory / "patients")
      || does_embeded_states_file_exists(runtime_directory / "states")
      || does_embeded_substances_file_exists(runtime_directory / "substances");

    }
    //---------------------------------------------------------------------------
    char const *   IOManager::find_resource_file(const char* file){

      char const* result = nullptr;
      if (result = find_xsd_file(file)) {
        return result;
      }
     
      if (result = find_config_file(file)) {
        return result;
      }

      if (result = find_ecg_file(file)) {
        return result;
      }

      if (result = find_environments_file(file)) {
        return result;
      }

      if (result = find_nutrition_file(file)) {
        return result;
      }

      if (result = find_override_file(file)) {
        return result;
      }

      if (result = find_patients_file(file)) {
        return result;
      }

      if (result = find_states_file(file)) {
        return result;
      }

      if (result = find_substances_file(file)) {
        return result;
      }
    }
    //---------------------------------------------------------------------------
    char const *   IOManager::get_hash (const char* file){
      char const* result = nullptr;
      if (result = get_xsd_file_hash(file)) {
        return result;
      }

      if (result = get_config_file_hash(file)) {
        return result;
      }

      if (result = get_ecg_file_hash(file)) {
        return result;
      }

      if (result = get_environments_file_hash(file)) {
        return result;
      }

      if (result = get_nutrition_file_hash(file)) {
        return result;
      }

      if (result = get_override_file_hash(file)) {
        return result;
      }

      if (result = get_patients_file_hash(file)) {
        return result;
      }

      if (result = get_states_file_hash(file)) {
        return result;
      }

      if (result = get_substances_file_hash(file)) {
        return result;
      }
    }
    //---------------------------------------------------------------------------
    char const *  IOManager::get_embeded_resource_file( const char* file){
      char const* result = nullptr;
      if (result = get_embeded_xsd_file(file)) {
        return result;
      }

      if (result = get_embeded_config_file(file)) {
        return result;
      }

      if (result = get_embeded_ecg_file(file)) {
        return result;
      }

      if (result = get_embeded_environments_file(file)) {
        return result;
      }

      if (result = get_embeded_nutrition_file(file)) {
        return result;
      }

      if (result = get_embeded_override_file(file)) {
        return result;
      }

      if (result = get_embeded_patients_file(file)) {
        return result;
      }

      if (result = get_embeded_states_file(file)) {
        return result;
      }

      if (result = get_embeded_substances_file(file)) {
        return result;
      }
    }

  }
}