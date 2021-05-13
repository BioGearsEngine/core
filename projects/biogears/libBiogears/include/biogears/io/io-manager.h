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

#include <biogears/filesystem/path.h>
#include <vector>
#include <array>
//!
//!  Functions for finding resource files that were part of the current biogears release
//!  These functions are encoded as part of libbiogears_io
//!
//!  Functions are exposed as either biogears::filesystem::path or char const *
//!  TODO: Migrate Biogears C++17 and replace biogears::filesystem with std::filesystem

namespace biogears {

BIOGEARS_API std::vector<filesystem::path> ListFiles(std::string const& dir, std::string const& regex, bool recurse = false);

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

  enum Fallbacks {
     CWD = 0,
     DATA,
     SCHEMA,
     HOME,
     INSTALL,
     TOTAL_FALLBACKS
  };
  std::array<std::string, TOTAL_FALLBACKS> m_prefixs;

  struct IoConfig {
    std::string log = "./";
    std::string results = "./";
    std::string config = "config/";
    std::string ecg = "ecg/";
    std::string environments = "environments/";
    std::string nutrition = "nutrition/";
    std::string override_dir = "override/"; //<! Override is a keyword in most languages so we break convention
    std::string patients = "patients/";
    std::string states = "states/";
    std::string substances = "substances/";
    std::string scenarios = "Scenarios/";
  } m_dirs;

public:
  BIOGEARS_API IOManager();
  BIOGEARS_API IOManager(char const* working_directory);
  BIOGEARS_API ~IOManager();
  //!
  //!  Configuration Functions allow users to override directories which
  //!  contain various biogears required input files. If the directories
  //!  are relative for the system they will be appended to BIOGEARS_DATA_ROOT
  //!

  BIOGEARS_API std::string ResolveConfigFileLocation(std::string const& filename); //! Use current state to determine the destination of a config file.
  BIOGEARS_API std::string GetConfigDirectory() const; //! The directory which contains CDM::PhysiologyEngine*StabilizationData files.
  BIOGEARS_API void SetConfigDirectory(std::string const& s); //! Defaults to config
  BIOGEARS_API filesystem::path FindConfigFile(char const*) const;

  BIOGEARS_API std::string ResolveEcgFileLocation(std::string const& filename); //! Use current state to determine the destination of a config file.
  BIOGEARS_API std::string GetEcgDirectory() const; //! The directory which contains  CDM::ElectroCardioGramWaveformInterpolatorData files.
  BIOGEARS_API void SetEcgDirectory(std::string const& s); //! Defaults to ecg
  BIOGEARS_API filesystem::path FindEcgFile(char const*) const;

  BIOGEARS_API std::string ResolveEnvironmentFileLocation(std::string const& filename); //! Use current state to determine the destination of a config file.
  BIOGEARS_API std::string GetEnvironmentsDirectory() const; //! The directory which contains  CDM::EnvironmentalConditionsData files.
  BIOGEARS_API void SetEnvironmentsDirectory(std::string const& s); //! Defaults to environments
  BIOGEARS_API filesystem::path FindEnvironmentFile(char const*) const;

  BIOGEARS_API std::string ResolveNutritionFileLocation(std::string const& filename); //! Use current state to determine the destination of a config file.
  BIOGEARS_API std::string GetNutritionDirectory() const; //! The directory which contains  CDM::NutritionData files.
  BIOGEARS_API void SetNutritionDirectory(std::string const& s); //! Defaults to nutrition
  BIOGEARS_API filesystem::path FindNutritionFile(char const*) const;

  BIOGEARS_API std::string ResolveOverrideFileLocation(std::string const& filename); //! Use current state to determine the destination of a config file.
  BIOGEARS_API std::string GetOverrideDirectory() const; //! The directory which contains  CDM::BioGearsOverrideData files.
  BIOGEARS_API void SetOverrideDirectory(std::string const& s); //! Defaults to override
  BIOGEARS_API filesystem::path FindOverrideFile(char const*) const;

  BIOGEARS_API std::string ResolvePatientFileLocation(std::string const& filename); //! Use current state to determine the destination of a config file.
  BIOGEARS_API std::string GetPatientsDirectory() const; //! The directory which contains  CDM::PatientData files.
  BIOGEARS_API void SetPatientsDirectory(std::string const& s); //! Defaults to patients
  BIOGEARS_API filesystem::path FindPatientFile(char const*) const;

