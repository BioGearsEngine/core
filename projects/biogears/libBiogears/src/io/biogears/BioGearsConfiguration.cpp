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
  void BiogearsEngineConfiguration::UnMarshall(const CDM::BloodChemistryConfigurationData& in, BioGearsConfiguration& out)
  {
    io::Property::UnMarshall(in.MeanCorpuscularVolume(), out.GetMeanCorpuscularVolume());
    io::Property::UnMarshall(in.MeanCorpuscularHemoglobin(), out.GetMeanCorpuscularHemoglobin());
    io::Property::UnMarshall(in.StandardDiffusionDistance(), out.GetStandardDiffusionDistance());
    io::Property::UnMarshall(in.StandardOxygenDiffusionCoefficient(), out.GetStandardOxygenDiffusionCoefficient());
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::BloodChemistryConfigurationData& out)
  {
    // Blood Chemistry
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanCorpuscularHemoglobin)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanCorpuscularVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, StandardDiffusionDistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, StandardOxygenDiffusionCoefficient)
  }
  // class CardiovascularConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::CardiovascularConfigurationData& in, BioGearsConfiguration& out)
  {
    // Cardiovascular
    io::Property::UnMarshall(in.LeftHeartElastanceMaximum(), out.GetLeftHeartElastanceMaximum());
    io::Property::UnMarshall(in.LeftHeartElastanceMinimum(), out.GetLeftHeartElastanceMinimum());
    io::Property::UnMarshall(in.MinimumBloodVolumeFraction(), out.GetMinimumBloodVolumeFraction());
    io::Property::UnMarshall(in.RightHeartElastanceMaximum(), out.GetRightHeartElastanceMaximum());
    io::Property::UnMarshall(in.RightHeartElastanceMinimum(), out.GetRightHeartElastanceMinimum());
    io::Property::UnMarshall(in.StandardPulmonaryCapillaryCoverage(), out.GetStandardPulmonaryCapillaryCoverage());
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::CardiovascularConfigurationData& out)
  {
    // Cardiovascular
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftHeartElastanceMaximum)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftHeartElastanceMinimum)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MinimumBloodVolumeFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightHeartElastanceMaximum)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightHeartElastanceMinimum)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, StandardPulmonaryCapillaryCoverage)
  }
  // class CircuitConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::CircuitConfigurationData& in, BioGearsConfiguration& out)
  {
    // Circuit
    io::Property::UnMarshall(in.CardiovascularOpenResistance(), out.GetCardiovascularOpenResistance());
    io::Property::UnMarshall(in.DefaultOpenElectricResistance(), out.GetDefaultOpenElectricResistance());
    io::Property::UnMarshall(in.DefaultOpenFlowResistance(), out.GetDefaultOpenFlowResistance());
    io::Property::UnMarshall(in.DefaultOpenHeatResistance(), out.GetDefaultOpenHeatResistance());
    io::Property::UnMarshall(in.DefaultClosedElectricResistance(), out.GetDefaultClosedElectricResistance());
    io::Property::UnMarshall(in.DefaultClosedFlowResistance(), out.GetDefaultClosedFlowResistance());
    io::Property::UnMarshall(in.DefaultClosedHeatResistance(), out.GetDefaultClosedHeatResistance());
    io::Property::UnMarshall(in.MachineClosedResistance(), out.GetMachineClosedResistance());
    io::Property::UnMarshall(in.MachineOpenResistance(), out.GetMachineOpenResistance());
    io::Property::UnMarshall(in.RespiratoryClosedResistance(), out.GetRespiratoryClosedResistance());
    io::Property::UnMarshall(in.RespiratoryOpenResistance(), out.GetRespiratoryOpenResistance());
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::CircuitConfigurationData& out)
  {
    // Circuits
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CardiovascularOpenResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DefaultClosedElectricResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DefaultClosedFlowResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DefaultClosedHeatResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DefaultOpenElectricResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DefaultOpenFlowResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DefaultOpenHeatResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MachineClosedResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MachineOpenResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespiratoryClosedResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespiratoryOpenResistance)
  }
  // class ConstantCOnfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::ConstantsConfigurationData& in, BioGearsConfiguration& out)
  {
    // Constants
    io::Property::UnMarshall(in.OxygenMetabolicConstant(), out.GetOxygenMetabolicConstant());
    io::Property::UnMarshall(in.StefanBoltzmann(), out.GetStefanBoltzmann());
    io::Property::UnMarshall(in.UniversalGasConstant(), out.GetUniversalGasConstant());
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::ConstantsConfigurationData& out)
  {
    // Constants
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, OxygenMetabolicConstant)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, StefanBoltzmann)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UniversalGasConstant)
  }
  // class DrugsConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::DrugsConfigurationData& in, BioGearsConfiguration& out)
  {
    // Drugs
    if (in.PDModel().present())
      io::Property::UnMarshall(in.PDModel().get(), out.m_PDEnabled);
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::DrugsConfigurationData& out)
  {
    // Drugs
    if (in.HasUsePDModel())
      io::Property::Marshall(in.m_PDEnabled, out.PDModel());
  }
  // class EnergyConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::EnergyConfigurationData& in, BioGearsConfiguration& out)
  {
    // Energy
    io::Property::UnMarshall(in.BodySpecificHeat(), out.GetBodySpecificHeat());
    io::Property::UnMarshall(in.CoreTemperatureLow(), out.GetCoreTemperatureLow());
    io::Property::UnMarshall(in.CoreTemperatureHigh(), out.GetCoreTemperatureHigh());
    io::Property::UnMarshall(in.DeltaCoreTemperatureLow(), out.GetDeltaCoreTemperatureLow());
    io::Property::UnMarshall(in.EnergyPerATP(), out.GetEnergyPerATP());
    io::Property::UnMarshall(in.SweatHeatTransfer(), out.GetSweatHeatTransfer());
    io::Property::UnMarshall(in.VaporizationEnergy(), out.GetVaporizationEnergy());
    io::Property::UnMarshall(in.VaporSpecificHeat(), out.GetVaporSpecificHeat());
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::EnergyConfigurationData& out)
  {
    // Energy
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BodySpecificHeat)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CoreTemperatureLow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CoreTemperatureHigh)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DeltaCoreTemperatureLow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, EnergyPerATP)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SweatHeatTransfer)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, VaporSpecificHeat)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, VaporizationEnergy)
  }
  // class EnvironmentConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::EnvironmentConfigurationData& in, BioGearsConfiguration& out)
  {

    // Environment
    io::Property::UnMarshall(in.AirDensity(), out.GetAirDensity());
    io::Property::UnMarshall(in.AirSpecificHeat(), out.GetAirSpecificHeat());
    io::Property::UnMarshall(in.MolarMassOfDryAir(), out.GetMolarMassOfDryAir());
    io::Property::UnMarshall(in.MolarMassOfWaterVapor(), out.GetMolarMassOfWaterVapor());
    if (in.InitialEnvironmentalConditionsFile().present()) {
      if (!out.GetInitialEnvironmentalConditions().Load(in.InitialEnvironmentalConditionsFile().get())) {
        throw biogears::CommonDataModelException("Unable to load InitialEnvironmentalConditions file");
      }
    } else if (in.InitialEnvironmentalConditions().present()) {
      if (!out.GetInitialEnvironmentalConditions().Load(in.InitialEnvironmentalConditions().get())) {
        throw biogears::CommonDataModelException("Unable to load InitialEnvironmentalConditions");
      }
    }
    io::Property::UnMarshall(in.WaterDensity(), out.GetWaterDensity());
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::EnvironmentConfigurationData& out)
  {
    // Environment
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AirDensity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AirSpecificHeat)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MolarMassOfDryAir)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MolarMassOfWaterVapor)

    CDM_OPTIONAL_ENVIRONMENT_MARSHALL_HELPER(in, out, InitialEnvironmentalConditions)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, WaterDensity)
  }
  // class GastrointestinalConfiguratio
  void BiogearsEngineConfiguration::UnMarshall(const CDM::GastrointestinalConfigurationData& in, BioGearsConfiguration& out)
  {
    // Gastrointestinal
    io::Property::UnMarshall(in.CalciumAbsorptionFraction(), out.GetCalciumAbsorptionFraction());
    io::Property::UnMarshall(in.CalciumDigestionRate(), out.GetCalciumDigestionRate());
    io::Property::UnMarshall(in.CarbohydrateAbsorptionFraction(), out.GetCarbohydrateAbsorptionFraction());
    if (in.DefaultStomachContentsFile().present()) {
      if (!out.GetDefaultStomachContents().Load(in.DefaultStomachContentsFile().get())) {
        throw biogears::CommonDataModelException("Unable to load Standard Stomach Contents file");
      }
    } else if (in.DefaultStomachContents().present()) {
      if (!out.GetDefaultStomachContents().Load(in.DefaultStomachContents().get())) {
        throw biogears::CommonDataModelException("Unable to load Standard Stomach Contents");
      }
    }
    io::Property::UnMarshall(in.FatAbsorptionFraction(), out.GetFatAbsorptionFraction());
    io::Property::UnMarshall(in.ProteinToUreaFraction(), out.GetProteinToUreaFraction());
    io::Property::UnMarshall(in.WaterDigestionRate(), out.GetWaterDigestionRate());
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::GastrointestinalConfigurationData& out)
  {

    // Gastrointestinal
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CalciumAbsorptionFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CalciumDigestionRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CarbohydrateAbsorptionFraction)
    CDM_OPTIONAL_PATIENT_NUTRITION_MARSHALL_HELPER(in, out, DefaultStomachContents)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, FatAbsorptionFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ProteinToUreaFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, WaterDigestionRate)
  }
  // class BioGearsConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::NervousConfigurationData& in, BioGearsConfiguration& out)
  {

    // Nervous
    if (in.EnableCerebral().present())
      io::Property::UnMarshall(in.EnableCerebral().get(), out.m_CerebralEnabled);
    io::Property::UnMarshall(in.PupilDiameterBaseline(), out.GetPupilDiameterBaseline());
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::NervousConfigurationData& out)
  {
    // Nervous
    if (in.HasEnableCerebral())
      io::Property::Marshall(in.m_CerebralEnabled, out.EnableCerebral());
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PupilDiameterBaseline)
  }
  // class RenalConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::RenalConfigurationData& in, BioGearsConfiguration& out)
  {

    // Renal
    if (in.EnableRenal().present())
      io::Property::UnMarshall(in.EnableRenal().get(), out.m_RenalEnabled);

    io::Property::UnMarshall(in.PlasmaSodiumConcentrationSetPoint(), out.GetPlasmaSodiumConcentrationSetPoint());

    io::Property::UnMarshall(in.LeftGlomerularFluidPermeabilityBaseline(), out.GetLeftGlomerularFluidPermeabilityBaseline());
    io::Property::UnMarshall(in.LeftGlomerularFilteringSurfaceAreaBaseline(), out.GetLeftGlomerularFilteringSurfaceAreaBaseline());
    io::Property::UnMarshall(in.LeftTubularReabsorptionFluidPermeabilityBaseline(), out.GetLeftTubularReabsorptionFluidPermeabilityBaseline());
    io::Property::UnMarshall(in.LeftTubularReabsorptionFilteringSurfaceAreaBaseline(), out.GetLeftTubularReabsorptionFilteringSurfaceAreaBaseline());

    io::Property::UnMarshall(in.MaximumAfferentResistance(), out.GetMaximumAfferentResistance());
    io::Property::UnMarshall(in.MinimumAfferentResistance(), out.GetMinimumAfferentResistance());

    io::Property::UnMarshall(in.RightGlomerularFluidPermeabilityBaseline(), out.GetRightGlomerularFluidPermeabilityBaseline());
    io::Property::UnMarshall(in.RightGlomerularFilteringSurfaceAreaBaseline(), out.GetRightGlomerularFilteringSurfaceAreaBaseline());
    io::Property::UnMarshall(in.RightTubularReabsorptionFluidPermeabilityBaseline(), out.GetRightTubularReabsorptionFluidPermeabilityBaseline());
    io::Property::UnMarshall(in.RightTubularReabsorptionFilteringSurfaceAreaBaseline(), out.GetRightTubularReabsorptionFilteringSurfaceAreaBaseline());
    io::Property::UnMarshall(in.TargetSodiumDelivery(), out.GetTargetSodiumDelivery());
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::RenalConfigurationData& out)
  {
    // Renal
    if (in.HasEnableRenal())
      io::Property::Marshall(in.m_RenalEnabled, out.EnableRenal());
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PlasmaSodiumConcentrationSetPoint)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftGlomerularFilteringSurfaceAreaBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftGlomerularFluidPermeabilityBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftTubularReabsorptionFilteringSurfaceAreaBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftTubularReabsorptionFluidPermeabilityBaseline)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MaximumAfferentResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MinimumAfferentResistance)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightGlomerularFilteringSurfaceAreaBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightGlomerularFluidPermeabilityBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightTubularReabsorptionFilteringSurfaceAreaBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightTubularReabsorptionFluidPermeabilityBaseline)
  }
  // class RespiratoryConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::RespiratoryConfigurationData& in, BioGearsConfiguration& out)
  {

    // Respiratory
    io::Property::UnMarshall(in.PleuralComplianceSensitivity(), out.GetPleuralComplianceSensitivity());
    io::Property::UnMarshall(in.PulmonaryVentilationRateMaximum(), out.GetPulmonaryVentilationRateMaximum());
    io::Property::UnMarshall(in.VentilatoryOcclusionPressure(), out.GetVentilatoryOcclusionPressure());
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::RespiratoryConfigurationData& out)
  {
    // Respiratory
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PleuralComplianceSensitivity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryVentilationRateMaximum)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, VentilatoryOcclusionPressure)
  }
  // class TissueConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::TissueConfigurationData& in, BioGearsConfiguration& out)
  {

    // Tissue
    if (in.EnableTissue().present())
      io::Property::UnMarshall(in.EnableTissue().get(), out.m_TissueEnabled);
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::TissueConfigurationData& out)
  {
    // Tissue
    if (in.HasEnableTissue())
      io::Property::Marshall(in.m_TissueEnabled, out.EnableTissue());
  }
  // class BioGearsConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::BioGearsConfigurationData& in, BioGearsConfiguration& out)
  {
    io::EngineConfiguration::UnMarshall(in, out);

    io::BiogearsEngineConfiguration::UnMarshall(in.BloodChemistryConfiguration(), out);
    io::BiogearsEngineConfiguration::UnMarshall(in.CardiovascularConfiguration(), out);
    io::BiogearsEngineConfiguration::UnMarshall(in.CircuitConfiguration(), out);
    io::BiogearsEngineConfiguration::UnMarshall(in.ConstantsConfiguration(), out);
    io::BiogearsEngineConfiguration::UnMarshall(in.DrugsConfiguration(), out);
    io::BiogearsEngineConfiguration::UnMarshall(in.EnergyConfiguration(), out);
    io::BiogearsEngineConfiguration::UnMarshall(in.EnvironmentConfiguration(), out);
    io::BiogearsEngineConfiguration::UnMarshall(in.GastrointestinalConfiguration(), out);
    io::BiogearsEngineConfiguration::UnMarshall(in.NervousConfiguration(), out);
    io::BiogearsEngineConfiguration::UnMarshall(in.RenalConfiguration(), out);
    io::BiogearsEngineConfiguration::UnMarshall(in.RespiratoryConfiguration(), out);
    io::BiogearsEngineConfiguration::UnMarshall(in.TissueConfiguration(), out);
  }
  void BiogearsEngineConfiguration::BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::BioGearsConfigurationData& out)
  {
    io::EngineConfiguration::Marshall(in, out);

    io::BiogearsEngineConfiguration::Marshall(in, out.BloodChemistryConfiguration());
    io::BiogearsEngineConfiguration::Marshall(in, out.CardiovascularConfiguration());
    io::BiogearsEngineConfiguration::Marshall(in, out.CircuitConfiguration());
    io::BiogearsEngineConfiguration::Marshall(in, out.ConstantsConfiguration());
    io::BiogearsEngineConfiguration::Marshall(in, out.DrugsConfiguration());
    io::BiogearsEngineConfiguration::Marshall(in, out.EnergyConfiguration());
    io::BiogearsEngineConfiguration::Marshall(in, out.EnvironmentConfiguration());
    io::BiogearsEngineConfiguration::Marshall(in, out.GastrointestinalConfiguration());
    io::BiogearsEngineConfiguration::Marshall(in, out.NervousConfiguration());
    io::BiogearsEngineConfiguration::Marshall(in, out.RenalConfiguration());
    io::BiogearsEngineConfiguration::Marshall(in, out.RespiratoryConfiguration());
    io::BiogearsEngineConfiguration::Marshall(in, out.TissueConfiguration());
  }
}
}