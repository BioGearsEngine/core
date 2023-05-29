%{
#include <biogears/cdm/patient/actions/SEForcedExhale.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEConsciousRespirationCommand.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace BIOGEARS_NAMESPACE SEForcedExhale;

%ignore BIOGEARS_NAMESPACE SEForcedExhale::Unload;
%ignore BIOGEARS_NAMESPACE SEForcedExhale::Load;

%include <biogears/cdm/patient/actions/SEForcedExhale.h>
