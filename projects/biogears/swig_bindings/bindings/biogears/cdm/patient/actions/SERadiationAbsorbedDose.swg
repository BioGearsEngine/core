

%{
#include <biogears/cdm/patient/actions/SERadiationAbsorbedDose.h>
%}

%import <biogears/exports.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarEnergyPerMass.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SERadiationAbsorbedDose;

%ignore biogears::SERadiationAbsorbedDose::Unload;
%ignore biogears::SERadiationAbsorbedDose::Load;

%include <biogears/cdm/patient/actions/SERadiationAbsorbedDose.h>
