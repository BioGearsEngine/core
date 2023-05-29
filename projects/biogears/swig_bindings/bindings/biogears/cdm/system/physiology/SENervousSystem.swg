
%{
#include <biogears/cdm/system/physiology/SENervousSystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/patient/actions/SEPatientAction.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%import <biogears/cdm/patient/actions/SEPupillaryResponse.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace BIOGEARS_NAMESPACE SENervousSystem;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Load;

%rename (GetComplianceScale_const) BIOGEARS_NAMESPACE SENervousSystem::GetComplianceScale() const;
%rename (GetHeartRateScale_const) BIOGEARS_NAMESPACE SENervousSystem::GetHeartRateScale() const ;
%rename (GetHeartElastanceScale_const) BIOGEARS_NAMESPACE SENervousSystem::GetHeartElastanceScale() const;
%rename (GetMentalStatus_const) BIOGEARS_NAMESPACE SENervousSystem::GetMentalStatus() const;
%rename (GetResistanceScaleExtrasplanchnic_const) BIOGEARS_NAMESPACE SENervousSystem::GetResistanceScaleExtrasplanchnic() const;
%rename (GetResistanceScaleMuscle_const) BIOGEARS_NAMESPACE SENervousSystem::GetResistanceScaleMuscle() const ;
%rename (GetResistanceScaleMyocardium_const) BIOGEARS_NAMESPACE SENervousSystem::GetResistanceScaleMyocardium() const;
%rename (GetResistanceScaleSplanchnic_const) BIOGEARS_NAMESPACE SENervousSystem::GetResistanceScaleSplanchnic() const;
%rename (GetLeftEyePupillaryResponse_const) BIOGEARS_NAMESPACE SENervousSystem::GetLeftEyePupillaryResponse() const ;
%rename (GetPainVisualAnalogueScale_const) BIOGEARS_NAMESPACE SENervousSystem::GetPainVisualAnalogueScale() const ;
%rename (GetRichmondAgitationSedationScale_const) BIOGEARS_NAMESPACE SENervousSystem::GetRichmondAgitationSedationScale() const ;
%rename (GetRightEyePupillaryResponse_const) BIOGEARS_NAMESPACE SENervousSystem::GetRightEyePupillaryResponse() const;
%rename (GetBiologicalDebt_const) BIOGEARS_NAMESPACE SENervousSystem::GetBiologicalDebt() const ;
%rename (GetAttentionLapses_const) BIOGEARS_NAMESPACE SENervousSystem::GetAttentionLapses() const;

%include <biogears/cdm/system/physiology/SENervousSystem.h>