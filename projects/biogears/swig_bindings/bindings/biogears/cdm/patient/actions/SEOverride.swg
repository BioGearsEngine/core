%{
#include <biogears/cdm/patient/actions/SEOverride.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarAmountPerTime.swg>
%import <biogears/cdm/properties/SEScalarAmountPerVolume.swg>
%import <biogears/cdm/properties/SEScalarFlowCompliance.swg>
%import <biogears/cdm/properties/SEScalarFlowResistance.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarFrequency.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerTime.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarOsmolality.swg>
%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarTemperature.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>

%nspace biogears::SEOverride;

%ignore biogears::SEOverride::Unload;
%ignore biogears::SEOverride::Load;

%rename (GetArterialPHOverride_const)  biogears::SEOverride::GetArterialPHOverride() const ;
%rename (GetVenousPHOverride_const)  biogears::SEOverride::GetVenousPHOverride() const ;
%rename (GetCO2SaturationOverride_const)  biogears::SEOverride::GetCO2SaturationOverride() const ;
%rename (GetCOSaturationOverride_const)  biogears::SEOverride::GetCOSaturationOverride() const ;
%rename (GetO2SaturationOverride_const)  biogears::SEOverride::GetO2SaturationOverride() const ;
%rename (GetAchievedExerciseLevelOverride_const)  biogears::SEOverride::GetAchievedExerciseLevelOverride() const ;
%rename (GetFatigueLevelOverride_const)  biogears::SEOverride::GetFatigueLevelOverride() const ;
%rename (GetTotalWorkRateLevelOverride_const)  biogears::SEOverride::GetTotalWorkRateLevelOverride() const ;

%include <biogears/cdm/patient/actions/SEOverride.h>
