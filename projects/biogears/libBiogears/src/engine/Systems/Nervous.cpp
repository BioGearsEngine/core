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
  m_AfferentPulmonaryStretchReceptor_Hz = 0.0;
  m_AfferentStrain = 0.04226;
  m_BaroreceptorOperatingPoint_mmHg = m_data.GetCardiovascular().GetSystolicArterialPressure(PressureUnit::mmHg);
  m_CardiopulmonaryInputBaseline_mmHg = m_data.GetCardiovascular().GetCentralVenousPressure(PressureUnit::mmHg) - (m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::PleuralCavity)->GetPressure(PressureUnit::mmHg) - m_data.GetCompartments().GetGasCompartment(BGE::EnvironmentCompartment::Ambient)->GetPressure(PressureUnit::mmHg));
  m_CardiopulmonaryInput_mmHg = m_CardiopulmonaryInputBaseline_mmHg;
  m_CentralFrequencyDelta_Per_min = 0.0;
  m_CentralPressureDelta_cmH2O = 0.0;
  m_CerebralArteriesEffectors_Large = std::vector<double>(3);
  m_CerebralArteriesEffectors_Small = std::vector<double>(3);
  m_CerebralOxygenSaturationBaseline = 0.0;
  m_CerebralPerfusionPressureBaseline_mmHg = m_data.GetCardiovascular().GetCerebralPerfusionPressure(PressureUnit::mmHg);
  m_CerebralBloodFlowBaseline_mL_Per_s = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralCapillariesToCerebralVeins1)->GetFlow(VolumePerTimeUnit::mL_Per_s);
  m_CerebralBloodFlowInput_mL_Per_s = m_CerebralBloodFlowBaseline_mL_Per_s;
  m_ChemoreceptorFiringRateSetPoint_Hz = m_AfferentChemoreceptor_Hz;
  m_ComplianceModifier = -300.0;
  m_HeartElastanceModifier = 1.0;
  m_HeartRateModifierSympathetic = -0.14;
  m_HeartRateModifierVagal = 0.32;
  m_HypocapniaThresholdHeart = 0.0;
  m_HypocapniaThresholdPeripheral = 0.0;
  m_HypoxiaThresholdHeart = 0.0;
  m_HypoxiaThresholdPeripheral = 0.0;
  m_IntrinsicHeartPeriod_s = 0.65; //Approx guess corresponding to 92 bpm -- should be higher than baseline since baseline assumes some vagal outflow
  m_HeartOxygenBaseline = m_data.GetCompartments().GetIntracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Myocardium)).GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  m_MeanLungVolume_L = m_data.GetRespiratory().GetTotalLungVolume(VolumeUnit::L);
  m_MeanPleuralPressure_mmHg = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::PleuralCavity)->GetPressure(PressureUnit::mmHg) - m_data.GetCompartments().GetGasCompartment(BGE::EnvironmentCompartment::Ambient)->GetPressure(PressureUnit::mmHg);
  m_MuscleOxygenBaseline = m_data.GetCompartments().GetIntracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle)).GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  m_OxygenAutoregulatorHeart = 0.0;
  m_OxygenAutoregulatorMuscle = 0.0;
  m_ResistanceModifierExtrasplanchnic = 2.2;
  m_ResistanceModifierMuscle = 2.8;
  m_ResistanceModifierSplanchnic = 0.8;
  m_PeripheralBloodGasInteractionBaseline_Hz = 0.0;
  m_PeripheralFrequencyDelta_Per_min = 0.0;
  m_PeripheralPressureDelta_cmH2O = 0.0;
  m_SympatheticPeripheralSignalFatigue = 0.0;

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
  m_AfferentStrain = in.AfferentStrain();
  m_ArterialCarbonDioxideBaseline_mmHg = in.ArterialCarbonDioxideBaseline_mmHg();
  m_ArterialOxygenBaseline_mmHg = in.ArterialOxygenBaseline_mmHg();
  m_BaroreceptorOperatingPoint_mmHg = in.BaroreceptorOperatingPoint_mmHg();
  m_CardiopulmonaryInputBaseline_mmHg = in.CardiopulmonaryInputBaseline_mmHg();
  m_CardiopulmonaryInput_mmHg = in.CardiopulmonaryInput_mmHg();
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
  m_HypocapniaThresholdHeart = in.HypocapniaThresholdHeart();
  m_HypocapniaThresholdPeripheral = in.HypocapniaThresholdPeripheral();
  m_HypoxiaThresholdHeart = in.HypoxiaThresholdHeart();
  m_HypoxiaThresholdPeripheral = in.HypoxiaThresholdPeripheral();
  m_IntrinsicHeartPeriod_s = in.IntrinsicHeartPeriod_s();
  m_MeanLungVolume_L = in.MeanLungVolume_L();
  m_MeanPleuralPressure_mmHg = in.MeanPleuralPressure_mmHg();
  m_MuscleOxygenBaseline = in.MuscleOxygenBaseline();
  m_OxygenAutoregulatorHeart = in.OxygenAutoregulatorHeart();
  m_OxygenAutoregulatorMuscle = in.OxygenAutoregulatorMuscle();
  m_PeripheralBloodGasInteractionBaseline_Hz = in.ChemoreceptorPeripheralBloodGasInteractionBaseline_Hz();
  m_PeripheralFrequencyDelta_Per_min = in.PeripheralFrequencyDelta_Per_min();
  m_PeripheralPressureDelta_cmH2O = in.PeripheralPressureDelta_cmH2O();
  m_ResistanceModifierExtrasplanchnic = in.ResistanceModifierExtrasplanchnic();
  m_ResistanceModifierMuscle = in.ResistanceModifierMuscle();
  m_ResistanceModifierSplanchnic = in.ResistanceModifierSplanchnic();
  m_SympatheticPeripheralSignalFatigue = in.SympatheticPeripheralSignalFatigue();

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
  data.AfferentStrain(m_AfferentStrain);
  data.ArterialCarbonDioxideBaseline_mmHg(m_ArterialCarbonDioxideBaseline_mmHg);
  data.ArterialOxygenBaseline_mmHg(m_ArterialOxygenBaseline_mmHg);
  data.BaroreceptorOperatingPoint_mmHg(m_BaroreceptorOperatingPoint_mmHg);
  data.CardiopulmonaryInputBaseline_mmHg(m_CardiopulmonaryInputBaseline_mmHg);
  data.CardiopulmonaryInput_mmHg(m_CardiopulmonaryInput_mmHg);
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
  data.HypocapniaThresholdHeart(m_HypocapniaThresholdHeart);
  data.HypocapniaThresholdPeripheral(m_HypocapniaThresholdPeripheral);
  data.HypoxiaThresholdHeart(m_HypoxiaThresholdHeart);
  data.HypoxiaThresholdPeripheral(m_HypoxiaThresholdPeripheral);
  data.IntrinsicHeartPeriod_s(m_IntrinsicHeartPeriod_s);
  data.MeanLungVolume_L(m_MeanLungVolume_L);
  data.MeanPleuralPressure_mmHg(m_MeanPleuralPressure_mmHg);
  data.MuscleOxygenBaseline(m_MuscleOxygenBaseline);
  data.OxygenAutoregulatorHeart(m_OxygenAutoregulatorHeart);
  data.OxygenAutoregulatorMuscle(m_OxygenAutoregulatorMuscle);
  data.PeripheralFrequencyDelta_Per_min(m_PeripheralFrequencyDelta_Per_min);
  data.PeripheralPressureDelta_cmH2O(m_PeripheralPressureDelta_cmH2O);
  data.ResistanceModifierExtrasplanchnic(m_ResistanceModifierExtrasplanchnic);
  data.ResistanceModifierMuscle(m_ResistanceModifierMuscle);
  data.ResistanceModifierSplanchnic(m_ResistanceModifierSplanchnic);
  data.SympatheticPeripheralSignalFatigue(m_SympatheticPeripheralSignalFatigue);
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

  m_AfferentBaroreceptor_Hz = 25.15;
  m_AfferentAorticBaroreceptor_Hz = 0.0;
  m_AorticStrain = 0.04;
  m_AfferentCardiopulmonary_Hz = 10.0;
  m_SympatheticHeartSignal_Hz = 4.0;
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

  //Debug tracking
  m_data.GetDataTrack().Probe("Afferent_Baroreceptor", m_AfferentBaroreceptor_Hz);
  m_data.GetDataTrack().Probe("Afferent_Chemoreceptor", m_AfferentChemoreceptor_Hz);
  m_data.GetDataTrack().Probe("Afferent_PulmonaryStretch", m_AfferentPulmonaryStretchReceptor_Hz);
  m_data.GetDataTrack().Probe("Ursino_SympatheticNode", m_SympatheticHeartSignal_Hz);
  m_data.GetDataTrack().Probe("Ursino_SympatheticPeripheral", m_SympatheticPeripheralSignal_Hz);
  m_data.GetDataTrack().Probe("Afferent_Strain", m_AfferentStrain);
  m_data.GetDataTrack().Probe("Ursino_Parasympathetic", m_VagalSignal_Hz);
  m_data.GetDataTrack().Probe("HypoxiaThreshold_Heart", m_HypoxiaThresholdHeart);
  m_data.GetDataTrack().Probe("HypoxiaThreshold_Peripheral", m_HypoxiaThresholdPeripheral);
  m_data.GetDataTrack().Probe("HypocapniaThreshold_Heart", m_HypocapniaThresholdHeart);
  m_data.GetDataTrack().Probe("HypocapniaThreshold_Peripheral", m_HypocapniaThresholdPeripheral);
  m_data.GetDataTrack().Probe("BaroreceptorOperatingPoint", m_BaroreceptorOperatingPoint_mmHg);
  m_data.GetDataTrack().Probe("SympatheticFatigue", m_SympatheticPeripheralSignalFatigue);
  m_data.GetDataTrack().Probe("HeartRateMod_Sympathetic", m_HeartRateModifierSympathetic);
  m_data.GetDataTrack().Probe("HeartRateMod_Vagal", m_HeartRateModifierVagal);
  m_data.GetDataTrack().Probe("IntrinsicHeartRate", 60.0 / m_IntrinsicHeartPeriod_s);
  m_data.GetDataTrack().Probe("ResistanceScale_Muscle", GetResistanceScaleMuscle().GetValue());
  m_data.GetDataTrack().Probe("ResistanceScale_Splanchnic", GetResistanceScaleSplanchnic().GetValue());
  m_data.GetDataTrack().Probe("ResistanceScale_Extrasplanchnic", GetResistanceScaleExtrasplanchnic().GetValue());
  m_data.GetDataTrack().Probe("ResistanceScale_Myocardium", GetResistanceScaleMyocardium().GetValue());
  m_data.GetDataTrack().Probe("Resistance_TotalPeripheral(mmHg_s_Per_mL)", m_data.GetCardiovascular().GetSystemicVascularResistance(FlowResistanceUnit::mmHg_s_Per_mL));
  m_data.GetDataTrack().Probe("ElastanceScale", GetHeartElastanceScale().GetValue());
  m_data.GetDataTrack().Probe("ComplianceScale", GetComplianceScale().GetValue());
  m_data.GetDataTrack().Probe("ComplianceModifier", m_ComplianceModifier);
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

  //Generate afferent lung stretch receptor signal (*ap = afferent pulmonary) -- note that signal only deviates from baselines at supra-normal tidal volumes (i.e. stretching)
  const double tauAP_s = 2.0;
  const double gainAP_Hz_Per_L = 4.4; //23.0; //11.25;
  // const double tidalVolume_L = m_data.GetRespiratory().GetTidalVolume(VolumeUnit::L);
  // const double baselineTidalVolume_L = m_data.GetPatient().GetTidalVolumeBaseline(VolumeUnit::L);
  // const double pulmonaryStretchInput_Hz = gainAP_Hz_Per_L * std::max(baselineTidalVolume_L, tidalVolume_L);
  // const double dFrequencyAP_Hz = (1.0 / tauAP_s) * (-m_AfferentPulmonaryStretchReceptor_Hz + pulmonaryStretchInput_Hz);
  const double totalLungVolume = m_data.GetRespiratory().GetTotalLungVolume(VolumeUnit::L);
  const double dVolume = 0.5 * (-m_MeanLungVolume_L + totalLungVolume);
  m_MeanLungVolume_L += (dVolume * m_dt_s);
  const double dFrequencyAP_Hz = (1.0 / tauAP_s) * (-m_AfferentPulmonaryStretchReceptor_Hz + gainAP_Hz_Per_L * m_MeanLungVolume_L);

  m_AfferentPulmonaryStretchReceptor_Hz += dFrequencyAP_Hz * m_dt_s;

  //Afferent thermal receptors (*AT)--may do these later, but for now leave at baseline so as to not disrupt model
  const double AfferentThermal_Hz = 5.0;
  m_AfferentThermal_Hz = AfferentThermal_Hz;
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
  const double xBasalSH = 3.85; //- 3.6;
  const double oxygenHalfMaxSH = 60.0;
  const double kOxygenSH = 8.0;
  const double xCO2SH = 0.25;
  const double xSatSP = 6.0;
  const double xBasalSP = 1.9; //- 1.7 * 3.6 + 0.34 * 12.0;
  const double oxygenHalfMaxSP = 30.0;
  const double kOxygenSP = 2.0;
  const double xCO2SP = 0.25;
  const double tauIschemia = 30.0;
  const double tauCO2 = 20.0;

  //Update hypoxia thresholds for sympathetic signals -- inactive during initial stabilization and when there is a drug disrupting CNS (opioids)
  const double arterialO2 = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg);
  const double expHypoxiaSH = std::exp((arterialO2 - oxygenHalfMaxSH) / kOxygenSH);
  const double expHypoxiaSP = std::exp((arterialO2 - oxygenHalfMaxSP) / kOxygenSP);
  const double hypoxiaSH = xSatSH / (1.0 + expHypoxiaSH);
  const double hypoxiaSP = xSatSP / (1.0 + expHypoxiaSP);
  const double dHypoxiaThresholdSH = (1.0 / tauIschemia) * (-m_HypoxiaThresholdHeart + hypoxiaSH);
  const double dHypoxiaThresholdSP = (1.0 / tauIschemia) * (-m_HypoxiaThresholdPeripheral + hypoxiaSP);
  //Update hypocapnia thresholds
  const double arterialCO2 = m_data.GetBloodChemistry().GetArterialCarbonDioxidePressure(PressureUnit::mmHg);
  const double dHypocapaniaThresholdSH = (1.0 / tauCO2) * (-m_HypocapniaThresholdHeart + xCO2SH * (arterialCO2 - m_ArterialCarbonDioxideBaseline_mmHg));
  const double dHypocapniaThresholdSP = (1.0 / tauCO2) * (-m_HypocapniaThresholdPeripheral + xCO2SP * (arterialCO2 - m_ArterialCarbonDioxideBaseline_mmHg));

  if (m_FeedbackActive && m_DrugRespirationEffects < ZERO_APPROX) {
    m_HypoxiaThresholdHeart += (dHypoxiaThresholdSH * m_dt_s);
    m_HypoxiaThresholdPeripheral += (dHypoxiaThresholdSP * m_dt_s);
    m_HypocapniaThresholdHeart += (dHypocapaniaThresholdSH * m_dt_s);
    m_HypocapniaThresholdPeripheral += (dHypocapniaThresholdSP * m_dt_s);
  }

  const double firingThresholdSH = xBasalSH - m_HypoxiaThresholdHeart - m_HypocapniaThresholdHeart;
  const double firingThresholdSP = xBasalSP - m_HypoxiaThresholdPeripheral - m_HypocapniaThresholdPeripheral;

  //Weights of sympathetic signal to heart (i.e. sino-atrial node)--AB = Afferent baroreceptor, AC = Afferent chemoreceptor, AA = Afferent atrial (cardiopulmonary), AT = Afferent thermal
  const double wSH_AB = -1.0;
  const double wSH_AC = 1.0; // 2.8
  const double wSH_AT = -1.0;
  const double wSH_AL = -1.38;
  const double afferentCardiopulmonaryBaseline_Hz = 10.0;

  const double exponentSH = kS * (0.4 * wSH_AB * m_AfferentBaroreceptor_Hz + 0.6 * wSH_AB * m_AfferentAorticBaroreceptor_Hz + wSH_AC * m_AfferentChemoreceptor_Hz + wSH_AL * (m_AfferentCardiopulmonary_Hz - afferentCardiopulmonaryBaseline_Hz) - firingThresholdSH);
  m_SympatheticHeartSignal_Hz = fSInf + (fS0 - fSInf) * std::exp(exponentSH);
  m_SympatheticHeartSignal_Hz = std::min(m_SympatheticHeartSignal_Hz, fSMax);

  //Weights of sympathetic signal to peripheral vascular beds--AB, AC, AT as before, AP = Afferent pulmonary stretch receptors, AA = Afferent atrial stretch receptors
  const double wSP_AB = -1.13;
  const double wSP_AC = 1.716; //4.0
  const double wSP_AP = -0.34;
  const double wSP_AL = -1.38;
  const double wSP_AT = 1.0;

  const double exponentSP = kS * (0.6 * wSP_AB * m_AfferentBaroreceptor_Hz + 0.4 * wSP_AB * m_AfferentAorticBaroreceptor_Hz + wSP_AC * m_AfferentChemoreceptor_Hz + wSP_AP * m_AfferentPulmonaryStretchReceptor_Hz + wSP_AL * (m_AfferentCardiopulmonary_Hz - afferentCardiopulmonaryBaseline_Hz) - firingThresholdSP);
  m_SympatheticPeripheralSignal_Hz = fSInf + (fS0 - fSInf) * std::exp(exponentSP);
  m_SympatheticPeripheralSignal_Hz = std::min(m_SympatheticPeripheralSignal_Hz, fSMax);

  //Model fatigue of sympathetic peripheral response during sepsis -- Future work should investigate relevance of fatigue in other scenarios
  //Currently applying only to the peripheral signal because the literature notes that vascular smooth muscle shows depressed responsiveness to sympathetic activiy,
  //(Sayk et al., 2008 and Brassard et al., 2016) which would inhibit ability to increase peripheral resistance
  if (m_data.GetBloodChemistry().GetInflammatoryResponse().HasInflammationSource(CDM::enumInflammationSource::Infection)) {
    //Baseline sympathetic signal is 0.2 -- don't begin accumulating fatigue until we pass this threshold (determined empirically)
    const double thresholdFatigue = 8.0;
    const double tauFatigue_hr = 2.0;
    const double peripheralGain = 1.0;
    const double inputFatigue = m_SympatheticPeripheralSignal_Hz - thresholdFatigue;
    double dFatigueScale = 0.0;
    if (inputFatigue > 0.0) {
      //If we are above the threshold, accumulate fatigue up to a maximum of the peripheral resitance gain (defined as 0.6 in Nervous::EfferentResponse)
      dFatigueScale = (1.0 / tauFatigue_hr) * inputFatigue * (peripheralGain - m_SympatheticPeripheralSignalFatigue);
    } else if (m_SympatheticPeripheralSignalFatigue > ZERO_APPROX) {
      //If we have fatigue signal but have dropped below threshold, lower the fatigue signal exponentially towards 0
      dFatigueScale = (-2.0 * m_SympatheticPeripheralSignalFatigue / tauFatigue_hr);
    }
    m_SympatheticPeripheralSignalFatigue += (dFatigueScale * m_data.GetTimeStep().GetValue(TimeUnit::hr));
  }

  //-------Determine vagal (parasympathetic) signal to heart------------------------------------------------------------------
  const double kV = 7.06;
  const double fVInf = 6.3;
  const double fV0 = 3.2;
  const double wV_AC = 0.03; //Was 0.2
  const double wV_AP = -0.103;
  const double hypoxiaThresholdV = 0.72 - 0.17 * 3.6;
  const double baroreceptorBaseline_Hz = 25.15; //This value is the average of the fBaroMin and fBaroMax parameters in BaroreceptorFeedback (NOT reset AtSteadyState because we want continuous signals)

  const double hypoxiaThresholdBase = hypoxiaThresholdV;
  const double fVmin = 0.0;
  const double fVmax = 2.66;
  const double pO2C = 45.0;
  const double kO2 = 29.27;

  const double hypoxiaExp = std::exp((arterialO2 - pO2C) / kO2);

  const double hypoxiaThreshold = (fVmin + fVmax * hypoxiaExp) / (1 + hypoxiaExp) - hypoxiaThresholdBase;

  const double exponentCarotid = (m_AfferentBaroreceptor_Hz - baroreceptorBaseline_Hz) / kV;
  const double exponentAortic = (m_AfferentAorticBaroreceptor_Hz - baroreceptorBaseline_Hz) / kV;
  m_VagalSignal_Hz = 0.5 * (fV0 + fVInf * std::exp(exponentCarotid)) / (1.0 + std::exp(exponentCarotid)) + 0.5 * (fV0 + fVInf * std::exp(exponentAortic)) / (1.0 + std::exp(exponentAortic));
  m_VagalSignal_Hz += (wV_AC * m_AfferentChemoreceptor_Hz + wV_AP * m_AfferentPulmonaryStretchReceptor_Hz - hypoxiaThresholdV);
  m_VagalSignal_Hz = std::max(m_VagalSignal_Hz, 0.0);
}

