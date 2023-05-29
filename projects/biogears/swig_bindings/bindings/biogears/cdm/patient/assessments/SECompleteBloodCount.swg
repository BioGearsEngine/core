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

%nspace BIOGEARS_NAMESPACE SECompleteBloodCount;

%ignore BIOGEARS_NAMESPACE SECompleteBloodCount::Unload;
%ignore BIOGEARS_NAMESPACE SECompleteBloodCount::Load;

%include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>
