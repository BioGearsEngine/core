%{
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/scenario/SECondition.swg>

%nspace BIOGEARS_NAMESPACE SEPatientCondition;

%ignore BIOGEARS_NAMESPACE SEPatientCondition::Unload;
%ignore BIOGEARS_NAMESPACE SEPatientCondition::Load;

%ignore BIOGEARS_NAMESPACE SECondition::ToString(std::ostream& str) const; 

%include <biogears/cdm/patient/conditions/SEPatientCondition.h>