void Nervous::EfferentResponse()
{
  //Common parameters
  const double fSympatheticMin_Hz = 2.66;

  //Heart Rate
  const double gainHRSympathetic = -0.13;
  const double gainHRVagal = 0.09;
  const double tauHRSympathetic = 2.0;
  const double tauHRVagal = 1.5;

  double hrSympatheticInput = m_SympatheticHeartSignal_Hz > fSympatheticMin_Hz ? gainHRSympathetic * std::log(m_SympatheticHeartSignal_Hz - fSympatheticMin_Hz + 1.0) : 0.0;
  const double hrVagalInput = gainHRVagal * m_VagalSignal_Hz;
  double dHRModifierSympathetic = (1.0 / tauHRSympathetic) * (-m_HeartRateModifierSympathetic + hrSympatheticInput);
  double dHRModifierVagal = (1.0 / 1.5) * (-m_HeartRateModifierVagal + hrVagalInput);
  m_HeartRateModifierSympathetic += (dHRModifierSympathetic * m_dt_s);
  m_HeartRateModifierVagal += (dHRModifierVagal * m_dt_s);
  double nextHeartPeriod_s = m_IntrinsicHeartPeriod_s + m_HeartRateModifierSympathetic + m_HeartRateModifierVagal;

  if (!m_FeedbackActive) {
    double baseHeartPeriod_s = 1.0 / m_data.GetCardiovascular().GetHeartRate(FrequencyUnit::Per_s);
    m_IntrinsicHeartPeriod_s = baseHeartPeriod_s - (m_HeartRateModifierSympathetic + m_HeartRateModifierVagal);
  }

  const double nextHeartRateNorm = (60.0 / nextHeartPeriod_s) / m_data.GetPatient().GetHeartRateBaseline(FrequencyUnit::Per_min);

  //Heart elastance
  double elastanceScaleInput = m_SympatheticHeartSignal_Hz > fSympatheticMin_Hz ? std::log(m_SympatheticHeartSignal_Hz - fSympatheticMin_Hz + 1.0) : 0.0;
  const double elastance0 = 0.518;
  const double gainElastance = 0.48;
  const double tauElastance = 8.0;

  const double dHeartElastanceModifier = (1.0 / tauElastance) * (-m_HeartElastanceModifier + gainElastance * elastanceScaleInput);
  m_HeartElastanceModifier += (dHeartElastanceModifier * m_dt_s);
  const double nextHeartElastanceNorm = m_HeartElastanceModifier / elastance0;

  //Resistance -- Note that gain is suppressed (bounded at 0) when patient accumulates sympathetic fatigue (only occurs during sepsis currently)
  //Might need to consider baseline sympathetic signal here to get right initial gains
  const double tauResistance = 6.0;
  const double gainResistanceExtasplanchnic = 1.94;
  const double gainResistanceMuscle = 2.47;
  const double gainResistanceSplanchnic = 0.695;
  const double extrasplanchnicScale0 = 1.80;
  const double muscleScale0 = 2.3;
  const double splanchnicScale0 = 0.647;

  double resistanceScaleInput = m_SympatheticPeripheralSignal_Hz > fSympatheticMin_Hz ? std::log(m_SympatheticPeripheralSignal_Hz - fSympatheticMin_Hz + 1.0) : 0.0;
  const double dResistanceScaleExtrasplanchnic = (1.0 / tauResistance) * (-m_ResistanceModifierExtrasplanchnic + gainResistanceExtasplanchnic * resistanceScaleInput * (1.0 - m_SympatheticPeripheralSignalFatigue));
  const double dResistanceScaleMuscle = (1.0 / tauResistance) * (-m_ResistanceModifierMuscle + gainResistanceMuscle * resistanceScaleInput * (1.0 - m_SympatheticPeripheralSignalFatigue));
  const double dResistanceScaleSplanchnic = (1.0 / tauResistance) * (-m_ResistanceModifierSplanchnic + gainResistanceSplanchnic * resistanceScaleInput * (1.0 - m_SympatheticPeripheralSignalFatigue));

  m_ResistanceModifierExtrasplanchnic += (dResistanceScaleExtrasplanchnic * m_dt_s);
  m_ResistanceModifierMuscle += (dResistanceScaleMuscle * m_dt_s);
  m_ResistanceModifierSplanchnic += (dResistanceScaleSplanchnic * m_dt_s);

  const double nextExtrasplanchnicResistanceNorm = m_ResistanceModifierExtrasplanchnic / extrasplanchnicScale0;
  const double nextMuscleResistanceNorm = m_ResistanceModifierMuscle / muscleScale0;
  const double nextSplanchnicResistanceNorm = m_ResistanceModifierSplanchnic / splanchnicScale0;

  //Venous Compliance
  double complianceScaleInput = resistanceScaleInput; //Both resistance and compliance modifiers use the sympthetic periperhal signal
  const double complianceScale0 = -326.0;
  const double gainVolume = -350.0;
  const double tauVolume = 20.0;

  const double dComplianceModifier = (1.0 / tauVolume) * (-m_ComplianceModifier + gainVolume * complianceScaleInput);
  m_ComplianceModifier += (dComplianceModifier * m_dt_s);
  //For norm compliance, doing 2.0 - scale makes sure that modifier is properly inverted, since compliance trends opposite the other modifiers above.
  //E.g. modifier is 20% above baseline --> 2.0 - 1.2 = 0.8
  const double nextComplianceNorm = 2.0 - m_ComplianceModifier / complianceScale0;  
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
  //Determine strain on arterial wall caused by pressure changes -- uses Voigt body (spring-dashpot system) to estimate
  const double A = 5.0;
  const double kVoigt = 0.1;
  const double tauVoigt = 0.9;
  const double slopeStrain = 0.04;

  //Pain effect changes the operating point of the baroreceptors.  Anesthetics, sedatives, and opioids blunt the sensivitiy of the baroreflex (reflected in kBaro)
  double painEffect = 0.0;
  double drugEffect = 0.0;
  if (m_data.GetActions().GetPatientActions().HasPainStimulus()) {
    const double painVAS = 0.1 * GetPainVisualAnalogueScale().GetValue();
    painEffect = 0.5 * painVAS * m_BaroreceptorOperatingPoint_mmHg;
  }
  for (SESubstance* sub : m_data.GetCompartments().GetLiquidCompartmentSubstances()) {
    if ((sub->GetClassification() == CDM::enumSubstanceClass::Anesthetic) || (sub->GetClassification() == CDM::enumSubstanceClass::Sedative) || (sub->GetClassification() == CDM::enumSubstanceClass::Opioid)) {
      drugEffect = m_data.GetDrugs().GetMeanBloodPressureChange(PressureUnit::mmHg) / m_data.GetPatient().GetMeanArterialPressureBaseline(PressureUnit::mmHg);
      break;
      //Only want to apply the blood pressure change ONCE (In case there are multiple sedative/opioids/etc)
      ///\TODO:  Look into a better way to implement drug classification search
    }
  }

  double k2 = 0.182;
  double k3 = 828.0;
  double k1 = -k2 * m_ArterialCarbonDioxideBaseline_mmHg - k3 / m_ArterialOxygenBaseline_mmHg; //-13.8;
  double bloodO2 = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg);
  double bloodCO2 = m_data.GetBloodChemistry().GetArterialCarbonDioxidePressure(PressureUnit::mmHg);
  double o2Input = bloodO2 < m_ArterialOxygenBaseline_mmHg ? bloodO2 : m_ArterialOxygenBaseline_mmHg;
  double co2Input = bloodCO2 > m_ArterialCarbonDioxideBaseline_mmHg ? bloodCO2 : m_ArterialCarbonDioxideBaseline_mmHg;

  double peripheralChemoreceptorEffect = k1 + k2 * co2Input + k3 / o2Input;

  m_data.GetDataTrack().Probe("Test_PeripheralDeltaMAP", peripheralChemoreceptorEffect);
  const double baroreceptorOperatingPoint_mmHg = m_BaroreceptorOperatingPoint_mmHg + painEffect + peripheralChemoreceptorEffect;
  m_data.GetDataTrack().Probe("Test_BaroreceptorOperatingPoint", baroreceptorOperatingPoint_mmHg);
  const double systolicPressure_mmHg = m_data.GetCardiovascular().GetSystolicArterialPressure(PressureUnit::mmHg);
  const double strainExp = std::exp(-slopeStrain * (systolicPressure_mmHg - baroreceptorOperatingPoint_mmHg));
  const double wallStrain = 1.0 - std::sqrt((1.0 + strainExp) / (A + strainExp));
  const double dStrain = (1.0 / tauVoigt) * (-m_AfferentStrain + kVoigt * wallStrain);
  m_AfferentStrain += (dStrain * m_dt_s);
  const double strainSignal = wallStrain - m_AfferentStrain;

  //Convert strain signal to be on same basis as Ursino model--this puts deviations in wall strain on same basis as other signals
  const double fBaroMax = 47.78;
  const double fBaroMin = 2.52;
  const double kBaro = 0.075 * (1.0 - 2.0 * drugEffect);
  const double baselineStrainSignal = 0.9 * (1.0 - std::sqrt(1.0 / 3.0)); //Derived by calculating wallStrain when systolic pressure = operating pressure and setting m_AfferentStrain = kVoigt * wallStrain (steady state)
  const double baroExponent = std::exp((strainSignal - baselineStrainSignal) / kBaro);
  m_AfferentBaroreceptor_Hz = (fBaroMin + fBaroMax * baroExponent) / (1.0 + baroExponent);

  //Update setpoint
  if (m_data.GetState() > EngineState::SecondaryStabilization) {
    //Pruett2013Population (cardiovascular hemorrhage model) assumes ~16 hr half-time for baroreceptor adaptation to new setpoint (They varied this parameter up to 1-2 days half-time)
    const double kAdapt_Per_hr = 0.042;
    const double dSetpointAdjust_mmHg_Per_hr = kAdapt_Per_hr * (systolicPressure_mmHg - m_BaroreceptorOperatingPoint_mmHg);
    m_BaroreceptorOperatingPoint_mmHg += (dSetpointAdjust_mmHg_Per_hr * m_data.GetTimeStep().GetValue(TimeUnit::hr));
  }

  //Aortic baroreceptors
  const double pressureInput = systolicPressure_mmHg - m_MeanPleuralPressure_mmHg;
  const double aorticStrainExp = std::exp(-slopeStrain * (pressureInput - baroreceptorOperatingPoint_mmHg));
  const double aorticWallStrain = 1.0 - std::sqrt((1.0 + aorticStrainExp) / (A + aorticStrainExp));
  const double dAorticStrain = (1.0 / tauVoigt) * (-m_AorticStrain + kVoigt * aorticWallStrain);
  m_AorticStrain += (dAorticStrain * m_dt_s);
  const double aorticStrainSignal = aorticWallStrain - m_AorticStrain;
  const double aorticExp = std::exp((aorticStrainSignal - baselineStrainSignal) / kBaro);
  m_AfferentAorticBaroreceptor_Hz = (fBaroMin + fBaroMax * aorticExp) / (1.0 + aorticExp);
  m_data.GetDataTrack().Probe("AorticWallStrain", m_AorticStrain);
  m_data.GetDataTrack().Probe("Afferent_AorticBaroreceptor", m_AfferentAorticBaroreceptor_Hz);

  double environmentPressure = m_data.GetCompartments().GetGasCompartment(BGE::EnvironmentCompartment::Ambient)->GetPressure(PressureUnit::mmHg);
  double absolutePleuralPressure = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::PleuralCavity)->GetPressure(PressureUnit::mmHg);
  double pleuralPressure = absolutePleuralPressure - environmentPressure;
  double dPleuralPressure = 0.1 * (-m_MeanPleuralPressure_mmHg + pleuralPressure);
  m_MeanPleuralPressure_mmHg += (dPleuralPressure * m_dt_s);

  m_data.GetDataTrack().Probe("PleuralPressure", m_MeanPleuralPressure_mmHg);

  //Cardiopulmonary Receptors
  double tZ = 6.0;
  double cvp = m_data.GetCardiovascular().GetCentralVenousPressure(PressureUnit::mmHg);
  double kAff = 3.429;
  double fmax2 = 20.0;

  double dFilterCVP = (1.0 / tZ) * (-m_CardiopulmonaryInput_mmHg + (cvp - m_MeanPleuralPressure_mmHg));
  m_CardiopulmonaryInput_mmHg += (dFilterCVP * m_dt_s);
  double centralExp = std::exp((m_CardiopulmonaryInputBaseline_mmHg - m_CardiopulmonaryInput_mmHg) / kAff);
  m_AfferentCardiopulmonary_Hz = fmax2 / (1.0 + centralExp);

  if (!m_FeedbackActive) {
    m_CardiopulmonaryInputBaseline_mmHg = m_CardiopulmonaryInput_mmHg;
  }

  m_data.GetDataTrack().Probe("Test_FilteredCVP", m_CardiopulmonaryInput_mmHg);
  m_data.GetDataTrack().Probe("Test_CentralSignal", m_AfferentCardiopulmonary_Hz);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the cerebral autoregulation
