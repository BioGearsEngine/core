

%{
#include <biogears/cdm/patient/actions/SEChestCompressionForceScale.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEChestCompression.swg>

%nspace biogears::SEChestCompressionForceScale;

%ignore biogears::SEChestCompressionForceScale::Unload;
%ignore biogears::SEChestCompressionForceScale::Load;

%include <biogears/cdm/patient/actions/SEChestCompressionForceScale.h>
