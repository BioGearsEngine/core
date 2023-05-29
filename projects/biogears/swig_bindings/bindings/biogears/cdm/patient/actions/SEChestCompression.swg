

%{
#include <biogears/cdm/patient/actions/SEChestCompression.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEChestCompression;

%ignore BIOGEARS_NAMESPACE SEChestCompression::Unload;
%ignore BIOGEARS_NAMESPACE SEChestCompression::Load;

%include <biogears/cdm/patient/actions/SEChestCompression.h>
