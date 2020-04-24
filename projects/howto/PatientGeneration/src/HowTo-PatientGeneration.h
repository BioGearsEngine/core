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


constexpr double weeks(double n_weeks) { return n_weeks * 7. * 24. * 60.; } //< weeks -> minutes
constexpr double days(double n_days) { return n_days * 24. * 60.; }//< days -> minutes
constexpr double hours(double n_hours) { return n_hours * 60.; }//< hours -> minutes
constexpr double minutes(double n_minutes) { return n_minutes; }//< minutes -> minutes
constexpr double seconds(double n_seconds) { return n_seconds / 60.; }//< seconds -> minutes
constexpr double to_seconds(double n_minutes) { return n_minutes * 60; }//< minutes -> seconds

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
  MONITORING,
  NOREPINEPHRINE_TITRATE,
};

enum class EGDTState {
  NONE,
  INITIAL_BOLUS,
  MONITORING,
  RAPID_FLUID_BOLUS,
  NOREPINEPHRINE_TITRATE
};

class PatientRun {
public:
  ~PatientRun()
  {
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
  void antibiotics_regimen();
  void refresh_treatment();
  void egdt_treatment();
  void nutrition_regimen();

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
  biogears::SESubstanceCompoundInfusion* _PiperacillinTazobactam_bag = nullptr;
  biogears::SESubstanceCompoundInfusion* _Saline_bag = nullptr;
  biogears::SESubstanceCompoundInfusion* _maintenance_bag = nullptr;

  biogears::SESubstanceInfusion* _Norepinphrine = nullptr;

  biogears::PhysiologyEngine* _bg = nullptr;

  double _time_remaining_min = 0.;
  double _time_since_feeding_min = 0.;
  double _time_since_antibiotic_treatment_min = 0.;
  double _time_applying_antibiotics_min = 0.;

  bool _patient_is_septic = false;
  bool _first_treatment_occured = false;
  bool _applying_antibiotics = false;

  //REFRESH Implementation Details
  RefreshState _refresh_state = RefreshState::NONE;
  double _time_to_reassessment_min = 60.;
  double _maintenance_fluids_remaining_min = 0.;
  bool   _norepinphrine_titrate_active = false;
  double _time_since_norepinphrine_titrage_active_min = 0.0;
  double _treatment_time_remaining_min = 360;

  //EGDT Implementation Details
  EGDTState _egdt_state = EGDTState::NONE;
  double _persistent_low_map_min = 0.;
  double _persistant_stable_map_min = 0.;
  double _high_map_count = 0.;
  double _time_to_full_assessment_min = 0.;
  double _persistant_normal_map_min = 0.;
};
