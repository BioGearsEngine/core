

%{
#include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEAsthmaAttack;

%ignore biogears::SEAsthmaAttack::Unload;
%ignore biogears::SEAsthmaAttack::Load;

%include <biogears/cdm/patient/actions/SEAsthmaAttack.h>
