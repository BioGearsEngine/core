%{
#include <biogears/cdm/patient/assessments/SESequentialOrganFailureAssessment.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalar.swg>


%nspace BIOGEARS_NAMESPACE SESequentialOrganFailureAssessment;

%ignore BIOGEARS_NAMESPACE SESequentialOrganFailureAssessment::Unload;
%ignore BIOGEARS_NAMESPACE SESequentialOrganFailureAssessment::Load;

%include <biogears/cdm/patient/assessments/SESequentialOrganFailureAssessment.h>
