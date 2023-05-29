%{
#include <biogears/cdm/patient/actions/SEUrinate.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SEUrinate;

%ignore BIOGEARS_NAMESPACE SEUrinate::Unload;
%ignore BIOGEARS_NAMESPACE SEUrinate::Load;

%include <biogears/cdm/patient/actions/SEUrinate.h>
