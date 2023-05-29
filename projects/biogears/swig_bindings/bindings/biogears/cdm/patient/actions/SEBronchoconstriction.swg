

%{
#include <biogears/cdm/patient/actions/SEBronchoconstriction.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEBronchoconstriction;

%ignore BIOGEARS_NAMESPACE SEBronchonconstriction::Unload;
%ignore BIOGEARS_NAMESPACE SEBronchoconstriction::Load;

%include <biogears/cdm/patient/actions/SEBronchoconstriction.h>
