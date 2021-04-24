%{
#include <biogears/cdm/patient/actions/SEPericardialEffusion.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>

%nspace biogears::SEPericardialEffusion;

%ignore biogears::SEPericardialEffusion::Unload;
%ignore biogears::SEPericardialEffusion::Load;
 
%include <biogears/cdm/patient/actions/SEPericardialEffusion.h>