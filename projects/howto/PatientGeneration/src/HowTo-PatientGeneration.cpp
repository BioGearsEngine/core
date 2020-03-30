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
#include "biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h"
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
#include <biogears/cdm/patient/actions/SEInfection.h>
#include <biogears/cdm/patient/actions/SESubstanceOralDose.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/string/manipulation.h>
using namespace biogears;

//Helper expressions to convert the input to minutes
constexpr double weeks(double n_weeks) { return n_weeks * 7. * 24. * 60.; }
constexpr double days(double n_days) { return n_days * 24. * 60.; }
constexpr double hours(double n_hours) { return n_hours * 60.; }
constexpr double minutes(double n_minutes) { return n_minutes; }
constexpr double seconds(double n_seconds) { return n_seconds / 60.; }

constexpr double to_seconds(double n_minutes) { return n_minutes * 60; }

enum class TreatmentPlan {
  NONE,
  STANDARD,
  REFRESH,
  EGDT,
  RANDOM
};

void antibiotics_regiment(bool& first_treatment_occured, bool& applying_antibiotics,
                          const double apply_at_m, const double application_interval_m,
                          double& time_applying_antibiotics_min, double& time_since_antibiotic_treatment_min,
                          SESubstanceCompoundInfusion& full_antibiotics_bag, SESubstanceCompoundInfusion& empty_antibiotics_bag,
                          std::unique_ptr<biogears::PhysiologyEngine>& bg)
{
  constexpr double deltaT = 1;

  auto current_time = bg->GetSimulationTime(TimeUnit::min);
  if (!first_treatment_occured) {
    if (apply_at_m < current_time) {
      bg->ProcessAction(full_antibiotics_bag);
      first_treatment_occured = true;
      applying_antibiotics = true;
    }
  }


  if (applying_antibiotics) {
    auto volume_applied = full_antibiotics_bag.GetRate().GetValue(VolumePerTimeUnit::mL_Per_min) * time_applying_antibiotics_min;
    if (full_antibiotics_bag.GetBagVolume().GetValue(VolumeUnit::mL) <= volume_applied) {
      bg->ProcessAction(empty_antibiotics_bag);
      applying_antibiotics = false;
    }
    time_applying_antibiotics_min += deltaT;
    time_since_antibiotic_treatment_min = 0;
  } else {
    if (application_interval_m < time_since_antibiotic_treatment_min) {     
      bg->ProcessAction(full_antibiotics_bag);
      applying_antibiotics = true;
    }
    time_applying_antibiotics_min = 0;
    time_since_antibiotic_treatment_min += deltaT;
  }
}
//--------------------------------------------------------------------------------------------------
void refresh_treatment()
{
}
//--------------------------------------------------------------------------------------------------
void egdt_treatment()
{
}
//--------------------------------------------------------------------------------------------------
void nutrition_regiment(double& time_since_feeding_min, std::unique_ptr<biogears::PhysiologyEngine>& bg)
{
  if (time_since_feeding_min > hours(8)) {
    time_since_feeding_min -= hours(8);
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
    bg->ProcessAction(meal);
  }
  time_since_feeding_min += 1;
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Run a Sepsis scenario with variable paramaters
///
/// \param std::string name - Log file name
/// \param double mic_g_per_l - (gram/litter) Minimal Inhibitory Concentration of the Sepsis Infection (Affects Antibiotic response)
/// \param int severity - 0 = low 1 = moderate 2 = severe ; Initial Severity of the Bacterial Infection
/// \param double apply_at_m - (minutes) how far after the initial infection we want o apply antibiotics
/// \param double application_interval_m - (minutes) How often after the first antibiotics application we reapply antibiotics
/// \param std::string patient-state- Path to a patient state relative to the working directory
/// \param double duration_hr - (hours) Total simulation lenght in hours
/// \details
///   Timeline -> Stablization ->  Apply Infection Moderate with MIC ->  AdvanceTime by apply_at -> apply antibiotics
///   Until duration has passed apply antibiotics every application interval
///
//--------------------------------------------------------------------------------------------------
bool HowToPatientGeneration(std::string name, std::string severity, std::string plan, double mic_g_per_l, double apply_at_m, double application_interval_m, std::string patient, double duration_hr)
{
  std::transform(severity.begin(), severity.end(), severity.begin(), [](unsigned char c) { return std::tolower(c); });

  auto eSeverity = CDM::enumInfectionSeverity::value::Eliminated;
  char const* sSeverity = "Eliminated";
  if (severity == "mild") {
    eSeverity = CDM::enumInfectionSeverity::value::Mild;
    sSeverity = "Mild";
  } else if (severity == "") {
    eSeverity = CDM::enumInfectionSeverity::value::Moderate;
    sSeverity = "Moderate";
  } else if (severity == "severe") {
    eSeverity = CDM::enumInfectionSeverity::value::Severe;
    sSeverity = "Severe";
  }

  auto eTreatment = TreatmentPlan::NONE;
  char const* sTreatment = "None";
  if (severity == "random") {
    std::random_device rdevice; //Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rdevice()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> uniform_distribution(0, 3);
    switch (uniform_distribution(generator)) {
    default:
    case 0:
      eTreatment = TreatmentPlan::NONE;
      sTreatment = "None";
      break;
    case 1:
      eTreatment = TreatmentPlan::STANDARD;
      sTreatment = "Standard";
      break;
    case 2:
      eTreatment = TreatmentPlan::REFRESH;
      sTreatment = "REFRESH";
      break;
    case 3:
      eTreatment = TreatmentPlan::EGDT;
      sTreatment = "EGDT";
      break;
    }
  } else if (severity == "standard") {
    eTreatment = TreatmentPlan::STANDARD;
    sTreatment = "Standard";
  } else if (severity == "refresh") {
    eTreatment = TreatmentPlan::REFRESH;
    sTreatment = "REFRESH";
  } else if (severity == "egdt") {
    eTreatment = TreatmentPlan::EGDT;
    sTreatment = "EGDT";
  }

  auto long_name = asprintf("%s_%s_%s_Inhibition_%0.0fg_Per_l_%0.0fm-%0.0fm-%0.0fhr", name.c_str(), sSeverity, sTreatment, mic_g_per_l, apply_at_m, application_interval_m, duration_hr);

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
  bg->GetEngineTrack()->GetDataRequestManager().SetSamplesPerSecond(1. / (5. * 60.));

  HowToTracker tracker(*bg);
  SEInfection infection{};
  infection.SetSeverity(eSeverity);
  SEScalarMassPerVolume infection_mic;
  infection_mic.SetValue(mic_g_per_l, MassPerVolumeUnit::g_Per_L);
  infection.GetMinimumInhibitoryConcentration().Set(infection_mic);
  infection.SetLocation("Gut");
  bg->ProcessAction(infection);
  auto& substances = bg->GetSubstanceManager();

  SESubstanceCompound* PiperacillinTazobactam = bg->GetSubstanceManager().GetCompound("PiperacillinTazobactam");
  SESubstanceCompoundInfusion full_antibiotics_bag{ *PiperacillinTazobactam };
  SESubstanceCompoundInfusion empty_antibiotics_bag{ *PiperacillinTazobactam };
  full_antibiotics_bag.GetBagVolume().SetValue(20.0, VolumeUnit::mL);
  empty_antibiotics_bag.GetBagVolume().SetValue(20.0, VolumeUnit::mL);
  full_antibiotics_bag.GetRate().SetValue(0.667, VolumePerTimeUnit::mL_Per_min);
  empty_antibiotics_bag.GetRate().SetValue(0, VolumePerTimeUnit::mL_Per_min);
  //Load substances we might use

  double time_remaining_min = hours(duration_hr);
  double time_since_feeding_min = 0;
  double time_since_antibiotic_treatment_min = 0;
  double time_applying_antibiotics_min = 0;

  bool first_treatment_occured = false;
  bool applying_antibiotics = false;

  while (0. < time_remaining_min) {
    switch (eTreatment) {
    default:
    case TreatmentPlan::NONE:
      break;
    case TreatmentPlan::STANDARD:
      antibiotics_regiment(first_treatment_occured, applying_antibiotics,
                           apply_at_m, application_interval_m,
                           time_applying_antibiotics_min, time_since_antibiotic_treatment_min,
                           full_antibiotics_bag, empty_antibiotics_bag,
                           bg);
      break;
    case TreatmentPlan::REFRESH:
      antibiotics_regiment(first_treatment_occured, applying_antibiotics,
                           apply_at_m, application_interval_m,
                           time_applying_antibiotics_min, time_since_antibiotic_treatment_min,
                           full_antibiotics_bag, empty_antibiotics_bag,
                           bg);
      refresh_treatment();
      break;
    case TreatmentPlan::EGDT:
      antibiotics_regiment(first_treatment_occured, applying_antibiotics,
                           apply_at_m, application_interval_m,
                           time_applying_antibiotics_min, time_since_antibiotic_treatment_min,
                           full_antibiotics_bag, empty_antibiotics_bag,
                           bg);
      egdt_treatment();
      break;
    }
    nutrition_regiment(time_since_feeding_min, bg);

    tracker.AdvanceModelTime(to_seconds(1));
    time_remaining_min = hours(duration_hr) - bg->GetSimulationTime(TimeUnit::min);
  }

  return true;
}
