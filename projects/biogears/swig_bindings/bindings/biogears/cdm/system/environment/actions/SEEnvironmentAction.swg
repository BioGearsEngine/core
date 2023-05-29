%{
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/scenario/SEAction.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%nspace BIOGEARS_NAMESPACE SEEnvironmentAction;

%ignore BIOGEARS_NAMESPACE SEEnvironmentAction::Unload;
%ignore BIOGEARS_NAMESPACE SEEnvironmentAction::Load;

%ignore BIOGEARS_NAMESPACE SEEnvironmentAction::newFromBind(const CDM::ActionData& action, SESubstanceManager& substances);
%ignore BIOGEARS_NAMESPACE SEEnvironmentAction::SetComment(const std::string& name);

%ignore operator<<(std::ostream& out, const SEAction& a);

%ignore SEEnvironmentAction::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>

