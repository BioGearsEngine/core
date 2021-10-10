%{
#include <biogears/cdm/patient/actions/SEExercise.h>
#include <biogears/cdm/patient/SENutrition.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/SENutrition.swg>

%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarLengthPerTime.swg>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarFrequency.swg>

%nspace biogears::SEExercise;
// %nspace biogears::SEExercise::SEGenericExercise;
// %nspace biogears::SEExercise::SECyclingExercise;
// %nspace biogears::SEExercise::SERunningExercise;
// %nspace biogears::SEExercise::SEStrengthExercise;
%ignore biogears::SEExercise::Unload;
%ignore biogears::SEExercise::Load;

%rename(GetNutrition_const) biogears::SEExercise::GetNutrition() const;
%ignore biogears::SEExercise::SetNutritionFile(std::string const &);

%rename(GetGenericExercise_const ) biogears::SEExercise::GetGenericExercise() const;
%rename(GetCyclingExercise_const ) biogears::SEExercise::GetCyclingExercise() const;
%rename(GetRunningExercise_const ) biogears::SEExercise::GetRunningExercise() const;
%rename(GetStrengthExercise_const) biogears::SEExercise::GetStrengthExercise() const;

%include <biogears/cdm/patient/actions/SEExercise.h>
