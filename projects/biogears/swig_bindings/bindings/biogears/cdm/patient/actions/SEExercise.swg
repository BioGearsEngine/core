%{
#include <biogears/cdm/patient/actions/SEExercise.h>
#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEExercise
#endif

%include <biogears/exports.swg>

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

%nspace BIOGEARS_NAMESPACE SEExercise;
// %nspace BIOGEARS_NAMESPACE SEExercise::SEGenericExercise;
// %nspace BIOGEARS_NAMESPACE SEExercise::SECyclingExercise;
// %nspace BIOGEARS_NAMESPACE SEExercise::SERunningExercise;
// %nspace BIOGEARS_NAMESPACE SEExercise::SEStrengthExercise;
%ignore BIOGEARS_NAMESPACE SEExercise::Unload;
%ignore BIOGEARS_NAMESPACE SEExercise::Load;

%rename(GetNutrition_const) BIOGEARS_NAMESPACE SEExercise::GetNutrition() const;
%ignore BIOGEARS_NAMESPACE SEExercise::SetNutritionFile(std::string const &);

%rename(GetGenericExercise_const ) BIOGEARS_NAMESPACE SEExercise::GetGenericExercise() const;
%rename(GetCyclingExercise_const ) BIOGEARS_NAMESPACE SEExercise::GetCyclingExercise() const;
%rename(GetRunningExercise_const ) BIOGEARS_NAMESPACE SEExercise::GetRunningExercise() const;
%rename(GetStrengthExercise_const) BIOGEARS_NAMESPACE SEExercise::GetStrengthExercise() const;

%include <biogears/cdm/patient/actions/SEExercise.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SEExercise*>;
%template (vector_SEExercise_ptr)  std::vector<BIOGEARS_NAMESPACE SEExercise*>;