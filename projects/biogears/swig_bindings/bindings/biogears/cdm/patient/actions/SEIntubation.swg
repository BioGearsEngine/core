%{
#include <biogears/cdm/patient/actions/SEIntubation.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEIntubation;

%ignore BIOGEARS_NAMESPACE SEIntubation::Unload;
%ignore BIOGEARS_NAMESPACE SEIntubation::Load;

%rename (SetIntubationType) BIOGEARS_NAMESPACE SEIntubation::SetType;
%rename (GetIntubationType) BIOGEARS_NAMESPACE SEIntubation::GetType;

%include <biogears/cdm/patient/actions/SEIntubation.h>
