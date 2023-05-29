%{
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/system/environment/actions/SEEnvironmentAction.swg>
%import <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.swg>
%import <biogears/cdm/system/environment/SEEnvironmentalConditions.swg>

%nspace BIOGEARS_NAMESPACE SEEnvironmentChange;

%ignore BIOGEARS_NAMESPACE SEEnvironmentChange::Unload;
%ignore BIOGEARS_NAMESPACE SEEnvironmentChange::Load;

%rename (GetConditions_config) GetConditions() const;

%ignore SetConditionsFile(const std::string& name);
%ignore BIOGEARS_NAMESPACE SEEnvironmentChange::ToString(std::ostream& str) const;


%include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>