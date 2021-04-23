%{
#include <biogears/cdm/patient/actions/SEUseInhaler.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEConsciousRespirationCommand.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace biogears::SEUseInhaler;

%ignore biogears::SEUseInhaler::Unload;
%ignore biogears::SEUseInhaler::Load;

%include <biogears/cdm/patient/actions/SEUseInhaler.h>
