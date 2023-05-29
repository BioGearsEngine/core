

%{
#include <biogears/cdm/patient/actions/SERadiationAbsorbedDose.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%import <biogears/exports.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarEnergyPerMass.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SERadiationAbsorbedDose;

%ignore BIOGEARS_NAMESPACE SERadiationAbsorbedDose::Unload;
%ignore BIOGEARS_NAMESPACE SERadiationAbsorbedDose::Load;

%include <biogears/cdm/patient/actions/SERadiationAbsorbedDose.h>
