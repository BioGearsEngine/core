%{
#include <biogears/cdm/system/environment/SEEnvironmentalConditions.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEEnvironmentalConditions
#endif

%include <biogears/exports.swg>

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

%nspace BIOGEARS_NAMESPACE SEEnvironmentalConditions;

%ignore BIOGEARS_NAMESPACE SEEnvironmentalConditions::Unload;
%ignore BIOGEARS_NAMESPACE SEEnvironmentalConditions::Load;
%rename(LoadFile) BIOGEARS_NAMESPACE SEEnvironmentalConditions::Load(const char* environmentFile);
%ignore BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetScalar(std::string const &);
%ignore BIOGEARS_NAMESPACE SEEnvironmentalConditions::SetName(const std::string& name);

%rename(GetAirDensity_const) BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetAirDensity(const MassPerVolumeUnit& unit) const;
%rename(GetAirVelocity_const) BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetAirVelocity(const LengthPerTimeUnit& unit) const;
%rename(GetAmbientTemperature_const) BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetAmbientTemperature(const TemperatureUnit& unit) const;
%rename(GetAtmosphericPressure_const) BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetAtmosphericPressure(const PressureUnit& unit) const;
%rename(GetClothingResistance_const) BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetClothingResistance(const HeatResistanceAreaUnit& unit) const;
%rename(GetEmissivity_const) BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetEmissivity() const;
%rename(GetMeanRadiantTemperature_const) BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetMeanRadiantTemperature(const TemperatureUnit& unit) const;
%rename(GetRelativeHumidity_const) BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetRelativeHumidity() const;
%rename(GetRespirationAmbientTemperature_const) BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetRespirationAmbientTemperature(const TemperatureUnit& unit) const;

%rename(GetAmbientGases_const)    BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetAmbientGases() const;
%rename(GetAmbientGas_const)      BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetAmbientGas(const SESubstance& substance) const;
%rename(GetAmbientAerosols_const) BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetAmbientAerosols() const;
%rename(GetAmbientAerosol_const)  BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetAmbientAerosol(const SESubstance& substance) const;

%rename(GetConditions_const) BIOGEARS_NAMESPACE SEEnvironmentalConditions::GetConditions() const;

%ignore SEEnvironmentalConditions::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/SEEnvironmentalConditions.h>