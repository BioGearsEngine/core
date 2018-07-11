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
#include <biogears/engine/Systems/Nervous.h>
#include <biogears/engine/stdafx.h>
#include <biogears/schema/BioGearsNervousSystemData.hxx>

#pragma warning(disable : 4786)
#pragma warning(disable : 4275)

// #define VERBOSE

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
  GetBaroreceptorHeartRateScale().SetValue(1.0);
  GetBaroreceptorHeartElastanceScale().SetValue(1.0);
  GetBaroreceptorResistanceScale().SetValue(1.0);
  GetBaroreceptorComplianceScale().SetValue(1.0);
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
  m_ArterialOxygenSetPoint_mmHg = in.ArterialOxygenSetPoint_mmHg();
  m_ArterialCarbonDioxideSetPoint_mmHg = in.ArterialCarbonDioxideSetPoint_mmHg();
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
  data.ArterialOxygenSetPoint_mmHg(m_ArterialOxygenSetPoint_mmHg);
  data.ArterialCarbonDioxideSetPoint_mmHg(m_ArterialCarbonDioxideSetPoint_mmHg);
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

  m_painStimulusDuration_s = 0.0;
  m_painVASDuration_s = 0.0;
  m_painVAS = 0.0;

  // Set when feedback is turned on
  m_ArterialOxygenSetPoint_mmHg = 0;
  m_ArterialCarbonDioxideSetPoint_mmHg = 0;
}

