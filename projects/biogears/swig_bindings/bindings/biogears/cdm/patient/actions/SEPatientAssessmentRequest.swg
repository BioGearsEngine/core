%{
#include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEPatientAssessmentRequest;

%ignore BIOGEARS_NAMESPACE SEPatientAssessmentRequest::Unload;
%ignore BIOGEARS_NAMESPACE SEPatientAssessmentRequest::Load;

%rename (SetPatientAssessmentType) BIOGEARS_NAMESPACE SEPatientAssessmentRequest::SetType;
%rename (GetPatientAssessmentType) BIOGEARS_NAMESPACE SEPatientAssessmentRequest::GetType;

%include <biogears/cdm/patient/actions/SEPatientAssessmentRequest.h>
