// Add necessary symbols to generated header
%{
#include<biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
#include <biogears/cdm/system/SESystem.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>


%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%import <biogears/cdm/system/equipment/Inhaler/SEInhaler.swg>
%import <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerAction.swg>


%rename (GetConfiguration_const) BIOGEARS_NAMESPACE SEInhalerConfiguration::GetConfiguration() const;
%rename (SetConfigurationFile_const) BIOGEARS_NAMESPACE SEInhalerConfiguration::SetConfigurationFile(std::string const &);

%include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>
