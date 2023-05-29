

%{
#include <biogears/cdm/patient/actions/SEConsciousRespiration.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/patient/actions/SEConsciousRespirationCommand.swg>
%import <biogears/cdm/patient/actions/SEForcedExhale.swg>
%import <biogears/cdm/patient/actions/SEForcedInhale.swg>
%import <biogears/cdm/patient/actions/SEBreathHold.swg>
%import <biogears/cdm/patient/actions/SEUseInhaler.swg>

%nspace BIOGEARS_NAMESPACE SEConsciousRespiration;

%ignore BIOGEARS_NAMESPACE SEConsciousRespiration::Unload;
%ignore BIOGEARS_NAMESPACE SEConsciousRespiration::Load;

%include <biogears/cdm/patient/actions/SEConsciousRespiration.h>