void Nervous::AtSteadyState()
{
  if (m_data.GetState() == EngineState::AtInitialStableState)
    m_FeedbackActive = true;

  // The set-points (Baselines) get reset at the end of each stabilization period.
  m_ArterialOxygenSetPoint_mmHg = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg);
  m_ArterialCarbonDioxideSetPoint_mmHg = m_data.GetBloodChemistry().GetArterialCarbonDioxidePressure(PressureUnit::mmHg);
  // The baroreceptor scales need to be reset any time the baselines are reset.
  GetBaroreceptorHeartRateScale().SetValue(1.0);
  GetBaroreceptorHeartElastanceScale().SetValue(1.0);
  GetBaroreceptorResistanceScale().SetValue(1.0);
  GetBaroreceptorComplianceScale().SetValue(1.0);
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
  BaroreceptorFeedback();
  ChemoreceptorFeedback();
  CheckPainStimulus();
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
  if (!m_FeedbackActive)
    return;
  //First calculate the sympathetic and parasympathetic firing rates:
  double nu = m_data.GetConfiguration().GetResponseSlope();
  double meanArterialPressure_mmHg = m_data.GetCardiovascular().GetMeanArterialPressure(PressureUnit::mmHg);
  //Adjusting the mean arterial pressure set-point to account for cardiovascular drug effects
  double meanArterialPressureSetPoint_mmHg = m_data.GetPatient().GetMeanArterialPressureBaseline(PressureUnit::mmHg) //m_MeanArterialPressureNoFeedbackBaseline_mmHg
    + m_data.GetEnergy().GetExerciseMeanArterialPressureDelta(PressureUnit::mmHg);

  //Adjust the MAP set-point for baroreceptors for anesthetics, opioids, and sedatives.  Other drugs should leave set-point as is.
  for (SESubstance* sub : m_data.GetCompartments().GetLiquidCompartmentSubstances()) {
    if (!sub->HasPD())
      continue;
    if ((sub->GetClassification() == CDM::enumSubstanceClass::Anesthetic) || (sub->GetClassification() == CDM::enumSubstanceClass::Sedative) || (sub->GetClassification() == CDM::enumSubstanceClass::Opioid)) {
      meanArterialPressureSetPoint_mmHg += m_data.GetDrugs().GetMeanBloodPressureChange(PressureUnit::mmHg);
      break;
      //Only want to apply the blood pressure change ONCE (In case there are multiple sedative/opioids/etc)
      ///\TODO:  Look into a better way to implement drug classification search
    }
  }

  //Neurological effects of pain action
  if (m_data.GetActions().GetPatientActions().HasPainStimulus()) {
    double painVAS = GetPainVisualAnalogueScale().GetValue();
    painVAS *= 0.1;
    meanArterialPressureSetPoint_mmHg *= (1 + 0.4 * painVAS);
  }

  double sympatheticFraction = 1.0 / (1.0 + pow(meanArterialPressure_mmHg / meanArterialPressureSetPoint_mmHg, nu));
  double parasympatheticFraction = 1.0 / (1.0 + pow(meanArterialPressure_mmHg / meanArterialPressureSetPoint_mmHg, -nu));

  //Calculate the normalized change in heart rate
  double normalizedHeartRate = GetBaroreceptorHeartRateScale().GetValue();
  double tauHeartRate_s = m_data.GetConfiguration().GetHeartRateDistributedTimeDelay(TimeUnit::s);
  double deltaNormalizedHeartRate = (1.0 / tauHeartRate_s) * (-normalizedHeartRate + m_normalizedAlphaHeartRate * sympatheticFraction - m_normalizedBetaHeartRate * parasympatheticFraction + m_normalizedGammaHeartRate) * m_dt_s;
  normalizedHeartRate += deltaNormalizedHeartRate;
  GetBaroreceptorHeartRateScale().SetValue(normalizedHeartRate);

  //Calculate the normalized change in heart elastance
  double normalizedHeartElastance = GetBaroreceptorHeartElastanceScale().GetValue();
  double tauElastance_s = m_data.GetConfiguration().GetHeartElastanceDistributedTimeDelay(TimeUnit::s);
  double deltaNormalizedHeartElastance = (1.0 / tauElastance_s) * (-normalizedHeartElastance + m_normalizedAlphaElastance * sympatheticFraction + m_normalizedGammaElastance) * m_dt_s;
  normalizedHeartElastance += deltaNormalizedHeartElastance;
  GetBaroreceptorHeartElastanceScale().SetValue(normalizedHeartElastance);

  //Calculate the normalized change in flow resistance for any cardiovascular resistor
  double normalizedResistance = GetBaroreceptorResistanceScale().GetValue();
  double tauResistance_s = m_data.GetConfiguration().GetSystemicResistanceDistributedTimeDelay(TimeUnit::s);
  double deltaNormalizedResistance = (1.0 / tauResistance_s) * (-normalizedResistance + m_normalizedAlphaResistance * sympatheticFraction + m_normalizedGammaResistance) * m_dt_s;
  normalizedResistance += deltaNormalizedResistance;
  GetBaroreceptorResistanceScale().SetValue(normalizedResistance);

  //Calculate the normalized change in flow compliance for any cardiovascular compliance
  double normalizedCompliance = GetBaroreceptorComplianceScale().GetValue();
  double tauCompliance_s = m_data.GetConfiguration().GetVenousComplianceDistributedTimeDelay(TimeUnit::s);
  double deltaNormalizedCompliance = (1.0 / tauCompliance_s) * (-normalizedCompliance + m_normalizedAlphaCompliance * parasympatheticFraction + m_normalizedGammaCompliance) * m_dt_s;
  normalizedCompliance += deltaNormalizedCompliance;
  GetBaroreceptorComplianceScale().SetValue(normalizedCompliance);
#ifdef VERBOSE
  m_data.GetDataTrack().Probe("normalizedHeartRate", normalizedHeartRate);
  m_data.GetDataTrack().Probe("normalizedHeartElastance", normalizedHeartElastance);
  m_data.GetDataTrack().Probe("normalizedResistance", normalizedResistance);
  m_data.GetDataTrack().Probe("normalizedCompliance", normalizedCompliance);
  m_data.GetDataTrack().Probe("meanArterialPressureSetPoint_mmHg", meanArterialPressureSetPoint_mmHg);
