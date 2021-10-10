%{
#include <biogears/cdm/system/physiology/SERenalSystem.h>
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
%import <biogears/cdm/properties/SEScalarFlowResistance.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTimePressure.swg>
%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.swg>
%import <biogears/cdm/properties/SEScalarOsmolarity.swg>
%import <biogears/cdm/properties/SEScalarOsmolality.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace biogears::SERenalSystem;

%ignore biogears::SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstancePharmacokinetics::Unload;
%ignore biogears::SESubstancePharmacokinetics::Load;

%rename (GetFiltrationFraction_const) biogears::SERenalSystem::GetFiltrationFraction() const;
%rename (GetLeftFiltrationFraction_const) biogears::SERenalSystem::GetLeftFiltrationFraction() const ;
%rename (GetRightFiltrationFraction_const) biogears::SERenalSystem::GetRightFiltrationFraction() const;
%rename (GetUrineSpecificGravity_const) biogears::SERenalSystem::GetUrineSpecificGravity() const ;

%include <biogears/cdm/system/physiology/SERenalSystem.h>