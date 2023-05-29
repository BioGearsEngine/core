

%{
#include <biogears/cdm/patient/actions/SEChestOcclusiveDressing.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEChestOcclusiveDressing;

%ignore BIOGEARS_NAMESPACE SEChestOcclusiveDressing::Unload;
%ignore BIOGEARS_NAMESPACE SEChestOcclusiveDressing::Load;

%include <biogears/cdm/patient/actions/SEChestOcclusiveDressing.h>
