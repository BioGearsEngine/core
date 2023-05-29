%{
#include <biogears/cdm/patient/conditions/SEChronicHeartFailure.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace BIOGEARS_NAMESPACE SEChronicHeartFailure;

%ignore BIOGEARS_NAMESPACE SEChronicHeartFailure::Unload;
%ignore BIOGEARS_NAMESPACE SEChronicHeartFailure::Load;

%include <biogears/cdm/patient/conditions/SEChronicHeartFailure.h>
