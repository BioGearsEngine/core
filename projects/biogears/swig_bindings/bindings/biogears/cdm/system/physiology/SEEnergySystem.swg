%{
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarTemperature.swg>
%import <biogears/cdm/properties/SEScalarAmountPerTime.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarMassPerTime.swg>
%import <biogears/cdm/properties/SEScalarPower.swg>

%nspace biogears::SEEnergySystem;

%ignore biogears::SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore biogears::SESubstancePharmacokinetics::Unload;
%ignore biogears::SESubstancePharmacokinetics::Load;

%rename(GetAchievedExerciseLevel_const) biogears::SEEnergySystem::GetAchievedExerciseLevel() const;
%rename(GetFatigueLevel_const) biogears::SEEnergySystem::GetFatigueLevel() const;
%rename(GetTotalWorkRateLevel_const) biogears::SEEnergySystem::GetTotalWorkRateLevel() const ;

%include <biogears/cdm/system/physiology/SEEnergySystem.h>