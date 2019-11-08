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
  m_TestBaroreceptors = true;
  m_blockActive = false;

  m_CentralFrequencyDelta_Per_min = 0.0;
  m_CentralPressureDelta_cmH2O = 0.0;
  m_PeripheralFrequencyDelta_Per_min = 0.0;
  m_PeripheralPressureDelta_cmH2O = 0.0;


  m_AfferentChemoreceptor_Hz = 3.65;
  m_AfferentPulmonaryStretchReceptor_Hz = 5.75;
  m_AfferentStrain = 0.04;
  m_AfferentStrainBaseline = 0.38;
  m_BaroreceptorBaseline = 25.0;
  m_BaroreceptorFatigueScale = 0.0;
  m_BaroreceptorOperatingPoint_mmHg = m_data.GetPatient().GetSystolicArterialPressureBaseline(PressureUnit::mmHg);

  m_ChemoreceptorFiringRateSetPoint_Hz = m_AfferentChemoreceptor_Hz;
  m_CerebralAutoregulator = 0.0;
  m_CerebralBloodFlowBaseline_mL_Per_s = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralCapillariesToCerebralVeins1)->GetFlow(VolumePerTimeUnit::mL_Per_s);
  //m_CerebralBloodFlowBaseline_mL_Per_s = 17.5;
  m_CerebralBloodFlowInput_mL_Per_s = m_CerebralBloodFlowBaseline_mL_Per_s;
  m_CerebralCarbonDioxideBaseline_mmHg = m_data.GetCompartments().GetExtracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain)).GetSubstanceQuantity(m_data.GetSubstances().GetCO2())->GetPartialPressure(PressureUnit::mmHg);
  m_ComplianceModifier = -50.0;
  m_HeartElastanceModifier = 1.0;
  m_HypoxiaThresholdHeart = 3.59;
  m_HypoxiaThresholdPeripheral = -9.0;
  m_IntrinsicHeartRate = 1.5 * m_data.GetPatient().GetHeartRateBaseline(FrequencyUnit::Per_min); //Approx guess -- should be higher than baseline since baseline assumes some vagal outflow
  m_ResistanceModifier = 0.0;
  m_PeripheralBloodGasInteractionBaseline_Hz = 0.0;
  m_PulmonaryVenousPressureBaseline_mmHg = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetNode(BGE::CardiovascularNode::LeftAtrium1)->GetPressure(PressureUnit::mmHg);
  m_PulmonaryVenousPressureInput_mmHg = m_PulmonaryVenousPressureBaseline_mmHg;
  m_SympatheticHeartSignalBaseline = 0.175;
  m_SympatheticPeripheralSignalBaseline = 0.2;
  m_VagalSignalBaseline = 0.80;


  GetBaroreceptorHeartRateScale().SetValue(1.0);
  GetBaroreceptorHeartElastanceScale().SetValue(1.0);
  GetBaroreceptorResistanceScale().SetValue(1.0);
  GetBaroreceptorComplianceScale().SetValue(1.0);
  GetLeftEyePupillaryResponse().GetSizeModifier().SetValue(0);
  GetLeftEyePupillaryResponse().GetReactivityModifier().SetValue(0);
  GetRightEyePupillaryResponse().GetSizeModifier().SetValue(0);
  GetRightEyePupillaryResponse().GetReactivityModifier().SetValue(0);
  GetPainVisualAnalogueScale().SetValue(0.0);

  GetResistanceScaleCerebral().SetValue(1.0);
  GetResistanceScaleExtrasplanchnic().SetValue(1.0);
  GetResistanceScaleMuscle().SetValue(1.0);
  GetResistanceScaleSplanchnic().SetValue(1.0);
  GetResistanceScaleVentricle().SetValue(1.0);
}

