#include "BioGearsConfiguration.h"

#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/system/environment/SEEnvironmentalConditions.h>
#include <biogears/engine/Controller/BioGearsConfiguration.h>

#include <biogears/engine/Systems/Environment.h>

#include <biogears/cdm/properties/SEScalarTypes.h>

#include "../cdm/EngineConfiguration.h"
#include "../cdm/Environment.h"
#include "../cdm/EnvironmentConditions.h"
#include "../cdm/PatientNutrition.h"
#include "../cdm/Property.h"

namespace biogears {
namespace io {

  // class BloodChemistryConfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::BloodChemistryConfigurationData& in, BioGearsConfiguration& out)
  {
    io::Property::Marshall(in.MeanCorpuscularVolume(), out.GetMeanCorpuscularVolume());
    io::Property::Marshall(in.MeanCorpuscularHemoglobin(), out.GetMeanCorpuscularHemoglobin());
    io::Property::Marshall(in.StandardDiffusionDistance(), out.GetStandardDiffusionDistance());
    io::Property::Marshall(in.StandardOxygenDiffusionCoefficient(), out.GetStandardOxygenDiffusionCoefficient());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::BloodChemistryConfigurationData& out)
  {
    // Blood Chemistry
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanCorpuscularHemoglobin)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanCorpuscularVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, StandardDiffusionDistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, StandardOxygenDiffusionCoefficient)
  }
  // class CardiovascularConfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::CardiovascularConfigurationData& in, BioGearsConfiguration& out)
  {
    // Cardiovascular
    io::Property::Marshall(in.LeftHeartElastanceMaximum(), out.GetLeftHeartElastanceMaximum());
    io::Property::Marshall(in.LeftHeartElastanceMinimum(), out.GetLeftHeartElastanceMinimum());
    io::Property::Marshall(in.MinimumBloodVolumeFraction(), out.GetMinimumBloodVolumeFraction());
    io::Property::Marshall(in.RightHeartElastanceMaximum(), out.GetRightHeartElastanceMaximum());
    io::Property::Marshall(in.RightHeartElastanceMinimum(), out.GetRightHeartElastanceMinimum());
    io::Property::Marshall(in.StandardPulmonaryCapillaryCoverage(), out.GetStandardPulmonaryCapillaryCoverage());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::CardiovascularConfigurationData& out)
  {
    // Cardiovascular
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftHeartElastanceMaximum)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftHeartElastanceMinimum)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MinimumBloodVolumeFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightHeartElastanceMaximum)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightHeartElastanceMinimum)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, StandardPulmonaryCapillaryCoverage)
  }
  // class CircuitConfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::CircuitConfigurationData& in, BioGearsConfiguration& out)
  {
    // Circuit
    io::Property::Marshall(in.CardiovascularOpenResistance(), out.GetCardiovascularOpenResistance());
    io::Property::Marshall(in.DefaultOpenElectricResistance(), out.GetDefaultOpenElectricResistance());
    io::Property::Marshall(in.DefaultOpenFlowResistance(), out.GetDefaultOpenFlowResistance());
    io::Property::Marshall(in.DefaultOpenHeatResistance(), out.GetDefaultOpenHeatResistance());
    io::Property::Marshall(in.DefaultClosedElectricResistance(), out.GetDefaultClosedElectricResistance());
    io::Property::Marshall(in.DefaultClosedFlowResistance(), out.GetDefaultClosedFlowResistance());
    io::Property::Marshall(in.DefaultClosedHeatResistance(), out.GetDefaultClosedHeatResistance());
    io::Property::Marshall(in.MachineClosedResistance(), out.GetMachineClosedResistance());
    io::Property::Marshall(in.MachineOpenResistance(), out.GetMachineOpenResistance());
    io::Property::Marshall(in.RespiratoryClosedResistance(), out.GetRespiratoryClosedResistance());
    io::Property::Marshall(in.RespiratoryOpenResistance(), out.GetRespiratoryOpenResistance());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::CircuitConfigurationData& out)
  {
    // Circuits
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CardiovascularOpenResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, DefaultClosedElectricResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, DefaultClosedFlowResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, DefaultClosedHeatResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, DefaultOpenElectricResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, DefaultOpenFlowResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, DefaultOpenHeatResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MachineClosedResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MachineOpenResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespiratoryClosedResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespiratoryOpenResistance)
  }
  // class ConstantCOnfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::ConstantsConfigurationData& in, BioGearsConfiguration& out)
  {
    // Constants
    io::Property::Marshall(in.OxygenMetabolicConstant(), out.GetOxygenMetabolicConstant());
    io::Property::Marshall(in.StefanBoltzmann(), out.GetStefanBoltzmann());
    io::Property::Marshall(in.UniversalGasConstant(), out.GetUniversalGasConstant());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::ConstantsConfigurationData& out)
  {
    // Constants
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, OxygenMetabolicConstant)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, StefanBoltzmann)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, UniversalGasConstant)
  }
  // class DrugsConfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::DrugsConfigurationData& in, BioGearsConfiguration& out)
  {
    // Drugs
    if (in.PDModel().present())
      out.UsePDModel(in.PDModel().get());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::DrugsConfigurationData& out)
  {
    // Drugs
    if (in.HasUsePDModel())
      out.PDModel(in.m_PDEnabled);
  }
  // class EnergyConfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::EnergyConfigurationData& in, BioGearsConfiguration& out)
  {
    // Energy
    io::Property::Marshall(in.BodySpecificHeat(), out.GetBodySpecificHeat());
    io::Property::Marshall(in.CoreTemperatureLow(), out.GetCoreTemperatureLow());
    io::Property::Marshall(in.CoreTemperatureHigh(), out.GetCoreTemperatureHigh());
    io::Property::Marshall(in.DeltaCoreTemperatureLow(), out.GetDeltaCoreTemperatureLow());
    io::Property::Marshall(in.EnergyPerATP(), out.GetEnergyPerATP());
    io::Property::Marshall(in.SweatHeatTransfer(), out.GetSweatHeatTransfer());
    io::Property::Marshall(in.VaporizationEnergy(), out.GetVaporizationEnergy());
    io::Property::Marshall(in.VaporSpecificHeat(), out.GetVaporSpecificHeat());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::EnergyConfigurationData& out)
  {
    // Energy
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BodySpecificHeat)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CoreTemperatureLow)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CoreTemperatureHigh)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, DeltaCoreTemperatureLow)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, EnergyPerATP)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SweatHeatTransfer)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, VaporSpecificHeat)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, VaporizationEnergy)
  }
  // class EnvironmentConfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::EnvironmentConfigurationData& in, BioGearsConfiguration& out)
  {

    // Environment
    io::Property::Marshall(in.AirDensity(), out.GetAirDensity());
    io::Property::Marshall(in.AirSpecificHeat(), out.GetAirSpecificHeat());
    io::Property::Marshall(in.MolarMassOfDryAir(), out.GetMolarMassOfDryAir());
    io::Property::Marshall(in.MolarMassOfWaterVapor(), out.GetMolarMassOfWaterVapor());
    if (in.InitialEnvironmentalConditionsFile().present()) {
      if (!out.GetInitialEnvironmentalConditions().Load(in.InitialEnvironmentalConditionsFile().get())) {
        throw biogears::CommonDataModelException("Unable to load InitialEnvironmentalConditions file");
      }
    } else if (in.InitialEnvironmentalConditions().present()) {
      if (!out.GetInitialEnvironmentalConditions().Load(in.InitialEnvironmentalConditions().get())) {
        throw biogears::CommonDataModelException("Unable to load InitialEnvironmentalConditions");
      }
    }
    io::Property::Marshall(in.WaterDensity(), out.GetWaterDensity());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::EnvironmentConfigurationData& out)
  {
    // Environment
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AirDensity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AirSpecificHeat)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MolarMassOfDryAir)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MolarMassOfWaterVapor)

    CDM_OPTIONAL_ENVIRONMENT_UNMARSHAL_HELPER(in, out, InitialEnvironmentalConditions)

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, WaterDensity)
  }
  // class GastrointestinalConfiguratio
  void BiogearsEngineConfiguration::Marshall(const CDM::GastrointestinalConfigurationData& in, BioGearsConfiguration& out)
  {
    // Gastrointestinal
    io::Property::Marshall(in.CalciumAbsorptionFraction(), out.GetCalciumAbsorptionFraction());
    io::Property::Marshall(in.CalciumDigestionRate(), out.GetCalciumDigestionRate());
    io::Property::Marshall(in.CarbohydrateAbsorptionFraction(), out.GetCarbohydrateAbsorptionFraction());
    if (in.DefaultStomachContentsFile().present()) {
      if (!out.GetDefaultStomachContents().Load(in.DefaultStomachContentsFile().get())) {
        throw biogears::CommonDataModelException("Unable to load Standard Stomach Contents file");
      }
    } else if (in.DefaultStomachContents().present()) {
      if (!out.GetDefaultStomachContents().Load(in.DefaultStomachContents().get())) {
        throw biogears::CommonDataModelException("Unable to load Standard Stomach Contents");
      }
    }
    io::Property::Marshall(in.FatAbsorptionFraction(), out.GetFatAbsorptionFraction());
    io::Property::Marshall(in.ProteinToUreaFraction(), out.GetProteinToUreaFraction());
    io::Property::Marshall(in.WaterDigestionRate(), out.GetWaterDigestionRate());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::GastrointestinalConfigurationData& out)
  {

    // Gastrointestinal
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CalciumAbsorptionFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CalciumDigestionRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CarbohydrateAbsorptionFraction)
    CDM_OPTIONAL_PATIENT_NUTRITION_UNMARSHAL_HELPER(in, out, DefaultStomachContents)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, FatAbsorptionFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ProteinToUreaFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, WaterDigestionRate)
  }
  // class BioGearsConfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::NervousConfigurationData& in, BioGearsConfiguration& out)
  {

    // Nervous
    if (in.EnableCerebral().present())
      out.EnableCerebral(in.EnableCerebral().get());
    io::Property::Marshall(in.PupilDiameterBaseline(), out.GetPupilDiameterBaseline());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::NervousConfigurationData& out)
  {
    // Nervous
    if (in.HasEnableCerebral())
      out.EnableCerebral(in.m_CerebralEnabled);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PupilDiameterBaseline)
  }
  // class RenalConfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::RenalConfigurationData& in, BioGearsConfiguration& out)
  {

    // Renal
    if (in.EnableRenal().present())
      out.EnableRenal(in.EnableRenal().get());

    io::Property::Marshall(in.PlasmaSodiumConcentrationSetPoint(), out.GetPlasmaSodiumConcentrationSetPoint());

    io::Property::Marshall(in.LeftGlomerularFluidPermeabilityBaseline(), out.GetLeftGlomerularFluidPermeabilityBaseline());
    io::Property::Marshall(in.LeftGlomerularFilteringSurfaceAreaBaseline(), out.GetLeftGlomerularFilteringSurfaceAreaBaseline());
    io::Property::Marshall(in.LeftTubularReabsorptionFluidPermeabilityBaseline(), out.GetLeftTubularReabsorptionFluidPermeabilityBaseline());
    io::Property::Marshall(in.LeftTubularReabsorptionFilteringSurfaceAreaBaseline(), out.GetLeftTubularReabsorptionFilteringSurfaceAreaBaseline());

    io::Property::Marshall(in.MaximumAfferentResistance(), out.GetMaximumAfferentResistance());
    io::Property::Marshall(in.MinimumAfferentResistance(), out.GetMinimumAfferentResistance());

    io::Property::Marshall(in.RightGlomerularFluidPermeabilityBaseline(), out.GetRightGlomerularFluidPermeabilityBaseline());
    io::Property::Marshall(in.RightGlomerularFilteringSurfaceAreaBaseline(), out.GetRightGlomerularFilteringSurfaceAreaBaseline());
    io::Property::Marshall(in.RightTubularReabsorptionFluidPermeabilityBaseline(), out.GetRightTubularReabsorptionFluidPermeabilityBaseline());
    io::Property::Marshall(in.RightTubularReabsorptionFilteringSurfaceAreaBaseline(), out.GetRightTubularReabsorptionFilteringSurfaceAreaBaseline());
    io::Property::Marshall(in.TargetSodiumDelivery(), out.GetTargetSodiumDelivery());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::RenalConfigurationData& out)
  {
    // Renal
    if (in.HasEnableRenal())
      out.EnableRenal(in.m_RenalEnabled);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PlasmaSodiumConcentrationSetPoint)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftGlomerularFilteringSurfaceAreaBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftGlomerularFluidPermeabilityBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftTubularReabsorptionFilteringSurfaceAreaBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftTubularReabsorptionFluidPermeabilityBaseline)

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MaximumAfferentResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MinimumAfferentResistance)

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightGlomerularFilteringSurfaceAreaBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightGlomerularFluidPermeabilityBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightTubularReabsorptionFilteringSurfaceAreaBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightTubularReabsorptionFluidPermeabilityBaseline)
  }
  // class RespiratoryConfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::RespiratoryConfigurationData& in, BioGearsConfiguration& out)
  {

    // Respiratory
    io::Property::Marshall(in.PleuralComplianceSensitivity(), out.GetPleuralComplianceSensitivity());
    io::Property::Marshall(in.PulmonaryVentilationRateMaximum(), out.GetPulmonaryVentilationRateMaximum());
    io::Property::Marshall(in.VentilatoryOcclusionPressure(), out.GetVentilatoryOcclusionPressure());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::RespiratoryConfigurationData& out)
  {
    // Respiratory
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PleuralComplianceSensitivity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryVentilationRateMaximum)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, VentilatoryOcclusionPressure)
  }
  // class TissueConfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::TissueConfigurationData& in, BioGearsConfiguration& out)
  {

    // Tissue
    if (in.EnableTissue().present())
      out.EnableTissue(in.EnableTissue().get());
  }
  void BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::TissueConfigurationData& out)
  {
    // Tissue
    if (in.HasEnableTissue())
      out.EnableTissue(in.m_TissueEnabled);
  }
  // class BioGearsConfiguration
  void BiogearsEngineConfiguration::Marshall(const CDM::BioGearsConfigurationData& in, BioGearsConfiguration& out)
  {
    io::EngineConfiguration::Marshall(in, out);

    io::BiogearsEngineConfiguration::Marshall(in.BloodChemistryConfiguration(), out);
    io::BiogearsEngineConfiguration::Marshall(in.CardiovascularConfiguration(), out);
    io::BiogearsEngineConfiguration::Marshall(in.CircuitConfiguration(), out);
    io::BiogearsEngineConfiguration::Marshall(in.ConstantsConfiguration(), out);
    io::BiogearsEngineConfiguration::Marshall(in.DrugsConfiguration(), out);
    io::BiogearsEngineConfiguration::Marshall(in.EnergyConfiguration(), out);
    io::BiogearsEngineConfiguration::Marshall(in.EnvironmentConfiguration(), out);
    io::BiogearsEngineConfiguration::Marshall(in.GastrointestinalConfiguration(), out);
    io::BiogearsEngineConfiguration::Marshall(in.NervousConfiguration(), out);
    io::BiogearsEngineConfiguration::Marshall(in.RenalConfiguration(), out);
    io::BiogearsEngineConfiguration::Marshall(in.RespiratoryConfiguration(), out);
    io::BiogearsEngineConfiguration::Marshall(in.TissueConfiguration(), out);
  }
  void BiogearsEngineConfiguration::BiogearsEngineConfiguration::UnMarshall(const BioGearsConfiguration& in, CDM::BioGearsConfigurationData& out)
  {
    io::EngineConfiguration::UnMarshall(in, out);

    io::BiogearsEngineConfiguration::UnMarshall(in, out.BloodChemistryConfiguration());
    io::BiogearsEngineConfiguration::UnMarshall(in, out.CardiovascularConfiguration());
    io::BiogearsEngineConfiguration::UnMarshall(in, out.CircuitConfiguration());
    io::BiogearsEngineConfiguration::UnMarshall(in, out.ConstantsConfiguration());
    io::BiogearsEngineConfiguration::UnMarshall(in, out.DrugsConfiguration());
    io::BiogearsEngineConfiguration::UnMarshall(in, out.EnergyConfiguration());
    io::BiogearsEngineConfiguration::UnMarshall(in, out.EnvironmentConfiguration());
    io::BiogearsEngineConfiguration::UnMarshall(in, out.GastrointestinalConfiguration());
    io::BiogearsEngineConfiguration::UnMarshall(in, out.NervousConfiguration());
    io::BiogearsEngineConfiguration::UnMarshall(in, out.RenalConfiguration());
    io::BiogearsEngineConfiguration::UnMarshall(in, out.RespiratoryConfiguration());
    io::BiogearsEngineConfiguration::UnMarshall(in, out.TissueConfiguration());
  }
}
}