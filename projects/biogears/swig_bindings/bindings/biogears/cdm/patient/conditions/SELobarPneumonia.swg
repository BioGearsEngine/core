%{
#include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace BIOGEARS_NAMESPACE SELobarPneumonia;

%ignore BIOGEARS_NAMESPACE SELobarPneumonia::Unload;
%ignore BIOGEARS_NAMESPACE SELobarPneumonia::Load;


%include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
