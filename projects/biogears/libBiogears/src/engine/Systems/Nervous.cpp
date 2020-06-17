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
#include <biogears/engine/Systems/Nervous.h>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFlowElastance.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarPressurePerVolume.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

#pragma warning(disable : 4786)
#pragma warning(disable : 4275)

// #define VERBOSE
namespace biogears {
auto Nervous::make_unique(BioGears& bg) -> std::unique_ptr<Nervous>
{
  return std::unique_ptr<Nervous>(new Nervous(bg));
}

Nervous::Nervous(BioGears& bg)
  : SENervousSystem(bg.GetLogger())
  , m_data(bg)
{
  Clear();
}

Nervous::~Nervous()
{
  Clear();
}

void Nervous::Clear()
{
  SENervousSystem::Clear();

  m_Patient = nullptr;
  m_Succinylcholine = nullptr;
  m_Sarin = nullptr;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Nervous::Initialize()
{
  BioGearsSystem::Initialize();
  m_FeedbackActive = false;
  m_blockActive = false;

  m_AfferentChemoreceptor_Hz = 3.55;
  m_AfferentPulmonaryStretchReceptor_Hz = 12.0;
  m_AorticBaroreceptorStrain = 0.04226;
  m_BaroreceptorOperatingPoint_mmHg = m_data.GetCardiovascular().GetSystolicArterialPressure(PressureUnit::mmHg);
  m_CardiopulmonaryInputBaseline_mmHg = m_data.GetCardiovascular().GetCentralVenousPressure(PressureUnit::mmHg) - (m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::PleuralCavity)->GetPressure(PressureUnit::mmHg) - m_data.GetCompartments().GetGasCompartment(BGE::EnvironmentCompartment::Ambient)->GetPressure(PressureUnit::mmHg));
  m_CardiopulmonaryInput_mmHg = m_CardiopulmonaryInputBaseline_mmHg;
  m_CarotidBaroreceptorStrain = 0.04226;
  m_CentralFrequencyDelta_Per_min = 0.0;
  m_CentralPressureDelta_cmH2O = 0.0;
  m_CerebralArteriesEffectors_Large = std::vector<double>(3);
  m_CerebralArteriesEffectors_Small = std::vector<double>(3);
  m_CerebralOxygenSaturationBaseline = 0.0;
  m_CerebralPerfusionPressureBaseline_mmHg = m_data.GetCardiovascular().GetCerebralPerfusionPressure(PressureUnit::mmHg);
  m_CerebralBloodFlowBaseline_mL_Per_s = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralCapillariesToCerebralVeins1)->GetFlow(VolumePerTimeUnit::mL_Per_s);
  m_CerebralBloodFlowInput_mL_Per_s = m_CerebralBloodFlowBaseline_mL_Per_s;
  m_ChemoreceptorFiringRateSetPoint_Hz = m_AfferentChemoreceptor_Hz;
  m_ComplianceModifier = -0.1;
  m_HeartElastanceModifier = 0.206;
  m_HeartRateModifierSympathetic = -0.233;
  m_HeartRateModifierVagal = 0.582;
  m_HypercapniaThresholdHeart = 0.0;
  m_HypercapniaThresholdPeripheral = 0.0;
  m_HypoxiaThresholdHeart = 0.0;
  m_HypoxiaThresholdPeripheral = 0.0;
  m_HeartOxygenBaseline = m_data.GetCompartments().GetIntracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Myocardium)).GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  m_MeanLungVolume_L = m_data.GetRespiratory().GetTotalLungVolume(VolumeUnit::L);
  m_MuscleOxygenBaseline = m_data.GetCompartments().GetIntracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle)).GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  m_OxygenAutoregulatorHeart = 0.0;
  m_OxygenAutoregulatorMuscle = 0.0;
  m_ResistanceModifierExtrasplanchnic = 1.02;
  m_ResistanceModifierMuscle = 1.02;
  m_ResistanceModifierSplanchnic = 0.24;
  m_PeripheralBloodGasInteractionBaseline_Hz = 0.0;
  m_PeripheralFrequencyDelta_Per_min = 0.0;
  m_PeripheralPressureDelta_cmH2O = 0.0;
  m_SympatheticPeripheralSignalBaseline_Hz = 4.05;
  m_SympatheticSinoatrialSignalBaseline_Hz = 4.5;
  m_SympatheticPeripheralSignalFatigue = 0.0;
  m_VagalSignalBaseline_Hz = 3.75;

  GetHeartRateScale().SetValue(1.0);
  GetHeartElastanceScale().SetValue(1.0);
  GetResistanceScaleExtrasplanchnic().SetValue(1.0);
  GetResistanceScaleMuscle().SetValue(1.0);
  GetResistanceScaleMyocardium().SetValue(1.0);
  GetResistanceScaleSplanchnic().SetValue(1.0);
  GetComplianceScale().SetValue(1.0);
  GetLeftEyePupillaryResponse().GetSizeModifier().SetValue(0);
  GetLeftEyePupillaryResponse().GetReactivityModifier().SetValue(0);
  GetRightEyePupillaryResponse().GetSizeModifier().SetValue(0);
  GetRightEyePupillaryResponse().GetReactivityModifier().SetValue(0);
  GetPainVisualAnalogueScale().SetValue(0.0);
}

