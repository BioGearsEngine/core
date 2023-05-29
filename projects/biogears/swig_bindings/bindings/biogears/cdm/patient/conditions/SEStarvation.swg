%{
#include <biogears/cdm/patient/conditions/SEStarvation.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace BIOGEARS_NAMESPACE SEStarvation;

%ignore BIOGEARS_NAMESPACE SEStarvation::Unload;
%ignore BIOGEARS_NAMESPACE SEStarvation::Load;


%include <biogears/cdm/patient/conditions/SEStarvation.h>