bool Nervous::Load(const CDM::BioGearsNervousSystemData& in)
{
  if (!SENervousSystem::Load(in))
    return false;
  BioGearsSystem::LoadState();
  // We assume state have to be after all stabilization
  m_FeedbackActive = true;
  m_AfferentChemoreceptor_Hz = in.AfferentChemoreceptor_Hz();

  m_ArterialOxygenBaseline_mmHg = in.ArterialOxygenBaseline_mmHg();
  m_ArterialCarbonDioxideBaseline_mmHg = in.ArterialCarbonDioxideBaseline_mmHg();
  m_BaroreceptorFatigueScale = in.BaroreceptorFatigueScale();
  m_ChemoreceptorFiringRateSetPoint_Hz = in.ChemoreceptorFiringRateSetPoint_Hz();
  m_CentralFrequencyDelta_Per_min = in.CentralFrequencyDelta_Per_min();
  m_CentralPressureDelta_cmH2O = in.CentralPressureDelta_cmH2O();
  m_PeripheralFrequencyDelta_Per_min = in.PeripheralFrequencyDelta_Per_min();
  m_PeripheralPressureDelta_cmH2O = in.PeripheralPressureDelta_cmH2O();

  m_AfferentPulmonaryStretchReceptor_Hz = in.AfferentPulmonaryStrechReceptor_Hz();
  m_AfferentStrain = in.AfferentStrain();
  m_AfferentStrainBaseline = in.AfferentStrainBaseline();
  m_BaroreceptorBaseline = in.BaroreceptorBaseline();
  m_BaroreceptorOperatingPoint_mmHg = in.BaroreceptorOperatingPoint_mmHg();
  
  m_CerebralAutoregulator = in.CerebralAutoregulator();
  m_CerebralBloodFlowBaseline_mL_Per_s = in.CerebralBloodFlowBaseline_mL_Per_s();
  m_CerebralBloodFlowInput_mL_Per_s = in.CerebralBloodFlowInput_mL_Per_s();
  m_CerebralCarbonDioxideBaseline_mmHg = in.CerebralCarbonDioxideBaseline_mmHg();
  m_ChemoreceptorFiringRateSetPoint_Hz = in.ChemoreceptorFiringRateSetPoint_Hz();
  m_ComplianceModifier = in.ComplianceModifier();
  m_HeartElastanceModifier = in.HeartElastanceModifier();
  m_HypoxiaThresholdHeart = in.HypoxiaThresholdHeart();
  m_HypoxiaThresholdPeripheral = in.HypoxiaThresholdPeripheral();
  m_IntrinsicHeartRate = in.IntrinsicHeartRate();
  m_PeripheralBloodGasInteractionBaseline_Hz = in.ChemoreceptorPeripheralBloodGasInteractionBaseline_Hz();

  m_PulmonaryVenousPressureBaseline_mmHg = in.PulmonaryVenousPressureBaseline_mmHg();
  m_PulmonaryVenousPressureInput_mmHg = in.PulmonaryVenousPressureInput_mmHg();
  m_ResistanceModifier = in.ResistanceModifier();
  m_SympatheticHeartSignalBaseline = in.SympatheticHeartSignalBaseline();
  m_SympatheticPeripheralSignalBaseline = in.SympatheticPeripheralSignalBaseline();
  m_VagalSignalBaseline = in.VagalSignalBaseline();

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

  data.ArterialOxygenBaseline_mmHg(m_ArterialOxygenBaseline_mmHg);
  data.ArterialCarbonDioxideBaseline_mmHg(m_ArterialCarbonDioxideBaseline_mmHg);
  data.BaroreceptorFatigueScale(m_BaroreceptorFatigueScale);
  data.ChemoreceptorFiringRateSetPoint_Hz(m_ChemoreceptorFiringRateSetPoint_Hz);
  data.CentralFrequencyDelta_Per_min(m_CentralFrequencyDelta_Per_min);
  data.CentralPressureDelta_cmH2O(m_CentralPressureDelta_cmH2O);
  data.PeripheralFrequencyDelta_Per_min(m_PeripheralFrequencyDelta_Per_min);
  data.PeripheralPressureDelta_cmH2O(m_PeripheralPressureDelta_cmH2O);
  
  data.AfferentPulmonaryStrechReceptor_Hz(m_AfferentPulmonaryStretchReceptor_Hz);
  data.AfferentStrain(m_AfferentStrain);
  data.AfferentStrainBaseline(m_AfferentStrainBaseline);
  data.BaroreceptorBaseline(m_BaroreceptorBaseline);
  data.BaroreceptorOperatingPoint_mmHg(m_BaroreceptorOperatingPoint_mmHg);
  data.CerebralAutoregulator(m_CerebralAutoregulator);
  data.CerebralBloodFlowBaseline_mL_Per_s(m_CerebralBloodFlowBaseline_mL_Per_s);
  data.CerebralBloodFlowInput_mL_Per_s(m_CerebralBloodFlowInput_mL_Per_s);
  data.CerebralCarbonDioxideBaseline_mmHg(m_CerebralCarbonDioxideBaseline_mmHg);
  data.ChemoreceptorPeripheralBloodGasInteractionBaseline_Hz(m_PeripheralBloodGasInteractionBaseline_Hz);
  data.ChemoreceptorFiringRateSetPoint_Hz(m_ChemoreceptorFiringRateSetPoint_Hz);
  data.ComplianceModifier(m_ComplianceModifier);
  data.HeartElastanceModifier(m_HeartElastanceModifier);
  data.HypoxiaThresholdHeart(m_HypoxiaThresholdHeart);
  data.HypoxiaThresholdPeripheral(m_HypoxiaThresholdPeripheral);
  data.IntrinsicHeartRate(m_IntrinsicHeartRate);
  data.PulmonaryVenousPressureBaseline_mmHg(m_PulmonaryVenousPressureBaseline_mmHg);
  data.PulmonaryVenousPressureInput_mmHg(m_PulmonaryVenousPressureInput_mmHg);
  data.ResistanceModifier(m_ResistanceModifier);
  data.SympatheticHeartSignalBaseline(m_SympatheticHeartSignalBaseline);
  data.SympatheticPeripheralSignalBaseline(m_SympatheticPeripheralSignalBaseline);
  data.VagalSignalBaseline(m_VagalSignalBaseline);

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
  m_normalizedGammaHeartRate = m_data.GetConfiguration().GetNormalizedHeartRateIntercept();
  m_normalizedGammaElastance = m_data.GetConfiguration().GetNormalizedHeartElastanceIntercept();
  m_normalizedGammaCompliance = m_data.GetConfiguration().GetNormalizedComplianceIntercept();
  m_normalizedGammaResistance = m_data.GetConfiguration().GetNormalizedResistanceIntercept();
  m_normalizedAlphaHeartRate = m_data.GetConfiguration().GetNormalizedHeartRateSympatheticSlope();
  m_normalizedAlphaElastance = m_data.GetConfiguration().GetNormalizedHeartElastanceSympatheticSlope();
  m_normalizedAlphaCompliance = m_data.GetConfiguration().GetNormalizedComplianceParasympatheticSlope();
  m_normalizedAlphaResistance = m_data.GetConfiguration().GetNormalizedResistanceSympatheticSlope();
  m_normalizedBetaHeartRate = m_data.GetConfiguration().GetNormalizedHeartRateParasympatheticSlope();
  m_Succinylcholine = m_data.GetSubstances().GetSubstance("Succinylcholine");
  m_Sarin = m_data.GetSubstances().GetSubstance("Sarin");
  m_Patient = &m_data.GetPatient();

  m_DrugRespirationEffects = 0.0;

  m_painStimulusDuration_s = 0.0;
  m_painVASDuration_s = 0.0;
  m_painVAS = 0.0;

  // Set when feedback is turned on
  m_ArterialOxygenBaseline_mmHg = 0;
  //Reset after stabilization
  m_ArterialCarbonDioxideBaseline_mmHg = 40.0;
  m_CerebralCarbonDioxideSetPoint_mmHg = 45.0;
  m_DrugRespirationEffects = 0.0;



  //Test Values
  m_HeartRateModSympathetic = 0.0;
  m_HeartRateModVagal = 0.0;
  m_ElastanceMod = 0.0;
  m_ComplianceMod = 0.0;
  m_ResistanceMod = 0.0;
  m_AfferentBaroreceptor = 25.0;
  m_BaroreceptorBaseline = 0.38;
  m_AfferentPulmonary = 5.75;
  m_FilteredPulmonaryVenousPressure_Hz = 6.0; //May need to tune depending on BG baseline for LeftAtrium
  m_AfferentAtrial = 9.0; //Same as above
  m_SympatheticHeartSignal = 0.175;
  m_SympatheticPeripheralSignal = 0.175;
  m_VagalSignal = 0.80;
  m_SympatheticHeartSignal_Baseline = m_SympatheticHeartSignal;
  m_SympatheticPeripheralSignal_Baseline = m_SympatheticPeripheralSignal;
  m_VagalSignal_Baseline = m_VagalSignal;
  m_AfferentStrain = 0.04;
  m_PressureHalfMax = m_data.GetPatient().GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
  m_HeartElastanceEffector = 0.0;
  m_CerebralBloodFlowBaseline_mL_Per_s = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralCapillariesToCerebralVeins1)->GetFlow(VolumePerTimeUnit::mL_Per_s);
  m_CerebralBloodFlowFilter = m_CerebralBloodFlowBaseline_mL_Per_s;
  m_CerebralAutoEffect = 0.0;
  m_CerebralCO2Baseline_mmHg = m_data.GetCompartments().GetExtracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain)).GetSubstanceQuantity(m_data.GetSubstances().GetCO2())->GetPartialPressure(PressureUnit::mmHg);

  m_AfferentBaroreceptor_Hz = 25.15;
  m_AfferentAtrial_Hz = 9.0;
  m_SympatheticHeartSignal_Hz = 0.175;
  m_SympatheticPeripheralSignal_Hz = 0.20;
  m_VagalSignal_Hz = 0.80;

}

