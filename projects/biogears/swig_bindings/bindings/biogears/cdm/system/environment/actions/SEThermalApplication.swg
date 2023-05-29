%{
#include <biogears/cdm/system/environment/actions/SEThermalApplication.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/system/environment/actions/SEEnvironmentAction.swg>
%import <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.swg>
%import <biogears/cdm/system/environment/SEActiveCooling.swg>
%import <biogears/cdm/system/environment/SEActiveHeating.swg>
%import <biogears/cdm/system/environment/SEAppliedTemperature.swg>

%nspace BIOGEARS_NAMESPACE SEThermalApplication;

%ignore BIOGEARS_NAMESPACE SEThermalApplication::Unload;
%ignore BIOGEARS_NAMESPACE SEThermalApplication::Load;

%ignore BIOGEARS_NAMESPACE SEThermalApplication::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/actions/SEThermalApplication.h>