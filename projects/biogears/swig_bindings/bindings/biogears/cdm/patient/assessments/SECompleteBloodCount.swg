

%{
#include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarAmountPerVolume.swg>
%import <biogears/cdm/properties/SEScalarMassPerAmount.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%nspace biogears::SEProperty;

%ignore biogears::SECompleteBloodCount::Unload;
%ignore biogears::SECompleteBloodCount::Load;

%include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>


