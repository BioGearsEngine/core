%{
#include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalarAmount.swg>

%nspace biogears::SEUrinalysisMicroscopic;

%ignore biogears::SEUrinalysisMicroscopic::Unload;
%ignore biogears::SEUrinalysisMicroscopic::Load;

%include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>
