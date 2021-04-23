

%{
#include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalarForce.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEChestcompression.swg>

%nspace biogears::SEChestCompressionForce;

%ignore biogears::SEChestCompressionForce::Unload;
%ignore biogears::SEChestCompressionForce::Load;

%include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
