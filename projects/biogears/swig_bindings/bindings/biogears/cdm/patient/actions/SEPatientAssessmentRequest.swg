%{
#include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace biogears::SEPatientAssessmentRequest;

%ignore biogears::SEPatientAssessmentRequest::Unload;
%ignore biogears::SEPatientAssessmentRequest::Load;

%rename (SetPatientAssessmentType) biogears::SEPatientAssessmentRequest::SetType;
%rename (GetPatientAssessmentType) biogears::SEPatientAssessmentRequest::GetType;

%include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
