%{
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/scenario/SECondition.swg>

%nspace biogears::SEPatientCondition;

%ignore biogears::SEPatientCondition::Unload;
%ignore biogears::SEPatientCondition::Load;

%ignore biogears::SECondition::ToString(std::ostream& str) const; 

%include <biogears/cdm/patient/conditions/SEPatientCondition.h>
