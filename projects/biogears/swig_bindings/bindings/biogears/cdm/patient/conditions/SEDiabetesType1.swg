%{
#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace BIOGEARS_NAMESPACE SEDiabetesType1;

%ignore BIOGEARS_NAMESPACE SEDiabetesType1::Unload;
%ignore BIOGEARS_NAMESPACE SEDiabetesType1::Load;


%include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
