%{
#include <biogears/cdm/patient/actions/SESubstanceAdministration.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceAdministration;

%ignore BIOGEARS_NAMESPACE SESubstanceAdministration::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceAdministration::Load;
  
%include <biogears/cdm/patient/actions/SESubstanceAdministration.h>