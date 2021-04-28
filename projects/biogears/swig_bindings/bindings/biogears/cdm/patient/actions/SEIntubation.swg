%{
#include <biogears/cdm/patient/actions/SEIntubation.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEIntubation;

%ignore biogears::SEIntubation::Unload;
%ignore biogears::SEIntubation::Load;

%rename (SetIntubationType) biogears::SEIntubation::SetType;
%rename (GetIntubationType) biogears::SEIntubation::GetType;

%include <biogears/cdm/patient/actions/SEIntubation.h>
