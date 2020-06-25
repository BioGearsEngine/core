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

#include <biogears/engine/Systems/Hepatic.h>

#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType2.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
auto Hepatic::make_unique(BioGears& bg) -> std::unique_ptr<Hepatic>
{
  return std::unique_ptr<Hepatic>(new Hepatic(bg));
}

Hepatic::Hepatic(BioGears& bg)
  : SEHepaticSystem(bg.GetLogger())
  , m_data(bg)
{
  Clear();
}

Hepatic::~Hepatic()
{
  Clear();
}

void Hepatic::Clear()
{
  SEHepaticSystem::Clear();
  m_liverInsulin = nullptr;
  m_liverGlucagon = nullptr;
  m_liverVascularGlucose = nullptr;
  m_liverExtracellularGlucose = nullptr;
  m_liverExtracellularAA = nullptr;
  m_liverExtracellularTAG = nullptr;
  m_liverExtracellularUrea = nullptr;
  m_liverExtracellularLactate = nullptr;
  m_liverExtracellularO2 = nullptr;
  m_liverExtracellularKetones = nullptr;
  m_muscleInsulin = nullptr;
  m_muscleGlucagon = nullptr;
  m_muscleVascularGlucose = nullptr;
  m_LiverTissueAlbumin = nullptr;
  m_Glucagon = nullptr;
  m_Insulin = nullptr;
  m_Glucose = nullptr;
  m_AminoAcids = nullptr;
  m_Triacylglycerol = nullptr;
  m_Urea = nullptr;
  m_Lactate = nullptr;
  m_O2 = nullptr;
  m_Ketones = nullptr;
  m_Albumin = nullptr;
  m_tsu = nullptr;
  m_energy = nullptr;
  m_Patient = nullptr;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Hepatic::Initialize()
{
  BioGearsSystem::Initialize();

  /// \cite garber1974ketone
  GetKetoneProductionRate().SetValue(300.0, AmountPerTimeUnit::umol_Per_min);
  /// \cite boron2012medical
  GetHepaticGluconeogenesisRate().SetValue(180, MassPerTimeUnit::g_Per_day);
}

bool Hepatic::Load(const CDM::BioGearsHepaticSystemData& in)
{
  if (!SEHepaticSystem::Load(in))
    return false;

  BioGearsSystem::LoadState();

  return true;
}
CDM::BioGearsHepaticSystemData* Hepatic::Unload() const
{
  CDM::BioGearsHepaticSystemData* data = new CDM::BioGearsHepaticSystemData();
  Unload(*data);
  return data;
}
void Hepatic::Unload(CDM::BioGearsHepaticSystemData& data) const
{
  SEHepaticSystem::Unload(data);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for the Hepatic Class
///
///  \details
///   Initializes member variables and system level values on the common data model.
//--------------------------------------------------------------------------------------------------
void Hepatic::SetUp()
{
  m_dt_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  m_tsu = &m_data.GetTissue();
  m_energy = &m_data.GetEnergy();
  m_Patient = &m_data.GetPatient();

  m_Insulin = &m_data.GetSubstances().GetInsulin();
  m_Glucagon = &m_data.GetSubstances().GetGlucagon();
  m_Glucose = &m_data.GetSubstances().GetGlucose();
  m_AminoAcids = &m_data.GetSubstances().GetAminoAcids();
  m_Triacylglycerol = &m_data.GetSubstances().GetTriacylglycerol();
  m_Urea = &m_data.GetSubstances().GetUrea();
  m_Lactate = &m_data.GetSubstances().GetLactate();
  m_O2 = &m_data.GetSubstances().GetO2();
  m_CO2 = &m_data.GetSubstances().GetCO2();
  m_Ketones = &m_data.GetSubstances().GetKetones();
  m_Albumin = &m_data.GetSubstances().GetAlbumin();

  m_liverInsulin = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Liver)->GetSubstanceQuantity(*m_Insulin);
  m_liverGlucagon = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Liver)->GetSubstanceQuantity(*m_Glucagon);
  m_muscleInsulin = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle)->GetSubstanceQuantity(*m_Insulin);
  m_muscleGlucagon = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle)->GetSubstanceQuantity(*m_Glucagon);
  m_liverVascularGlucose = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Liver)->GetSubstanceQuantity(*m_Glucose);
  m_muscleVascularGlucose = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle)->GetSubstanceQuantity(*m_Glucose);
  m_liverExtracellularGlucose = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetSubstanceQuantity(*m_Glucose);
  m_liverExtracellularAA = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetSubstanceQuantity(*m_AminoAcids);
  m_liverExtracellularTAG = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetSubstanceQuantity(*m_Triacylglycerol);
  m_liverExtracellularUrea = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetSubstanceQuantity(*m_Urea);
  m_liverExtracellularLactate = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetSubstanceQuantity(*m_Lactate);
  m_liverExtracellularO2 = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetSubstanceQuantity(*m_O2);
  m_liverExtracellularCO2 = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetSubstanceQuantity(*m_CO2);
  m_liverExtracellularKetones = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetSubstanceQuantity(*m_Ketones);
  m_LiverTissueAlbumin = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetSubstanceQuantity(*m_Albumin);

  //Glycogen can make up 5-8% of liver's weight, and average liver is 1.5 kg, so max glycogen should be around 97.5 g (guyton)
  m_maxLiverGlycogen_g = .065 * m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver)->GetTotalMass(MassUnit::g);

  //Glycogen in muscles can make up 1-3% of their weight, but this glycogen can't diffuse out of the muscle (guyton);
  m_maxMuscleGlycogen_g = .02 * m_Patient->GetMuscleMass(MassUnit::g);

  m_AlbuminProdutionRate_g_Per_s = 1.5e-4; /// \cite jarnum1972plasma
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determines if the engine is stable.
///
/// \details
/// When the engine is stable, the CDM makes this call to update the member variable.
//--------------------------------------------------------------------------------------------------
void Hepatic::AtSteadyState()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Preprocess function
///
/// \details
//--------------------------------------------------------------------------------------------------
void Hepatic::PreProcess()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Process function
///
/// \details
//--------------------------------------------------------------------------------------------------
void Hepatic::Process()
{
  //Hormone change > 0 = storage mode
  Glycogenesis();
  Lipogenesis();

  //Hormone change < 0 = burning mode
  Glycogenolysis();
  Gluconeogenesis();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// PostProcess function
///
/// \details
//--------------------------------------------------------------------------------------------------
void Hepatic::PostProcess()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Flat Rate production of albumin in the liver
///
/// \param  duration_s
/// Time period for production of albumin in seconds.
///
/// \details
/// The mass of albumin to produce is calculated from a flat rate production value and the time passed.
/// This mass is added to the liver. The rate is currently specified as .15 mg/s as found in \cite Jarnum1972plasma
//--------------------------------------------------------------------------------------------------
void Hepatic::ProduceAlbumin(double duration_s)
{
  double massConverted_g = m_AlbuminProdutionRate_g_Per_s * duration_s;
  m_LiverTissueAlbumin->GetMass().IncrementValue(massConverted_g, MassUnit::g);
  m_LiverTissueAlbumin->Balance(BalanceLiquidBy::Mass);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Removes excess glucose from blood to store as glycogen
///
/// \details
/// Uses relative changes in insulin and glucagon to determine if new glycogen should be generated from blood
/// glucose. There is no glycogen substance, so store off glucose in a Hepatic member variable.
//--------------------------------------------------------------------------------------------------
void Hepatic::Glycogenesis()
{
  double hormoneFactor = CalculateRelativeHormoneChange(m_tsu->GetLiverInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L) * 1e9, m_tsu->GetLiverGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL) * 1e9, m_liverInsulin, m_liverGlucagon, m_data);

  //Useful debugging information
  /*
  double insulinDeviation = (m_liverInsulin->GetMolarity(AmountPerVolumeUnit::mmol_Per_L)*1e9 - m_tsu->GetLiverInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L)*1e9) / (m_tsu->GetLiverInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L)*1e9);
  double glucagonDeviation = (m_liverGlucagon->GetConcentration(MassPerVolumeUnit::mg_Per_mL)*1e9 - m_tsu->GetLiverGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL)*1e9) / (m_tsu->GetLiverGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL)*1e9);

  m_data.GetDataTrack().Probe("LiverHormoneFactor", hormoneFactor);
  m_data.GetDataTrack().Probe("LiverInsulinDeviation", insulinDeviation);
  m_data.GetDataTrack().Probe("LiverGlucagonDeviation", glucagonDeviation);
  m_data.GetDataTrack().Probe("LiverGlucagonSetPoint_pg_Per_mL", m_tsu->GetLiverGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL)*1e9);
  m_data.GetDataTrack().Probe("LiverInsulinSetPoint_pmol_Per_L", m_tsu->GetLiverInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L)*1e9);
  m_data.GetDataTrack().Probe("LiverGlucagon_pg_Per_mL", m_liverGlucagon->GetConcentration(MassPerVolumeUnit::mg_Per_mL)*1e9);
  m_data.GetDataTrack().Probe("LiverInsulin_pmol_Per_L", m_liverInsulin->GetMolarity(AmountPerVolumeUnit::mmol_Per_L)*1e9);
  */

  //Normally, 2% of glycogen is regenerated per hour after exercise, but can reach 5% \cite tardie2008glycogen
  //Roughly .00375 g/s after a meal after a 64 hour fast \cite rothman1991quantitative
  double glycogenesisLowerRate_g_Per_s = .02 * (m_maxLiverGlycogen_g + m_maxMuscleGlycogen_g) / 3600; //~.00365
  double glycogenesisUpperRate_g_Per_s = .05 * (m_maxLiverGlycogen_g + m_maxMuscleGlycogen_g) / 3600; //~.009

  //https://www.wolframalpha.com/input/?i=y%3D.0005417%2B.0008125%2F(1%2Be%5E(-6(x-1)))+from+0%3Cy%3C.0015+and+0%3Cx%3C2
  //Can tweak sigmoid midpoint based on ratio values observed; shape parameter should be tweaked to give appropriate refill rates for glycogen (>20 hours)
  double glycogenesisRate_g_Per_s = glycogenesisLowerRate_g_Per_s + GeneralMath::LogisticFunction(glycogenesisUpperRate_g_Per_s - glycogenesisLowerRate_g_Per_s, 1, 6, hormoneFactor);

  //remove excess glucose from blood and store in glycogen while there's room
  if (hormoneFactor > 0 && m_tsu->GetLiverGlycogen(MassUnit::g) < m_maxLiverGlycogen_g) {
    if (m_liverVascularGlucose->GetMass().GetValue(MassUnit::g) < glycogenesisRate_g_Per_s * m_dt_s) {
      m_ss << "Not enough glucose in blood to store as glycogen!";
      Info(m_ss);
      return;
    }

    m_liverVascularGlucose->GetMass().IncrementValue(-glycogenesisRate_g_Per_s * m_dt_s, MassUnit::g);
    m_tsu->GetLiverGlycogen().IncrementValue(glycogenesisRate_g_Per_s * m_dt_s, MassUnit::g);
    m_liverVascularGlucose->Balance(BalanceLiquidBy::Mass);
  }

  //Now check the hormone factor in the muscle for storage of muscle glycogen
  hormoneFactor = CalculateRelativeHormoneChange(m_tsu->GetMuscleInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L) * 1e9, m_tsu->GetMuscleGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL) * 1e9, m_muscleInsulin, m_muscleGlucagon, m_data);

  //Useful debugging information
  /*
  insulinDeviation = (m_muscleInsulin->GetMolarity(AmountPerVolumeUnit::mmol_Per_L)*1e9 - m_tsu->GetMuscleInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L)*1e9) / (m_tsu->GetMuscleInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L)*1e9);
  glucagonDeviation = (m_muscleGlucagon->GetConcentration(MassPerVolumeUnit::mg_Per_mL)*1e9 - m_tsu->GetMuscleGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL)*1e9) / (m_tsu->GetMuscleGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL)*1e9);

  m_data.GetDataTrack().Probe("MuscleHormoneFactor", hormoneFactor);
  m_data.GetDataTrack().Probe("MuscleInsulinDeviation", insulinDeviation);
  m_data.GetDataTrack().Probe("MuscleGlucagonDeviation", glucagonDeviation);
  */

  //remove excess glucose from blood and store in muscle glycogen while there's room
  if ((hormoneFactor > 0) && (m_tsu->GetMuscleGlycogen(MassUnit::g) < m_maxMuscleGlycogen_g)) {
    if (m_muscleVascularGlucose->GetMass(MassUnit::g) < glycogenesisRate_g_Per_s * m_dt_s) {
      //m_ss << "Not enough glucose in blood to store as muscle glycogen!";
      //Info(m_ss);
    } else {
      m_muscleVascularGlucose->GetMass().IncrementValue(-glycogenesisRate_g_Per_s * m_dt_s, MassUnit::g);
      m_tsu->GetMuscleGlycogen().IncrementValue(glycogenesisRate_g_Per_s * m_dt_s, MassUnit::g);
      m_muscleVascularGlucose->Balance(BalanceLiquidBy::Mass);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Removes glucose from storage as glycogen and puts into blood
///
/// \details
/// Uses relative changes in insulin and glucagon to determine if glycogen should be broken down
/// to glucose and moved to bloodstream.
//--------------------------------------------------------------------------------------------------
void Hepatic::Glycogenolysis()
{
  double hormoneFactor = CalculateRelativeHormoneChange(m_tsu->GetLiverInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L) * 1e9, m_tsu->GetLiverGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL) * 1e9, m_liverInsulin, m_liverGlucagon, m_data);

  //Glycogenolysis rate
  double glycogenolysisLowerRate_g_Per_s = .000926; //\cite rothman1991quantitation
  double glycogenolysisUpperRate_g_Per_s = .00329; //\cite rothman1991quantitation \cite petersen2004regulation

  //https://www.wolframalpha.com/input/?i=y%3D.000926%2B.002364%2F(1%2Be%5E(-6(x-1)))+from+0%3Cy%3C.004+and+0%3Cx%3C2
  double glycogenolysisRate_g_Per_s = glycogenolysisLowerRate_g_Per_s + GeneralMath::LogisticFunction(glycogenolysisUpperRate_g_Per_s - glycogenolysisLowerRate_g_Per_s, 1, 6, -hormoneFactor);

  //remove glucose from glycogen stores and dump into blood
  if (hormoneFactor < 0) {
    if (m_tsu->GetLiverGlycogen(MassUnit::g) < glycogenolysisRate_g_Per_s * m_dt_s) {
      //m_ss << "Not enough glycogen remaining!";
      //Info(m_ss);
      return;
    }

    m_liverVascularGlucose->GetMass().IncrementValue(glycogenolysisRate_g_Per_s * m_dt_s, MassUnit::g);
    m_tsu->GetLiverGlycogen().IncrementValue(-glycogenolysisRate_g_Per_s * m_dt_s, MassUnit::g);
    m_liverVascularGlucose->Balance(BalanceLiquidBy::Mass);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// The liver's process of generating new glucose from lactate, AA, and TAG.
///
/// \details
/// During times of low insulin vs. glucagon, the liver recombines lactate into glucose, deaminates and
/// combines AA to make glucose, and splits TAG to make the glycogen backbone into glucose. Fat breakdown
/// results in ketones. These are energy and O2 consuming processes.
//--------------------------------------------------------------------------------------------------
void Hepatic::Gluconeogenesis()
{
  //Outputs
  double ketoneProductionRate_mol_Per_s = 0;
  double totalO2Consumed_mol = 0;
  double totalGlucoseFromGluconeogenesis_mol = 0;
  double glucoseFromLactate_mol = 0;
  double glucoseFromGlycerol_mol = 0;
  double glucoseFromAA_mol = 0;

  //Determine hormone factor for activity control
  double hormoneFactor = CalculateRelativeHormoneChange(m_tsu->GetLiverInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L) * 1e9, m_tsu->GetLiverGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL) * 1e9, m_liverInsulin, m_liverGlucagon, m_data);

  //Handle lactate conversion
  //TODO should this only be done when hormone factor is negative? This would allow for some lactate excretion and maybe make it so that the blood maintains the expected 2-23 mg/dL
  //Guyton says up to 75% of lactate is converted back to glucose in the liver after exercise (the rest is converted to pyruvate for use in citric acid cycle)
  //Some other tissues can also do this, to a lesser extent, but we only do it in liver
  //Since lactate->pyruvate is one reaction facilitated by one enzyme, and 2 pyruvate->glucose is glycolysis in reverse,
  //we will assume this conversion is limited only by the amount of lactate in the liver and presence of O2
  //TODO Can't find how much O2 this costs, if any
  double rateLimitingTuningFactor = 1;
  double liverLactate_mol = m_liverExtracellularLactate->GetMass().GetValue(MassUnit::g) / m_Lactate->GetMolarMass(MassPerAmountUnit::g_Per_mol);
  double reconvertedGlucose_mol = liverLactate_mol * .5;
  if (rateLimitingTuningFactor == 1)
    m_liverExtracellularLactate->GetMass().SetValue(0, MassUnit::g); //using IncrementValue to remove ALL lactate can result in numerical error negative masses
  else
    m_liverExtracellularLactate->GetMass().IncrementValue(-rateLimitingTuningFactor * liverLactate_mol * m_Lactate->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
  m_liverExtracellularGlucose->GetMass().IncrementValue(rateLimitingTuningFactor * reconvertedGlucose_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
  totalGlucoseFromGluconeogenesis_mol += rateLimitingTuningFactor * reconvertedGlucose_mol;
  glucoseFromLactate_mol += rateLimitingTuningFactor * reconvertedGlucose_mol;

  //Handle fat conversion (glycerol to glucose and ketogenesis)
  //One TAG breaks down to one glycerol (which converts to glucose) and 3 palmitic acids
  //One palmitic acid breaks down to 8 acetyl-COAs, which all convert to ketones (2 acetyl-COAs combine to make acetoacetate, our "ketones")
  //This rate is really governed by the presence of oxaloacetate, but since we're not modeling that,
  //we have to empirically determine rates
  //A fasting person will have low levels of oxaloacetate, meaning it can't aid in converting acetyl-COA for Citric Acid Cycle
  //Thus, Acetyl-COA converts to ketones instead
  double TAGBreakdownLowerRate_g_Per_s = .0001;
  double TAGBreakdownUpperRate_g_Per_s = .001;

  //https://www.wolframalpha.com/input/?i=y%3D.0001%2B.0009%2F(1%2Be%5E(-8(x-.4)))+from+0%3Cy%3C.001and+0%3Cx%3C1
  double TAGBreakdownRate_g_Per_s = TAGBreakdownLowerRate_g_Per_s + GeneralMath::LogisticFunction(TAGBreakdownUpperRate_g_Per_s - TAGBreakdownLowerRate_g_Per_s, .4, 8, -hormoneFactor);

  double glucosePerTAG_mol = 1; //one glycerol backbone
  double ketonesPerTAG_mol = 12; //8*3 acetyl-COAs -> 12 acetoacetate (in reality, there's also acetone and B-hydroxybutyrate)
  double O2ConsumedToMakeKetones = 5;

  double TAGBrokenDown_mol = TAGBreakdownRate_g_Per_s * m_dt_s / m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);

  //If we have enough TAG in the liver
  if (m_liverExtracellularTAG->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_mL) * m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetVolume(VolumeUnit::mL) > TAGBrokenDown_mol) {
    //If we have enough O2 as well
    if (m_liverExtracellularO2->GetMass(MassUnit::g) > TAGBrokenDown_mol * O2ConsumedToMakeKetones * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
      m_liverExtracellularTAG->GetMass().IncrementValue(-TAGBrokenDown_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      m_liverExtracellularGlucose->GetMass().IncrementValue(TAGBrokenDown_mol * glucosePerTAG_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      m_liverExtracellularKetones->GetMass().IncrementValue(TAGBrokenDown_mol * ketonesPerTAG_mol * m_Ketones->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      m_liverExtracellularO2->GetMass().IncrementValue(-TAGBrokenDown_mol * O2ConsumedToMakeKetones * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      ketoneProductionRate_mol_Per_s += TAGBrokenDown_mol * ketonesPerTAG_mol / m_dt_s;
      totalO2Consumed_mol += TAGBrokenDown_mol * O2ConsumedToMakeKetones;
      totalGlucoseFromGluconeogenesis_mol += TAGBrokenDown_mol * glucosePerTAG_mol;
      glucoseFromGlycerol_mol += TAGBrokenDown_mol * glucosePerTAG_mol;
    }
    //Else we're running out of O2, it limits
    else {
      double TAGActuallyBrokenDown_mol = m_liverExtracellularO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2ConsumedToMakeKetones;
      m_liverExtracellularTAG->GetMass().IncrementValue(-TAGActuallyBrokenDown_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      m_liverExtracellularGlucose->GetMass().IncrementValue(TAGActuallyBrokenDown_mol * glucosePerTAG_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      m_liverExtracellularKetones->GetMass().IncrementValue(TAGActuallyBrokenDown_mol * ketonesPerTAG_mol * m_Ketones->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      m_liverExtracellularO2->GetMass().SetValue(0, MassUnit::g);
      ketoneProductionRate_mol_Per_s += TAGActuallyBrokenDown_mol * ketonesPerTAG_mol / m_dt_s;
      totalO2Consumed_mol += TAGActuallyBrokenDown_mol * O2ConsumedToMakeKetones;
      totalGlucoseFromGluconeogenesis_mol += TAGActuallyBrokenDown_mol * glucosePerTAG_mol;
      glucoseFromGlycerol_mol += TAGActuallyBrokenDown_mol * glucosePerTAG_mol;
    }
  }
  //If we don't have enough TAG in the liver, break down what's there
  else {
    double TAGActuallyBrokenDown_mol = m_liverExtracellularTAG->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_mL) * m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetVolume(VolumeUnit::mL);

    //If we have enough O2
    if (m_liverExtracellularO2->GetMass(MassUnit::g) > TAGActuallyBrokenDown_mol * O2ConsumedToMakeKetones * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
      m_liverExtracellularTAG->GetMass().SetValue(0, MassUnit::g);
      m_liverExtracellularGlucose->GetMass().IncrementValue(TAGActuallyBrokenDown_mol * glucosePerTAG_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      m_liverExtracellularKetones->GetMass().IncrementValue(TAGActuallyBrokenDown_mol * ketonesPerTAG_mol * m_Ketones->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      m_liverExtracellularO2->GetMass().IncrementValue(-TAGActuallyBrokenDown_mol * O2ConsumedToMakeKetones * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      ketoneProductionRate_mol_Per_s += TAGActuallyBrokenDown_mol * ketonesPerTAG_mol / m_dt_s;
      totalO2Consumed_mol += TAGActuallyBrokenDown_mol * O2ConsumedToMakeKetones;
      totalGlucoseFromGluconeogenesis_mol += TAGActuallyBrokenDown_mol * glucosePerTAG_mol;
      glucoseFromGlycerol_mol += TAGActuallyBrokenDown_mol * glucosePerTAG_mol;
    }
    //Otherwise, we don't have enough O2 for even this limited amount of TAG
    else {
      double TAGReallyActuallyBrokenDown_mol = m_liverExtracellularO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2ConsumedToMakeKetones;
      m_liverExtracellularTAG->GetMass().IncrementValue(-TAGReallyActuallyBrokenDown_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      m_liverExtracellularGlucose->GetMass().IncrementValue(TAGReallyActuallyBrokenDown_mol * glucosePerTAG_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      m_liverExtracellularKetones->GetMass().IncrementValue(TAGReallyActuallyBrokenDown_mol * ketonesPerTAG_mol * m_Ketones->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      m_liverExtracellularO2->GetMass().SetValue(0, MassUnit::g);
      ketoneProductionRate_mol_Per_s += TAGReallyActuallyBrokenDown_mol * ketonesPerTAG_mol / m_dt_s;
      totalO2Consumed_mol += TAGReallyActuallyBrokenDown_mol * O2ConsumedToMakeKetones;
      totalGlucoseFromGluconeogenesis_mol += TAGReallyActuallyBrokenDown_mol * glucosePerTAG_mol;
      glucoseFromGlycerol_mol += TAGReallyActuallyBrokenDown_mol * glucosePerTAG_mol;
    }
  }

  //Handle AA conversion
  //We assume all AA is alanine, which will undergo the same conversion to pyruvate as in consumption, releasing urea as a byproduct
  //1 alanine -> 1 pyruvate + .5 urea
  //2 pyruvate -> 1 glucose
  //TODO figure out O2 consumption, if any
  if (hormoneFactor < 0) {
    double AAConversionRate_g_Per_s = .0003; //May need to be tuned to maintain brain glucose levels
    double glucosePerAA_mol = .5;
    double ureaPerAA_mol = .5;

    double AAConverted_mol = AAConversionRate_g_Per_s * m_dt_s / m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol);

    //If we have enough AA in the liver
    if (m_liverExtracellularAA->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_mL) * m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetVolume(VolumeUnit::mL) > AAConverted_mol) {
      if (true) //we have enough O2 (if required)
      {
        m_liverExtracellularAA->GetMass().IncrementValue(-AAConverted_mol * m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        m_liverExtracellularGlucose->GetMass().IncrementValue(AAConverted_mol * glucosePerAA_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        m_liverExtracellularUrea->GetMass().IncrementValue(AAConverted_mol * ureaPerAA_mol * m_Urea->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        //O2 decrement here, don't forget to track totalO2Consumed
        totalGlucoseFromGluconeogenesis_mol += AAConverted_mol * glucosePerAA_mol;
        glucoseFromAA_mol += AAConverted_mol * glucosePerAA_mol;
      }
      //If O2 is limiting
      else {
      }
    }
    //If we don't have enough AA in the liver, break down what's there
    else {
      if (true) //we have enough O2 (if required)
      {
        double AAActuallyConverted_mol = m_liverExtracellularAA->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_mL) * m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->GetVolume(VolumeUnit::mL);
        m_liverExtracellularAA->GetMass().SetValue(0, MassUnit::g);
        m_liverExtracellularGlucose->GetMass().IncrementValue(AAActuallyConverted_mol * glucosePerAA_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        m_liverExtracellularUrea->GetMass().IncrementValue(AAActuallyConverted_mol * ureaPerAA_mol * m_Urea->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        //O2 decrement here, don't forget to track totalO2Consumed
        totalGlucoseFromGluconeogenesis_mol += AAActuallyConverted_mol * glucosePerAA_mol;
        glucoseFromAA_mol += AAActuallyConverted_mol * glucosePerAA_mol;
      }
      //O2 is limiting
      else {
      }
    }
  }
  m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->Balance(BalanceLiquidBy::Mass);
  if (ketoneProductionRate_mol_Per_s < .0001138) //don't record values greater than 1000 g/day to eliminate initial spikes in ketogenesis and gluconeogenesis
    GetKetoneProductionRate().SetValue(ketoneProductionRate_mol_Per_s, AmountPerTimeUnit::mol_Per_s);
  if (totalGlucoseFromGluconeogenesis_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol) * (1 / m_dt_s) * 3600 * 24 < 1000)
    GetHepaticGluconeogenesisRate().SetValue(totalGlucoseFromGluconeogenesis_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol) * (1 / m_dt_s) * 3600 * 24, MassPerTimeUnit::g_Per_day);
  Tissue::m_hepaticO2Consumed_mol += totalO2Consumed_mol;

  /*
  if (totalGlucoseFromGluconeogenesis_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol) * (1 / m_dt_s) * 3600 * 24 < 1000)
    m_data.GetDataTrack().Probe("GluconeogenesisRate_g_Per_day", totalGlucoseFromGluconeogenesis_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol) * (1 / m_dt_s) * 3600 * 24);
  if (ketoneProductionRate_mol_Per_s < .0001138)
    m_data.GetDataTrack().Probe("KetogenesisRate_g_Per_day", ketoneProductionRate_mol_Per_s * m_Ketones->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 3600 * 24);
  m_data.GetDataTrack().Probe("GluconeogenesisLactateFraction", glucoseFromLactate_mol/totalGlucoseFromGluconeogenesis_mol);
  m_data.GetDataTrack().Probe("GluconeogenesisGlycerolFraction", glucoseFromGlycerol_mol / totalGlucoseFromGluconeogenesis_mol);
  m_data.GetDataTrack().Probe("GluconeogenesisAAFraction", glucoseFromAA_mol / totalGlucoseFromGluconeogenesis_mol);
  */
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the relative change in insulin to glucagon to determine direction of nutrient flow
///
/// \details
/// Calculate a percent deviation from given insulin and glucagon set points, then return the relative
/// deviation in insulin. So, if a positive number is returned, insulin increased more than glucagon
/// did, and vice versa. A zero value means insulin and glucagon didn't change or stayed the same
/// relative to each other.
//--------------------------------------------------------------------------------------------------
double Hepatic::CalculateRelativeHormoneChange(double insulinSetPoint_pmol_Per_L, double glucagonSetPoint_pg_Per_mL, SELiquidSubstanceQuantity* currentInsulin, SELiquidSubstanceQuantity* currentGlucagon, BioGears& m_data)
{
  double currentInsulin_pmol_Per_L = currentInsulin->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) * 1e9;
  double currentGlucagon_pg_Per_mL = currentGlucagon->GetConcentration(MassPerVolumeUnit::mg_Per_mL) * 1e9;

  //Patients with diabetes type 2 can have insulin resistance, which in essence means they require more insulin to achieve a normal effect
  if (m_data.GetConditions().HasDiabetesType2()) {
    if (m_data.GetConditions().GetDiabetesType2()->HasInsulinResistanceSeverity()) {
      double insulinScale = 1 - (.986189 - .9820614 * exp(-6.808106 * m_data.GetConditions().GetDiabetesType2()->GetInsulinResistanceSeverity().GetValue()));
      currentInsulin_pmol_Per_L *= insulinScale;
    }
  }

  // Insulin can deviate a lot from set point, even as high as 20 times, and still be in acceptable ranges
  // Glucagon doesn't vary as much, expect a max of about 7 times
  // Because of our production sigmoids plateauing, we don't see such large deviations
  double insulinDeviation = (currentInsulin_pmol_Per_L - insulinSetPoint_pmol_Per_L) / insulinSetPoint_pmol_Per_L;
  double glucagonDeviation = (currentGlucagon_pg_Per_mL - glucagonSetPoint_pg_Per_mL) / glucagonSetPoint_pg_Per_mL;

  return insulinDeviation - glucagonDeviation;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Generation of new TAG from AA and glucose in times of excess
///
/// \details
/// Triacylglycerol is generated from excess glucose or amino acids. The amounts of each substance that
/// are converted to TAG depend on the relative concentrations of those substances in the liver. Lipogenesis
/// is an ATP-consuming process, but we assume that energy consumption is rolled into BMR.
//--------------------------------------------------------------------------------------------------
void Hepatic::Lipogenesis()
{
  double hormoneFactor = CalculateRelativeHormoneChange(m_tsu->GetLiverInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L) * 1e9, m_tsu->GetLiverGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL) * 1e9, m_liverInsulin, m_liverGlucagon, m_data);

  double TAGGenerated_g = 0;
  double CO2Generated_mol = 0;

  //Get ratio of glucose to AA in liver to determine proportions that get converted to fat (will be a percentage of nutrients as glucose)
  //Really, what gets broken down to fat depends on concentrations of the facilitating enzymes
  //But it seems reasonable that you'll convert more of the one that's in excess
  double liverGlucoseToAARatio = m_liverExtracellularGlucose->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_L) / (m_liverExtracellularGlucose->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_L) + m_liverExtracellularAA->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_L));

  //For negative hormone factors, we shouldn't make fat at all
  //Boron p 1222 shows that this process generates 7 CO2 per palmitate produced
  //TODO a pure protein meal could conceivably not raise the hormone factor, and so AA wouldn't get converted
  double lipogenesisLowerRate_g_Per_s = .0000232; //Low rate from \cite mcdevitt2001denovo
  double lipogenesisUpperRate_g_Per_s = .00143; //Calculated from \cite chascione1987effect

  //https://www.wolframalpha.com/input/?i=y%3D.0000232+%2B+.0014068%2F(1%2Be%5E(-12(x-.4)))+from+-.0002%3Cy%3C.002+and+-.5%3Cx%3C2
  double lipogenesisRate_g_Per_s = lipogenesisLowerRate_g_Per_s + GeneralMath::LogisticFunction(lipogenesisUpperRate_g_Per_s - lipogenesisLowerRate_g_Per_s, .4, 12, hormoneFactor);

  //Don't make new TAG if hormone factor is negative
  if (hormoneFactor <= 0)
    lipogenesisRate_g_Per_s = 0;

  //https://www.diapedia.org/metabolism-insulin-and-other-hormones/5105592814/fatty-acid-oxidation-and-synthesis
  //Lipogenesis of one palmitic acid requires 8 acetyl-COAs (also 7 ATPs, but we won't consider that for now)
  //Breakdown of AA (we assume alanine) is same as in protein usage for energy, gives NH4+ and pyruvate
  //From pyruvate we get one acetyl-COA
  //Then 2 NH4+ to urea
  //Glucose breaks down to 2 pyruvates by glycolysis, which then turn to acetyl-COA (4 glucose = 8 acetyl-COA); then you need one more for the glycerol backbone
  //Then you need 3 palmitic acids per triacylglycerol
  double totalTriacylglycerolToBeFormed_mol = lipogenesisRate_g_Per_s * m_dt_s / m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);
  double glucoseToBeConverted_mol = liverGlucoseToAARatio * totalTriacylglycerolToBeFormed_mol * 13;
  double AAToBeConverted_mol = (1 - liverGlucoseToAARatio) * totalTriacylglycerolToBeFormed_mol * 24;
  double UreaFormed_mol = AAToBeConverted_mol * .5;
  double leftoverTAGDeficit_mol = 0;

  //If there's enough AA, convert it to triacylglycerol
  if (m_liverExtracellularAA->GetMass().GetValue(MassUnit::g) > AAToBeConverted_mol * m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
    m_liverExtracellularAA->GetMass().IncrementValue(-AAToBeConverted_mol * m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularUrea->GetMass().IncrementValue(UreaFormed_mol * m_Urea->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularTAG->GetMass().IncrementValue((AAToBeConverted_mol / 24) * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularCO2->GetMass().IncrementValue((AAToBeConverted_mol / 24) * 21 * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->Balance(BalanceLiquidBy::Mass);
    TAGGenerated_g += (AAToBeConverted_mol / 24) * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    CO2Generated_mol += (AAToBeConverted_mol / 24) * 21;
  }
  //If there's not enough to be converted, convert what's there, shift the deficit to glucose conversion
  else {
    double AAActuallyConsumed_mol = m_liverExtracellularAA->GetMass().GetValue(MassUnit::g) / m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    leftoverTAGDeficit_mol = (AAToBeConverted_mol - AAActuallyConsumed_mol) / 24;
    m_liverExtracellularAA->GetMass().SetValue(0, MassUnit::g);
    m_liverExtracellularUrea->GetMass().IncrementValue(AAActuallyConsumed_mol * .5 * m_Urea->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularTAG->GetMass().IncrementValue((AAActuallyConsumed_mol / 24) * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularCO2->GetMass().IncrementValue((AAActuallyConsumed_mol / 24) * 21 * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->Balance(BalanceLiquidBy::Mass);
    TAGGenerated_g += (AAActuallyConsumed_mol / 24) * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    CO2Generated_mol += (AAActuallyConsumed_mol / 24) * 21;
  }

  glucoseToBeConverted_mol += leftoverTAGDeficit_mol * 13;

  //If there's enough glucose, convert it to triacylglycerol
  if (m_liverExtracellularGlucose->GetMass().GetValue(MassUnit::g) > glucoseToBeConverted_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
    m_liverExtracellularGlucose->GetMass().IncrementValue(-glucoseToBeConverted_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularTAG->GetMass().IncrementValue((glucoseToBeConverted_mol / 13) * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularCO2->GetMass().IncrementValue((glucoseToBeConverted_mol / 13) * 21 * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->Balance(BalanceLiquidBy::Mass);
    TAGGenerated_g += (glucoseToBeConverted_mol / 13) * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    CO2Generated_mol += (glucoseToBeConverted_mol / 13) * 21;
  }
  //If there's not enough to be converted, convert what's there
  else {
    double glucoseActuallyConsumed_mol = m_liverExtracellularGlucose->GetMass().GetValue(MassUnit::g) / m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    leftoverTAGDeficit_mol = (glucoseToBeConverted_mol - glucoseActuallyConsumed_mol) / 13;
    m_liverExtracellularGlucose->GetMass().SetValue(0, MassUnit::g);
    m_liverExtracellularTAG->GetMass().IncrementValue((glucoseActuallyConsumed_mol / 13) * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularCO2->GetMass().IncrementValue((glucoseActuallyConsumed_mol / 13) * 21 * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->Balance(BalanceLiquidBy::Mass);
    TAGGenerated_g += (glucoseActuallyConsumed_mol / 13) * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    CO2Generated_mol += (glucoseToBeConverted_mol / 13) * 21;
  }

  //Try one last time to convert from AA, in case glucose had a deficit
  if (m_liverExtracellularAA->GetMass().GetValue(MassUnit::g) > leftoverTAGDeficit_mol * 24 * m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
    m_liverExtracellularAA->GetMass().IncrementValue(-leftoverTAGDeficit_mol * 24 * m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularUrea->GetMass().IncrementValue(leftoverTAGDeficit_mol * 24 * .5 * m_Urea->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularTAG->GetMass().IncrementValue(leftoverTAGDeficit_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularCO2->GetMass().IncrementValue(leftoverTAGDeficit_mol * 21 * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->Balance(BalanceLiquidBy::Mass);
    TAGGenerated_g += leftoverTAGDeficit_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    CO2Generated_mol += leftoverTAGDeficit_mol * 21;
  }
  //If there's not enough to be converted, convert what's there, remaining deficit just means lipogenesis is limited by lack of nutrients in liver
  else {
    double AAActuallyConsumed_mol = m_liverExtracellularAA->GetMass().GetValue(MassUnit::g) / m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    m_liverExtracellularAA->GetMass().SetValue(0, MassUnit::g);
    m_liverExtracellularUrea->GetMass().IncrementValue(AAActuallyConsumed_mol * .5 * m_Urea->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularTAG->GetMass().IncrementValue((AAActuallyConsumed_mol / 24) * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_liverExtracellularCO2->GetMass().IncrementValue((AAActuallyConsumed_mol / 24) * 21 * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
    m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)->Balance(BalanceLiquidBy::Mass);
    TAGGenerated_g += (AAActuallyConsumed_mol / 24) * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    CO2Generated_mol += (AAActuallyConsumed_mol / 24) * 21;
  }
  Tissue::m_hepaticCO2Produced_mol += CO2Generated_mol;
  //TODO Add lipogenesis rate to CDM
  //m_data.GetDataTrack().Probe("InstantaneousTAGGenerated(mg)", TAGGenerated_g * 1000);
}

SEScalar& Hepatic::CalculateLiverSOFA()
{
  SEScalar* sofa = new SEScalar();
  double sofaScore = 0.0;
  const double bilirubinCount = m_data.GetBloodChemistry().GetTotalBilirubin(MassPerVolumeUnit::mg_Per_dL);
  if (bilirubinCount <= 1.2) {
    //Normal, leave sofaScore = 0
  } else if (bilirubinCount <= 2.0) {
    sofaScore = 1.0;
  } else if (bilirubinCount <= 6.0) {
    sofaScore = 2.0;
  } else if (bilirubinCount <= 12.0) {
    sofaScore = 3.0;
  } else {
    sofaScore = 4.0;
  }
  sofa->SetValue(sofaScore);
  return *sofa;
}

}