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

#pragma once

#include "biogears/cdm/engine/PhysiologyEngine.h"
#include "biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h"
#include "biogears/cdm/patient/actions/SESubstanceInfusion.h"
#include <biogears/filesystem/path.h>

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

enum class RefreshState {
  NONE,
  INITIAL_BOLUS,
  NEOADRENELINE_INFUSION,
  MAINTINCE_FLUIDS,
  REASSESSMENT,
};

class PatientRun {
public:
  ~PatientRun()
  {
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
  }
  void run();
  void reset();

  PatientRun& patient_name(std::string);
  PatientRun& infection_severity(std::string);
  PatientRun& treatment_plan(std::string);

  PatientRun& patient_state(biogears::filesystem::path);

  PatientRun& mic_g_Per_l(double);
  PatientRun& apply_at_m(double);
  PatientRun& application_interval_m(double);
  PatientRun& duration_hr(double);

protected:
  void antibiotics_regiment();
  void refresh_treatment();
  void egdt_treatment();
  void nutrition_regiment();

private:
  //Configuration Details
  std::string _patient_name;
  char const* _infection_severity_str;
  CDM::enumInfectionSeverity::value _infection_severity = CDM::enumInfectionSeverity::value::Eliminated;
  char const* _treatment_plan_str;
  TreatmentPlan _treatment_plan = TreatmentPlan::STANDARD;

  biogears::filesystem::path _patient_state;

  double _mic_g_per_l;
  double _apply_at_m;
  double _application_interval_m;
  double _duration_hr;

  //Implementation Details
  biogears::SESubstanceCompoundInfusion* _full_PiperacillinTazobactam_bag = nullptr;
  biogears::SESubstanceCompoundInfusion* _empty_PiperacillinTazobactam_bag = nullptr;
  biogears::SESubstanceCompoundInfusion* _full_Saline_bag = nullptr;
  biogears::SESubstanceCompoundInfusion* _empty_Saline_bag = nullptr;
  biogears::SESubstanceCompoundInfusion* _full_maintenance_bag = nullptr;
  biogears::SESubstanceCompoundInfusion* _empty_maintenance_bag = nullptr;

  biogears::SESubstanceInfusion* _Norepinphrine = nullptr;

  biogears::PhysiologyEngine* _bg = nullptr;

  double _time_remaining_min = 0;
  double _time_since_feeding_min = 0;
  double _time_since_antibiotic_treatment_min = 0;
  double _time_applying_antibiotics_min = 0;

  bool _patient_is_septic = false;
  bool _first_treatment_occured = false;
  bool _applying_antibiotics = false;

  //REFRESH Implementation Details
  RefreshState _refresh_state = RefreshState::NONE;
  double _time_to_reassessment = 60.;
  double _initial_hour_remaining_m = 60.;
  double _maintenance_fluids_remaining_m = 0.;
  bool _norepinphrine_titrate_active = false;
  double _time_since_norepinphrine_titrage_active_m = 0.0
};
