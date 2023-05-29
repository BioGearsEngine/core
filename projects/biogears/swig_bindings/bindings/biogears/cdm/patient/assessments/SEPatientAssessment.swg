

%{
#include <biogears/cdm/patient/assessments/SEPatientAssessment.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace BIOGEARS_NAMESPACE SEProperty;

%ignore BIOGEARS_NAMESPACE SEPatientAssessment::Unload;
%ignore BIOGEARS_NAMESPACE SEPatientAssessment::Load;

%include <biogears/cdm/patient/assessments/SEPatientAssessment.h>


