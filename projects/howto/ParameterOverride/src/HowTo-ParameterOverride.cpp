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
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEndocrineSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SERenalSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/system/physiology/SETissueSystem.h>
#include <biogears/cdm/patient/actions/SEOverride.h>

#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/scenario/SEPatientActionCollection.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>



using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for the Parameter Override Insult
///
//--------------------------------------------------------------------------------------------------
void HowToParameterOverride()
{
  std::stringstream ss;
  // Create a BioGears Engine and load the standard patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToParameterOverride.log");

  bg->GetLogger()->Info("HowToParameterOverride");
  if (!bg->LoadState("./states/StandardMale@0s.xml"))
  {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects 
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TidalVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalLungVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("OxygenSaturation");

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToParameterOverride.csv");

  bg->GetLogger()->Info("The patient is nice and healthy");
  bg->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());

  bg->AdvanceModelTime(30, TimeUnit::s);

  // Create an SEParameterOverride object
  // Set a parameter change (based on individual parameter units.)	
  // Does ordering matter? Unlike in a scenario action call, the ordering does not matter for SDK calls
  SEOverride override;
  override.SetOverrideState(CDM::enumOnOff::On);
  override.SetOverrideConformance(CDM::enumOnOff::On);
  override.GetHeartRateOverride().SetValue(70, FrequencyUnit::Per_min);
  bg->ProcessAction(override);
  bg->GetLogger()->Info("Overriding BioGears Parameter.");

  // Advance time to see how the obstruction affects the patient
  bg->AdvanceModelTime(90, TimeUnit::s);

  bg->GetLogger()->Info(std::stringstream() << "The override has been in place for 90s...");
  bg->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());

  // Turn off and add another, this time with conformance off. NOTE: unlike in a patient action, here the heart rate override must be cleared to not be listed in the logger
  override.SetOverrideState(CDM::enumOnOff::Off);
  override.SetOverrideConformance(CDM::enumOnOff::On);
  override.GetHeartRateOverride().Clear();
  bg->ProcessAction(override);
  bg->GetLogger()->Info("Override Turning Off.");

  // Advance time
  bg->AdvanceModelTime(30, TimeUnit::s);

  bg->GetLogger()->Info(std::stringstream() << "The override has been off for 30s...");
  bg->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());

  override.SetOverrideState(CDM::enumOnOff::On);
  override.SetOverrideConformance(CDM::enumOnOff::Off);
  override.GetRespirationRateOverride().SetValue(20, FrequencyUnit::Per_min);
  override.GetHeartRateOverride().SetValue(90, FrequencyUnit::Per_min);
  bg->ProcessAction(override);
  bg->GetLogger()->Info("Overriding BioGears Parameter.");

  // Advance time
  bg->AdvanceModelTime(90, TimeUnit::s);

  bg->GetLogger()->Info(std::stringstream() << "The override has been in place for 90s...");
  bg->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());

  // Adding an override parameter while keeping the last override in place. Do NOT turn off the override, just add to it with an on call
  override.SetOverrideState(CDM::enumOnOff::On);
  override.SetOverrideConformance(CDM::enumOnOff::Off);
  override.GetTidalVolumeOverride().SetValue(650, VolumeUnit::mL);
  bg->ProcessAction(override);
  bg->GetLogger()->Info("Overriding BioGears Parameter.");

  // Advance time
  bg->AdvanceModelTime(90, TimeUnit::s);

  bg->GetLogger()->Info(std::stringstream() << "The override has been in place for 90s...");
  bg->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());

}