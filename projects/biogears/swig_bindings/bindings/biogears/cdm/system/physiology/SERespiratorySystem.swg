%{
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarFlowCompliance.swg>
%import <biogears/cdm/properties/SEScalarFlowResistance.swg>
%import <biogears/cdm/properties/SEScalarFrequency.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%nspace BIOGEARS_NAMESPACE SERespiratorySystem;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Load;

%rename(GetEndTidalCarbonDioxideFraction_const) BIOGEARS_NAMESPACE SERespiratorySystem::GetEndTidalCarbonDioxideFraction() const ;
%rename(GetInspiratoryExpiratoryRatio_const) BIOGEARS_NAMESPACE SERespiratorySystem::GetInspiratoryExpiratoryRatio() const ;
%rename(GetSpecificVentilation_const) BIOGEARS_NAMESPACE SERespiratorySystem::GetSpecificVentilation() const ;

%include <biogears/cdm/system/physiology/SERespiratorySystem.h>