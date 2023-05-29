%{
#include <biogears/cdm/patient/actions/SESubstanceNasalDose.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SESubstanceAdministration.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SESubstanceNasalDose;

%ignore BIOGEARS_NAMESPACE SESubstanceNasalDose::Unload;
%ignore BIOGEARS_NAMESPACE SESubstanceNasalDose::Load;

%include <biogears/cdm/patient/actions/SESubstanceNasalDose.h>