bool Nervous::Load(const CDM::BioGearsNervousSystemData& in)
{
  if (!SENervousSystem::Load(in))
    return false;
  BioGearsSystem::LoadState();
  // We assume state have to be after all stabilization
  m_FeedbackActive = true;

  m_AfferentChemoreceptor_Hz = in.AfferentChemoreceptor_Hz();
  m_AfferentPulmonaryStretchReceptor_Hz = in.AfferentPulmonaryStrechReceptor_Hz();
  m_AorticBaroreceptorStrain = in.AorticBaroreceptorStrain();
  m_ArterialCarbonDioxideBaseline_mmHg = in.ArterialCarbonDioxideBaseline_mmHg();
  m_ArterialOxygenBaseline_mmHg = in.ArterialOxygenBaseline_mmHg();
  m_BaroreceptorOperatingPoint_mmHg = in.BaroreceptorOperatingPoint_mmHg();
  m_CardiopulmonaryInputBaseline_mmHg = in.CardiopulmonaryInputBaseline_mmHg();
  m_CardiopulmonaryInput_mmHg = in.CardiopulmonaryInput_mmHg();
  m_CarotidBaroreceptorStrain = in.CarotidBaroreceptorStrain();
  m_CentralFrequencyDelta_Per_min = in.CentralFrequencyDelta_Per_min();
  m_CentralPressureDelta_cmH2O = in.CentralPressureDelta_cmH2O();
  m_CerebralArteriesEffectors_Large.clear();
  for (auto effectorLarge : in.CerebralArteriesEffectors_Large()) {
    m_CerebralArteriesEffectors_Large.push_back(effectorLarge);
  }
  m_CerebralArteriesEffectors_Small.clear();
  for (auto effectorSmall : in.CerebralArteriesEffectors_Small()) {
    m_CerebralArteriesEffectors_Small.push_back(effectorSmall);
  }
  m_CerebralBloodFlowBaseline_mL_Per_s = in.CerebralBloodFlowBaseline_mL_Per_s();
  m_CerebralBloodFlowInput_mL_Per_s = in.CerebralBloodFlowInput_mL_Per_s();
  m_CerebralOxygenSaturationBaseline = in.CerebralOxygenSaturationBaseline();
  m_CerebralPerfusionPressureBaseline_mmHg = in.CerebralPerfusionPressureBaseline_mmHg();
  m_ChemoreceptorFiringRateSetPoint_Hz = in.ChemoreceptorFiringRateSetPoint_Hz();
  m_ComplianceModifier = in.ComplianceModifier();
  m_HeartElastanceModifier = in.HeartElastanceModifier();
  m_HeartOxygenBaseline = in.HeartOxygenBaseline();
  m_HeartRateModifierSympathetic = in.HeartRateModifierSympathetic();
  m_HeartRateModifierVagal = in.HeartRateModifierVagal();
  m_HypercapniaThresholdHeart = in.HypercapniaThresholdHeart();
  m_HypercapniaThresholdPeripheral = in.HypercapniaThresholdPeripheral();
  m_HypoxiaThresholdHeart = in.HypoxiaThresholdHeart();
  m_HypoxiaThresholdPeripheral = in.HypoxiaThresholdPeripheral();
  m_MeanLungVolume_L = in.MeanLungVolume_L();
  m_MuscleOxygenBaseline = in.MuscleOxygenBaseline();
  m_OxygenAutoregulatorHeart = in.OxygenAutoregulatorHeart();
  m_OxygenAutoregulatorMuscle = in.OxygenAutoregulatorMuscle();
  m_PeripheralBloodGasInteractionBaseline_Hz = in.ChemoreceptorPeripheralBloodGasInteractionBaseline_Hz();
  m_PeripheralFrequencyDelta_Per_min = in.PeripheralFrequencyDelta_Per_min();
  m_PeripheralPressureDelta_cmH2O = in.PeripheralPressureDelta_cmH2O();
  m_ResistanceModifierExtrasplanchnic = in.ResistanceModifierExtrasplanchnic();
  m_ResistanceModifierMuscle = in.ResistanceModifierMuscle();
  m_ResistanceModifierSplanchnic = in.ResistanceModifierSplanchnic();
  m_SympatheticPeripheralSignalBaseline_Hz = in.SympatheticPeripheralSignalBaseline();
  m_SympatheticSinoatrialSignalBaseline_Hz = in.SympatheticSinoatrialSignalBaseline();
  m_SympatheticPeripheralSignalFatigue = in.SympatheticPeripheralSignalFatigue();
  m_VagalSignalBaseline_Hz = in.VagalSignalBaseline();

  return true;
}
CDM::BioGearsNervousSystemData* Nervous::Unload() const
{
  CDM::BioGearsNervousSystemData* data = new CDM::BioGearsNervousSystemData();
  Unload(*data);
  return data;
}
void Nervous::Unload(CDM::BioGearsNervousSystemData& data) const
{
  SENervousSystem::Unload(data);
  data.AfferentChemoreceptor_Hz(m_AfferentChemoreceptor_Hz);
  data.AfferentPulmonaryStrechReceptor_Hz(m_AfferentPulmonaryStretchReceptor_Hz);
  data.AorticBaroreceptorStrain(m_AorticBaroreceptorStrain);
  data.ArterialCarbonDioxideBaseline_mmHg(m_ArterialCarbonDioxideBaseline_mmHg);
  data.ArterialOxygenBaseline_mmHg(m_ArterialOxygenBaseline_mmHg);
  data.BaroreceptorOperatingPoint_mmHg(m_BaroreceptorOperatingPoint_mmHg);
  data.CardiopulmonaryInputBaseline_mmHg(m_CardiopulmonaryInputBaseline_mmHg);
  data.CardiopulmonaryInput_mmHg(m_CardiopulmonaryInput_mmHg);
  data.CarotidBaroreceptorStrain(m_CarotidBaroreceptorStrain);
  data.CentralFrequencyDelta_Per_min(m_CentralFrequencyDelta_Per_min);
  data.CentralPressureDelta_cmH2O(m_CentralPressureDelta_cmH2O);
  for (auto eLarge : m_CerebralArteriesEffectors_Large) {
    data.CerebralArteriesEffectors_Large().push_back(eLarge);
  }
  for (auto eSmall : m_CerebralArteriesEffectors_Small) {
    data.CerebralArteriesEffectors_Small().push_back(eSmall);
  }

  data.CerebralBloodFlowBaseline_mL_Per_s(m_CerebralBloodFlowBaseline_mL_Per_s);
  data.CerebralBloodFlowInput_mL_Per_s(m_CerebralBloodFlowInput_mL_Per_s);
  data.CerebralOxygenSaturationBaseline(m_CerebralOxygenSaturationBaseline);
  data.CerebralPerfusionPressureBaseline_mmHg(m_CerebralPerfusionPressureBaseline_mmHg);
  data.ChemoreceptorFiringRateSetPoint_Hz(m_ChemoreceptorFiringRateSetPoint_Hz);
  data.ChemoreceptorPeripheralBloodGasInteractionBaseline_Hz(m_PeripheralBloodGasInteractionBaseline_Hz);
  data.ComplianceModifier(m_ComplianceModifier);
  data.HeartElastanceModifier(m_HeartElastanceModifier);
  data.HeartOxygenBaseline(m_HeartOxygenBaseline);
  data.HeartRateModifierSympathetic(m_HeartRateModifierSympathetic);
  data.HeartRateModifierVagal(m_HeartRateModifierVagal);
  data.HypercapniaThresholdHeart(m_HypercapniaThresholdHeart);
  data.HypercapniaThresholdPeripheral(m_HypercapniaThresholdPeripheral);
  data.HypoxiaThresholdHeart(m_HypoxiaThresholdHeart);
  data.HypoxiaThresholdPeripheral(m_HypoxiaThresholdPeripheral);
  data.MeanLungVolume_L(m_MeanLungVolume_L);
  data.MuscleOxygenBaseline(m_MuscleOxygenBaseline);
  data.OxygenAutoregulatorHeart(m_OxygenAutoregulatorHeart);
  data.OxygenAutoregulatorMuscle(m_OxygenAutoregulatorMuscle);
  data.PeripheralFrequencyDelta_Per_min(m_PeripheralFrequencyDelta_Per_min);
  data.PeripheralPressureDelta_cmH2O(m_PeripheralPressureDelta_cmH2O);
  data.ResistanceModifierExtrasplanchnic(m_ResistanceModifierExtrasplanchnic);
  data.ResistanceModifierMuscle(m_ResistanceModifierMuscle);
  data.ResistanceModifierSplanchnic(m_ResistanceModifierSplanchnic);
  data.SympatheticPeripheralSignalBaseline(m_SympatheticPeripheralSignalBaseline_Hz);
  data.SympatheticSinoatrialSignalBaseline(m_SympatheticSinoatrialSignalBaseline_Hz);
  data.SympatheticPeripheralSignalFatigue(m_SympatheticPeripheralSignalFatigue);
  data.VagalSignalBaseline(m_VagalSignalBaseline_Hz);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes the nervous specific quantities
///
/// \details
/// Initializes the nervous system.
//--------------------------------------------------------------------------------------------------
void Nervous::SetUp()
{
  m_dt_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  m_Succinylcholine = m_data.GetSubstances().GetSubstance("Succinylcholine");
  m_Sarin = m_data.GetSubstances().GetSubstance("Sarin");
  m_Patient = &m_data.GetPatient();

  m_DrugRespirationEffects = 0.0;

  m_painStimulusDuration_s = 0.0;
  m_painVASDuration_s = 0.0;
  m_painVAS = 0.0;

  // Set when feedback is turned on
  m_ArterialOxygenBaseline_mmHg = 95.0;
  //Reset after stabilization
  m_ArterialCarbonDioxideBaseline_mmHg = 40.0;

  m_AfferentBaroreceptorAortic_Hz = 25.15;
  m_AfferentBaroreceptorCarotid_Hz = 25.15;
  m_AfferentCardiopulmonary_Hz = 10.0;
  m_SympatheticSinoatrialSignal_Hz = 4.0;
  m_SympatheticPeripheralSignal_Hz = 4.8;
  m_VagalSignal_Hz = 0.80;
}

void Nervous::AtSteadyState()
{
  if (m_data.GetState() == EngineState::AtInitialStableState) {
    m_FeedbackActive = true;
    //Only reset oxygen baselines after initial stabilization because we want conditions (e.g. pneumonia) to put body in state of actively trying to get back to "normal"
    m_HeartOxygenBaseline = m_data.GetCompartments().GetIntracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Myocardium)).GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    m_MuscleOxygenBaseline = m_data.GetCompartments().GetIntracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle)).GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    m_OxygenAutoregulatorHeart = 0.0;
    m_OxygenAutoregulatorMuscle = 0.0;
    m_SympatheticPeripheralSignalBaseline_Hz = m_SympatheticPeripheralSignal_Hz;
    m_SympatheticSinoatrialSignalBaseline_Hz = m_SympatheticSinoatrialSignal_Hz;
    m_VagalSignalBaseline_Hz = m_VagalSignal_Hz;
  }

  // The set-points (Baselines) get reset at the end of each stabilization period.
  m_ArterialOxygenBaseline_mmHg = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg);
  m_ArterialCarbonDioxideBaseline_mmHg = m_data.GetBloodChemistry().GetArterialCarbonDioxidePressure(PressureUnit::mmHg);

  //Central and peripheral ventilation changes are set to 0 because patient baseline ventilation is updated to include
  //their contributions at steady state.
  m_CentralFrequencyDelta_Per_min = 0.0;
  m_CentralPressureDelta_cmH2O = 0.0;
  m_PeripheralFrequencyDelta_Per_min = 0.0;
  m_PeripheralPressureDelta_cmH2O = 0.0;

  //The chemoreceptor firing rate and its setpoint are reset so that central and peripheral derivatives will evaluate to 0
  //the first time step after stabilization (and will stay that way, assuming no other perturbations to blood gas levels)
  m_BaroreceptorOperatingPoint_mmHg = m_data.GetCardiovascular().GetSystolicArterialPressure(PressureUnit::mmHg);
  m_ChemoreceptorFiringRateSetPoint_Hz = m_AfferentChemoreceptor_Hz;
  m_CerebralPerfusionPressureBaseline_mmHg = m_data.GetCardiovascular().GetCerebralPerfusionPressure(PressureUnit::mmHg);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Preprocess methods for the nervous system
