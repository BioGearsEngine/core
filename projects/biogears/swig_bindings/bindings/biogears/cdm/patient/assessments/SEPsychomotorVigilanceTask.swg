%{
#include <biogears/cdm/patient/assessments/SEPsychomotorVigilanceTask.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace biogears::SEPsychomotorVigilanceTask;

%ignore biogears::SEPsychomotorVigilanceTask::Unload;
%ignore biogears::SEPsychomotorVigilanceTask::Load;

%include <biogears/cdm/patient/assessments/SEPsychomotorVigilanceTask.h>
