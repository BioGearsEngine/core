%{
#include <biogears/cdm/patient/actions/SEPulmonaryShunt.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace BIOGEARS_NAMESPACE SEPulmonaryShunt;

%ignore BIOGEARS_NAMESPACE SEPulmonaryShunt::Unload;
%ignore BIOGEARS_NAMESPACE SEPulmonaryShunt::Load;

%include <biogears/cdm/patient/actions/SEPulmonaryShunt.h>
