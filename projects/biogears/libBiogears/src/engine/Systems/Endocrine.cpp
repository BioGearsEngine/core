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
#include <biogears/engine/Systems/Endocrine.h>

#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType2.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
auto Endocrine::make_unique(BioGears& bg) -> std::unique_ptr<Endocrine>
{
  return std::unique_ptr<Endocrine>(new Endocrine(bg));
}

Endocrine::Endocrine(BioGears& bg)
  : SEEndocrineSystem(bg.GetLogger())
  , m_data(bg)
{
  Clear();
}

Endocrine::~Endocrine()
{
  Clear();
}

void Endocrine::Clear()
{
  SEEndocrineSystem::Clear();
  m_AortaGlucose = nullptr;
  m_AortaEpinephrine = nullptr;
  m_SplanchnicInsulin = nullptr;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Endocrine::Initialize()
{
  BioGearsSystem::Initialize();
}

bool Endocrine::Load(const CDM::BioGearsEndocrineSystemData& in)
{
  if (!SEEndocrineSystem::Load(in))
    return false;
  BioGearsSystem::LoadState();
  return true;
}
CDM::BioGearsEndocrineSystemData* Endocrine::Unload() const
{
  CDM::BioGearsEndocrineSystemData* data = new CDM::BioGearsEndocrineSystemData();
  Unload(*data);
  return data;
}
void Endocrine::Unload(CDM::BioGearsEndocrineSystemData& data) const
{
  SEEndocrineSystem::Unload(data);
}

void Endocrine::SetUp()
{
  m_dt_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  SELiquidCompartment* aorta = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Aorta);
  SELiquidCompartment* rkidney = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightEfferentArteriole);
  SELiquidCompartment* lkidney = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftEfferentArteriole);
  m_AortaEpinephrine = aorta->GetSubstanceQuantity(m_data.GetSubstances().GetEpi());
  m_rKidneyEpinephrine = rkidney->GetSubstanceQuantity(m_data.GetSubstances().GetEpi());
  m_lKidneyEpinephrine = lkidney->GetSubstanceQuantity(m_data.GetSubstances().GetEpi());
  m_AortaGlucose = aorta->GetSubstanceQuantity(m_data.GetSubstances().GetGlucose());
  SESubstance* insulin = &m_data.GetSubstances().GetInsulin();
  m_InsulinMolarMass_g_Per_mol = insulin->GetMolarMass(MassPerAmountUnit::g_Per_mol);
  SESubstance* glucagon = &m_data.GetSubstances().GetGlucagon();
  m_GlucagonMolarMass_g_Per_mol = glucagon->GetMolarMass(MassPerAmountUnit::g_Per_mol);
  m_SplanchnicInsulin = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Splanchnic)->GetSubstanceQuantity(*insulin);
  m_SplanchnicGlucagon = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Splanchnic)->GetSubstanceQuantity(*glucagon);
}

