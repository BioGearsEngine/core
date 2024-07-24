#include "Physiology.h"

#include "Actions.h"
#include "PatientNutrition.h"
#include "Property.h"
#include "System.h"

#include <biogears/cdm/properties/SEProperties.h>

#include <biogears/cdm/substance/SESubstance.h>
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
  void Physiology::UnMarshall(const CDM::PupillaryResponseData& in, SEPupillaryResponse& out)
  {
    io::Property::UnMarshall(in.ReactivityModifier(), out.GetReactivityModifier());
    io::Property::UnMarshall(in.ShapeModifier(), out.GetShapeModifier());
    io::Property::UnMarshall(in.SizeModifier(), out.GetSizeModifier());
  }
  void Physiology::Marshall(const SEPupillaryResponse& in, CDM::PupillaryResponseData& out)
  {
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ReactivityModifier)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ShapeModifier)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SizeModifier)
  }
  //----------------------------------------------------------------------------------
  // class SEInflammationState
  void Physiology::UnMarshall(const CDM::InflammatoryResponseData& in, SEInflammatoryResponse& out)
  {
    auto activeTLR = out.GetActiveTLR();
    io::Property::UnMarshall(in.ActiveTLR(), activeTLR);
    out.SetActiveTLR(activeTLR);

    io::Property::UnMarshall(in.LocalPathogen(), out.GetLocalPathogen());
    io::Property::UnMarshall(in.LocalMacrophage(), out.GetLocalMacrophage());
    io::Property::UnMarshall(in.LocalNeutrophil(), out.GetLocalNeutrophil());
    io::Property::UnMarshall(in.LocalBarrier(), out.GetLocalBarrier());
    io::Property::UnMarshall(in.AutonomicResponseLevel(), out.GetAutonomicResponseLevel());
    io::Property::UnMarshall(in.Catecholamines(), out.GetCatecholamines());
    io::Property::UnMarshall(in.ConstitutiveNOS(), out.GetConstitutiveNOS());
    io::Property::UnMarshall(in.InducibleNOSPre(), out.GetInducibleNOSPre());
    io::Property::UnMarshall(in.InducibleNOS(), out.GetInducibleNOS());
    io::Property::UnMarshall(in.Interleukin6(), out.GetInterleukin6());
    io::Property::UnMarshall(in.Interleukin10(), out.GetInterleukin10());
    io::Property::UnMarshall(in.Interleukin12(), out.GetInterleukin12());
    io::Property::UnMarshall(in.MacrophageResting(), out.GetMacrophageResting());
    io::Property::UnMarshall(in.MacrophageActive(), out.GetMacrophageActive());
    io::Property::UnMarshall(in.NeutrophilResting(), out.GetNeutrophilResting());
    io::Property::UnMarshall(in.NeutrophilActive(), out.GetNeutrophilActive());
    io::Property::UnMarshall(in.Nitrate(), out.GetNitrate());
    io::Property::UnMarshall(in.NitricOxide(), out.GetNitricOxide());
    io::Property::UnMarshall(in.BloodPathogen(), out.GetBloodPathogen());
    io::Property::UnMarshall(in.TissueIntegrity(), out.GetTissueIntegrity());
    io::Property::UnMarshall(in.Trauma(), out.GetTrauma());
    io::Property::UnMarshall(in.TumorNecrosisFactor(), out.GetTumorNecrosisFactor());
    io::Property::UnMarshall(in.InflammationTime(), out.GetInflammationTime());

    SEInflammationSource conversion;
    for (auto src : in.Source()) {

      UnMarshall(src, conversion);
      out.m_InflammationSources.push_back(conversion);
    }
  }
  void Physiology::Marshall(const SEInflammatoryResponse& in, CDM::InflammatoryResponseData& out)
  {

    SE_PROPERTY_ENUM_MARSHALL_HELPER(in, out, ActiveTLR);

    CDM_PROPERTY_MARSHALL_HELPER(in, out, LocalPathogen)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, LocalMacrophage)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, LocalNeutrophil)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, LocalBarrier)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, AutonomicResponseLevel)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Catecholamines)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, ConstitutiveNOS)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, InducibleNOSPre)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, InducibleNOS)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Interleukin6)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Interleukin10)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Interleukin12)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, MacrophageResting)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, MacrophageActive)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, NeutrophilResting)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, NeutrophilActive)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Nitrate)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, NitricOxide)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, BloodPathogen)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, TissueIntegrity)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Trauma)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, TumorNecrosisFactor)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, InflammationTime)

    CDM::enumInflammationSource conversion;
    for (auto src : in.m_InflammationSources) {

      Marshall(src, conversion);
      out.Source().push_back(conversion);
    }
  }
  //----------------------------------------------------------------------------------
  // class SEBloodChemistrySystem
  void Physiology::UnMarshall(const CDM::BloodChemistrySystemData& in, SEBloodChemistrySystem& out)
  {
    System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));

    io::Property::UnMarshall(in.BloodDensity(), out.GetBloodDensity());
    io::Property::UnMarshall(in.ArterialBloodPH(), out.GetArterialBloodPH());
    io::Property::UnMarshall(in.ArterialBloodPHBaseline(), out.GetArterialBloodPHBaseline());
    io::Property::UnMarshall(in.VenousBloodPH(), out.GetVenousBloodPH());
    io::Property::UnMarshall(in.BloodSpecificHeat(), out.GetBloodSpecificHeat());
    io::Property::UnMarshall(in.BloodUreaNitrogenConcentration(), out.GetBloodUreaNitrogenConcentration());
    io::Property::UnMarshall(in.CarbonDioxideSaturation(), out.GetCarbonDioxideSaturation());
    io::Property::UnMarshall(in.CarbonMonoxideSaturation(), out.GetCarbonMonoxideSaturation());
    io::Property::UnMarshall(in.Hematocrit(), out.GetHematocrit());
    io::Property::UnMarshall(in.HemoglobinContent(), out.GetHemoglobinContent());
    io::Property::UnMarshall(in.HemoglobinLostToUrine(), out.GetHemoglobinLostToUrine());
    io::Property::UnMarshall(in.LymphocyteCellCount(), out.GetLymphocyteCellCount());
    io::Property::UnMarshall(in.NeutrophilCellCount(), out.GetNeutrophilCellCount());
    io::Property::UnMarshall(in.OxygenSaturation(), out.GetOxygenSaturation());
    io::Property::UnMarshall(in.OxygenVenousSaturation(), out.GetOxygenVenousSaturation());
    io::Property::UnMarshall(in.Phosphate(), out.GetPhosphate());
    io::Property::UnMarshall(in.PlasmaVolume(), out.GetPlasmaVolume());
    io::Property::UnMarshall(in.PulseOximetry(), out.GetPulseOximetry());
    io::Property::UnMarshall(in.RedBloodCellAcetylcholinesterase(), out.GetRedBloodCellAcetylcholinesterase());
    io::Property::UnMarshall(in.RedBloodCellCount(), out.GetRedBloodCellCount());
    io::Property::UnMarshall(in.ShuntFraction(), out.GetShuntFraction());
    io::Property::UnMarshall(in.StrongIonDifference(), out.GetStrongIonDifference());
    io::Property::UnMarshall(in.TotalBilirubin(), out.GetTotalBilirubin());
    io::Property::UnMarshall(in.TotalProteinConcentration(), out.GetTotalProteinConcentration());
    io::Property::UnMarshall(in.RhTransfusionReactionVolume(), out.GetRhTransfusionReactionVolume());
    io::Property::UnMarshall(in.ViralLoad(), out.GetViralLoad());
    io::Property::UnMarshall(in.VolumeFractionNeutralPhospholipidInPlasma(), out.GetVolumeFractionNeutralPhospholipidInPlasma());
    io::Property::UnMarshall(in.VolumeFractionNeutralLipidInPlasma(), out.GetVolumeFractionNeutralLipidInPlasma());
    io::Property::UnMarshall(in.WhiteBloodCellCount(), out.GetWhiteBloodCellCount());

    io::Property::UnMarshall(in.ArterialCarbonDioxidePressure(), out.GetArterialCarbonDioxidePressure());
    io::Property::UnMarshall(in.ArterialOxygenPressure(), out.GetArterialOxygenPressure());
    io::Property::UnMarshall(in.PulmonaryArterialCarbonDioxidePressure(), out.GetPulmonaryArterialCarbonDioxidePressure());
    io::Property::UnMarshall(in.PulmonaryArterialOxygenPressure(), out.GetPulmonaryArterialOxygenPressure());
    io::Property::UnMarshall(in.PulmonaryVenousCarbonDioxidePressure(), out.GetPulmonaryVenousCarbonDioxidePressure());
    io::Property::UnMarshall(in.PulmonaryVenousOxygenPressure(), out.GetPulmonaryVenousOxygenPressure());
    io::Property::UnMarshall(in.VenousCarbonDioxidePressure(), out.GetVenousCarbonDioxidePressure());
    io::Property::UnMarshall(in.VenousOxygenPressure(), out.GetVenousOxygenPressure());

    UnMarshall(in.InflammatoryResponse(), out.GetInflammatoryResponse());
  }
  void Physiology::Marshall(const SEBloodChemistrySystem& in, CDM::BloodChemistrySystemData& out)
  {
    System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BloodDensity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ArterialBloodPH)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ArterialBloodPHBaseline)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, VenousBloodPH)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BloodSpecificHeat)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BloodUreaNitrogenConcentration)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CarbonDioxideSaturation)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CarbonMonoxideSaturation)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Hematocrit)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HemoglobinContent)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HemoglobinLostToUrine)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LymphocyteCellCount)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, NeutrophilCellCount)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, OxygenSaturation)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, OxygenVenousSaturation)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Phosphate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PlasmaVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulseOximetry)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RedBloodCellAcetylcholinesterase)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RedBloodCellCount)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ShuntFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, StrongIonDifference)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalBilirubin)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalProteinConcentration)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RhTransfusionReactionVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ViralLoad)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, VolumeFractionNeutralPhospholipidInPlasma)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, VolumeFractionNeutralLipidInPlasma)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, WhiteBloodCellCount)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ArterialCarbonDioxidePressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ArterialOxygenPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryArterialCarbonDioxidePressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryArterialOxygenPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryVenousCarbonDioxidePressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryVenousOxygenPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, VenousCarbonDioxidePressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, VenousOxygenPressure)

    Marshall(*in.m_InflammatoryResponse, out.InflammatoryResponse());
  }
  //----------------------------------------------------------------------------------
  // class SECardiovascularSystem
  void Physiology::UnMarshall(const CDM::CardiovascularSystemData& in, SECardiovascularSystem& out)
  {
    System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));

    UnMarshall(in.HeartRhythm(), out.m_HeartRhythm);

    io::Property::UnMarshall(in.ArterialPressure(), out.GetArterialPressure());
    io::Property::UnMarshall(in.BloodVolume(), out.GetBloodVolume());
    io::Property::UnMarshall(in.CardiacIndex(), out.GetCardiacIndex());
    io::Property::UnMarshall(in.CardiacOutput(), out.GetCardiacOutput());
    io::Property::UnMarshall(in.CentralVenousPressure(), out.GetCentralVenousPressure());
    io::Property::UnMarshall(in.CerebralBloodFlow(), out.GetCerebralBloodFlow());
    io::Property::UnMarshall(in.CerebralPerfusionPressure(), out.GetCerebralPerfusionPressure());
    io::Property::UnMarshall(in.DiastolicArterialPressure(), out.GetDiastolicArterialPressure());
    io::Property::UnMarshall(in.ExtremityPressureLeftArm(), out.GetExtremityPressureLeftArm());
    io::Property::UnMarshall(in.ExtremityPressureLeftLeg(), out.GetExtremityPressureLeftLeg());
    io::Property::UnMarshall(in.ExtremityPressureRightArm(), out.GetExtremityPressureRightArm());
    io::Property::UnMarshall(in.ExtremityPressureRightLeg(), out.GetExtremityPressureRightLeg());
    io::Property::UnMarshall(in.HeartEjectionFraction(), out.GetHeartEjectionFraction());
    io::Property::UnMarshall(in.HeartRate(), out.GetHeartRate());

    io::Property::UnMarshall(in.HeartStrokeVolume(), out.GetHeartStrokeVolume());
    io::Property::UnMarshall(in.IntracranialPressure(), out.GetIntracranialPressure());
    io::Property::UnMarshall(in.MeanArterialPressure(), out.GetMeanArterialPressure());
    io::Property::UnMarshall(in.MeanArterialCarbonDioxidePartialPressure(), out.GetMeanArterialCarbonDioxidePartialPressure());
    io::Property::UnMarshall(in.MeanArterialCarbonDioxidePartialPressureDelta(), out.GetMeanArterialCarbonDioxidePartialPressureDelta());
    io::Property::UnMarshall(in.MeanCentralVenousPressure(), out.GetMeanCentralVenousPressure());
    io::Property::UnMarshall(in.MeanSkinFlow(), out.GetMeanSkinFlow());
    io::Property::UnMarshall(in.PulmonaryArterialPressure(), out.GetPulmonaryArterialPressure());
    io::Property::UnMarshall(in.PulmonaryCapillariesWedgePressure(), out.GetPulmonaryCapillariesWedgePressure());
    io::Property::UnMarshall(in.PulmonaryDiastolicArterialPressure(), out.GetPulmonaryDiastolicArterialPressure());
    io::Property::UnMarshall(in.PulmonaryMeanArterialPressure(), out.GetPulmonaryMeanArterialPressure());
    io::Property::UnMarshall(in.PulmonaryMeanCapillaryFlow(), out.GetPulmonaryMeanCapillaryFlow());
    io::Property::UnMarshall(in.PulmonaryMeanShuntFlow(), out.GetPulmonaryMeanShuntFlow());
    io::Property::UnMarshall(in.PulmonarySystolicArterialPressure(), out.GetPulmonarySystolicArterialPressure());
    io::Property::UnMarshall(in.PulmonaryVascularResistance(), out.GetPulmonaryVascularResistance());
    io::Property::UnMarshall(in.PulmonaryVascularResistanceIndex(), out.GetPulmonaryVascularResistanceIndex());
    io::Property::UnMarshall(in.PulsePressure(), out.GetPulsePressure());
    io::Property::UnMarshall(in.SystolicArterialPressure(), out.GetSystolicArterialPressure());
    io::Property::UnMarshall(in.SystemicVascularResistance(), out.GetSystemicVascularResistance());
  }
  void Physiology::Marshall(const SECardiovascularSystem& in, CDM::CardiovascularSystemData& out)
  {
    System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));

    SE_OPTIONAL_PHYSIOLOGY_ENUM_MARSHALL_HELPER(in, out, HeartRhythm)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ArterialPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BloodVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CardiacIndex)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CardiacOutput)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CentralVenousPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CerebralBloodFlow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CerebralPerfusionPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DiastolicArterialPressure)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExtremityPressureLeftArm)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExtremityPressureLeftLeg)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExtremityPressureRightArm)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExtremityPressureRightLeg)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HeartEjectionFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HeartRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HeartStrokeVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, IntracranialPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanArterialPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanArterialCarbonDioxidePartialPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanArterialCarbonDioxidePartialPressureDelta)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanCentralVenousPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanSkinFlow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryArterialPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryCapillariesWedgePressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryDiastolicArterialPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryMeanArterialPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryMeanCapillaryFlow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryMeanShuntFlow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonarySystolicArterialPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryVascularResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryVascularResistanceIndex)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulsePressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SystolicArterialPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SystemicVascularResistance)
  }
  //----------------------------------------------------------------------------------
  // class SEDrugSystem
  void Physiology::UnMarshall(const CDM::DrugSystemData& in, SEDrugSystem& out)
  {
    System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::UnMarshall(in.AntibioticActivity(), out.GetAntibioticActivity());
    io::Property::UnMarshall(in.BronchodilationLevel(), out.GetBronchodilationLevel());
    io::Property::UnMarshall(in.FeverChange(), out.GetFeverChange());
    io::Property::UnMarshall(in.HeartRateChange(), out.GetHeartRateChange());
    io::Property::UnMarshall(in.HemorrhageChange(), out.GetHemorrhageChange());
    io::Property::UnMarshall(in.MeanBloodPressureChange(), out.GetMeanBloodPressureChange());
    io::Property::UnMarshall(in.NeuromuscularBlockLevel(), out.GetNeuromuscularBlockLevel());
    io::Property::UnMarshall(in.PainToleranceChange(), out.GetPainToleranceChange());
    io::Property::UnMarshall(in.PulsePressureChange(), out.GetPulsePressureChange());
    io::Property::UnMarshall(in.RespirationRateChange(), out.GetRespirationRateChange());
    io::Property::UnMarshall(in.SedationLevel(), out.GetSedationLevel());
    io::Property::UnMarshall(in.TidalVolumeChange(), out.GetTidalVolumeChange());
    io::Property::UnMarshall(in.TubularPermeabilityChange(), out.GetTubularPermeabilityChange());
    io::Property::UnMarshall(in.CentralNervousResponse(), out.GetCentralNervousResponse());
  }
  void Physiology::Marshall(const SEDrugSystem& in, CDM::DrugSystemData& out)
  {
    System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AntibioticActivity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BronchodilationLevel)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, FeverChange)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HeartRateChange)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HemorrhageChange)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanBloodPressureChange)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, NeuromuscularBlockLevel)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PainToleranceChange)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulsePressureChange)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespirationRateChange)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SedationLevel)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TidalVolumeChange)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TubularPermeabilityChange)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CentralNervousResponse)
  }
  //----------------------------------------------------------------------------------
  // class SEEndocrineSystem
  void Physiology::UnMarshall(const CDM::EndocrineSystemData& in, SEEndocrineSystem& out)
  {
    System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::UnMarshall(in.InsulinSynthesisRate(), out.GetInsulinSynthesisRate());
    io::Property::UnMarshall(in.GlucagonSynthesisRate(), out.GetGlucagonSynthesisRate());
  }
  void Physiology::Marshall(const SEEndocrineSystem& in, CDM::EndocrineSystemData& out)
  {
    System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, InsulinSynthesisRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, GlucagonSynthesisRate)
  }
  //----------------------------------------------------------------------------------
  // class SEEnergySystem
  void Physiology::UnMarshall(const CDM::EnergySystemData& in, SEEnergySystem& out)
  {

    System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::UnMarshall(in.AchievedExerciseLevel(), out.GetAchievedExerciseLevel());
    io::Property::UnMarshall(in.ChlorideLostToSweat(), out.GetChlorideLostToSweat());
    io::Property::UnMarshall(in.CoreTemperature(), out.GetCoreTemperature());
    io::Property::UnMarshall(in.CreatinineProductionRate(), out.GetCreatinineProductionRate());
    io::Property::UnMarshall(in.EnergyDeficit(), out.GetEnergyDeficit());
    io::Property::UnMarshall(in.ExerciseEnergyDemand(), out.GetExerciseEnergyDemand());
    io::Property::UnMarshall(in.ExerciseMeanArterialPressureDelta(), out.GetExerciseMeanArterialPressureDelta());
    io::Property::UnMarshall(in.FatigueLevel(), out.GetFatigueLevel());
    io::Property::UnMarshall(in.LactateProductionRate(), out.GetLactateProductionRate());
    io::Property::UnMarshall(in.PotassiumLostToSweat(), out.GetPotassiumLostToSweat());
    io::Property::UnMarshall(in.SkinTemperature(), out.GetSkinTemperature());
    io::Property::UnMarshall(in.SkinTemperatureTorso(), out.GetSkinTemperatureTorso());
    io::Property::UnMarshall(in.SkinTemperatureHead(), out.GetSkinTemperatureHead());
    io::Property::UnMarshall(in.SkinTemperatureLeftArm(), out.GetSkinTemperatureLeftArm());
    io::Property::UnMarshall(in.SkinTemperatureRightArm(), out.GetSkinTemperatureRightArm());
    io::Property::UnMarshall(in.SkinTemperatureLeftLeg(), out.GetSkinTemperatureLeftLeg());
    io::Property::UnMarshall(in.SkinTemperatureRightLeg(), out.GetSkinTemperatureRightLeg());
    io::Property::UnMarshall(in.BurnSkinTemperature(), out.GetBurnSkinTemperature());
    io::Property::UnMarshall(in.SodiumLostToSweat(), out.GetSodiumLostToSweat());
    io::Property::UnMarshall(in.SweatRate(), out.GetSweatRate());
    io::Property::UnMarshall(in.TotalMetabolicRate(), out.GetTotalMetabolicRate());
    io::Property::UnMarshall(in.TotalWorkRateLevel(), out.GetTotalWorkRateLevel());
  }
  void Physiology::Marshall(const SEEnergySystem& in, CDM::EnergySystemData& out)
  {
    System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AchievedExerciseLevel)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ChlorideLostToSweat)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CoreTemperature)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CreatinineProductionRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, EnergyDeficit)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExerciseEnergyDemand)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExerciseMeanArterialPressureDelta)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, FatigueLevel)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LactateProductionRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PotassiumLostToSweat)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SkinTemperature)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SkinTemperatureTorso)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SkinTemperatureHead)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SkinTemperatureLeftArm)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SkinTemperatureRightArm)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SkinTemperatureLeftLeg)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SkinTemperatureRightLeg)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BurnSkinTemperature);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SodiumLostToSweat)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SweatRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalMetabolicRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalWorkRateLevel)
  }
  //----------------------------------------------------------------------------------
  // class SEGastrointestinalSystem
  void Physiology::UnMarshall(const CDM::GastrointestinalSystemData& in, SEGastrointestinalSystem& out)
  {
    System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    if (in.ChymeAbsorptionRate().present()) {
      io::Property::UnMarshall(in.ChymeAbsorptionRate(), out.GetChymeAbsorptionRate());
    }
    if (in.StomachContents().present()) {
      out.GetStomachContents().Load(in.StomachContents().get());
    }
  }
  void Physiology::Marshall(const SEGastrointestinalSystem& in, CDM::GastrointestinalSystemData& out)
  {
    System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ChymeAbsorptionRate)
    CDM_OPTIONAL_PATIENT_NUTRITION_MARSHALL_HELPER(in, out, StomachContents)
  }
  //----------------------------------------------------------------------------------
  // class SEDrugTransitState
  void Physiology::UnMarshall(const CDM::DrugTransitStateData& in, SEDrugTransitState& out) {
    out.m_LumenDissolvedMasses.clear();
    SEScalarMass mass;
    for (auto disMass : in.LumenDissolvedMasses()) {
      io::Property::UnMarshall(disMass, mass);
      out.m_LumenDissolvedMasses.push_back(mass);
    }
    out.m_LumenSolidMasses.clear();
    for (auto solMass : in.LumenSolidMasses()) {
      io::Property::UnMarshall(solMass, mass);
      out.m_LumenSolidMasses.push_back(mass);
    }
    out.m_EnterocyteMasses.clear();
    for (auto entMass : in.EnterocyteMasses()) {
      io::Property::UnMarshall(entMass, mass);
      out.m_EnterocyteMasses.push_back(mass);
    }
    io::Property::UnMarshall(in.MassExcreted(), out.GetTotalMassExcreted());
    io::Property::UnMarshall(in.MassMetabolized(), out.GetTotalMassMetabolized());
  
  }

  void Physiology::Marshall(const SEDrugTransitState& in, CDM::DrugTransitStateData& out) {
    for (auto tdMass : in.m_LumenDissolvedMasses) {
      out.LumenDissolvedMasses().push_back(std::unique_ptr<CDM::ScalarMassData>());
      io::Property::Marshall(tdMass, out.LumenDissolvedMasses().back());
    }
    for (auto tsMass : in.m_LumenSolidMasses) {
      out.LumenSolidMasses().push_back(std::unique_ptr<CDM::ScalarMassData>());
      io::Property::Marshall(tsMass, out.LumenSolidMasses().back());
    }
    for (auto eMass : in.m_EnterocyteMasses) {
      out.EnterocyteMasses().push_back(std::unique_ptr<CDM::ScalarMassData>());
      io::Property::Marshall(eMass, out.EnterocyteMasses().back());
    }
    if (in.m_TotalMassMetabolized && in.m_TotalMassMetabolized->IsValid()) {
      out.MassMetabolized(std::make_unique<std::remove_reference<decltype(out.MassMetabolized())>::type>());
      io::Property::Marshall(*in.m_TotalMassMetabolized, out.MassMetabolized());
    }
    if (in.m_TotalMassExcreted && in.m_TotalMassExcreted->IsValid()) {
      out.MassExcreted(std::make_unique<std::remove_reference<decltype(out.MassExcreted())>::type>());
      io::Property::Marshall(*in.m_TotalMassMetabolized, out.MassExcreted());
    }

    out.Substance(in.m_Substance->GetName());
  }
  //----------------------------------------------------------------------------------
  // class SEHepaticSystem
  void Physiology::UnMarshall(const CDM::HepaticSystemData& in, SEHepaticSystem& out)
  {
    System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::UnMarshall(in.KetoneProductionRate(), out.GetKetoneProductionRate());
    io::Property::UnMarshall(in.HepaticGluconeogenesisRate(), out.GetHepaticGluconeogenesisRate());
  }
  //----------------------------------------------------------------------------------
  void Physiology::Marshall(const SEHepaticSystem& in, CDM::HepaticSystemData& out)
  {
    System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, KetoneProductionRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HepaticGluconeogenesisRate)
  }
  //----------------------------------------------------------------------------------
  // class SENervousSystem
  void Physiology::UnMarshall(const CDM::NervousSystemData& in, SENervousSystem& out)
  {
    System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));

    if (in.ComplianceScale().present())
      io::Property::UnMarshall(in.ComplianceScale(), out.GetComplianceScale());
    if (in.HeartRateScale().present())
      io::Property::UnMarshall(in.HeartRateScale(), out.GetHeartRateScale());
    if (in.HeartElastanceScale().present())
      io::Property::UnMarshall(in.HeartElastanceScale(), out.GetHeartElastanceScale());
    if (in.MentalStatus().present())
      io::Property::UnMarshall(in.MentalStatus(), out.GetMentalStatus());
    if (in.GlasgowComaScalar().present())
      io::Property::UnMarshall(in.GlasgowComaScalar(), out.GetGlasgowComaScalar());

    if (in.LeftEyePupillaryResponse().present())
      UnMarshall(in.LeftEyePupillaryResponse(), out.GetLeftEyePupillaryResponse());
    if (in.RightEyePupillaryResponse().present())
      UnMarshall(in.RightEyePupillaryResponse(), out.GetRightEyePupillaryResponse());

    if (in.PainVisualAnalogueScale().present())
      io::Property::UnMarshall(in.PainVisualAnalogueScale(), out.GetPainVisualAnalogueScale());
    if (in.ResistanceScaleExtrasplanchnic().present())
      io::Property::UnMarshall(in.ResistanceScaleExtrasplanchnic(), out.GetResistanceScaleExtrasplanchnic());
    if (in.ResistanceScaleMuscle().present())
      io::Property::UnMarshall(in.ResistanceScaleMuscle(), out.GetResistanceScaleMuscle());
    if (in.ResistanceScaleMyocardium().present())
      io::Property::UnMarshall(in.ResistanceScaleMyocardium(), out.GetResistanceScaleMyocardium());
    if (in.ResistanceScaleSplanchnic().present())
      io::Property::UnMarshall(in.ResistanceScaleSplanchnic(), out.GetResistanceScaleSplanchnic());
    if (in.SleepTime().present())
      io::Property::UnMarshall(in.SleepTime(), out.GetSleepTime());
    if (in.WakeTime().present())
      io::Property::UnMarshall(in.WakeTime(), out.GetWakeTime());

    if (in.SleepState().present())
      UnMarshall(in.SleepState(), out.m_SleepState);

    if (in.BiologicalDebt().present())
      io::Property::UnMarshall(in.BiologicalDebt(), out.GetBiologicalDebt());
    if (in.ReactionTime().present())
      io::Property::UnMarshall(in.ReactionTime(), out.GetReactionTime());
    if (in.AttentionLapses().present())
      io::Property::UnMarshall(in.AttentionLapses(), out.GetAttentionLapses());
    if (in.RichmondAgitationSedationScale().present())
      io::Property::UnMarshall(in.RichmondAgitationSedationScale(), out.GetRichmondAgitationSedationScale());
  }
  void Physiology::Marshall(const SENervousSystem& in, CDM::NervousSystemData& out)
  {
    System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ComplianceScale)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HeartRateScale)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HeartElastanceScale)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MentalStatus)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, GlasgowComaScalar)
    Marshall(*in.m_LeftEyePupillaryResponse, out.LeftEyePupillaryResponse());
    Marshall(*in.m_RightEyePupillaryResponse, out.RightEyePupillaryResponse());
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PainVisualAnalogueScale)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ResistanceScaleExtrasplanchnic)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ResistanceScaleMuscle)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ResistanceScaleMyocardium)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ResistanceScaleSplanchnic)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SleepTime)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, WakeTime)

    SE_OPTIONAL_PHYSIOLOGY_ENUM_MARSHALL_HELPER(in, out, SleepState)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BiologicalDebt)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ReactionTime)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AttentionLapses)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RichmondAgitationSedationScale)
  }
  //----------------------------------------------------------------------------------
  // class SERenalSystem
  void Physiology::UnMarshall(const CDM::RenalSystemData& in, SERenalSystem& out)
  {
    System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::UnMarshall(in.BladderPressure(), out.GetBladderPressure());
    io::Property::UnMarshall(in.GlomerularFiltrationRate(), out.GetGlomerularFiltrationRate());
    io::Property::UnMarshall(in.FiltrationFraction(), out.GetFiltrationFraction());

    io::Property::UnMarshall(in.LeftAfferentArterioleResistance(), out.GetLeftAfferentArterioleResistance());
    io::Property::UnMarshall(in.LeftBowmansCapsulesHydrostaticPressure(), out.GetLeftBowmansCapsulesHydrostaticPressure());
    io::Property::UnMarshall(in.LeftBowmansCapsulesOsmoticPressure(), out.GetLeftBowmansCapsulesOsmoticPressure());
    io::Property::UnMarshall(in.LeftEfferentArterioleResistance(), out.GetLeftEfferentArterioleResistance());
    io::Property::UnMarshall(in.LeftGlomerularCapillariesHydrostaticPressure(), out.GetLeftGlomerularCapillariesHydrostaticPressure());
    io::Property::UnMarshall(in.LeftGlomerularCapillariesOsmoticPressure(), out.GetLeftGlomerularCapillariesOsmoticPressure());
    io::Property::UnMarshall(in.LeftGlomerularFiltrationCoefficient(), out.GetLeftGlomerularFiltrationCoefficient());
    io::Property::UnMarshall(in.LeftGlomerularFiltrationRate(), out.GetLeftGlomerularFiltrationRate());
    io::Property::UnMarshall(in.LeftGlomerularFiltrationSurfaceArea(), out.GetLeftGlomerularFiltrationSurfaceArea());
    io::Property::UnMarshall(in.LeftGlomerularFluidPermeability(), out.GetLeftGlomerularFluidPermeability());
    io::Property::UnMarshall(in.LeftFiltrationFraction(), out.GetLeftFiltrationFraction());
    io::Property::UnMarshall(in.LeftNetFiltrationPressure(), out.GetLeftNetFiltrationPressure());
    io::Property::UnMarshall(in.LeftNetReabsorptionPressure(), out.GetLeftNetReabsorptionPressure());
    io::Property::UnMarshall(in.LeftPeritubularCapillariesHydrostaticPressure(), out.GetLeftPeritubularCapillariesHydrostaticPressure());
    io::Property::UnMarshall(in.LeftPeritubularCapillariesOsmoticPressure(), out.GetLeftPeritubularCapillariesOsmoticPressure());
    io::Property::UnMarshall(in.LeftReabsorptionFiltrationCoefficient(), out.GetLeftReabsorptionFiltrationCoefficient());
    io::Property::UnMarshall(in.LeftReabsorptionRate(), out.GetLeftReabsorptionRate());
    io::Property::UnMarshall(in.LeftTubularReabsorptionFiltrationSurfaceArea(), out.GetLeftTubularReabsorptionFiltrationSurfaceArea());
    io::Property::UnMarshall(in.LeftTubularReabsorptionFluidPermeability(), out.GetLeftTubularReabsorptionFluidPermeability());
    io::Property::UnMarshall(in.LeftTubularHydrostaticPressure(), out.GetLeftTubularHydrostaticPressure());
    io::Property::UnMarshall(in.LeftTubularOsmoticPressure(), out.GetLeftTubularOsmoticPressure());

    io::Property::UnMarshall(in.RenalBloodFlow(), out.GetRenalBloodFlow());
    io::Property::UnMarshall(in.RenalPlasmaFlow(), out.GetRenalPlasmaFlow());
    io::Property::UnMarshall(in.RenalVascularResistance(), out.GetRenalVascularResistance());

    io::Property::UnMarshall(in.RightAfferentArterioleResistance(), out.GetRightAfferentArterioleResistance());
    io::Property::UnMarshall(in.RightBowmansCapsulesHydrostaticPressure(), out.GetRightBowmansCapsulesHydrostaticPressure());
    io::Property::UnMarshall(in.RightBowmansCapsulesOsmoticPressure(), out.GetRightBowmansCapsulesOsmoticPressure());
    io::Property::UnMarshall(in.RightEfferentArterioleResistance(), out.GetRightEfferentArterioleResistance());
    io::Property::UnMarshall(in.RightGlomerularCapillariesHydrostaticPressure(), out.GetRightGlomerularCapillariesHydrostaticPressure());
    io::Property::UnMarshall(in.RightGlomerularCapillariesOsmoticPressure(), out.GetRightGlomerularCapillariesOsmoticPressure());
    io::Property::UnMarshall(in.RightGlomerularFiltrationCoefficient(), out.GetRightGlomerularFiltrationCoefficient());
    io::Property::UnMarshall(in.RightGlomerularFiltrationRate(), out.GetRightGlomerularFiltrationRate());
    io::Property::UnMarshall(in.RightGlomerularFiltrationSurfaceArea(), out.GetRightGlomerularFiltrationSurfaceArea());
    io::Property::UnMarshall(in.RightGlomerularFluidPermeability(), out.GetRightGlomerularFluidPermeability());
    io::Property::UnMarshall(in.RightFiltrationFraction(), out.GetRightFiltrationFraction());
    io::Property::UnMarshall(in.RightNetFiltrationPressure(), out.GetRightNetFiltrationPressure());
    io::Property::UnMarshall(in.RightNetReabsorptionPressure(), out.GetRightNetReabsorptionPressure());
    io::Property::UnMarshall(in.RightPeritubularCapillariesHydrostaticPressure(), out.GetRightPeritubularCapillariesHydrostaticPressure());
    io::Property::UnMarshall(in.RightPeritubularCapillariesOsmoticPressure(), out.GetRightPeritubularCapillariesOsmoticPressure());
    io::Property::UnMarshall(in.RightReabsorptionFiltrationCoefficient(), out.GetRightReabsorptionFiltrationCoefficient());
    io::Property::UnMarshall(in.RightReabsorptionRate(), out.GetRightReabsorptionRate());
    io::Property::UnMarshall(in.RightTubularReabsorptionFiltrationSurfaceArea(), out.GetRightTubularReabsorptionFiltrationSurfaceArea());
    io::Property::UnMarshall(in.RightTubularReabsorptionFluidPermeability(), out.GetRightTubularReabsorptionFluidPermeability());
    io::Property::UnMarshall(in.RightTubularHydrostaticPressure(), out.GetRightTubularHydrostaticPressure());
    io::Property::UnMarshall(in.RightTubularOsmoticPressure(), out.GetRightTubularOsmoticPressure());

    io::Property::UnMarshall(in.UrinationRate(), out.GetUrinationRate());
    io::Property::UnMarshall(in.UrineOsmolality(), out.GetUrineOsmolality());
    io::Property::UnMarshall(in.UrineOsmolarity(), out.GetUrineOsmolarity());
    io::Property::UnMarshall(in.UrineProductionRate(), out.GetUrineProductionRate());
    io::Property::UnMarshall(in.MeanUrineOutput(), out.GetMeanUrineOutput());
    io::Property::UnMarshall(in.UrineSpecificGravity(), out.GetUrineSpecificGravity());
    io::Property::UnMarshall(in.UrineVolume(), out.GetUrineVolume());
    io::Property::UnMarshall(in.UrineUreaNitrogenConcentration(), out.GetUrineUreaNitrogenConcentration());
  }
  void Physiology::Marshall(const SERenalSystem& in, CDM::RenalSystemData& out)
  {
    System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BladderPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, GlomerularFiltrationRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, FiltrationFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftAfferentArterioleResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftBowmansCapsulesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftBowmansCapsulesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftEfferentArterioleResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftGlomerularCapillariesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftGlomerularCapillariesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftGlomerularFiltrationCoefficient)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftGlomerularFiltrationRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftGlomerularFiltrationSurfaceArea)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftGlomerularFluidPermeability)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftFiltrationFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftNetFiltrationPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftNetReabsorptionPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftPeritubularCapillariesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftPeritubularCapillariesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftReabsorptionFiltrationCoefficient)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftReabsorptionRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftTubularReabsorptionFiltrationSurfaceArea)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftTubularReabsorptionFluidPermeability)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftTubularHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LeftTubularOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RenalBloodFlow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RenalPlasmaFlow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RenalVascularResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightAfferentArterioleResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightBowmansCapsulesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightBowmansCapsulesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightEfferentArterioleResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightGlomerularCapillariesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightGlomerularCapillariesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightGlomerularFiltrationCoefficient)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightGlomerularFiltrationRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightGlomerularFiltrationSurfaceArea)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightGlomerularFluidPermeability)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightFiltrationFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightNetFiltrationPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightNetReabsorptionPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightPeritubularCapillariesHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightPeritubularCapillariesOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightReabsorptionFiltrationCoefficient)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightReabsorptionRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightTubularReabsorptionFiltrationSurfaceArea)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightTubularReabsorptionFluidPermeability)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightTubularHydrostaticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RightTubularOsmoticPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UrinationRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UrineOsmolality)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UrineOsmolarity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UrineProductionRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanUrineOutput)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UrineSpecificGravity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UrineVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, UrineUreaNitrogenConcentration)
  }
  //----------------------------------------------------------------------------------
  // class SERespiratorySystem
  void Physiology::UnMarshall(const CDM::RespiratorySystemData& in, SERespiratorySystem& out)
  {
    System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::UnMarshall(in.AlveolarArterialGradient(), out.GetAlveolarArterialGradient());
    io::Property::UnMarshall(in.CarricoIndex(), out.GetCarricoIndex());
    io::Property::UnMarshall(in.EndTidalCarbonDioxideFraction(), out.GetEndTidalCarbonDioxideFraction());
    io::Property::UnMarshall(in.EndTidalCarbonDioxidePressure(), out.GetEndTidalCarbonDioxidePressure());
    io::Property::UnMarshall(in.ExpiratoryFlow(), out.GetExpiratoryFlow());
    io::Property::UnMarshall(in.InspiratoryExpiratoryRatio(), out.GetInspiratoryExpiratoryRatio());
    io::Property::UnMarshall(in.InspiratoryFlow(), out.GetInspiratoryFlow());
    io::Property::UnMarshall(in.MeanPleuralPressure(), out.GetMeanPleuralPressure());
    io::Property::UnMarshall(in.PulmonaryCompliance(), out.GetPulmonaryCompliance());
    io::Property::UnMarshall(in.PulmonaryResistance(), out.GetPulmonaryResistance());
    io::Property::UnMarshall(in.RespirationDriverFrequency(), out.GetRespirationDriverFrequency());
    io::Property::UnMarshall(in.RespirationDriverPressure(), out.GetRespirationDriverPressure());
    io::Property::UnMarshall(in.RespirationMusclePressure(), out.GetRespirationMusclePressure());
    io::Property::UnMarshall(in.RespirationRate(), out.GetRespirationRate());
    io::Property::UnMarshall(in.SpecificVentilation(), out.GetSpecificVentilation());
    io::Property::UnMarshall(in.TargetPulmonaryVentilation(), out.GetTargetPulmonaryVentilation());
    io::Property::UnMarshall(in.TidalVolume(), out.GetTidalVolume());
    io::Property::UnMarshall(in.TotalAlveolarVentilation(), out.GetTotalAlveolarVentilation());
    io::Property::UnMarshall(in.TotalDeadSpaceVentilation(), out.GetTotalDeadSpaceVentilation());
    io::Property::UnMarshall(in.TotalLungVolume(), out.GetTotalLungVolume());
    io::Property::UnMarshall(in.TotalPulmonaryVentilation(), out.GetTotalPulmonaryVentilation());
    io::Property::UnMarshall(in.TranspulmonaryPressure(), out.GetTranspulmonaryPressure());
  }
  void Physiology::Marshall(const SERespiratorySystem& in, CDM::RespiratorySystemData& out)
  {
    System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AlveolarArterialGradient)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CarricoIndex)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, EndTidalCarbonDioxideFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, EndTidalCarbonDioxidePressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExpiratoryFlow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, InspiratoryExpiratoryRatio)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, InspiratoryFlow)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanPleuralPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryCompliance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PulmonaryResistance)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespirationDriverFrequency)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespirationDriverPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespirationMusclePressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespirationRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SpecificVentilation)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TargetPulmonaryVentilation)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TidalVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalAlveolarVentilation)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalDeadSpaceVentilation)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalLungVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalPulmonaryVentilation)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TranspulmonaryPressure)
  }
  //----------------------------------------------------------------------------------
  // class SETissueSystem
  void Physiology::UnMarshall(const CDM::TissueSystemData& in, SETissueSystem& out)
  {
    System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::UnMarshall(in.CarbonDioxideProductionRate(), out.GetCarbonDioxideProductionRate());
    io::Property::UnMarshall(in.DehydrationFraction(), out.GetDehydrationFraction());
    io::Property::UnMarshall(in.ExtracellularFluidVolume(), out.GetExtracellularFluidVolume());
    io::Property::UnMarshall(in.ExtravascularFluidVolume(), out.GetExtravascularFluidVolume());
    io::Property::UnMarshall(in.IntracellularFluidPH(), out.GetIntracellularFluidPH());
    io::Property::UnMarshall(in.IntracellularFluidVolume(), out.GetIntracellularFluidVolume());
    io::Property::UnMarshall(in.TotalBodyFluidVolume(), out.GetTotalBodyFluidVolume());
    io::Property::UnMarshall(in.OxygenConsumptionRate(), out.GetOxygenConsumptionRate());
    io::Property::UnMarshall(in.RespiratoryExchangeRatio(), out.GetRespiratoryExchangeRatio());
    io::Property::UnMarshall(in.LiverInsulinSetPoint(), out.GetLiverInsulinSetPoint());
    io::Property::UnMarshall(in.LiverGlucagonSetPoint(), out.GetLiverGlucagonSetPoint());
    io::Property::UnMarshall(in.MuscleInsulinSetPoint(), out.GetMuscleInsulinSetPoint());
    io::Property::UnMarshall(in.MuscleGlucagonSetPoint(), out.GetMuscleGlucagonSetPoint());
    io::Property::UnMarshall(in.FatInsulinSetPoint(), out.GetFatInsulinSetPoint());
    io::Property::UnMarshall(in.FatGlucagonSetPoint(), out.GetFatGlucagonSetPoint());
    io::Property::UnMarshall(in.LiverGlycogen(), out.GetLiverGlycogen());
    io::Property::UnMarshall(in.MuscleGlycogen(), out.GetMuscleGlycogen());
    io::Property::UnMarshall(in.StoredProtein(), out.GetStoredProtein());
    io::Property::UnMarshall(in.StoredFat(), out.GetStoredFat());
  }
  void Physiology::Marshall(const SETissueSystem& in, CDM::TissueSystemData& out)
  {
    System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CarbonDioxideProductionRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DehydrationFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExtracellularFluidVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ExtravascularFluidVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, IntracellularFluidPH)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, IntracellularFluidVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TotalBodyFluidVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, OxygenConsumptionRate)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespiratoryExchangeRatio)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LiverInsulinSetPoint)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LiverGlucagonSetPoint)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MuscleInsulinSetPoint)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MuscleGlucagonSetPoint)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, FatInsulinSetPoint)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, FatGlucagonSetPoint)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LiverGlycogen)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MuscleGlycogen)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, StoredProtein)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, StoredFat)
  }
  //----------------------------------------------------------------------------------
  // SEInflammationSource
  void Physiology::UnMarshall(const CDM::enumInflammationSource& in, SEInflammationSource& out)
  {
    try {
      switch (in) {
      case CDM::enumInflammationSource::Burn:
        out = SEInflammationSource::Burn;
        break;
      case CDM::enumInflammationSource::Hemorrhage:
        out = SEInflammationSource::Hemorrhage;
        break;
      case CDM::enumInflammationSource::Infection:
        out = SEInflammationSource::Infection;
        break;
      case CDM::enumInflammationSource::Ebola:
        out = SEInflammationSource::Ebola;
        break;
      default:
        out = SEInflammationSource::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEInflammationSource::Invalid;
    }
  }
  void Physiology::Marshall(const SEInflammationSource& in, CDM::enumInflammationSource& out)
  {
    switch (in) {
    case SEInflammationSource::Burn:
      out = CDM::enumInflammationSource::Burn;
      break;
    case SEInflammationSource::Hemorrhage:
      out = CDM::enumInflammationSource::Hemorrhage;
      break;
    case SEInflammationSource::Infection:
      out = CDM::enumInflammationSource::Infection;
      break;
    case SEInflammationSource::Ebola:
      out = CDM::enumInflammationSource::Ebola;
      break;
    default:
      out = "";
      break;
    }
  }
  //----------------------------------------------------------------------------------
  // SEHeartRhythm
  void Physiology::UnMarshall(const CDM::enumHeartRhythm& in, SEHeartRhythm& out)
  {
    try {
      switch (in) {
      case CDM::enumHeartRhythm::Asystole:
        out = SEHeartRhythm::Asystole;
        break;
      case CDM::enumHeartRhythm::NormalSinus:
        out = SEHeartRhythm::NormalSinus;
        break;
      default:
        out = SEHeartRhythm::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEHeartRhythm::Invalid;
    }
  }
  void Physiology::Marshall(const SEHeartRhythm& in, CDM::enumHeartRhythm& out)
  {
    switch (in) {
    case SEHeartRhythm::Asystole:
      out = CDM::enumHeartRhythm::Asystole;
      break;
    case SEHeartRhythm::NormalSinus:
      out = CDM::enumHeartRhythm::NormalSinus;
      break;
    default:
      out = "";
      break;
    }
  }
  //----------------------------------------------------------------------------------
  // SESleepState
  void Physiology::UnMarshall(const CDM::enumSleepState& in, SESleepState& out)
  {
    try {
      switch (in) {
      case CDM::enumSleepState::Asleep:
        out = SESleepState::Sleeping;
        break;
      case CDM::enumSleepState::Awake:
        out = SESleepState::Awake;
        break;
      default:
        out = SESleepState::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SESleepState::Invalid;
    }
  }
  void Physiology::Marshall(const SESleepState& in, CDM::enumSleepState& out)
  {
    switch (in) {
    case SESleepState::Sleeping:
      out = CDM::enumSleepState::Asleep;
      break;
    case SESleepState::Awake:
      out = CDM::enumSleepState::Awake;
      break;
    default:
      out = "";
      break;
    }
  }
  //----------------------------------------------------------------------------------
}

bool operator==(CDM::enumInflammationSource const& lhs, SEInflammationSource const& rhs)
{
  switch (rhs) {
  case SEInflammationSource::Burn:
    return (CDM::enumInflammationSource::Burn == lhs);
  case SEInflammationSource::Hemorrhage:
    return (CDM::enumInflammationSource::Hemorrhage == lhs);
  case SEInflammationSource::Infection:
    return (CDM::enumInflammationSource::Infection == lhs);
  case SEInflammationSource::Ebola:
    return (CDM::enumInflammationSource::Ebola == lhs);
  case SEInflammationSource::Invalid:
    return ((CDM::enumInflammationSource::value)-1 == lhs);
  default:
    return false;
  }
}
bool operator==(CDM::enumHeartRhythm const& lhs, SEHeartRhythm const& rhs)
{
  switch (rhs) {
  case SEHeartRhythm::Asystole:
    return (CDM::enumHeartRhythm::Asystole == lhs);
  case SEHeartRhythm::NormalSinus:
    return (CDM::enumHeartRhythm::NormalSinus == lhs);
  case SEHeartRhythm::Invalid:
    return ((CDM::enumHeartRhythm::value)-1 == lhs);
  default:
    return false;
  }
}
bool operator==(CDM::enumSleepState const& lhs, SESleepState const& rhs)
{
  switch (rhs) {
  case SESleepState::Awake:
    return (CDM::enumSleepState::Awake == lhs);
  case SESleepState::Sleeping:
    return (CDM::enumSleepState::Asleep == lhs);
  case SESleepState::Invalid:
    return ((CDM::enumSleepState::value)-1 == lhs);
  default:
    return false;
  }
}
}
