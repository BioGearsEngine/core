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
#include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/string/manipulation.h>

using namespace biogears;

void PatientRun::antibiotics_regiment()
{
  constexpr double deltaT = 1;

  auto current_time = _bg->GetSimulationTime(TimeUnit::min);
  if (!_first_treatment_occured) {
    if (_apply_at_m < current_time) {
      _bg->ProcessAction(*_full_PiperacillinTazobactam_bag);
      _first_treatment_occured = true;
      _applying_antibiotics = true;
    }
  }

  if (_applying_antibiotics) {
    auto volume_applied = _full_PiperacillinTazobactam_bag->GetRate().GetValue(VolumePerTimeUnit::mL_Per_min) * _time_applying_antibiotics_min;
    if (_full_PiperacillinTazobactam_bag->GetBagVolume().GetValue(VolumeUnit::mL) <= volume_applied) {
      _bg->ProcessAction(*_empty_PiperacillinTazobactam_bag);
      _applying_antibiotics = false;
    }
    _time_applying_antibiotics_min += deltaT;
    _time_since_antibiotic_treatment_min = 0;
  } else {
    if (_application_interval_m < _time_since_antibiotic_treatment_min) {
      _bg->ProcessAction(*_full_PiperacillinTazobactam_bag);
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
    _full_PiperacillinTazobactam_bag->GetRate().SetValue(0.75, VolumePerTimeUnit::mL_Per_min);
    _full_PiperacillinTazobactam_bag->GetBagVolume().SetValue(100.0, VolumeUnit::mL);
    _empty_PiperacillinTazobactam_bag->GetRate().SetValue(0, VolumePerTimeUnit::mL_Per_min);
    _empty_PiperacillinTazobactam_bag->GetBagVolume().SetValue(0.0, VolumeUnit::mL);

    SESubstanceCompound* saline = _bg->GetSubstanceManager().GetCompound("Saline");
    SESubstanceCompoundInfusion SalineBolus{ *saline };
    SalineBolus.GetBagVolume().SetValue(100, VolumeUnit::mL);
    SalineBolus.GetRate().SetValue(1000, VolumePerTimeUnit::mL_Per_hr);
    _bg->ProcessAction(SalineBolus);
    _initial_hour_remaining_m = hours(1);

  } else {
    switch (_refresh_state) {
    case RefreshState::INITIAL_BOLUS:
      if (0.99 <= _initial_hour_remaining_m) {
        _initial_hour_remaining_m -= 1.;
      } else {
        _refresh_state = RefreshState::REASSESSMENT;
      }
      break;
    case RefreshState::MAINTINCE_FLUIDS: {
      if (0.99 < _maintenance_fluids_remaining_m) {
        _maintenance_fluids_remaining_m = hours(6);
        double patient_wgt_kg = _bg->GetPatient().GetWeight(MassUnit::kg);
        _full_maintenance_bag->GetBagVolume().SetValue(patient_wgt_kg * 1. /* (ml/kg * hr)*/* 6. /*(hours)*/, VolumeUnit::mL);
        _full_maintenance_bag->GetRate().SetValue(patient_wgt_kg * 1./* (ml/kg * hr) */, VolumePerTimeUnit::mL_Per_hr);
        _bg->ProcessAction(*_full_maintenance_bag);
        _time_since_norepinphrine_titrage_active_m = 0;
      } else {
        _maintenance_fluids_remaining_m -= 1.;
      }
      if ( 0.99 < _time_to_reassessment) {
        _time_to_reassessment -= 1.;
      } else {
        _refresh_state = RefreshState::REASSESSMENT;
      }
      //aintenance fluids and norepinephrine at respective rates of 1 mL/kg/h and 0.18 µg/kg/min.
    } break;
    case RefreshState::NEOADRENELINE_INFUSION: {
      if(!_Norepinphrine) {
        SESubstance* norepinphrine = _bg->GetSubstanceManager().GetSubstance("Norepinphrine");
        _Norepinphrine = std::make_unique<SESubstanceInfusion>(*norepinphrine).release();
      }
      if (!_norepinphrine_titrate_active) {
        _Norepinphrine->GetRate().SetValue(1.0, VolumePerTimeUnit::mL_Per_min);
        _Norepinphrine->GetConcentration().SetValue( 1.0 /* (ug/kg*min) */ * _bg->GetPatient().GetWeight(MassUnit::kg),MassPerVolumeUnit::ug_Per_mL);
        _bg->ProcessAction(*_Norepinphrine);
        _norepinphrine_titrate_active = true;
      } else {
        _time_since_norepinphrine_titrage_active_m += 1.;
        if (65. <= _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) && _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) <= 70.) {
          //Condition 1: Finding correct titrate rate.
          double rate = _Norepinphrine->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
          _Norepinphrine->GetConcentration().SetValue(rate * 0.9, MassPerVolumeUnit::ug_Per_mL);
          _bg->ProcessAction(*_Norepinphrine);
          if( 60. <= _time_since_norepinphrine_titrage_active_m ) {
            _refresh_state = RefreshState::REASSESSMENT;
          }
        } else if (_bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) <= 65.) {
          double rate = _Norepinphrine->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
          _Norepinphrine->GetConcentration().SetValue(rate * 1.1, MassPerVolumeUnit::ug_Per_mL);
          _bg->ProcessAction(*_Norepinphrine);
        } else /* 70. <= _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) .*/{
          //MAYBE THIS IS AN EXIT CONDITION ON TITRATE?
          double rate = _Norepinphrine->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
          _Norepinphrine->GetConcentration().SetValue(rate * 0.75, MassPerVolumeUnit::ug_Per_mL); // FASTER PULL BACK
          _bg->ProcessAction(*_Norepinphrine);
        }
      }
    } break;
    case RefreshState::REASSESSMENT:
      _time_to_reassessment = hours(1);
      if (_bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) < 90. && _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) < 65.) {
        _refresh_state = RefreshState::NEOADRENELINE_INFUSION;
      } else {
        _refresh_state = RefreshState::MAINTINCE_FLUIDS;
      }
      break;
    default:
      break;
    }
  }
}
//-------------------------------------------------------------------------------
void PatientRun::egdt_treatment()
{
  if (!_patient_is_septic && _bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) < 100.) {
    _patient_is_septic = true;
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
  SEInfection infection{};
  infection.SetSeverity(_infection_severity);
  SEScalarMassPerVolume infection_mic;
  infection_mic.SetValue(_mic_g_per_l, MassPerVolumeUnit::g_Per_L);
  infection.GetMinimumInhibitoryConcentration().Set(infection_mic);
  infection.SetLocation("Gut");
  _bg->ProcessAction(infection);
  auto& substances = _bg->GetSubstanceManager();

  SESubstanceCompound* PiperacillinTazobactam = _bg->GetSubstanceManager().GetCompound("PiperacillinTazobactam");
  _full_PiperacillinTazobactam_bag = std::make_unique<SESubstanceCompoundInfusion>(*PiperacillinTazobactam).release();
  _full_PiperacillinTazobactam_bag->GetRate().SetValue(0.75, VolumePerTimeUnit::mL_Per_min);
  _full_PiperacillinTazobactam_bag->GetBagVolume().SetValue(100.0, VolumeUnit::mL);

  _empty_PiperacillinTazobactam_bag = std::make_unique<SESubstanceCompoundInfusion>(*PiperacillinTazobactam).release();
  _empty_PiperacillinTazobactam_bag->GetBagVolume().SetValue(0.0, VolumeUnit::mL);
  _empty_PiperacillinTazobactam_bag->GetRate().SetValue(0, VolumePerTimeUnit::mL_Per_min);

  SESubstanceCompound* Saline = _bg->GetSubstanceManager().GetCompound("Saline");
  _full_Saline_bag = std::make_unique<SESubstanceCompoundInfusion>(*Saline).release();
  _full_Saline_bag->GetBagVolume().SetValue(1000, VolumeUnit::mL);
  _full_Saline_bag->GetRate().SetValue(10, VolumePerTimeUnit::mL_Per_min);

  _empty_Saline_bag = std::make_unique<SESubstanceCompoundInfusion>(*Saline).release();
  _empty_Saline_bag->GetRate().SetValue(0, VolumePerTimeUnit::mL_Per_min);
  _empty_Saline_bag->GetBagVolume().SetValue(0, VolumeUnit::mL);

  SESubstanceCompound* SalinelowDrip = _bg->GetSubstanceManager().GetCompound("SalineSlowDrip");
  _full_maintenance_bag = std::make_unique<SESubstanceCompoundInfusion>(*SalinelowDrip).release();
  _full_maintenance_bag->GetBagVolume().SetValue(1000, VolumeUnit::mL);
  _full_maintenance_bag->GetRate().SetValue(10, VolumePerTimeUnit::mL_Per_min);

  _empty_maintenance_bag = std::make_unique<SESubstanceCompoundInfusion>(*SalinelowDrip).release();
  _empty_maintenance_bag->GetRate().SetValue(0, VolumePerTimeUnit::mL_Per_min);
  _empty_maintenance_bag->GetBagVolume().SetValue(0, VolumeUnit::mL);
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
  if (_full_PiperacillinTazobactam_bag) {
    delete _full_PiperacillinTazobactam_bag;
    _full_PiperacillinTazobactam_bag = nullptr;
  }
  if (_empty_PiperacillinTazobactam_bag) {
    delete _empty_PiperacillinTazobactam_bag;
    _empty_PiperacillinTazobactam_bag = nullptr;
  }
  if (_full_Saline_bag) {
    delete _full_Saline_bag;
    _full_Saline_bag = nullptr;
  }
  if (_empty_Saline_bag) {
    delete _empty_Saline_bag;
    _empty_Saline_bag = nullptr;
  }
  if (_full_maintenance_bag) {
    delete _full_maintenance_bag;
    _full_maintenance_bag = nullptr;
  }
  if (_empty_maintenance_bag) {
    delete _empty_maintenance_bag;
    _empty_maintenance_bag = nullptr;
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
