%{
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarTemperature.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarFrequency.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%nspace BIOGEARS_NAMESPACE SEDrugSystem;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Load;


%rename (GetAntibioticActivity_const) BIOGEARS_NAMESPACE SEDrugSystem::GetAntibioticActivity() const;
%rename (GetBronchodilationLevel_const) BIOGEARS_NAMESPACE SEDrugSystem::GetBronchodilationLevel() const ;
%rename (GetHemorrhageChange_const) BIOGEARS_NAMESPACE SEDrugSystem::GetHemorrhageChange() const;
%rename (GetNeuromuscularBlockLevel_const) BIOGEARS_NAMESPACE SEDrugSystem::GetNeuromuscularBlockLevel() const;
%rename (GetPainToleranceChange_const) BIOGEARS_NAMESPACE SEDrugSystem::GetPainToleranceChange() const ;
%rename (GetSedationLevel_const) BIOGEARS_NAMESPACE SEDrugSystem::GetSedationLevel() const ;
%rename (GetTubularPermeabilityChange_const) BIOGEARS_NAMESPACE SEDrugSystem::GetTubularPermeabilityChange() const ;
%rename (GetCentralNervousResponse_const) BIOGEARS_NAMESPACE SEDrugSystem::GetCentralNervousResponse() const ;

%include <biogears/cdm/system/physiology/SEDrugSystem.h>