#endif
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
  if (!m_data.GetActions().GetPatientActions().HasPainStimulus()) {
    GetPainVisualAnalogueScale().SetValue(0.0);
    return;
  }

  //initialize:
  SEPainStimulus* p;
  const std::map<std::string, SEPainStimulus*>& pains = m_data.GetActions().GetPatientActions().GetPainStimuli();
  double patientSusceptability = m_Patient->GetPainSusceptibility().GetValue();
  double susceptabilityMapping = GeneralMath::LinearInterpolator(-1.0, 1.0, 0.0, 2.0, patientSusceptability); //mapping [-1,1] -> [0, 2] for scaling the pain stimulus
  double severity = 0.0;
  double painVASMapping = 0.0; //for each location map the [0,1] severity to the [0,10] VAS scale
  double tempPainVAS = 0.0; //sum, scale and store the patient score
  double CNSPainBuffer = 1.0;

  m_painVAS = GetPainVisualAnalogueScale().GetValue();

  //reset duration if VAS falls below approx zero
  if (m_painVAS == 0.0)
    m_painStimulusDuration_s = 0.0;

  //grab drug effects if there are in the body
  if (m_data.GetDrugs().HasCentralNervousResponse()) {
    double NervousScalar = 10.0;
    double CNSModifier = m_data.GetDrugs().GetCentralNervousResponse().GetValue();
    CNSPainBuffer = exp(-CNSModifier * NervousScalar);
  }

  //iterate over all locations to get a cumulative stimulus and buffer them
  for (auto pain : pains) {
    p = pain.second;
    severity = p->GetSeverity().GetValue();
    painVASMapping = 10.0 * severity;

    tempPainVAS += (painVASMapping * susceptabilityMapping * CNSPainBuffer) / (1 + exp(-20 * m_painStimulusDuration_s + 4.0)); //temp time will increase so long as a stimulus is present
  }

  //advance time over the duration of the stimulus

  if (severity < ZERO_APPROX)
    m_painVASDuration_s += m_dt_s;

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

  if (!m_FeedbackActive)
    return;

  double normalized_pO2 = m_data.GetBloodChemistry().GetArterialOxygenPressure(PressureUnit::mmHg) / m_ArterialOxygenSetPoint_mmHg;
  double normalized_pCO2 = m_data.GetBloodChemistry().GetArterialCarbonDioxidePressure(PressureUnit::mmHg) / m_ArterialCarbonDioxideSetPoint_mmHg;

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
  //double maxHeartRateDelta = 1.23 * m_HeartRateNoFeedbackBaseline_per_min;
  double maxHeartRateDelta = 1.23 * m_data.GetPatient().GetHeartRateBaseline(FrequencyUnit::Per_min);
  double modifier = GeneralMath::LogisticFunction(amax, a50, aeta, normalized_pCO2);
  modifier += GeneralMath::LogisticFunction(bmax, b50, beta, normalized_pCO2);
  modifier += GeneralMath::LogisticFunction(cmax, c50, ceta, normalized_pO2);
  modifier += GeneralMath::LogisticFunction(dmax, d50, deta, normalized_pO2);

  //Apply central nervous depressant effects (currently only applies to morphine)
  SEDrugSystem& Drugs = m_data.GetDrugs();
  double CNSChange = Drugs.GetCentralNervousResponse().GetValue();
  if (CNSChange >= 0.25)
    modifier = 0.0;

  //set to zero if below .1 percent
  if (modifier < 0.001)
    modifier = 0.0;

  GetChemoreceptorHeartRateScale().SetValue(maxHeartRateDelta * modifier);

  // Calculate the normalized change in heart elastance
  double normalizedHeartElastance = 1.0;
  /// \todo Compute and apply chemoreceptor-mediated contractility changes
  GetChemoreceptorHeartElastanceScale().SetValue(normalizedHeartElastance);
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
        leftPupilSizeResponseLevel += (1 / (1 + exp(-2.0 * (icp_mmHg - 24))));
        //https://www.wolframalpha.com/input/?i=y%3D-.001*pow(10,+.27*(x+-+15))+from+18%3Cx%3C28+and+-1%3Cy%3C0
        leftPupilReactivityResponseLevel += -.001 * pow(10, .27 * (icp_mmHg - 15));
        rightPupilSizeResponseLevel = leftPupilSizeResponseLevel;
        rightPupilReactivityResponseLevel = leftPupilReactivityResponseLevel;
      } else if (b->GetType() == CDM::enumBrainInjuryType::LeftFocal) {
        leftPupilSizeResponseLevel += (1 / (1 + exp(-2.0 * (icp_mmHg - 24))));
        leftPupilReactivityResponseLevel += -.001 * pow(10, .27 * (icp_mmHg - 15));
      } else if (b->GetType() == CDM::enumBrainInjuryType::RightFocal) {
        rightPupilSizeResponseLevel += (1 / (1 + exp(-2.0 * (icp_mmHg - 24))));
        rightPupilReactivityResponseLevel += -.001 * pow(10, .27 * (icp_mmHg - 15));
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
