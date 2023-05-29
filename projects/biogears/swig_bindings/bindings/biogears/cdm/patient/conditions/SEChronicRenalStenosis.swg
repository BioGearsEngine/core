%{
#include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace BIOGEARS_NAMESPACE SEChronicRenalStenosis;

%ignore BIOGEARS_NAMESPACE SEChronicRenalStenosis::Unload;
%ignore BIOGEARS_NAMESPACE SEChronicRenalStenosis::Load;


%include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
