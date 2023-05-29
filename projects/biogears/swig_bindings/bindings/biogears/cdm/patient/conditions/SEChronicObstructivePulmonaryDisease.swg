%{
#include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace BIOGEARS_NAMESPACE SEChronicObstructivePulmonaryDisease;

%ignore BIOGEARS_NAMESPACE SEChronicObstructivePulmonaryDisease::Unload;
%ignore BIOGEARS_NAMESPACE SEChronicObstructivePulmonaryDisease::Load;


%include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>