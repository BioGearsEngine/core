

%{
#include <biogears/cdm/patient/actions/SEBronchoconstriction.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEBronchoconstriction;

%ignore biogears::SEBronchonconstriction::Unload;
%ignore biogears::SEBronchoconstriction::Load;

%include <biogears/cdm/patient/actions/SEBronchoconstriction.h>
