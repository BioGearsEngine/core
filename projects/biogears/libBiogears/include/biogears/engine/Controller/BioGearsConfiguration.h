/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/exports.h>
#include <biogears/schema/biogears/BioGearsConfiguration.hxx>
#include "biogears/cdm/properties/SEScalarFlowElastance.h"

namespace biogears {
class SENutrition;
class SEEnvironmentalConditions;
class SEScalarArea;
class AreaUnit;
class SEScalarLength;
class LengthUnit;
class SEScalarAreaPerTimePressure;
class AreaPerTimePressureUnit;
class SEScalarFlowElastance;
class FlowElastanceUnit;
class SEScalarPowerPerAreaTemperatureToTheFourth;
class PowerPerAreaTemperatureToTheFourthUnit;
class SEScalarHeatCapacitancePerAmount;
class HeatCapacitancePerAmountUnit;
class SEScalarHeatCapacitancePerMass;
class HeatCapacitancePerMassUnit;
class SEScalarEnergyPerAmount;
class EnergyPerAmountUnit;
class SEScalarHeatConductance;
class HeatConductanceUnit;
class SEScalarEnergyPerMass;
class EnergyPerMassUnit;
class SEScalarVolumePerTimePressureArea;
class VolumePerTimePressureAreaUnit;
class SEScalarInverseVolume;
class InverseVolumeUnit;
class SEScalarMassPerAmount;
class MassPerAmountUnit;
class VolumeUnit;
class SEScalarFlowResistance;
class FlowResistanceUnit;
class ElectricResistanceUnit;
class SEScalarElectricResistance;
class SEScalarHeatResistance;
class HeatResistanceUnit;
class SEScalarTemperatureUnit;
class TemperatureUnit;
class MassPerVolumeUnit;
class SEScalarMassPerTime;
class MassPerTimeUnit;
class SEScalarVolumePerTime;
class VolumePerTimeUnit;
class PressureUnit;
/**
* @brief %BioGears specific configuration parameters for all systems/equipment
*/
class BIOGEARS_API BioGearsConfiguration : public PhysiologyEngineConfiguration {
public:
  BioGearsConfiguration(SESubstanceManager& substances);
  virtual ~BioGearsConfiguration();

  virtual void Clear();
  virtual void Initialize();

  virtual void Merge(const PhysiologyEngineConfiguration& from);
  virtual void Merge(const BioGearsConfiguration& from);

  bool Load(const std::string& file);

  virtual bool Load(const CDM::PhysiologyEngineConfigurationData& in);
  virtual bool Load(const CDM::BioGearsConfigurationData& in);
  virtual CDM::BioGearsConfigurationData* Unload() const;

protected:
  virtual void Unload(CDM::BioGearsConfigurationData& data) const;

  SESubstanceManager& m_Substances;

  //////////////////////
  /** Blood Chemistry */
  //////////////////////
public:
  virtual bool HasMeanCorpuscularHemoglobin() const;
  virtual SEScalarMassPerAmount& GetMeanCorpuscularHemoglobin();
  virtual double GetMeanCorpuscularHemoglobin(const MassPerAmountUnit& unit) const;

  virtual bool HasMeanCorpuscularVolume() const;
  virtual SEScalarVolume& GetMeanCorpuscularVolume();
  virtual double GetMeanCorpuscularVolume(const VolumeUnit& unit) const;

  virtual bool HasStandardDiffusionDistance() const;
  virtual SEScalarLength& GetStandardDiffusionDistance();
  virtual double GetStandardDiffusionDistance(const LengthUnit& unit) const;

  virtual bool HasStandardOxygenDiffusionCoefficient() const;
  virtual SEScalarAreaPerTimePressure& GetStandardOxygenDiffusionCoefficient();
  virtual double GetStandardOxygenDiffusionCoefficient(const AreaPerTimePressureUnit& unit) const;

protected:
  SEScalarMassPerAmount* m_MeanCorpuscularHemoglobin;
  SEScalarVolume* m_MeanCorpuscularVolume;
  SEScalarLength* m_StandardDiffusionDistance;
  SEScalarAreaPerTimePressure* m_StandardOxygenDiffusionCoefficient;

  /////////////////////
  /** Cardiovascular */
  /////////////////////
public:
  virtual bool HasLeftHeartElastanceMaximum() const;
  virtual SEScalarFlowElastance& GetLeftHeartElastanceMaximum();
  virtual double GetLeftHeartElastanceMaximum(const FlowElastanceUnit& unit) const;

