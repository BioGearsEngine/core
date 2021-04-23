%{
#include <biogears/cdm/patient/assessments/SEUrinalysis.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.swg>

%nspace biogears::SEUrinalysis;

%ignore biogears::SEUrinalysis::Unload;
%ignore biogears::SEUrinalysis::Load;

%include <biogears/cdm/patient/assessments/SEUrinalysis.h>
