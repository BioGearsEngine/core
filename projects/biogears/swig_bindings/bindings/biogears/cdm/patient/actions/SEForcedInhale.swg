%{
#include <biogears/cdm/patient/actions/SEForcedInhale.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEConsciousRespirationCommand.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace biogears::ForcedInhale;

%ignore biogears::ForcedInhale::Unload;
%ignore biogears::ForcedInhale::Load;

%include <biogears/cdm/patient/actions/SEForcedInhale.h>