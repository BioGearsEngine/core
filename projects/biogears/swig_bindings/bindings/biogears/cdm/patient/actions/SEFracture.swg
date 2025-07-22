

%{
#include <biogears/cdm/patient/actions/SEFracture.h>
%}

%import <biogears/exports.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEFracture;

%ignore biogears::SEFracture::Unload;
%ignore biogears::SEFracture::Load;

%include <biogears/cdm/patient/actions/SEFracture.h>