///
/// \details
/// Computes nervous system regulation of the body.
/// Baroreceptor and chemoreceptor feedback is computed and modifiers set in preparation for systems processing.
//--------------------------------------------------------------------------------------------------
void Nervous::PreProcess()
{
  CheckPainStimulus();

  AfferentResponse();
  CentralSignalProcess();
  EfferentResponse();
  LocalAutoregulation();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Nervous Process Step
///
/// \details
/// The only current Process-specific function checks the brain status to set events.
//--------------------------------------------------------------------------------------------------
void Nervous::Process()
{
  CheckNervousStatus();
  SetPupilEffects();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Nervous PostProcess Step
///
/// \details
/// Currently no nervous postprocess methods.
//--------------------------------------------------------------------------------------------------
void Nervous::PostProcess()
{
}

void Nervous::AfferentResponse()
{
  //Generate afferent arterial (high pressure) barorceptor signal
  BaroreceptorFeedback();

  //Generate afferent chemoreceptor signal -- combined respiration drug effects modifier calculated in this function since it primarily affects chemoreceptors
  ChemoreceptorFeedback();

  //Generate afferent lung stretch receptor signal (*ap = afferent pulmonary), based on Ursino2000Acute
  //We assume that the input is a running average of the total lung volume rather than the tidal volume (as source model does).
  //We make this choice because the tidal volume in BioGears can change drastically and be set to 0, causing model
  //instabilities.  The total lung volume is therefore a better reflection of the state of the lungs.
  const double tauAP_s = 2.0;
  const double gainAP_Hz_Per_L = 4.4;
  const double totalLungVolume = m_data.GetRespiratory().GetTotalLungVolume(VolumeUnit::L);
  const double dVolume = 0.5 * (-m_MeanLungVolume_L + totalLungVolume);
  m_MeanLungVolume_L += (dVolume * m_dt_s);
  const double dFrequencyAP_Hz = (1.0 / tauAP_s) * (-m_AfferentPulmonaryStretchReceptor_Hz + gainAP_Hz_Per_L * m_MeanLungVolume_L);
  m_AfferentPulmonaryStretchReceptor_Hz += dFrequencyAP_Hz * m_dt_s;
}

void Nervous::CentralSignalProcess()
{
  //--------Determine sympathetic signal to heart (SH) and periphery (SP)-------------------------------------------------------
  //Sympathetic signal constants
  const double kS = 0.0675;
  const double fSInf = 2.1;
  const double fS0 = 16.11;
  const double fSMax = 60.0;
  const double xSatSH = 50.0;
  const double xBasalSH = 3.85;
  const double oxygenHalfMaxSH = 60.0;
  const double kOxygenSH = 8.0;
  const double xCO2SH = 0.25;
  const double xSatSP = 6.0;
  const double xBasalSP = 1.9;
  const double oxygenHalfMaxSP = 30.0;
  const double kOxygenSP = 2.0;
  const double xCO2SP = 0.25;
  const double tauIschemia = 30.0;
  const double tauCO2 = 20.0;

  //As hypoxia or hypercapnia deepens, the ischemic CNS response leads to increased sympathetic outflow.
  //This is modeled as a reduction on the threshold required to cause sympathetic firing.
  //See Magosso2001Mathematical for more detail

  //Hypoxia thresholds
  const double arterialO2 = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg);
  const double expHypoxiaSH = std::exp((arterialO2 - oxygenHalfMaxSH) / kOxygenSH);
  const double expHypoxiaSP = std::exp((arterialO2 - oxygenHalfMaxSP) / kOxygenSP);
  const double hypoxiaSH = xSatSH / (1.0 + expHypoxiaSH);
  const double hypoxiaSP = xSatSP / (1.0 + expHypoxiaSP);
  const double dHypoxiaThresholdSH = (1.0 / tauIschemia) * (-m_HypoxiaThresholdHeart + hypoxiaSH);
  const double dHypoxiaThresholdSP = (1.0 / tauIschemia) * (-m_HypoxiaThresholdPeripheral + hypoxiaSP);
  //Hypercapnia thresholds
  const double arterialCO2 = m_data.GetBloodChemistry().GetArterialCarbonDioxidePressure(PressureUnit::mmHg);
  const double dHypercapaniaThresholdSH = (1.0 / tauCO2) * (-m_HypercapniaThresholdHeart + xCO2SH * (arterialCO2 - m_ArterialCarbonDioxideBaseline_mmHg));
  const double dHypercapniaThresholdSP = (1.0 / tauCO2) * (-m_HypercapniaThresholdPeripheral + xCO2SP * (arterialCO2 - m_ArterialCarbonDioxideBaseline_mmHg));

  //Inactive during initial stabilization and when there is a drug disrupting CNS(opioids)
  if (m_FeedbackActive && m_DrugRespirationEffects < ZERO_APPROX) {
    m_HypoxiaThresholdHeart += (dHypoxiaThresholdSH * m_dt_s);
    m_HypoxiaThresholdPeripheral += (dHypoxiaThresholdSP * m_dt_s);
    m_HypercapniaThresholdHeart += (dHypercapaniaThresholdSH * m_dt_s);
    m_HypercapniaThresholdPeripheral += (dHypercapniaThresholdSP * m_dt_s);
  }

  //Determine final values of sympathetic firing thresholds
  const double firingThresholdSH = xBasalSH - m_HypoxiaThresholdHeart - m_HypercapniaThresholdHeart;
  const double firingThresholdSP = xBasalSP - m_HypoxiaThresholdPeripheral - m_HypercapniaThresholdPeripheral;

  //The equations for sinoatrial and peripheral sympathetic firing are outlined in Ursino2000Acute and further refined
  //in Magosso2001Theoretical and Lim2013Cardiovascular.  Relative weights assigned to carotid and aortic baroreceptors
  //(no distinction between the two in Ursino) were obtained from Liang2006Simulation.

  //Weights of sympathetic signal to heart (i.e. sino-atrial node)--ABA = Afferent baroreceptors (aortic), ABC = Afferent baroreceptors (carotid), AC = Afferent chemoreceptors, ACP = Afferent cardiopulmonary
  const double wSH_ABA = -0.6;
  const double wSH_ABC = -0.4;
  const double wSH_AC = 1.0;
  const double wSH_ACP = -1.38;
  const double afferentCardiopulmonaryBaseline_Hz = 10.0;

  const double exponentSH = kS * (wSH_ABC * m_AfferentBaroreceptorCarotid_Hz + wSH_ABA * m_AfferentBaroreceptorAortic_Hz + wSH_AC * m_AfferentChemoreceptor_Hz + wSH_ACP * (m_AfferentCardiopulmonary_Hz - afferentCardiopulmonaryBaseline_Hz) - firingThresholdSH);
  m_SympatheticSinoatrialSignal_Hz = fSInf + (fS0 - fSInf) * std::exp(exponentSH);
  // m_SympatheticSinoatrialSignal_Hz = std::min(60.0, m_SympatheticSinoatrialSignal_Hz);
  //Weights of sympathetic signal to peripheral vascular beds--AB, AC, AT as before, AP = Afferent pulmonary stretch receptors, AA = Afferent atrial stretch receptors
  const double wSP_ABA = -0.452;
  const double wSP_ABC = -0.678;
  const double wSP_AC = 1.716;
  const double wSP_AP = -0.34;
  const double wSP_ACP = -1.38;

  const double exponentSP = kS * (wSP_ABC * m_AfferentBaroreceptorCarotid_Hz + wSP_ABA * m_AfferentBaroreceptorAortic_Hz + wSP_AC * m_AfferentChemoreceptor_Hz + wSP_AP * m_AfferentPulmonaryStretchReceptor_Hz + wSP_ACP * (m_AfferentCardiopulmonary_Hz - afferentCardiopulmonaryBaseline_Hz) - firingThresholdSP);
  m_SympatheticPeripheralSignal_Hz = fSInf + (fS0 - fSInf) * std::exp(exponentSP);
  // m_SympatheticPeripheralSignal_Hz = std::min(60.0, m_SympatheticPeripheralSignal_Hz);
  //Model fatigue of sympathetic peripheral response during sepsis -- Future work should investigate relevance of fatigue in other scenarios
  //Currently applying only to the peripheral signal because the literature notes that vascular smooth muscle shows depressed responsiveness to sympathetic activiy,
  //(Sayk et al., 2008 and Brassard et al., 2016) which would inhibit ability to increase peripheral resistance
  if (m_data.GetBloodChemistry().GetInflammatoryResponse().HasInflammationSource(CDM::enumInflammationSource::Infection)) {
    double fatigueThreshold = 6.0;
    double fatigueTimeConstant_hr = 2.0;
    double dFatigueScale_hr = 0.0;
    double fatigueInput = m_SympatheticPeripheralSignal_Hz - fatigueThreshold;
    if (fatigueInput > 0.0) {
      dFatigueScale_hr = (1.0 / fatigueTimeConstant_hr) * fatigueInput;
    } else if (m_SympatheticPeripheralSignalFatigue > ZERO_APPROX && fatigueInput < -0.1) {
      //Scale down fatigue if signal drops below threshold (0.1 buffer to make sure we don't trigger when hovering near SympatheticSignal = Threshold)
      dFatigueScale_hr = -m_SympatheticPeripheralSignalFatigue / 100.0;
    }
    m_SympatheticPeripheralSignalFatigue += (dFatigueScale_hr * m_data.GetTimeStep().GetValue(TimeUnit::hr));
  }

  //-------Determine vagal (parasympathetic) signal to heart------------------------------------------------------------------

  //Vagal model as developed in Ursino2000Acute.  Assumed that carotid and aortic baroreceptors have equal weight
  //The minimum firing rate and chemoreceptor weight have been lowered from there nominal values to increase
  //parasympathetic withdrawal during hemorrhage.
  const double kV = 7.06;
  const double fVInf = 6.3;
  const double fV0 = 1.2;
  const double wV_AC = 0.15;
  const double wV_AP = 0.0745;
  const double hypoxiaThresholdV = -0.648;
  const double baroreceptorBaseline_Hz = 25.15; //This value is the average of the fBaroMin and fBaroMax parameters in BaroreceptorFeedback (NOT reset AtSteadyState because we want continuous signals)

  const double exponentCarotid = (m_AfferentBaroreceptorCarotid_Hz - baroreceptorBaseline_Hz) / kV;
  const double exponentAortic = (m_AfferentBaroreceptorAortic_Hz - baroreceptorBaseline_Hz) / kV;
  m_VagalSignal_Hz = 0.5 * (fV0 + fVInf * std::exp(exponentCarotid)) / (1.0 + std::exp(exponentCarotid)) + 0.5 * (fV0 + fVInf * std::exp(exponentAortic)) / (1.0 + std::exp(exponentAortic));
  m_VagalSignal_Hz += (wV_AC * m_AfferentChemoreceptor_Hz + wV_AP * m_AfferentPulmonaryStretchReceptor_Hz - hypoxiaThresholdV);
  m_VagalSignal_Hz = std::max(m_VagalSignal_Hz, 0.0);
}

void Nervous::EfferentResponse()
{
  //Common parameters
  const double fSympatheticMin_Hz = 2.66;
  const double basePeripheralInput = std::log(m_SympatheticPeripheralSignalBaseline_Hz - fSympatheticMin_Hz + 1.0);
  const double baseSinoatrialInput = std::log(m_SympatheticSinoatrialSignalBaseline_Hz - fSympatheticMin_Hz + 1.0);

  //NOTE: Effector equations based on Ursino2000Acute and Ursino1998Interaction.  These equations are written:
  //            Effect(t) = del + EffectBase
  //      where del is the output of a low pass, first order filter and EffectBase is the baseline effector value
  //      when signal is at it's minimum.
  //      All del based on sympthetic signals are of the form :
  //            d(del)/dt = (1/tau)*(-del + G * ln(signal - signalMin + 1))
  //      where signal is either the sinoatrial or peripheral signal, signalMin is the minimum signal output, and
  //      G is the gain.
  //      For parasympathetic, the relationship is linear rather than logarithmic (only applies to HR vagal input)
  //            d(del)dt = (1/tau) * (-del + G * vagalSignal)
  //      We cannot use the Effect equations as written because the BioGears initial values (Effect0) are not identical
  //      to those in the cited papers.  We solve this problem by normalizing the equations.
  //      Observe first that, at baseline, each sympathetic effector will have a value:
  //            Effect(0) = G*ln(signal0 - signalMin + 1) + EffectBase    (1)
  //      Effect(0) will be different than EffectBase because the initial steady state signal0 is greater than signalMin
  //      Perturbing system to a new steady state will eventually satisfy:
  //            Effect(T) = G*ln(signalT - signalMin + 1) + EffectBase    (2)
  //      The fractional change from baseline will be (2) / (1).  We can multiply both numerator and denominator by
  //      (1 / EffectBase), leading to:
  //            FractionalChange = (G' * ln(signalT - signalMin + 1) + 1) / (G' * ln(signal0 - signalMin + 1) + 1)
  //      in which G' is a normalized gain equal to G / EffectBase.  An example of such a G' is below in "gainHRSympathetic.
  //      The reported gain in Ursino is -0.13 and the EffectBase (heart period, in this case) is 0.58.  The normalized
  //      gain is thus -0.13 / 0.58.

  //Heart Rate
  const double gainHRSympathetic = -0.13 / 0.58;
  const double gainHRVagal = 0.09 / 0.58;
  const double tauHRSympathetic = 2.0;
  const double heartPeriod0 = 1.0 + gainHRSympathetic * baseSinoatrialInput + gainHRVagal * m_VagalSignalBaseline_Hz;
  const double tauHRVagal = 1.5;

  double hrSympatheticInput = m_SympatheticSinoatrialSignal_Hz > fSympatheticMin_Hz ? gainHRSympathetic * std::log(m_SympatheticSinoatrialSignal_Hz - fSympatheticMin_Hz + 1.0) : 0.0;
  const double hrVagalInput = gainHRVagal * m_VagalSignal_Hz;
  double dHRModifierSympathetic = (1.0 / tauHRSympathetic) * (-m_HeartRateModifierSympathetic + hrSympatheticInput);
  double dHRModifierVagal = (1.0 / 1.5) * (-m_HeartRateModifierVagal + hrVagalInput);
  m_HeartRateModifierSympathetic += (dHRModifierSympathetic * m_dt_s);
  m_HeartRateModifierVagal += (dHRModifierVagal * m_dt_s);
  double nextHeartRateNorm = heartPeriod0 / (1.0 + m_HeartRateModifierSympathetic + m_HeartRateModifierVagal);

  //Heart elastance
  double elastanceScaleInput = m_SympatheticSinoatrialSignal_Hz > fSympatheticMin_Hz ? std::log(m_SympatheticSinoatrialSignal_Hz - fSympatheticMin_Hz + 1.0) : 0.0;
  const double gainElastance = 0.475 / 2.392;
  const double elastance0 = gainElastance * baseSinoatrialInput + 1.0;
  const double tauElastance = 8.0;

  const double dHeartElastanceModifier = (1.0 / tauElastance) * (-m_HeartElastanceModifier + gainElastance * elastanceScaleInput);
  m_HeartElastanceModifier += (dHeartElastanceModifier * m_dt_s);
  const double nextHeartElastanceNorm = (m_HeartElastanceModifier + 1.0) / elastance0;

  //Resistance -- Note that gain is suppressed (bounded at 0) when patient accumulates sympathetic fatigue (only occurs during sepsis currently)
  const double tauResistance = 6.0;
  const double gainResistanceExtrasplanchnic = 1.94 / 1.655;
  const double gainResistanceMuscle = 2.47 / 2.106;
  const double gainResistanceSplanchnic = 0.695 / 2.49;
  const double extrasplanchnicScale0 = gainResistanceExtrasplanchnic * basePeripheralInput + 1.0;
  const double muscleScale0 = gainResistanceMuscle * basePeripheralInput + 1.0;
  const double splanchnicScale0 = gainResistanceSplanchnic * basePeripheralInput + 1.0;

  double resistanceScaleInput = m_SympatheticPeripheralSignal_Hz > fSympatheticMin_Hz ? std::log(m_SympatheticPeripheralSignal_Hz - fSympatheticMin_Hz + 1.0) : 0.0;
  const double dResistanceScaleExtrasplanchnic = (1.0 / tauResistance) * (-m_ResistanceModifierExtrasplanchnic + gainResistanceExtrasplanchnic * resistanceScaleInput / (1.0 + m_SympatheticPeripheralSignalFatigue));
  const double dResistanceScaleMuscle = (1.0 / tauResistance) * (-m_ResistanceModifierMuscle + gainResistanceMuscle * resistanceScaleInput / (1.0 + m_SympatheticPeripheralSignalFatigue));
  const double dResistanceScaleSplanchnic = (1.0 / tauResistance) * (-m_ResistanceModifierSplanchnic + gainResistanceSplanchnic * resistanceScaleInput / (1.0 + m_SympatheticPeripheralSignalFatigue));

  m_ResistanceModifierExtrasplanchnic += (dResistanceScaleExtrasplanchnic * m_dt_s);
  m_ResistanceModifierMuscle += (dResistanceScaleMuscle * m_dt_s);
  m_ResistanceModifierSplanchnic += (dResistanceScaleSplanchnic * m_dt_s);

  const double nextExtrasplanchnicResistanceNorm = (m_ResistanceModifierExtrasplanchnic + 1.0) / extrasplanchnicScale0;
  const double nextMuscleResistanceNorm = (m_ResistanceModifierMuscle + 1.0) / muscleScale0;
  const double nextSplanchnicResistanceNorm = (m_ResistanceModifierSplanchnic + 1.0) / splanchnicScale0;

  //Venous Compliance--most models calculate change in unstressed volume.  We translate that volume change into a compliance change
  double complianceScaleInput = resistanceScaleInput; //Both resistance and compliance modifiers use the sympthetic periperhal signal
  const double volumeBase = 2870;
  const double gainVolume = -58.0 / 500.0;
  const double volume0 = gainVolume * basePeripheralInput + 1.0;
  const double tauVolume = 20.0;

  const double dComplianceModifier = (1.0 / tauVolume) * (-m_ComplianceModifier + gainVolume * complianceScaleInput);
  m_ComplianceModifier += (dComplianceModifier * m_dt_s);
  const double nextComplianceNorm = (m_ComplianceModifier + 1.0) / volume0;

  if (m_FeedbackActive) {
    GetHeartRateScale().SetValue(nextHeartRateNorm);
    GetHeartElastanceScale().SetValue(nextHeartElastanceNorm);
    GetResistanceScaleExtrasplanchnic().SetValue(nextExtrasplanchnicResistanceNorm);
    GetResistanceScaleMuscle().SetValue(nextMuscleResistanceNorm);
    GetResistanceScaleSplanchnic().SetValue(nextSplanchnicResistanceNorm);
    GetComplianceScale().SetValue(nextComplianceNorm);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the baroreceptor feedback and sets the scaling parameters in the CDM
///
/// \details
/// The baroreceptor feedback function uses the current mean arterial pressure relative to the mean arterial
/// pressure set-point in order to calculate the sympathetic and parasympathetic response fractions.
/// These fractions are used to update the scaling parameters of heart rate, heart elastance, resistance and compliance
/// for each time step.
//--------------------------------------------------------------------------------------------------
/// \todo Add decompensation. Perhaps a reduction in the effect that is a function of blood volume below a threshold... and maybe time.
void Nervous::BaroreceptorFeedback()
{
  //Determine strain on carotid and aortic arterial wall caused by pressure changes -- uses Voigt body (spring-dashpot system) to estimate
  //See Randall2019ModelBased for details about Voght body model
  const double A = 5.0;
  const double kVoigt = 0.1;
  const double tauVoigt = 0.9;
  const double slopeStrain = 0.04;

  //Pain exercise, and some drugs alter the apparent operating point of the baroreceptors
  double painEffect = 0.0;
  double drugEffect = 0.0;
  const double exerciseEffect = m_data.GetEnergy().GetExerciseMeanArterialPressureDelta(PressureUnit::mmHg);

  if (m_data.GetActions().GetPatientActions().HasPainStimulus()) {
    const double painVAS = 0.1 * GetPainVisualAnalogueScale().GetValue();
    painEffect = 0.5 * painVAS * m_BaroreceptorOperatingPoint_mmHg;
  }
  for (SESubstance* drug : m_data.GetSubstances().GetActiveDrugs()) {
    if ((drug->GetClassification() == CDM::enumSubstanceClass::Anesthetic) || (drug->GetClassification() == CDM::enumSubstanceClass::Sedative) || (drug->GetClassification() == CDM::enumSubstanceClass::Opioid)) {
      drugEffect = m_data.GetDrugs().GetMeanBloodPressureChange(PressureUnit::mmHg); // / m_data.GetPatient().GetMeanArterialPressureBaseline(PressureUnit::mmHg);
      break;
      //Only want to apply the blood pressure change ONCE (In case there are multiple sedative/opioids/etc)
    }
  }
  const double baroreceptorOperatingPoint_mmHg = m_BaroreceptorOperatingPoint_mmHg + painEffect + exerciseEffect + drugEffect;

  //Carotid baroreceptors are sensitive to arterial pressure changes. Future work could consider the effects of gravitational
  //force, as the pressure at these receptors is technically a function of height above the heart.  Thus, in a lying down
  //position, carotid receptors would "sense" a different pressure than when standing.  See Lim2013Cardiovascular for such
  //an application
  const double systolicPressure_mmHg = m_data.GetCardiovascular().GetSystolicArterialPressure(PressureUnit::mmHg);
  const double carotidStrainExp = std::exp(-slopeStrain * (systolicPressure_mmHg - baroreceptorOperatingPoint_mmHg));
  const double carotidWallStrain = 1.0 - std::sqrt((1.0 + carotidStrainExp) / (A + carotidStrainExp));
  const double dCarotidStrain = (1.0 / tauVoigt) * (-m_CarotidBaroreceptorStrain + kVoigt * carotidWallStrain);
  m_CarotidBaroreceptorStrain += (dCarotidStrain * m_dt_s);
  const double carotidStrainSignal = carotidWallStrain - m_CarotidBaroreceptorStrain;

  //Aortic baroreceptors detect transmural pressure between aorta and thoracic (pleural) space.
  //Thus, the pressure input is the difference between the sytolic pressure and mean pleural pressure.
  //This means that the aortic baroreceptors will have different sensitivities to events that effect the pleural
  //space (like tension pneumothorax).
  const double meanPleuralPressure_mmHg = m_data.GetRespiratory().GetMeanPleuralPressure(PressureUnit::mmHg);
  const double aorticPressureInput = systolicPressure_mmHg - meanPleuralPressure_mmHg;
  const double aorticStrainExp = std::exp(-slopeStrain * (aorticPressureInput - baroreceptorOperatingPoint_mmHg));
  const double aorticWallStrain = 1.0 - std::sqrt((1.0 + aorticStrainExp) / (A + aorticStrainExp));
  const double dAorticStrain = (1.0 / tauVoigt) * (-m_AorticBaroreceptorStrain + kVoigt * aorticWallStrain);
  m_AorticBaroreceptorStrain += (dAorticStrain * m_dt_s);
  const double aorticStrainSignal = aorticWallStrain - m_AorticBaroreceptorStrain;

  //Convert strain signals to be on same basis as Ursino2000Acute--this puts deviations in wall strain on same basis as other signals
  const double fBaroMax = 47.78;
  const double fBaroMin = 2.52;
  const double kBaro = 0.075;
  const double baselineStrainSignal = 0.9 * (1.0 - std::sqrt(1.0 / 3.0)); //Derived by calculating wallStrain when systolic pressure = operating pressure and setting m_AfferentStrain = kVoigt * wallStrain (steady state)
  const double carotidExponent = std::exp((carotidStrainSignal - baselineStrainSignal) / kBaro);
  m_AfferentBaroreceptorCarotid_Hz = (fBaroMin + fBaroMax * carotidExponent) / (1.0 + carotidExponent);
  const double aorticExponent = std::exp((aorticStrainSignal - baselineStrainSignal) / kBaro);
  m_AfferentBaroreceptorAortic_Hz = (fBaroMin + fBaroMax * aorticExponent) / (1.0 + aorticExponent);

  //Cardiopulmonary (or low pressure) receptors are sensitive to the volume of blood being return to heart
  //Since they are located by the heart, they are also effected by changes in transmural pressure.
  //This model for cardiopulmonary receptors is based on Lim2013Cardiovascular
  const double tZ = 6.0;
  const double cvp = m_data.GetCardiovascular().GetCentralVenousPressure(PressureUnit::mmHg);
  const double kAff = 3.429;
  const double fmax2 = 20.0;

  const double dFilterCVP = (1.0 / tZ) * (-m_CardiopulmonaryInput_mmHg + (cvp - meanPleuralPressure_mmHg));
  m_CardiopulmonaryInput_mmHg += (dFilterCVP * m_dt_s);
  const double centralExp = std::exp((m_CardiopulmonaryInputBaseline_mmHg - m_CardiopulmonaryInput_mmHg) / kAff);
  m_AfferentCardiopulmonary_Hz = fmax2 / (1.0 + centralExp);

  if (!m_FeedbackActive) {
    m_CardiopulmonaryInputBaseline_mmHg = m_CardiopulmonaryInput_mmHg;
  }

  //Update baroreceptor setpoint -- the study from which this time constant was obtained focused on hemorrhagic shock.  The time scale is much different
  //than septic shock and so it is not clear how (or if) this value would change for sepsis.  For now, we will track baroreceptor adaptation and sympathetic fatigue
  //separately.  Future work should try to consolidate these two phenomena into a single model
  if (m_data.GetState() > EngineState::SecondaryStabilization && !m_data.GetBloodChemistry().GetInflammatoryResponse().HasInflammationSource(CDM::enumInflammationSource::Infection)) {
    //Pruett2013Population assumes ~16 hr half-time for baroreceptor adaptation to new setpoint (They varied this parameter up to 1-2 days half-time)
    const double kAdapt_Per_hr = 0.042;
    const double dSetpointAdjust_mmHg_Per_hr = kAdapt_Per_hr * (systolicPressure_mmHg - m_BaroreceptorOperatingPoint_mmHg);
    m_BaroreceptorOperatingPoint_mmHg += (dSetpointAdjust_mmHg_Per_hr * m_data.GetTimeStep().GetValue(TimeUnit::hr));
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the cerebral autoregulation and local autoregulation of muscle and myocardium
///
/// \details

//--------------------------------------------------------------------------------------------------
void Nervous::LocalAutoregulation()
{
  //-------------Cerebral autoregulation:  Based on Ursino1998Interaction and Urinso2001Role ---------
  //Feedback constants -- order is CBF auto, CO2, O2
  const std::vector<double> gainsLargeArteries { 0.0159, 1.958, 1.12 };
  const std::vector<double> tausLargeArteries { 10.0, 20.0, 20.0 };
  const std::vector<double> gainsSmallArteries { 1.68, 10.17, 8.79 };
  const std::vector<double> tausSmallArteries { 20.0, 20.0, 20.0 };
  //Boundaries on large and small cerebral arteries resistances
  const double minResistanceMultiplierLarge = 0.8;
  const double maxResistanceMultiplierLarge = 1.2;
  const double minResistanceMultiplierSmall = 0.75;
  const double maxResistanceMultiplierSmall = 1.25;
  //Cerebral venous O2 saturation constants
  const double o2BindingCapacity = 0.204;
  const double o2Solubility = 3.0e-5;
  const double cerebralO2ConsumptionRate = 0.77;
  const double halfSaturationPressure = 26.0;
  const double alpha = 2.6;
  //Carbon dioxide gain modulation
  const double kCO2 = 20.0;
  const double bCO2 = 10.0;

  //Determine cerebral blood flow (filtered for noise)
  double filterConstant = m_FeedbackActive ? 0.5 : 0.02; //Lower during initial stabilization so that our baseline we derive from this output is not noisy
  const double cerebralBloodFlow_mL_Per_s = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralVeins2ToNeckVeins)->GetFlow(VolumePerTimeUnit::mL_Per_s);
  const double dCerebralBloodFlow = filterConstant * (-m_CerebralBloodFlowInput_mL_Per_s + cerebralBloodFlow_mL_Per_s);
  m_CerebralBloodFlowInput_mL_Per_s += (dCerebralBloodFlow * m_dt_s);

  //Current physiological values
  const double cerebralPerfusionPressure = m_data.GetCardiovascular().GetCerebralPerfusionPressure(PressureUnit::mmHg);
  const double arterialCO2Pressure = m_data.GetBloodChemistry().GetArterialCarbonDioxidePressure(PressureUnit::mmHg);
  const double arterialO2Pressure = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg);
  const double largePialArteriesResistanceBaseline = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::NeckArteriesToCerebralArteries1)->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
  const double smallPialArteriesResistanceBaseline = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralArteries2ToCapillaries)->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);

  //Derive cerebral venous O2 saturation
  const double arterialO2Saturation = std::pow(arterialO2Pressure / halfSaturationPressure, alpha) / (1.0 + std::pow(arterialO2Pressure / halfSaturationPressure, alpha));
  const double arterialO2Concentration = o2Solubility * arterialO2Pressure + o2BindingCapacity * arterialO2Saturation;
  const double venousCerebralO2Concentration = arterialO2Concentration - cerebralO2ConsumptionRate / m_CerebralBloodFlowInput_mL_Per_s;
  const double cerebralVenousO2Saturation = venousCerebralO2Concentration / o2BindingCapacity;

  //Fractional change in cerebral blood flow and CO2 effect attenuation
  const double fracCBFChange = (m_CerebralBloodFlowInput_mL_Per_s - m_CerebralBloodFlowBaseline_mL_Per_s) / m_CerebralBloodFlowBaseline_mL_Per_s;
  const double aCO2 = 1.0 / (1.0 + std::exp(-kCO2 * fracCBFChange - bCO2));

  //Inputs to derivatives -- note that CBF input is different for large and small arteries, but CO2 and O2 inputs are identical for each
  const double inputCBF_Large = cerebralPerfusionPressure - m_CerebralPerfusionPressureBaseline_mmHg;
  const double inputCBF_Small = fracCBFChange;
  const double inputCO2 = std::log10(arterialCO2Pressure / m_ArterialCarbonDioxideBaseline_mmHg) * aCO2;
  const double inputO2 = cerebralVenousO2Saturation - m_CerebralOxygenSaturationBaseline;
  const std::vector<double> largeArteriesInputs { inputCBF_Large, inputCO2, inputO2 };
  const std::vector<double> smallArteriesInputs { inputCBF_Small, inputCO2, inputO2 };

  double dEffect = 0.0;
  //Large pial arteries
  for (size_t itr = 0; itr < m_CerebralArteriesEffectors_Large.size(); ++itr) {
    dEffect = (1.0 / tausLargeArteries[itr]) * (-m_CerebralArteriesEffectors_Large[itr] + gainsLargeArteries[itr] * largeArteriesInputs[itr]);
    m_CerebralArteriesEffectors_Large[itr] += dEffect * m_dt_s;
  }
  //Small pial arteries
  for (size_t itr = 0; itr < m_CerebralArteriesEffectors_Small.size(); ++itr) {
    dEffect = (1.0 / tausSmallArteries[itr]) * (-m_CerebralArteriesEffectors_Small[itr] + gainsSmallArteries[itr] * smallArteriesInputs[itr]);
    m_CerebralArteriesEffectors_Small[itr] += dEffect * m_dt_s;
  }

  //Combined large/small input is sum of all components
  const double combinedLargePialRegulator = std::accumulate(m_CerebralArteriesEffectors_Large.begin(), m_CerebralArteriesEffectors_Large.end(), 0.0);
  const double combinedSmallPialRegulator = std::accumulate(m_CerebralArteriesEffectors_Small.begin(), m_CerebralArteriesEffectors_Small.end(), 0.0);

  //Update cerebral resistances -- sigmoids aren't symmetric so we need to adjust width/slope depending on value of autoregulator variables
  double resistanceWidthLarge = 1.0 - minResistanceMultiplierLarge;
  double resistanceSlopeLarge = (1.0 - minResistanceMultiplierLarge) / 4.0;
  if (combinedLargePialRegulator > 0.0) {
    resistanceWidthLarge = maxResistanceMultiplierLarge - 1.0;
    resistanceSlopeLarge = (maxResistanceMultiplierLarge - 1.0) / 4.0;
  }
  double resistanceWidthSmall = 1.0 - minResistanceMultiplierSmall;
  double resistanceSlopeSmall = (1.0 - minResistanceMultiplierSmall) / 4.0;
  if (combinedSmallPialRegulator > 0.0) {
    resistanceWidthSmall = maxResistanceMultiplierSmall - 1.0;
    resistanceSlopeSmall = (maxResistanceMultiplierSmall - 1.0) / 4.0;
  }
  const double largePialResistanceMultiplier = ((1.0 + resistanceWidthLarge) + (1.0 - resistanceWidthLarge) * std::exp(-combinedLargePialRegulator / resistanceSlopeLarge)) / (1.0 + std::exp(-combinedLargePialRegulator / resistanceSlopeLarge));
  const double smallPialResistanceMultiplier = ((1.0 + resistanceWidthSmall) + (1.0 - resistanceWidthSmall) * std::exp(-combinedSmallPialRegulator / resistanceSlopeSmall)) / (1.0 + std::exp(-combinedSmallPialRegulator / resistanceSlopeSmall));

  if (!m_FeedbackActive) {
    m_CerebralBloodFlowBaseline_mL_Per_s = m_CerebralBloodFlowInput_mL_Per_s;
    m_CerebralOxygenSaturationBaseline = cerebralVenousO2Saturation;
  } else {
    m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::NeckArteriesToCerebralArteries1)->GetNextResistance().SetValue(largePialResistanceMultiplier * largePialArteriesResistanceBaseline, FlowResistanceUnit::mmHg_s_Per_mL);
    m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralArteries2ToCapillaries)->GetNextResistance().SetValue(smallPialResistanceMultiplier * smallPialArteriesResistanceBaseline, FlowResistanceUnit::mmHg_s_Per_mL);
  }

  //------Muscle and heart autoregulation:  Based on Ursino2000Acute-----------------------------------------
  const double gainHeart_L_Blood_Per_L_O2 = 35.0;
  const double gainMuscle_L_Blood_Per_L_O2 = 750.0;
  const double tauAutoregulator = 10.0;
  const double conversion_L_O2_Per_mmol_O2 = 0.227;
  const double heartOxygenMolarity = m_data.GetCompartments().GetIntracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Myocardium)).GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  const double muscleOxygenMolarity = m_data.GetCompartments().GetIntracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle)).GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);

  const double dHeartAutoEffect = (1.0 / tauAutoregulator) * (-m_OxygenAutoregulatorHeart - gainHeart_L_Blood_Per_L_O2 * conversion_L_O2_Per_mmol_O2 * (heartOxygenMolarity - m_HeartOxygenBaseline));
  const double dMuscleAutoEffect = (1.0 / tauAutoregulator) * (-m_OxygenAutoregulatorMuscle - gainMuscle_L_Blood_Per_L_O2 * conversion_L_O2_Per_mmol_O2 * (muscleOxygenMolarity - m_MuscleOxygenBaseline));
  m_OxygenAutoregulatorHeart += dHeartAutoEffect * m_dt_s;
  m_OxygenAutoregulatorMuscle += dMuscleAutoEffect * m_dt_s;

  m_OxygenAutoregulatorHeart = std::max(0.0, m_OxygenAutoregulatorHeart);
  m_OxygenAutoregulatorMuscle = std::max(0.0, m_OxygenAutoregulatorMuscle);
  double nextMuscleResistance = GetResistanceScaleMuscle().GetValue();
  nextMuscleResistance *= (1.0 / (1.0 + m_OxygenAutoregulatorMuscle));

  const double metabolicFraction = m_data.GetEnergy().GetTotalMetabolicRate(PowerUnit::W) / m_data.GetPatient().GetBasalMetabolicRate(PowerUnit::W);

  //Avoid applying modifiers when drugs affecting respiration are present. If respiration rate --> 0 (like with succinylcholine),
  //  then we get outputs from this function that overwhelm all other modifiers.  This technically isn't unreasonable if you
  //  weren't breathing for a long time.  But some of our PD test scenarios apply neuromuscular blockers without respiratory
  //  support and we want to make sure we can still run those tests.
  //We are also not applying the muscle autoregulatory modifier when total metabolic rate is elevated.  This is because
  //  when metabolicFraction > 1, the function Cardiovascular::MetabolicToneResponse handles muscle vasodilation.  Future
  //  work should try to combine these functions (perhaps move MetabolicToneResponse to Nervous).  Putting cutoff at 2.5% above
  //  baseline metabolic fraction so that small perturbations don't cause deactivation.
  if (m_FeedbackActive) {
    GetResistanceScaleMyocardium().SetValue(1.0 / (1.0 + m_OxygenAutoregulatorHeart));
    if (metabolicFraction < 1.025 && m_DrugRespirationEffects < 0.05) {
      GetResistanceScaleMuscle().SetValue(nextMuscleResistance);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the chemoreceptor feedback and sets the scaling parameters in the CDM
///
/// \details
/// The chemoreceptor feedback function uses the current arterial partial pressure of oxygen and carbon dioxide
/// relative to the partial pressure set-points in order to calculate response signal.
/// The affected systems identify the signal and adjust accordingly. Note that chemoreception
/// is currently built into the respiratory driver; therefore, the chemoreceptor feedback only sets CV modifiers.
//--------------------------------------------------------------------------------------------------
void Nervous::ChemoreceptorFeedback()
{
  //-------Respiratory Feedback:  This is active throughtout the simulation (including stabilization)------------------------------
  //Chemoreceptor time constants (all in s)
  const double tau_p_P = 100.0;
  const double tau_p_F = 100.0;
  const double tau_c_P = 180.0;
  const double tau_c_F = 180.0;
  //Chemoreceptor gains--Tuned to data from Reynold, 1972 and Reynolds, 1973 (cited in Cheng, 2016)
  const double gain_p_P = 1.0;
  const double gain_p_F = 3.0;
  double gain_c_P = 0.65;
  double gain_c_F = 0.70;
  //Afferent peripheral constants
  const double firingRateMin_Hz = 0.835;
  const double firingRateMax_Hz = 12.3;
  const double oxygenHalfMax_mmHg = 45.0;
  const double oxygenScale_mmHg = 29.27;
  const double gasInteractionMax = 3.0;
  const double tau_Peripheral = 2.0;
  const double tuningFactor = 1.4;

  //Determine a combined drug effect on the respiratory arm of the nervous system
  SEDrugSystem& Drugs = m_data.GetDrugs();
  const double cnsModifier = Drugs.GetCentralNervousResponse().GetValue(); //Apply effects of opioids that depress central nervous activity
  const double sedationModifier = Drugs.GetSedationLevel().GetValue(); //Apply effects of sedatives
  const double nbModifier = Drugs.GetNeuromuscularBlockLevel().GetValue(); //Apply effects of neuromuscular blockers
  m_DrugRespirationEffects = cnsModifier + sedationModifier; //Assume cns and sedation modifiers gradually accumulate affect on respiratory drive
  if (nbModifier > 0.1) {
    //Assume that neuromuscular block shuts down respiratory drive completely
    m_DrugRespirationEffects = 1.0;
  }
  BLIM(m_DrugRespirationEffects, 0.0, 1.0);

  //Note that this method uses instantaneous values of blood gas levels, not running averages
  const double arterialO2Pressure_mmHg = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg);
  const double arterialCO2Pressure_mmHg = m_data.GetBloodChemistry().GetArterialCarbonDioxidePressure(PressureUnit::mmHg);

  //Magosso and Ursino cite findings that central chemoreceptors are less sensitive at sub-normal levels of CO2 than to super-normal levels
  if (arterialCO2Pressure_mmHg < m_ArterialCarbonDioxideBaseline_mmHg) {
    gain_c_P *= 0.067;
    gain_c_F *= 0.067;
  }
  //The psi parameter captures the combined interactive effect of O2 and CO2 on the peripheral chemoreceptors.  The degree
  //of interaction varies as hypoxia deepens, with CO2 having less impact as O2 levels decrease
  const double psiNum = firingRateMax_Hz + firingRateMin_Hz * exp((arterialO2Pressure_mmHg - oxygenHalfMax_mmHg) / oxygenScale_mmHg);
  const double psiDen = 1.0 + exp((arterialO2Pressure_mmHg - oxygenHalfMax_mmHg) / oxygenScale_mmHg);
  double gasInteraction = gasInteractionMax;
  if (arterialO2Pressure_mmHg <= 80.0) {
    if (arterialO2Pressure_mmHg > 40.0) {
      gasInteraction = gasInteractionMax - 1.2 * (80.0 - arterialO2Pressure_mmHg) / 30.0;
    } else {
      gasInteraction = gasInteractionMax - 1.6;
    }
  }
  const double psi = (psiNum / psiDen) * (gasInteraction * std::log(arterialCO2Pressure_mmHg / m_ArterialCarbonDioxideBaseline_mmHg) + tuningFactor);

  //Adjust inputs to differential equations so that they approach their setpoints (i.e. baseline signal) when drugs that modifiy CNS are present
  //The peripherial input is not adjusted because the afferent signal is already accounted for.
  const double afferentFiringInput = psi * std::exp(-3.5 * cnsModifier) + m_ChemoreceptorFiringRateSetPoint_Hz * (1.0 - std::exp(-3.5 * cnsModifier));
  const double centralInput = (arterialCO2Pressure_mmHg - m_ArterialCarbonDioxideBaseline_mmHg) * std::exp(-3.5 * cnsModifier);
  const double peripheralInput = (m_AfferentChemoreceptor_Hz - m_ChemoreceptorFiringRateSetPoint_Hz);

  //Evaluate model derivatives pertaining to change in chemoreceptor firing rate, and changes in central and peripheral contributions to ventilation
  const double dFiringRate_Hz = (-m_AfferentChemoreceptor_Hz + afferentFiringInput) / tau_Peripheral * m_dt_s;
  const double dFrequencyCentral_Per_min = (-m_CentralFrequencyDelta_Per_min + gain_c_F * centralInput) / tau_c_F * m_dt_s;
  const double dPressureCentral_cmH2O = (-m_CentralPressureDelta_cmH2O + gain_c_P * centralInput) / tau_c_P * m_dt_s;
  const double dFrequencyPeripheral_Per_min = (-m_PeripheralFrequencyDelta_Per_min + gain_p_F * peripheralInput) / tau_p_F * m_dt_s;
  const double dPressurePeripheral_cmH2O = (-m_PeripheralPressureDelta_cmH2O + gain_p_P * peripheralInput) / tau_p_P * m_dt_s;

  m_AfferentChemoreceptor_Hz += dFiringRate_Hz * m_dt_s;
  m_AfferentChemoreceptor_Hz = std::max(0.0, m_AfferentChemoreceptor_Hz);
  m_AfferentChemoreceptor_Hz = std::min(m_AfferentChemoreceptor_Hz, firingRateMax_Hz);
  m_CentralFrequencyDelta_Per_min += dFrequencyCentral_Per_min;
  m_CentralPressureDelta_cmH2O += dPressureCentral_cmH2O;
  m_PeripheralFrequencyDelta_Per_min += dFrequencyPeripheral_Per_min;
  m_PeripheralPressureDelta_cmH2O += dPressurePeripheral_cmH2O;

  //Update Respiratory metrics
  const double baselineRespirationRate_Per_min = m_data.GetPatient().GetRespirationRateBaseline(FrequencyUnit::Per_min);
  const double baselineDrivePressure_cmH2O = m_Patient->GetRespiratoryDriverAmplitudeBaseline(PressureUnit::cmH2O);
  double nextRespirationRate_Per_min = baselineRespirationRate_Per_min + m_CentralFrequencyDelta_Per_min + m_PeripheralFrequencyDelta_Per_min;
  double nextDrivePressure_cmH2O = baselineDrivePressure_cmH2O - m_CentralPressureDelta_cmH2O - m_PeripheralPressureDelta_cmH2O;

  //Apply metabolic effects. The modifier is tuned to achieve the correct respiratory response for near maximal exercise.
  //A linear relationship is assumed for the respiratory effects due to increased metabolic exertion
  //Old driver multiplied a target ventilation by the metabolic modifier.  Since Vent (L/min) = RR(/min) * TV(L) and
  //relationship between driver pressure and TV is approx linear (e.g. 10% change in pressure -> 10% change in TV),
  //, we should be able to maintain this assumption to achieve desired effect
  const double TMR_W = m_data.GetEnergy().GetTotalMetabolicRate(PowerUnit::W);
  const double BMR_W = m_data.GetPatient().GetBasalMetabolicRate(PowerUnit::W);
  const double energyDeficit_W = m_data.GetEnergy().GetEnergyDeficit(PowerUnit::W);
  const double metabolicFraction = (TMR_W + energyDeficit_W) / BMR_W;
  const double tunedVolumeMetabolicSlope = 0.2;
  const double metabolicModifier = 1.0 + tunedVolumeMetabolicSlope * (metabolicFraction - 1.0);
  nextRespirationRate_Per_min *= metabolicModifier;
  nextDrivePressure_cmH2O *= metabolicModifier;

  //We want to make sure the patient respiration rate baseline is met.  Therefore, we only allow RR to change +/- 5% during initial stabilization
  if (m_data.GetState() < EngineState::AtInitialStableState) {
    const double upperTolerance = baselineRespirationRate_Per_min * 1.05;
    const double lowerTolerance = baselineRespirationRate_Per_min * 0.95;
    if (nextRespirationRate_Per_min < upperTolerance && nextRespirationRate_Per_min > lowerTolerance) {
      m_data.GetRespiratory().GetRespirationDriverFrequency().SetValue(nextRespirationRate_Per_min, FrequencyUnit::Per_min);
    }
  } else {
    m_data.GetRespiratory().GetRespirationDriverFrequency().SetValue(nextRespirationRate_Per_min, FrequencyUnit::Per_min);
  }
  //Driver pressure is always updated
  m_data.GetRespiratory().GetRespirationDriverPressure().SetValue(nextDrivePressure_cmH2O, PressureUnit::cmH2O);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the patient pain response due to stimulus, susceptibility and drugs
///
/// \details
/// A patient reacts to a noxious stimulus in a certain way. Generally this is reported as a VAS
/// scale value. This value is generally reported by the patient after the nervous system has already parsed
/// the stimulus. For a robotic manikin trainer we need to determine the nervous system and systemic responses
/// related to that stimulus
//--------------------------------------------------------------------------------------------------
void Nervous::CheckPainStimulus()
{
  //Screen for both external pain stimulus and presence of inflammation
  if (!m_data.GetActions().GetPatientActions().HasPainStimulus() && !m_data.GetBloodChemistry().GetInflammatoryResponse().HasInflammationSources()) {
    GetPainVisualAnalogueScale().SetValue(0.0);
    return;
  }

  //initialize:
  SEPainStimulus* p;
  const std::map<std::string, SEPainStimulus*>& pains = m_data.GetActions().GetPatientActions().GetPainStimuli();
  double patientSusceptability = m_Patient->GetPainSusceptibility().GetValue();
  double susceptabilityMapping = GeneralMath::LinearInterpolator(-1.0, 1.0, 0.0, 2.0, patientSusceptability); //mapping [-1,1] -> [0, 2] for scaling the pain stimulus
  double severity = 0.0;
  double halfLife_s = 0.0;
  double painVASMapping = 0.0; //for each location map the [0,1] severity to the [0,10] VAS scale
  double tempPainVAS = 0.0; //sum, scale and store the patient score
  double PainBuffer = 1.0;

  m_painVAS = GetPainVisualAnalogueScale().GetValue();

  //reset duration if VAS falls below approx zero
  if (m_painVAS == 0.0)
    m_painStimulusDuration_s = 0.0;

  //determine pain response from inflammation caused by burn trauma
  if (m_data.GetActions().GetPatientActions().HasBurnWound()) {
    double traumaPain = m_data.GetActions().GetPatientActions().GetBurnWound()->GetTotalBodySurfaceArea().GetValue();
    traumaPain *= 20.0; //25% TBSA burn will give pain scale = 5, 40% TBSA will give pain scale = 8.0
    tempPainVAS += (traumaPain * susceptabilityMapping * PainBuffer) / (1 + exp(-m_painStimulusDuration_s + 4.0));
  }

  //iterate over all locations to get a cumulative stimulus and buffer them
  for (auto pain : pains) {
    p = pain.second;
    severity = p->GetSeverity().GetValue();
    if (p->HasHalfLife()) {
      halfLife_s = p->GetHalfLife().GetValue(TimeUnit::s);
      severity = severity * (std::pow(0.5, (m_painStimulusDuration_s / halfLife_s)));
    }
    painVASMapping = 10.0 * severity;

    tempPainVAS += (painVASMapping * susceptabilityMapping) / (1.0 + exp(-m_painStimulusDuration_s + 4.0)); //temp time will increase so long as a stimulus is present
  }

  //apply effects of pain medication
  if (m_data.GetDrugs().HasPainToleranceChange()) {
    double PainModifier = 10.0 * m_data.GetDrugs().GetPainToleranceChange().GetValue();
    tempPainVAS += -PainModifier;
    tempPainVAS = std::max(tempPainVAS, 0.0);
  }

  //advance time over the duration of the stimulus

  if (severity < ZERO_APPROX) {
    m_painVASDuration_s += m_dt_s;
  }

  m_painStimulusDuration_s += m_dt_s;

  //set the VAS data:
  if (tempPainVAS > 10)
    tempPainVAS = 10.0;

  GetPainVisualAnalogueScale().SetValue(tempPainVAS);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Checks metrics in the nervous system to determine events to be thrown.  Currently includes brain status
/// and presence of fasciculation.
///
/// \details
/// Intracranial pressure is checked to determine if the patient has Intracranial Hyper/hypotension
/// Fasciculation can occur as a result of calcium/magnesium deficiency
/// (or other electrolyte imbalances),succinylcholine, nerve agents, ALS
/// Currently, only fasciculations due to the nerve agent Sarin are active.  Other causes are a subject of model improvement
//------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Nervous::CheckNervousStatus()
{
  //-----Check Brain Status-----------------
  double icp_mmHg = m_data.GetCardiovascular().GetIntracranialPressure().GetValue(PressureUnit::mmHg);

  //Intracranial Hypertension
  if (icp_mmHg > 25.0) // \cite steiner2006monitoring
  {
    /// \event Patient: Intracranial Hypertension. The intracranial pressure has risen above 25 mmHg.
    m_data.GetPatient().SetEvent(CDM::enumPatientEvent::IntracranialHypertension, true, m_data.GetSimulationTime());
  } else if (m_data.GetPatient().IsEventActive(CDM::enumPatientEvent::IntracranialHypertension) && icp_mmHg < 24.0) {
    /// \event Patient: End Intracranial Hypertension. The intracranial pressure has fallen below 24 mmHg.
    m_data.GetPatient().SetEvent(CDM::enumPatientEvent::IntracranialHypertension, false, m_data.GetSimulationTime());
  }

  //Intracranial Hypotension
  if (icp_mmHg < 7.0) // \cite steiner2006monitoring
  {
    /// \event Patient: Intracranial Hypotension. The intracranial pressure has fallen below 7 mmHg.
    m_data.GetPatient().SetEvent(CDM::enumPatientEvent::IntracranialHypotension, true, m_data.GetSimulationTime());
  } else if (m_data.GetPatient().IsEventActive(CDM::enumPatientEvent::IntracranialHypotension) && icp_mmHg > 7.5) {
    /// \event Patient: End Intracranial Hypotension. The intracranial pressure has risen above 7.5 mmHg.
    m_data.GetPatient().SetEvent(CDM::enumPatientEvent::IntracranialHypertension, false, m_data.GetSimulationTime());
  }

  //---Check Sedatation / Agitation State and output a Richmond Agitation Sedation Scale (RASS) score
  const double painVAS = GetPainVisualAnalogueScale().GetValue();
  const double maxSedationLevel = 0.75;
  const double sedationLevel = std::min(m_data.GetDrugs().GetSedationLevel().GetValue(), maxSedationLevel);
  const double sedationTerm = -std::floor(sedationLevel / 0.15);
  const double agitationTerm = std::ceil((std::pow(5.0, painVAS / 10.0) - 1.0) * (1.0 - sedationLevel / maxSedationLevel));
  const double rassScore = sedationTerm + agitationTerm;
  GetRichmondAgitationSedationScale().SetValue(rassScore);

  //------Fasciculations:-------------------------------------------

  //----Fasciculations due to calcium deficiency (inactive)----------------------------------
  /*if (m_Muscleintracellular.GetSubstanceQuantity(*m_Calcium)->GetConcentration(MassPerVolumeUnit::g_Per_L) < 1.0)
    {
    /// \event Patient: Patient is fasciculating due to calcium deficiency
    m_data.GetPatient().SetEvent(CDM::enumPatientEvent::Fasciculation, true, m_data.GetSimulationTime());
    }
    else if (m_Muscleintracellular.GetSubstanceQuantity(*m_Calcium)->GetConcentration(MassPerVolumeUnit::g_Per_L) > 3.0)
    {
    m_data.GetPatient().SetEvent(CDM::enumPatientEvent::Fasciculation, false, m_data.GetSimulationTime());
    }*/

  //-----Fasciculations due to Sarin--------------------------------------------------
  //Occurs due to inhibition of acetylcholinesterase, the enzyme which breaks down the neurotransmitter acetylcholine
  double RbcAche_mol_Per_L
    = m_data.GetBloodChemistry().GetRedBloodCellAcetylcholinesterase(AmountPerVolumeUnit::mol_Per_L);
  double RbcFractionInhibited = 1.0 - RbcAche_mol_Per_L / (8e-9); //8 nM is the baseline activity of Rbc-Ache
  if (m_data.GetSubstances().IsActive(*m_Sarin)) {
    ///\cite nambda1971cholinesterase
    //The above study found that individuals exposed to the organophosphate parathion did not exhibit fasciculation until at least
    //80% of Rbc-Ache was inhibited.  This was relaxed to 70% because BioGears is calibrated to throw an irreversible state at
    //100% inhibition when, in actuality, a patient with 100% rbc-ache inhibition will likely survive (rbc-ache thought to act as a buffer
    //for neuromuscular ache)
    if (RbcFractionInhibited > 0.7)
      m_data.GetPatient().SetEvent(CDM::enumPatientEvent::Fasciculation, true, m_data.GetSimulationTime());
    else if ((m_data.GetSubstances().IsActive(*m_Sarin)) && (RbcFractionInhibited < 0.68)) {
      //Oscillations around 70% rbc-ache inhibition are highly unlikely but give some leeway for reversal just in case
      m_data.GetPatient().SetEvent(CDM::enumPatientEvent::Fasciculation, false, m_data.GetSimulationTime());
    }
  }
  //----Fasciculations due to Succinylcholine administration.---------------------------------------------------
  //No evidence exists for a correlation between the plasma concentration of succinylcholine
  //and the degree or presence of fasciculation.  Rather, it has been observed that transient fasciculation tends to occur in most patients after initial dosing
  //(particularly at a dose of 1.5 mg/kg, see refs below), subsiding once depolarization at neuromuscular synapses is accomplished.  Therefore, we model this
  //effect by initiating fasciculation when succinylcholine enters the body and removing it when the neuromuscular block level (calculated in Drugs.cpp) reaches
  //90% of maximum.  To prevent fasciculation from being re-flagged as succinylcholine leaves the body and the block dissipates, we use a sentinel (m_blockActive,
  //initialized to FALSE) so that the event cannot be triggered more than once.
  /// \cite @appiah2004pharmacology, @cite mcloughlin1994influence
  double neuromuscularBlockLevel = m_data.GetDrugs().GetNeuromuscularBlockLevel().GetValue();
  if (m_data.GetSubstances().IsActive(*m_Succinylcholine) && (neuromuscularBlockLevel > 0.0)) {
    if ((neuromuscularBlockLevel < 0.9) && (!m_blockActive))
      m_data.GetPatient().SetEvent(CDM::enumPatientEvent::Fasciculation, true, m_data.GetSimulationTime());
    else {
      m_data.GetPatient().SetEvent(CDM::enumPatientEvent::Fasciculation, false, m_data.GetSimulationTime());
      m_blockActive = true;
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets pupil size and reactivity modifiers based on drug and TBI effects
///
/// \details
/// Modifiers are on a scale between -1 (for reduction in size/reactivity) and 1 (for increase)
/// TBI effects are applied to the eye on the same side of the injury if localized or both if diffuse
/// Drug and TBI pupil effects are simply summed together
//--------------------------------------------------------------------------------------------------
void Nervous::SetPupilEffects()
{
  // Get modifiers from Drugs
  double leftPupilSizeResponseLevel = 0.0;
   //m_data.GetDrugs().GetPupillaryResponse().GetSizeModifier().GetValue();
  double leftPupilReactivityResponseLevel = 0.0;
  //m_data.GetDrugs().GetPupillaryResponse().GetReactivityModifier().GetValue();
  double rightPupilSizeResponseLevel = leftPupilSizeResponseLevel;
  double rightPupilReactivityResponseLevel = leftPupilReactivityResponseLevel;

  // Calculate the TBI response
  if (m_data.GetActions().GetPatientActions().HasBrainInjury()) {
    SEBrainInjury* b = m_data.GetActions().GetPatientActions().GetBrainInjury();

    if (b->GetSeverity().GetValue() > 0) {
      double icp_mmHg = m_data.GetCardiovascular().GetIntracranialPressure().GetValue(PressureUnit::mmHg);

      if (b->GetType() == CDM::enumBrainInjuryType::Diffuse) {
        //https://www.wolframalpha.com/input/?i=y%3D(1+%2F+(1+%2B+exp(-2.0*(x+-+24))))+from+18%3Cx%3C28
        leftPupilSizeResponseLevel += (1 / (1 + exp(-2.0 * (icp_mmHg - 20))));
        //https://www.wolframalpha.com/input/?i=y%3D-.001*pow(10,+.27*(x+-+15))+from+18%3Cx%3C28+and+-1%3Cy%3C0
        leftPupilReactivityResponseLevel += -.001 * std::pow(10, .27 * (icp_mmHg - 13));
        rightPupilSizeResponseLevel = leftPupilSizeResponseLevel;
        rightPupilReactivityResponseLevel = leftPupilReactivityResponseLevel;
      } else if (b->GetType() == CDM::enumBrainInjuryType::LeftFocal) {
        leftPupilSizeResponseLevel += (1 / (1 + exp(-2.0 * (icp_mmHg - 20))));
        leftPupilReactivityResponseLevel += -.001 * std::pow(10, .27 * (icp_mmHg - 13));
      } else if (b->GetType() == CDM::enumBrainInjuryType::RightFocal) {
        rightPupilSizeResponseLevel += (1 / (1 + exp(-2.0 * (icp_mmHg - 20))));
        rightPupilReactivityResponseLevel += -.001 * std::pow(10, .27 * (icp_mmHg - 13));
      }
    }
  }

  BLIM(leftPupilSizeResponseLevel, -1, 1);
  BLIM(leftPupilReactivityResponseLevel, -1, 1);
  BLIM(rightPupilSizeResponseLevel, -1, 1);
  BLIM(rightPupilReactivityResponseLevel, -1, 1);
  GetLeftEyePupillaryResponse().GetSizeModifier().SetValue(leftPupilSizeResponseLevel);
  GetLeftEyePupillaryResponse().GetReactivityModifier().SetValue(leftPupilReactivityResponseLevel);
  GetRightEyePupillaryResponse().GetSizeModifier().SetValue(rightPupilSizeResponseLevel);
  GetRightEyePupillaryResponse().GetReactivityModifier().SetValue(rightPupilReactivityResponseLevel);
}
}