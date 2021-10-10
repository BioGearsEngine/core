// Add necessary symbols to generated header
%{
#include<biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>


%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%import <biogears/cdm/system/equipment/Inhaler/SEInhaler.swg>
%import <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerAction.swg>


%rename (GetConfiguration_const) biogears::SEInhalerConfiguration::GetConfiguration() const;
%rename (SetConfigurationFile_const) biogears::SEInhalerConfiguration::SetConfigurationFile(std::string const &);

%include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>
