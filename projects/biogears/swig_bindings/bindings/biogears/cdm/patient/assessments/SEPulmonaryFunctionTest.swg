%{
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEFunctionVolumeVsTime.swg>

%nspace biogears::SEPulmonaryFunctionTest;

%ignore biogears::SEPulmonaryFunctionTest::Unload;
%ignore biogears::SEPulmonaryFunctionTest::Load;

%include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