  virtual bool HasLeftHeartElastanceMinimum() const;
  virtual SEScalarFlowElastance& GetLeftHeartElastanceMinimum();
  virtual double GetLeftHeartElastanceMinimum(const FlowElastanceUnit& unit) const;

  virtual bool HasMinimumBloodVolumeFraction() const;
  virtual SEScalarFraction& GetMinimumBloodVolumeFraction();
  virtual double GetMinimumBloodVolumeFraction() const;

  virtual bool HasRightHeartElastanceMaximum() const;
  virtual SEScalarFlowElastance& GetRightHeartElastanceMaximum();
  virtual double GetRightHeartElastanceMaximum(const FlowElastanceUnit& unit) const;

  virtual bool HasRightHeartElastanceMinimum() const;
  virtual SEScalarFlowElastance& GetRightHeartElastanceMinimum();
  virtual double GetRightHeartElastanceMinimum(const FlowElastanceUnit& unit) const;

  virtual bool HasStandardPulmonaryCapillaryCoverage() const;
  virtual SEScalar& GetStandardPulmonaryCapillaryCoverage();
  virtual double GetStandardPulmonaryCapillaryCoverage() const;

protected:
  SEScalarFlowElastance* m_LeftHeartElastanceMaximum;
  SEScalarFlowElastance* m_LeftHeartElastanceMinimum;
  SEScalarFraction* m_MinimumBloodVolumeFraction;
  SEScalarFlowElastance* m_RightHeartElastanceMaximum;
  SEScalarFlowElastance* m_RightHeartElastanceMinimum;
  SEScalar* m_StandardPulmonaryCapillaryCoverage;

  //////////////
  /** Circuit */
  //////////////
public:
  virtual bool HasCardiovascularOpenResistance() const;
  virtual SEScalarFlowResistance& GetCardiovascularOpenResistance();
  virtual double GetCardiovascularOpenResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasDefaultClosedElectricResistance() const;
  virtual SEScalarElectricResistance& GetDefaultClosedElectricResistance();
  virtual double GetDefaultClosedElectricResistance(const ElectricResistanceUnit& unit) const;

  virtual bool HasDefaultClosedFlowResistance() const;
  virtual SEScalarFlowResistance& GetDefaultClosedFlowResistance();
  virtual double GetDefaultClosedFlowResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasDefaultClosedHeatResistance() const;
  virtual SEScalarHeatResistance& GetDefaultClosedHeatResistance();
  virtual double GetDefaultClosedHeatResistance(const HeatResistanceUnit& unit) const;

  virtual bool HasDefaultOpenElectricResistance() const;
  virtual SEScalarElectricResistance& GetDefaultOpenElectricResistance();
  virtual double GetDefaultOpenElectricResistance(const ElectricResistanceUnit& unit) const;

  virtual bool HasDefaultOpenFlowResistance() const;
  virtual SEScalarFlowResistance& GetDefaultOpenFlowResistance();
  virtual double GetDefaultOpenFlowResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasDefaultOpenHeatResistance() const;
  virtual SEScalarHeatResistance& GetDefaultOpenHeatResistance();
  virtual double GetDefaultOpenHeatResistance(const HeatResistanceUnit& unit) const;

  virtual bool HasMachineClosedResistance() const;
  virtual SEScalarFlowResistance& GetMachineClosedResistance();
  virtual double GetMachineClosedResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasMachineOpenResistance() const;
  virtual SEScalarFlowResistance& GetMachineOpenResistance();
  virtual double GetMachineOpenResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasRespiratoryClosedResistance() const;
  virtual SEScalarFlowResistance& GetRespiratoryClosedResistance();
  virtual double GetRespiratoryClosedResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasRespiratoryOpenResistance() const;
  virtual SEScalarFlowResistance& GetRespiratoryOpenResistance();
  virtual double GetRespiratoryOpenResistance(const FlowResistanceUnit& unit) const;

protected:
  SEScalarFlowResistance* m_CardiovascularOpenResistance;
  SEScalarElectricResistance* m_DefaultClosedElectricResistance;
  SEScalarFlowResistance* m_DefaultClosedFlowResistance;
  SEScalarHeatResistance* m_DefaultClosedHeatResistance;
  SEScalarElectricResistance* m_DefaultOpenElectricResistance;
  SEScalarFlowResistance* m_DefaultOpenFlowResistance;
  SEScalarHeatResistance* m_DefaultOpenHeatResistance;
  SEScalarFlowResistance* m_MachineClosedResistance;
  SEScalarFlowResistance* m_MachineOpenResistance;
  SEScalarFlowResistance* m_RespiratoryClosedResistance;
  SEScalarFlowResistance* m_RespiratoryOpenResistance;

