%{
#include <biogears/cdm/patient/assessments/SESequentialOrganFailureAssessment.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalar.swg>


%nspace biogears::SESequentialOrganFailureAssessment;

%ignore biogears::SESequentialOrganFailureAssessment::Unload;
%ignore biogears::SESequentialOrganFailureAssessment::Load;

%include <biogears/cdm/patient/assessments/SESequentialOrganFailureAssessment.h>
