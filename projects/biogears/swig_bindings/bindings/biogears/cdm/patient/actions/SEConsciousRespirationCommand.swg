

%{
#include <biogears/cdm/patient/actions/SEConsciousRespirationCommand.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEConsciousRespirationCommand;

%ignore BIOGEARS_NAMESPACE SEConsciousRespirationCommand::Unload;
%ignore BIOGEARS_NAMESPACE SEConsciousRespirationCommand::Load;

%include <biogears/cdm/patient/actions/SEConsciousRespirationCommand.h>
