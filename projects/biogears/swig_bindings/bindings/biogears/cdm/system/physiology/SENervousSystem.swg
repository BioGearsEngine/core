%{
#include <biogears/cdm/system/physiology/SENervousSystem.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%import <biogears/cdm/patient/actions/SEPupillaryResponse.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace biogears::SENervousSystem;

%ignore biogears::SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstancePharmacokinetics::Unload;
%ignore biogears::SESubstancePharmacokinetics::Load;

%rename (GetComplianceScale_const) biogears::SENervousSystem::GetComplianceScale() const;
%rename (GetHeartRateScale_const) biogears::SENervousSystem::GetHeartRateScale() const ;
%rename (GetHeartElastanceScale_const) biogears::SENervousSystem::GetHeartElastanceScale() const;
%rename (GetResistanceScaleExtrasplanchnic_const) biogears::SENervousSystem::GetResistanceScaleExtrasplanchnic() const;
%rename (GetResistanceScaleMuscle_const) biogears::SENervousSystem::GetResistanceScaleMuscle() const ;
%rename (GetResistanceScaleMyocardium_const) biogears::SENervousSystem::GetResistanceScaleMyocardium() const;
%rename (GetResistanceScaleSplanchnic_const) biogears::SENervousSystem::GetResistanceScaleSplanchnic() const;
%rename (GetLeftEyePupillaryResponse_const) biogears::SENervousSystem::GetLeftEyePupillaryResponse() const ;
%rename (GetPainVisualAnalogueScale_const) biogears::SENervousSystem::GetPainVisualAnalogueScale() const ;
%rename (GetRichmondAgitationSedationScale_const) biogears::SENervousSystem::GetRichmondAgitationSedationScale() const ;
%rename (GetRightEyePupillaryResponse_const) biogears::SENervousSystem::GetRightEyePupillaryResponse() const;
%rename (GetBiologicalDebt_const) biogears::SENervousSystem::GetBiologicalDebt() const ;
%rename (GetAttentionLapses_const) biogears::SENervousSystem::GetAttentionLapses() const;

%include <biogears/cdm/system/physiology/SENervousSystem.h>