void Endocrine::AtSteadyState()
{
  if (m_data.GetState() == EngineState::AtInitialStableState) {
    double diabetesScale = 1;
    if (m_data.GetConditions().HasDiabetesType1()) {
      if (m_data.GetConditions().GetDiabetesType1()->HasInsulinProductionSeverity())
        diabetesScale = 1 - m_data.GetConditions().GetDiabetesType1()->GetInsulinProductionSeverity().GetValue();

      const std::vector<SELiquidCompartment*>& vascular = m_data.GetCompartments().GetVascularLeafCompartments();
      SESubstance& insulin = m_data.GetSubstances().GetInsulin();

      SELiquidSubstanceQuantity* subQ;

      for (SELiquidCompartment* cmpt : vascular) {
        subQ = cmpt->GetSubstanceQuantity(insulin);
        subQ->GetConcentration().SetValue(diabetesScale * .6859, MassPerVolumeUnit::ug_Per_L);
        subQ->Balance(BalanceLiquidBy::Concentration);
      }
    }
  }

  m_AverageBiologicalDebt = m_data.GetNervous().GetBiologicalDebt().GetValue();  //save off the average after stabilization
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Endocrine Preprocess function
///
/// \details
/// Currently, only three hormones exist in the BioGears system: epinephrine, insulin, and glucagon.
/// These functions determine the release of these hormones.
/// The hormones will then circulate using the transport and substances methodologies.
//--------------------------------------------------------------------------------------------------
void Endocrine::PreProcess()
{
  ReleaseEpinephrine();
  SynthesizeInsulin();
  SynthesizeGlucagon();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Endocrine process function
///
/// \details
/// The Endocrine system does not currently have any Process functionality.
//--------------------------------------------------------------------------------------------------
void Endocrine::Process()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Endocrine PostProcess function
///
/// \details
/// The Endocrine system does not currently have any PostProcess functionality.
//--------------------------------------------------------------------------------------------------
void Endocrine::PostProcess()
{
  if (m_data.GetActions().GetPatientActions().HasOverride()
      && m_data.GetState() == EngineState::Active) {
    if (m_data.GetActions().GetPatientActions().GetOverride()->HasEndocrineOverride()) {
      ProcessOverride();
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculate the rate of insulin production
///
/// \details
/// The insulin production rate is calculated based on the relevant range of glucose and instantaneous concentration of glucose in the aorta
/// (representative of the body). The equation for insulin production is from \cite tolic2000insulin
/// Patients with diabetes mellitus may have a reduced capacity for insulin production or insulin resistance, based on severity.
//--------------------------------------------------------------------------------------------------
void Endocrine::SynthesizeInsulin()
{
  double bloodGlucoseConcentration_g_Per_L = m_AortaGlucose->GetConcentration(MassPerVolumeUnit::g_Per_L);
  const double biologicalDebt = m_data.GetNervous().GetBiologicalDebt().GetValue();
  double diabetesScale = 1;
  double metabolicScaling = 0.0;
  double insulinSynthesisRate_pmol_Per_min = 0.0;
  double insulinMolConversion = 6.67;


  //In type 1 diabetes, the ability to produce insulin is lessened
  if (m_data.GetConditions().HasDiabetesType1()) {
    if (m_data.GetConditions().GetDiabetesType1()->HasInsulinProductionSeverity())
      diabetesScale = 1 - m_data.GetConditions().GetDiabetesType1()->GetInsulinProductionSeverity().GetValue();
  }

  //In type 2 diabetes, the beta cells can have impaired production like in type 1,
  //but also there is the potential for increased production due to elevated blood glucose.
  //Since our normal production curve tops out at a lower glucose concentration, we use a multiplier
  //based on the plot in Guyton p 991 to up our production maximum
  else if (m_data.GetConditions().HasDiabetesType2()) {
    if (m_data.GetConditions().GetDiabetesType2()->HasInsulinProductionSeverity())
      diabetesScale = 1 - m_data.GetConditions().GetDiabetesType2()->GetInsulinProductionSeverity().GetValue();

    //Non-symmetric sigmoid roughly fit to Guyton curve
    //https://www.wolframalpha.com/input/?i=y+%3D+20.61421+%2B+(0.5089411+-+20.61421)%2F(1+%2B+(x%2F1.349053)%5E6.362276)%5E0.32948+from+0%3Cy%3C25+and+0%3Cx%3C7
    double multiplier = 20.61421 + (.5089411 - 20.61421) / std::pow((1 + std::pow((bloodGlucoseConcentration_g_Per_L / 1.349053), 6.362276)), .32948);

    diabetesScale *= multiplier;
  }

  //want to scale insulin synthesis down ~30 % during sleep deprived states
  metabolicScaling = -0.35 / (1 + exp(-82.0 * biologicalDebt + 18.0)) + 1.0;
  metabolicScaling = std::max(metabolicScaling, 0.0);

  // Note: Guyton says insulin production at 90 mg/dL glucose concentration should be
  // 25 ng/min/kg, which is about 300 pmol/min, double what we have using this curve from Tolic.
  // Because of this, we won't capture insulin behavior for very high glucose concentrations in non-diabetics, see Guyton p 991
  // Also, since we only key off of the instantaneous aorta glucose, we miss out on any parasympathetic
  // signals that affect Beta cells, meaning if we implement stress response, we might also see hyperinsulinemia (see Boron p 1222)
  if (bloodGlucoseConcentration_g_Per_L < 0.9) {
    insulinSynthesisRate_pmol_Per_min = diabetesScale * metabolicScaling * insulinMolConversion * (23.421 / (1.0 + exp((2.0 - 3.0 * (bloodGlucoseConcentration_g_Per_L + 0.4)) / 0.3)));
  }
  else {
    insulinSynthesisRate_pmol_Per_min = diabetesScale * metabolicScaling * insulinMolConversion * (23.4 + (37.0 / (1.0 + exp((2.0 - 5.0 * (bloodGlucoseConcentration_g_Per_L - 1.2)) / 0.3))));
  }

  GetInsulinSynthesisRate().SetValue(insulinSynthesisRate_pmol_Per_min, AmountPerTimeUnit::pmol_Per_min);

  double insulinMassDelta_g = Convert(insulinSynthesisRate_pmol_Per_min, AmountPerTimeUnit::pmol_Per_min, AmountPerTimeUnit::mol_Per_s);
  insulinMassDelta_g *= m_InsulinMolarMass_g_Per_mol * m_dt_s;

  m_SplanchnicInsulin->GetMass().IncrementValue(insulinMassDelta_g, MassUnit::g);
  m_SplanchnicInsulin->Balance(BalanceLiquidBy::Mass);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculate the rate of glucagon production
///
/// \details
/// The glucagon production rate is set based on the aorta glucose concentration to values that
/// allow it to have stable concentrations when paired with the clearance rate of 9 mL/min kg from
/// https://www.ncbi.nlm.nih.gov/pubmed/773949
//--------------------------------------------------------------------------------------------------
void Endocrine::SynthesizeGlucagon()
{
  //https://www.wolframalpha.com/input/?i=y%3D21.3-(21.3%2F(1%2Bexp((2-2x)%2F.3)))+from+.8%3Cx%3C1.2+and+0%3Cy%3C16

  double bloodGlucoseConcentration_g_Per_L = m_AortaGlucose->GetConcentration(MassPerVolumeUnit::g_Per_L);
  double glucagonSynthesisRate_pmol_Per_min = 0.0;

  //check to see which curve we are on: 
  if ( bloodGlucoseConcentration_g_Per_L < 0.75) {
    glucagonSynthesisRate_pmol_Per_min = 14.07 + (1.97 / (0.14 + exp(-1.0 + 9.4 * (bloodGlucoseConcentration_g_Per_L - 2.0))));
  }
  else {
    glucagonSynthesisRate_pmol_Per_min = 1.97 / (0.14 + exp(-1.0 + 9.7 * (bloodGlucoseConcentration_g_Per_L - 2.0)));
  }

  //Diabetic patients see glucagon abnormalities chronically, but even in short time-scales we expect to see glucagon levels
  //of ~40 ng/L, so we need to make sure glucagon is being produced even if blood sugar gets very high \cite brown2008too
  if (m_data.GetConditions().HasDiabetesType1()) {
    double minimumGlucagonRate_pmol_Per_min = 0;
    if (m_data.GetConditions().GetDiabetesType1()->HasInsulinProductionSeverity())
      minimumGlucagonRate_pmol_Per_min = GeneralMath::LinearInterpolator(0, 1, 0, .57 * 14.07, m_data.GetConditions().GetDiabetesType1()->GetInsulinProductionSeverity().GetValue());
    glucagonSynthesisRate_pmol_Per_min = std::max(glucagonSynthesisRate_pmol_Per_min, minimumGlucagonRate_pmol_Per_min);
  } else if (m_data.GetConditions().HasDiabetesType2()) {
    double minimumGlucagonRate_pmol_Per_min = 0;
    double totalEffect = 0;
    if (m_data.GetConditions().GetDiabetesType2()->HasInsulinProductionSeverity())
      totalEffect += m_data.GetConditions().GetDiabetesType2()->GetInsulinProductionSeverity().GetValue();
    if (m_data.GetConditions().GetDiabetesType2()->HasInsulinResistanceSeverity())
      totalEffect += m_data.GetConditions().GetDiabetesType2()->GetInsulinResistanceSeverity().GetValue();

    BLIM(totalEffect, 0, 1);

    minimumGlucagonRate_pmol_Per_min = GeneralMath::LinearInterpolator(0, 1, 0, .57 * 14.07, totalEffect);
    glucagonSynthesisRate_pmol_Per_min = std::max(glucagonSynthesisRate_pmol_Per_min, minimumGlucagonRate_pmol_Per_min);
  }

  GetGlucagonSynthesisRate().SetValue(glucagonSynthesisRate_pmol_Per_min, AmountPerTimeUnit::pmol_Per_min);

  double glucagonMassDelta_g = Convert(glucagonSynthesisRate_pmol_Per_min, AmountPerTimeUnit::pmol_Per_min, AmountPerTimeUnit::mol_Per_s);
  glucagonMassDelta_g *= m_GlucagonMolarMass_g_Per_mol * m_dt_s;

  m_SplanchnicGlucagon->GetMass().IncrementValue(glucagonMassDelta_g, MassUnit::g);
  m_SplanchnicGlucagon->Balance(BalanceLiquidBy::Mass);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Release epinephrine into the bloodstream and handle sympathetic responses
///
/// \details
/// Epinephrine is released at a basal rate of .18 ug/min \cite best1982release from the kidneys. During
/// certain events, the release rate of epinephrine increases. This is sympathetic response.
//--------------------------------------------------------------------------------------------------
void Endocrine::ReleaseEpinephrine()
{
  SEPatient& Patient = m_data.GetPatient();
  double patientWeight_kg = Patient.GetWeight(MassUnit::kg);
  double epinephrineBasalReleaseRate_ug_Per_min = .00229393 * patientWeight_kg; //We want it to be ~.18 ug/min for our StandardMale
  double epinephrineRelease_ug = (epinephrineBasalReleaseRate_ug_Per_min / 60) * m_dt_s; //amount released per timestep

  double currentMetabolicRate_W = m_data.GetEnergy().GetTotalMetabolicRate(PowerUnit::W);
  double basalMetabolicRate_W = Patient.GetBasalMetabolicRate(PowerUnit::W);
  double releaseMultiplier = 1.0;

  // If we have exercise, release more epi. Release multiplier is a sigmoid based on the total metabolic rate
  // with the maximum multiplier adapted from concentration data presented in @cite tidgren1991renal and @cite stratton1985hemodynamic
  // and the shape adjusted to match data in @cite tidgren1991renal.
  if (currentMetabolicRate_W > basalMetabolicRate_W) {
    double exercise_W = (currentMetabolicRate_W - basalMetabolicRate_W);
    double e50_W = 190;
    double eta = 0.035;
    double maxMultiplier = 18.75;
    releaseMultiplier = 1.0 + GeneralMath::LogisticFunction(maxMultiplier, e50_W, eta, exercise_W);
  }

  // If we have a stress/anxiety response, release more epi
  if (m_data.GetActions().GetPatientActions().HasAcuteStress()) {
    SEAcuteStress* s = m_data.GetActions().GetPatientActions().GetAcuteStress();
    double severity = s->GetSeverity().GetValue();

    //The highest stress multiplier we currently support is 30
    releaseMultiplier += GeneralMath::LinearInterpolator(0, 1, 0, 35.0, severity);
  }

  // If we have a pain response, release more epi
  if (m_data.GetActions().GetPatientActions().HasPainStimulus()) {
    double patientPainVAS = m_data.GetNervous().GetPainVisualAnalogueScale().GetValue();

    //The highest stress multiplier we currently support is 30
    releaseMultiplier += GeneralMath::LinearInterpolator(0, 1, 0, 5, patientPainVAS);
  }

  epinephrineRelease_ug *= releaseMultiplier;

  m_rKidneyEpinephrine->GetMass().IncrementValue(0.5 * epinephrineRelease_ug, MassUnit::ug);
  m_lKidneyEpinephrine->GetMass().IncrementValue(0.5 * epinephrineRelease_ug, MassUnit::ug);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// determine override requirements from user defined inputs
///
/// \details
/// User specified override outputs that are specific to the cardiovascular system are implemented here.
/// If overrides aren't present for this system then this function will not be called during preprocess.
//--------------------------------------------------------------------------------------------------
void Endocrine::ProcessOverride()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();

#ifdef BIOGEARS_USE_OVERRIDE_CONTROL
  OverrideControlLoop();
#endif
 
  if (override->HasInsulinSynthesisRateOverride()) {
    GetInsulinSynthesisRate().SetValue(override->GetInsulinSynthesisRateOverride(AmountPerTimeUnit::pmol_Per_min), AmountPerTimeUnit::pmol_Per_min);
  }
  if (override->HasGlucagonSynthesisRateOverride()) {
    GetGlucagonSynthesisRate().SetValue(override->GetGlucagonSynthesisRateOverride(AmountPerTimeUnit::pmol_Per_min), AmountPerTimeUnit::pmol_Per_min);
  }
}

//// Can be turned on or off (for debugging purposes) using the Biogears_USE_OVERRIDE_CONTROL external in CMake
void Endocrine::OverrideControlLoop()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();

  constexpr double maxInsulinSynthesisOverride = 10.0; //pmol_Per_min
  constexpr double minInsulinSynthesisOverride = 0.0; //pmol_Per_min
  constexpr double maxGlucagonSynthesisOverride = 10.0; //pmol_Per_min
  constexpr double minGlucagonSynthesisOverride = 0.0; //pmol_Per_min

  double currentInsulinSynthesisOverride = 0.0; //value gets changed in next check
  double currentGlucagonSynthesisOverride = 0.0; //value gets changed in next check

  if (override->HasInsulinSynthesisRateOverride()) {
    currentInsulinSynthesisOverride = override->GetInsulinSynthesisRateOverride(AmountPerTimeUnit::pmol_Per_min);
  }
  if (override->HasGlucagonSynthesisRateOverride()) {
    currentGlucagonSynthesisOverride = override->GetGlucagonSynthesisRateOverride(AmountPerTimeUnit::pmol_Per_min);
  }

  if ((currentInsulinSynthesisOverride < minInsulinSynthesisOverride || currentInsulinSynthesisOverride > maxInsulinSynthesisOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Insulin Synthesis Rate Override (Endocrine) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentGlucagonSynthesisOverride < minGlucagonSynthesisOverride || currentGlucagonSynthesisOverride > maxGlucagonSynthesisOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Glucagon Synthesis Rate Override (Endocrine) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  return;
}
}