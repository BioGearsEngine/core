%{
#include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>
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
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.swg>
%import <biogears/cdm/system/environment/SEEnvironmentalConditions.swg>

%nspace BIOGEARS_NAMESPACE SEInitialEnvironment;

%ignore BIOGEARS_NAMESPACE SEInitialEnvironment::Unload;
%ignore BIOGEARS_NAMESPACE SEInitialEnvironment::Load;
%ignore BIOGEARS_NAMESPACE SEInitialEnvironment::SetConditionsFile(const std::string& fileName);

%rename (GetConditions_const) BIOGEARS_NAMESPACE SEInitialEnvironment::GetConditions() const;
%ignore SEInitialEnvironment::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>