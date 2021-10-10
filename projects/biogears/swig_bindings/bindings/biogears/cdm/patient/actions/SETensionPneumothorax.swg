%{
#include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace biogears::SETensionPneumothorax;

%ignore biogears::SETensionPneumothorax::Unload;
%ignore biogears::SETensionPneumothorax::Load;

%rename (SetTensionPneumothoraxType) biogears::SETensionPneumothorax::SetType;
%rename (GetTensionPneumothoraxType) biogears::SETensionPneumothorax::GetType;

%include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
