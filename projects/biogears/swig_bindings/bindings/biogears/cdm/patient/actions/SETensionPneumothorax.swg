%{
#include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace BIOGEARS_NAMESPACE SETensionPneumothorax;

%ignore BIOGEARS_NAMESPACE SETensionPneumothorax::Unload;
%ignore BIOGEARS_NAMESPACE SETensionPneumothorax::Load;

%rename (SetTensionPneumothoraxType) BIOGEARS_NAMESPACE SETensionPneumothorax::SetType;
%rename (GetTensionPneumothoraxType) BIOGEARS_NAMESPACE SETensionPneumothorax::GetType;

%include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
