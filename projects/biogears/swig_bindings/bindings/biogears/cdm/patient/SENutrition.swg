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

%nspace biogears::SENutrition;

%ignore biogears::SENutrition::Unload;
%ignore biogears::SENutrition::Load;

%ignore biogears::SENutrition::GetScalar(std::string const &);
%ignore biogears::SENutrition::SetName(std::string const &);
%ignore biogears::SENutrition::SetAnnotation(std::string const &);

%ignore biogears::operator<<(std::ostream& out, const SENutrition& n);

%include <biogears/cdm/patient/SENutrition.h>