void Nervous::AtSteadyState()
{
  if (m_data.GetState() == EngineState::AtInitialStableState) {
    m_FeedbackActive = true;
    SetBaroreceptorFrequencyComponents(std::vector<double>(3), FrequencyUnit::Hz); //Vector of 0's
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
   m_ChemoreceptorFiringRateSetPoint_Hz = m_AfferentChemoreceptor_Hz;
  //m_AfferentChemoreceptor_Hz = m_PeripheralBloodGasInteractionBaseline_Hz;

  m_BaroreceptorBaseline = m_AfferentBaroreceptor_Hz;
  m_BaroreceptorOperatingPoint_mmHg = m_data.GetCardiovascular().GetSystolicArterialPressure(PressureUnit::mmHg);
  const double wallStrain = 1.0 - std::sqrt(1.0 / 3.0); //This is the wall strain when arterial pressure = operating point (see BaroreceptorFeedback)
  m_AfferentStrain = 0.1 * wallStrain;
  m_AfferentStrainBaseline = wallStrain - m_AfferentStrain;
  
 
  m_CerebralCarbonDioxideBaseline_mmHg = m_data.GetCompartments().GetExtracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain)).GetSubstanceQuantity(m_data.GetSubstances().GetCO2())->GetPartialPressure(PressureUnit::mmHg);
  m_SympatheticHeartSignalBaseline = m_SympatheticHeartSignal_Hz;
  m_SympatheticPeripheralSignalBaseline = m_SympatheticPeripheralSignal_Hz;
  m_VagalSignalBaseline = m_VagalSignal_Hz;
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
  CerebralAutoregulation();
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
  m_data.GetDataTrack().Probe("AtrialVenousPressure_Filtered", m_PulmonaryVenousPressureBaseline_mmHg);
  m_data.GetDataTrack().Probe("AtrialVenousPressure_Filtered", m_PulmonaryVenousPressureInput_mmHg);
  m_data.GetDataTrack().Probe("Afferent_Atrial", m_AfferentAtrial_Hz);
  m_data.GetDataTrack().Probe("Ursino_SympatheticNode", m_SympatheticHeartSignal_Hz);
  m_data.GetDataTrack().Probe("Ursino_SympatheticPeripheral", m_SympatheticPeripheralSignal_Hz);
  m_data.GetDataTrack().Probe("Afferent_Strain", m_AfferentStrain);
  m_data.GetDataTrack().Probe("BaroreceptorOperatingPoint", m_BaroreceptorOperatingPoint_mmHg);
  m_data.GetDataTrack().Probe("Ursino_Parasympathetic", m_VagalSignal_Hz);
  m_data.GetDataTrack().Probe("HypoxiaThreshold_Heart", m_HypoxiaThresholdHeart);
  m_data.GetDataTrack().Probe("HypoxiaThreshold_Peripheral", m_HypoxiaThresholdPeripheral);
  m_data.GetDataTrack().Probe(m_data.GetCircuits().GetCerebralCircuit());
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
  //Generate afferent barorceptor signal
  BaroreceptorFeedback();

  //Generate afferent chemoreceptor signal -- combined respiration drug effects modifier calculated in this function since it primarily affects chemoreceptors
  ChemoreceptorFeedback();

  //Generate afferent lung stretch receptor signal (*ap = afferent pulmonary) -- push this input towards baseline when anesthetics/opioids are effecting respiratory drive
  const double tauAP_s = 2.0;
  const double gainAP_Hz_Per_L = 11.25;
  const double tidalVolume_L = m_data.GetRespiratory().GetTidalVolume(VolumeUnit::L);
  const double baselineTidalVolume_L = m_data.GetPatient().GetTidalVolumeBaseline(VolumeUnit::L);
  const double pulmonaryStretchInput = gainAP_Hz_Per_L * (tidalVolume_L * std::exp(-5.0 * m_DrugRespirationEffects) + baselineTidalVolume_L * (1.0 - std::exp(-5.0 * m_DrugRespirationEffects)));
  const double dFrequencyAP_Hz = (1.0 / tauAP_s) * (-m_AfferentPulmonaryStretchReceptor_Hz + pulmonaryStretchInput);
  m_AfferentPulmonaryStretchReceptor_Hz += dFrequencyAP_Hz * m_dt_s;

  //Afferent atrial stretch receptors (*aa = afferent atrial)--Input is a filtered venous pressure (less noisy) that is determined first
  const double tauAA_s = 6.37;
  const double maxAA_Hz = 18.0;
  const double kAA_mmHg = 3.429;
  const double pulmonaryVenousPressure_mmHg = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetNode(BGE::CardiovascularNode::LeftAtrium1)->GetPressure(PressureUnit::mmHg);
  const double dFilteredPulmonaryVenousPressure = (1.0 / tauAA_s) * (pulmonaryVenousPressure_mmHg - m_PulmonaryVenousPressureInput_mmHg);
  m_PulmonaryVenousPressureInput_mmHg += dFilteredPulmonaryVenousPressure * m_dt_s;
  const double exponentAA = (m_PulmonaryVenousPressureInput_mmHg - m_PulmonaryVenousPressureBaseline_mmHg) / kAA_mmHg;
  m_AfferentAtrial_Hz = (maxAA_Hz * std::exp(exponentAA)) / (1.0 + std::exp(exponentAA));

  //Afferent thermal receptors (*AT)--may do these later, but for now leave at baseline so as to not disrupt model
  const double AfferentThermal_Hz = 5.0;
  m_AfferentThermal_Hz = AfferentThermal_Hz;
}

