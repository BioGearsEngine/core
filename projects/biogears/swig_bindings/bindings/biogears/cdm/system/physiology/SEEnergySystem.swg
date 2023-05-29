%{
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
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

%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarTemperature.swg>
%import <biogears/cdm/properties/SEScalarAmountPerTime.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarMassPerTime.swg>
%import <biogears/cdm/properties/SEScalarPower.swg>

%nspace BIOGEARS_NAMESPACE SEEnergySystem;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Load;

%rename(GetAchievedExerciseLevel_const) BIOGEARS_NAMESPACE SEEnergySystem::GetAchievedExerciseLevel() const;
%rename(GetFatigueLevel_const) BIOGEARS_NAMESPACE SEEnergySystem::GetFatigueLevel() const;
%rename(GetTotalWorkRateLevel_const) BIOGEARS_NAMESPACE SEEnergySystem::GetTotalWorkRateLevel() const ;

%include <biogears/cdm/system/physiology/SEEnergySystem.h>