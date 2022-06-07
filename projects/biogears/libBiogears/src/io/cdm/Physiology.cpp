#include "Physiology.h"

#include "Property.h"
#include "System.h"

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
#include <biogears/cdm/properties/SEScalarOsmolality.h>
#include <biogears/cdm/properties/SEScalarOsmolarity.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressureTimePerVolumeArea.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeArea.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>

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
  //class SEPupillaryResponse
  void Physiology::Marshall(const CDM::PupillaryResponseData& in, SEPupillaryResponse& out)
  {
    io::Property::Marshall(in.ReactivityModifier(), out.GetReactivityModifier());
    io::Property::Marshall(in.ShapeModifier(), out.GetShapeModifier());
    io::Property::Marshall(in.SizeModifier(), out.GetSizeModifier());
  }
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SEPupillaryResponse& in, CDM::PupillaryResponseData& out)
  {
    if (in.m_ReactivityModifier != nullptr) {
      io::Property::UnMarshall(*in.m_ReactivityModifier, out.ReactivityModifier());
    }
    if (in.m_ShapeModifier != nullptr) {
      io::Property::UnMarshall(*in.m_ShapeModifier, out.ShapeModifier());
    }
    if (in.m_SizeModifier != nullptr) {
      io::Property::UnMarshall(*in.m_SizeModifier, out.SizeModifier());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEInflammationState
  void Physiology::Marshall(const CDM::InflammatoryResponseData& in, SEInflammatoryResponse& out)
  {
    io::Property::Marshall(in.LocalPathogen(), out.GetLocalPathogen());
    io::Property::Marshall(in.Trauma(), out.GetTrauma());
    io::Property::Marshall(in.MacrophageResting(), out.GetMacrophageResting());
    io::Property::Marshall(in.MacrophageActive(), out.GetMacrophageActive());
    io::Property::Marshall(in.NeutrophilResting(), out.GetNeutrophilResting());
    io::Property::Marshall(in.NeutrophilActive(), out.GetNeutrophilActive());
    io::Property::Marshall(in.InducibleNOSPre(), out.GetInducibleNOSPre());
    io::Property::Marshall(in.InducibleNOS(), out.GetInducibleNOS());
    io::Property::Marshall(in.ConstitutiveNOS(), out.GetConstitutiveNOS());
    io::Property::Marshall(in.Nitrate(), out.GetNitrate());
    io::Property::Marshall(in.NitricOxide(), out.GetNitricOxide());
    io::Property::Marshall(in.TumorNecrosisFactor(), out.GetTumorNecrosisFactor());
    io::Property::Marshall(in.Interleukin6(), out.GetInterleukin6());
    io::Property::Marshall(in.Interleukin10(), out.GetInterleukin10());
    io::Property::Marshall(in.Interleukin12(), out.GetInterleukin12());
    io::Property::Marshall(in.Catecholamines(), out.GetCatecholamines());
    io::Property::Marshall(in.TissueIntegrity(), out.GetTissueIntegrity());
    for (auto src : in.Source()) {
      out.m_InflammationSources.push_back(src);
    }
  }
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SEInflammatoryResponse& in, CDM::InflammatoryResponseData& out)
  {
    io::Property::UnMarshall(*in.m_LocalPathogen, out.LocalPathogen());
    io::Property::UnMarshall(*in.m_Trauma, out.Trauma());
    io::Property::UnMarshall(*in.m_MacrophageResting, out.MacrophageResting());
    io::Property::UnMarshall(*in.m_MacrophageActive, out.MacrophageActive());
    io::Property::UnMarshall(*in.m_NeutrophilResting, out.NeutrophilResting());
    io::Property::UnMarshall(*in.m_NeutrophilActive, out.NeutrophilActive());
    io::Property::UnMarshall(*in.m_InducibleNOSPre, out.InducibleNOSPre());
    io::Property::UnMarshall(*in.m_InducibleNOS, out.InducibleNOS());
    io::Property::UnMarshall(*in.m_ConstitutiveNOS, out.ConstitutiveNOS());
    io::Property::UnMarshall(*in.m_Nitrate, out.Nitrate());
    io::Property::UnMarshall(*in.m_NitricOxide, out.NitricOxide());
    io::Property::UnMarshall(*in.m_TumorNecrosisFactor, out.TumorNecrosisFactor());
    io::Property::UnMarshall(*in.m_Interleukin6, out.Interleukin6());
    io::Property::UnMarshall(*in.m_Interleukin10, out.Interleukin10());
    io::Property::UnMarshall(*in.m_Interleukin12, out.Interleukin12());
    io::Property::UnMarshall(*in.m_Catecholamines, out.Catecholamines());
    io::Property::UnMarshall(*in.m_TissueIntegrity, out.TissueIntegrity());
    for (auto src : in.m_InflammationSources) {
      out.Source().push_back(src);
    }
  }
  //----------------------------------------------------------------------------------
  //class SEBloodChemistrySystem
  void Physiology::Marshall(const CDM::BloodChemistrySystemData& in, SEBloodChemistrySystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.ArterialBloodPH(), out.GetArterialBloodPH());
    io::Property::Marshall(in.ArterialBloodPHBaseline(), out.GetArterialBloodPHBaseline());
    io::Property::Marshall(in.BloodDensity(), out.GetBloodDensity());
    io::Property::Marshall(in.BloodSpecificHeat(), out.GetBloodSpecificHeat());
    io::Property::Marshall(in.BloodUreaNitrogenConcentration(), out.GetBloodUreaNitrogenConcentration());
    io::Property::Marshall(in.CarbonDioxideSaturation(), out.GetCarbonDioxideSaturation());
    io::Property::Marshall(in.CarbonMonoxideSaturation(), out.GetCarbonMonoxideSaturation());
    io::Property::Marshall(in.Hematocrit(), out.GetHematocrit());
    io::Property::Marshall(in.HemoglobinContent(), out.GetHemoglobinContent());
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
    io::Property::Marshall(in.VenousBloodPH(), out.GetVenousBloodPH());
    io::Property::Marshall(in.VolumeFractionNeutralPhospholipidInPlasma(), out.GetVolumeFractionNeutralPhospholipidInPlasma());
    io::Property::Marshall(in.VolumeFractionNeutralLipidInPlasma(), out.GetVolumeFractionNeutralLipidInPlasma());
    io::Property::Marshall(in.WhiteBloodCellCount(), out.GetWhiteBloodCellCount());
    io::Property::Marshall(in.ArterialCarbonDioxidePressure(), out.GetArterialCarbonDioxidePressure());
    io::Property::Marshall(in.ArterialOxygenPressure(), out.GetArterialOxygenPressure());
    io::Property::Marshall(in.PulmonaryArterialCarbonDioxidePressure(), out.GetPulmonaryArterialCarbonDioxidePressure());
    io::Property::Marshall(in.PulmonaryArterialOxygenPressure(), out.GetPulmonaryArterialOxygenPressure());
    io::Property::Marshall(in.PulmonaryVenousOxygenPressure(), out.GetPulmonaryVenousOxygenPressure());
    io::Property::Marshall(in.PulmonaryVenousCarbonDioxidePressure(), out.GetPulmonaryVenousCarbonDioxidePressure());
    io::Property::Marshall(in.VenousCarbonDioxidePressure(), out.GetVenousCarbonDioxidePressure());
    io::Property::Marshall(in.VenousOxygenPressure(), out.GetVenousOxygenPressure());
    io::Property::Marshall(in.VenousBloodPH(), out.GetVenousBloodPH());
    io::Property::Marshall(in.ViralLoad(), out.GetViralLoad());
    Marshall(in.InflammatoryResponse(), out.GetInflammatoryResponse());
  }
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SEBloodChemistrySystem& in, CDM::BloodChemistrySystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    if (in.m_ArterialBloodPH != nullptr) {
      io::Property::UnMarshall(*in.m_ArterialBloodPH, out.ArterialBloodPH());
    }
    if (in.m_ArterialBloodPHBaseline != nullptr) {
      io::Property::UnMarshall(*in.m_ArterialBloodPHBaseline, out.ArterialBloodPHBaseline());
    }
    if (in.m_BloodDensity != nullptr) {
      io::Property::UnMarshall(*in.m_BloodDensity, out.BloodDensity());
    }
    if (in.m_BloodSpecificHeat != nullptr) {
      io::Property::UnMarshall(*in.m_BloodSpecificHeat, out.BloodSpecificHeat());
    }
    if (in.m_BloodUreaNitrogenConcentration != nullptr) {
      io::Property::UnMarshall(*in.m_BloodUreaNitrogenConcentration, out.BloodUreaNitrogenConcentration());
    }
    if (in.m_CarbonDioxideSaturation != nullptr) {
      io::Property::UnMarshall(*in.m_CarbonDioxideSaturation, out.CarbonDioxideSaturation());
    }
    if (in.m_CarbonMonoxideSaturation != nullptr) {
      io::Property::UnMarshall(*in.m_CarbonMonoxideSaturation, out.CarbonMonoxideSaturation());
    }
    if (in.m_Hematocrit != nullptr) {
      io::Property::UnMarshall(*in.m_Hematocrit, out.Hematocrit());
    }
    if (in.m_HemoglobinContent != nullptr) {
      io::Property::UnMarshall(*in.m_HemoglobinContent, out.HemoglobinContent());
    }
    if (in.m_OxygenSaturation != nullptr) {
      io::Property::UnMarshall(*in.m_OxygenSaturation, out.OxygenSaturation());
    }
    if (in.m_OxygenVenousSaturation != nullptr) {
      io::Property::UnMarshall(*in.m_OxygenVenousSaturation, out.OxygenVenousSaturation());
    }
    if (in.m_Phosphate != nullptr) {
      io::Property::UnMarshall(*in.m_Phosphate, out.Phosphate());
    }
    if (in.m_PlasmaVolume != nullptr) {
      io::Property::UnMarshall(*in.m_PlasmaVolume, out.PlasmaVolume());
    }
    if (in.m_PulseOximetry != nullptr) {
      io::Property::UnMarshall(*in.m_PulseOximetry, out.PulseOximetry());
    }
    if (in.m_RedBloodCellAcetylcholinesterase != nullptr) {
      io::Property::UnMarshall(*in.m_RedBloodCellAcetylcholinesterase, out.RedBloodCellAcetylcholinesterase());
    }
    if (in.m_RedBloodCellCount != nullptr) {
      io::Property::UnMarshall(*in.m_RedBloodCellCount, out.RedBloodCellCount());
    }
    if (in.m_ShuntFraction != nullptr) {
      io::Property::UnMarshall(*in.m_ShuntFraction, out.ShuntFraction());
    }
    if (in.m_StrongIonDifference != nullptr) {
      io::Property::UnMarshall(*in.m_StrongIonDifference, out.StrongIonDifference());
    }
    if (in.m_TotalBilirubin != nullptr) {
      io::Property::UnMarshall(*in.m_TotalBilirubin, out.TotalBilirubin());
    }
    if (in.m_TotalProteinConcentration != nullptr) {
      io::Property::UnMarshall(*in.m_TotalProteinConcentration, out.TotalProteinConcentration());
    }
    if (in.m_VolumeFractionNeutralPhospholipidInPlasma != nullptr) {
      io::Property::UnMarshall(*in.m_VolumeFractionNeutralPhospholipidInPlasma, out.VolumeFractionNeutralPhospholipidInPlasma());
    }
    if (in.m_VolumeFractionNeutralLipidInPlasma != nullptr) {
      io::Property::UnMarshall(*in.m_VolumeFractionNeutralLipidInPlasma, out.VolumeFractionNeutralLipidInPlasma());
    }
    if (in.m_WhiteBloodCellCount != nullptr) {
      io::Property::UnMarshall(*in.m_WhiteBloodCellCount, out.WhiteBloodCellCount());
    }
    if (in.m_ArterialCarbonDioxidePressure != nullptr) {
      io::Property::UnMarshall(*in.m_ArterialCarbonDioxidePressure, out.ArterialCarbonDioxidePressure());
    }
    if (in.m_ArterialOxygenPressure != nullptr) {
      io::Property::UnMarshall(*in.m_ArterialOxygenPressure, out.ArterialOxygenPressure());
    }
    if (in.m_PulmonaryArterialCarbonDioxidePressure != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryArterialCarbonDioxidePressure, out.PulmonaryArterialCarbonDioxidePressure());
    }
    if (in.m_PulmonaryArterialOxygenPressure != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryArterialOxygenPressure, out.PulmonaryArterialOxygenPressure());
    }
    if (in.m_PulmonaryVenousCarbonDioxidePressure != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryVenousCarbonDioxidePressure, out.PulmonaryVenousCarbonDioxidePressure());
    }
    if (in.m_PulmonaryVenousOxygenPressure != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryVenousOxygenPressure, out.PulmonaryVenousOxygenPressure());
    }
    if (in.m_VenousCarbonDioxidePressure != nullptr) {
      io::Property::UnMarshall(*in.m_VenousCarbonDioxidePressure, out.VenousCarbonDioxidePressure());
    }
    if (in.m_VenousBloodPH != nullptr) {
      io::Property::UnMarshall(*in.m_VenousBloodPH, out.VenousBloodPH());
    }
    if (in.m_VenousOxygenPressure != nullptr) {
      io::Property::UnMarshall(*in.m_VenousOxygenPressure, out.VenousOxygenPressure());
    }
    if (in.m_ViralLoad != nullptr) {
      io::Property::UnMarshall(*in.m_ViralLoad, out.ViralLoad());
    }
    if (in.m_InflammatoryResponse != nullptr) {
      UnMarshall(*in.m_InflammatoryResponse, out.InflammatoryResponse());
    }
  }
  //----------------------------------------------------------------------------------
  //class SECardiovascularSystem
  void Physiology::Marshall(const CDM::CardiovascularSystemData& in, SECardiovascularSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.ArterialPressure(), out.GetArterialPressure());
    io::Property::Marshall(in.BloodVolume(), out.GetBloodVolume());
    io::Property::Marshall(in.CardiacIndex(), out.GetCardiacIndex());
    io::Property::Marshall(in.CardiacOutput(), out.GetCardiacOutput());
    io::Property::Marshall(in.CentralVenousPressure(), out.GetCentralVenousPressure());
    io::Property::Marshall(in.CerebralBloodFlow(), out.GetCerebralBloodFlow());
    io::Property::Marshall(in.CerebralPerfusionPressure(), out.GetCerebralPerfusionPressure());
    io::Property::Marshall(in.DiastolicArterialPressure(), out.GetDiastolicArterialPressure());
    io::Property::Marshall(in.HeartEjectionFraction(), out.GetHeartEjectionFraction());
    io::Property::Marshall(in.HeartRate(), out.GetHeartRate());
    if (in.HeartRhythm().present()) {
      out.SetHeartRhythm(in.HeartRhythm().get());
    }
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
    io::Property::Marshall(in.SystemicVascularResistance(), out.GetSystemicVascularResistance());
    io::Property::Marshall(in.SystolicArterialPressure(), out.GetSystolicArterialPressure());
  }
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SECardiovascularSystem& in, CDM::CardiovascularSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    if (in.m_ArterialPressure != nullptr) {
      io::Property::UnMarshall(*in.m_ArterialPressure, out.ArterialPressure());
    }
    if (in.m_BloodVolume != nullptr) {
      io::Property::UnMarshall(*in.m_BloodVolume, out.BloodVolume());
    }
    if (in.m_CardiacIndex != nullptr) {
      io::Property::UnMarshall(*in.m_CardiacIndex, out.CardiacIndex());
    }
    if (in.m_CardiacOutput != nullptr) {
      io::Property::UnMarshall(*in.m_CardiacOutput, out.CardiacOutput());
    }
    if (in.m_CentralVenousPressure != nullptr) {
      io::Property::UnMarshall(*in.m_CentralVenousPressure, out.CentralVenousPressure());
    }
    if (in.m_CerebralBloodFlow != nullptr) {
      io::Property::UnMarshall(*in.m_CerebralBloodFlow, out.CerebralBloodFlow());
    }
    if (in.m_CerebralPerfusionPressure != nullptr) {
      io::Property::UnMarshall(*in.m_CerebralPerfusionPressure, out.CerebralPerfusionPressure());
    }
    if (in.m_DiastolicArterialPressure != nullptr) {
      io::Property::UnMarshall(*in.m_DiastolicArterialPressure, out.DiastolicArterialPressure());
    }
    if (in.m_HeartEjectionFraction != nullptr) {
      io::Property::UnMarshall(*in.m_HeartEjectionFraction, out.HeartEjectionFraction());
    }
    if (in.m_HeartRate != nullptr) {
      io::Property::UnMarshall(*in.m_HeartRate, out.HeartRate());
    }
    if (in.HasHeartRhythm()) {
      out.HeartRhythm(in.m_HeartRhythm);
    }
    if (in.m_HeartStrokeVolume != nullptr) {
      io::Property::UnMarshall(*in.m_HeartStrokeVolume, out.HeartStrokeVolume());
    }
    if (in.m_IntracranialPressure != nullptr) {
      io::Property::UnMarshall(*in.m_IntracranialPressure, out.IntracranialPressure());
    }
    if (in.m_MeanArterialPressure != nullptr) {
      io::Property::UnMarshall(*in.m_MeanArterialPressure, out.MeanArterialPressure());
    }
    if (in.m_MeanArterialCarbonDioxidePartialPressure != nullptr) {
      io::Property::UnMarshall(*in.m_MeanArterialCarbonDioxidePartialPressure, out.MeanArterialCarbonDioxidePartialPressure());
    }
    if (in.m_MeanArterialCarbonDioxidePartialPressureDelta != nullptr) {
      io::Property::UnMarshall(*in.m_MeanArterialCarbonDioxidePartialPressureDelta, out.MeanArterialCarbonDioxidePartialPressureDelta());
    }
    if (in.m_MeanCentralVenousPressure != nullptr) {
      io::Property::UnMarshall(*in.m_MeanCentralVenousPressure, out.MeanCentralVenousPressure());
    }
    if (in.m_MeanSkinFlow != nullptr) {
      io::Property::UnMarshall(*in.m_MeanSkinFlow, out.MeanSkinFlow());
    }
    if (in.m_PulmonaryArterialPressure != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryArterialPressure, out.PulmonaryArterialPressure());
    }
    if (in.m_PulmonaryCapillariesWedgePressure != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryCapillariesWedgePressure, out.PulmonaryCapillariesWedgePressure());
    }
    if (in.m_PulmonaryDiastolicArterialPressure != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryDiastolicArterialPressure, out.PulmonaryDiastolicArterialPressure());
    }
    if (in.m_PulmonaryMeanArterialPressure != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryMeanArterialPressure, out.PulmonaryMeanArterialPressure());
    }
    if (in.m_PulmonaryMeanCapillaryFlow != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryMeanCapillaryFlow, out.PulmonaryMeanCapillaryFlow());
    }
    if (in.m_PulmonaryMeanShuntFlow != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryMeanShuntFlow, out.PulmonaryMeanShuntFlow());
    }
    if (in.m_PulmonarySystolicArterialPressure != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonarySystolicArterialPressure, out.PulmonarySystolicArterialPressure());
    }
    if (in.m_PulmonaryVascularResistance != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryVascularResistance, out.PulmonaryVascularResistance());
    }
    if (in.m_PulmonaryVascularResistanceIndex != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryVascularResistanceIndex, out.PulmonaryVascularResistanceIndex());
    }
    if (in.m_PulsePressure != nullptr) {
      io::Property::UnMarshall(*in.m_PulsePressure, out.PulsePressure());
    }
    if (in.m_SystemicVascularResistance != nullptr) {
      io::Property::UnMarshall(*in.m_SystemicVascularResistance, out.SystemicVascularResistance());
    }
    if (in.m_SystolicArterialPressure != nullptr) {
      io::Property::UnMarshall(*in.m_SystolicArterialPressure, out.SystolicArterialPressure());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEDrugSystem
  void Physiology::Marshall(const CDM::DrugSystemData& in, SEDrugSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.BronchodilationLevel(), out.GetBronchodilationLevel());
    io::Property::Marshall(in.HeartRateChange(), out.GetHeartRateChange());
    io::Property::Marshall(in.MeanBloodPressureChange(), out.GetMeanBloodPressureChange());
    io::Property::Marshall(in.NeuromuscularBlockLevel(), out.GetNeuromuscularBlockLevel());
    io::Property::Marshall(in.PulsePressureChange(), out.GetPulsePressureChange());

    io::Property::Marshall(in.RespirationRateChange(), out.GetRespirationRateChange());
    io::Property::Marshall(in.SedationLevel(), out.GetSedationLevel());
    io::Property::Marshall(in.TidalVolumeChange(), out.GetTidalVolumeChange());
    io::Property::Marshall(in.TubularPermeabilityChange(), out.GetTubularPermeabilityChange());
    io::Property::Marshall(in.CentralNervousResponse(), out.GetCentralNervousResponse());
  }
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SEDrugSystem& in, CDM::DrugSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    if (in.m_BronchodilationLevel != nullptr) {
      io::Property::UnMarshall(*in.m_BronchodilationLevel, out.BronchodilationLevel());
    }
    if (in.m_HeartRateChange != nullptr) {
      io::Property::UnMarshall(*in.m_HeartRateChange, out.HeartRateChange());
    }
    if (in.m_MeanBloodPressureChange != nullptr) {
      io::Property::UnMarshall(*in.m_MeanBloodPressureChange, out.MeanBloodPressureChange());
    }
    if (in.m_NeuromuscularBlockLevel != nullptr) {
      io::Property::UnMarshall(*in.m_NeuromuscularBlockLevel, out.NeuromuscularBlockLevel());
    }
    if (in.m_PulsePressureChange != nullptr) {
      io::Property::UnMarshall(*in.m_PulsePressureChange, out.PulsePressureChange());
    }
    if (in.m_RespirationRateChange != nullptr) {
      io::Property::UnMarshall(*in.m_RespirationRateChange, out.RespirationRateChange());
    }
    if (in.m_SedationLevel != nullptr) {
      io::Property::UnMarshall(*in.m_SedationLevel, out.SedationLevel());
    }
    if (in.m_TidalVolumeChange != nullptr) {
      io::Property::UnMarshall(*in.m_TidalVolumeChange, out.TidalVolumeChange());
    }
    if (in.m_TubularPermeabilityChange != nullptr) {
      io::Property::UnMarshall(*in.m_TubularPermeabilityChange, out.TubularPermeabilityChange());
    }
    if (in.m_CentralNervousResponse != nullptr) {
      io::Property::UnMarshall(*in.m_CentralNervousResponse, out.CentralNervousResponse());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEEndocrineSystem
  void Physiology::Marshall(const CDM::EndocrineSystemData& in, SEEndocrineSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.InsulinSynthesisRate(), out.GetInsulinSynthesisRate());
    io::Property::Marshall(in.GlucagonSynthesisRate(), out.GetGlucagonSynthesisRate());
  }
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SEEndocrineSystem& in, CDM::EndocrineSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    if (in.m_InsulinSynthesisRate != nullptr) {
      io::Property::UnMarshall(*in.m_InsulinSynthesisRate, out.InsulinSynthesisRate());
    }
    if (in.m_GlucagonSynthesisRate != nullptr) {
      io::Property::UnMarshall(*in.m_GlucagonSynthesisRate, out.GlucagonSynthesisRate());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEEnergySystem
  void Physiology::Marshall(const CDM::EnergySystemData& in, SEEnergySystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.AchievedExerciseLevel(), out.GetAchievedExerciseLevel());
    io::Property::Marshall(in.ChlorideLostToSweat(), out.GetChlorideLostToSweat());
    io::Property::Marshall(in.CoreTemperature(), out.GetCoreTemperature());
    io::Property::Marshall(in.CreatinineProductionRate(), out.GetCreatinineProductionRate());
    io::Property::Marshall(in.EnergyDeficit(), out.GetEnergyDeficit());
    io::Property::Marshall(in.ExerciseMeanArterialPressureDelta(), out.GetExerciseMeanArterialPressureDelta());
    io::Property::Marshall(in.FatigueLevel(), out.GetFatigueLevel());
    io::Property::Marshall(in.LactateProductionRate(), out.GetLactateProductionRate());
    io::Property::Marshall(in.PotassiumLostToSweat(), out.GetPotassiumLostToSweat());
    io::Property::Marshall(in.SkinTemperature(), out.GetSkinTemperature());
    io::Property::Marshall(in.SodiumLostToSweat(), out.GetSodiumLostToSweat());
    io::Property::Marshall(in.SweatRate(), out.GetSweatRate());
    io::Property::Marshall(in.TotalMetabolicRate(), out.GetTotalMetabolicRate());
    io::Property::Marshall(in.TotalWorkRateLevel(), out.GetTotalWorkRateLevel());
  }
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SEEnergySystem& in, CDM::EnergySystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    if (in.m_AchievedExerciseLevel != nullptr) {
      io::Property::UnMarshall(*in.m_AchievedExerciseLevel, out.AchievedExerciseLevel());
    }
    if (in.m_ChlorideLostToSweat != nullptr) {
      io::Property::UnMarshall(*in.m_ChlorideLostToSweat, out.ChlorideLostToSweat());
    }
    if (in.m_CoreTemperature != nullptr) {
      io::Property::UnMarshall(*in.m_CoreTemperature, out.CoreTemperature());
    }
    if (in.m_CreatinineProductionRate != nullptr) {
      io::Property::UnMarshall(*in.m_CreatinineProductionRate, out.CreatinineProductionRate());
    }
    if (in.m_EnergyDeficit != nullptr) {
      io::Property::UnMarshall(*in.m_EnergyDeficit, out.EnergyDeficit());
    }
    if (in.m_ExerciseMeanArterialPressureDelta != nullptr) {
      io::Property::UnMarshall(*in.m_ExerciseMeanArterialPressureDelta, out.ExerciseMeanArterialPressureDelta());
    }
    if (in.m_FatigueLevel != nullptr) {
      io::Property::UnMarshall(*in.m_FatigueLevel, out.FatigueLevel());
    }
    if (in.m_LactateProductionRate != nullptr) {
      io::Property::UnMarshall(*in.m_LactateProductionRate, out.LactateProductionRate());
    }
    if (in.m_PotassiumLostToSweat != nullptr) {
      io::Property::UnMarshall(*in.m_PotassiumLostToSweat, out.PotassiumLostToSweat());
    }
    if (in.m_SkinTemperature != nullptr) {
      io::Property::UnMarshall(*in.m_SkinTemperature, out.SkinTemperature());
    }
    if (in.m_SodiumLostToSweat != nullptr) {
      io::Property::UnMarshall(*in.m_SodiumLostToSweat, out.SodiumLostToSweat());
    }
    if (in.m_SweatRate != nullptr) {
      io::Property::UnMarshall(*in.m_SweatRate, out.SweatRate());
    }
    if (in.m_TotalMetabolicRate != nullptr) {
      io::Property::UnMarshall(*in.m_TotalMetabolicRate, out.TotalMetabolicRate());
    }
    if (in.m_TotalWorkRateLevel != nullptr) {
      io::Property::UnMarshall(*in.m_TotalWorkRateLevel, out.TotalWorkRateLevel());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEGastrointestinalSystem
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
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SEGastrointestinalSystem& in, CDM::GastrointestinalSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    if (in.m_ChymeAbsorptionRate != nullptr) {
      io::Property::UnMarshall(*in.m_ChymeAbsorptionRate, out.ChymeAbsorptionRate());
    }
    if (in.m_StomachContents != nullptr) {
      out.StomachContents(std::unique_ptr<CDM::NutritionData>(in.m_StomachContents->Unload()));
    }
  }
  //----------------------------------------------------------------------------------
  //class SEHepaticSystem
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
    if (in.m_KetoneProductionRate != nullptr) {
      io::Property::UnMarshall(*in.m_KetoneProductionRate, out.KetoneProductionRate());
    }
    if (in.m_HepaticGluconeogenesisRate != nullptr) {
      io::Property::UnMarshall(*in.m_HepaticGluconeogenesisRate, out.HepaticGluconeogenesisRate());
    }
  }
  //----------------------------------------------------------------------------------
  //class SENervousSystem
  void Physiology::Marshall(const CDM::NervousSystemData& in, SENervousSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
    io::Property::Marshall(in.AttentionLapses(), out.GetAttentionLapses());
    io::Property::Marshall(in.BiologicalDebt(), out.GetBiologicalDebt());
    io::Property::Marshall(in.HeartRateScale(), out.GetHeartRateScale());
    io::Property::Marshall(in.HeartElastanceScale(), out.GetHeartElastanceScale());
    io::Property::Marshall(in.ReactionTime(), out.GetReactionTime());
    io::Property::Marshall(in.ResistanceScaleExtrasplanchnic(), out.GetResistanceScaleExtrasplanchnic());
    io::Property::Marshall(in.ResistanceScaleMuscle(), out.GetResistanceScaleMuscle());
    io::Property::Marshall(in.ResistanceScaleMyocardium(), out.GetResistanceScaleMyocardium());
    io::Property::Marshall(in.ResistanceScaleSplanchnic(), out.GetResistanceScaleSplanchnic());
    io::Property::Marshall(in.ComplianceScale(), out.GetComplianceScale());
    io::Property::Marshall(in.PainVisualAnalogueScale(), out.GetPainVisualAnalogueScale());
    Marshall(in.LeftEyePupillaryResponse(), out.GetLeftEyePupillaryResponse());
    io::Property::Marshall(in.RichmondAgitationSedationScale(), out.GetRichmondAgitationSedationScale());
    Marshall(in.RightEyePupillaryResponse(), out.GetRightEyePupillaryResponse());
    io::Property::Marshall(in.SleepTime(), out.GetSleepTime());
    out.SetSleepState(in.SleepState().get());
    io::Property::Marshall(in.WakeTime(), out.GetWakeTime());
  }
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SENervousSystem& in, CDM::NervousSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    if (in.m_AttentionLapses != nullptr)
      io::Property::UnMarshall(*in.m_AttentionLapses, out.AttentionLapses());
    if (in.m_BiologicalDebt != nullptr)
      io::Property::UnMarshall(*in.m_BiologicalDebt, out.BiologicalDebt());
    if (in.m_HeartRateScale != nullptr)
      io::Property::UnMarshall(*in.m_HeartRateScale, out.HeartRateScale());
    if (in.m_HeartElastanceScale != nullptr)
      io::Property::UnMarshall(*in.m_HeartElastanceScale, out.HeartElastanceScale());
    if (in.m_ReactionTime != nullptr)
      io::Property::UnMarshall(*in.m_ReactionTime, out.ReactionTime());
    if (in.m_ResistanceScaleExtrasplanchnic != nullptr)
      io::Property::UnMarshall(*in.m_ResistanceScaleExtrasplanchnic, out.ResistanceScaleExtrasplanchnic());
    if (in.m_ResistanceScaleMuscle != nullptr)
      io::Property::UnMarshall(*in.m_ResistanceScaleMuscle, out.ResistanceScaleMuscle());
    if (in.m_ResistanceScaleMyocardium != nullptr)
      io::Property::UnMarshall(*in.m_ResistanceScaleMyocardium, out.ResistanceScaleMyocardium());
    if (in.m_ResistanceScaleSplanchnic != nullptr)
      io::Property::UnMarshall(*in.m_ResistanceScaleSplanchnic, out.ResistanceScaleSplanchnic());
    if (in.m_ComplianceScale != nullptr)
      io::Property::UnMarshall(*in.m_ComplianceScale, out.ComplianceScale());
    if (in.m_PainVisualAnalogueScale != nullptr)
      io::Property::UnMarshall(*in.m_PainVisualAnalogueScale, out.PainVisualAnalogueScale());
    if (in.m_LeftEyePupillaryResponse != nullptr)
      UnMarshall(*in.m_LeftEyePupillaryResponse, out.LeftEyePupillaryResponse());
    if (in.m_RichmondAgitationSedationScale != nullptr)
      io::Property::UnMarshall(*in.m_RichmondAgitationSedationScale, out.RichmondAgitationSedationScale());
    if (in.m_RightEyePupillaryResponse != nullptr)
      UnMarshall(*in.m_RightEyePupillaryResponse, out.RightEyePupillaryResponse());
    if (in.m_SleepTime != nullptr)
      io::Property::UnMarshall(*in.m_SleepTime, out.SleepTime());
    if (in.HasSleepState())
      out.SleepState(in.m_SleepState);
    if (in.m_WakeTime != nullptr)
      io::Property::UnMarshall(*in.m_WakeTime, out.WakeTime());
  }
  //----------------------------------------------------------------------------------
  //class SERenalSystem
  void Physiology::Marshall(const CDM::RenalSystemData& in, SERenalSystem& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
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
    io::Property::Marshall(in.UrineSpecificGravity(), out.GetUrineSpecificGravity());
    io::Property::Marshall(in.UrineVolume(), out.GetUrineVolume());
    io::Property::Marshall(in.UrineUreaNitrogenConcentration(), out.GetUrineUreaNitrogenConcentration());
  }
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SERenalSystem& in, CDM::RenalSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    if (in.m_GlomerularFiltrationRate != nullptr) {
      io::Property::UnMarshall(*in.m_GlomerularFiltrationRate, out.GlomerularFiltrationRate());
    }
    if (in.m_FiltrationFraction != nullptr) {
      io::Property::UnMarshall(*in.m_FiltrationFraction, out.FiltrationFraction());
    }

    if (in.m_LeftAfferentArterioleResistance != nullptr) {
      io::Property::UnMarshall(*in.m_LeftAfferentArterioleResistance, out.LeftAfferentArterioleResistance());
    }
    if (in.m_LeftBowmansCapsulesHydrostaticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_LeftBowmansCapsulesHydrostaticPressure, out.LeftBowmansCapsulesHydrostaticPressure());
    }
    if (in.m_LeftBowmansCapsulesOsmoticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_LeftBowmansCapsulesOsmoticPressure, out.LeftBowmansCapsulesOsmoticPressure());
    }
    if (in.m_LeftEfferentArterioleResistance != nullptr) {
      io::Property::UnMarshall(*in.m_LeftEfferentArterioleResistance, out.LeftEfferentArterioleResistance());
    }
    if (in.m_LeftGlomerularCapillariesHydrostaticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_LeftGlomerularCapillariesHydrostaticPressure, out.LeftGlomerularCapillariesHydrostaticPressure());
    }
    if (in.m_LeftGlomerularCapillariesOsmoticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_LeftGlomerularCapillariesOsmoticPressure, out.LeftGlomerularCapillariesOsmoticPressure());
    }
    if (in.m_LeftGlomerularFiltrationCoefficient != nullptr) {
      io::Property::UnMarshall(*in.m_LeftGlomerularFiltrationCoefficient, out.LeftGlomerularFiltrationCoefficient());
    }
    if (in.m_LeftGlomerularFiltrationRate != nullptr) {
      io::Property::UnMarshall(*in.m_LeftGlomerularFiltrationRate, out.LeftGlomerularFiltrationRate());
    }
    if (in.m_LeftGlomerularFiltrationSurfaceArea != nullptr) {
      io::Property::UnMarshall(*in.m_LeftGlomerularFiltrationSurfaceArea, out.LeftGlomerularFiltrationSurfaceArea());
    }
    if (in.m_LeftGlomerularFluidPermeability != nullptr) {
      io::Property::UnMarshall(*in.m_LeftGlomerularFluidPermeability, out.LeftGlomerularFluidPermeability());
    }
    if (in.m_LeftFiltrationFraction != nullptr) {
      io::Property::UnMarshall(*in.m_LeftFiltrationFraction, out.LeftFiltrationFraction());
    }
    if (in.m_LeftNetFiltrationPressure != nullptr) {
      io::Property::UnMarshall(*in.m_LeftNetFiltrationPressure, out.LeftNetFiltrationPressure());
    }
    if (in.m_LeftNetReabsorptionPressure != nullptr) {
      io::Property::UnMarshall(*in.m_LeftNetReabsorptionPressure, out.LeftNetReabsorptionPressure());
    }
    if (in.m_LeftPeritubularCapillariesHydrostaticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_LeftPeritubularCapillariesHydrostaticPressure, out.LeftPeritubularCapillariesHydrostaticPressure());
    }
    if (in.m_LeftPeritubularCapillariesOsmoticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_LeftPeritubularCapillariesOsmoticPressure, out.LeftPeritubularCapillariesOsmoticPressure());
    }
    if (in.m_LeftReabsorptionFiltrationCoefficient != nullptr) {
      io::Property::UnMarshall(*in.m_LeftReabsorptionFiltrationCoefficient, out.LeftReabsorptionFiltrationCoefficient());
    }
    if (in.m_LeftReabsorptionRate != nullptr) {
      io::Property::UnMarshall(*in.m_LeftReabsorptionRate, out.LeftReabsorptionRate());
    }
    if (in.m_LeftTubularReabsorptionFiltrationSurfaceArea != nullptr) {
      io::Property::UnMarshall(*in.m_LeftTubularReabsorptionFiltrationSurfaceArea, out.LeftTubularReabsorptionFiltrationSurfaceArea());
    }
    if (in.m_LeftTubularReabsorptionFluidPermeability != nullptr) {
      io::Property::UnMarshall(*in.m_LeftTubularReabsorptionFluidPermeability, out.LeftTubularReabsorptionFluidPermeability());
    }
    if (in.m_LeftTubularHydrostaticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_LeftTubularHydrostaticPressure, out.LeftTubularHydrostaticPressure());
    }
    if (in.m_LeftTubularOsmoticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_LeftTubularOsmoticPressure, out.LeftTubularOsmoticPressure());
    }

    if (in.m_RenalBloodFlow != nullptr) {
      io::Property::UnMarshall(*in.m_RenalBloodFlow, out.RenalBloodFlow());
    }
    if (in.m_RenalPlasmaFlow != nullptr) {
      io::Property::UnMarshall(*in.m_RenalPlasmaFlow, out.RenalPlasmaFlow());
    }
    if (in.m_RenalVascularResistance != nullptr) {
      io::Property::UnMarshall(*in.m_RenalVascularResistance, out.RenalVascularResistance());
    }

    if (in.m_RightAfferentArterioleResistance != nullptr) {
      io::Property::UnMarshall(*in.m_RightAfferentArterioleResistance, out.RightAfferentArterioleResistance());
    }
    if (in.m_RightBowmansCapsulesHydrostaticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_RightBowmansCapsulesHydrostaticPressure, out.RightBowmansCapsulesHydrostaticPressure());
    }
    if (in.m_RightBowmansCapsulesOsmoticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_RightBowmansCapsulesOsmoticPressure, out.RightBowmansCapsulesOsmoticPressure());
    }
    if (in.m_RightEfferentArterioleResistance != nullptr) {
      io::Property::UnMarshall(*in.m_RightEfferentArterioleResistance, out.RightEfferentArterioleResistance());
    }
    if (in.m_RightGlomerularCapillariesHydrostaticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_RightGlomerularCapillariesHydrostaticPressure, out.RightGlomerularCapillariesHydrostaticPressure());
    }
    if (in.m_RightGlomerularCapillariesOsmoticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_RightGlomerularCapillariesOsmoticPressure, out.RightGlomerularCapillariesOsmoticPressure());
    }
    if (in.m_RightGlomerularFiltrationCoefficient != nullptr) {
      io::Property::UnMarshall(*in.m_RightGlomerularFiltrationCoefficient, out.RightGlomerularFiltrationCoefficient());
    }
    if (in.m_RightGlomerularFiltrationRate != nullptr) {
      io::Property::UnMarshall(*in.m_RightGlomerularFiltrationRate, out.RightGlomerularFiltrationRate());
    }
    if (in.m_RightGlomerularFiltrationSurfaceArea != nullptr) {
      io::Property::UnMarshall(*in.m_RightGlomerularFiltrationSurfaceArea, out.RightGlomerularFiltrationSurfaceArea());
    }
    if (in.m_RightGlomerularFluidPermeability != nullptr) {
      io::Property::UnMarshall(*in.m_RightGlomerularFluidPermeability, out.RightGlomerularFluidPermeability());
    }
    if (in.m_RightFiltrationFraction != nullptr) {
      io::Property::UnMarshall(*in.m_RightFiltrationFraction, out.RightFiltrationFraction());
    }
    if (in.m_RightNetFiltrationPressure != nullptr) {
      io::Property::UnMarshall(*in.m_RightNetFiltrationPressure, out.RightNetFiltrationPressure());
    }
    if (in.m_RightNetReabsorptionPressure != nullptr) {
      io::Property::UnMarshall(*in.m_RightNetReabsorptionPressure, out.RightNetReabsorptionPressure());
    }
    if (in.m_RightPeritubularCapillariesHydrostaticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_RightPeritubularCapillariesHydrostaticPressure, out.RightPeritubularCapillariesHydrostaticPressure());
    }
    if (in.m_RightPeritubularCapillariesOsmoticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_RightPeritubularCapillariesOsmoticPressure, out.RightPeritubularCapillariesOsmoticPressure());
    }
    if (in.m_RightReabsorptionFiltrationCoefficient != nullptr) {
      io::Property::UnMarshall(*in.m_RightReabsorptionFiltrationCoefficient, out.RightReabsorptionFiltrationCoefficient());
    }
    if (in.m_RightReabsorptionRate != nullptr) {
      io::Property::UnMarshall(*in.m_RightReabsorptionRate, out.RightReabsorptionRate());
    }
    if (in.m_RightTubularReabsorptionFiltrationSurfaceArea != nullptr) {
      io::Property::UnMarshall(*in.m_RightTubularReabsorptionFiltrationSurfaceArea, out.RightTubularReabsorptionFiltrationSurfaceArea());
    }
    if (in.m_RightTubularReabsorptionFluidPermeability != nullptr) {
      io::Property::UnMarshall(*in.m_RightTubularReabsorptionFluidPermeability, out.RightTubularReabsorptionFluidPermeability());
    }
    if (in.m_RightTubularHydrostaticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_RightTubularHydrostaticPressure, out.RightTubularHydrostaticPressure());
    }
    if (in.m_RightTubularOsmoticPressure != nullptr) {
      io::Property::UnMarshall(*in.m_RightTubularOsmoticPressure, out.RightTubularOsmoticPressure());
    }

    if (in.m_UrinationRate != nullptr) {
      io::Property::UnMarshall(*in.m_UrinationRate, out.UrinationRate());
    }
    if (in.m_UrineOsmolality != nullptr) {
      io::Property::UnMarshall(*in.m_UrineOsmolality, out.UrineOsmolality());
    }
    if (in.m_UrineOsmolarity != nullptr) {
      io::Property::UnMarshall(*in.m_UrineOsmolarity, out.UrineOsmolarity());
    }
    if (in.m_UrineProductionRate != nullptr) {
      io::Property::UnMarshall(*in.m_UrineProductionRate, out.UrineProductionRate());
    }
    if (in.m_UrineSpecificGravity != nullptr) {
      io::Property::UnMarshall(*in.m_UrineSpecificGravity, out.UrineSpecificGravity());
    }
    if (in.m_UrineVolume != nullptr) {
      io::Property::UnMarshall(*in.m_UrineVolume, out.UrineVolume());
    }
    if (in.m_UrineUreaNitrogenConcentration != nullptr) {
      io::Property::UnMarshall(*in.m_UrineUreaNitrogenConcentration, out.UrineUreaNitrogenConcentration());
    }
  }
  //----------------------------------------------------------------------------------
  //class SERespiratorySystem
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
    io::Property::Marshall(in.PulmonaryCompliance(), out.GetPulmonaryCompliance());
    io::Property::Marshall(in.PulmonaryResistance(), out.GetPulmonaryResistance());
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
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SERespiratorySystem& in, CDM::RespiratorySystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    if (in.m_AlveolarArterialGradient != nullptr) {
      io::Property::UnMarshall(*in.m_AlveolarArterialGradient, out.AlveolarArterialGradient());
    }
    if (in.m_CarricoIndex != nullptr) {
      io::Property::UnMarshall(*in.m_CarricoIndex, out.CarricoIndex());
    }
    if (in.m_EndTidalCarbonDioxideFraction != nullptr) {
      io::Property::UnMarshall(*in.m_EndTidalCarbonDioxideFraction, out.EndTidalCarbonDioxideFraction());
    }
    if (in.m_EndTidalCarbonDioxidePressure != nullptr) {
      io::Property::UnMarshall(*in.m_EndTidalCarbonDioxidePressure, out.EndTidalCarbonDioxidePressure());
    }
    if (in.m_ExpiratoryFlow != nullptr) {
      io::Property::UnMarshall(*in.m_ExpiratoryFlow, out.ExpiratoryFlow());
    }
    if (in.m_InspiratoryExpiratoryRatio != nullptr) {
      io::Property::UnMarshall(*in.m_InspiratoryExpiratoryRatio, out.InspiratoryExpiratoryRatio());
    }
    if (in.m_InspiratoryFlow != nullptr) {
      io::Property::UnMarshall(*in.m_InspiratoryFlow, out.InspiratoryFlow());
    }
    if (in.m_PulmonaryCompliance != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryCompliance, out.PulmonaryCompliance());
    }
    if (in.m_PulmonaryResistance != nullptr) {
      io::Property::UnMarshall(*in.m_PulmonaryResistance, out.PulmonaryResistance());
    }
    if (in.m_RespirationDriverPressure != nullptr) {
      io::Property::UnMarshall(*in.m_RespirationDriverPressure, out.RespirationDriverPressure());
    }
    if (in.m_RespirationMusclePressure != nullptr) {
      io::Property::UnMarshall(*in.m_RespirationMusclePressure, out.RespirationMusclePressure());
    }
    if (in.m_RespirationRate != nullptr) {
      io::Property::UnMarshall(*in.m_RespirationRate, out.RespirationRate());
    }
    if (in.m_SpecificVentilation != nullptr) {
      io::Property::UnMarshall(*in.m_SpecificVentilation, out.SpecificVentilation());
    }
    if (in.m_TargetPulmonaryVentilation != nullptr) {
      io::Property::UnMarshall(*in.m_TargetPulmonaryVentilation, out.TargetPulmonaryVentilation());
    }
    if (in.m_TidalVolume != nullptr) {
      io::Property::UnMarshall(*in.m_TidalVolume, out.TidalVolume());
    }
    if (in.m_TotalAlveolarVentilation != nullptr) {
      io::Property::UnMarshall(*in.m_TotalAlveolarVentilation, out.TotalAlveolarVentilation());
    }
    if (in.m_TotalDeadSpaceVentilation != nullptr) {
      io::Property::UnMarshall(*in.m_TotalDeadSpaceVentilation, out.TotalDeadSpaceVentilation());
    }
    if (in.m_TotalLungVolume != nullptr) {
      io::Property::UnMarshall(*in.m_TotalLungVolume, out.TotalLungVolume());
    }
    if (in.m_TotalPulmonaryVentilation != nullptr) {
      io::Property::UnMarshall(*in.m_TotalPulmonaryVentilation, out.TotalPulmonaryVentilation());
    }
    if (in.m_TranspulmonaryPressure != nullptr) {
      io::Property::UnMarshall(*in.m_TranspulmonaryPressure, out.TranspulmonaryPressure());
    }
  }
  //----------------------------------------------------------------------------------
  //class SETissueSystem
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
  //----------------------------------------------------------------------------------
  void Physiology::UnMarshall(const SETissueSystem& in, CDM::TissueSystemData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));
    if (in.m_CarbonDioxideProductionRate != nullptr) {
      io::Property::UnMarshall(*in.m_CarbonDioxideProductionRate, out.CarbonDioxideProductionRate());
    }
    if (in.m_DehydrationFraction != nullptr) {
      io::Property::UnMarshall(*in.m_DehydrationFraction, out.DehydrationFraction());
    }
    if (in.m_ExtracellularFluidVolume != nullptr) {
      io::Property::UnMarshall(*in.m_ExtracellularFluidVolume, out.ExtracellularFluidVolume());
    }
    if (in.m_ExtravascularFluidVolume != nullptr) {
      io::Property::UnMarshall(*in.m_ExtravascularFluidVolume, out.ExtravascularFluidVolume());
    }
    if (in.m_IntracellularFluidPH != nullptr) {
      io::Property::UnMarshall(*in.m_IntracellularFluidPH, out.IntracellularFluidPH());
    }
    if (in.m_IntracellularFluidVolume != nullptr) {
      io::Property::UnMarshall(*in.m_IntracellularFluidVolume, out.IntracellularFluidVolume());
    }
    if (in.m_TotalBodyFluidVolume != nullptr) {
      io::Property::UnMarshall(*in.m_TotalBodyFluidVolume, out.TotalBodyFluidVolume());
    }
    if (in.m_OxygenConsumptionRate != nullptr) {
      io::Property::UnMarshall(*in.m_OxygenConsumptionRate, out.OxygenConsumptionRate());
    }
    if (in.m_RespiratoryExchangeRatio != nullptr) {
      io::Property::UnMarshall(*in.m_RespiratoryExchangeRatio, out.RespiratoryExchangeRatio());
    }
    if (in.m_LiverInsulinSetPoint != nullptr) {
      io::Property::UnMarshall(*in.m_LiverInsulinSetPoint, out.LiverInsulinSetPoint());
    }
    if (in.m_LiverGlucagonSetPoint != nullptr) {
      io::Property::UnMarshall(*in.m_LiverGlucagonSetPoint, out.LiverGlucagonSetPoint());
    }
    if (in.m_MuscleInsulinSetPoint != nullptr) {
      io::Property::UnMarshall(*in.m_MuscleInsulinSetPoint, out.MuscleInsulinSetPoint());
    }
    if (in.m_MuscleGlucagonSetPoint != nullptr) {
      io::Property::UnMarshall(*in.m_MuscleGlucagonSetPoint, out.MuscleGlucagonSetPoint());
    }
    if (in.m_FatInsulinSetPoint != nullptr) {
      io::Property::UnMarshall(*in.m_FatInsulinSetPoint, out.FatInsulinSetPoint());
    }
    if (in.m_FatGlucagonSetPoint != nullptr) {
      io::Property::UnMarshall(*in.m_FatGlucagonSetPoint, out.FatGlucagonSetPoint());
    }
    if (in.m_LiverGlycogen != nullptr) {
      io::Property::UnMarshall(*in.m_LiverGlycogen, out.LiverGlycogen());
    }
    if (in.m_MuscleGlycogen != nullptr) {
      io::Property::UnMarshall(*in.m_MuscleGlycogen, out.MuscleGlycogen());
    }
    if (in.m_StoredProtein != nullptr) {
      io::Property::UnMarshall(*in.m_StoredProtein, out.StoredProtein());
    }
    if (in.m_StoredFat != nullptr) {
      io::Property::UnMarshall(*in.m_StoredFat, out.StoredFat());
    }
  }
  //----------------------------------------------------------------------------------
}
}