  ////////////////
  /** Constants */
  ////////////////
public:
  virtual bool HasOxygenMetabolicConstant() const;
  virtual SEScalar& GetOxygenMetabolicConstant();
  virtual double GetOxygenMetabolicConstant() const;

  virtual bool HasStefanBoltzmann() const;
  virtual SEScalarPowerPerAreaTemperatureToTheFourth& GetStefanBoltzmann();
  virtual double GetStefanBoltzmann(const PowerPerAreaTemperatureToTheFourthUnit& unit) const;

  virtual bool HasUniversalGasConstant() const;
  virtual SEScalarHeatCapacitancePerAmount& GetUniversalGasConstant();
  virtual double GetUniversalGasConstant(const HeatCapacitancePerAmountUnit& unit) const;

protected:
  SEScalar* m_OxygenMetabolicConstant;
  SEScalarPowerPerAreaTemperatureToTheFourth* m_StefanBoltzmann;
  SEScalarHeatCapacitancePerAmount* m_UniversalGasConstant;

  ////////////
  /** Drugs */
  ////////////
public:
  virtual bool HasUsePDModel() const { return m_PDEnabled != (CDM::enumOnOff::value)-1; }
  virtual bool IsPDEnabled() const { return m_PDEnabled == CDM::enumOnOff::On; }
  virtual void UsePDModel(CDM::enumOnOff::value s) { m_PDEnabled = s; }

protected:
  CDM::enumOnOff::value m_PDEnabled;

  /////////////
  /** Energy */
  /////////////
public:
  virtual bool HasBodySpecificHeat() const;
  virtual SEScalarHeatCapacitancePerMass& GetBodySpecificHeat();
  virtual double GetBodySpecificHeat(const HeatCapacitancePerMassUnit& unit) const;

  virtual bool HasCarbondDioxideProductionFromOxygenConsumptionConstant() const;
  virtual SEScalar& GetCarbondDioxideProductionFromOxygenConsumptionConstant();
  virtual double GetCarbondDioxideProductionFromOxygenConsumptionConstant() const;

  virtual bool HasCoreTemperatureHigh() const;
  virtual SEScalarTemperature& GetCoreTemperatureHigh();
  virtual double GetCoreTemperatureHigh(const TemperatureUnit& unit) const;

  virtual bool HasCoreTemperatureLow() const;
  virtual SEScalarTemperature& GetCoreTemperatureLow();
  virtual double GetCoreTemperatureLow(const TemperatureUnit& unit) const;

  virtual bool HasDeltaCoreTemperatureLow() const;
  virtual SEScalarTemperature& GetDeltaCoreTemperatureLow();
  virtual double GetDeltaCoreTemperatureLow(const TemperatureUnit& unit) const;

  virtual bool HasEnergyPerATP() const;
  virtual SEScalarEnergyPerAmount& GetEnergyPerATP();
  virtual double GetEnergyPerATP(const EnergyPerAmountUnit& unit) const;

  virtual bool HasSweatHeatTransfer() const;
  virtual SEScalarHeatConductance& GetSweatHeatTransfer();
  virtual double GetSweatHeatTransfer(const HeatConductanceUnit& unit) const;

  virtual bool HasVaporizationEnergy() const;
  virtual SEScalarEnergyPerMass& GetVaporizationEnergy();
  virtual double GetVaporizationEnergy(const EnergyPerMassUnit& unit) const;

