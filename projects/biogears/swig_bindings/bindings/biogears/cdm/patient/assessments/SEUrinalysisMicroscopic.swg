%{
#include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalarAmount.swg>

%nspace BIOGEARS_NAMESPACE SEUrinalysisMicroscopic;

%ignore BIOGEARS_NAMESPACE SEUrinalysisMicroscopic::Unload;
%ignore BIOGEARS_NAMESPACE SEUrinalysisMicroscopic::Load;

%include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>
