%{
#include <biogears/cdm/patient/actions/SESubstanceAdministration.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SESubstanceAdministration;

%ignore biogears::SESubstanceAdministration::Unload;
%ignore biogears::SESubstanceAdministration::Load;
  
%include <biogears/cdm/patient/actions/SESubstanceAdministration.h>