%{
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/SENutrition.swg>

%import <biogears/cdm/patient/SEPatient.swg>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerTime.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%nspace BIOGEARS_NAMESPACE SEConsumeNutrients;

%ignore BIOGEARS_NAMESPACE SEConsumeNutrients::Unload;
%ignore BIOGEARS_NAMESPACE SEConsumeNutrients::Load;

%rename (GetNutrition_const) BIOGEARS_NAMESPACE SEConsumeNutrients::GetNutrition() const;
%ignore BIOGEARS_NAMESPACE SEConsumeNutrients::SetNutritionFile(std::string const &);

%include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
