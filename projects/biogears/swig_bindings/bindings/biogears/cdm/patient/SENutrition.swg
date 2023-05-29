%{
#include <biogears/cdm/patient/SENutrition.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerTime.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%nspace BIOGEARS_NAMESPACE SENutrition;

%ignore BIOGEARS_NAMESPACE SENutrition::Unload;
%ignore BIOGEARS_NAMESPACE SENutrition::Load;

%ignore BIOGEARS_NAMESPACE SENutrition::GetScalar(std::string const &);
%ignore BIOGEARS_NAMESPACE SENutrition::SetName(std::string const &);
%ignore BIOGEARS_NAMESPACE SENutrition::SetAnnotation(std::string const &);

%ignore BIOGEARS_NAMESPACE operator<<(std::ostream& out, const SENutrition& n);

%include <biogears/cdm/patient/SENutrition.h>
