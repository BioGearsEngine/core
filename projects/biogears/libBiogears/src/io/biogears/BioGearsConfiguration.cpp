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
    if (in.MeanCorpuscularVolume().present() || !out.m_Merge)
      io::Property::UnMarshall(in.MeanCorpuscularVolume(), out.GetMeanCorpuscularVolume());
    if (in.MeanCorpuscularHemoglobin().present() || !out.m_Merge)
      io::Property::UnMarshall(in.MeanCorpuscularHemoglobin(), out.GetMeanCorpuscularHemoglobin());
    if (in.StandardDiffusionDistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.StandardDiffusionDistance(), out.GetStandardDiffusionDistance());
    if (in.StandardOxygenDiffusionCoefficient().present() || !out.m_Merge)
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
    if (in.LeftHeartElastanceMaximum().present() || !out.m_Merge)
      io::Property::UnMarshall(in.LeftHeartElastanceMaximum(), out.GetLeftHeartElastanceMaximum());
    if (in.LeftHeartElastanceMinimum().present() || !out.m_Merge)
      io::Property::UnMarshall(in.LeftHeartElastanceMinimum(), out.GetLeftHeartElastanceMinimum());
    if (in.MinimumBloodVolumeFraction().present() || !out.m_Merge)
      io::Property::UnMarshall(in.MinimumBloodVolumeFraction(), out.GetMinimumBloodVolumeFraction());
    if (in.RightHeartElastanceMaximum().present() || !out.m_Merge)
      io::Property::UnMarshall(in.RightHeartElastanceMaximum(), out.GetRightHeartElastanceMaximum());
    if (in.RightHeartElastanceMinimum().present() || !out.m_Merge)
      io::Property::UnMarshall(in.RightHeartElastanceMinimum(), out.GetRightHeartElastanceMinimum());
    if (in.StandardPulmonaryCapillaryCoverage().present() || !out.m_Merge)
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
    if (in.CardiovascularOpenResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.CardiovascularOpenResistance(), out.GetCardiovascularOpenResistance());
    if (in.DefaultOpenElectricResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.DefaultOpenElectricResistance(), out.GetDefaultOpenElectricResistance());
    if (in.DefaultOpenFlowResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.DefaultOpenFlowResistance(), out.GetDefaultOpenFlowResistance());
    if (in.DefaultOpenHeatResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.DefaultOpenHeatResistance(), out.GetDefaultOpenHeatResistance());
    if (in.DefaultClosedElectricResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.DefaultClosedElectricResistance(), out.GetDefaultClosedElectricResistance());
    if (in.DefaultClosedFlowResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.DefaultClosedFlowResistance(), out.GetDefaultClosedFlowResistance());
    if (in.DefaultClosedHeatResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.DefaultClosedHeatResistance(), out.GetDefaultClosedHeatResistance());
    if (in.MachineClosedResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.MachineClosedResistance(), out.GetMachineClosedResistance());
    if (in.MachineOpenResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.MachineOpenResistance(), out.GetMachineOpenResistance());
    if (in.RespiratoryClosedResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.RespiratoryClosedResistance(), out.GetRespiratoryClosedResistance());
    if (in.RespiratoryOpenResistance().present() || !out.m_Merge)
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
    if (in.OxygenMetabolicConstant().present() || !out.m_Merge)
      io::Property::UnMarshall(in.OxygenMetabolicConstant(), out.GetOxygenMetabolicConstant());
    if (in.StefanBoltzmann().present() || !out.m_Merge)
      io::Property::UnMarshall(in.StefanBoltzmann(), out.GetStefanBoltzmann());
    if (in.UniversalGasConstant().present() || !out.m_Merge)
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
    if (in.PDModel().present() || !out.m_Merge)
      io::Property::UnMarshall(in.PDModel().get(), out.m_PDEnabled);
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::DrugsConfigurationData& out)
  {
    // Drugs
    if (in.m_PDEnabled != decltype(in.m_PDEnabled)::Invalid) {
      io::Property::Marshall(in.m_PDEnabled, out.PDModel());
    }
  }
  // class EnergyConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::EnergyConfigurationData& in, BioGearsConfiguration& out)
  {
    // Energy
    if (in.BodySpecificHeat().present() || !out.m_Merge)
      io::Property::UnMarshall(in.BodySpecificHeat(), out.GetBodySpecificHeat());
    if (in.CoreTemperatureLow().present() || !out.m_Merge)
      io::Property::UnMarshall(in.CoreTemperatureLow(), out.GetCoreTemperatureLow());
    if (in.CoreTemperatureHigh().present() || !out.m_Merge)
      io::Property::UnMarshall(in.CoreTemperatureHigh(), out.GetCoreTemperatureHigh());
    if (in.DeltaCoreTemperatureLow().present() || !out.m_Merge)
      io::Property::UnMarshall(in.DeltaCoreTemperatureLow(), out.GetDeltaCoreTemperatureLow());
    if (in.EnergyPerATP().present() || !out.m_Merge)
      io::Property::UnMarshall(in.EnergyPerATP(), out.GetEnergyPerATP());
    if (in.SweatHeatTransfer().present() || !out.m_Merge)
      io::Property::UnMarshall(in.SweatHeatTransfer(), out.GetSweatHeatTransfer());
    if (in.VaporizationEnergy().present() || !out.m_Merge)
      io::Property::UnMarshall(in.VaporizationEnergy(), out.GetVaporizationEnergy());
    if (in.VaporSpecificHeat().present() || !out.m_Merge)
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
    if (in.AirDensity().present() || !out.m_Merge)
      io::Property::UnMarshall(in.AirDensity(), out.GetAirDensity());
    if (in.AirSpecificHeat().present() || !out.m_Merge)
      io::Property::UnMarshall(in.AirSpecificHeat(), out.GetAirSpecificHeat());
    if (in.MolarMassOfDryAir().present() || !out.m_Merge)
      io::Property::UnMarshall(in.MolarMassOfDryAir(), out.GetMolarMassOfDryAir());
    if (in.MolarMassOfWaterVapor().present() || !out.m_Merge)
      io::Property::UnMarshall(in.MolarMassOfWaterVapor(), out.GetMolarMassOfWaterVapor());

    if (out.m_Merge == false) {
      out.GetInitialEnvironmentalConditions().Clear();
    }

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
    if (in.CalciumAbsorptionFraction().present() || !out.m_Merge)
      io::Property::UnMarshall(in.CalciumAbsorptionFraction(), out.GetCalciumAbsorptionFraction());
    if (in.CalciumDigestionRate().present() || !out.m_Merge)
      io::Property::UnMarshall(in.CalciumDigestionRate(), out.GetCalciumDigestionRate());
    if (in.CarbohydrateAbsorptionFraction().present() || !out.m_Merge)
      io::Property::UnMarshall(in.CarbohydrateAbsorptionFraction(), out.GetCarbohydrateAbsorptionFraction());

    if (out.m_Merge == false) {
      out.GetDefaultStomachContents().Clear();
    }

    if (in.DefaultStomachContentsFile().present()) {
      if (!out.GetDefaultStomachContents().Load(in.DefaultStomachContentsFile().get())) {
        throw biogears::CommonDataModelException("Unable to load Standard Stomach Contents file");
      }
    } else if (in.DefaultStomachContents().present()) {
      try {
        io::PatientNutrition::UnMarshall(in.DefaultStomachContents().get(), out.GetDefaultStomachContents());
      } catch (CommonDataModelException ex) {
        throw biogears::CommonDataModelException(std::string("Unable to load Standard Stomach Contents") + ex.what());
      }
    }

    if (in.FatAbsorptionFraction().present() || !out.m_Merge)
      io::Property::UnMarshall(in.FatAbsorptionFraction(), out.GetFatAbsorptionFraction());
    if (in.ProteinToUreaFraction().present() || !out.m_Merge)
      io::Property::UnMarshall(in.ProteinToUreaFraction(), out.GetProteinToUreaFraction());
    if (in.WaterDigestionRate().present() || !out.m_Merge)
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
    if (in.EnableCerebral().present() || !out.m_Merge)
      io::Property::UnMarshall(in.EnableCerebral().get(), out.m_CerebralEnabled);
    if (in.PupilDiameterBaseline().present() || !out.m_Merge)
      io::Property::UnMarshall(in.PupilDiameterBaseline(), out.GetPupilDiameterBaseline());
  }
  void BiogearsEngineConfiguration::Marshall(const BioGearsConfiguration& in, CDM::NervousConfigurationData& out)
  {
    // Nervous
    if (in.m_CerebralEnabled != decltype(in.m_CerebralEnabled)::Invalid) {
      io::Property::Marshall(in.m_CerebralEnabled, out.EnableCerebral());
    }
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PupilDiameterBaseline)
  }
  // class RenalConfiguration
  void BiogearsEngineConfiguration::UnMarshall(const CDM::RenalConfigurationData& in, BioGearsConfiguration& out)
  {

    // Renal
    if (in.EnableRenal().present() || !out.m_Merge)
      io::Property::UnMarshall(in.EnableRenal(), out.m_RenalEnabled);
    if (in.PlasmaSodiumConcentrationSetPoint().present() || !out.m_Merge)
      io::Property::UnMarshall(in.PlasmaSodiumConcentrationSetPoint(), out.GetPlasmaSodiumConcentrationSetPoint());
    if (in.LeftGlomerularFluidPermeabilityBaseline().present() || !out.m_Merge)
      io::Property::UnMarshall(in.LeftGlomerularFluidPermeabilityBaseline(), out.GetLeftGlomerularFluidPermeabilityBaseline());
    if (in.LeftGlomerularFilteringSurfaceAreaBaseline().present() || !out.m_Merge)
      io::Property::UnMarshall(in.LeftGlomerularFilteringSurfaceAreaBaseline(), out.GetLeftGlomerularFilteringSurfaceAreaBaseline());
    if (in.LeftTubularReabsorptionFluidPermeabilityBaseline().present() || !out.m_Merge)
      io::Property::UnMarshall(in.LeftTubularReabsorptionFluidPermeabilityBaseline(), out.GetLeftTubularReabsorptionFluidPermeabilityBaseline());
    if (in.LeftTubularReabsorptionFilteringSurfaceAreaBaseline().present() || !out.m_Merge)
      io::Property::UnMarshall(in.LeftTubularReabsorptionFilteringSurfaceAreaBaseline(), out.GetLeftTubularReabsorptionFilteringSurfaceAreaBaseline());

    if (in.MaximumAfferentResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.MaximumAfferentResistance(), out.GetMaximumAfferentResistance());
    if (in.MinimumAfferentResistance().present() || !out.m_Merge)
      io::Property::UnMarshall(in.MinimumAfferentResistance(), out.GetMinimumAfferentResistance());

    if (in.RightGlomerularFluidPermeabilityBaseline().present() || !out.m_Merge)
      io::Property::UnMarshall(in.RightGlomerularFluidPermeabilityBaseline(), out.GetRightGlomerularFluidPermeabilityBaseline());
    if (in.RightGlomerularFilteringSurfaceAreaBaseline().present() || !out.m_Merge)
      io::Property::UnMarshall(in.RightGlomerularFilteringSurfaceAreaBaseline(), out.GetRightGlomerularFilteringSurfaceAreaBaseline());
    if (in.RightTubularReabsorptionFluidPermeabilityBaseline().present() || !out.m_Merge)
      io::Property::UnMarshall(in.RightTubularReabsorptionFluidPermeabilityBaseline(), out.GetRightTubularReabsorptionFluidPermeabilityBaseline());
    if (in.RightTubularReabsorptionFilteringSurfaceAreaBaseline().present() || !out.m_Merge)
      io::Property::UnMarshall(in.RightTubularReabsorptionFilteringSurfaceAreaBaseline(), out.GetRightTubularReabsorptionFilteringSurfaceAreaBaseline());
    if (in.TargetSodiumDelivery().present() || !out.m_Merge)
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
    if (in.PleuralComplianceSensitivity().present() || !out.m_Merge)
      io::Property::UnMarshall(in.PleuralComplianceSensitivity(), out.GetPleuralComplianceSensitivity());
    if (in.PulmonaryVentilationRateMaximum().present() || !out.m_Merge)
      io::Property::UnMarshall(in.PulmonaryVentilationRateMaximum(), out.GetPulmonaryVentilationRateMaximum());
    if (in.VentilatoryOcclusionPressure().present() || !out.m_Merge)
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
    if (in.EnableTissue().present() || !out.m_Merge)
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