  virtual bool HasVaporSpecificHeat() const;
  virtual SEScalarHeatCapacitancePerMass& GetVaporSpecificHeat();
  virtual double GetVaporSpecificHeat(const HeatCapacitancePerMassUnit& unit) const;

protected:
  SEScalarHeatCapacitancePerMass* m_BodySpecificHeat;
  SEScalar* m_CarbondDioxideProductionFromOxygenConsumptionConstant;
  SEScalarTemperature* m_CoreTemperatureLow;
  SEScalarTemperature* m_CoreTemperatureHigh;
  SEScalarTemperature* m_DeltaCoreTemperatureLow;
  SEScalarEnergyPerAmount* m_EnergyPerATP;
  SEScalarHeatConductance* m_SweatHeatTransfer;
  SEScalarEnergyPerMass* m_VaporizationEnergy;
  SEScalarHeatCapacitancePerMass* m_VaporSpecificHeat;

  //////////////////
  /** Environment */
  //////////////////
public:
  virtual bool HasAirDensity() const;
  virtual SEScalarMassPerVolume& GetAirDensity();
  virtual double GetAirDensity(const MassPerVolumeUnit& unit) const;

  virtual bool HasAirSpecificHeat() const;
  virtual SEScalarHeatCapacitancePerMass& GetAirSpecificHeat();
  virtual double GetAirSpecificHeat(const HeatCapacitancePerMassUnit& unit) const;

  virtual bool HasMolarMassOfDryAir() const;
  virtual SEScalarMassPerAmount& GetMolarMassOfDryAir();
  virtual double GetMolarMassOfDryAir(const MassPerAmountUnit& unit) const;

  virtual bool HasMolarMassOfWaterVapor() const;
  virtual SEScalarMassPerAmount& GetMolarMassOfWaterVapor();
  virtual double GetMolarMassOfWaterVapor(const MassPerAmountUnit& unit) const;

  virtual bool HasInitialEnvironmentalConditions() const;
  virtual SEEnvironmentalConditions& GetInitialEnvironmentalConditions();
  virtual const SEEnvironmentalConditions* GetInitialEnvironmentalConditions() const;

  virtual bool HasWaterDensity() const;
  virtual SEScalarMassPerVolume& GetWaterDensity();
  virtual double GetWaterDensity(const MassPerVolumeUnit& unit) const;

protected:
  SEScalarMassPerVolume* m_AirDensity;
  SEScalarHeatCapacitancePerMass* m_AirSpecificHeat;
  SEScalarMassPerAmount* m_MolarMassOfDryAir;
  SEScalarMassPerAmount* m_MolarMassOfWaterVapor;
  SEEnvironmentalConditions* m_InitialEnvironmentalConditions;
  SEScalarMassPerVolume* m_WaterDensity;

  ///////////////////////
  /** Gastrointestinal */
  ///////////////////////
public:
  virtual bool HasCalciumDigestionRate() const;
  virtual SEScalarMassPerTime& GetCalciumDigestionRate();
  virtual double GetCalciumDigestionRate(const MassPerTimeUnit& unit) const;

  virtual bool HasCalciumAbsorptionFraction() const;
  virtual SEScalarFraction& GetCalciumAbsorptionFraction();
  virtual double GetCalciumAbsorptionFraction() const;

  virtual bool HasCarbohydrateAbsorptionFraction() const;
  virtual SEScalarFraction& GetCarbohydrateAbsorptionFraction();
  virtual double GetCarbohydrateAbsorptionFraction() const;

  virtual bool HasDefaultCarbohydrateDigestionRate() const;
  virtual SEScalarMassPerTime& GetDefaultCarbohydrateDigestionRate();
  virtual double GetDefaultCarbohydrateDigestionRate(const MassPerTimeUnit& unit) const;

  virtual bool HasDefaultFatDigestionRate() const;
  virtual SEScalarMassPerTime& GetDefaultFatDigestionRate();
  virtual double GetDefaultFatDigestionRate(const MassPerTimeUnit& unit) const;

  virtual bool HasDefaultProteinDigestionRate() const;
  virtual SEScalarMassPerTime& GetDefaultProteinDigestionRate();
  virtual double GetDefaultProteinDigestionRate(const MassPerTimeUnit& unit) const;

  virtual bool HasDefaultStomachContents() const;
  virtual SENutrition& GetDefaultStomachContents();
  virtual const SENutrition* GetDefaultStomachContents() const;

  virtual bool HasFatAbsorptionFraction() const;
  virtual SEScalarFraction& GetFatAbsorptionFraction();
  virtual double GetFatAbsorptionFraction() const;

  virtual bool HasProteinToUreaFraction() const;
  virtual SEScalarFraction& GetProteinToUreaFraction();
  virtual double GetProteinToUreaFraction() const;

