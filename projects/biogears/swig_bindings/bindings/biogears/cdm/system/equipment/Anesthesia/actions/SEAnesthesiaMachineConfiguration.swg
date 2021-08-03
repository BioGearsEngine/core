// Add necessary symbols to generated header
%{
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%import <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.swg>
%import <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineAction.swg>

%rename(GetConfiguration_const) GetConfiguration() const;
%ignore SetConfigurationFile(std::string const &);

%include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>