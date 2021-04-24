%{
#include <biogears/cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/conditions/SEPatientCondition.swg>
%import <biogears/cdm/patient/conditions/SEChronicHeartFailure.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace biogears::SEChronicVentricularSystolicDysfunction;

%ignore biogears::SEChronicVentricularSystolicDysfunction::Unload;
%ignore biogears::SEChronicVentricularSystolicDysfunction::Load;

%ignore biogears::SEChronicVentricularSystolicDysfunction::ToString(std::ostream& str) const;

%include <biogears/cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.h>
