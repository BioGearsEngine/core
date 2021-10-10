%{
#include <biogears/cdm/system/physiology/SETissueSystem.h>
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

%nspace biogears::SETissueSystem;

%ignore biogears::SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstancePharmacokinetics::Unload;
%ignore biogears::SESubstancePharmacokinetics::Load;

%rename (GetDehydrationFraction_const)  biogears::SETissueSystem::GetDehydrationFraction() const ;
%rename (GetIntracellularFluidPH_const)  biogears::SETissueSystem::GetIntracellularFluidPH() const ;
%rename (GetRespiratoryExchangeRatio_const)  biogears::SETissueSystem::GetRespiratoryExchangeRatio() const ;

%include <biogears/cdm/system/physiology/SETissueSystem.h>