%{
#include <biogears/cdm/system/physiology/SEHepaticSystem.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%import <biogears/cdm/properties/SEScalarAmountPerTime.swg>
%import <biogears/cdm/properties/SEScalarMassPerTime.swg>

%nspace BIOGEARS_NAMESPACE SEHepaticSystem;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Load;

%include <biogears/cdm/system/physiology/SEHepaticSystem.h>