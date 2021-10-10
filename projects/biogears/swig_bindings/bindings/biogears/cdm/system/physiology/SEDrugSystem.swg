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

%nspace biogears::SEDrugSystem;

%ignore biogears::SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstancePharmacokinetics::Unload;
%ignore biogears::SESubstancePharmacokinetics::Load;


%rename (GetAntibioticActivity_const) biogears::SEDrugSystem::GetAntibioticActivity() const;
%rename (GetBronchodilationLevel_const) biogears::SEDrugSystem::GetBronchodilationLevel() const ;
%rename (GetHemorrhageChange_const) biogears::SEDrugSystem::GetHemorrhageChange() const;
%rename (GetNeuromuscularBlockLevel_const) biogears::SEDrugSystem::GetNeuromuscularBlockLevel() const;
%rename (GetPainToleranceChange_const) biogears::SEDrugSystem::GetPainToleranceChange() const ;
%rename (GetSedationLevel_const) biogears::SEDrugSystem::GetSedationLevel() const ;
%rename (GetTubularPermeabilityChange_const) biogears::SEDrugSystem::GetTubularPermeabilityChange() const ;
%rename (GetCentralNervousResponse_const) biogears::SEDrugSystem::GetCentralNervousResponse() const ;

%include <biogears/cdm/system/physiology/SEDrugSystem.h>