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


// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#include <biogears/string/manipulation.h>

using namespace biogears;

int using_scenario_exec();
int using_direct_control();
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying an Asthma Attack insult to the patient
///
/// \details
/// Refer to the SEAsthmaAttack class
//--------------------------------------------------------------------------------------------------
int  HowToCWD_Change()
{
  int rc = using_scenario_exec();
  if ( rc == 0 ) {
    return using_direct_control();
  } 
  return rc;
}

int using_direct_control()
{
  constexpr const char* logfile_path = "C:/biogears/direct_control/HowTo_CWD_Change.log";
  constexpr const char* working_dir_path = "C:/biogears/runtime/";

  //NOTE: Absolute Path to logger independent of any working path later passed to BioGearsEngine
  Logger absolute_logger { logfile_path };
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine(working_dir_path, &absolute_logger);
  bg->GetLogger()->Info("HowToAsthmaAttack");
  if (!bg->LoadState("states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return 1;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HemoglobinContent", MassUnit::g);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("InspiratoryExpiratoryRatio");
  bg->GetEngineTrack()->GetDataRequestManager().CreateGasCompartmentDataRequest().Set(BGE::PulmonaryCompartment::Trachea, "InFlow");

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowTo-CWD_Change.csv");

  // Advance some time to get some healthy data
  bg->AdvanceModelTime(50, TimeUnit::s);
  // Cache off compartments of interest!
  const SEGasCompartment* carina = bg->GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::Trachea);

  bg->GetLogger()->Info("The patient is nice and healthy");
  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Hemoglobin Content : %f %s", bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g), "g"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("InspiratoryExpiratoryRatio : %f", bg->GetRespiratorySystem()->GetInspiratoryExpiratoryRatio()));
  bg->GetLogger()->Info(asprintf("Carina InFlow : %f %s", carina->GetInFlow(VolumePerTimeUnit::L_Per_s), "L_Per_s"));
  ;

  // Asthma Attack Starts - instantiate an asthma attack action and have the engine process it
  // Asthma is a common inflammatory disease of the airways where air flow into the lungs is partially obstructed.
  // Acute asthma is an exacerbation of asthma that does not respond to standard treatments.
  // BioGears models asthma by increasing the airway flow resistance in the circuit model.
  // The higher the severity, the more severe the asthma attack.
  SEAsthmaAttack asthmaAttack;
  asthmaAttack.GetSeverity().SetValue(0.3);
  bg->ProcessAction(asthmaAttack);

  bg->AdvanceModelTime(550, TimeUnit::s);

  bg->GetLogger()->Info("The patient has been having an asthma attack for 550s");
  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Hemoglobin Content : %f %s", bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g), "g"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("InspiratoryExpiratoryRatio : %f", bg->GetRespiratorySystem()->GetInspiratoryExpiratoryRatio()));
  bg->GetLogger()->Info(asprintf("Carina InFlow : %f %s", carina->GetInFlow(VolumePerTimeUnit::L_Per_s), "L_Per_s"));

  // Asthma Attack Stops
  asthmaAttack.GetSeverity().SetValue(0.0);
  bg->ProcessAction(asthmaAttack);

  // Advance some time while the patient catches their breath
  bg->AdvanceModelTime(200, TimeUnit::s);

  bg->GetLogger()->Info("The patient has NOT had an asthma attack for 200s");
  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Hemoglobin Content : %f %s", bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g), "g"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("InspiratoryExpiratoryRatio : %f", bg->GetRespiratorySystem()->GetInspiratoryExpiratoryRatio()));
  bg->GetLogger()->Info(asprintf("Carina InFlow : %f %s", carina->GetInFlow(VolumePerTimeUnit::L_Per_s), "L_Per_s"));
  bg->GetLogger()->Info("Finished");
  return 0;
}

int using_scenario_exec()
{

  constexpr const char* working_dir_path = "C:/biogears/runtime/";

  constexpr const char* log_file = "";
  constexpr const char* scenario_file = "Scenarios/Showcase/AsthmaAttack.xml";
  constexpr const char* results_file = "";

  //NOTE: If your passing a log_file with a working_dir the logfile should be relative to the working dir

  std::unique_ptr<PhysiologyEngine> bioGears = CreateBioGearsEngine(working_dir_path, log_file);
  if (!bioGears) {
    std::cerr << "Unable to create BioGearsEngine" << std::endl;
    return 1;
  }
  try {
    BioGearsScenarioExec exec(*bioGears);
    exec.Execute(scenario_file, results_file, nullptr);
  } catch (std::exception ex) {
    std::cerr << ex.what() << std::endl;
    return 2;
  } catch (...) {
    std::cerr << "Unable to run scenario " << working_dir_path << scenario_file << std::endl;
    return 3;
  }
  return 0;
}

int main ( int argc, char* argv[] ) {
  return HowToCWD_Change();
}