  virtual bool HasWaterDigestionRate() const;
  virtual SEScalarVolumePerTime& GetWaterDigestionRate();
  virtual double GetWaterDigestionRate(const VolumePerTimeUnit& unit) const;

protected:
  SEScalarMassPerTime* m_CalciumDigestionRate;
  SEScalarFraction* m_CalciumAbsorptionFraction;
  SEScalarFraction* m_CarbohydrateAbsorptionFraction;
  SEScalarMassPerTime* m_DefaultCarbohydrateDigestionRate;
  SEScalarMassPerTime* m_DefaultFatDigestionRate;
  SEScalarMassPerTime* m_DefaultProteinDigestionRate;
  SENutrition* m_DefaultStomachContents;
  SEScalarFraction* m_FatAbsorptionFraction;
  SEScalarFraction* m_ProteinToUreaFraction;
  SEScalarVolumePerTime* m_WaterDigestionRate;

  ///////////////
  /** Nervous */
  /////////////
public:
  virtual bool HasPupilDiameterBaseline() const;
  virtual SEScalarLength& GetPupilDiameterBaseline();
  virtual double GetPupilDiameterBaseline(const LengthUnit& unit) const;

  virtual bool HasEnableCerebral() const { return m_CerebralEnabled != (CDM::enumOnOff::value)-1; }
  virtual bool IsCerebralEnabled() const { return m_CerebralEnabled == CDM::enumOnOff::On; }
  virtual void EnableCerebral(CDM::enumOnOff::value s) { m_CerebralEnabled = s; }

protected:
  CDM::enumOnOff::value m_CerebralEnabled;
  SEScalarLength* m_PupilDiameterBaseline;

  ////////////
  /** Renal */
  ////////////
public:
  virtual bool HasEnableRenal() const { return m_RenalEnabled != (CDM::enumOnOff::value)-1; }
  virtual bool IsRenalEnabled() const { return m_RenalEnabled == CDM::enumOnOff::On; }
  virtual void EnableRenal(CDM::enumOnOff::value s) { m_RenalEnabled = s; }

  virtual bool HasPlasmaSodiumConcentrationSetPoint() const;
  virtual SEScalarMassPerVolume& GetPlasmaSodiumConcentrationSetPoint();
  virtual double GetPlasmaSodiumConcentrationSetPoint(const MassPerVolumeUnit& unit) const;

  virtual bool HasPeritubularPotassiumConcentrationSetPoint() const;
  virtual SEScalarMassPerVolume& GetPeritubularPotassiumConcentrationSetPoint();
  virtual double GetPeritubularPotassiumConcentrationSetPoint(const MassPerVolumeUnit& unit) const;

  virtual bool HasLeftGlomerularFilteringSurfaceAreaBaseline() const;
  virtual SEScalarArea& GetLeftGlomerularFilteringSurfaceAreaBaseline();
  virtual double GetLeftGlomerularFilteringSurfaceAreaBaseline(const AreaUnit& unit) const;

  virtual bool HasLeftGlomerularFluidPermeabilityBaseline() const;
  virtual SEScalarVolumePerTimePressureArea& GetLeftGlomerularFluidPermeabilityBaseline();
  virtual double GetLeftGlomerularFluidPermeabilityBaseline(const VolumePerTimePressureAreaUnit& unit) const;

  virtual bool HasLeftTubularReabsorptionFilteringSurfaceAreaBaseline() const;
  virtual SEScalarArea& GetLeftTubularReabsorptionFilteringSurfaceAreaBaseline();
  virtual double GetLeftTubularReabsorptionFilteringSurfaceAreaBaseline(const AreaUnit& unit) const;

  virtual bool HasLeftTubularReabsorptionFluidPermeabilityBaseline() const;
  virtual SEScalarVolumePerTimePressureArea& GetLeftTubularReabsorptionFluidPermeabilityBaseline();
  virtual double GetLeftTubularReabsorptionFluidPermeabilityBaseline(const VolumePerTimePressureAreaUnit& unit) const;

  virtual bool HasMaximumAfferentResistance() const;
  virtual SEScalarFlowResistance& GetMaximumAfferentResistance();
  virtual double GetMaximumAfferentResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasMinimumAfferentResistance() const;
  virtual SEScalarFlowResistance& GetMinimumAfferentResistance();
  virtual double GetMinimumAfferentResistance(const FlowResistanceUnit& unit) const;

