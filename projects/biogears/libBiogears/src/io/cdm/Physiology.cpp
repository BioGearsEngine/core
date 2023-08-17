#include "Physiology.h"

#include "PatientNutrition.h"
#include "Property.h"
#include "System.h"

#include <biogears/cdm/properties/SEProperties.h>

#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SEEndocrineSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>
#include <biogears/cdm/system/physiology/SEHepaticSystem.h>
#include <biogears/cdm/system/physiology/SENervousSystem.h>
#include <biogears/cdm/system/physiology/SERenalSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/system/physiology/SETissueSystem.h>

namespace biogears {
namespace io {
  // class SEPupillaryResponse
  void Physiology::Marshall(const CDM::PupillaryResponseData& in, SEPupillaryResponse& out)
  {
    io::Property::Marshall(in.ReactivityModifier(), out.GetReactivityModifier());
    io::Property::Marshall(in.ShapeModifier(), out.GetShapeModifier());
    io::Property::Marshall(in.SizeModifier(), out.GetSizeModifier());
  }
  void Physiology::UnMarshall(const SEPupillaryResponse& in, CDM::PupillaryResponseData& out)
  {
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ReactivityModifier)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ShapeModifier)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SizeModifier)
  }
  //----------------------------------------------------------------------------------
  // class SEInflammationState
  void Physiology::Marshall(const CDM::InflammatoryResponseData& in, SEInflammatoryResponse& out)
  {
    out.SetActiveTLR(in.ActiveTLR());

    io::Property::Marshall(in.LocalPathogen(), out.GetLocalPathogen());
    io::Property::Marshall(in.LocalMacrophage(), out.GetLocalMacrophage());
    io::Property::Marshall(in.LocalNeutrophil(), out.GetLocalNeutrophil());
    io::Property::Marshall(in.LocalBarrier(), out.GetLocalBarrier());
    io::Property::Marshall(in.AutonomicResponseLevel(), out.GetAutonomicResponseLevel());
    io::Property::Marshall(in.Catecholamines(), out.GetCatecholamines());
    io::Property::Marshall(in.ConstitutiveNOS(), out.GetConstitutiveNOS());
    io::Property::Marshall(in.InducibleNOSPre(), out.GetInducibleNOSPre());
    io::Property::Marshall(in.InducibleNOS(), out.GetInducibleNOS());
    io::Property::Marshall(in.Interleukin6(), out.GetInterleukin6());
    io::Property::Marshall(in.Interleukin10(), out.GetInterleukin10());
    io::Property::Marshall(in.Interleukin12(), out.GetInterleukin12());
    io::Property::Marshall(in.MacrophageResting(), out.GetMacrophageResting());
    io::Property::Marshall(in.MacrophageActive(), out.GetMacrophageActive());
    io::Property::Marshall(in.NeutrophilResting(), out.GetNeutrophilResting());
    io::Property::Marshall(in.NeutrophilActive(), out.GetNeutrophilActive());
    io::Property::Marshall(in.Nitrate(), out.GetNitrate());
    io::Property::Marshall(in.NitricOxide(), out.GetNitricOxide());
    io::Property::Marshall(in.BloodPathogen(), out.GetBloodPathogen());
    io::Property::Marshall(in.TissueIntegrity(), out.GetTissueIntegrity());
    io::Property::Marshall(in.Trauma(), out.GetTrauma());
    io::Property::Marshall(in.TumorNecrosisFactor(), out.GetTumorNecrosisFactor());
    io::Property::Marshall(in.InflammationTime(), out.GetInflammationTime());
    for (auto src : in.Source()) {
      out.m_InflammationSources.push_back(src);
    }
  }
  void Physiology::UnMarshall(const SEInflammatoryResponse& in, CDM::InflammatoryResponseData& out)
  {
    CDM_ENUM_UNMARSHAL_HELPER(in, out, ActiveTLR);

    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, LocalPathogen)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, LocalMacrophage)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, LocalNeutrophil)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, LocalBarrier)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, AutonomicResponseLevel)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Catecholamines)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, ConstitutiveNOS)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, InducibleNOSPre)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, InducibleNOS)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Interleukin6)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Interleukin10)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Interleukin12)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, MacrophageResting)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, MacrophageActive)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, NeutrophilResting)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, NeutrophilActive)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Nitrate)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, NitricOxide)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, BloodPathogen)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, TissueIntegrity)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Trauma)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, TumorNecrosisFactor)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, InflammationTime)
    for (auto src : in.m_InflammationSources) {
      out.Source().push_back(src);
    }
  }
  //----------------------------------------------------------------------------------
  // class SEBloodChemistrySystem
  void Physiology::Marshall(const CDM::BloodChemistrySystemData& in, SEBloodChemistrySystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));

    io::Property::Marshall(in.BloodDensity(), out.GetBloodDensity());
    io::Property::Marshall(in.ArterialBloodPH(), out.GetArterialBloodPH());
    io::Property::Marshall(in.ArterialBloodPHBaseline(), out.GetArterialBloodPHBaseline());
    io::Property::Marshall(in.VenousBloodPH(), out.GetVenousBloodPH());
    io::Property::Marshall(in.BloodSpecificHeat(), out.GetBloodSpecificHeat());
    io::Property::Marshall(in.BloodUreaNitrogenConcentration(), out.GetBloodUreaNitrogenConcentration());
    io::Property::Marshall(in.CarbonDioxideSaturation(), out.GetCarbonDioxideSaturation());
    io::Property::Marshall(in.CarbonMonoxideSaturation(), out.GetCarbonMonoxideSaturation());
    io::Property::Marshall(in.Hematocrit(), out.GetHematocrit());
    io::Property::Marshall(in.HemoglobinContent(), out.GetHemoglobinContent());
    io::Property::Marshall(in.HemoglobinLostToUrine(), out.GetHemoglobinLostToUrine());
    io::Property::Marshall(in.LymphocyteCellCount(), out.GetLymphocyteCellCount());
    io::Property::Marshall(in.NeutrophilCellCount(), out.GetNeutrophilCellCount());
    io::Property::Marshall(in.OxygenSaturation(), out.GetOxygenSaturation());
    io::Property::Marshall(in.OxygenVenousSaturation(), out.GetOxygenVenousSaturation());
    io::Property::Marshall(in.Phosphate(), out.GetPhosphate());
    io::Property::Marshall(in.PlasmaVolume(), out.GetPlasmaVolume());
    io::Property::Marshall(in.PulseOximetry(), out.GetPulseOximetry());
    io::Property::Marshall(in.RedBloodCellAcetylcholinesterase(), out.GetRedBloodCellAcetylcholinesterase());
    io::Property::Marshall(in.RedBloodCellCount(), out.GetRedBloodCellCount());
    io::Property::Marshall(in.ShuntFraction(), out.GetShuntFraction());
    io::Property::Marshall(in.StrongIonDifference(), out.GetStrongIonDifference());
    io::Property::Marshall(in.TotalBilirubin(), out.GetTotalBilirubin());
    io::Property::Marshall(in.TotalProteinConcentration(), out.GetTotalProteinConcentration());
    io::Property::Marshall(in.RhTransfusionReactionVolume(), out.GetRhTransfusionReactionVolume());
    io::Property::Marshall(in.ViralLoad(), out.GetViralLoad());
    io::Property::Marshall(in.VolumeFractionNeutralPhospholipidInPlasma(), out.GetVolumeFractionNeutralPhospholipidInPlasma());
    io::Property::Marshall(in.VolumeFractionNeutralLipidInPlasma(), out.GetVolumeFractionNeutralLipidInPlasma());
    io::Property::Marshall(in.WhiteBloodCellCount(), out.GetWhiteBloodCellCount());

    io::Property::Marshall(in.ArterialCarbonDioxidePressure(), out.GetArterialCarbonDioxidePressure());
    io::Property::Marshall(in.ArterialOxygenPressure(), out.GetArterialOxygenPressure());
    io::Property::Marshall(in.PulmonaryArterialCarbonDioxidePressure(), out.GetPulmonaryArterialCarbonDioxidePressure());
    io::Property::Marshall(in.PulmonaryArterialOxygenPressure(), out.GetPulmonaryArterialOxygenPressure());
    io::Property::Marshall(in.PulmonaryVenousCarbonDioxidePressure(), out.GetPulmonaryVenousCarbonDioxidePressure());
    io::Property::Marshall(in.PulmonaryVenousOxygenPressure(), out.GetPulmonaryVenousOxygenPressure());
    io::Property::Marshall(in.VenousCarbonDioxidePressure(), out.GetVenousCarbonDioxidePressure());
    io::Property::Marshall(in.VenousOxygenPressure(), out.GetVenousOxygenPressure());

    Marshall(in.InflammatoryResponse(), out.GetInflammatoryResponse());
  }
  void Physiology::UnMarshall(const SEBloodChemistrySystem& in, CDM::BloodChemistrySystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BloodDensity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ArterialBloodPH)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ArterialBloodPHBaseline)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, VenousBloodPH)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BloodSpecificHeat)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BloodUreaNitrogenConcentration)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CarbonDioxideSaturation)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CarbonMonoxideSaturation)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Hematocrit)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HemoglobinContent)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HemoglobinLostToUrine)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LymphocyteCellCount)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, NeutrophilCellCount)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, OxygenSaturation)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, OxygenVenousSaturation)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Phosphate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PlasmaVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulseOximetry)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RedBloodCellAcetylcholinesterase)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RedBloodCellCount)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ShuntFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, StrongIonDifference)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TotalBilirubin)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TotalProteinConcentration)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RhTransfusionReactionVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ViralLoad)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, VolumeFractionNeutralPhospholipidInPlasma)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, VolumeFractionNeutralLipidInPlasma)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, WhiteBloodCellCount)

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ArterialCarbonDioxidePressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ArterialOxygenPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryArterialCarbonDioxidePressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryArterialOxygenPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryVenousCarbonDioxidePressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryVenousOxygenPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, VenousCarbonDioxidePressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, VenousOxygenPressure)

    UnMarshall(*in.m_InflammatoryResponse, out.InflammatoryResponse());
  }
  //----------------------------------------------------------------------------------
  // class SECardiovascularSystem
  void Physiology::Marshall(const CDM::CardiovascularSystemData& in, SECardiovascularSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    if (in.HeartRhythm().present()) {
      out.SetHeartRhythm(in.HeartRhythm().get());
    }

    io::Property::Marshall(in.ArterialPressure(), out.GetArterialPressure());
    io::Property::Marshall(in.BloodVolume(), out.GetBloodVolume());
    io::Property::Marshall(in.CardiacIndex(), out.GetCardiacIndex());
    io::Property::Marshall(in.CardiacOutput(), out.GetCardiacOutput());
    io::Property::Marshall(in.CentralVenousPressure(), out.GetCentralVenousPressure());
    io::Property::Marshall(in.CerebralBloodFlow(), out.GetCerebralBloodFlow());
    io::Property::Marshall(in.CerebralPerfusionPressure(), out.GetCerebralPerfusionPressure());
    io::Property::Marshall(in.DiastolicArterialPressure(), out.GetDiastolicArterialPressure());
    io::Property::Marshall(in.ExtremityPressureLeftArm(), out.GetExtremityPressureLeftArm());
    io::Property::Marshall(in.ExtremityPressureLeftLeg(), out.GetExtremityPressureLeftLeg());
    io::Property::Marshall(in.ExtremityPressureRightArm(), out.GetExtremityPressureRightArm());
    io::Property::Marshall(in.ExtremityPressureRightLeg(), out.GetExtremityPressureRightLeg());
    io::Property::Marshall(in.HeartEjectionFraction(), out.GetHeartEjectionFraction());
    io::Property::Marshall(in.HeartRate(), out.GetHeartRate());

    io::Property::Marshall(in.HeartStrokeVolume(), out.GetHeartStrokeVolume());
    io::Property::Marshall(in.IntracranialPressure(), out.GetIntracranialPressure());
    io::Property::Marshall(in.MeanArterialPressure(), out.GetMeanArterialPressure());
    io::Property::Marshall(in.MeanArterialCarbonDioxidePartialPressure(), out.GetMeanArterialCarbonDioxidePartialPressure());
    io::Property::Marshall(in.MeanArterialCarbonDioxidePartialPressureDelta(), out.GetMeanArterialCarbonDioxidePartialPressureDelta());
    io::Property::Marshall(in.MeanCentralVenousPressure(), out.GetMeanCentralVenousPressure());
    io::Property::Marshall(in.MeanSkinFlow(), out.GetMeanSkinFlow());
    io::Property::Marshall(in.PulmonaryArterialPressure(), out.GetPulmonaryArterialPressure());
    io::Property::Marshall(in.PulmonaryCapillariesWedgePressure(), out.GetPulmonaryCapillariesWedgePressure());
    io::Property::Marshall(in.PulmonaryDiastolicArterialPressure(), out.GetPulmonaryDiastolicArterialPressure());
    io::Property::Marshall(in.PulmonaryMeanArterialPressure(), out.GetPulmonaryMeanArterialPressure());
    io::Property::Marshall(in.PulmonaryMeanCapillaryFlow(), out.GetPulmonaryMeanCapillaryFlow());
    io::Property::Marshall(in.PulmonaryMeanShuntFlow(), out.GetPulmonaryMeanShuntFlow());
    io::Property::Marshall(in.PulmonarySystolicArterialPressure(), out.GetPulmonarySystolicArterialPressure());
    io::Property::Marshall(in.PulmonaryVascularResistance(), out.GetPulmonaryVascularResistance());
    io::Property::Marshall(in.PulmonaryVascularResistanceIndex(), out.GetPulmonaryVascularResistanceIndex());
    io::Property::Marshall(in.PulsePressure(), out.GetPulsePressure());
    io::Property::Marshall(in.SystolicArterialPressure(), out.GetSystolicArterialPressure());
    io::Property::Marshall(in.SystemicVascularResistance(), out.GetSystemicVascularResistance());
  }
  void Physiology::UnMarshall(const SECardiovascularSystem& in, CDM::CardiovascularSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    if (in.HasHeartRhythm()) {
      out.HeartRhythm(in.m_HeartRhythm);
    }

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ArterialPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BloodVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CardiacIndex)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CardiacOutput)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CentralVenousPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CerebralBloodFlow)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CerebralPerfusionPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, DiastolicArterialPressure)

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ExtremityPressureLeftArm)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ExtremityPressureLeftLeg)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ExtremityPressureRightArm)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ExtremityPressureRightLeg)

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HeartEjectionFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HeartRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HeartStrokeVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, IntracranialPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanArterialPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanArterialCarbonDioxidePartialPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanArterialCarbonDioxidePartialPressureDelta)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanCentralVenousPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanSkinFlow)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryArterialPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryCapillariesWedgePressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryDiastolicArterialPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryMeanArterialPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryMeanCapillaryFlow)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryMeanShuntFlow)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonarySystolicArterialPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryVascularResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryVascularResistanceIndex)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulsePressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SystolicArterialPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SystemicVascularResistance)
  }
  //----------------------------------------------------------------------------------
  // class SEDrugSystem
  void Physiology::Marshall(const CDM::DrugSystemData& in, SEDrugSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.AntibioticActivity(), out.GetAntibioticActivity());
    io::Property::Marshall(in.BronchodilationLevel(), out.GetBronchodilationLevel());
    io::Property::Marshall(in.FeverChange(), out.GetFeverChange());
    io::Property::Marshall(in.HeartRateChange(), out.GetHeartRateChange());
    io::Property::Marshall(in.HemorrhageChange(), out.GetHemorrhageChange());
    io::Property::Marshall(in.MeanBloodPressureChange(), out.GetMeanBloodPressureChange());
    io::Property::Marshall(in.NeuromuscularBlockLevel(), out.GetNeuromuscularBlockLevel());
    io::Property::Marshall(in.PainToleranceChange(), out.GetPainToleranceChange());
    io::Property::Marshall(in.PulsePressureChange(), out.GetPulsePressureChange());
    io::Property::Marshall(in.RespirationRateChange(), out.GetRespirationRateChange());
    io::Property::Marshall(in.SedationLevel(), out.GetSedationLevel());
    io::Property::Marshall(in.TidalVolumeChange(), out.GetTidalVolumeChange());
    io::Property::Marshall(in.TubularPermeabilityChange(), out.GetTubularPermeabilityChange());
    io::Property::Marshall(in.CentralNervousResponse(), out.GetCentralNervousResponse());
  }
  void Physiology::UnMarshall(const SEDrugSystem& in, CDM::DrugSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AntibioticActivity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BronchodilationLevel)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, FeverChange)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HeartRateChange)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HemorrhageChange)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanBloodPressureChange)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, NeuromuscularBlockLevel)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PainToleranceChange)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulsePressureChange)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespirationRateChange)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SedationLevel)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TidalVolumeChange)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TubularPermeabilityChange)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CentralNervousResponse)
  }
  //----------------------------------------------------------------------------------
  // class SEEndocrineSystem
  void Physiology::Marshall(const CDM::EndocrineSystemData& in, SEEndocrineSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.InsulinSynthesisRate(), out.GetInsulinSynthesisRate());
    io::Property::Marshall(in.GlucagonSynthesisRate(), out.GetGlucagonSynthesisRate());
  }
  void Physiology::UnMarshall(const SEEndocrineSystem& in, CDM::EndocrineSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, InsulinSynthesisRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, GlucagonSynthesisRate)
  }
  //----------------------------------------------------------------------------------
  // class SEEnergySystem
  void Physiology::Marshall(const CDM::EnergySystemData& in, SEEnergySystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.AchievedExerciseLevel(), out.GetAchievedExerciseLevel());
    io::Property::Marshall(in.ChlorideLostToSweat(), out.GetChlorideLostToSweat());
    io::Property::Marshall(in.CoreTemperature(), out.GetCoreTemperature());
    io::Property::Marshall(in.CreatinineProductionRate(), out.GetCreatinineProductionRate());
    io::Property::Marshall(in.EnergyDeficit(), out.GetEnergyDeficit());
    io::Property::Marshall(in.ExerciseEnergyDemand(), out.GetExerciseEnergyDemand());
    io::Property::Marshall(in.ExerciseMeanArterialPressureDelta(), out.GetExerciseMeanArterialPressureDelta());
    io::Property::Marshall(in.FatigueLevel(), out.GetFatigueLevel());
    io::Property::Marshall(in.LactateProductionRate(), out.GetLactateProductionRate());
    io::Property::Marshall(in.PotassiumLostToSweat(), out.GetPotassiumLostToSweat());
    io::Property::Marshall(in.SkinTemperatureTorso(), out.GetSkinTemperatureTorso());
    io::Property::Marshall(in.SkinTemperatureHead(), out.GetSkinTemperatureHead());
    io::Property::Marshall(in.SkinTemperatureLeftArm(), out.GetSkinTemperatureLeftArm());
    io::Property::Marshall(in.SkinTemperatureRightArm(), out.GetSkinTemperatureRightArm());
    io::Property::Marshall(in.SkinTemperatureLeftLeg(), out.GetSkinTemperatureLeftLeg());
    io::Property::Marshall(in.SkinTemperatureRightLeg(), out.GetSkinTemperatureRightLeg());
    io::Property::Marshall(in.SodiumLostToSweat(), out.GetSodiumLostToSweat());
    io::Property::Marshall(in.SweatRate(), out.GetSweatRate());
    io::Property::Marshall(in.TotalMetabolicRate(), out.GetTotalMetabolicRate());
    io::Property::Marshall(in.TotalWorkRateLevel(), out.GetTotalWorkRateLevel());
  }
  void Physiology::UnMarshall(const SEEnergySystem& in, CDM::EnergySystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AchievedExerciseLevel)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ChlorideLostToSweat)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CoreTemperature)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CreatinineProductionRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, EnergyDeficit)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ExerciseEnergyDemand)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ExerciseMeanArterialPressureDelta)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, FatigueLevel)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LactateProductionRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PotassiumLostToSweat)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SkinTemperatureTorso)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SkinTemperatureHead)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SkinTemperatureLeftArm)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SkinTemperatureRightArm)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SkinTemperatureLeftLeg)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SkinTemperatureRightLeg)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SodiumLostToSweat)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SweatRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TotalMetabolicRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TotalWorkRateLevel)
  }
  //----------------------------------------------------------------------------------
  // class SEGastrointestinalSystem
  void Physiology::Marshall(const CDM::GastrointestinalSystemData& in, SEGastrointestinalSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    if (in.ChymeAbsorptionRate().present()) {
      io::Property::Marshall(in.ChymeAbsorptionRate(), out.GetChymeAbsorptionRate());
    }
    if (in.StomachContents().present()) {
      out.GetStomachContents().Load(in.StomachContents().get());
    }
  }
  void Physiology::UnMarshall(const SEGastrointestinalSystem& in, CDM::GastrointestinalSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ChymeAbsorptionRate)
    CDM_OPTIONAL_PATIENT_NUTRITION_UNMARSHAL_HELPER(in, out, StomachContents)
  }
  //----------------------------------------------------------------------------------
  // class SEHepaticSystem
  void Physiology::Marshall(const CDM::HepaticSystemData& in, SEHepaticSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.KetoneProductionRate(), out.GetKetoneProductionRate());
    io::Property::Marshall(in.HepaticGluconeogenesisRate(), out.GetHepaticGluconeogenesisRate());
  }
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SEHepaticSystem& in, CDM::HepaticSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, KetoneProductionRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HepaticGluconeogenesisRate)
  }
  //----------------------------------------------------------------------------------
  // class SENervousSystem
  void Physiology::Marshall(const CDM::NervousSystemData& in, SENervousSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    out.SetSleepState(in.SleepState().get());

    io::Property::Marshall(in.ComplianceScale(), out.GetComplianceScale());
    io::Property::Marshall(in.HeartRateScale(), out.GetHeartRateScale());
    io::Property::Marshall(in.HeartElastanceScale(), out.GetHeartElastanceScale());
    io::Property::Marshall(in.MentalStatus(), out.GetMentalStatus());

    io::Property::Marshall(in.PainVisualAnalogueScale(), out.GetPainVisualAnalogueScale());
    io::Property::Marshall(in.ResistanceScaleExtrasplanchnic(), out.GetResistanceScaleExtrasplanchnic());
    io::Property::Marshall(in.ResistanceScaleMuscle(), out.GetResistanceScaleMuscle());
    io::Property::Marshall(in.ResistanceScaleMyocardium(), out.GetResistanceScaleMyocardium());
    io::Property::Marshall(in.ResistanceScaleSplanchnic(), out.GetResistanceScaleSplanchnic());
    io::Property::Marshall(in.SleepTime(), out.GetSleepTime());
    io::Property::Marshall(in.WakeTime(), out.GetWakeTime());

    io::Property::Marshall(in.BiologicalDebt(), out.GetBiologicalDebt());
    io::Property::Marshall(in.ReactionTime(), out.GetReactionTime());
    io::Property::Marshall(in.AttentionLapses(), out.GetAttentionLapses());

    io::Property::Marshall(in.RichmondAgitationSedationScale(), out.GetRichmondAgitationSedationScale());

    Marshall(in.LeftEyePupillaryResponse(), out.GetLeftEyePupillaryResponse());
    Marshall(in.RightEyePupillaryResponse(), out.GetRightEyePupillaryResponse());
  }
  void Physiology::UnMarshall(const SENervousSystem& in, CDM::NervousSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    out.SleepState(in.m_SleepState);

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ComplianceScale)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HeartRateScale)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HeartElastanceScale)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MentalStatus)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PainVisualAnalogueScale)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ResistanceScaleExtrasplanchnic)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ResistanceScaleMuscle)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ResistanceScaleMyocardium)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ResistanceScaleSplanchnic)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SleepTime)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, WakeTime)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BiologicalDebt)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ReactionTime)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AttentionLapses)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RichmondAgitationSedationScale)

    UnMarshall(*in.m_LeftEyePupillaryResponse, out.LeftEyePupillaryResponse());
    UnMarshall(*in.m_RightEyePupillaryResponse, out.RightEyePupillaryResponse());
  }
  //----------------------------------------------------------------------------------
  // class SERenalSystem
  void Physiology::Marshall(const CDM::RenalSystemData& in, SERenalSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.BladderPressure(), out.GetBladderPressure());
    io::Property::Marshall(in.GlomerularFiltrationRate(), out.GetGlomerularFiltrationRate());
    io::Property::Marshall(in.FiltrationFraction(), out.GetFiltrationFraction());

    io::Property::Marshall(in.LeftAfferentArterioleResistance(), out.GetLeftAfferentArterioleResistance());
    io::Property::Marshall(in.LeftBowmansCapsulesHydrostaticPressure(), out.GetLeftBowmansCapsulesHydrostaticPressure());
    io::Property::Marshall(in.LeftBowmansCapsulesOsmoticPressure(), out.GetLeftBowmansCapsulesOsmoticPressure());
    io::Property::Marshall(in.LeftEfferentArterioleResistance(), out.GetLeftEfferentArterioleResistance());
    io::Property::Marshall(in.LeftGlomerularCapillariesHydrostaticPressure(), out.GetLeftGlomerularCapillariesHydrostaticPressure());
    io::Property::Marshall(in.LeftGlomerularCapillariesOsmoticPressure(), out.GetLeftGlomerularCapillariesOsmoticPressure());
    io::Property::Marshall(in.LeftGlomerularFiltrationCoefficient(), out.GetLeftGlomerularFiltrationCoefficient());
    io::Property::Marshall(in.LeftGlomerularFiltrationRate(), out.GetLeftGlomerularFiltrationRate());
    io::Property::Marshall(in.LeftGlomerularFiltrationSurfaceArea(), out.GetLeftGlomerularFiltrationSurfaceArea());
    io::Property::Marshall(in.LeftGlomerularFluidPermeability(), out.GetLeftGlomerularFluidPermeability());
    io::Property::Marshall(in.LeftFiltrationFraction(), out.GetLeftFiltrationFraction());
    io::Property::Marshall(in.LeftNetFiltrationPressure(), out.GetLeftNetFiltrationPressure());
    io::Property::Marshall(in.LeftNetReabsorptionPressure(), out.GetLeftNetReabsorptionPressure());
    io::Property::Marshall(in.LeftPeritubularCapillariesHydrostaticPressure(), out.GetLeftPeritubularCapillariesHydrostaticPressure());
    io::Property::Marshall(in.LeftPeritubularCapillariesOsmoticPressure(), out.GetLeftPeritubularCapillariesOsmoticPressure());
    io::Property::Marshall(in.LeftReabsorptionFiltrationCoefficient(), out.GetLeftReabsorptionFiltrationCoefficient());
    io::Property::Marshall(in.LeftReabsorptionRate(), out.GetLeftReabsorptionRate());
    io::Property::Marshall(in.LeftTubularReabsorptionFiltrationSurfaceArea(), out.GetLeftTubularReabsorptionFiltrationSurfaceArea());
    io::Property::Marshall(in.LeftTubularReabsorptionFluidPermeability(), out.GetLeftTubularReabsorptionFluidPermeability());
    io::Property::Marshall(in.LeftTubularHydrostaticPressure(), out.GetLeftTubularHydrostaticPressure());
    io::Property::Marshall(in.LeftTubularOsmoticPressure(), out.GetLeftTubularOsmoticPressure());

    io::Property::Marshall(in.RenalBloodFlow(), out.GetRenalBloodFlow());
    io::Property::Marshall(in.RenalPlasmaFlow(), out.GetRenalPlasmaFlow());
    io::Property::Marshall(in.RenalVascularResistance(), out.GetRenalVascularResistance());

    io::Property::Marshall(in.RightAfferentArterioleResistance(), out.GetRightAfferentArterioleResistance());
    io::Property::Marshall(in.RightBowmansCapsulesHydrostaticPressure(), out.GetRightBowmansCapsulesHydrostaticPressure());
    io::Property::Marshall(in.RightBowmansCapsulesOsmoticPressure(), out.GetRightBowmansCapsulesOsmoticPressure());
    io::Property::Marshall(in.RightEfferentArterioleResistance(), out.GetRightEfferentArterioleResistance());
    io::Property::Marshall(in.RightGlomerularCapillariesHydrostaticPressure(), out.GetRightGlomerularCapillariesHydrostaticPressure());
    io::Property::Marshall(in.RightGlomerularCapillariesOsmoticPressure(), out.GetRightGlomerularCapillariesOsmoticPressure());
    io::Property::Marshall(in.RightGlomerularFiltrationCoefficient(), out.GetRightGlomerularFiltrationCoefficient());
    io::Property::Marshall(in.RightGlomerularFiltrationRate(), out.GetRightGlomerularFiltrationRate());
    io::Property::Marshall(in.RightGlomerularFiltrationSurfaceArea(), out.GetRightGlomerularFiltrationSurfaceArea());
    io::Property::Marshall(in.RightGlomerularFluidPermeability(), out.GetRightGlomerularFluidPermeability());
    io::Property::Marshall(in.RightFiltrationFraction(), out.GetRightFiltrationFraction());
    io::Property::Marshall(in.RightNetFiltrationPressure(), out.GetRightNetFiltrationPressure());
    io::Property::Marshall(in.RightNetReabsorptionPressure(), out.GetRightNetReabsorptionPressure());
    io::Property::Marshall(in.RightPeritubularCapillariesHydrostaticPressure(), out.GetRightPeritubularCapillariesHydrostaticPressure());
    io::Property::Marshall(in.RightPeritubularCapillariesOsmoticPressure(), out.GetRightPeritubularCapillariesOsmoticPressure());
    io::Property::Marshall(in.RightReabsorptionFiltrationCoefficient(), out.GetRightReabsorptionFiltrationCoefficient());
    io::Property::Marshall(in.RightReabsorptionRate(), out.GetRightReabsorptionRate());
    io::Property::Marshall(in.RightTubularReabsorptionFiltrationSurfaceArea(), out.GetRightTubularReabsorptionFiltrationSurfaceArea());
    io::Property::Marshall(in.RightTubularReabsorptionFluidPermeability(), out.GetRightTubularReabsorptionFluidPermeability());
    io::Property::Marshall(in.RightTubularHydrostaticPressure(), out.GetRightTubularHydrostaticPressure());
    io::Property::Marshall(in.RightTubularOsmoticPressure(), out.GetRightTubularOsmoticPressure());

    io::Property::Marshall(in.UrinationRate(), out.GetUrinationRate());
    io::Property::Marshall(in.UrineOsmolality(), out.GetUrineOsmolality());
    io::Property::Marshall(in.UrineOsmolarity(), out.GetUrineOsmolarity());
    io::Property::Marshall(in.UrineProductionRate(), out.GetUrineProductionRate());
    io::Property::Marshall(in.MeanUrineOutput(), out.GetMeanUrineOutput());
    io::Property::Marshall(in.UrineSpecificGravity(), out.GetUrineSpecificGravity());
    io::Property::Marshall(in.UrineVolume(), out.GetUrineVolume());
    io::Property::Marshall(in.UrineUreaNitrogenConcentration(), out.GetUrineUreaNitrogenConcentration());
  }
  void Physiology::UnMarshall(const SERenalSystem& in, CDM::RenalSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BladderPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, GlomerularFiltrationRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, FiltrationFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftAfferentArterioleResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftBowmansCapsulesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftBowmansCapsulesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftEfferentArterioleResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftGlomerularCapillariesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftGlomerularCapillariesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftGlomerularFiltrationCoefficient)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftGlomerularFiltrationRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftGlomerularFiltrationSurfaceArea)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftGlomerularFluidPermeability)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftFiltrationFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftNetFiltrationPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftNetReabsorptionPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftPeritubularCapillariesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftPeritubularCapillariesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftReabsorptionFiltrationCoefficient)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftReabsorptionRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftTubularReabsorptionFiltrationSurfaceArea)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftTubularReabsorptionFluidPermeability)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftTubularHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftTubularOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RenalBloodFlow)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RenalPlasmaFlow)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RenalVascularResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightAfferentArterioleResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightBowmansCapsulesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightBowmansCapsulesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightEfferentArterioleResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightGlomerularCapillariesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightGlomerularCapillariesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightGlomerularFiltrationCoefficient)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightGlomerularFiltrationRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightGlomerularFiltrationSurfaceArea)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightGlomerularFluidPermeability)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightFiltrationFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightNetFiltrationPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightNetReabsorptionPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightPeritubularCapillariesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightPeritubularCapillariesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightReabsorptionFiltrationCoefficient)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightReabsorptionRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightTubularReabsorptionFiltrationSurfaceArea)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightTubularReabsorptionFluidPermeability)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightTubularHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightTubularOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, UrinationRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, UrineOsmolality)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, UrineOsmolarity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, UrineProductionRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanUrineOutput)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, UrineSpecificGravity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, UrineVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, UrineUreaNitrogenConcentration)
  }
  //----------------------------------------------------------------------------------
  // class SERespiratorySystem
  void Physiology::Marshall(const CDM::RespiratorySystemData& in, SERespiratorySystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.AlveolarArterialGradient(), out.GetAlveolarArterialGradient());
    io::Property::Marshall(in.CarricoIndex(), out.GetCarricoIndex());
    io::Property::Marshall(in.EndTidalCarbonDioxideFraction(), out.GetEndTidalCarbonDioxideFraction());
    io::Property::Marshall(in.EndTidalCarbonDioxidePressure(), out.GetEndTidalCarbonDioxidePressure());
    io::Property::Marshall(in.ExpiratoryFlow(), out.GetExpiratoryFlow());
    io::Property::Marshall(in.InspiratoryExpiratoryRatio(), out.GetInspiratoryExpiratoryRatio());
    io::Property::Marshall(in.InspiratoryFlow(), out.GetInspiratoryFlow());
    io::Property::Marshall(in.MeanPleuralPressure(), out.GetMeanPleuralPressure());
    io::Property::Marshall(in.PulmonaryCompliance(), out.GetPulmonaryCompliance());
    io::Property::Marshall(in.PulmonaryResistance(), out.GetPulmonaryResistance());
    io::Property::Marshall(in.RespirationDriverFrequency(), out.GetRespirationDriverFrequency());
    io::Property::Marshall(in.RespirationDriverPressure(), out.GetRespirationDriverPressure());
    io::Property::Marshall(in.RespirationMusclePressure(), out.GetRespirationMusclePressure());
    io::Property::Marshall(in.RespirationRate(), out.GetRespirationRate());
    io::Property::Marshall(in.SpecificVentilation(), out.GetSpecificVentilation());
    io::Property::Marshall(in.TargetPulmonaryVentilation(), out.GetTargetPulmonaryVentilation());
    io::Property::Marshall(in.TidalVolume(), out.GetTidalVolume());
    io::Property::Marshall(in.TotalAlveolarVentilation(), out.GetTotalAlveolarVentilation());
    io::Property::Marshall(in.TotalDeadSpaceVentilation(), out.GetTotalDeadSpaceVentilation());
    io::Property::Marshall(in.TotalLungVolume(), out.GetTotalLungVolume());
    io::Property::Marshall(in.TotalPulmonaryVentilation(), out.GetTotalPulmonaryVentilation());
    io::Property::Marshall(in.TranspulmonaryPressure(), out.GetTranspulmonaryPressure());
  }
  void Physiology::UnMarshall(const SERespiratorySystem& in, CDM::RespiratorySystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AlveolarArterialGradient)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CarricoIndex)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, EndTidalCarbonDioxideFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, EndTidalCarbonDioxidePressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ExpiratoryFlow)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, InspiratoryExpiratoryRatio)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, InspiratoryFlow)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanPleuralPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryCompliance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PulmonaryResistance)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespirationDriverFrequency)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespirationDriverPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespirationMusclePressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespirationRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SpecificVentilation)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TargetPulmonaryVentilation)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TidalVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TotalAlveolarVentilation)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TotalDeadSpaceVentilation)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TotalLungVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TotalPulmonaryVentilation)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TranspulmonaryPressure)
  }
  //----------------------------------------------------------------------------------
  // class SETissueSystem
  void Physiology::Marshall(const CDM::TissueSystemData& in, SETissueSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.CarbonDioxideProductionRate(), out.GetCarbonDioxideProductionRate());
    io::Property::Marshall(in.DehydrationFraction(), out.GetDehydrationFraction());
    io::Property::Marshall(in.ExtracellularFluidVolume(), out.GetExtracellularFluidVolume());
    io::Property::Marshall(in.ExtravascularFluidVolume(), out.GetExtravascularFluidVolume());
    io::Property::Marshall(in.IntracellularFluidPH(), out.GetIntracellularFluidPH());
    io::Property::Marshall(in.IntracellularFluidVolume(), out.GetIntracellularFluidVolume());
    io::Property::Marshall(in.TotalBodyFluidVolume(), out.GetTotalBodyFluidVolume());
    io::Property::Marshall(in.OxygenConsumptionRate(), out.GetOxygenConsumptionRate());
    io::Property::Marshall(in.RespiratoryExchangeRatio(), out.GetRespiratoryExchangeRatio());
    io::Property::Marshall(in.LiverInsulinSetPoint(), out.GetLiverInsulinSetPoint());
    io::Property::Marshall(in.LiverGlucagonSetPoint(), out.GetLiverGlucagonSetPoint());
    io::Property::Marshall(in.MuscleInsulinSetPoint(), out.GetMuscleInsulinSetPoint());
    io::Property::Marshall(in.MuscleGlucagonSetPoint(), out.GetMuscleGlucagonSetPoint());
    io::Property::Marshall(in.FatInsulinSetPoint(), out.GetFatInsulinSetPoint());
    io::Property::Marshall(in.FatGlucagonSetPoint(), out.GetFatGlucagonSetPoint());
    io::Property::Marshall(in.LiverGlycogen(), out.GetLiverGlycogen());
    io::Property::Marshall(in.MuscleGlycogen(), out.GetMuscleGlycogen());
    io::Property::Marshall(in.StoredProtein(), out.GetStoredProtein());
    io::Property::Marshall(in.StoredFat(), out.GetStoredFat());
  }
  void Physiology::UnMarshall(const SETissueSystem& in, CDM::TissueSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, CarbonDioxideProductionRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, DehydrationFraction)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ExtracellularFluidVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ExtravascularFluidVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, IntracellularFluidPH)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, IntracellularFluidVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TotalBodyFluidVolume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, OxygenConsumptionRate)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespiratoryExchangeRatio)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LiverInsulinSetPoint)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LiverGlucagonSetPoint)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MuscleInsulinSetPoint)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MuscleGlucagonSetPoint)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, FatInsulinSetPoint)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, FatGlucagonSetPoint)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LiverGlycogen)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MuscleGlycogen)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, StoredProtein)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, StoredFat)
  }
  //----------------------------------------------------------------------------------
}
}
