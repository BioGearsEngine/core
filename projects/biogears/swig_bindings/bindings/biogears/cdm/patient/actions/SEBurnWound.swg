

%{
#include <biogears/cdm/patient/actions/SEBurnWound.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEBurnWound;

%ignore biogears::SEBurnWound::Unload;
%ignore biogears::SEBurnWound::Load;

%include <biogears/cdm/patient/actions/SEBurnWound.h>
