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
#include <biogears/config.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEExercise.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/string/manipulation.h>

#include <sstream>

USING_BIOGEARS_NAMESPACE;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying an Exercise insult to the patient
///
/// \details
/// Refer to the SEExercise class
//--------------------------------------------------------------------------------------------------
int HowToExercise()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToExercise.log");
  bg->GetLogger()->Info("HowToExercise");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
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
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalMetabolicRate", PowerUnit::kcal_Per_day);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CoreTemperature", TemperatureUnit::C);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("AchievedExerciseLevel");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("FatigueLevel");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalMetabolicRate", PowerUnit::W);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalWorkRateLevel");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SodiumLostToSweat", MassUnit::mg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PotassiumLostToSweat", MassUnit::mg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("ChlorideLostToSweat", MassUnit::mg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SweatRate", MassPerTimeUnit::mg_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SkinTemperature", TemperatureUnit::C);




  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToExercise.csv");

  // Advance some time to get some resting data
  bg->AdvanceModelTime(20, TimeUnit::s);

  bg->GetLogger()->Info("The patient is nice and healthy");
  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Total Metabolic Rate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::kcal_Per_day), "kcal_Per_day"));
  bg->GetLogger()->Info(asprintf("Core Temperature : %f %s", bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("RespirationRate : %f %s", bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min), "bpm"));
  //bg->GetLogger()->Info(asprintf("AchievedExerciseLevel : %f", bg->GetEnergySystem()->GetAchievedExerciseLevel())); // This will be NaN as the patient is not doing any exercise
  //bg->GetLogger()->Info(asprintf("FatigueLevel : %f", bg->GetEnergySystem()->GetFatigueLevel())); // No fatigue either
  bg->GetLogger()->Info(asprintf("TotalMetabolicRate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W), "W")); //This will be at Basal Level

  // Exercise Starts - instantiate an Exercise action and have the engine process it.
  // After initiating exercise the patient’s metabolic rate begins to increased.
  // An intensity of 1.0 asks the patient to exercise at the maximum work rate capable by the patient (an output of ~1200W for a patient in relativly good shape).
  // The patient will not be able to sustain this pace for long... and will get fatigued
  // This leads to an increase in core temperature, cardiac output, respiration rate and tidal volume.
  // The increase in core temperature leads to an elevated sweat rate, which causes the patient’s skin temperature to drop due to evaporation.
  SEExercise::SEGeneric ge;
  ge.Intensity.SetValue(0.5);
  SEExercise exG { ge };
  bg->ProcessAction(exG);
  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Total Metabolic Rate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::kcal_Per_day), "kcal_Per_day"));
  bg->GetLogger()->Info(asprintf("Core Temperature : %f %s", bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("RespirationRate : %f %s", bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("AchievedExerciseLevel : %f", bg->GetEnergySystem()->GetAchievedExerciseLevel())); // this is the fraction of what we asked for, 1.0 means we are doing what you asked for.
  bg->GetLogger()->Info(asprintf("FatigueLevel : %f", bg->GetEnergySystem()->GetFatigueLevel())); // Patient is very tired
  bg->GetLogger()->Info(asprintf("TotalMetabolicRate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W), "W")); // We are burning
  bg->GetLogger()->Info(asprintf("TotalWorkRateLevel : %f", bg->GetEnergySystem()->GetTotalWorkRateLevel()));
  ; // How much work we are getting / the max work rate possible

  // Once exercise has ended, the patient is in a recovery period where the metabolic rate begins to return to its basal value.
  // The cardiac output, respiration rate and tidal volume follow this recovery trend towards their normal values.
  ge.Intensity.SetValue(0.0);
  SEExercise exGStop { ge };
  bg->ProcessAction(exGStop);

  // Advance some time while the medic gets the drugs ready
  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Total Metabolic Rate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::kcal_Per_day), "kcal_Per_day"));
  bg->GetLogger()->Info(asprintf("Core Temperature : %f %s", bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("RespirationRate : %f %s", bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min), "bpm"));
  //bg->GetLogger()->Info(asprintf("AchievedExerciseLevel : %f", bg->GetEnergySystem()->GetAchievedExerciseLevel())); // This will be NaN as the patient is not doing any exercise
  //bg->GetLogger()->Info(asprintf("FatigueLevel : %f", bg->GetEnergySystem()->GetFatigueLevel())); // We are not working out but we are still fatigued
  bg->GetLogger()->Info(asprintf("TotalMetabolicRate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W), "W")); // We are still burning
  //////////////////////////////// Next Exercise
  // Cycling Inputs
  SEExercise::SECycling cyc;
  cyc.CadenceCycle.SetValue(60, FrequencyUnit::Per_min);
  cyc.PowerCycle.SetValue(100.0, PowerUnit::W);
  SEExercise exC { cyc };
  bg->ProcessAction(exC);
  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Total Metabolic Rate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::kcal_Per_day), "kcal_Per_day"));
  bg->GetLogger()->Info(asprintf("Core Temperature : %f %s", bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("RespirationRate : %f %s", bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("AchievedExerciseLevel : %f", bg->GetEnergySystem()->GetAchievedExerciseLevel())); // this is the fraction of what we asked for, 1.0 means we are doing what you asked for.
  bg->GetLogger()->Info(asprintf("FatigueLevel : %f", bg->GetEnergySystem()->GetFatigueLevel())); // Patient is very tired
  bg->GetLogger()->Info(asprintf("TotalMetabolicRate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W), "W")); // We are burning
  bg->GetLogger()->Info(asprintf("TotalWorkRateLevel : %f", bg->GetEnergySystem()->GetTotalWorkRateLevel()));
  ; // How much work we are getting / the max work rate possible

  // Once exercise has ended, the patient is in a recovery period where the metabolic rate begins to return to its basal value.
  // The cardiac output, respiration rate and tidal volume follow this recovery trend towards their normal values.
  cyc.CadenceCycle.SetValue(0.0, FrequencyUnit::Per_min);
  cyc.PowerCycle.SetValue(0.0, PowerUnit::W);
  SEExercise exCStop { cyc };
  bg->ProcessAction(exCStop);

  // Advance some time while the medic gets the drugs ready
  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Total Metabolic Rate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::kcal_Per_day), "kcal_Per_day"));
  bg->GetLogger()->Info(asprintf("Core Temperature : %f %s", bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("RespirationRate : %f %s", bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min), "bpm"));
  //bg->GetLogger()->Info(asprintf("AchievedExerciseLevel : %f", bg->GetEnergySystem()->GetAchievedExerciseLevel())); // This will be NaN as the patient is not doing any exercise
  //bg->GetLogger()->Info(asprintf("FatigueLevel : %f", bg->GetEnergySystem()->GetFatigueLevel())); // We are not working out but we are still fatigued
  bg->GetLogger()->Info(asprintf("TotalMetabolicRate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W), "W")); // We are still burning
  //////////////////////////////// Next Exercise
  // Running Inputs
  SEExercise::SERunning run;
  run.SpeedRun.SetValue(5, LengthPerTimeUnit::m_Per_s);
  run.InclineRun.SetValue(0.1);
  SEExercise exR { run };
  bg->ProcessAction(exR);
  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Total Metabolic Rate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::kcal_Per_day), "kcal_Per_day"));
  bg->GetLogger()->Info(asprintf("Core Temperature : %f %s", bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("RespirationRate : %f %s", bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("AchievedExerciseLevel : %f", bg->GetEnergySystem()->GetAchievedExerciseLevel())); // this is the fraction of what we asked for, 1.0 means we are doing what you asked for.
  bg->GetLogger()->Info(asprintf("FatigueLevel : %f", bg->GetEnergySystem()->GetFatigueLevel())); // Patient is very tired
  bg->GetLogger()->Info(asprintf("TotalMetabolicRate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W), "W")); // We are burning
  bg->GetLogger()->Info(asprintf("TotalWorkRateLevel : %f", bg->GetEnergySystem()->GetTotalWorkRateLevel()));
  ; // How much work we are getting / the max work rate possible

  // Once exercise has ended, the patient is in a recovery period where the metabolic rate begins to return to its basal value.
  // The cardiac output, respiration rate and tidal volume follow this recovery trend towards their normal values.
  run.SpeedRun.SetValue(0.0, LengthPerTimeUnit::m_Per_s);
  run.InclineRun.SetValue(0.0);
  SEExercise exRStop { run };
  bg->ProcessAction(exRStop);

  // Advance some time while the medic gets the drugs ready
  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Total Metabolic Rate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::kcal_Per_day), "kcal_Per_day"));
  bg->GetLogger()->Info(asprintf("Core Temperature : %f %s", bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("RespirationRate : %f %s", bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min), "bpm"));
  //bg->GetLogger()->Info(asprintf("AchievedExerciseLevel : %f", bg->GetEnergySystem()->GetAchievedExerciseLevel())); // This will be NaN as the patient is not doing any exercise
  //bg->GetLogger()->Info(asprintf("FatigueLevel : %f", bg->GetEnergySystem()->GetFatigueLevel())); // We are not working out but we are still fatigued
  bg->GetLogger()->Info(asprintf("TotalMetabolicRate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W), "W")); // We are still burning

  //////////////////////////////// Next Exercise
  // Strength Training
  SEExercise::SEStrengthTraining str;
  str.WeightStrength.SetValue(20.0, MassUnit::kg);
  str.RepsStrength.SetValue(10.0);
  SEExercise exS { str };
  bg->ProcessAction(exS);
  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Total Metabolic Rate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::kcal_Per_day), "kcal_Per_day"));
  bg->GetLogger()->Info(asprintf("Core Temperature : %f %s", bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("RespirationRate : %f %s", bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("AchievedExerciseLevel : %f", bg->GetEnergySystem()->GetAchievedExerciseLevel())); // this is the fraction of what we asked for, 1.0 means we are doing what you asked for.
  bg->GetLogger()->Info(asprintf("FatigueLevel : %f", bg->GetEnergySystem()->GetFatigueLevel())); // Patient is very tired
  bg->GetLogger()->Info(asprintf("TotalMetabolicRate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W), "W")); // We are burning
  bg->GetLogger()->Info(asprintf("TotalWorkRateLevel : %f", bg->GetEnergySystem()->GetTotalWorkRateLevel()));
  ; // How much work we are getting / the max work rate possible

  // Once exercise has ended, the patient is in a recovery period where the metabolic rate begins to return to its basal value.
  // The cardiac output, respiration rate and tidal volume follow this recovery trend towards their normal values.
  str.WeightStrength.SetValue(0.0, MassUnit::kg);
  str.RepsStrength.SetValue(0.0);
  SEExercise exSStop { str };
  bg->ProcessAction(exSStop);

  // Advance some time while the medic gets the drugs ready
  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(asprintf("Cardiac Output : %f %s", bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min), "mL_Per_min"));
  bg->GetLogger()->Info(asprintf("Mean Arterial Pressure : %f %s", bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Systolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Diastolic Pressure : %f %s", bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg), "mmHg"));
  bg->GetLogger()->Info(asprintf("Heart Rate : %f %s", bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min), "bpm"));
  bg->GetLogger()->Info(asprintf("Total Metabolic Rate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::kcal_Per_day), "kcal_Per_day"));
  bg->GetLogger()->Info(asprintf("Core Temperature : %f %s", bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C), "C"));
  bg->GetLogger()->Info(asprintf("RespirationRate : %f %s", bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min), "bpm"));
  //bg->GetLogger()->Info(asprintf("AchievedExerciseLevel : %f", bg->GetEnergySystem()->GetAchievedExerciseLevel())); // This will be NaN as the patient is not doing any exercise
  //bg->GetLogger()->Info(asprintf("FatigueLevel : %f", bg->GetEnergySystem()->GetFatigueLevel())); // We are not working out but we are still fatigued
  bg->GetLogger()->Info(asprintf("TotalMetabolicRate : %f %s", bg->GetEnergySystem()->GetTotalMetabolicRate(PowerUnit::W), "W")); // We are still burning

  bg->GetLogger()->Info("Finished");
  return 0;
}

int main ( int argc, char* argv[] ) {
  return HowToExercise();
}