  BIOGEARS_API std::string ResolveStateFileLocation(std::string const& filename); //! Use current state to determine the destination of a config file.
  BIOGEARS_API std::string GetStatesDirectory() const; //! The directory which contains  CDM::BioGearsStateData files.
  BIOGEARS_API void SetStatesDirectory(std::string const& s); //! Defaults to states
  BIOGEARS_API filesystem::path FindStateFile(char const*) const;

  BIOGEARS_API std::string ResolveSubstanceFileLocation(std::string const& filename); //! Use current state to determine the destination of a config file.
  BIOGEARS_API std::string GetSubstancesDirectory() const; //! The directory which contains  CDM::SubstanceData files.
  BIOGEARS_API void SetSubstancesDirectory(std::string const& s); //! Defaults to substances
  BIOGEARS_API std::vector<filesystem::path> FindAllSubstanceFiles() const;
  BIOGEARS_API filesystem::path FindSubstanceFile(const char*) const;

  BIOGEARS_API std::string ResolveScenarioFileLocation(std::string const& filename); //! Use current state to determine the destination of a config file.
  BIOGEARS_API std::string GetScenariosDirectory() const; //! The directory which contains  CDM::ScenarioData files.
  BIOGEARS_API void SetScenariosDirectory(std::string const& s); //! Defaults to Scenarios
  BIOGEARS_API filesystem::path FindScenarioFile(const char*) const;

  BIOGEARS_API std::string GetBioGearsDataRootDirectory() const; //! The root directory of pulled from ENV{BIOGEARS_DATA_ROOT} which all but xsd paths are appended. If it does not exist it will be set to the PWD.
  BIOGEARS_API void SetBioGearsDataRootDirectory(std::string const& s);

  BIOGEARS_API std::string GetBioGearsSchemaDirectory() const; //! The root directory of pulled from ENV{BIOGEARS_SCHEMA_ROOT} which all but xsd paths are appended.  If not existant will be set to DataRoot
  BIOGEARS_API void SetBioGearsSchemaDirectory(std::string const& s);

  BIOGEARS_API std::string GetBioGearsWorkingDirectory() const; //! Sets the Working directory that all new files will be relative to if given relative directories
  BIOGEARS_API void SetBioGearsWorkingDirectory(std::string const& s); //! Defaults to ENV{PWD}

  BIOGEARS_API std::string ResolveLogFileLocation(std::string const& filename); //! Use current state to determine the destination of a config file.
  BIOGEARS_API std::string GetLogDirectory() const; //! The directory for which Logs will be relative to
  BIOGEARS_API void SetLogDirectory(std::string const& s); //! Defaults to the executables current working directory

  BIOGEARS_API std::string ResolveResultsFileLocation(std::string const& filename); //! Use current state to determine the destination of a Result file.
  BIOGEARS_API std::string GetResultsDirectory() const; //! The directory for which DataTracks and Assessments will be recoreded
  BIOGEARS_API void SetResultsDirectory(std::string const& s); //! Defaults to the executables current working directory

#ifdef BIOGEARS_IO_PRESENT
  //!
  //! API Avaliable when BIOGEARS_IO_PRESENT is true
  //! Exposes probing of embedded IO files
  BIOGEARS_API bool generate_runtime_directory(const char* file) const; //!< Recreates the embeded runtime directory at the path given.
  BIOGEARS_API bool does_embedded_file_exist(const char* file) const; //!< When given a relative path to data_root (e.g ecg/StandardECG.xml ) will return if it is part of the embedded library
  BIOGEARS_API char const* get_embedded_resource_file(const char* file, std::size_t& size) const; //!< Return embeded cstring which represents the requested embeded string
  BIOGEARS_API size_t get_embedded_resource_file_size(const char* file) const; //!< Return embeded cstring which represents the requested embeded string
  BIOGEARS_API char const* get_expected_sha1(const char* file) const; //!< When given a relative path to data_root will return the sha1 data for the matching file
  BIOGEARS_API size_t get_directory_count() const; //!< Returns the list of directories embeded IE 1 for each cpp file in libIO/src/directories
  BIOGEARS_API bool validate_file(const char* path, const char* embeded_path = nullptr) const; //!< Returns true if the sha1 of path is equal to the embeded files
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
  //!
  BIOGEARS_API std::string find_resource_file(char const* file) const;
  BIOGEARS_API size_t read_resource_file(char const* file, char* buffer, size_t buffer_size) const;

  //!
  //! Calculates the SHA1 of the filepath given or the buffer provided
  //!
  BIOGEARS_API std::string calculate_sha1(const char* path) const;
  BIOGEARS_API std::string calculate_sha1(const char* buffer, size_t buffer_size) const;

private:
  std::vector<filesystem::path> find_files(std::string suffix, std::string regex, bool recurse = false) const;
};
}
