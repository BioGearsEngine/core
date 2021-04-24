%{
#include <biogears/cdm/system/environment/SEEnvironmentalConditions.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceFraction.swg>
%import <biogears/cdm/substance/SESubstanceConcentration.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarLengthPerTime.swg>
%import <biogears/cdm/properties/SEScalarTemperature.swg>
%import <biogears/cdm/properties/SEScalarHeatResistanceArea.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>

%nspace biogears::SEEnvironmentalConditions;

%ignore biogears::SEEnvironmentalConditions::Unload;
%ignore biogears::SEEnvironmentalConditions::Load;
%rename(%s) biogears::SEEnvironmentalConditions::Load(const char* environmentFile);
%ignore biogears::SEEnvironmentalConditions::GetScalar(std::string const &);
%ignore biogears::SEEnvironmentalConditions::SetName(const std::string& name);

%rename(GetAirDensity_const) biogears::SEEnvironmentalConditions::GetAirDensity(const MassPerVolumeUnit& unit) const;
%rename(GetAirVelocity_const) biogears::SEEnvironmentalConditions::GetAirVelocity(const LengthPerTimeUnit& unit) const;
%rename(GetAmbientTemperature_const) biogears::SEEnvironmentalConditions::GetAmbientTemperature(const TemperatureUnit& unit) const;
%rename(GetAtmosphericPressure_const) biogears::SEEnvironmentalConditions::GetAtmosphericPressure(const PressureUnit& unit) const;
%rename(GetClothingResistance_const) biogears::SEEnvironmentalConditions::GetClothingResistance(const HeatResistanceAreaUnit& unit) const;
%rename(GetEmissivity_const) biogears::SEEnvironmentalConditions::GetEmissivity() const;
%rename(GetMeanRadiantTemperature_const) biogears::SEEnvironmentalConditions::GetMeanRadiantTemperature(const TemperatureUnit& unit) const;
%rename(GetRelativeHumidity_const) biogears::SEEnvironmentalConditions::GetRelativeHumidity() const;
%rename(GetRespirationAmbientTemperature_const) biogears::SEEnvironmentalConditions::GetRespirationAmbientTemperature(const TemperatureUnit& unit) const;

%rename(GetAmbientGases_const)    biogears::SEEnvironmentalConditions::GetAmbientGases() const;
%rename(GetAmbientGas_const)      biogears::SEEnvironmentalConditions::GetAmbientGas(const SESubstance& substance) const;
%rename(GetAmbientAerosols_const) biogears::SEEnvironmentalConditions::GetAmbientAerosols() const;
%rename(GetAmbientAerosol_const)  biogears::SEEnvironmentalConditions::GetAmbientAerosol(const SESubstance& substance) const;

%rename(GetConditions_const) biogears::SEEnvironmentalConditions::GetConditions() const;

%ignore SEEnvironmentalConditions::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/SEEnvironmentalConditions.h>