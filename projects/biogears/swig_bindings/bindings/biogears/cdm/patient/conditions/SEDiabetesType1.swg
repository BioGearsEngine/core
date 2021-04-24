%{
#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace biogears::SEDiabetesType1;

%ignore biogears::SEDiabetesType1::Unload;
%ignore biogears::SEDiabetesType1::Load;


%include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
