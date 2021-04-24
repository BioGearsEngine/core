%{
#include <biogears/cdm/system/environment/SEEnvironment.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/system/SESystem.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>
%import <biogears/cdm/system/environment/SEEnvironmentalConditions.swg>
%import <biogears/cdm/system/environment/actions/SEEnvironmentChange.swg>
%import <biogears/cdm/system/environment/conditions/SEInitialEnvironment.swg>
%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarHeatConductancePerArea.swg>

%nspace biogears::SEEnvironment;

%ignore biogears::SEEnvironment::Unload;
%ignore biogears::SEEnvironment::Load;

%ignore biogears::SEEnvironment::GetScalar(std::string const &);
%ignore biogears::SEEnvironment::SetName(const std::string& name);

%rename (GetConditions_const) biogears::SEEnvironment::GetConditions() const;

%ignore SEEnvironment::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/SEEnvironment.h>


