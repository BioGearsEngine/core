%{
#include <biogears/cdm/patient/conditions/SEChronicPericardialEffusion.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%nspace biogears::SEChronicPericardialEffusion;

%ignore biogears::SEChronicPericardialEffusion::Unload;
%ignore biogears::SEChronicPericardialEffusion::Load;


%include <biogears/cdm/patient/conditions/SEChronicPericardialEffusion.h>