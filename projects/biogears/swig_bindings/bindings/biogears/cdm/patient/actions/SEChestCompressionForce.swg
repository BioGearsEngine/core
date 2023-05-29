

%{
#include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalarForce.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEChestcompression.swg>

%nspace BIOGEARS_NAMESPACE SEChestCompressionForce;

%ignore BIOGEARS_NAMESPACE SEChestCompressionForce::Unload;
%ignore BIOGEARS_NAMESPACE SEChestCompressionForce::Load;

%include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
