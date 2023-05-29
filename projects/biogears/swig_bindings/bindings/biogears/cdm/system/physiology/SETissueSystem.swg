%{
#include <biogears/cdm/system/physiology/SETissueSystem.h>
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

%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarAmountPerVolume.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>

%nspace BIOGEARS_NAMESPACE SETissueSystem;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Load;

%rename (GetDehydrationFraction_const)  BIOGEARS_NAMESPACE SETissueSystem::GetDehydrationFraction() const ;
%rename (GetIntracellularFluidPH_const)  BIOGEARS_NAMESPACE SETissueSystem::GetIntracellularFluidPH() const ;
%rename (GetRespiratoryExchangeRatio_const)  BIOGEARS_NAMESPACE SETissueSystem::GetRespiratoryExchangeRatio() const ;

%include <biogears/cdm/system/physiology/SETissueSystem.h>