void Nervous::CentralSignalProcess()
{
  //--------Determine sympathetic signal to heart (SH) and periphery (SP)-------------------------------------------------------
  //Sympathetic signal constants
  const double kS = 0.0675;
  const double xMinSH = -49.38;
  const double xMaxSH = 3.59;
  const double oxygenHalfMaxSH = 45.0;
  const double kOxygenSH = 6.0;
  const double xMinSP = -15.0;
  const double xMaxSP = -9.0;
  const double oxygenHalfMaxSP = 30.0;
  const double kOxygenSP = 2.0;
  const double tauIschemia = 30.0;

  //Update hypoxia thresholds for sympathetic signals -- inactive during initial stabilization and when there is a drug disrupting CNS (opioids)
  const double arterialO2 = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg);
  const double expHypoxiaSH = std::exp((arterialO2 - oxygenHalfMaxSH) / kOxygenSH);
  const double expHypoxiaSP = std::exp((arterialO2 - oxygenHalfMaxSP) / kOxygenSP);
  const double hypoxiaSH = (xMinSH + xMaxSH * expHypoxiaSH) / (1.0 + expHypoxiaSH);
  const double hypoxiaSP = (xMinSP + xMaxSP * expHypoxiaSP) / (1.0 + expHypoxiaSP);
  const double dHypoxiaThresholdSH = (1.0 / tauIschemia) * (-m_HypoxiaThresholdHeart + hypoxiaSH);
  const double dHypoxiaThresholdSP = (1.0 / tauIschemia) * (-m_HypoxiaThresholdPeripheral + hypoxiaSP);

  if (m_FeedbackActive && m_DrugRespirationEffects < ZERO_APPROX) {
    m_HypoxiaThresholdHeart += (dHypoxiaThresholdSH * m_dt_s);
    m_HypoxiaThresholdPeripheral += (dHypoxiaThresholdSP * m_dt_s);
  }
  //Weights of sympathetic signal to heart (i.e. sino-atrial node)--AB = Afferent baroreceptor, AC = Afferent chemoreceptor, AT = Afferent thermal
  const double wSH_AB = -1.0;
  const double wSH_AC = 1.0;
  const double wSH_AT = -1.0;
  const double exponentSH = kS * (wSH_AB * m_AfferentBaroreceptor_Hz + wSH_AC * m_AfferentChemoreceptor_Hz - m_HypoxiaThresholdHeart);
  m_SympatheticHeartSignal_Hz = std::exp(exponentSH);

  //Weights of sympathetic signal to peripheral vascular beds--AB, AC, AT as before, AP = Afferent pulmonary stretch receptors, AA = Afferent atrial stretch receptors
  const double wSP_AB = -1.13;
  const double wSP_AC = 1.716;
  const double wSP_AP = -0.34;
  const double wSP_AA = -1.0;
  const double wSP_AT = 1.0;
  const double exponentSP = kS * (wSP_AB * m_AfferentBaroreceptor_Hz + wSP_AC * m_AfferentChemoreceptor_Hz + wSP_AP * m_AfferentPulmonaryStretchReceptor_Hz + wSP_AA * m_AfferentAtrial_Hz - m_HypoxiaThresholdPeripheral);
  m_SympatheticPeripheralSignal_Hz = std::exp(exponentSP);

  //-------Determine vagal (parasympathetic) signal to heart------------------------------------------------------------------
  const double kV = 7.06;
  const double wV_AC = 0.2;
  const double wV_AP = 0.103;
  const double hypoxiaThresholdV = 0.0; //Currently not adjusted like symptathetic values
  const double baroreceptorScaleFactor = 1.2;
  const double exponentV = (m_AfferentBaroreceptor_Hz - m_BaroreceptorBaseline) / kV;
  m_VagalSignal_Hz = baroreceptorScaleFactor * std::exp(exponentV) / (1.0 + std::exp(exponentV));
  m_VagalSignal_Hz += (wV_AC * m_AfferentChemoreceptor_Hz - wV_AP * m_AfferentPulmonaryStretchReceptor_Hz - hypoxiaThresholdV);
}

