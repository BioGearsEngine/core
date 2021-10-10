

%{
#include <biogears/cdm/patient/assessments/SEPatientAssessment.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%nspace biogears::SEProperty;

%ignore biogears::SEPatientAssessment::Unload;
%ignore biogears::SEPatientAssessment::Load;

%include <biogears/cdm/patient/assessments/SEPatientAssessment.h>


