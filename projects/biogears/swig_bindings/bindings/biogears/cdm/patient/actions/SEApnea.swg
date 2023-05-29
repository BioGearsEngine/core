

%{
#include <biogears/cdm/patient/actions/SEApnea.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEApnea;

%ignore BIOGEARS_NAMESPACE SEApnea::Unload;
%ignore BIOGEARS_NAMESPACE SEApnea::Load;

%include <biogears/cdm/patient/actions/SEApnea.h>