void Nervous::EfferentResponse()
{
  //Heart Rate
  const double gainVagalHR = 0.5;
  const double gainSympatheticHR = 0.3;
  const double nextHR = m_IntrinsicHeartRate * (1.0 - gainVagalHR * m_VagalSignal_Hz + gainSympatheticHR * m_SympatheticHeartSignal_Hz);
  if (!m_FeedbackActive) {
    //Keep resetting intrinsic heart rate so that when feedback is turned on it is tuned to the vagal and sympathethic signal baselines
    m_IntrinsicHeartRate = m_data.GetPatient().GetHeartRateBaseline(FrequencyUnit::Per_min) / (1.0 - gainVagalHR * m_VagalSignal_Hz + gainSympatheticHR * m_SympatheticHeartSignal_Hz);
  }
  m_data.GetDataTrack().Probe("Randall_HR", nextHR);
  m_data.GetDataTrack().Probe("HR_Intrinsic", m_IntrinsicHeartRate);
  m_data.GetDataTrack().Probe("HR_Scaled", nextHR / m_data.GetPatient().GetHeartRateBaseline(FrequencyUnit::Per_min));
  //Heart elastance
  const double baseElastance = 2.49;
  const double gainElastance = 0.4;
  const double tauElastance = 2.0;
  const double initialElastance = baseElastance - gainElastance * m_SympatheticHeartSignalBaseline; //Different than baseline because sympathetic signal is non-zero

  const double dHeartElastance = (1.0 / tauElastance) * (-m_HeartElastanceModifier + gainElastance * m_SympatheticHeartSignal_Hz);
  m_HeartElastanceModifier += (dHeartElastance * m_dt_s);

  m_data.GetDataTrack().Probe("LeftHeartElastance_Mod", m_HeartElastanceModifier);
  m_data.GetDataTrack().Probe("LeftHeartElastance_Next", m_HeartElastanceModifier + initialElastance);
  m_data.GetDataTrack().Probe("ElastanceFracChanged", (m_HeartElastanceModifier + initialElastance) / baseElastance);

  //Resistance
  const double tauResistance = 3.0;
  const double gainResistance = 0.6;
  const double baseR_mmHg_s_Per_mL = 1.0 - gainResistance * m_SympatheticPeripheralSignalBaseline;

  const double dResistance = (1.0 / tauResistance) * (-m_ResistanceModifier + gainResistance * m_SympatheticPeripheralSignal_Hz);
  m_ResistanceModifier += (dResistance * m_dt_s);
  m_data.GetDataTrack().Probe("Resistance_Mod", m_ResistanceModifier);
  m_data.GetDataTrack().Probe("Resistance_Next", baseR_mmHg_s_Per_mL + m_ResistanceModifier);

  //Venous Compliance
  const double baseVolume = 3200.0;
  const double gainVolume = -400.0;
  const double tauVolume = 10.0;
  const double initialVolume = baseVolume - gainVolume * m_SympatheticPeripheralSignalBaseline; //Different than baseline volume because baseline sympathetic signal is non-zero

  const double dVolume = (1.0 / tauVolume) * (-m_ComplianceModifier + gainVolume * m_SympatheticPeripheralSignal_Hz);
  m_ComplianceModifier += (dVolume * m_dt_s);

  m_data.GetDataTrack().Probe("Volume_Mod", m_ComplianceModifier);
  m_data.GetDataTrack().Probe("Compliance_Fraction", (initialVolume + m_ComplianceModifier) / baseVolume);

  m_data.GetDataTrack().Probe("LeftLungResistance", m_data.GetCircuits().GetFluidPath(BGE::CardiovascularPath::LeftIntermediatePulmonaryArteriesToLeftPulmonaryArteries)->GetResistance(FlowResistanceUnit::mmHg_s_Per_mL));
  m_data.GetDataTrack().Probe("RightLungResistance", m_data.GetCircuits().GetFluidPath(BGE::CardiovascularPath::RightIntermediatePulmonaryArteriesToRightPulmonaryArteries)->GetResistance(FlowResistanceUnit::mmHg_s_Per_mL));
  if (m_FeedbackActive) {
    GetBaroreceptorHeartRateScale().SetValue(nextHR / m_data.GetPatient().GetHeartRateBaseline(FrequencyUnit::Per_min));
    GetBaroreceptorHeartElastanceScale().SetValue((m_HeartElastanceModifier + initialElastance) / baseElastance);
    GetBaroreceptorResistanceScale().SetValue(baseR_mmHg_s_Per_mL + m_ResistanceModifier);
    GetBaroreceptorComplianceScale().SetValue((initialVolume + m_ComplianceModifier) / baseVolume);
  }
}

