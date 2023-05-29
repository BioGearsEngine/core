%{
#include <biogears/cdm/patient/actions/SEPericardialEffusion.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>

%nspace BIOGEARS_NAMESPACE SEPericardialEffusion;

%ignore BIOGEARS_NAMESPACE SEPericardialEffusion::Unload;
%ignore BIOGEARS_NAMESPACE SEPericardialEffusion::Load;
 
%include <biogears/cdm/patient/actions/SEPericardialEffusion.h>