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

%nspace BIOGEARS_NAMESPACE SEUrinalysis;

%ignore BIOGEARS_NAMESPACE SEUrinalysis::Unload;
%ignore BIOGEARS_NAMESPACE SEUrinalysis::Load;

%include <biogears/cdm/patient/assessments/SEUrinalysis.h>
