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
#include <algorithm>
#include <cctype>
#include <random>
#include <string>

// Include the various types you will be using in your code
#include "HowTo-PatientGeneration.h"
#include "biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h"
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
#include <biogears/cdm/patient/assessments/SEUrinalysis.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERenalSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/string/manipulation.h>

using namespace biogears;

void PatientRun::antibiotics_regiment()
{
  constexpr double deltaT = 1;

  auto current_time = _bg->GetSimulationTime(TimeUnit::min);
  if (!_first_treatment_occured) {
    if (_apply_at_m < current_time) {
      _PiperacillinTazobactam_bag->GetBagVolume().SetValue(100.0, VolumeUnit::mL);
      _bg->ProcessAction(*_PiperacillinTazobactam_bag);
      _first_treatment_occured = true;
      _applying_antibiotics = true;
    }
  }

  if (_applying_antibiotics) {
    if (_PiperacillinTazobactam_bag->GetBagVolume().GetValue(VolumeUnit::mL) == 0) {
      _applying_antibiotics = false;
    }
    _time_applying_antibiotics_min += deltaT;
    _time_since_antibiotic_treatment_min = 0;
  } else {
    if (_application_interval_m < _time_since_antibiotic_treatment_min) {

      _PiperacillinTazobactam_bag->GetBagVolume().SetValue(100.0, VolumeUnit::mL);
      _bg->ProcessAction(*_PiperacillinTazobactam_bag);
      _applying_antibiotics = true;
    }
    _time_applying_antibiotics_min = 0;
    _time_since_antibiotic_treatment_min += deltaT;
  }
}
//--------------------------------------------------------------------------------
void PatientRun::refresh_treatment()
{
  if (!_patient_is_septic && _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) < 100) {

    //Step 1: Initial Treatment 2 500ml bolus of Saline over 1 hour
    _patient_is_septic = true;
    _refresh_state = RefreshState::INITIAL_BOLUS;
    _PiperacillinTazobactam_bag->GetRate().SetValue(0.75, VolumePerTimeUnit::mL_Per_min);
    _PiperacillinTazobactam_bag->GetBagVolume().SetValue(100.0, VolumeUnit::mL);


    //Matt et.al Frontiers Physiology 2019-10
    //"Just prior to septic shock onset, we administered two separate 500 mL boluses over the course of an hour."
    _Saline_bag->GetBagVolume().SetValue(1000, VolumeUnit::mL);
    _Saline_bag->GetRate().SetValue(500, VolumePerTimeUnit::mL_Per_hr);
    _bg->ProcessAction(*_Saline_bag);

  } else {
    switch (_refresh_state) {
    case RefreshState::INITIAL_BOLUS:
      if (_Saline_bag->GetBagVolume().GetValue(VolumeUnit::mL) < 1.0) {
        _refresh_state = RefreshState::FULL_ASSESSMENT;
      }
      break;
    case RefreshState::MAINTENANCE_FLUIDS: {
      //On might concider maintenance fluids an always on condition
      //In that case move this entire case outide the switch
      if (_maintenance_bag->GetBagVolume().GetValue(VolumeUnit::mL) < 1.0) {
        double patient_wgt_kg = _bg->GetPatient().GetWeight(MassUnit::kg);
        //maintenance fluids 1 mL/kg/h
        _maintenance_bag->GetBagVolume().SetValue(patient_wgt_kg * 1. /* (ml/kg * hr)*/ * 6. /*(hours)*/, VolumeUnit::mL);
        _maintenance_bag->GetRate().SetValue(patient_wgt_kg * 1. /* (ml/kg * hr) */, VolumePerTimeUnit::mL_Per_hr);
        _bg->ProcessAction(*_maintenance_bag);
      }
    } break;
    case RefreshState::NOREPINEPHRINE_TITRATE: {
      if (!_Norepinphrine) {
        SESubstance* norepinphrine = _bg->GetSubstanceManager().GetSubstance("Norepinphrine");
        _Norepinphrine = std::make_unique<SESubstanceInfusion>(*norepinphrine).release();
      }
      if (!_norepinphrine_titrate_active) {
        _Norepinphrine->GetRate().SetValue(1.0, VolumePerTimeUnit::mL_Per_min);
        _Norepinphrine->GetConcentration().SetValue(1.0 /* (ug/kg*min) */ * _bg->GetPatient().GetWeight(MassUnit::kg), MassPerVolumeUnit::ug_Per_mL);
        _bg->ProcessAction(*_Norepinphrine);
        _norepinphrine_titrate_active = true;
      } else {
        if (65. <= _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) && _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) <= 70.) {
          //Condition 1: Finding correct titrate rate.
          double rate = _Norepinphrine->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
          _Norepinphrine->GetConcentration().SetValue(rate * 0.9, MassPerVolumeUnit::ug_Per_mL);
          _bg->ProcessAction(*_Norepinphrine);
        } else if (_bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) <= 65.) {
          double rate = _Norepinphrine->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
          _Norepinphrine->GetConcentration().SetValue(rate * 1.1, MassPerVolumeUnit::ug_Per_mL);
          _bg->ProcessAction(*_Norepinphrine);
        } else /* 70. <= _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) .*/ {
          //MAYBE THIS IS AN EXIT CONDITION ON TITRATE?
          double rate = _Norepinphrine->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
          _Norepinphrine->GetConcentration().SetValue(rate * 0.75, MassPerVolumeUnit::ug_Per_mL); // FASTER PULL BACK
          _bg->ProcessAction(*_Norepinphrine);
        }
      }
      if (65 < _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg)
          && _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) < 70) {
        //Check for 10 minutes of 65 < MAP < 70
        _persistant_stable_map_min += 1;
      } else {
        _persistant_stable_map_min -= 0;
      }
      if (10 < _persistant_stable_map_min) {
        _refresh_state = RefreshState::MONITORING;
      }
    } break;
    case RefreshState::FULL_ASSESSMENT: {
      _time_to_reassessment_min = hours(1);
      _time_to_full_assessment_min = hours(3);
      biogears::SEUrinalysis ua(_bg->GetLogger());
      _bg->GetPatientAssessment(ua);
      if (ua.GetBloodResult() == CDM::enumPresenceIndicator::Positive || _bg->GetRenalSystem()->GetMeanUrineOutput(VolumePerTimeUnit::L_Per_day) < 1) {

        _Saline_bag->GetBagVolume().SetValue(500, VolumeUnit::mL);
        _Saline_bag->GetRate().SetValue(1000, VolumePerTimeUnit::mL_Per_hr);
        _bg->ProcessAction(*_Saline_bag);
        _refresh_state = RefreshState::NOREPINEPHRINE_TITRATE;
      } else {
        _refresh_state = RefreshState::MAINTENANCE_FLUIDS;
      }
    } break;
    default:
      break;
    }
    if (_PiperacillinTazobactam_bag->GetBagVolume().GetValue(VolumeUnit::mL) < 1) {
      _PiperacillinTazobactam_bag->GetBagVolume().SetValue(100, VolumeUnit::mL);
      _bg->ProcessAction(*_PiperacillinTazobactam_bag);
    }

    if (_time_to_reassessment_min < 1.0) {
      _refresh_state = RefreshState::MONITORING;
    }
    if (_time_to_full_assessment_min < 1.0) {
      _refresh_state = RefreshState::FULL_ASSESSMENT;
    }
    _time_to_reassessment_min -= 1.;
    _time_to_full_assessment_min -= 1.;
  }
}
//-------------------------------------------------------------------------------
void PatientRun::egdt_treatment()
{
  if (!_patient_is_septic && _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) < 100) {

    //Step 1: Initial Treatment 2 500ml bolus of Saline over 1 hour
    _patient_is_septic = true;
    _egdt_state = EGDTState::INITIAL_BOLUS;
    _Saline_bag->GetRate().SetValue(2500.0, VolumePerTimeUnit::mL_Per_hr);
    _Saline_bag->GetBagVolume().SetValue(2500, VolumeUnit::mL);

    _bg->ProcessAction(*_Saline_bag);
    _time_to_full_assessment_min = hours(3);
  } else {
    switch (_egdt_state) {
    case EGDTState::INITIAL_BOLUS:
      if ( _Saline_bag->GetBagVolume().GetValue(VolumeUnit::mL) < 1.) {
        _egdt_state = EGDTState::FULL_ASSESSMENT;
      }      
      break;
    case EGDTState::MAINTENANCE_FLUIDS: {

      //On might concider maintenance fluids an always on condition
      //In that case move this entire case outide the switch
      if (_maintenance_bag->GetBagVolume().GetValue(VolumeUnit::mL) < 1) {
        double patient_wgt_kg = _bg->GetPatient().GetWeight(MassUnit::kg);
        //maintenance fluids 1 mL/kg/h
        _maintenance_bag->GetBagVolume().SetValue(patient_wgt_kg * 1. /* (ml/kg * hr)*/ * 6. /*(hours)*/, VolumeUnit::mL);
        _maintenance_bag->GetRate().SetValue(patient_wgt_kg * 1. /* (ml/kg * hr) */, VolumePerTimeUnit::mL_Per_hr);
        _bg->ProcessAction(*_maintenance_bag);
      }
    } break;
    case EGDTState::RAPID_FLUID_BOLAS: {
      if (_Saline_bag->GetBagVolume().GetValue(VolumeUnit::mL) < 1.) {
        _Saline_bag->GetBagVolume().SetValue(500, VolumeUnit::mL);
        _Saline_bag->GetRate().SetValue(1000, VolumePerTimeUnit::mL_Per_hr);
        _bg->ProcessAction(*_Saline_bag);
      }
      if ((_bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) < 91.
           || _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) < 66.)) {
        //Check for consistant low MAP. If it is consistant for 30 minutes move to Norepinephrine infusion
        //I check for this by asking if MAP < 66 for 30 iterations with out seeing MAP > 66 for 5 consecutive iterations
        _persistent_low_map_min += 1;
        _high_map_count = (_high_map_count) ? _high_map_count - 1 : 0;
      } else {
        _high_map_count += 0;
        if (5 < _high_map_count) {
          _persistent_low_map_min = 0;
        }
      }
      if (30 < _persistent_low_map_min) {
        _egdt_state = EGDTState::NOREPHINEPRIN_INFUSION;
      }

      biogears::SEUrinalysis ua(_bg->GetLogger());
      if (ua.GetBloodResult() == CDM::enumPresenceIndicator::Positive || _bg->GetRenalSystem()->GetMeanUrineOutput(VolumePerTimeUnit::L_Per_day) < 1) {
        _egdt_state = EGDTState::RAPID_FLUID_BOLAS;
      } else {
        _egdt_state = EGDTState::MONITORING;
      }

    } break;
    case EGDTState::MONITORING:
      _time_to_reassessment_min = hours(1);
      if (_bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) < 90. || _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) < 65.) {
        _egdt_state = EGDTState::RAPID_FLUID_BOLAS;
      } else {
        _egdt_state = EGDTState::MAINTENANCE_FLUIDS;
      }
      break;
    case EGDTState::NOREPHINEPRIN_INFUSION: {
      if (!_Norepinphrine) {
        SESubstance* norepinphrine = _bg->GetSubstanceManager().GetSubstance("Norepinphrine");
        _Norepinphrine = std::make_unique<SESubstanceInfusion>(*norepinphrine).release();
      }
      if (!_norepinphrine_titrate_active) {
        _Norepinphrine->GetRate().SetValue(1.0, VolumePerTimeUnit::mL_Per_min);
        _Norepinphrine->GetConcentration().SetValue(1.0 /* (ug/kg*min) */ * _bg->GetPatient().GetWeight(MassUnit::kg), MassPerVolumeUnit::ug_Per_mL);
        _bg->ProcessAction(*_Norepinphrine);
        _norepinphrine_titrate_active = true;
      } else {
        if (65. <= _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) && _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) <= 70.) {
          //Condition 1: Finding correct titrate rate.
          double rate = _Norepinphrine->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
          _Norepinphrine->GetConcentration().SetValue(rate * 0.9, MassPerVolumeUnit::ug_Per_mL);
          _bg->ProcessAction(*_Norepinphrine);
        } else if (_bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) <= 65.) {
          double rate = _Norepinphrine->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
          _Norepinphrine->GetConcentration().SetValue(rate * 1.1, MassPerVolumeUnit::ug_Per_mL);
          _bg->ProcessAction(*_Norepinphrine);
        } else /* 70. <= _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) .*/ {
          //MAYBE THIS IS AN EXIT CONDITION ON TITRATE?
          double rate = _Norepinphrine->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
          _Norepinphrine->GetConcentration().SetValue(rate * 0.75, MassPerVolumeUnit::ug_Per_mL); // FASTER PULL BACK
          _bg->ProcessAction(*_Norepinphrine);
        }
      }
      if (65 < _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg)
          && _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) < 70) {
        //Check for 10 minutes of 65 < MAP < 70
        _persistant_stable_map_min += 1;
      } else {
        _persistant_stable_map_min -= 0;
      }
      if (10 < _persistant_stable_map_min) {
        _egdt_state = EGDTState::MONITORING;
      }
    } break;
    case EGDTState::FULL_ASSESSMENT: {
      _time_to_reassessment_min = hours(1);
      _time_to_full_assessment_min = hours(3);
      biogears::SEUrinalysis ua(_bg->GetLogger());
      _bg->GetPatientAssessment(ua);
      if (ua.GetBloodResult() == CDM::enumPresenceIndicator::Positive || _bg->GetRenalSystem()->GetMeanUrineOutput(VolumePerTimeUnit::L_Per_day) < 1) {

        _Saline_bag->GetBagVolume().SetValue(500, VolumeUnit::mL);
        _Saline_bag->GetRate().SetValue(1000, VolumePerTimeUnit::mL_Per_hr);
        _bg->ProcessAction(*_Saline_bag);
        _egdt_state = EGDTState::RAPID_FLUID_BOLAS;
      } else {
        _egdt_state = EGDTState::MAINTENANCE_FLUIDS;
      }
    } break;
    default:
      break;
    }

    if (_PiperacillinTazobactam_bag->GetBagVolume().GetValue(VolumeUnit::mL) < 1) {
      _PiperacillinTazobactam_bag->GetBagVolume().SetValue(100, VolumeUnit::mL);
      _bg->ProcessAction(*_PiperacillinTazobactam_bag);
    }

    if (_time_to_reassessment_min < 1.0) {
      _egdt_state = EGDTState::MONITORING;
    }
    if (_time_to_full_assessment_min < 1.0) {
      _egdt_state = EGDTState::FULL_ASSESSMENT;
    }
    
    _time_to_reassessment_min -= 1.;
    _time_to_full_assessment_min -= 1.;
  }
}
//-------------------------------------------------------------------------------
void PatientRun::nutrition_regiment()
{
  if (_time_since_feeding_min > hours(8)) {
    _time_since_feeding_min -= hours(8);
    SEConsumeNutrients meal;
    meal.GetNutrition().GetCarbohydrate().SetValue(50, MassUnit::g);
    meal.GetNutrition().GetFat().SetValue(10, MassUnit::g);
    meal.GetNutrition().GetProtein().SetValue(20, MassUnit::g);
    meal.GetNutrition().GetCalcium().SetValue(100, MassUnit::mg);
    meal.GetNutrition().GetWater().SetValue(480, VolumeUnit::mL);
    meal.GetNutrition().GetSodium().SetValue(1, MassUnit::g);
    //meal.GetNutrition().GetCarbohydrateDigestionRate().SetValue(50, VolumeUnit::mL);
    //meal.GetNutrition().GetFatDigestionRate().MassUnit::g(50, VolumeUnit::mL);
    //meal.GetNutrition().GetProteinDigestionRate().SetValue(50, VolumeUnit::mL);
    _bg->ProcessAction(meal);
  }
  _time_since_feeding_min += 1;
}
//-------------------------------------------------------------------------------
/// \brief
/// Run a Sepsis scenario with variable paramaters
///
/// \param std::string name - Log file name
/// \param double mic_g_per_l - (gram/litter) Minimal Inhibitory Concentration of the Sepsis Infection (Affects Antibiotic response)
/// \param int severity - 0 = low 1 = moderate 2 = severe ; Initial Severity of the Bacterial Infection
/// \param double _apply_at_m - (minutes) how far after the initial infection we want o apply antibiotics
/// \param double _application_interval_m - (minutes) How often after the first antibiotics application we reapply antibiotics
/// \param std::string patient-state- Path to a patient state relative to the working directory
/// \param double duration_hr - (hours) Total simulation lenght in hours
/// \details
///   Timeline -> Stablization ->  Apply Infection Moderate with MIC ->  AdvanceTime by apply_at -> apply antibiotics
///   Until duration has passed apply antibiotics every application interval
///
//-------------------------------------------------------------------------------
void PatientRun::run()
{

  auto long_name = asprintf("%s_%s_%s_Inhibition_%0.0fg_Per_l_%0.0fm-%0.0fm-%0.0fhr", _patient_name.c_str(), _infection_severity_str,
                            _treatment_plan_str, _mic_g_per_l, _apply_at_m, _application_interval_m, _duration_hr);

  // Create the engine and load the patient
  Logger logger(_patient_name + ".log");
  std::unique_ptr<
    PhysiologyEngine>
    _bg = CreateBioGearsEngine(&logger);
  _bg->GetLogger()->Info("Starting " + long_name);
  if (!_bg->LoadState("states/StandardMale@0s.xml")) {
    _bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  //Example _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::L_Per_min);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HemoglobinContent", MassUnit::g);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CentralVenousPressure", PressureUnit::mmHg);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("Hematocrit");
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("ArterialBloodPH");
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrinationRate", VolumePerTimeUnit::mL_Per_hr);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("WhiteBloodCellCount", AmountPerVolumeUnit::ct_Per_uL);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrineProductionRate", VolumePerTimeUnit::mL_Per_min);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", FrequencyUnit::Per_min);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("OxygenSaturation");
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CarbonDioxideSaturation");
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CoreTemperature", TemperatureUnit::C);
  _bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SkinTemperature", TemperatureUnit::C);

  _bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*_bg->GetSubstanceManager().GetSubstance("Bicarbonate"), "BloodConcentration", MassPerVolumeUnit::mg_Per_dL);
  _bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*_bg->GetSubstanceManager().GetSubstance("Creatinine"), "BloodConcentration", MassPerVolumeUnit::mg_Per_dL);
  _bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*_bg->GetSubstanceManager().GetSubstance("Lactate"), "BloodConcentration", MassPerVolumeUnit::mg_Per_dL);
  _bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*_bg->GetSubstanceManager().GetSubstance("Piperacillin"), "BloodConcentration", MassPerVolumeUnit::mg_Per_dL);
  _bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*_bg->GetSubstanceManager().GetSubstance("Tazobactam"), "BloodConcentration", MassPerVolumeUnit::mg_Per_dL);

  _bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename(long_name + ".csv");
  _bg->GetEngineTrack()->GetDataRequestManager().SetSamplesPerSecond(1. / (5. * 60.));

  HowToTracker tracker(*_bg);
  SEInfection infection {};
  infection.SetSeverity(_infection_severity);
  SEScalarMassPerVolume infection_mic;
  infection_mic.SetValue(_mic_g_per_l, MassPerVolumeUnit::g_Per_L);
  infection.GetMinimumInhibitoryConcentration().Set(infection_mic);
  infection.SetLocation("Gut");
  _bg->ProcessAction(infection);
  auto& substances = _bg->GetSubstanceManager();

  SESubstanceCompound* PiperacillinTazobactam = _bg->GetSubstanceManager().GetCompound("PiperacillinTazobactam");
  _PiperacillinTazobactam_bag = std::make_unique<SESubstanceCompoundInfusion>(*PiperacillinTazobactam).release();
  _PiperacillinTazobactam_bag->GetRate().SetValue(0.75, VolumePerTimeUnit::mL_Per_min);
  _PiperacillinTazobactam_bag->GetBagVolume().SetValue(0, VolumeUnit::mL);

  SESubstanceCompound* Saline = _bg->GetSubstanceManager().GetCompound("Saline");
  _Saline_bag = std::make_unique<SESubstanceCompoundInfusion>(*Saline).release();
  _Saline_bag->GetRate().SetValue(10, VolumePerTimeUnit::mL_Per_min);
  _Saline_bag->GetBagVolume().SetValue(0, VolumeUnit::mL);

  SESubstanceCompound* SalinelowDrip = _bg->GetSubstanceManager().GetCompound("SalineSlowDrip");
  _maintenance_bag = std::make_unique<SESubstanceCompoundInfusion>(*SalinelowDrip).release();
  _maintenance_bag->GetRate().SetValue(10, VolumePerTimeUnit::mL_Per_min);
  _maintenance_bag->GetBagVolume().SetValue(0, VolumeUnit::mL);

  //Load substances we might use

  while (0. < _time_remaining_min) {
    switch (_treatment_plan) {
    default:
    case TreatmentPlan::NONE:
      break;
    case TreatmentPlan::STANDARD:
      antibiotics_regiment();
      break;
    case TreatmentPlan::REFRESH:
      antibiotics_regiment();
      refresh_treatment();
      break;
    case TreatmentPlan::EGDT:
      antibiotics_regiment();
      egdt_treatment();
      break;
    }
    nutrition_regiment();

    tracker.AdvanceModelTime(to_seconds(1));
    _time_remaining_min = hours(_duration_hr) - _bg->GetSimulationTime(TimeUnit::min);
  }

  return;
}
//-------------------------------------------------------------------------------
void PatientRun::reset()
{
  //Implementation Details
  if (_PiperacillinTazobactam_bag) {
    delete _PiperacillinTazobactam_bag;
    _PiperacillinTazobactam_bag = nullptr;
  }
  if (_Saline_bag) {
    delete _Saline_bag;
    _Saline_bag = nullptr;
  }

  if (_maintenance_bag) {
    delete _maintenance_bag;
    _maintenance_bag = nullptr;
  }

  if (_bg) {
    delete _bg;
    _bg = nullptr;
  }
  _time_remaining_min = hours(_duration_hr);
  _time_since_feeding_min = 0;
  _time_since_antibiotic_treatment_min = 0;
  _time_applying_antibiotics_min = 0;

  _patient_is_septic = false;
  _first_treatment_occured = false;
  _applying_antibiotics = false;
}
//-------------------------------------------------------------------------------
PatientRun& PatientRun::patient_name(std::string name)
{
  _patient_name = name;
  return *this;
}
//-------------------------------------------------------------------------------
PatientRun& PatientRun::infection_severity(std::string severity)
{
  std::transform(severity.begin(), severity.end(), severity.begin(), [](unsigned char c) { return std::tolower(c); });
  if (severity == "mild") {
    _infection_severity = CDM::enumInfectionSeverity::value::Mild;
    _infection_severity_str = "Mild";
  } else if (severity == "") {
    _infection_severity = CDM::enumInfectionSeverity::value::Moderate;
    _infection_severity_str = "Moderate";
  } else if (severity == "severe") {
    _infection_severity = CDM::enumInfectionSeverity::value::Severe;
    _infection_severity_str = "Severe";
  }
  return *this;
}
//-------------------------------------------------------------------------------
PatientRun& PatientRun::treatment_plan(std::string treatment_plan)
{
  std::transform(treatment_plan.begin(), treatment_plan.end(), treatment_plan.begin(), [](unsigned char c) { return std::tolower(c); });
  if (treatment_plan == "random") {
    std::random_device rdevice; //Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rdevice()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> uniform_distribution(0, 3);
    switch (uniform_distribution(generator)) {
    default:
    case 0:
      _treatment_plan = TreatmentPlan::NONE;
      _treatment_plan_str = "None";
      break;
    case 1:
      _treatment_plan = TreatmentPlan::STANDARD;
      _treatment_plan_str = "Standard";
      break;
    case 2:
      _treatment_plan = TreatmentPlan::REFRESH;
      _treatment_plan_str = "REFRESH";
      break;
    case 3:
      _treatment_plan = TreatmentPlan::EGDT;
      _treatment_plan_str = "EGDT";
      break;
    }
  } else if (treatment_plan == "standard") {
    _treatment_plan = TreatmentPlan::STANDARD;
    _treatment_plan_str = "Standard";
  } else if (treatment_plan == "refresh") {
    _treatment_plan = TreatmentPlan::REFRESH;
    _treatment_plan_str = "REFRESH";
  } else if (treatment_plan == "egdt") {
    _treatment_plan = TreatmentPlan::EGDT;
    _treatment_plan_str = "EGDT";
  }
  return *this;
}
//-------------------------------------------------------------------------------
PatientRun& PatientRun::patient_state(biogears::filesystem::path path)
{
  _patient_state = path;
  return *this;
}
//-------------------------------------------------------------------------------
PatientRun& PatientRun::mic_g_Per_l(double mic)
{
  _mic_g_per_l = mic;
  return *this;
}
//-------------------------------------------------------------------------------
PatientRun& PatientRun::apply_at_m(double apply_at)
{
  _apply_at_m = apply_at;
  return *this;
}
//-------------------------------------------------------------------------------
PatientRun& PatientRun::application_interval_m(double interval)
{
  _application_interval_m = interval;
  return *this;
}
//-------------------------------------------------------------------------------
PatientRun& PatientRun::duration_hr(double trial_length)
{
  _duration_hr = trial_length;
  _time_remaining_min = hours(_duration_hr);
  return *this;
}
