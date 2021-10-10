%{
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
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

%nspace biogears::SERespiratorySystem;

%ignore biogears::SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstancePharmacokinetics::Unload;
%ignore biogears::SESubstancePharmacokinetics::Load;

%rename(GetEndTidalCarbonDioxideFraction_const) biogears::SERespiratorySystem::GetEndTidalCarbonDioxideFraction() const ;
%rename(GetInspiratoryExpiratoryRatio_const) biogears::SERespiratorySystem::GetInspiratoryExpiratoryRatio() const ;
%rename(GetSpecificVentilation_const) biogears::SERespiratorySystem::GetSpecificVentilation() const ;

%include <biogears/cdm/system/physiology/SERespiratorySystem.h>