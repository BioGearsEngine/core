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

#include <biogears/io-exports.h>
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
  //!  Look First in BIOGEARS_DATA_ROOT
  //!  Look Second in CWD  FILE
  //!  Look in the LIBRARY

  class IOManager {
  private:
    bool _overwrite_mode = true;
    bool _error_if_not_empty = false;
    bool _transaction_based_writes = false;

    char* _staging_ground = nullptr;
    char* _biogears_data_root = nullptr;
    char* _biogears_schema_root = nullptr;

  public:
    BIOGEARS_IO_API bool generate_runtime_directory(const char* file);
    BIOGEARS_IO_API bool does_embedded_file_exist(const char* file);

    BIOGEARS_IO_API size_t find_resource_file(char const* file, char* buffer, size_t buffer_size);

    BIOGEARS_IO_API char const* get_expected_sha1(const char* file);
    BIOGEARS_IO_API std::string calculate_sha1(const char* path);
    BIOGEARS_IO_API std::string calculate_sha1(const char* buffer, size_t buffer_size);

    BIOGEARS_IO_API bool validate_file(const char* path, const char* embeded_path = nullptr);

    BIOGEARS_IO_API char const* get_embedded_resource_file(const char* file);

    BIOGEARS_IO_API size_t get_directory_count(); //!< Returns the list of directories embeded IE 1 for each cpp file in libIO/src/directories
    BIOGEARS_IO_API char const** get_directory_list();
    //!< Returns a static character array that is hard coded to those expected directories no need for cmake we just have to know to maintain it for now
  private:
	  // config
	  // ecg
	  // environments
	  // nutrition
	  // override
	  // patients
	  // substances
	  // xsd
	const char * resource_files[152] = {"config/DynamicStabilization.xml"
							,"config/TimedStabilization.xml"
							,"ecg/StandardECG.xml"
							,"environments/AnchorageDecember.xml"
							,"environments/AnchorageInside.xml"
							,"environments/CarbonMonoxideAtmospheric.xml"
							,"environments/CheyenneMountainAmbulance.xml"
							,"environments/CheyenneMountainAprilCool.xml"
							,"environments/CheyenneMountainAprilWarm.xml"
							,"environments/CheyenneMountainFireFighter.xml"
							,"environments/ExerciseEnvironment.xml"
							,"environments/Hypobaric3000m.xml"
							,"environments/Hypobaric4000m.xml"
							,"environments/Standard.xml"
							,"environments/StandardEnvironment.xml"
							,"environments/Submerged.xml"
							,"environments/SubmergedFreezing.xml"
							,"nutrition/NoMacros.xml"
							,"nutrition/Soylent.xml"
							,"nutrition/Standard.xml"
							,"nutrition/StandardNutrition.xml"
							,"nutrition/Water.xml"
							,"override/BioGearsOverride.xml"
							,"patients/Bradycardic.xml"
							,"patients/Carol.xml"
							,"patients/Cynthia.xml"
							,"patients/DefaultFemale.xml"
							,"patients/DefaultMale.xml"
							,"patients/DefaultTemplateFemale.xml"
							,"patients/DefaultTemplateMale.xml"
							,"patients/ExtremeFemale.xml"
							,"patients/ExtremeMale.xml"
							,"patients/Gus.xml"
							,"patients/Hassan.xml"
							,"patients/Jane.xml"
							,"patients/Jeff.xml"
							,"patients/Joel.xml"
							,"patients/Nathan.xml"
							,"patients/Overweight.xml"
							,"patients/Rick.xml"
							,"patients/Roy.xml"
							,"patients/SleepDeprived.xml"
							,"patients/Soldier.xml"
							,"patients/StandardFemale.xml"
							,"patients/StandardMale.xml"
							,"patients/Tachycardic.xml"
							,"patients/ToughGirl.xml"
							,"patients/ToughGuy.xml"
							,"patients/Tristan.xml"
							,"patients/Underweight.xml"
							,"substances/Acetaminophen.xml"
							,"substances/Acetoacetate.xml"
							,"substances/Albumin.xml"
							,"substances/Albuterol.xml"
							,"substances/AminoAcids.xml"
							,"substances/Antigen_A.xml"
							,"substances/Antigen_B.xml"
							,"substances/Bicarbonate.xml"
							,"substances/Blood_ABNegative.xml"
							,"substances/Blood_ABPositive.xml"
							,"substances/Blood_ANegative.xml"
							,"substances/Blood_APositive.xml"
							,"substances/Blood_BNegative.xml"
							,"substances/Blood_BPositive.xml"
							,"substances/Blood_ONegative.xml"
							,"substances/Blood_OPositive.xml"
							,"substances/Calcium.xml"
							,"substances/Carbaminohemoglobin.xml"
							,"substances/CarbonDioxide.xml"
							,"substances/CarbonMonoxide.xml"
							,"substances/Carboxyhemoglobin.xml"
							,"substances/Chloride.xml"
							,"substances/Creatinine.xml"
							,"substances/Desflurane.xml"
							,"substances/Epinephrine.xml"
							,"substances/Ertapenem.xml"
							,"substances/Fentanyl.xml"
							,"substances/ForestFireParticulate.xml"
							,"substances/Furosemide.xml"
							,"substances/Globulin.xml"
							,"substances/Glucagon.xml"
							,"substances/Glucose.xml"
							,"substances/Hemoglobin.xml"
							,"substances/Insulin.xml"
							,"substances/Ketamine.xml"
							,"substances/Ketones.xml"
							,"substances/Lactate.xml"
							,"substances/Magnesium.xml"
							,"substances/Midazolam.xml"
							,"substances/Morphine.xml"
							,"substances/Moxifloxacin.xml"
							,"substances/Naloxone.xml"
							,"substances/Nitrogen.xml"
							,"substances/Norepinephrine.xml"
							,"substances/OxyCarbaminohemoglobin.xml"
							,"substances/Oxygen.xml"
							,"substances/Oxyhemoglobin.xml"
							,"substances/Piperacillin.xml"
							,"substances/PiperacillinTazobactam.xml"
							,"substances/Plasma.xml"
							,"substances/PlasmaLyteA.xml"
							,"substances/Platelet.xml"
							,"substances/Potassium.xml"
							,"substances/Pralidoxime.xml"
							,"substances/Prednisone.xml"
							,"substances/Propofol.xml"
							,"substances/RedBloodCell.xml"
							,"substances/RingersLactate.xml"
							,"substances/Rocuronium.xml"
							,"substances/Saline.xml"
							,"substances/SalineSlowDrip.xml"
							,"substances/Sarin.xml"
							,"substances/Sodium.xml"
							,"substances/Succhinylcholine.xml"
							,"substances/Tazobactam.xml"
							,"substances/TranexamicAcid.xml"
							,"substances/Triacylglycerol.xml"
							,"substances/Tristearin.xml"
							,"substances/Urea.xml"
							,"substances/Vasopressin.xml"
							,"substances/WhiteBloodCell.xml"
							,"xsd/BioGearsDataModel.xsd"
							,"xsd/CommonDataModel.xsd"
							,"xsd/biogears/BioGears.xsd"
							,"xsd/biogears/BioGearsConfiguration.xsd"
							,"xsd/biogears/BioGearsEnvironment.xsd"
							,"xsd/biogears/BioGearsEquipment.xsd"
							,"xsd/biogears/BioGearsPhysiology.xsd"
							,"xsd/cdm/Anesthesia.xsd"
							,"xsd/cdm/AnesthesiaActions.xsd"
							,"xsd/cdm/Circuit.xsd"
							,"xsd/cdm/Compartment.xsd"
							,"xsd/cdm/ElectroCardioGram.xsd"
							,"xsd/cdm/EngineConfiguration.xsd"
							,"xsd/cdm/EngineState.xsd"
							,"xsd/cdm/Environment.xsd"
							,"xsd/cdm/EnvironmentActions.xsd"
							,"xsd/cdm/EnvironmentConditions.xsd"
							,"xsd/cdm/Inhaler.xsd"
							,"xsd/cdm/InhalerActions.xsd"
							,"xsd/cdm/Patient.xsd"
							,"xsd/cdm/PatientActions.xsd"
							,"xsd/cdm/PatientAssessments.xsd"
							,"xsd/cdm/PatientConditions.xsd"
							,"xsd/cdm/PatientNutrition.xsd"
							,"xsd/cdm/Physiology.xsd"
							,"xsd/cdm/Properties.xsd"
							,"xsd/cdm/Scenario.xsd"
							,"xsd/cdm/Substance.xsd"
							,"xsd/cdm/SubstanceQuantity.xsd"
							,"xsd/cdm/System.xsd"
							,"xsd/cdm/TestReport.xsd"};
  };
}
}