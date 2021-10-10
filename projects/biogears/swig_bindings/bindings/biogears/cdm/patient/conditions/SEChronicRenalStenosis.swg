%{
#include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace biogears::SEChronicRenalStenosis;

%ignore biogears::SEChronicRenalStenosis::Unload;
%ignore biogears::SEChronicRenalStenosis::Load;


%include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