void Nervous::CerebralAutoregulation()
{
  const double tauAuto = 40.0;
  const double complianceGainHigh = 2.87;
  const double complianceGainLow = 0.11;
  double Kr = 4.96e4;
  const double complianceMid = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralArteries1ToSpinalFluid)->GetComplianceBaseline(FlowComplianceUnit::mL_Per_mmHg);
  double complianceGain = 0.0;
  double complianceSlope = 0.0;
  double filterConstant = m_FeedbackActive ? 0.5 : 0.1;

  const double cranialCO2_mmHg = m_data.GetCompartments().GetExtracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain)).GetSubstanceQuantity(m_data.GetSubstances().GetCO2())->GetPartialPressure(PressureUnit::mmHg);
  double cbfBaseDelta = 1.8 / (1.0 + std::exp(-0.06 * (cranialCO2_mmHg - 57.0))) - 0.6;
  cbfBaseDelta = 0.0; //Still tuning--for now leave cbf at baseline
  const double cbfBaseline_CO2Adjust = m_CerebralBloodFlowBaseline_mL_Per_s * (1.0 + cbfBaseDelta);
  const double kAuto = 18.0 / (1.0 + std::exp(2.0 * (cranialCO2_mmHg - m_CerebralCarbonDioxideBaseline_mmHg) / m_CerebralCarbonDioxideBaseline_mmHg));
  const double cerebralBloodFlow_mL_Per_s = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralVeins2ToNeckVeins)->GetFlow(VolumePerTimeUnit::mL_Per_s);
  const double dCerebralBloodFlow = filterConstant * (-m_CerebralBloodFlowInput_mL_Per_s + cerebralBloodFlow_mL_Per_s);
  m_CerebralBloodFlowInput_mL_Per_s += (dCerebralBloodFlow * m_dt_s);

  if (!m_FeedbackActive) {
    m_CerebralBloodFlowBaseline_mL_Per_s = m_CerebralBloodFlowInput_mL_Per_s;
  }
  const double dAuto = (1.0 / tauAuto) * (-m_CerebralAutoregulator + kAuto * (m_CerebralBloodFlowInput_mL_Per_s - cbfBaseline_CO2Adjust) / cbfBaseline_CO2Adjust);
  m_CerebralAutoregulator += (dAuto * m_dt_s);
 
  if (m_CerebralAutoregulator < 0.0) {
    complianceGain = complianceGainHigh;
    complianceSlope = complianceGainHigh / 4.0;
  } else {
    complianceGain = complianceGainLow;
    complianceSlope = complianceGainLow / 4.0;
  }

  const double nextCompliance = ((complianceMid - 0.5 * complianceGain) + (complianceMid + 0.5 * complianceGain) * std::exp(-m_CerebralAutoregulator / complianceSlope)) / (1.0 + std::exp(-m_CerebralAutoregulator / complianceSlope));
  const double cerebralArteryVolume_mL = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::CerebralArteries)->GetVolume(VolumeUnit::mL);
  const double cerebralArteryVolumeBase = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetNode(BGE::CerebralNode::CerebralArteries1)->GetVolumeBaseline(VolumeUnit::mL);
  const double cerebralResistanceBase = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralArteries2ToCapillaries)->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
  Kr = cerebralResistanceBase / (std::pow(complianceMid / 12.5, 2.0));
  const double nextResistance = Kr * std::pow(complianceMid / cerebralArteryVolume_mL, 2.0);

  //if (m_data.GetState() >= EngineState::AtSecondaryStableState) {
    m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralArteries1ToSpinalFluid)->GetNextCompliance().SetValue(nextCompliance, FlowComplianceUnit::mL_Per_mmHg);
    m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::CerebralPath::CerebralArteries2ToCapillaries)->GetNextResistance().SetValue(nextResistance, FlowResistanceUnit::mmHg_s_Per_mL);
  //}

  m_data.GetDataTrack().Probe("Cerebral_CO2", cranialCO2_mmHg);
  m_data.GetDataTrack().Probe("Cerebral_Compliance", nextCompliance);
  m_data.GetDataTrack().Probe("Cerebral_Resistance", nextResistance);
  m_data.GetDataTrack().Probe("Cerebral_Volume", cerebralArteryVolume_mL);
  m_data.GetDataTrack().Probe("Cerebral_FilterFlow", m_CerebralBloodFlowInput_mL_Per_s);
  m_data.GetDataTrack().Probe("Cerebral_Baseline", m_CerebralBloodFlowBaseline_mL_Per_s);
  m_data.GetDataTrack().Probe("Cerebral_Flow", cerebralBloodFlow_mL_Per_s);
  m_data.GetDataTrack().Probe("Cerebral_KrConstant", Kr);
  m_data.GetDataTrack().Probe("Cerebral_AutoEffect", m_CerebralAutoregulator);
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

  const double baroreceptorOperatingPoint_mmHg = m_BaroreceptorOperatingPoint_mmHg + painEffect;

  const double systolicPressure_mmHg = m_data.GetCardiovascular().GetSystolicArterialPressure(PressureUnit::mmHg);
  const double strainExp = std::exp(-slopeStrain * (systolicPressure_mmHg - baroreceptorOperatingPoint_mmHg));
  const double wallStrain = 1.0 - std::sqrt((1.0 + strainExp) / (A + strainExp));
  const double dStrain = (1.0 / tauVoigt) * (-m_AfferentStrain + kVoigt * wallStrain);
  m_AfferentStrain += (dStrain * m_dt_s);
  const double strainSignal = wallStrain - m_AfferentStrain;
  m_data.GetDataTrack().Probe("WallStrain", wallStrain);
  m_data.GetDataTrack().Probe("StrainSignal", strainSignal);

  //Convert strain signal to be on same basis as Ursino model--this puts deviations in wall strain on same basis as other signals
  const double fBaroMax = 47.78;
  const double fBaroMin = 2.52;
  const double kBaro = 0.075 * (1.0 - 2.0 * drugEffect);
  m_data.GetDataTrack().Probe("BaroSlope", kBaro);
  const double baroExponent = std::exp((strainSignal - m_AfferentStrainBaseline) / kBaro);
  m_AfferentBaroreceptor_Hz = (fBaroMin + fBaroMax * baroExponent) / (1.0 + baroExponent);

  //Update setpoint
  if (m_data.GetState() > EngineState::SecondaryStabilization) {
    const double kAdapt = 7.0e-5;
    double dSetpointAdjust = kAdapt * (systolicPressure_mmHg - m_BaroreceptorOperatingPoint_mmHg);
    m_BaroreceptorOperatingPoint_mmHg += (dSetpointAdjust * m_dt_s);
  }

  if (!m_FeedbackActive) {
    return;
  }

  double meanArterialPressure_mmHg = m_data.GetCardiovascular().GetMeanArterialPressure(PressureUnit::mmHg);

  //First calculate the sympathetic and parasympathetic firing rates:
  double nu = m_data.GetConfiguration().GetResponseSlope();

  //Adjusting the mean arterial pressure set-point to account for cardiovascular drug effects
  double meanArterialPressureSetPoint_mmHg = m_data.GetPatient().GetMeanArterialPressureBaseline(PressureUnit::mmHg) //m_MeanArterialPressureNoFeedbackBaseline_mmHg
    + m_data.GetEnergy().GetExerciseMeanArterialPressureDelta(PressureUnit::mmHg);

  double sympatheticFraction = 1.0 / (1.0 + std::pow(meanArterialPressure_mmHg / meanArterialPressureSetPoint_mmHg, nu));
  double parasympatheticFraction = 1.0 / (1.0 + std::pow(meanArterialPressure_mmHg / meanArterialPressureSetPoint_mmHg, -nu));


  //Currently baroreceptor fatigue has only been tested for severe infections leading to sepsis.  We only accumulate fatigue if the sympathetic
  // outflow is above a certain threshold.  If we drop below threshold, we allow fatigue parameter to return towards 0. Note that even if infetion
  //is eliminated, the inflammation source will still be found (which we want so that inflammatory model has time to return to baseline).
  const double fatigueThreshold = 0.65;
  const double fatigueTimeConstant_hr = 2.0;
  double fatigueInput = sympatheticFraction - fatigueThreshold;
  double dFatigueScale = 0.0;
  if (m_data.GetBloodChemistry().GetInflammatoryResponse().HasInflammationSource(CDM::enumInflammationSource::Infection)) {
    if (fatigueInput > 0.0) {
      dFatigueScale = (1.0 / fatigueTimeConstant_hr) * (fatigueInput) * (1.2 - m_BaroreceptorFatigueScale);
    } else if (m_BaroreceptorFatigueScale > ZERO_APPROX) {
      dFatigueScale = (-2.0 * m_BaroreceptorFatigueScale / fatigueTimeConstant_hr);
    }
    m_BaroreceptorFatigueScale += (dFatigueScale * m_data.GetTimeStep().GetValue(TimeUnit::hr));
  }
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
  double drugCNSModifier = m_data.GetDrugs().GetCentralNervousResponse().GetValue();

  //-------Respiratory Feedback:  This is active throughtout the simulation (including stabilization)------------------------------
  //Chemoreceptor time constants (all in s)
  const double tau_p_P = 100.0;
  const double tau_p_F = 100.0;
  const double tau_c_P = 180.0;
  const double tau_c_F = 180.0;
  //Chemoreceptor gains--Tuned to data from Reynold, 1972 and Reynolds, 1973 (cited in Cheng, 2016)
  const double gain_p_P = 1.0;
  const double gain_p_F = 0.70;
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
  const double afferentFiringInput = psi * std::exp(-3.5 * m_DrugRespirationEffects) + m_ChemoreceptorFiringRateSetPoint_Hz * (1.0 - std::exp(-3.5 * m_DrugRespirationEffects));
  const double centralInput = (arterialCO2Pressure_mmHg - m_ArterialCarbonDioxideBaseline_mmHg) * std::exp(-3.5 * m_DrugRespirationEffects);
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
  //Old driver multiplied a target ventilation by the metabolic modifier.  Since Vent (L/min) = RR(/min) * TV(L), we'll mulitply
  //the next respiration rate and next drive pressure by sqrt(modifier).  Relationship between driver pressure and TV is
  //approx linear (e.g. 10% change in pressure -> 10% change in TV), so this should achieve desired effect
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

  //-----Cardiovascular Feedback:  This functionality is currently only active after stabilization.
  if (!m_FeedbackActive)
    return;

  //Heart Rate modifications
  double normalized_pO2 = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg) / m_ArterialOxygenBaseline_mmHg;
  double normalized_pCO2 = m_data.GetBloodChemistry().GetArterialCarbonDioxidePressure(PressureUnit::mmHg) / m_ArterialCarbonDioxideBaseline_mmHg;

  // The chemoreceptor heart rate modification function shape parameters.
  // See NervousMethodology documentation for details.
  double amax = -0.1;
  double a50 = 0.5;
  double aeta = -12.;
  double bmax = 1.;
  double b50 = 1.7;
  double beta = 18;
  double cmax = 1.;
  double c50 = 0.65;
  double ceta = -20;
  double dmax = -0.1;
  double d50 = b50;
  double deta = -aeta;

  //Calculate the normalized change in heart rate
  //double HRBaseline_per_min = m_HeartRateNoFeedbackBaseline_per_min;
  // Maximum HR delta is 1.23 times baseline. The derivation of this maximum is described in the NervousMethodology documentation
  double maxHeartRateDelta = 1.23 * m_data.GetPatient().GetHeartRateBaseline(FrequencyUnit::Per_min);
  double modifier = GeneralMath::LogisticFunction(amax, a50, aeta, normalized_pCO2);
  modifier += GeneralMath::LogisticFunction(bmax, b50, beta, normalized_pCO2);
  modifier += GeneralMath::LogisticFunction(cmax, c50, ceta, normalized_pO2);
  modifier += GeneralMath::LogisticFunction(dmax, d50, deta, normalized_pO2);

  //Apply central nervous depressant effects (currently only applies to morphine)

  if (drugCNSModifier >= 0.25)
    modifier = 0.0;

  //set to zero if below .1 percent
  if (modifier < 0.001)
    modifier = 0.0;

  //GetChemoreceptorHeartRateScale().SetValue(maxHeartRateDelta * modifier);

  // Calculate the normalized change in heart elastance
  double normalizedHeartElastance = 1.0;
  /// \todo Compute and apply chemoreceptor-mediated contractility changes
  //GetChemoreceptorHeartElastanceScale().SetValue(normalizedHeartElastance);
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