///
/// \details

//--------------------------------------------------------------------------------------------------
void Nervous::LocalAutoregulation()
{
  //-------------Do cerebral autoregulation first -- this is the most complex one--------------------------------------------------------------------

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
  //const double arterialCO2Pressure = m_ArterialCarbonDioxideBaseline_mmHg;
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

  //---------------------Muscle and heart autoregulation---------------------------------------------------------------------------------
  const double gainHeart_L_Blood_Per_L_O2 = 35.0;
  const double gainMuscle_L_Blood_Per_L_O2 = 240.0;
  const double tauAutoregulator = 10.0;
  const double conversion_L_O2_Per_mmol_O2 = 0.227;
  const double heartOxygenMolarity = m_data.GetCompartments().GetIntracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Myocardium)).GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  const double muscleOxygenMolarity = m_data.GetCompartments().GetIntracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle)).GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);

  const double dHeartAutoEffect = (1.0 / tauAutoregulator) * (-m_OxygenAutoregulatorHeart - gainHeart_L_Blood_Per_L_O2 * conversion_L_O2_Per_mmol_O2 * (heartOxygenMolarity - m_HeartOxygenBaseline));
  const double dMuscleAutoEffect = (1.0 / tauAutoregulator) * (-m_OxygenAutoregulatorMuscle - gainMuscle_L_Blood_Per_L_O2 * conversion_L_O2_Per_mmol_O2 * (muscleOxygenMolarity - m_MuscleOxygenBaseline));
  m_OxygenAutoregulatorHeart += dHeartAutoEffect * m_dt_s;
  m_OxygenAutoregulatorMuscle += dMuscleAutoEffect * m_dt_s;

  double nextMuscleResistance = GetResistanceScaleMuscle().GetValue();
  nextMuscleResistance *= (1.0 / (1.0 + m_OxygenAutoregulatorMuscle));

  if (m_FeedbackActive) {
    GetResistanceScaleMyocardium().SetValue(1.0 / (1.0 + m_OxygenAutoregulatorHeart));
    GetResistanceScaleMuscle().SetValue(nextMuscleResistance);
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
  const double gain_p_F = 2.0; //1.25;
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

  //grab drug effects if there are in the body
  if (m_data.GetDrugs().HasPainToleranceChange()) {
    double NervousScalar = 10.0;
    double PainModifier = m_data.GetDrugs().GetPainToleranceChange().GetValue();
    PainBuffer = exp(-PainModifier * NervousScalar);
  }

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

    tempPainVAS += (painVASMapping * susceptabilityMapping * PainBuffer) / (1 + exp(-m_painStimulusDuration_s + 4.0)); //temp time will increase so long as a stimulus is present
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
  double RbcAche_mol_Per_L = m_data.GetBloodChemistry().GetRedBloodCellAcetylcholinesterase(AmountPerVolumeUnit::mol_Per_L);
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
  double leftPupilSizeResponseLevel = m_data.GetDrugs().GetPupillaryResponse().GetSizeModifier().GetValue();
  double leftPupilReactivityResponseLevel = m_data.GetDrugs().GetPupillaryResponse().GetReactivityModifier().GetValue();
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