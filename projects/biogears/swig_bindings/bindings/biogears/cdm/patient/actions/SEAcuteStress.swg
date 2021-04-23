

%{
#include <biogears/cdm/patient/actions/SEAcuteStress.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEAcuteStress;

%ignore biogears::SEAcuteStress::Unload;
%ignore biogears::SEAcuteStress::Load;

%include <biogears/cdm/patient/actions/SEAcuteStress.h>
