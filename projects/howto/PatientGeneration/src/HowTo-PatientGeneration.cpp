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

#include "HowToTracker.h"

// Include the various types you will be using in your code
#include "biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h"
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/string/manipulation.h>
using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Run a Sepsis scenario with variable paramaters
///
/// \param std::string name - Log file name
/// \param double mic_g_per_l - (gram/litter) Minimal Inhibitory Concentration of the Sepsis Infection (Affects Antibiotic response)
/// \param double apply_at_m - (minutes) how far after the initial infection we want o apply antibiotics
/// \param double application_interval_m - (minutes) How often after the first antibiotics application we reapply antibiotics
/// \param double duration_hr - (hours) Total simulation lenght in hours
/// \details
///   Timeline -> Stablization ->  Apply Infection Moderate with MIC ->  AdvanceTime by apply_at -> apply antibiotics
///   Until duration has passed apply antibiotics every application interval
///
//--------------------------------------------------------------------------------------------------
bool HowToPatientGeneration(std::string name, double mic_g_per_l, double apply_at_m, double application_interval_m, double duration_hr)
{
  std::string long_name = name + asprintf("-%fg_Per_l-%fm-%fm-%fhr", mic_g_per_l, apply_at_m, application_interval_m, duration_hr);

  // Create the engine and load the patient
  Logger logger(name + ".log");
  std::unique_ptr<
    PhysiologyEngine>
    bg = CreateBioGearsEngine(&logger);
  bg->GetLogger()->Info("Starting " + long_name);
  if (!bg->LoadState("states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return false;
  }

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  //Example bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::L_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HemoglobinContent", MassUnit::g);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CentralVenousPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("Hematocrit");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("ArterialBloodPH");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrinationRate", VolumePerTimeUnit::mL_Per_hr);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("WhiteBloodCellCount", AmountPerVolumeUnit::ct_Per_uL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrineProductionRate", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("OxygenSaturation");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CarbonDioxideSaturation");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CoreTemperature", TemperatureUnit::C);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SkinTemperature", TemperatureUnit::C);

  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*bg->GetSubstanceManager().GetSubstance("Bicarbonate"), "BloodConcentration", MassPerVolumeUnit::mg_Per_dL);
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*bg->GetSubstanceManager().GetSubstance("Creatinine"), "BloodConcentration", MassPerVolumeUnit::mg_Per_dL);
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*bg->GetSubstanceManager().GetSubstance("Lactate"), "BloodConcentration", MassPerVolumeUnit::mg_Per_dL);
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*bg->GetSubstanceManager().GetSubstance("Piperacillin"), "BloodConcentration", MassPerVolumeUnit::mg_Per_dL);
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*bg->GetSubstanceManager().GetSubstance("Tazobactam"), "BloodConcentration", MassPerVolumeUnit::mg_Per_dL);

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename(long_name + ".csv");
  bg->GetEngineTrack()->GetDataRequestManager().SetSamplesPerSecond(1 / (5 * 60));

  HowToTracker tracker(*bg);
  SEInfection infection {};
  infection.SetSeverity(CDM::enumInfectionSeverity::Moderate);
  SEScalarMassPerVolume infection_mic;
  infection_mic.SetValue(mic_g_per_l, MassPerVolumeUnit::g_Per_L);
  infection.GetMinimumInhibitoryConcentration().Set(infection_mic);
  infection.SetLocation("Gut");
  bg->ProcessAction(infection);
  auto& substances = bg->GetSubstanceManager();

  SESubstanceCompound* PiperacillinTazobactam = bg->GetSubstanceManager().GetCompound("PiperacillinTazobactam");
  SESubstanceCompoundInfusion full_antibiotics_bag { *PiperacillinTazobactam };
  SESubstanceCompoundInfusion empty_antibiotics_bag { *PiperacillinTazobactam };
  full_antibiotics_bag.GetBagVolume().SetValue(20.0, VolumeUnit::mL);
  empty_antibiotics_bag.GetBagVolume().SetValue(20.0, VolumeUnit::mL);
  full_antibiotics_bag.GetRate().SetValue(0.667, VolumePerTimeUnit::mL_Per_min);
  empty_antibiotics_bag.GetRate().SetValue(0, VolumePerTimeUnit::mL_Per_min);
  //Load substances we might use

  tracker.AdvanceModelTime(apply_at_m * 60);
  bg->ProcessAction(full_antibiotics_bag);
  tracker.AdvanceModelTime(full_antibiotics_bag.GetBagVolume().GetValue(VolumeUnit::mL) * full_antibiotics_bag.GetRate().GetValue(VolumePerTimeUnit::mL_Per_min) * 60);
  bg->ProcessAction(empty_antibiotics_bag);
  auto time_remaining_min = duration_hr * 60 - bg->GetSimulationTime(TimeUnit::min);
  while (0 < time_remaining_min) {
    if (0 < time_remaining_min - application_interval_m) {
      tracker.AdvanceModelTime(application_interval_m * 60);
      bg->ProcessAction(full_antibiotics_bag);
      tracker.AdvanceModelTime(full_antibiotics_bag.GetBagVolume().GetValue(VolumeUnit::mL) * full_antibiotics_bag.GetRate().GetValue(VolumePerTimeUnit::mL_Per_min) * 60);
      bg->ProcessAction(empty_antibiotics_bag);
    } else {
      tracker.AdvanceModelTime(time_remaining_min * 60);
    }
    time_remaining_min = duration_hr * 60 - bg->GetSimulationTime(TimeUnit::min);
  }

  // Asthma Attack Stops

  // Advance some time while the patient catches their breath
  tracker.AdvanceModelTime(200);

  return true;
}
