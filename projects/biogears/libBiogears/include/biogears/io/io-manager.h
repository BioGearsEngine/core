
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

#include <biogears/exports.h>
//!
//!  Functions for finding resource files that were part of the current biogears release
//!  These functions are encoded as part of libbiogears_io
//!
//!  Functions are exposed as either biogears::filesystem::path or char const *
//!  TODO: Migrate Biogears C++17 and replace biogears::filesystem with std::filesystem

namespace biogears {
namespace io {
  //!
  //!  IOManager is a class for interacting with the static functions in biogears::io
  //!  It simplfies interaction with the various resource_file_types at the cost of running
  //!  If Checks across the entire embeded library when testing for membership
  //!
  //!  TODO: Implmement behavior control
  //!
  //!
  //!  Look Second in CWD  FILE
  //!  Look First in BIOGEARS_DATA_ROOT
  //!  Look in BIOGEARS_INSTALL_ROOT <determined by cmake>
  //!  Look in the LIBRARY

  class IOManager {
  private:

    std::string _biogears_install_root;
    std::string _biogears_data_root;
    std::string _biogears_schema_root;
    struct IoConfig{
      std::string config = "config/";
      std::string ecg = "ecg/";
      std::string environments = "environments/";
      std::string nutrition = "nutrition/";
      std::string override = "override/";
      std::string patients = "patients/";
      std::string states = "states/";
      std::string substances = "substances/";
      std::string scenarios = "Scenarios/";
    } _dirs;

  public:
    IOManager() = default;
    IOManager(char const* dataroot, char const* schemaroot = nullptr);
    //!
    //!  Configuration Functions allow users to override directories which
    //!  contain various biogears required input files. If the directories
    //!  are relative for the system they will be appended to BIOGEARS_DATA_ROOT
    //!

    BIOGEARS_API std::string GetConfigDirectory() const; //! The directory which contains CDM::PhysiologyEngine*StabilizationData files.
    BIOGEARS_API void SetConfigDirectory( std::string const& s); //! Defaults to config

    BIOGEARS_API std::string GetEcgDirectory() const; //! The directory which contains  CDM::ElectroCardioGramWaveformInterpolatorData files.
    BIOGEARS_API void SetEcgDirectory( std::string const& s); //! Defaults to ecg

    BIOGEARS_API std::string GetEnvironmentsDirectory() const; //! The directory which contains  CDM::EnvironmentalConditionsData files.
    BIOGEARS_API void SetEnvironmentsDirectory( std::string const& s); //! Defaults to environments

    BIOGEARS_API std::string GetNutritionDirectory() const; //! The directory which contains  CDM::NutritionData files.
    BIOGEARS_API void SetNutritionDirectory( std::string const& s); //! Defaults to nutrition

    BIOGEARS_API std::string GetOverrideDirectory() const; //! The directory which contains  CDM::BioGearsOverrideData files.
    BIOGEARS_API void SetOverrideDirectory( std::string const& s); //! Defaults to override

    BIOGEARS_API std::string GetPatientsDirectory() const; //! The directory which contains  CDM::PatientData files.
    BIOGEARS_API void SetPatientsDirectory( std::string const& s); //! Defaults to patients

    BIOGEARS_API std::string GetStatesDirectory() const; //! The directory which contains  CDM::BioGearsStateData files.
    BIOGEARS_API void SetStatesDirectory( std::string const& s); //! Defaults to states

    BIOGEARS_API std::string GetSubstancesDirectory() const; //! The directory which contains  CDM::SubstanceData files.
    BIOGEARS_API void SetSubstancesDirectory( std::string const& s); //! Defaults to substances

    BIOGEARS_API std::string GetScenariosDirectory() const; //! The directory which contains  CDM::ScenarioData files.
    BIOGEARS_API void SetScenariosDirectory( std::string const& s); //! Defaults to Scenarios

    BIOGEARS_API std::string GetBioGearsDataRootDirectory() const; //! The root directory of pulled from ENV{BIOGEARS_DATA_ROOT} which all but xsd paths are appended. If it does not exist it will be set to the PWD.
    BIOGEARS_API void SetBioGearsDataRootDirectory( std::string const& s);

    BIOGEARS_API std::string GetBioGearsSchemaDirectory() const; //! The root directory of pulled from ENV{BIOGEARS_SCHEMA_ROOT} which all but xsd paths are appended.  If not existant will be set to DataRoot
    BIOGEARS_API void SetBioGearsSchemaDirectory( std::string const& s);

#ifdef BIOGEARS_IO_PRESENT
    //!
    //! API Avaliable when BIOGEARS_IO_PRESENT is true
    //! Exposes probing of embedded IO files
    BIOGEARS_API bool generate_runtime_directory(const char* file) const;        //!< Recreates the embeded runtime directory at the path given.
    BIOGEARS_API bool does_embedded_file_exist(const char* file) const;          //!< When given a relative path to data_root (e.g ecg/StandardECG.xml ) will return if it is part of the embedded library
    BIOGEARS_API char const* get_expected_sha1(const char* file) const;          //!< When given a relative path to data_root will return the sha1 data for the matching file
    BIOGEARS_API size_t get_directory_count() const;                             //!< Returns the list of directories embeded IE 1 for each cpp file in libIO/src/directories
    BIOGEARS_API bool validate_file(const char* path, const char* embeded_path = nullptr) const;  //!< Returns true if the sha1 of path is equal to the embeded files
#endif

    //!
    //! If provided a file path relative to a biogears_data_root or biogears_scema_root. 
    //! This function will test the following locations.
    //!  ENV{PWD}/${file}
    //!  _biogears_data_root/file
    //!  _biogears_install_root/file<determined by cmake>
    //!  BIOGEARS_IO/file
    //! 
    //! If provided an absolute file returns file if it exists. 
    //! 
    //! \param file IN c_string of resource file which needs to be resovled
    //! \param buffer INOUT memory buffer to be populated with data
    //! \param buffer_size IN maximum size of data that can be written to buffer
    //! \return size_t  - When content_size < buffer_size then this is the number of bytes written to buffer.  
    //!                   When content_size >= buffer_size this is the amount of data needed to resolove the resource file
    //!                   buffer[0] will be eof in this case
    BIOGEARS_API size_t find_resource_file(char const* file, char* buffer, size_t buffer_size) const;

    //!
    //! Calculates the SHA1 of the filepath given or the buffer provided
    //! 
    BIOGEARS_API std::string calculate_sha1(const char* path) const;
    BIOGEARS_API std::string calculate_sha1(const char* buffer, size_t buffer_size) const;



    BIOGEARS_API const char* get_embedded_resource_file(const char* file, size_t& content_size) const;


  };
}
}
