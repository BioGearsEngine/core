%{
#include <biogears/cdm/patient/conditions/SEDiabetesType2.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace biogears::SEDiabetesType2;

%ignore biogears::SEDiabetesType2::Unload;
%ignore biogears::SEDiabetesType2::Load;


%include <biogears/cdm/patient/conditions/SEDiabetesType2.h>