  virtual bool HasRightGlomerularFilteringSurfaceAreaBaseline() const;
  virtual SEScalarArea& GetRightGlomerularFilteringSurfaceAreaBaseline();
  virtual double GetRightGlomerularFilteringSurfaceAreaBaseline(const AreaUnit& unit) const;

  virtual bool HasRightGlomerularFluidPermeabilityBaseline() const;
  virtual SEScalarVolumePerTimePressureArea& GetRightGlomerularFluidPermeabilityBaseline();
  virtual double GetRightGlomerularFluidPermeabilityBaseline(const VolumePerTimePressureAreaUnit& unit) const;

  virtual bool HasRightTubularReabsorptionFilteringSurfaceAreaBaseline() const;
  virtual SEScalarArea& GetRightTubularReabsorptionFilteringSurfaceAreaBaseline();
  virtual double GetRightTubularReabsorptionFilteringSurfaceAreaBaseline(const AreaUnit& unit) const;

  virtual bool HasRightTubularReabsorptionFluidPermeabilityBaseline() const;
  virtual SEScalarVolumePerTimePressureArea& GetRightTubularReabsorptionFluidPermeabilityBaseline();
  virtual double GetRightTubularReabsorptionFluidPermeabilityBaseline(const VolumePerTimePressureAreaUnit& unit) const;

  virtual bool HasTargetSodiumDelivery();
  virtual SEScalarMassPerTime& GetTargetSodiumDelivery();

protected:
  CDM::enumOnOff::value m_RenalEnabled;

  SEScalarMassPerVolume* m_PlasmaSodiumConcentrationSetPoint;
  SEScalarMassPerVolume* m_PeritubularPotassiumConcentrationSetPoint;

  SEScalarArea* m_LeftGlomerularFilteringSurfaceAreaBaseline;
  SEScalarVolumePerTimePressureArea* m_LeftGlomerularFluidPermeabilityBaseline;
  SEScalarArea* m_LeftTubularReabsorptionFilteringSurfaceAreaBaseline;
  SEScalarVolumePerTimePressureArea* m_LeftTubularReabsorptionFluidPermeabilityBaseline;

  SEScalarArea* m_RightGlomerularFilteringSurfaceAreaBaseline;
  SEScalarVolumePerTimePressureArea* m_RightGlomerularFluidPermeabilityBaseline;
  SEScalarArea* m_RightTubularReabsorptionFilteringSurfaceAreaBaseline;
  SEScalarVolumePerTimePressureArea* m_RightTubularReabsorptionFluidPermeabilityBaseline;

  SEScalarFlowResistance* m_MaximumAfferentResistance;
  SEScalarFlowResistance* m_MinimumAfferentResistance;

  SEScalarMassPerTime* m_TargetSodiumDelivery;

  //////////////////
  /** Respiratory */
  //////////////////
public:
  virtual bool HasPleuralComplianceSensitivity() const;
  virtual SEScalarInverseVolume& GetPleuralComplianceSensitivity();
  virtual double GetPleuralComplianceSensitivity(const InverseVolumeUnit& unit) const;

  virtual bool HasPulmonaryVentilationRateMaximum() const;
  virtual SEScalarVolumePerTime& GetPulmonaryVentilationRateMaximum();
  virtual double GetPulmonaryVentilationRateMaximum(const VolumePerTimeUnit& unit) const;

  virtual bool HasVentilatoryOcclusionPressure() const;
  virtual SEScalarPressure& GetVentilatoryOcclusionPressure();
  virtual double GetVentilatoryOcclusionPressure(const PressureUnit& unit) const;

protected:
  SEScalarInverseVolume* m_PleuralComplianceSensitivity;
  SEScalarVolumePerTime* m_PulmonaryVentilationRateMaximum;
  SEScalarPressure* m_VentilatoryOcclusionPressure;

  /////////////
  /** Tissue */
  /////////////
public:
  virtual bool HasEnableTissue() const { return m_TissueEnabled != (CDM::enumOnOff::value)-1; }
  virtual bool IsTissueEnabled() const { return m_TissueEnabled == CDM::enumOnOff::On; }
  virtual void EnableTissue(CDM::enumOnOff::value s) { m_TissueEnabled = s; }

protected:
  CDM::enumOnOff::value m_TissueEnabled;
};
}