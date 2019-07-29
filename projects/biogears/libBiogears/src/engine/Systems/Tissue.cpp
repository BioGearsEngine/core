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
#include <biogears/engine/Systems/Tissue.h>

//Standad Includes
#include <cmath>
//Project Includes
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/compartment/tissue/SETissueCompartment.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/conditions/SEDehydration.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType2.h>
#include <biogears/cdm/patient/conditions/SEStarvation.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>
#include <biogears/cdm/properties/SEScalarElectricResistance.h>
#include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarInversePressure.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Systems/Cardiovascular.h>
#include <biogears/engine/Systems/Drugs.h>
#include <biogears/engine/Systems/Energy.h>
#include <biogears/engine/Systems/Respiratory.h>
namespace BGE = mil::tatrc::physiology::biogears;

#pragma warning(disable : 4786)
#pragma warning(disable : 4275)

//#define PROBE_BLOOD_GASES
#define GAS_ONLY_PRODCOM

namespace biogears {
double Tissue::m_hepaticCO2Produced_mol;
double Tissue::m_hepaticO2Consumed_mol;

Tissue::Tissue(BioGears& bg)
  : SETissueSystem(bg.GetLogger())
  , m_data(bg)
{
  Clear();
}

Tissue::~Tissue()
{
  Clear();
}

void Tissue::Clear()
{
  SETissueSystem::Clear();
  m_Albumin = nullptr;
  m_Glucose = nullptr;
  m_Triacylglycerol = nullptr;
  m_O2 = nullptr;
  m_CO2 = nullptr;
  m_CO = nullptr;
  m_Lactate = nullptr;
  m_Ketones = nullptr;
  m_Creatinine = nullptr;
  m_Sodium = nullptr;
  m_Calcium = nullptr;
  m_Chloride = nullptr;
  m_Potassium = nullptr;
  m_Insulin = nullptr;

  m_LeftLungTissue = nullptr;
  m_RightLungTissue = nullptr;
  m_MuscleTissue = nullptr;
  m_LiverTissue = nullptr;
  m_FatTissue = nullptr;

  m_FatVascular = nullptr;
  m_FatVascularLipid = nullptr;
  m_LiverVascularGlucose = nullptr;
  m_MuscleVascular = nullptr;
  m_MuscleVascularGlucose = nullptr;

  m_LeftAlveoli = nullptr;
  m_RightAlveoli = nullptr;
  m_LeftPulmonaryCapillaries = nullptr;
  m_RightPulmonaryCapillaries = nullptr;

  m_PatientActions = nullptr;

  m_O2ConsumedRunningAverage_mL_Per_s.Reset();
  m_CO2ProducedRunningAverage_mL_Per_s.Reset();
  m_RespiratoryQuotientRunningAverage.Reset();
  m_FatigueRunningAverage.Reset();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Tissue::Initialize()
{
  BioGearsSystem::Initialize();

  // Get total tissue resting values for substances
  SETissueCompartment* tissue;
  SELiquidCompartment* vascular;
  SEScalarMassPerVolume density;
  GeneralMath::CalculateWaterDensity(m_data.GetEnergy().GetCoreTemperature(), density);
  m_RestingFluidMass_kg = 0.0;
  for (auto tissueVascular : m_TissueToVascular) {
    tissue = tissueVascular.first;
    vascular = tissueVascular.second;
    SELiquidCompartment& extracellular = m_data.GetCompartments().GetExtracellularFluid(*tissue);
    SELiquidCompartment& intracellular = m_data.GetCompartments().GetIntracellularFluid(*tissue);
    m_RestingFluidMass_kg += vascular->GetVolume(VolumeUnit::mL) * m_data.GetBloodChemistry().GetBloodDensity(MassPerVolumeUnit::kg_Per_mL);
    m_RestingFluidMass_kg += intracellular.GetVolume(VolumeUnit::mL) * density.GetValue(MassPerVolumeUnit::kg_Per_mL);
    m_RestingFluidMass_kg += extracellular.GetVolume(VolumeUnit::mL) * density.GetValue(MassPerVolumeUnit::kg_Per_mL);
  }
  m_RestingPatientMass_kg = m_data.GetPatient().GetWeight(MassUnit::kg);
  GetIntracellularFluidPH().SetValue(7.0);

  /// \cite guyton2006medical
  GetOxygenConsumptionRate().SetValue(250.0, VolumePerTimeUnit::mL_Per_min);
  GetCarbonDioxideProductionRate().SetValue(200.0, VolumePerTimeUnit::mL_Per_min);
  GetRespiratoryExchangeRatio().SetValue(0.8);

  //Set baseline insulin and glucagon values after stabilization
  GetLiverInsulinSetPoint().Set(m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Liver)->GetSubstanceQuantity(*m_Insulin)->GetMolarity());
  GetLiverGlucagonSetPoint().Set(m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Liver)->GetSubstanceQuantity(*m_Glucagon)->GetConcentration());
  GetMuscleInsulinSetPoint().Set(m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle)->GetSubstanceQuantity(*m_Insulin)->GetMolarity());
  GetMuscleGlucagonSetPoint().Set(m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle)->GetSubstanceQuantity(*m_Glucagon)->GetConcentration());
  GetFatInsulinSetPoint().Set(m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Fat)->GetSubstanceQuantity(*m_Insulin)->GetMolarity());
  GetFatGlucagonSetPoint().Set(m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Fat)->GetSubstanceQuantity(*m_Glucagon)->GetConcentration());

  //Set nutrient stores (also reset in AtSteadyState)
  GetLiverGlycogen().SetValue(.065 * m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver)->GetTotalMass(MassUnit::g), MassUnit::g);
  GetMuscleGlycogen().SetValue(.02 * m_data.GetPatient().GetMuscleMass(MassUnit::g), MassUnit::g); //2% of muscle mass
  GetStoredProtein().SetValue(110, MassUnit::g); //"Reusable" protein stores are usually about 1% of total body protein, ~110 g (https://www.nap.edu/read/10490/chapter/12#595)
  GetStoredFat().SetValue(m_data.GetPatient().GetWeight(MassUnit::g) * m_data.GetPatient().GetBodyFatFraction().GetValue(), MassUnit::g);

  GetDehydrationFraction().SetValue(0);



}

bool Tissue::Load(const CDM::BioGearsTissueSystemData& in)
{
  if (!SETissueSystem::Load(in))
    return false;

  m_O2ConsumedRunningAverage_mL_Per_s.Load(in.O2ConsumedRunningAverage_mL_Per_s());
  m_CO2ProducedRunningAverage_mL_Per_s.Load(in.CO2ProducedRunningAverage_mL_Per_s());
  m_RespiratoryQuotientRunningAverage.Load(in.RespiratoryQuotientRunningAverage());
  m_RestingPatientMass_kg = in.RestingPatientMass_kg();
  m_RestingFluidMass_kg = in.RestingFluidMass_kg();
  m_FatigueRunningAverage.Load(in.FatigueRunningAverage());

  BioGearsSystem::LoadState();
  return true;
}
CDM::BioGearsTissueSystemData* Tissue::Unload() const
{
  CDM::BioGearsTissueSystemData* data = new CDM::BioGearsTissueSystemData();
  Unload(*data);
  return data;
}
void Tissue::Unload(CDM::BioGearsTissueSystemData& data) const
{
  SETissueSystem::Unload(data);

  data.O2ConsumedRunningAverage_mL_Per_s(std::unique_ptr<CDM::RunningAverageData>(m_O2ConsumedRunningAverage_mL_Per_s.Unload()));
  data.CO2ProducedRunningAverage_mL_Per_s(std::unique_ptr<CDM::RunningAverageData>(m_CO2ProducedRunningAverage_mL_Per_s.Unload()));
  data.RespiratoryQuotientRunningAverage(std::unique_ptr<CDM::RunningAverageData>(m_RespiratoryQuotientRunningAverage.Unload()));
  data.RestingPatientMass_kg(m_RestingPatientMass_kg);
  data.RestingFluidMass_kg(m_RestingFluidMass_kg);
  data.FatigueRunningAverage(std::unique_ptr<CDM::RunningAverageData>(m_FatigueRunningAverage.Unload()));
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes the tissue specific quantities
///
/// \details
/// Initializes substance concentrations and other data in the tissues.
//--------------------------------------------------------------------------------------------------
void Tissue::SetUp()
{
  m_Dt_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  m_Patient = &m_data.GetPatient();

  //"Reusable" protein stores are usually about 1% of total body protein, ~110 g (https://www.nap.edu/read/10490/chapter/12#595)
  m_maxProteinStorage_g = 110;

  Tissue::m_hepaticO2Consumed_mol = 0; //Hepatic system will send O2/CO2 changes from lipogenesis/gluconeogenesis to Tissue to factor into O2 consumption/CO2 production outputs
  Tissue::m_hepaticCO2Produced_mol = 0;

  m_PatientActions = &m_data.GetActions().GetPatientActions();
  m_Patient = &m_data.GetPatient();
  m_energy = &m_data.GetEnergy();

  m_Albumin = &m_data.GetSubstances().GetAlbumin();
  m_AminoAcids = &m_data.GetSubstances().GetAminoAcids();
  m_Glucose = &m_data.GetSubstances().GetGlucose();
  m_Glucagon = &m_data.GetSubstances().GetGlucagon();
  m_Triacylglycerol = &m_data.GetSubstances().GetTriacylglycerol();
  m_O2 = &m_data.GetSubstances().GetO2();
  m_CO2 = &m_data.GetSubstances().GetCO2();
  m_CO = &m_data.GetSubstances().GetCO();
  m_Lactate = &m_data.GetSubstances().GetLactate();
  m_Ketones = &m_data.GetSubstances().GetKetones();
  m_Creatinine = &m_data.GetSubstances().GetCreatinine();
  m_Sodium = &m_data.GetSubstances().GetSodium();
  m_Potassium = &m_data.GetSubstances().GetPotassium();
  m_Calcium = &m_data.GetSubstances().GetCalcium();
  m_Chloride = &m_data.GetSubstances().GetChloride();
  m_Insulin = &m_data.GetSubstances().GetInsulin();
  m_Urea = &m_data.GetSubstances().GetUrea();

  m_MuscleInsulin = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle)->GetSubstanceQuantity(*m_Insulin);
  m_MuscleGlucagon = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle)->GetSubstanceQuantity(*m_Glucagon);
  m_MuscleAA = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle)->GetSubstanceQuantity(*m_AminoAcids);
  m_FatInsulin = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Fat)->GetSubstanceQuantity(*m_Insulin);
  m_FatGlucagon = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Fat)->GetSubstanceQuantity(*m_Glucagon);
  m_FatTAG = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Fat)->GetSubstanceQuantity(*m_Triacylglycerol);

  //These were m_GutE1 and m_GutE1ToE3, respectively.  These were compliance elements in old tissue circuits so I have changed them to their new names
  m_GutE3 = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetNode(BGE::TissueNode::GutE3);
  m_GutE3ToGround = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::GutE3ToGround);

  m_MuscleVascular = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle);
  m_FatVascular = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Fat);

  m_FatVascularLipid = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Fat)->GetSubstanceQuantity(*m_Triacylglycerol);
  m_LiverVascularGlucose = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Liver)->GetSubstanceQuantity(*m_Glucose);
  m_MuscleVascularGlucose = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle)->GetSubstanceQuantity(*m_Glucose);

  m_LeftLungTissue = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::LeftLung);
  m_RightLungTissue = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::RightLung);
  m_LiverTissue = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver);
  m_FatTissue = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Fat);
  m_MuscleTissue = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle);
  m_BrainTissue = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain);

  m_LiverIntracellular = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverIntracellular);
  m_LiverExtracellular = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular);
  m_FatIntracellular = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::FatIntracellular);
  m_MuscleIntracellular = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::MuscleIntracellular);

  m_LeftAlveoli = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::LeftAlveoli);
  m_RightAlveoli = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::RightAlveoli);
  m_LeftPulmonaryCapillaries = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryCapillaries);
  m_RightPulmonaryCapillaries = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightPulmonaryCapillaries);

  //Store tissue-blood pairs
  m_TissueToVascular.clear();
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Fat)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Fat);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Bone)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Bone);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Brain);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Gut)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Gut);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::LeftKidney)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftKidney);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::RightKidney)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightKidney);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Liver);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::LeftLung)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftLung);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::RightLung)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightLung);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Myocardium)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Myocardium);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Skin)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Skin);
  m_TissueToVascular[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Spleen)] = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Spleen);

  //Store paths from vascular node to first extracellular node (i.e. vascular oncotic pressure source)
  m_VascularCopPaths.clear();
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Fat)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::FatVToFatE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Bone)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::BoneVToBoneE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Brain)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::BrainVToBrainE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LargeIntestine)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LargeIntestineVToGutE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::SmallIntestine)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SmallIntestineVToGutE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Splanchnic)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SplanchnicVToGutE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftRenalArtery)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LeftKidneyVToLeftKidneyE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightRenalArtery)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::RightKidneyVToRightKidneyE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Liver)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LiverVToLiverE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftLung)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LeftLungVToLeftLungE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightLung)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::RightLungVToRightLungE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Muscle)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::MuscleVToMuscleE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Myocardium)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::MyocardiumVToMyocardiumE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Skin)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SkinVToSkinE1);
  m_VascularCopPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Spleen)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SpleenVToSpleenE1);

  //In all likelihood will need a map at some point for capillary resistance paths (first extracellular node to second extracellular node)

  //Store paths from second extracellular node to third extracellular node (i.e. interstitial oncotic pressure source)
  m_InterstitialCopPaths.clear();
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Fat)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::FatE2ToFatE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Bone)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::BoneE2ToBoneE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::BrainE2ToBrainE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Gut)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::GutE2ToGutE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::LeftKidney)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LeftKidneyE2ToLeftKidneyE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::RightKidney)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::RightKidneyE2ToRightKidneyE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LiverE2ToLiverE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::LeftLung)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LeftLungE2ToLeftLungE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::RightLung)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::RightLungE2ToRightLungE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::MuscleE2ToMuscleE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Myocardium)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::MyocardiumE2ToMyocardiumE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Skin)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SkinE2ToSkinE3);
  m_InterstitialCopPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Spleen)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SpleenE2ToSpleenE3);

  //Store paths from extracellular space to intracellular space (i.e. flow source paths)
  m_ExtraToIntraPaths.clear();
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Fat)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::FatE3ToFatI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Bone)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::BoneE3ToBoneI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::BrainE3ToBrainI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Gut)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::GutE3ToGutI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::LeftKidney)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LeftKidneyE3ToLeftKidneyI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::RightKidney)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::RightKidneyE3ToRightKidneyI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LiverE3ToLiverI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::LeftLung)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LeftLungE3ToLeftLungI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::RightLung)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::RightLungE3ToRightLungI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::MuscleE3ToMuscleI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Myocardium)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::MyocardiumE3ToMyocardiumI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Skin)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SkinE3ToSkinI);
  m_ExtraToIntraPaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Spleen)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SpleenE3ToSpleenI);

  m_EndothelialResistancePaths.clear();
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Fat)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::FatE1ToFatE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Bone)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::BoneE1ToBoneE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::BrainE1ToBrainE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Gut)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::GutE1ToGutE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::LeftKidney)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LeftKidneyE1ToLeftKidneyE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::RightKidney)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::RightKidneyE1ToRightKidneyE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LiverE1ToLiverE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::LeftLung)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LeftLungE1ToLeftLungE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::RightLung)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::RightLungE1ToRightLungE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::MuscleE1ToMuscleE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Myocardium)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::MyocardiumE1ToMyocardiumE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Skin)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SkinE1ToSkinE2);
  m_EndothelialResistancePaths[m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Spleen)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SpleenE1ToSpleenE2);

  m_LymphPaths.clear();
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::FatExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::FatE3ToFatL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::BoneExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::BoneE3ToBoneL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::BrainExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::BrainE3ToBrainL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::GutExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::GutE3ToGutL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LeftKidneyExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LeftKidneyE3ToLeftKidneyL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::RightKidneyExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::RightKidneyE3ToRightKidneyL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LiverE3ToLiverL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LeftLungExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LeftLungE3ToLeftLungL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::RightLungExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::RightLungE3ToRightLungL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::MuscleExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::MuscleE3ToMuscleL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::MyocardiumExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::MyocardiumE3ToMyocardiumL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::SkinExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SkinE3ToSkinL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::SpleenExtracellular)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SpleenE3ToSpleenL1);
  m_LymphPaths[m_data.GetCompartments().GetLiquidCompartment(BGE::LymphCompartment::Lymph)] = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LymphToVenaCava);

  m_ConsumptionProdutionTissues.clear();
  m_ConsumptionProdutionTissues.push_back(m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Fat));
  m_ConsumptionProdutionTissues.push_back(m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Bone));
  m_ConsumptionProdutionTissues.push_back(m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain));
  m_ConsumptionProdutionTissues.push_back(m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Gut));
  m_ConsumptionProdutionTissues.push_back(m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::LeftKidney));
  m_ConsumptionProdutionTissues.push_back(m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::RightKidney));
  m_ConsumptionProdutionTissues.push_back(m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver));
  m_ConsumptionProdutionTissues.push_back(m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle));
  m_ConsumptionProdutionTissues.push_back(m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Myocardium));
  m_ConsumptionProdutionTissues.push_back(m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Skin));
  m_ConsumptionProdutionTissues.push_back(m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Spleen));

  // Here is some code to cross check our maps with what is in the compartment manager
  // If by some chance, some other system added a new tissue compartment we don't know about
  // this will put out a warning
  for (SETissueCompartment* tissue : m_data.GetCompartments().GetTissueLeafCompartments()) {
    if (m_TissueToVascular.find(tissue) == m_TissueToVascular.end() || m_TissueToVascular[tissue] == nullptr)
      Warning("Tissue found a tissue compartment that is not mapped to a vascular compartment  : " + std::string{ tissue->GetName() });
    /*if (m_VascularCopPaths.find(tissue) == m_VascularCopPaths.end() || m_VascularCopPaths[tissue] == nullptr)
      Warning("Tissue found a tissue compartment that does not have a vascular colloid oncotic pressure path  : " + tissue->GetName());*/
    if (m_InterstitialCopPaths.find(tissue) == m_InterstitialCopPaths.end() || m_InterstitialCopPaths[tissue] == nullptr)
      Warning("Tissue found a tissue compartment that does not have an interstitial colloid oncotic pressure path  : " + std::string{ tissue->GetName() });
    if (m_ExtraToIntraPaths.find(tissue) == m_ExtraToIntraPaths.end() || m_ExtraToIntraPaths[tissue] == nullptr)
      Warning("Tissue found a tissue compartment that does not an extracellular to intracellular path  : " + std::string{ tissue->GetName() });
    if (m_LeftLungTissue != tissue && m_RightLungTissue != tissue) { // We don't use the lungs in the consumption/production methodology
      if (!Contains(m_ConsumptionProdutionTissues, (*tissue)))
        Warning("Tissue found a tissue compartment that it is not using in Consumption/Production : " + std::string{ tissue->GetName() });
    }
  }
}

//#define logMeal
void Tissue::AtSteadyState()
{

  if (m_data.GetState() == EngineState::AtInitialStableState) {
    // Apply our conditions
    if (m_data.GetConditions().HasStarvation())
      SetStarvationState();
    if (m_data.GetConditions().HasDehydration())
      Dehydrate();
  }

  if (m_data.GetState() == EngineState::AtSecondaryStableState && !m_data.GetConditions().HasStarvation()) {
    //refill nutrient stores that were depleted during stabilization (should match SetUp)
    GetLiverGlycogen().SetValue(.065 * m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver)->GetTotalMass(MassUnit::g), MassUnit::g);
    GetMuscleGlycogen().SetValue(.02 * m_data.GetPatient().GetMuscleMass(MassUnit::g), MassUnit::g); //2% of muscle mass
    GetStoredProtein().SetValue(110, MassUnit::g);
    GetStoredFat().SetValue(m_data.GetPatient().GetWeight(MassUnit::g) * m_data.GetPatient().GetBodyFatFraction().GetValue(), MassUnit::g);
    Info("Nutrient stores refilled after stabilization");
    //TEMPORARILY set to starved state
    /*
    GetLiverGlycogen().SetValue(0, MassUnit::g);
    GetMuscleGlycogen().SetValue(0, MassUnit::g);
    GetStoredProtein().SetValue(110, MassUnit::g);
    GetStoredFat().SetValue(m_data.GetPatient().GetWeight(MassUnit::g) * m_data.GetPatient().GetBodyFatFraction().GetValue(), MassUnit::g);
    Info("Nutrient stores set to altered values for TESTING");
    */
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets the status of nutrient stores and blood concentrations to appropriate starved values
///
/// \details
/// Liver glycogen, muscle glycogen, muscle mass, stored fat, and stored protein are set based on a
/// linear relationship with the starvation duration. Blood concentrations of glucose and ketones are
/// set based on second-order fits of literature values. No other substances are directly set by the
/// starvation condition.
//--------------------------------------------------------------------------------------------------
void Tissue::SetStarvationState()
{
  double starvedTime_hr = m_data.GetConditions().GetStarvation()->GetTimeSinceMeal().GetValue(TimeUnit::hr);
  double totalMassLost_g = 0;

  //Set new storage values

  //Liver glycogen
  //Based on engine runtime, lasts ~51 hours, decrease is nearly linear
  double liverGlycogen_g = GeneralMath::LinearInterpolator(0, 51, .065 * m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver)->GetTotalMass(MassUnit::g), 0, starvedTime_hr);
  LLIM(liverGlycogen_g, 0); //can't have negative
  GetLiverGlycogen().SetValue(liverGlycogen_g, MassUnit::g);
  totalMassLost_g += (.065 * m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver)->GetTotalMass(MassUnit::g)) - liverGlycogen_g;

  //Muscle glycogen
  //Based on engine runtime, drops to ~425g after 76.4 hours, nearly linear
  double muscleGlycogen_g = GeneralMath::LinearInterpolator(0, 76.4, .02 * m_Patient->GetMuscleMass(MassUnit::g), 425, starvedTime_hr);
  LLIM(muscleGlycogen_g, 0); //can't have negative
  GetMuscleGlycogen().SetValue(muscleGlycogen_g, MassUnit::g);
  totalMassLost_g += (.02 * m_Patient->GetMuscleMass(MassUnit::g)) - muscleGlycogen_g;

  //Stored Protein
  //Based on engine runtime, lasts ~45.5 hours, decrease is nearly linear
  double storedProtein_g = GeneralMath::LinearInterpolator(0, 45.5, 110, 0, starvedTime_hr);
  LLIM(storedProtein_g, 0); //can't have negative
  GetStoredProtein().SetValue(liverGlycogen_g, MassUnit::g);
  totalMassLost_g += (110) - storedProtein_g;

  //Muscle Mass
  //Drops at same rate as stored protein, but only after that is used up
  //If they're starved so long they've used all of their protein, they're dead
  double muscleMassDecrease_g = GeneralMath::LinearInterpolator(0, 45.5, 0, 110, starvedTime_hr);
  BLIM(muscleMassDecrease_g, 0, m_Patient->GetMuscleMass().GetValue(MassUnit::g));
  m_Patient->GetMuscleMass().IncrementValue(-muscleMassDecrease_g, MassUnit::g);
  totalMassLost_g += muscleMassDecrease_g;

  //Stored fat
  //Based on engine runtime, drops ~521g in 96 hours, decrease is nearly linear
  double fat = GetStoredFat().GetValue(MassUnit::g);
  double fatMassDecrease_g = GeneralMath::LinearInterpolator(0, 96, 0, 521, starvedTime_hr);
  BLIM(fatMassDecrease_g, 0, GetStoredFat().GetValue(MassUnit::g));
  GetStoredFat().IncrementValue(-fatMassDecrease_g, MassUnit::g);
  fat = GetStoredFat().GetValue(MassUnit::g);
  totalMassLost_g += fatMassDecrease_g;

  //Body weight and other patient parameters
  double weight = m_Patient->GetWeight().GetValue(MassUnit::g);
  m_Patient->GetWeight().IncrementValue(-totalMassLost_g, MassUnit::g);
  weight = m_Patient->GetWeight().GetValue(MassUnit::g);
  m_Patient->GetBodyFatFraction().SetValue(GetStoredFat().GetValue(MassUnit::g) / m_Patient->GetWeight().GetValue(MassUnit::g));
  m_Patient->GetLeanBodyMass().SetValue(m_Patient->GetWeight().GetValue(MassUnit::g) - GetStoredFat().GetValue(MassUnit::g), MassUnit::g);
  double SiriBodyDensity_g_Per_cm3 = 4.95 / (m_Patient->GetBodyFatFraction().GetValue() + 4.50);
  double BrozekBodyDensity_g_Per_cm3 = 4.57 / (m_Patient->GetBodyFatFraction().GetValue() + 4.142);
  double bodyDensity_g_Per_cm3 = (SiriBodyDensity_g_Per_cm3 + BrozekBodyDensity_g_Per_cm3) / 2.0;
  m_Patient->GetBodyDensity().SetValue(bodyDensity_g_Per_cm3, MassPerVolumeUnit::g_Per_cm3); //See BioGears::SetUpPatient()

  //Set new blood concentrations
  m_data.GetSubstances().SetLiquidCompartmentNonGasesForStarvation(starvedTime_hr);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Preprocess performs the systems interactions steps required for processing of the substances
///
/// \details
/// This function calculates the amount of albumin produced for a defined time period and adds it to the liver.
//--------------------------------------------------------------------------------------------------
void Tissue::PreProcess()
{
  CalculateTissueFluidFluxes();
  CalculateOncoticPressure();
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Process completes substance transport by performing diffusion and alveoli transfer
///
/// \details
/// This function completes substance transport between systems by calculating the oxygen and carbon dioxide saturation
/// throughout the cardiovascular system and calculating diffusion and alveoli transfer of substances.
//--------------------------------------------------------------------------------------------------
void Tissue::Process()
{
  ProteinStorageAndRelease();
  FatStorageAndRelease();
  CalculateMetabolicConsumptionAndProduction(m_Dt_s);
  CalculatePulmonaryCapillarySubstanceTransfer();
  CalculateDiffusion();
  ManageSubstancesAndSaturation();
  CalculateVitals();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Postprocess step
///
/// \details
/// There are currently no postprocess steps in the tissue system.
//--------------------------------------------------------------------------------------------------
void Tissue::PostProcess()
{
  if (m_data.GetActions().GetPatientActions().HasOverride()
      && m_data.GetState() == EngineState::Active) {
    if (m_data.GetActions().GetPatientActions().GetOverride()->HasTissueOverride()) {
      ProcessOverride();
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determines the correct type of diffusion for a substance and executes the diffusion.
///
///
/// \details
/// Determines the type of diffusion for each substance and calls the function for that type of diffusion.
/// This is executed for all compartments in the tissue system. Options include, perfusion limited diffusion,
/// permeability limited diffusion (untested), pressure gradient diffusion, and flat rate diffusion. The appropriate
/// diffusion methodology is chosen based on the substance parameters.
//--------------------------------------------------------------------------------------------------
void Tissue::CalculateDiffusion()
{
  SETissueCompartment* tissue;
  SELiquidCompartment* vascular;
  SELiquidCompartment* lymph = m_data.GetCompartments().GetLiquidCompartment(BGE::LymphCompartment::Lymph);
  SELiquidCompartment* venaCava = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::VenaCava);
  const SESubstanceTissuePharmacokinetics* tissueKinetics;
  for (auto tissueVascular : m_TissueToVascular) {
    tissue = tissueVascular.first;
    vascular = tissueVascular.second;
    SELiquidCompartment& extracellular = m_data.GetCompartments().GetExtracellularFluid(*tissue);
    SELiquidCompartment& intracellular = m_data.GetCompartments().GetIntracellularFluid(*tissue);
    //The extracellular to intracellular transport of the ions Na, K, Cl, and Ca are handled by the CoupledIonTransport method
    CoupledIonTransport(*tissue, extracellular, intracellular, *vascular);
    for (const SESubstance* sub : m_data.GetCompartments().GetLiquidCompartmentSubstances()) {
      double moved_ug; //used only for possible debugging output
      tissueKinetics = nullptr;
      if (sub->HasPK())
        tissueKinetics = sub->GetPK()->GetTissueKinetics(tissue->GetName());
      //Check to see if substance is a drug with the appropriate parameters to calculate PK diffusion
      // If the substance is a PBPK drug, then diffusion is computed by perfusion limited diffusion, as described in \cite huisinga2012modeling
      if (tissueKinetics != nullptr) {
        if (!tissueKinetics->HasPartitionCoefficient()) {
          Error("Attempted to diffuse a substance with PK that had no partition coefficient available.");
          continue;
        }
        PerfusionLimitedDiffusion(*tissue, *vascular, *sub, tissueKinetics->GetPartitionCoefficient(), m_Dt_s); //Balance happens in the method
      }
      // Otherwise, the diffusion is computed by either:
      // Instantaneous diffusion, Simple diffusion, Facilitated diffusion, or Active diffusion
      else {

        //We have to make an exception for the brain and TAGs, since TAG can't cross blood-brain barrier
        if (sub->GetName() == "Triacylglycerol" && std::string{ extracellular.GetName() }.find("Brain") != std::string::npos)
          continue;

        //Gases get moved by instant diffusion
        if (sub->GetState() == CDM::enumSubstanceState::Gas) {
          //Vascular to Extracellular
          moved_ug = MoveMassByInstantDiffusion(*vascular, extracellular, *sub, m_Dt_s);

          //Extracellular to Intracellular
          moved_ug = MoveMassByInstantDiffusion(extracellular, intracellular, *sub, m_Dt_s);
        }
        //Bicarbonate doesn't diffuse because it is charged
        else if (sub->GetName() != "Bicarbonate") {
          double molarMass_g_Per_mol = sub->GetMolarMass(MassPerAmountUnit::g_Per_mol);

          //Simple diffusion calculates a permeability based on molecular weight. Even large molecules will diffuse, though slowly.
          //We want to prevent movement of large molecules like proteins completely. A gate of 1000 g/mol will filter out things like
          //albumin, insulin, etc while allowing glucose, ions, and others to be governed by their molecular weight.
          //Note: it doesn't consider lipophilicity, so TAG will need to be artificially tweaked using other diffusion methods.
          if (molarMass_g_Per_mol < 1000) {
            // Compute the vascular to extracellular permeability coefficient
            // This is the coefficient per gram of tissue independent of the tissue type.
            // This uses the Renkin and Curry data for capillary exchange as reported in \cite fournier2011basic
            // Divide by 100 is because the Renkin-Curry equations are in per hectogram units, and 100 g/hg
            /// \todo I believe we can optimize with a cache of these values. Also, we can cache permeabilityCoefficient_mL_Per_s_g which is not a function of the tissue properties
            double molecularRadius_nm = 0.0348 * std::pow(molarMass_g_Per_mol, 0.4175);
            double vToECpermeabilityCoefficient_mL_Per_s_g = 0.0287 * std::pow(molecularRadius_nm, -2.920) / 100.0; // This is only valid if the molecular radius is > 1.0 nm.
            if (molecularRadius_nm < 1.0)
              vToECpermeabilityCoefficient_mL_Per_s_g = 0.0184 * std::pow(molecularRadius_nm, -1.223) / 100.0;

            // Multiply by tissue mass to get the tissue-dependent coefficient.
            double vToECpermeabilityCoefficient_mL_Per_s = vToECpermeabilityCoefficient_mL_Per_s_g * tissue->GetTotalMass(MassUnit::g);

            // Tuning factors can be used to help tune the dynamics - note that concentrations will ALWAYS equilibrate in steady state given enough time regardless of the permeability
            double vToECPermeabilityTuningFactor = 1.0;
            double ECtoICPermeabilityTuningFactor = 1.0;

            //Vascular to Extracellular
            moved_ug = MoveMassBySimpleDiffusion(*vascular, extracellular, *sub, vToECPermeabilityTuningFactor * vToECpermeabilityCoefficient_mL_Per_s, m_Dt_s);

            //Extracellular to Intracellular
            // Assuming that the capillary permeability coefficient is proportional to the cellular membrane permeability coefficient for a given tissue and substance
            //Don't this is if substance is an ion, since extra<->intra has already been taken care of for them.  All of the ions have
            //a molar mass < 1000 so they will all funnel here
            if (sub->GetClassification() != CDM::enumSubstanceClass::Ion)
              moved_ug = MoveMassBySimpleDiffusion(extracellular, intracellular, *sub, ECtoICPermeabilityTuningFactor * vToECpermeabilityCoefficient_mL_Per_s, m_Dt_s);
          }

          //Facilitated diffusion depends on the substance having flux values
          //Currently, glucose is the only substance with "real" flux values (but even they are chosen to give good engine behavior)
          //TAG and ketones have "fake" flux values meant to give extra diffusion movement due to lipophilicity
          if (sub->HasMaximumDiffusionFlux()) {
            double massToAreaCoefficient_cm2_Per_g = 1.0; /// \todo Define relationship between tissue mass and membrane area.
            double capCoverage_cm2 = massToAreaCoefficient_cm2_Per_g * tissue->GetTotalMass(MassUnit::g);
            double maximumMassFlux = sub->GetMaximumDiffusionFlux(MassPerAreaTimeUnit::g_Per_cm2_s);
            double combinedCoefficient_g_Per_s = maximumMassFlux * capCoverage_cm2;

            //Vascular to Extracellular
            moved_ug = MoveMassByFacilitatedDiffusion(*vascular, extracellular, *sub, combinedCoefficient_g_Per_s, m_Dt_s);

            //Extracellular to Intracellular
            moved_ug = MoveMassByFacilitatedDiffusion(extracellular, intracellular, *sub, combinedCoefficient_g_Per_s, m_Dt_s);
          }
          ////This Albumin diffusion is in case we start dynamically adjusting oncotic pressures, perhaps during hemorrhages or sepsis
          if (sub->GetName() == "Albumin") {
            moved_ug = AlbuminTransport(*vascular, extracellular, *tissue, m_Dt_s);
            moved_ug = MoveMassByConvection(extracellular, *lymph, *sub, m_Dt_s);
          }
        }

        //Now that mass has been moved, balance to set concentrations and molarities
        vascular->GetSubstanceQuantity(*sub)->Balance(BalanceLiquidBy::Mass);
        extracellular.GetSubstanceQuantity(*sub)->Balance(BalanceLiquidBy::Mass);
        intracellular.GetSubstanceQuantity(*sub)->Balance(BalanceLiquidBy::Mass);
      }
    }
  }
  //Need to transport albumin from lymph to vena cava
  double toLymph = MoveMassByConvection(*lymph, *venaCava, *m_Albumin, m_Dt_s);
  lymph->GetSubstanceQuantity(*m_Albumin)->Balance(BalanceLiquidBy::Mass);
  venaCava->GetSubstanceQuantity(*m_Albumin)->Balance(BalanceLiquidBy::Mass);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Generic substance alveoli transfer calculations
///
/// \param  vSubstances
/// Alveoli transfer is completed for each substance in the input vector of substances.
///
/// \details
/// The mass transferred between the alveoli and the capillaries is calculated for all substances in the
/// input vector. The concentration gradient between the alveoli and the capillaries, the substance-specific
/// alveoli transfer factor, the pulmonary capillary flow, the time step, and the patient-specific alveoli
/// transfer factor are used to calculate the mass transfer. The mass in both the alveoli and the
/// capillaries is then updated. This represents gases moving from the respiratory to the cardiovascular
/// systems and vice versa.
//--------------------------------------------------------------------------------------------------
void Tissue::CalculatePulmonaryCapillarySubstanceTransfer()
{
  SEPatient& Patient = m_data.GetPatient();
  const BioGearsConfiguration& Configuration = m_data.GetConfiguration();

  double AlveoliSurfaceArea_cm2 = Patient.GetAlveoliSurfaceArea(AreaUnit::cm2);
  double PulmonaryCapillaryCoverage = Configuration.GetStandardPulmonaryCapillaryCoverage();
  double DiffusionSurfaceArea_cm2 = AlveoliSurfaceArea_cm2 * PulmonaryCapillaryCoverage;
  double RightLungRatio = Patient.GetRightLungRatio().GetValue();

  double StandardDiffusingCapacityOfOxygen_mLPersPermmHg = (DiffusionSurfaceArea_cm2 * Configuration.GetStandardOxygenDiffusionCoefficient(AreaPerTimePressureUnit::cm2_Per_s_mmHg)) / Configuration.GetStandardDiffusionDistance(LengthUnit::cm);
  double DiffusingCapacityPerSide_mLPerSPermmHg = StandardDiffusingCapacityOfOxygen_mLPersPermmHg;

  for (SESubstance* sub : m_data.GetSubstances().GetActiveGases()) {
    sub->GetAlveolarTransfer().SetValue(0, VolumePerTimeUnit::mL_Per_s);
    sub->GetDiffusingCapacity().SetValue(0, VolumePerTimePressureUnit::mL_Per_s_mmHg);

    //Left Side Alveoli Transfer
    DiffusingCapacityPerSide_mLPerSPermmHg = StandardDiffusingCapacityOfOxygen_mLPersPermmHg * (1 - RightLungRatio);
    AlveolarPartialPressureGradientDiffusion(*m_LeftAlveoli, *m_LeftPulmonaryCapillaries, *sub, DiffusingCapacityPerSide_mLPerSPermmHg, m_Dt_s);

    //Right Side Alveoli Transfer
    DiffusingCapacityPerSide_mLPerSPermmHg = StandardDiffusingCapacityOfOxygen_mLPersPermmHg * RightLungRatio;
    AlveolarPartialPressureGradientDiffusion(*m_RightAlveoli, *m_RightPulmonaryCapillaries, *sub, DiffusingCapacityPerSide_mLPerSPermmHg, m_Dt_s);

    if (m_LeftAlveoli->GetSubstanceQuantity(*sub)->GetVolume(VolumeUnit::mL) < 0.0 || m_LeftPulmonaryCapillaries->GetSubstanceQuantity(*sub)->GetMass(MassUnit::ug) < 0.0 || m_RightAlveoli->GetSubstanceQuantity(*sub)->GetVolume(VolumeUnit::mL) < 0.0 || m_RightPulmonaryCapillaries->GetSubstanceQuantity(*sub)->GetMass(MassUnit::ug) < 0.0) {
      Fatal("Diffusion mass cannot be negative");
    }
  }
  m_LeftAlveoli->Balance(BalanceGasBy::Volume);
  m_RightAlveoli->Balance(BalanceGasBy::Volume);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Conversions of nutrients to metabolic energy are calculated for each relevant compartment
///
/// \details
/// The desired metabolic rate (including exercise) is used to drive the consumption of nutrients
/// and the production of waste products in the tissues. Each compartment is given a metabolic need
/// based on the amount of blood flow to the compartment, and the substances in the compartment
/// are modified based on stoichiometric relationships. O2 and CO2 are consumed and produced,
/// respectively. This method handles both aerobic and anaerobic metabolism, and it sets the
/// Fatigue event if necessary.
//--------------------------------------------------------------------------------------------------
void Tissue::CalculateMetabolicConsumptionAndProduction(double time_s)
{
  //Inputs and outputs
  double TMR_kcal_Per_s = m_data.GetEnergy().GetTotalMetabolicRate(PowerUnit::kcal_Per_s);
  double BMR_kcal_Per_s = m_data.GetPatient().GetBasalMetabolicRate(PowerUnit::kcal_Per_s);
  double baseEnergyRequested_kcal = BMR_kcal_Per_s * time_s;
  double exerciseEnergyRequested_kcal = (TMR_kcal_Per_s - BMR_kcal_Per_s) * time_s;
  double hypoperfusionDeficit_kcal = m_data.GetEnergy().GetEnergyDeficit(PowerUnit::kcal_Per_s) * time_s;
  double brainNeededEnergy_kcal = .2 * baseEnergyRequested_kcal; //brain requires a roughly constant 20% of basal energy regardless of exercise \cite raichle2002appraising
  double nonbrainNeededEnergy_kcal = baseEnergyRequested_kcal - brainNeededEnergy_kcal + exerciseEnergyRequested_kcal + hypoperfusionDeficit_kcal;
  double brainEnergyDeficit_kcal = 0;
  double nonbrainEnergyDeficit_kcal = 0;
  double totalO2Consumed_mol = 0;
  double totalCO2Produced_mol = 0;
  double respiratoryQuotient = 0;
  double oxygenConsumptionRate_g_Per_s = 0.0;
  double carbonDioxideProductionRate_g_Per_s = 0.0;
  double lactateProductionRate_mol_Per_s = 0;
  double heatGenerated_kcal = 0;
  double achievedWorkRate_W = 0;
  double achievedExerciseLevel = 0;
  double fatigueLevel = 0;
  static double totalFatConsumed_g = 0;

  //Data
  double energyPerMolATP_kcal = m_data.GetConfiguration().GetEnergyPerATP(EnergyPerAmountUnit::kcal_Per_mol);
  double ATP_Per_Glucose = 29.85; //The full aerobic glucose breakdown gives ~29.85 ATP, including inefficiencies \cite rich2003molecular
  double CO2_Per_Glucose = 6;
  double O2_Per_Glucose = 6;
  double ATP_Per_Ketone = 24; //Assuming acetoacetate
  double CO2_Per_Ketone = 6; //double check this, ketones consumed via citric acid cycle, so should be the same as glucose
  double O2_Per_Ketone = 6; //Not sure about this either
  double ATP_Per_AA = 13;
  double CO2_Per_AA = 1.5;
  double Urea_Per_AA = .5;
  double O2_Per_AA = 1.875; //assuming RQ is .8 for AA, this should be 1.875
  double ATP_Per_TAG = 330; //Assuming all 3 fatty chains are palmitate, not consumed at theoretical yield (so 106 ATP each), and 12 ATP from glycerol backbone to pyruvate to Citric Acid Cycle \cite boron2012medical p 1231
  double CO2_Per_TAG = 55; // \cite meerman2014when
  double O2_Per_TAG = 78;
  double aerobic_ATP_Per_Glycogen = ATP_Per_Glucose + 1; //Since muscle glycogen is already phosphorylated, we get more ATP from it than when we split glucose by glycolysis \cite guyton2006medical p 904
  double anaerobic_ATP_Per_Glycogen = 3;
  double lactate_Per_Glycogen = 2;
  double anaerobic_ATP_Per_Glucose = 2;
  double lactate_Per_Glucose = 2;
  double glucose_CellularEfficiency = energyPerMolATP_kcal * ATP_Per_Glucose / 686; //686 kcal/mol energy generated when burning glucose completely via bomb calorimeter \cite boron2012medical
  double TAG_CellularEfficiency = energyPerMolATP_kcal * ATP_Per_TAG / (3 * 2340 + 686); //Palmitic acid free energy is 2340 kcal, glycerol is similar to glucose, so assume 686 \cite voet2013fundamentals
  double AA_CellularEfficiency = energyPerMolATP_kcal * ATP_Per_AA / 387.189; //Alanine heat of combustion is 1.62 MJ/mol \cite livesey1984energy
  double ketones_CellularEfficiency = glucose_CellularEfficiency; //Assuming the same as glucose
  double mandatoryMuscleAnaerobicFraction = .1; //There is always some anaerobic consumption in the body, particularly in muscle fibers with few mitochondria \cite boron2012medical
  double kcal_Per_day_Per_Watt = 20.6362855;
  double maxWorkRate_W = 1200; //see Energy::Exercise

  //Patients with COPD show higher levels of anaerobic metabolism \cite mathur1999cerebral \cite engelen2000factors
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease()) {
    mandatoryMuscleAnaerobicFraction *= 1.5; //50% increase
  }
  //Patients with sepsis have elevated lactate levels (i.e. increased anaerobic activity) because of poor oxygen perfusion to tissues
  //Assume that the muscles are most effected by this deficit and increase the anaerobic fraction as a function of sepsis progression
  if (m_PatientActions->HasSepsis()) {
    //Making this so that highest severity increases levels by 50% like the COPD function above, time scale seems about right.  Lower increases for lower severities because those
    //scenarios will have more time to accumulate lactate
    mandatoryMuscleAnaerobicFraction *= (1.0 + m_PatientActions->GetSepsis()->GetSeverity().GetValue() / 2.0);
    //If we get to severe sepsis but do not have lactic acidosis yet, ramp up fraction another 50% (theoretical max = 0.1 * (1.5)^2 = 0.225)
    if (m_Patient->IsEventActive(CDM::enumPatientEvent::SevereSepsis) && !m_Patient->IsEventActive(CDM::enumPatientEvent::LacticAcidosis)) {
      mandatoryMuscleAnaerobicFraction *= 1.5;
    }
  }
  if (m_PatientActions->HasHemorrhage()) {
    double maxBleedingRate_mL_Per_min = 200.0;
    double bleedingRate_mL_Per_min = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Ground)->GetInFlow(VolumePerTimeUnit::mL_Per_min);
    mandatoryMuscleAnaerobicFraction = 0.8 * bleedingRate_mL_Per_min / maxBleedingRate_mL_Per_min;
  }

  //Reusable values for looping
  SELiquidCompartment* vascular;
  SELiquidSubstanceQuantity* TissueO2;
  SELiquidSubstanceQuantity* TissueCO2;
  SELiquidSubstanceQuantity* TissueGlucose;
  SELiquidSubstanceQuantity* TissueTAG;
  SELiquidSubstanceQuantity* TissueLactate;
  SELiquidSubstanceQuantity* TissueKetones;
  SELiquidSubstanceQuantity* TissueAA;
  SELiquidSubstanceQuantity* TissueUrea;
  double TissueVolume_L;

  //--------------------------------------------------------------------------------------------------
  /// The metabolic fraction for each tissue compartment is computed as the fraction of the total vascular blood flow
  /// that is going into the specific vascular compartment that is associated with the tissue compartment.
  //--------------------------------------------------------------------------------------------------
  double totalFlowRate_mL_Per_min = 0.0;
  // First sum the vascular flow into all of the vascular compartments that are associated with the tissue compartments.
  // We do not use the cardiac output as total flow rate because all of the fractions must sum to one at each time slice.
  for (SETissueCompartment* tissue : m_ConsumptionProdutionTissues) {
    if (tissue == m_BrainTissue)
      continue;
    vascular = m_TissueToVascular[tissue];
    if (vascular->HasInFlow())
      totalFlowRate_mL_Per_min += vascular->GetInFlow(VolumePerTimeUnit::mL_Per_min);
  }

  for (SETissueCompartment* tissue : m_ConsumptionProdutionTissues) {
    vascular = m_TissueToVascular[tissue];
    SELiquidCompartment& intracellular = m_data.GetCompartments().GetIntracellularFluid(*tissue);
    TissueO2 = intracellular.GetSubstanceQuantity(*m_O2);
    TissueCO2 = intracellular.GetSubstanceQuantity(*m_CO2);
    TissueGlucose = intracellular.GetSubstanceQuantity(*m_Glucose);
    TissueTAG = intracellular.GetSubstanceQuantity(*m_Triacylglycerol);
    TissueLactate = intracellular.GetSubstanceQuantity(*m_Lactate);
    TissueKetones = intracellular.GetSubstanceQuantity(*m_Ketones);
    TissueAA = intracellular.GetSubstanceQuantity(*m_AminoAcids);
    TissueUrea = intracellular.GetSubstanceQuantity(*m_Urea);
    TissueVolume_L = intracellular.GetVolume(VolumeUnit::L);

    double BloodFlowFraction = 0;
    if (vascular->HasInFlow() && totalFlowRate_mL_Per_min > 0) {
      BloodFlowFraction = vascular->GetInFlow(VolumePerTimeUnit::mL_Per_min) / totalFlowRate_mL_Per_min;
    }



    //First, we'll handle brain consumption/production, since it's special
    //Brain can only consume glucose and ketones
    if (tissue == m_BrainTissue) {
      //First, let's check to see how much TOTAL energy exists in the brain as intracellular glucose
      //We take the values that represent the cell's [inefficient] ability to conserve energy in ATP and use the efficiency to quantify the energy lost as heat
      double totalEnergyAsIntracellularBrainGlucose_kcal = (TissueGlucose->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * TissueVolume_L) * ATP_Per_Glucose * energyPerMolATP_kcal / glucose_CellularEfficiency;

      //If we have enough energy as glucose to meet the brain's energy request this timestep, consume it
      if (totalEnergyAsIntracellularBrainGlucose_kcal >= brainNeededEnergy_kcal) {
        //The actual work done by the cell only accounts for a fraction of this energy, the rest is heat
        double glucoseToConsume_mol = glucose_CellularEfficiency * brainNeededEnergy_kcal / energyPerMolATP_kcal / ATP_Per_Glucose;

        //Since all brain consumption is aerobic and so requires O2, check if there's enough
        if (TissueO2->GetMass(MassUnit::g) > glucoseToConsume_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
          TissueGlucose->GetMass().IncrementValue(-glucoseToConsume_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueCO2->GetMass().IncrementValue(glucoseToConsume_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().IncrementValue(-glucoseToConsume_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          heatGenerated_kcal += (1 - glucose_CellularEfficiency) * brainNeededEnergy_kcal;
          brainNeededEnergy_kcal = 0;
        } else {
          glucoseToConsume_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_Glucose;
          TissueGlucose->GetMass().IncrementValue(-glucoseToConsume_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueCO2->GetMass().IncrementValue(glucoseToConsume_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().SetValue(0, MassUnit::g);
          double totalEnergyUsed = glucoseToConsume_mol * ATP_Per_Glucose * energyPerMolATP_kcal / glucose_CellularEfficiency; //we consume glucose, but also generate heat
          brainNeededEnergy_kcal -= totalEnergyUsed;
          heatGenerated_kcal += totalEnergyUsed * (1 - glucose_CellularEfficiency);
        }
        totalO2Consumed_mol += glucoseToConsume_mol * O2_Per_Glucose;
        totalCO2Produced_mol += glucoseToConsume_mol * CO2_Per_Glucose;
      }
      //Otherwise, consume what intracellular glucose is there and track the deficit
      else {
        double glucoseToConsume_mol = TissueGlucose->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * TissueVolume_L;

        //We're still limited by O2
        if (TissueO2->GetMass(MassUnit::g) > glucoseToConsume_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
          TissueGlucose->GetMass().SetValue(0, MassUnit::g);
          TissueCO2->GetMass().IncrementValue(glucoseToConsume_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().IncrementValue(-glucoseToConsume_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          double totalEnergyUsed = glucoseToConsume_mol * ATP_Per_Glucose * energyPerMolATP_kcal / glucose_CellularEfficiency;
          brainNeededEnergy_kcal -= totalEnergyUsed;
          heatGenerated_kcal += totalEnergyUsed * (1 - glucose_CellularEfficiency);
        } else {
          glucoseToConsume_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_Glucose;
          TissueGlucose->GetMass().IncrementValue(-glucoseToConsume_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueCO2->GetMass().IncrementValue(glucoseToConsume_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().SetValue(0, MassUnit::g);
          double totalEnergyUsed = glucoseToConsume_mol * ATP_Per_Glucose * energyPerMolATP_kcal / glucose_CellularEfficiency;
          brainNeededEnergy_kcal -= totalEnergyUsed;
          heatGenerated_kcal += totalEnergyUsed * (1 - glucose_CellularEfficiency);
        }
        totalO2Consumed_mol += glucoseToConsume_mol * O2_Per_Glucose;
        totalCO2Produced_mol += glucoseToConsume_mol * CO2_Per_Glucose;
      }

      //If we still need energy in the brain, ketones have to cover it
      double totalEnergyAsIntracellularBrainKetones_kcal = (TissueKetones->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * TissueVolume_L) * ATP_Per_Ketone * energyPerMolATP_kcal / ketones_CellularEfficiency;

      //If we still need brain energy and we have enough ketones to cover it
      if (brainNeededEnergy_kcal > 0 && totalEnergyAsIntracellularBrainKetones_kcal >= brainNeededEnergy_kcal) {
        double ketonesToConsume_mol = ketones_CellularEfficiency * brainNeededEnergy_kcal / energyPerMolATP_kcal / ATP_Per_Ketone;

        if (TissueO2->GetMass(MassUnit::g) > ketonesToConsume_mol * O2_Per_Ketone * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
          TissueKetones->GetMass().IncrementValue(-ketonesToConsume_mol * m_Ketones->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueCO2->GetMass().IncrementValue(ketonesToConsume_mol * CO2_Per_Ketone * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().IncrementValue(-ketonesToConsume_mol * O2_Per_Ketone * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          heatGenerated_kcal += (1 - glucose_CellularEfficiency) * brainNeededEnergy_kcal;
          brainNeededEnergy_kcal = 0;
        } else {
          ketonesToConsume_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_Ketone;
          TissueKetones->GetMass().IncrementValue(-ketonesToConsume_mol * m_Ketones->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueCO2->GetMass().IncrementValue(ketonesToConsume_mol * CO2_Per_Ketone * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().SetValue(0, MassUnit::g);
          double totalEnergyUsed = ketonesToConsume_mol * ATP_Per_Ketone * energyPerMolATP_kcal / ketones_CellularEfficiency;
          brainNeededEnergy_kcal -= totalEnergyUsed;
          heatGenerated_kcal += totalEnergyUsed * (1 - ketones_CellularEfficiency);
        }
        totalO2Consumed_mol += ketonesToConsume_mol * O2_Per_Ketone;
        totalCO2Produced_mol += ketonesToConsume_mol * CO2_Per_Ketone;
      }
      //Otherwise, consume the ketones in the brain and track the deficit
      else if (brainNeededEnergy_kcal > 0) {
        double ketonesToConsume_mol = TissueKetones->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * TissueVolume_L;

        if (TissueO2->GetMass(MassUnit::g) > ketonesToConsume_mol * O2_Per_Ketone * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
          TissueKetones->GetMass().SetValue(0, MassUnit::g);
          TissueCO2->GetMass().IncrementValue(ketonesToConsume_mol * CO2_Per_Ketone * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().IncrementValue(-ketonesToConsume_mol * O2_Per_Ketone * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          double totalEnergyUsed = ketonesToConsume_mol * ATP_Per_Ketone * energyPerMolATP_kcal / ketones_CellularEfficiency;
          brainNeededEnergy_kcal -= totalEnergyUsed;
          heatGenerated_kcal += totalEnergyUsed * (1 - ketones_CellularEfficiency);
        } else {
          ketonesToConsume_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_Ketone;
          TissueKetones->GetMass().IncrementValue(-ketonesToConsume_mol * m_Ketones->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueCO2->GetMass().IncrementValue(ketonesToConsume_mol * CO2_Per_Ketone * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().SetValue(0, MassUnit::g);
          double totalEnergyUsed = ketonesToConsume_mol * ATP_Per_Ketone * energyPerMolATP_kcal / ketones_CellularEfficiency;
          brainNeededEnergy_kcal -= totalEnergyUsed;
          heatGenerated_kcal += totalEnergyUsed * (1 - ketones_CellularEfficiency);
        }
        totalO2Consumed_mol += ketonesToConsume_mol * O2_Per_Ketone;
        totalCO2Produced_mol += ketonesToConsume_mol * CO2_Per_Ketone;
      }

      brainEnergyDeficit_kcal = brainNeededEnergy_kcal > 0 ? brainNeededEnergy_kcal : 0; //Any needed energy we have left is a deficit

      //Useful debugging information
      //m_data.GetDataTrack().Probe("BrainDeficitFraction", brainEnergyDeficit_kcal / (.2 * baseEnergyRequested_kcal));

      TissueGlucose->Balance(BalanceLiquidBy::Mass);
      TissueO2->Balance(BalanceLiquidBy::Mass);
      TissueCO2->Balance(BalanceLiquidBy::Mass);
      TissueKetones->Balance(BalanceLiquidBy::Mass);

      continue; //nothing else to do for this tissue
    }

    //Now do non-brain tissues, which can consume TAG, AA, and glucose (and glycogen for muscles)
    //They can also consume ketones in some quantities, but we're not modeling that
    //The muscles always have some level of anaerobic activity
    //Additionally, the muscles perform all of the additional work from exercise
    double tissueNeededEnergy_kcal = nonbrainNeededEnergy_kcal * BloodFlowFraction;
    double muscleMandatoryAnaerobicNeededEnergy_kcal = 0;
    if (tissue == m_MuscleTissue) {
      muscleMandatoryAnaerobicNeededEnergy_kcal = mandatoryMuscleAnaerobicFraction * tissueNeededEnergy_kcal;
      tissueNeededEnergy_kcal -= muscleMandatoryAnaerobicNeededEnergy_kcal;
      tissueNeededEnergy_kcal += exerciseEnergyRequested_kcal;

      double creatinineProductionRate_mg_Per_s = 2.0e-5; /// \todo Creatinine production rate should be a function of muscle mass.
      intracellular.GetSubstanceQuantity(*m_Creatinine)->GetMass().IncrementValue(creatinineProductionRate_mg_Per_s * m_Dt_s, MassUnit::mg);
    }

    //Start with AA, since obligatory protein loss is 30g/day minimum going up to 125g/day in starvation \cite guyton2006medical
    //However, some of this consumption is due to gluconeogenesis (10-20 grams or so, see \cite garber1974hepatic)
    //Use hormone factor (based on liver) to determine "how starved" you are and vary AA consumption linearly from 15 to 110
    double localHormoneFactor = Hepatic::CalculateRelativeHormoneChange(GetLiverInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L) * 1e9, GetLiverGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL) * 1e9, vascular->GetSubstanceQuantity(*m_Insulin), vascular->GetSubstanceQuantity(*m_Glucagon), m_data);
    BLIM(localHormoneFactor, -2, 0); //positive hormone factors mean we should consume the expected 30g/day
    double AAConsumptionRate_g_Per_day = GeneralMath::LinearInterpolator(0, 2, 15, 110, -localHormoneFactor);

    double AAToConsume_mol = (AAConsumptionRate_g_Per_day * time_s * BloodFlowFraction) / (24 * 3600 * m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol));

    //See if we actually have enough AA to meet the request and carry it out
    double intracellularAA_mol = TissueAA->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * TissueVolume_L;
    double AADeficit_mol = intracellularAA_mol - AAToConsume_mol;

    //There wasn't enough; consume all intracellular AA and track deficit if desired
    if (AADeficit_mol < 0) {
      //AA consumption is aerobic
      if (TissueO2->GetMass(MassUnit::g) > intracellularAA_mol * O2_Per_AA * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
        TissueAA->GetMass().SetValue(0, MassUnit::g);
        m_LiverExtracellular->GetSubstanceQuantity(*m_Urea)->GetMass().IncrementValue(intracellularAA_mol * Urea_Per_AA * m_Urea->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueCO2->GetMass().IncrementValue(intracellularAA_mol * CO2_Per_AA * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().IncrementValue(-intracellularAA_mol * O2_Per_AA * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        double totalEnergyUsed = intracellularAA_mol * ATP_Per_AA * energyPerMolATP_kcal / AA_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        tissueNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - AA_CellularEfficiency);
        totalO2Consumed_mol += intracellularAA_mol * O2_Per_AA;
        totalCO2Produced_mol += intracellularAA_mol * CO2_Per_AA;
      } else {
        double AAActuallyConsumed_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_AA;
        TissueAA->GetMass().IncrementValue(-AAActuallyConsumed_mol * m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        m_LiverExtracellular->GetSubstanceQuantity(*m_Urea)->GetMass().IncrementValue(AAActuallyConsumed_mol * Urea_Per_AA * m_Urea->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueCO2->GetMass().IncrementValue(AAActuallyConsumed_mol * CO2_Per_AA * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().SetValue(0, MassUnit::g);
        double totalEnergyUsed = AAActuallyConsumed_mol * ATP_Per_AA * energyPerMolATP_kcal / AA_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        tissueNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - AA_CellularEfficiency);
        totalO2Consumed_mol += AAActuallyConsumed_mol * O2_Per_AA;
        totalCO2Produced_mol += AAActuallyConsumed_mol * CO2_Per_AA;
      }
    }
    //There was enough; consume the required amount
    else {
      //AA consumption is aerobic
      if (TissueO2->GetMass(MassUnit::g) > AAToConsume_mol * O2_Per_AA * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
        TissueAA->GetMass().IncrementValue(-AAToConsume_mol * m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        m_LiverExtracellular->GetSubstanceQuantity(*m_Urea)->GetMass().IncrementValue(AAToConsume_mol * Urea_Per_AA * m_Urea->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueCO2->GetMass().IncrementValue(AAToConsume_mol * CO2_Per_AA * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().IncrementValue(-AAToConsume_mol * O2_Per_AA * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        double totalEnergyUsed = AAToConsume_mol * ATP_Per_AA * energyPerMolATP_kcal / AA_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        tissueNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - AA_CellularEfficiency);
        totalO2Consumed_mol += AAToConsume_mol * O2_Per_AA;
        totalCO2Produced_mol += AAToConsume_mol * CO2_Per_AA;
      }
      //O2 is limiting
      else {
        double AAActuallyConsumed_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_AA;
        TissueAA->GetMass().IncrementValue(-AAActuallyConsumed_mol * m_AminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        m_LiverExtracellular->GetSubstanceQuantity(*m_Urea)->GetMass().IncrementValue(AAActuallyConsumed_mol * Urea_Per_AA * m_Urea->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueCO2->GetMass().IncrementValue(AAActuallyConsumed_mol * CO2_Per_AA * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().SetValue(0, MassUnit::g);
        double totalEnergyUsed = AAActuallyConsumed_mol * ATP_Per_AA * energyPerMolATP_kcal / AA_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        tissueNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - AA_CellularEfficiency);
        totalO2Consumed_mol += AAActuallyConsumed_mol * O2_Per_AA;
        totalCO2Produced_mol += AAActuallyConsumed_mol * CO2_Per_AA;
      }
    }

    //Under aerobic conditions, muscles and other tissues burn intracellular FFA by B-oxidation
    //When fat consumption is unlimited, glycogen is virtually unused, so limit the rate of fat consumption based on hormone factor
    //When fat is completely unused, full glycogen stores last about 16 hours
    //The rate should obviously vary between these endpoints, but we have to manage RQ and fat consumption, too
    //We expect to consume 50-70 or so grams of fat per day, so mols of TAG should be on the order of 1.7e-8, ballpark
    //For now, vary linearly between 0 and an empirically determined .001 that gives 60g/day fat consumption and lowers glycogen duration
    //todo readdress this scaling with respect to exercise to try to make the respiratory quotient trend correctly
    double rateLimitingTuningFactor = GeneralMath::LinearInterpolator(0, 2, 0, .001, -localHormoneFactor);

    double usableEnergyAsTissueTAG_kcal = rateLimitingTuningFactor * (TissueTAG->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * TissueVolume_L) * ATP_Per_TAG * energyPerMolATP_kcal / TAG_CellularEfficiency;

    //If we have enough usable intracellular TAG to meet the request for this tissue
    if (tissueNeededEnergy_kcal > 0 && usableEnergyAsTissueTAG_kcal >= tissueNeededEnergy_kcal) {
      double TAGToConsume_mol = TAG_CellularEfficiency * tissueNeededEnergy_kcal / energyPerMolATP_kcal / ATP_Per_TAG;

      //TAG consumption is aerobic
      if (TissueO2->GetMass(MassUnit::g) > TAGToConsume_mol * O2_Per_TAG * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
        TissueTAG->GetMass().IncrementValue(-TAGToConsume_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueCO2->GetMass().IncrementValue(TAGToConsume_mol * CO2_Per_TAG * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().IncrementValue(-TAGToConsume_mol * O2_Per_TAG * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        double totalEnergyUsed = TAGToConsume_mol * ATP_Per_TAG * energyPerMolATP_kcal / TAG_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - TAG_CellularEfficiency);
        tissueNeededEnergy_kcal = 0;
        totalFatConsumed_g += TAGToConsume_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);
      } else {
        TAGToConsume_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_TAG;
        TissueTAG->GetMass().IncrementValue(-TAGToConsume_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueCO2->GetMass().IncrementValue(TAGToConsume_mol * CO2_Per_TAG * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().SetValue(0, MassUnit::g);
        double totalEnergyUsed = TAGToConsume_mol * ATP_Per_TAG * energyPerMolATP_kcal / TAG_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - TAG_CellularEfficiency);
        tissueNeededEnergy_kcal -= totalEnergyUsed;
        totalFatConsumed_g += TAGToConsume_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);
      }
      totalO2Consumed_mol += TAGToConsume_mol * O2_Per_TAG;
      totalCO2Produced_mol += TAGToConsume_mol * CO2_Per_TAG;
    }
    //If there's not enough, consume what we can
    else if (tissueNeededEnergy_kcal > 0 && usableEnergyAsTissueTAG_kcal > 0) {
      double TAGToConsume_mol = rateLimitingTuningFactor * TissueTAG->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * TissueVolume_L;

      //TAG consumption is aerobic
      if (TissueO2->GetMass(MassUnit::g) > TAGToConsume_mol * O2_Per_TAG * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
        double massToConsume = ((TissueTAG->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_L) * TissueVolume_L) - (TAGToConsume_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol))) < 0 ? TissueTAG->GetMass().GetValue(MassUnit::g) : (TAGToConsume_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol)); //Since we're using the tuning factor, we want to avoid setting to 0 in the wrong cases
        TissueTAG->GetMass().IncrementValue(-massToConsume, MassUnit::g);
        TissueCO2->GetMass().IncrementValue(TAGToConsume_mol * CO2_Per_TAG * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().IncrementValue(-TAGToConsume_mol * O2_Per_TAG * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        double totalEnergyUsed = TAGToConsume_mol * ATP_Per_TAG * energyPerMolATP_kcal / TAG_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - TAG_CellularEfficiency);
        tissueNeededEnergy_kcal -= totalEnergyUsed;
        totalFatConsumed_g += TAGToConsume_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);
      } else {
        TAGToConsume_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_TAG;
        TissueTAG->GetMass().IncrementValue(-TAGToConsume_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueCO2->GetMass().IncrementValue(TAGToConsume_mol * CO2_Per_TAG * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().SetValue(0, MassUnit::g);
        double totalEnergyUsed = TAGToConsume_mol * ATP_Per_TAG * energyPerMolATP_kcal / TAG_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - TAG_CellularEfficiency);
        tissueNeededEnergy_kcal -= totalEnergyUsed;
        totalFatConsumed_g += TAGToConsume_mol * m_Triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol);
      }
      totalO2Consumed_mol += TAGToConsume_mol * O2_Per_TAG;
      totalCO2Produced_mol += TAGToConsume_mol * CO2_Per_TAG;
    }

    //Tissues can consume glucose aerobically via glycolysis and Citric Acid Cycle
    double totalEnergyAsTissueIntracellularGlucose_kcal = (TissueGlucose->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * TissueVolume_L) * ATP_Per_Glucose * energyPerMolATP_kcal / glucose_CellularEfficiency;

    //If we have enough intracellular glucose to meet the request
    if (tissueNeededEnergy_kcal > 0 && totalEnergyAsTissueIntracellularGlucose_kcal >= tissueNeededEnergy_kcal) {
      double glucoseToConsume_mol = glucose_CellularEfficiency * tissueNeededEnergy_kcal / energyPerMolATP_kcal / ATP_Per_Glucose;

      //This is aerobic glucose consumption, so it's limited by O2
      if (TissueO2->GetMass(MassUnit::g) > glucoseToConsume_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
        TissueGlucose->GetMass().IncrementValue(-glucoseToConsume_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueCO2->GetMass().IncrementValue(glucoseToConsume_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().IncrementValue(-glucoseToConsume_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        double totalEnergyUsed = glucoseToConsume_mol * ATP_Per_Glucose * energyPerMolATP_kcal / glucose_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - glucose_CellularEfficiency);
        tissueNeededEnergy_kcal = 0;
      } else {
        glucoseToConsume_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_Glucose;
        TissueGlucose->GetMass().IncrementValue(-glucoseToConsume_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueCO2->GetMass().IncrementValue(glucoseToConsume_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().SetValue(0, MassUnit::g);
        double totalEnergyUsed = glucoseToConsume_mol * ATP_Per_Glucose * energyPerMolATP_kcal / glucose_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - glucose_CellularEfficiency);
        tissueNeededEnergy_kcal -= totalEnergyUsed;
      }
      totalO2Consumed_mol += glucoseToConsume_mol * O2_Per_Glucose;
      totalCO2Produced_mol += glucoseToConsume_mol * CO2_Per_Glucose;
    }
    //If we don't have enough glucose
    else if (tissueNeededEnergy_kcal > 0) {
      double glucoseToConsume_mol = TissueGlucose->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * TissueVolume_L;

      //If we have enough O2
      if (TissueO2->GetMass(MassUnit::g) > glucoseToConsume_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
        TissueGlucose->GetMass().SetValue(0, MassUnit::g);
        TissueCO2->GetMass().IncrementValue(glucoseToConsume_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().IncrementValue(-glucoseToConsume_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        double totalEnergyUsed = glucoseToConsume_mol * ATP_Per_Glucose * energyPerMolATP_kcal / glucose_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - glucose_CellularEfficiency);
        tissueNeededEnergy_kcal -= totalEnergyUsed;
      } else {
        glucoseToConsume_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_Glucose;
        TissueGlucose->GetMass().IncrementValue(-glucoseToConsume_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueCO2->GetMass().IncrementValue(glucoseToConsume_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        TissueO2->GetMass().SetValue(0, MassUnit::g);
        double totalEnergyUsed = glucoseToConsume_mol * ATP_Per_Glucose * energyPerMolATP_kcal / glucose_CellularEfficiency;
        nonbrainNeededEnergy_kcal -= totalEnergyUsed;
        heatGenerated_kcal += totalEnergyUsed * (1 - glucose_CellularEfficiency);
        tissueNeededEnergy_kcal -= totalEnergyUsed;
      }
      totalO2Consumed_mol += glucoseToConsume_mol * O2_Per_Glucose;
      totalCO2Produced_mol += glucoseToConsume_mol * CO2_Per_Glucose;
    }

    //The muscle compartment has its own stores of glycogen that it can consume aerobically
    if (tissueNeededEnergy_kcal > 0 && tissue == m_MuscleTissue) {
      double totalEnergyAsMuscleGlycogen_kcal = (GetMuscleGlycogen(MassUnit::g) / m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol)) * aerobic_ATP_Per_Glycogen * energyPerMolATP_kcal / glucose_CellularEfficiency;

      //If we have enough energy as glycogen, consume it
      //TODO Glycogen should be consumed at a particular rate, like AA
      if (totalEnergyAsMuscleGlycogen_kcal >= tissueNeededEnergy_kcal) {
        double glycogenConsumed_mol = glucose_CellularEfficiency * tissueNeededEnergy_kcal / energyPerMolATP_kcal / aerobic_ATP_Per_Glycogen;

        //If we have enough O2
        if (TissueO2->GetMass(MassUnit::g) > glycogenConsumed_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
          GetMuscleGlycogen().IncrementValue(-glycogenConsumed_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueCO2->GetMass().IncrementValue(glycogenConsumed_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().IncrementValue(-glycogenConsumed_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          double totalEnergyUsed = glycogenConsumed_mol * aerobic_ATP_Per_Glycogen * energyPerMolATP_kcal / glucose_CellularEfficiency;
          nonbrainNeededEnergy_kcal -= totalEnergyUsed;
          heatGenerated_kcal += totalEnergyUsed * (1 - glucose_CellularEfficiency);
          tissueNeededEnergy_kcal = 0;
        } else {
          glycogenConsumed_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_Glucose;
          GetMuscleGlycogen().IncrementValue(-glycogenConsumed_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueCO2->GetMass().IncrementValue(glycogenConsumed_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().SetValue(0, MassUnit::g);
          double totalEnergyUsed = glycogenConsumed_mol * aerobic_ATP_Per_Glycogen * energyPerMolATP_kcal / glucose_CellularEfficiency;
          nonbrainNeededEnergy_kcal -= totalEnergyUsed;
          heatGenerated_kcal += totalEnergyUsed * (1 - glucose_CellularEfficiency);
          tissueNeededEnergy_kcal -= totalEnergyUsed;
        }
        totalO2Consumed_mol += glycogenConsumed_mol * O2_Per_Glucose;
        totalCO2Produced_mol += glycogenConsumed_mol * CO2_Per_Glucose;
      }
      //Otherwise, consume what glycogen we can
      else {
        double glycogenConsumed_mol = GetMuscleGlycogen(MassUnit::g) / m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol);

        //If we have enough O2
        if (TissueO2->GetMass(MassUnit::g) > glycogenConsumed_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
          GetMuscleGlycogen().SetValue(0, MassUnit::g);
          TissueCO2->GetMass().IncrementValue(glycogenConsumed_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().IncrementValue(-glycogenConsumed_mol * O2_Per_Glucose * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          double totalEnergyUsed = glycogenConsumed_mol * aerobic_ATP_Per_Glycogen * energyPerMolATP_kcal / glucose_CellularEfficiency;
          nonbrainNeededEnergy_kcal -= totalEnergyUsed;
          heatGenerated_kcal += totalEnergyUsed * (1 - glucose_CellularEfficiency);
          tissueNeededEnergy_kcal -= totalEnergyUsed;
        } else {
          glycogenConsumed_mol = TissueO2->GetMass(MassUnit::g) / m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / O2_Per_Glucose;
          GetMuscleGlycogen().IncrementValue(-glycogenConsumed_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueCO2->GetMass().IncrementValue(glycogenConsumed_mol * CO2_Per_Glucose * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          TissueO2->GetMass().SetValue(0, MassUnit::g);
          double totalEnergyUsed = glycogenConsumed_mol * aerobic_ATP_Per_Glycogen * energyPerMolATP_kcal / glucose_CellularEfficiency;
          nonbrainNeededEnergy_kcal -= totalEnergyUsed;
          heatGenerated_kcal += totalEnergyUsed * (1 - glucose_CellularEfficiency);
          tissueNeededEnergy_kcal -= totalEnergyUsed;
        }
        totalO2Consumed_mol += glycogenConsumed_mol * O2_Per_Glucose;
        totalCO2Produced_mol += glycogenConsumed_mol * CO2_Per_Glucose;
      }
    }

    //If all O2 is gone, energy can be gotten from glycolysis only, with the pyruvate products converted to lactate (i.e. anaerobic)
    //Since this metabolic pathway doesn't have the inefficiency of oxidative phosphorylation and citric acid cycle, we won't use the efficiency value
    //This means we won't generate heat, which is probably not accurate
    totalEnergyAsTissueIntracellularGlucose_kcal = (TissueGlucose->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * TissueVolume_L) * anaerobic_ATP_Per_Glucose * energyPerMolATP_kcal;

    //If we have enough intracellular glucose to meet the request
    if (tissueNeededEnergy_kcal > 0 && totalEnergyAsTissueIntracellularGlucose_kcal >= tissueNeededEnergy_kcal) {
      double glucoseToConsume_mol = tissueNeededEnergy_kcal / energyPerMolATP_kcal / anaerobic_ATP_Per_Glucose;
      TissueGlucose->GetMass().IncrementValue(-glucoseToConsume_mol * m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      TissueLactate->GetMass().IncrementValue(glucoseToConsume_mol * lactate_Per_Glucose * m_Lactate->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      nonbrainNeededEnergy_kcal -= glucoseToConsume_mol * anaerobic_ATP_Per_Glucose * energyPerMolATP_kcal;
      tissueNeededEnergy_kcal = 0;
      lactateProductionRate_mol_Per_s += glucoseToConsume_mol * lactate_Per_Glucose / time_s;
      if (m_AnaerobicTissues.find(tissue->GetName()) == std::string::npos) //for tracking only
        m_AnaerobicTissues.append(std::string{ tissue->GetName() } + " ");
    }
    //If we'll use up all the glucose
    else if (tissueNeededEnergy_kcal > 0) {
      double glucoseToConsume_mol = totalEnergyAsTissueIntracellularGlucose_kcal / energyPerMolATP_kcal / anaerobic_ATP_Per_Glucose;
      TissueGlucose->GetMass().SetValue(0, MassUnit::g);
      TissueLactate->GetMass().IncrementValue(glucoseToConsume_mol * lactate_Per_Glucose * m_Lactate->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      nonbrainNeededEnergy_kcal -= glucoseToConsume_mol * anaerobic_ATP_Per_Glucose * energyPerMolATP_kcal;
      tissueNeededEnergy_kcal -= glucoseToConsume_mol * anaerobic_ATP_Per_Glucose * energyPerMolATP_kcal;
      lactateProductionRate_mol_Per_s += glucoseToConsume_mol * lactate_Per_Glucose / time_s;
      if (m_AnaerobicTissues.find(tissue->GetName()) == std::string::npos) //for tracking only
        m_AnaerobicTissues.append(std::string{ tissue->GetName() } + " ");
    }

    //Muscles can convert glycogen anaerobically, too
    //Note anaerobic glycolysis is limited by low pH, a feedback mechanism that isn't modeled here at this time
    if ((tissueNeededEnergy_kcal > 0 || muscleMandatoryAnaerobicNeededEnergy_kcal > 0) && tissue == m_MuscleTissue) {
      double energyAsMuscleGlycogen_kcal = (GetMuscleGlycogen(MassUnit::g) / m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol)) * anaerobic_ATP_Per_Glycogen * energyPerMolATP_kcal;

      //If we have enough
      if (energyAsMuscleGlycogen_kcal >= (tissueNeededEnergy_kcal + muscleMandatoryAnaerobicNeededEnergy_kcal)) {
        double glycogenConsumed_g = ((tissueNeededEnergy_kcal + muscleMandatoryAnaerobicNeededEnergy_kcal) / energyAsMuscleGlycogen_kcal) * GetMuscleGlycogen(MassUnit::g);
        double glycogenConsumed_mol = glycogenConsumed_g / m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol);
        GetMuscleGlycogen().IncrementValue(-glycogenConsumed_g, MassUnit::g);
        TissueLactate->GetMass().IncrementValue(glycogenConsumed_mol * lactate_Per_Glycogen * m_Lactate->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        nonbrainNeededEnergy_kcal -= glycogenConsumed_mol * anaerobic_ATP_Per_Glycogen * energyPerMolATP_kcal;
        lactateProductionRate_mol_Per_s += glycogenConsumed_mol * lactate_Per_Glycogen / time_s;
        if (m_AnaerobicTissues.find(tissue->GetName()) == std::string::npos && tissueNeededEnergy_kcal != 0) //for tracking only
          m_AnaerobicTissues.append(std::string{ tissue->GetName() } + " ");
        muscleMandatoryAnaerobicNeededEnergy_kcal = 0;
        tissueNeededEnergy_kcal = 0;
      }
      //If we still can't meet the energy request, we have an energy deficit
      else {
        double glycogenConsumed_mol = GetMuscleGlycogen(MassUnit::g) / m_Glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol);
        GetMuscleGlycogen().SetValue(0, MassUnit::g);
        TissueLactate->GetMass().IncrementValue(glycogenConsumed_mol * lactate_Per_Glycogen * m_Lactate->GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        nonbrainNeededEnergy_kcal -= glycogenConsumed_mol * anaerobic_ATP_Per_Glycogen * energyPerMolATP_kcal;
        tissueNeededEnergy_kcal -= glycogenConsumed_mol * anaerobic_ATP_Per_Glycogen * energyPerMolATP_kcal;
        lactateProductionRate_mol_Per_s += glycogenConsumed_mol * lactate_Per_Glycogen / time_s;
        if (m_AnaerobicTissues.find(tissue->GetName()) == std::string::npos && tissueNeededEnergy_kcal != 0) //for tracking only
          m_AnaerobicTissues.append(std::string{ tissue->GetName() } + " ");
        tissueNeededEnergy_kcal += muscleMandatoryAnaerobicNeededEnergy_kcal; //add the still-needed mandatory anaerobic energy back to muscle's needed energy for tracking of the deficit
      }
    }
    //

    //Balance everything
    TissueO2->Balance(BalanceLiquidBy::Mass);
    TissueCO2->Balance(BalanceLiquidBy::Mass);
    TissueGlucose->Balance(BalanceLiquidBy::Mass);
    TissueTAG->Balance(BalanceLiquidBy::Mass);
    TissueLactate->Balance(BalanceLiquidBy::Mass);
    TissueKetones->Balance(BalanceLiquidBy::Mass);
    TissueAA->Balance(BalanceLiquidBy::Mass);
    TissueUrea->Balance(BalanceLiquidBy::Mass);
    intracellular.GetSubstanceQuantity(*m_Creatinine)->Balance(BalanceLiquidBy::Mass);

    //If none of this met our energy needs, we have a deficit (fatigue)
    nonbrainEnergyDeficit_kcal += (tissueNeededEnergy_kcal > 0 ? tissueNeededEnergy_kcal : 0);

  } //end of the tissue loop

  //Update outputs
  totalO2Consumed_mol += m_hepaticO2Consumed_mol;
  totalCO2Produced_mol += m_hepaticCO2Produced_mol;
  oxygenConsumptionRate_g_Per_s = totalO2Consumed_mol * m_O2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / time_s;
  carbonDioxideProductionRate_g_Per_s = totalCO2Produced_mol * m_CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / time_s;
  double O2Consumption_mL_Per_s = (oxygenConsumptionRate_g_Per_s / m_O2->GetDensity(MassPerVolumeUnit::g_Per_mL));
  double CO2Production_mL_Per_s = (carbonDioxideProductionRate_g_Per_s / m_CO2->GetDensity(MassPerVolumeUnit::g_Per_mL));
  respiratoryQuotient = totalCO2Produced_mol / totalO2Consumed_mol;

  m_O2ConsumedRunningAverage_mL_Per_s.Sample(O2Consumption_mL_Per_s);
  m_CO2ProducedRunningAverage_mL_Per_s.Sample(CO2Production_mL_Per_s);
  m_RespiratoryQuotientRunningAverage.Sample(respiratoryQuotient);

  //Only record data every 50 steps for these to iron out noise
  int steps = static_cast<int>(m_data.GetSimulationTime().GetValue(TimeUnit::s) / time_s);
  if (steps % 50 == 0) {
    GetOxygenConsumptionRate().SetValue(m_O2ConsumedRunningAverage_mL_Per_s.Value(), VolumePerTimeUnit::mL_Per_s);
    GetCarbonDioxideProductionRate().SetValue(m_CO2ProducedRunningAverage_mL_Per_s.Value(), VolumePerTimeUnit::mL_Per_s);
    GetRespiratoryExchangeRatio().SetValue(m_RespiratoryQuotientRunningAverage.Value());

    m_O2ConsumedRunningAverage_mL_Per_s.Reset();
    m_CO2ProducedRunningAverage_mL_Per_s.Reset();
    m_RespiratoryQuotientRunningAverage.Reset();
  }

  //GetOxygenConsumptionRate().SetValue(O2Consumption_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  //GetCarbonDioxideProductionRate().SetValue(CO2Production_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  //GetRespiratoryExchangeRatio().SetValue(respiratoryQuotient);
  m_energy->GetLactateProductionRate().SetValue(lactateProductionRate_mol_Per_s, AmountPerTimeUnit::mol_Per_s);
  achievedWorkRate_W = (1 / m_Dt_s) * 3600 * 24 * (exerciseEnergyRequested_kcal - brainEnergyDeficit_kcal - nonbrainEnergyDeficit_kcal) / kcal_Per_day_Per_Watt;

  if (m_PatientActions->HasExercise()) {
    m_energy->GetTotalWorkRateLevel().SetValue(achievedWorkRate_W / maxWorkRate_W);
    double intensity = m_PatientActions->GetExercise()->GetIntensity().GetValue();
    if (intensity > 1e-6) { //approx. zero 
      m_energy->GetAchievedExerciseLevel().SetValue(achievedWorkRate_W / maxWorkRate_W / intensity);
    } else {
          m_energy->GetAchievedExerciseLevel().SetValue(0.0);
    }
    } else {
    m_energy->GetAchievedExerciseLevel().SetValue(0.0);
    m_energy->GetTotalWorkRateLevel().SetValue(0.0);
  }

  double fatigue = ((brainEnergyDeficit_kcal + nonbrainEnergyDeficit_kcal) / (baseEnergyRequested_kcal + exerciseEnergyRequested_kcal));

  //sample to set so that it doesn't log like crazy
  fatigue = m_FatigueRunningAverage.Sample(fatigue);

  /// \event Patient: Fatigue - Energy stores are sub-maximal, skip if beginning of cardiac cycle.
  if (!m_Patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
    if (fatigue > 0.0035) {
      m_Patient->SetEvent(CDM::enumPatientEvent::Fatigue, true, m_data.GetSimulationTime());
    } else if (fatigue < 0.000001) {
      m_Patient->SetEvent(CDM::enumPatientEvent::Fatigue, false, m_data.GetSimulationTime());
    }
  }

  m_energy->GetFatigueLevel().SetValue(fatigue);

  //Update other patient parameters
  //\todo: make sure body weight decreases when CO2 is exhaled
  m_Patient->GetBodyFatFraction().SetValue(GetStoredFat().GetValue(MassUnit::g) / m_Patient->GetWeight().GetValue(MassUnit::g));
  m_Patient->GetLeanBodyMass().SetValue(m_Patient->GetWeight().GetValue(MassUnit::g) - GetStoredFat().GetValue(MassUnit::g), MassUnit::g);
  double SiriBodyDensity_g_Per_cm3 = 4.95 / (m_Patient->GetBodyFatFraction().GetValue() + 4.50);
  double BrozekBodyDensity_g_Per_cm3 = 4.57 / (m_Patient->GetBodyFatFraction().GetValue() + 4.142);
  double bodyDensity_g_Per_cm3 = (SiriBodyDensity_g_Per_cm3 + BrozekBodyDensity_g_Per_cm3) / 2.0;
  m_Patient->GetBodyDensity().SetValue(bodyDensity_g_Per_cm3, MassPerVolumeUnit::g_Per_cm3); //See BioGears::SetUpPatient()

  //Reset O2/CO2 member variables since they're static
  m_hepaticCO2Produced_mol = 0;
  m_hepaticO2Consumed_mol = 0;

  //reset average at beginning of cardiac cycle:
  if (m_Patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
    m_FatigueRunningAverage.Reset();
    m_FatigueRunningAverage.Reset();
  }
  //Useful debugging information
  //m_data.GetDataTrack().Probe("InstantaneousBrainEnergyDeficit_kcal", brainEnergyDeficit_kcal);
  //m_data.GetDataTrack().Probe("InstantaneousNonBrainEnergyDeficit_kcal", nonbrainEnergyDeficit_kcal);
  //m_data.GetDataTrack().Probe("NonBrainDeficitFraction", nonbrainEnergyDeficit_kcal / (.8*baseEnergyRequested_kcal + exerciseEnergyRequested_kcal));
  //m_data.GetDataTrack().Probe("InstantaneousMetabolicHeatGenerated_kcal", heatGenerated_kcal);
  //m_data.GetDataTrack().Probe("CumulativeTAGConsumed_g", totalFatConsumed_g);

  //Gives you an idea of what non-muscle compartments don't get enough O2 flow and is better than using Info every timestep
  //Can remove once we get everything tuned adequately
  /*
  int steps = m_data.GetSimulationTime().GetValue(TimeUnit::s)/time_s;
  if (steps % 5000 == 0 && m_AnaerobicTissues.length() > 1)
  {
    Info(m_AnaerobicTissues + "used anaerobic energy in the last 100 seconds.");
    m_AnaerobicTissues.clear();
  }
  */
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Regulates blood amino acid levels by storing/releasing from muscle tissue.
///
/// \details
/// The blood amino acid level is regulated based on the relative changes in insulin and glucagon. A certain amount of
/// amino acids are stored in the tissues as protein. When insulin dominates, amino acids are pushed to the muscle tissue
/// for storage. We don't model their transformation into combined protein; we just use muscle as a storage tank. When
/// glucagon dominates, amino acids are released from storage into the blood for use in gluconeogenesis.
//--------------------------------------------------------------------------------------------------
void Tissue::ProteinStorageAndRelease()
{

  double aminoAcidsBaseline_mg_Per_dL = 50; // \todo make this a CDM set point like glucose?

  //Patients with diabetes type 2 show increased blood AA; allow for up to 30 mg/dL elevation in severe cases \cite guyton2006medical p989
  if (m_data.GetConditions().HasDiabetesType2()) {
    if (m_data.GetConditions().GetDiabetesType2()->HasInsulinResistanceSeverity()) {
      double increasedAA_mg_Per_dL = GeneralMath::LinearInterpolator(0, 1, 0, 30, m_data.GetConditions().GetDiabetesType2()->GetInsulinResistanceSeverity().GetValue());
      aminoAcidsBaseline_mg_Per_dL += increasedAA_mg_Per_dL;
    }
  }

  double hormoneFactor = Hepatic::CalculateRelativeHormoneChange(GetMuscleInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L) * 1e9, GetMuscleGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL) * 1e9, m_MuscleInsulin, m_MuscleGlucagon, m_data);

  //Guyton says protein blood concentrations should only rise 2-3 mg/dL even after eating because of absorption into tissues
  double proteinStorageLowerRate_g_Per_s = .3;
  double proteinStorageUpperRate_g_Per_s = .6;

  //https://www.wolframalpha.com/input/?i=y%3D.3%2B.3%2F(1%2Be%5E(-6(x-1)))+from+0%3Cy%3C.6+and+0%3Cx%3C2
  double proteinStorageRate_g_Per_s = proteinStorageLowerRate_g_Per_s + GeneralMath::LogisticFunction(proteinStorageUpperRate_g_Per_s - proteinStorageLowerRate_g_Per_s, 1, 6, hormoneFactor);

  //Assume protein breakdown rates are similar to storage rates, maybe a bit slower; could be adjusted
  double proteinBreakdownLowerRate_g_Per_s = .1;
  double proteinBreakdownUpperRate_g_Per_s = .2;

  //https://www.wolframalpha.com/input/?i=y%3D.1%2B.1%2F(1%2Be%5E(-15(x-.75)))+from+0%3Cy%3C.2+and+0%3Cx%3C2
  double proteinBreakdownRate_g_Per_s = proteinBreakdownLowerRate_g_Per_s + GeneralMath::LogisticFunction(proteinBreakdownUpperRate_g_Per_s - proteinBreakdownLowerRate_g_Per_s, .75, 15, -hormoneFactor);

  //Patients with COPD experience high protein turnover, so up the breakdown rate by a percentage \cite engelen2000enhanced
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease()) {
    proteinBreakdownRate_g_Per_s *= 1.13; //13% increase
  }

  //remove excess amino acids from blood and store in muscle while there's room
  //Body mobilizes protein when glucagon dominates, but we'll have protein shift toward storage unless insulin drops significantly
  if (hormoneFactor >= -.2 && GetStoredProtein(MassUnit::g) < m_maxProteinStorage_g && m_MuscleAA->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_dL) > aminoAcidsBaseline_mg_Per_dL) {
    if (m_MuscleAA->GetMass(MassUnit::g) < proteinStorageRate_g_Per_s * m_Dt_s) {
      m_ss << "Not enough amino acids in blood to store!";
      Info(m_ss);
    } else {
      m_MuscleAA->GetMass().IncrementValue(-proteinStorageRate_g_Per_s * m_Dt_s, MassUnit::g);
      GetStoredProtein().IncrementValue(proteinStorageRate_g_Per_s * m_Dt_s, MassUnit::g);
      m_MuscleAA->Balance(BalanceLiquidBy::Mass);
    }

    //If we rise above 10% of protein storage, turn off the Muscle Catabolism event
    if (m_Patient->IsEventActive(CDM::enumPatientEvent::MuscleCatabolism) && GetStoredProtein(MassUnit::g) > .1 * m_maxProteinStorage_g) {
      m_Patient->SetEvent(CDM::enumPatientEvent::MuscleCatabolism, false, m_data.GetSimulationTime());
    }
  }

  //Remove amino acids from protein stores (or muscle if that's depleted) and dump into blood
  //We don't dump AA into blood if blood concentration is already at basal level
  else if (hormoneFactor < 0 && m_MuscleAA->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_dL) < aminoAcidsBaseline_mg_Per_dL) {
    //Muscle catabolism
    if (GetStoredProtein(MassUnit::g) < proteinBreakdownRate_g_Per_s * m_Dt_s) {
      if (!m_Patient->IsEventActive(CDM::enumPatientEvent::MuscleCatabolism)) {
        m_Patient->SetEvent(CDM::enumPatientEvent::MuscleCatabolism, true, m_data.GetSimulationTime());
      }
      if (m_Patient->GetMuscleMass(MassUnit::g) < proteinBreakdownRate_g_Per_s * m_Dt_s) {
        m_ss << "The patient has consumed all of their body's amino acids. They've effectively starved.";
        Warning(m_ss);
        if (!m_PatientActions->HasOverride()) {
          m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        } else {
          if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
            m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
          }
        }
      } else {
        m_MuscleAA->GetMass().IncrementValue(proteinBreakdownRate_g_Per_s * m_Dt_s, MassUnit::g);
        m_Patient->GetMuscleMass().IncrementValue(-proteinBreakdownRate_g_Per_s * m_Dt_s, MassUnit::g);
        m_MuscleAA->Balance(BalanceLiquidBy::Mass);
      }
    }
    //Temporary protein storage used
    else {
      m_MuscleAA->GetMass().IncrementValue(proteinBreakdownRate_g_Per_s * m_Dt_s, MassUnit::g);
      GetStoredProtein().IncrementValue(-proteinBreakdownRate_g_Per_s * m_Dt_s, MassUnit::g);
      m_MuscleAA->Balance(BalanceLiquidBy::Mass);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Establishes the anemia condition by removing hemoglobin and reducing blood viscosity (simulated by vascular resistance reduction).
///
/// \details
/// We are modeling iron deficiency anemia as a chronic reduction in hemoglobin in the blood as well as
/// a reduction in the cardiovascular resistances to lower the systemic vascular resistance that is
/// seen with the reduced viscosity. The oxygen carrying capacity of the blood is reduced due to the
/// decrease in hemoglobin content.
//--------------------------------------------------------------------------------------------------
void Tissue::Dehydrate()
{
  SEPatient& Patient = m_data.GetPatient();
  SEDehydration* dehydration = m_data.GetConditions().GetDehydration();
  SEScalarMassPerVolume density;
  GeneralMath::CalculateWaterDensity(m_data.GetEnergy().GetCoreTemperature(), density);

  //dehydration determine patient weight loss due to water deficiency
  double fractionalWeightLoss = dehydration->GetDehydrationFraction().GetValue();

  //Set tissue value on CDM
  GetDehydrationFraction().SetValue(fractionalWeightLoss);

  double patientMass_kg = m_Patient->GetWeight(MassUnit::kg);

  double waterReduction_L = patientMass_kg * fractionalWeightLoss / density.GetValue(MassPerVolumeUnit::kg_Per_L);

  //from fraction that is water
  double waterReductionFraction = waterReduction_L / m_data.GetTissue().GetTotalBodyFluidVolume(VolumeUnit::L);

  //tracking fluid losses
  double temp_mL = 0.0;
  double totalFluidLoss_mL = 0.0;

  //loop over extracellular fluid compartments and decrement
  for (SETissueCompartment* tissue : m_data.GetCompartments().GetTissueLeafCompartments()) {
    SELiquidCompartment& extracellularFluid = m_data.GetCompartments().GetExtracellularFluid(*tissue); //get the compartment
    temp_mL = extracellularFluid.GetVolume().GetValue(VolumeUnit::mL); //pull the volume
    totalFluidLoss_mL += temp_mL * waterReductionFraction;
    extracellularFluid.GetVolume().SetValue(temp_mL * (1 - waterReductionFraction), VolumeUnit::mL); //set the total volume
    extracellularFluid.Balance(BalanceLiquidBy::Mass);

    SELiquidCompartment& intracellularFluid = m_data.GetCompartments().GetIntracellularFluid(*tissue); //get the compartment
    temp_mL = intracellularFluid.GetVolume().GetValue(VolumeUnit::mL); //pull the volume
    totalFluidLoss_mL += temp_mL * waterReductionFraction;
    intracellularFluid.GetVolume().SetValue(temp_mL * (1 - waterReductionFraction), VolumeUnit::mL); //set new volume
    intracellularFluid.Balance(BalanceLiquidBy::Mass);
  }
  double blood_mL = 0.0;
  for (SELiquidCompartment* cmpt : m_data.GetCompartments().GetVascularLeafCompartments()) {
    if (!cmpt->HasVolume())
      continue;
    temp_mL = cmpt->GetVolume(VolumeUnit::mL); //pull the volume
    totalFluidLoss_mL += temp_mL * waterReductionFraction;
    blood_mL += temp_mL;
    cmpt->GetVolume().SetValue(temp_mL * (1 - waterReductionFraction), VolumeUnit::mL);
    cmpt->Balance(BalanceLiquidBy::Mass);
  }

  //set patient weight
  double bloodDensity_kg_Per_mL = m_data.GetBloodChemistry().GetBloodDensity(MassPerVolumeUnit::kg_Per_mL);
  double bodyWeightLost_kg = bloodDensity_kg_Per_mL * totalFluidLoss_mL;

  patientMass_kg -= bodyWeightLost_kg;
  m_Patient->GetWeight().SetValue(patientMass_kg, MassUnit::kg);

  //need to set blood volume here
  m_data.GetCardiovascular().GetBloodVolume().SetValue(blood_mL, VolumeUnit::mL);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Regulates blood triacylglyceride levels by storing/releasing in fat tissue.
///
/// \details
/// The blood triacylglyceride level is regulated based on the relative changes in insulin and glucagon.
/// Fat is stored in the adipocytes of the fat tissue. When insulin dominates, triacylglycerides are pushed to the fat tissue
/// for storage. When glucagon dominates, triacylglycerol is released from storage into the blood for use in metabolism.
//--------------------------------------------------------------------------------------------------
void Tissue::FatStorageAndRelease()
{
  double triacylglycerolBaseline_mg_Per_dL = 75; // \todo make this a CDM set point like glucose?

  double hormoneFactor = Hepatic::CalculateRelativeHormoneChange(GetFatInsulinSetPoint().GetValue(AmountPerVolumeUnit::mmol_Per_L) * 1e9, GetFatGlucagonSetPoint().GetValue(MassPerVolumeUnit::mg_Per_mL) * 1e9, m_FatInsulin, m_FatGlucagon, m_data);

  //Guyton says triglycerides in chylomicrons are clear from blood in "a few hours" with chylomicrons having half-life <1 hour
  //Other sources show triglycerides returning to normal in ~4 hours after peak
  //These values currently allow 75g of fat to be absorbed into bloodstream in ~4 hours, with all being stored in ~6 hours
  //Peak triglyceride levels are about 5x normal, maybe too high
  double fatStorageLowerRate_g_Per_s = .004;
  double fatStorageUpperRate_g_Per_s = .012;

  //https://www.wolframalpha.com/input/?i=y%3D.4%2B.8%2F(1%2Be%5E(-6(x-.5)))+from+0%3Cy%3C1.2+and+0%3Cx%3C2
  double fatStorageRate_g_Per_s = fatStorageLowerRate_g_Per_s + GeneralMath::LogisticFunction(fatStorageUpperRate_g_Per_s - fatStorageLowerRate_g_Per_s, 1, 6, hormoneFactor);

  //Assume fat release rates are similar to glycogen for now... but this could be revisited
  double fatReleaseLowerRate_g_Per_s = .002546; //maxStoredProtein_g/ 12 * 3600
  double fatReleaseUpperRate_g_Per_s = .061111; //maxStoredProtein_g/ .5 * 3600

  //https://www.wolframalpha.com/input/?i=y%3D.002546%2B.058565%2F(1%2Be%5E(-15(x-.75)))+from+0%3Cy%3C.08+and+0%3Cx%3C2
  double fatReleaseRate_g_Per_s = fatReleaseLowerRate_g_Per_s + GeneralMath::LogisticFunction(fatReleaseUpperRate_g_Per_s - fatReleaseLowerRate_g_Per_s, .75, 15, -hormoneFactor);

  //remove excess triacylglycerol from blood and store in fat tissue
  if (hormoneFactor >= -.2 && m_FatTAG->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_dL) > triacylglycerolBaseline_mg_Per_dL) {
    if (m_FatTAG->GetMass(MassUnit::g) < fatStorageRate_g_Per_s * m_Dt_s) {
      m_ss << "Not enough triacylglycerol in blood to store!";
      Info(m_ss);
    } else {
      m_FatTAG->GetMass().IncrementValue(-fatStorageRate_g_Per_s * m_Dt_s, MassUnit::g);
      GetStoredFat().IncrementValue(fatStorageRate_g_Per_s * m_Dt_s, MassUnit::g);
      m_FatTAG->Balance(BalanceLiquidBy::Mass);
    }
  }

  //remove triacylglycerol from fat stores and dump into blood
  //Guyton says during starvation (i.e. glucagon dominant times) triacylglycerol blood concentration can be 5-8x normal
  //We won't continue to dump TAG into blood if blood concentration is already high, but we will try to maintain base level
  else if ((hormoneFactor < 0 && m_FatTAG->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_dL) < triacylglycerolBaseline_mg_Per_dL * 6.5) || m_FatTAG->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_dL) < triacylglycerolBaseline_mg_Per_dL) {
    if (GetStoredFat(MassUnit::g) < fatReleaseRate_g_Per_s * m_Dt_s) {
      m_ss << "The patient has consumed all of their body's fat. They've effectively starved.";
      Warning(m_ss);
      if (!m_PatientActions->HasOverride()) {
        m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
      } else {
        if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
          m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        }
      }
    } else {
      m_FatTAG->GetMass().IncrementValue(fatReleaseRate_g_Per_s * m_Dt_s, MassUnit::g);
      GetStoredFat().IncrementValue(-fatReleaseRate_g_Per_s * m_Dt_s, MassUnit::g);
      m_FatTAG->Balance(BalanceLiquidBy::Mass);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Computes and sets patient system level data.
///
///
/// \param time
/// The time step
///
/// \details
/// All of the extravascular fluid, including extravascular-intracellular and extravascular-extracellular,
/// is computed for total volume accounting.
//--------------------------------------------------------------------------------------------------
void Tissue::CalculateVitals()
{
  // Hydration Status
  double currentFluidMass_kg = 0.0;
  SETissueCompartment* tissue;
  SELiquidCompartment* vascular;
  SEScalarMassPerVolume density;
  GeneralMath::CalculateWaterDensity(m_data.GetEnergy().GetCoreTemperature(), density);
  for (auto tissueVascular : m_TissueToVascular) {
    tissue = tissueVascular.first;
    vascular = tissueVascular.second;
    currentFluidMass_kg += vascular->GetVolume(VolumeUnit::mL) * m_data.GetBloodChemistry().GetBloodDensity(MassPerVolumeUnit::kg_Per_mL);
    currentFluidMass_kg += m_data.GetCompartments().GetIntracellularFluid(*tissue).GetVolume(VolumeUnit::mL) * density.GetValue(MassPerVolumeUnit::kg_Per_mL);
    currentFluidMass_kg += m_data.GetCompartments().GetExtracellularFluid(*tissue).GetVolume(VolumeUnit::mL) * density.GetValue(MassPerVolumeUnit::kg_Per_mL);
  }
  double dehydrationFraction = (m_RestingFluidMass_kg - currentFluidMass_kg) / m_RestingPatientMass_kg;
  BLIM(dehydrationFraction, 0.0, 1.0);

  GetDehydrationFraction().SetValue(dehydrationFraction);
  if (dehydrationFraction > 0.03) {
    /// \event Patient: Patient is dehydrated when 3% of body mass is lost due to fluid reduction
    m_data.GetPatient().SetEvent(CDM::enumPatientEvent::Dehydration, true, m_data.GetSimulationTime()); /// \cite who2005dehydration
  } else if (dehydrationFraction < 0.01) {
    m_data.GetPatient().SetEvent(CDM::enumPatientEvent::Dehydration, false, m_data.GetSimulationTime());
  }

  ///\TODO: check for patient thirst, when plasma osmolarity increases 3 percent \cite cheuvront2014dehydration

  // Total tissue/body fluid volume
  double ecVol_mL = 0.;
  double icvol_mL = 0.;
  double bloodvol_mL = 0.0;
  for (auto itr : m_data.GetCompartments().GetExtracellularFluid()) {
    ecVol_mL += itr.second->GetVolume(VolumeUnit::mL);
  }
  for (auto itr : m_data.GetCompartments().GetIntracellularFluid()) {
    icvol_mL += itr.second->GetVolume(VolumeUnit::mL);
  }
  GetExtracellularFluidVolume().SetValue(ecVol_mL, VolumeUnit::mL);
  GetIntracellularFluidVolume().SetValue(icvol_mL, VolumeUnit::mL);
  GetExtravascularFluidVolume().SetValue(ecVol_mL + icvol_mL, VolumeUnit::mL);
  bloodvol_mL = m_data.GetCardiovascular().GetBloodVolume(VolumeUnit::mL);
  GetTotalBodyFluidVolume().SetValue(ecVol_mL + icvol_mL + bloodvol_mL, VolumeUnit::mL);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Checks glycogen levels and throws events if necessary
///
/// \details
/// Throws events for depleted glycogen levels in the liver and/or muscle
//--------------------------------------------------------------------------------------------------
void Tissue::CheckGlycogenLevels()
{
  //liver glycogen
  if (GetLiverGlycogen().GetValue(MassUnit::g) < 1) {
    m_Patient->SetEvent(CDM::enumPatientEvent::LiverGlycogenDepleted, true, m_data.GetSimulationTime());
  } else if (m_Patient->IsEventActive(CDM::enumPatientEvent::LiverGlycogenDepleted) && GetLiverGlycogen().GetValue(MassUnit::g) > 10) {
    m_Patient->SetEvent(CDM::enumPatientEvent::LiverGlycogenDepleted, false, m_data.GetSimulationTime());
  }

  //muscle glycogen
  if (GetMuscleGlycogen().GetValue(MassUnit::g) < 1) {
    m_Patient->SetEvent(CDM::enumPatientEvent::MuscleGlycogenDepleted, true, m_data.GetSimulationTime());
  } else if (m_Patient->IsEventActive(CDM::enumPatientEvent::MuscleGlycogenDepleted) && GetMuscleGlycogen().GetValue(MassUnit::g) > 10) {
    m_Patient->SetEvent(CDM::enumPatientEvent::MuscleGlycogenDepleted, false, m_data.GetSimulationTime());
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets body state with regard to some substances and manages blood gas distribution
///
/// \details
/// Until substance fidelity is improved, some values for blood pH are hardcoded. This funciton
/// also manages the calls to blood gas distribution so it doesn't clutter up Process().
//--------------------------------------------------------------------------------------------------
void Tissue::ManageSubstancesAndSaturation()
{
  SEScalarMassPerVolume albuminConcentration;
  albuminConcentration = m_data.GetSubstances().GetAlbumin().GetBloodConcentration();
  // Currently, substances are not where they need to be, we will hard code this for now until we fix them
  /// \todo Remove SetBodyState hardcode and replace with computed values after substance redux is complete
  m_data.GetSaturationCalculator().SetBodyState(albuminConcentration,
                                                m_data.GetBloodChemistry().GetHematocrit(),
                                                m_data.GetEnergy().GetCoreTemperature(),
                                                m_data.GetBloodChemistry().GetStrongIonDifference(),
                                                m_data.GetBloodChemistry().GetPhosphate());
  for (SELiquidCompartment* cmpt : m_data.GetCompartments().GetVascularLeafCompartments()) {
    if (cmpt->HasVolume()) {
      m_data.GetSaturationCalculator().CalculateBloodGasDistribution(*cmpt);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Distributes mass to child compartments by volume-weighted distribution
///
/// \param  cmpt  Parent compartment
/// \param  sub   Substance being distributed.
/// \param  mass  mass of substance to distribute.
/// \param  unit  unit of mass.
///
/// \details
/// This method is intended to be used to distribute a mass INCREMENT amongst child compartments
/// for transport between parent compartments, but it can be used for a decrement and for transport
/// between compartments that do not have children.
//--------------------------------------------------------------------------------------------------
void Tissue::DistributeMassbyVolumeWeighted(SELiquidCompartment& cmpt, const SESubstance& sub, double mass, const MassUnit& unit)
{
  SELiquidSubstanceQuantity* subQ = cmpt.GetSubstanceQuantity(sub);
  if (mass < 0.0) {
    if (-mass > subQ->GetMass(unit)) {
      mass = -subQ->GetMass(unit);
      Info("The amount of mass decrement to distribute by volume weighted was greater than available. High probability of negative mass. DistributeMassbyMassWeighted is preferred for decrements.");
    }
  }

  if (!cmpt.HasChildren()) {
    subQ->GetMass().IncrementValue(mass, unit);
    if (std::abs(subQ->GetMass(MassUnit::ug)) < ZERO_APPROX) {
      subQ->GetMass().SetValue(0.0, MassUnit::ug);
    }
  } else {
    double volume_mL = cmpt.GetVolume(VolumeUnit::mL);
    for (SELiquidCompartment* leaf : cmpt.GetLeaves()) {
      double leafMass = mass * (leaf->GetVolume(VolumeUnit::mL) / volume_mL);
      SELiquidSubstanceQuantity* subQ = leaf->GetSubstanceQuantity(sub);
      subQ->GetMass().IncrementValue(leafMass, unit);
      if (std::abs(subQ->GetMass(MassUnit::ug)) < ZERO_APPROX) {
        subQ->GetMass().SetValue(0.0, MassUnit::ug);
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Distributes mass to child compartments by mass-weighted distribution
///
/// \param  cmpt  Parent compartment
/// \param  sub   Substance being distributed.
/// \param  mass  mass of substance to distribute.
/// \param  unit  unit of mass.
///
/// \details
/// This method is intended to be used to distribute a mass DECREMENT amongst child compartments
/// for transport between parent compartments, but it can be used for a increment and for transport
/// between compartments that do not have children.
//--------------------------------------------------------------------------------------------------
void Tissue::DistributeMassbyMassWeighted(SELiquidCompartment& cmpt, const SESubstance& sub, double mass, const MassUnit& unit)
{
  SELiquidSubstanceQuantity* subQ = cmpt.GetSubstanceQuantity(sub);
  if (mass < 0.0) {
    mass = -mass > subQ->GetMass(unit) ? -subQ->GetMass(unit) : mass;
  }

  if (!cmpt.HasChildren()) {
    subQ->GetMass().IncrementValue(mass, unit);
    if (std::abs(subQ->GetMass(MassUnit::ug)) < ZERO_APPROX) {
      subQ->GetMass().SetValue(0.0, MassUnit::ug);
    }
  } else {
    double mass_ug = subQ->GetMass(MassUnit::ug);
    for (SELiquidCompartment* leaf : cmpt.GetLeaves()) {
      SELiquidSubstanceQuantity* subQ = leaf->GetSubstanceQuantity(sub);
      double leafMass = 0.0;
      if (mass_ug != 0.0) {
        leafMass = mass * (subQ->GetMass(MassUnit::ug) / mass_ug);
      }
      subQ->GetMass().IncrementValue(leafMass, unit);
      if (std::abs(subQ->GetMass(MassUnit::ug)) < ZERO_APPROX) {
        subQ->GetMass().SetValue(0.0, MassUnit::ug);
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the mass transfer between tissue and vascular based on perfusion limited diffusion
///
/// \param  acmpt  compartment for diffusion.
/// \param  sub Substance being transferred.
/// \param  fx  compartment effects for the given substance.
/// \param  timestep Time step of the model.
///
/// \details
/// Uses the input compartment to find the corresponding vascular and tissue flow and concentration values to calculate diffusion.
/// MassDiffused = VascularFlow * dT * (VascularConcentration - TissueConcentration) / PartitionCoefficient
/// Where TissueConcentration is the Intracelluar Substance Mass / Tissue Matrix Volume
/// The mass is then updated on the vascular and tissue components of the compartment.
/// Concentration will be automatically recalculated when the compartment is refreshed.
//--------------------------------------------------------------------------------------------------
double Tissue::PerfusionLimitedDiffusion(SETissueCompartment& tissue, SELiquidCompartment& vascular, const SESubstance& sub, double partitionCoeff, double timestep_s)
{
  //Put an if statement here to check if tissue volume is nullptr, if so continue.

  SELiquidCompartment& intracellular = m_data.GetCompartments().GetIntracellularFluid(tissue);

  //Calculate Diffusion
  SELiquidSubstanceQuantity* vSubQ = vascular.GetSubstanceQuantity(sub);
  if (vSubQ == nullptr)
    throw CommonDataModelException("No Vascular Substance Quantity found for substance " + std::string{ sub.GetName() });
  double VascularFlow_m_LPer_s = vascular.GetInFlow(VolumePerTimeUnit::mL_Per_s);
  double VascularConcentration_ug_Per_mL = vSubQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);

  SELiquidSubstanceQuantity* tSubQ = intracellular.GetSubstanceQuantity(sub);
  if (tSubQ == nullptr)
    throw CommonDataModelException("No Tissue-Intracellular Substance Quantity found for substance " + std::string{ sub.GetName() });
  SEScalarMassPerVolume tissueConcentration;
  GeneralMath::CalculateConcentration(tSubQ->GetMass(), tissue.GetMatrixVolume(), tissueConcentration, m_Logger);
  double TissueConcentration_ug_Per_mL = tissueConcentration.GetValue(MassPerVolumeUnit::ug_Per_mL);
  double MassIncrement_ug = 0;
  if (!(partitionCoeff == 0)) {
    MassIncrement_ug = VascularFlow_m_LPer_s * timestep_s * (VascularConcentration_ug_Per_mL - (TissueConcentration_ug_Per_mL / partitionCoeff));
  } else {
    MassIncrement_ug = 0;
  }

  //Update the mass and concentration on the nodes
  if (MassIncrement_ug != 0) {
    // If it's going in, distribute by volume
    // If it's coming out, distribute by mass
    // If mass is exactly zero then going in by mass weighted won't work.
    if (MassIncrement_ug > 0.) {
      if (MassIncrement_ug > vSubQ->GetMass(MassUnit::ug))
        MassIncrement_ug = vSubQ->GetMass(MassUnit::ug);
      DistributeMassbyMassWeighted(vascular, sub, -MassIncrement_ug, MassUnit::ug);
      DistributeMassbyVolumeWeighted(intracellular, sub, MassIncrement_ug, MassUnit::ug);
    } else {
      if (-MassIncrement_ug > tSubQ->GetMass(MassUnit::ug))
        MassIncrement_ug = -tSubQ->GetMass(MassUnit::ug);
      DistributeMassbyVolumeWeighted(vascular, sub, -MassIncrement_ug, MassUnit::ug);
      DistributeMassbyMassWeighted(intracellular, sub, MassIncrement_ug, MassUnit::ug);
    }

    vSubQ->Balance(BalanceLiquidBy::Mass);
    tSubQ->Balance(BalanceLiquidBy::Mass);
  }
  return MassIncrement_ug;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the mass transfer due to a partial pressure gradient
///
/// \param  Source pulmonary compartment for the mass transfer.
/// \param  Target vascular compartment for the mass transfer.
/// \param  sub Substance being transferred.
/// \param  DiffusingCapacityO2_mL_Per_s_mmHg the current diffusion capacity of oxygen for this alveolar exchange region.
/// \param  timestep Time step of the engine.
///
/// \details
/// Calculates the mass transfer from a pulmonary compartment/fluid to a vascular compartment/fluid
/// based on the partial pressure gradient of the gas in the two compartments/fluids.
///
/// We cannot balance gas compartment substance quantities on a per substance basis like we can a liquid
/// When the gas substance volume changes, the compartment volume changes as well,
/// which would then change the volume fraction for ALL the substances in the compartment.
/// So it is up to the end user to call balance on the compartment when they are done updating substances on it.
/// It would be inefficient to balance the compartment in this method, as this diffusion could be in a loop over all
/// active substances, and we would be looping over all substance repeatedly.
/// The alveolar exchange of any gas is computed from diffusion capacities relative to oxygen.
//--------------------------------------------------------------------------------------------------
void Tissue::AlveolarPartialPressureGradientDiffusion(SEGasCompartment& pulmonary, SELiquidCompartment& vascular, SESubstance& sub, double DiffusingCapacityO2_mL_Per_s_mmHg, double timestep_s)
{
  SEGasSubstanceQuantity* pSubQ = pulmonary.GetSubstanceQuantity(sub);
  SELiquidSubstanceQuantity* vSubQ = vascular.GetSubstanceQuantity(sub);
  if (pSubQ == nullptr || vSubQ == nullptr)
    throw CommonDataModelException("No Substance Quantity found for substance " + std::string{ sub.GetName() });

  double PressureGradient_mmHg = pSubQ->GetPartialPressure(PressureUnit::mmHg) - vSubQ->GetPartialPressure(PressureUnit::mmHg);

  double DiffusedVolume_mL = PressureGradient_mmHg * DiffusingCapacityO2_mL_Per_s_mmHg * sub.GetRelativeDiffusionCoefficient().GetValue() * timestep_s;

  //Get special diffused volume if sub is CO
  if (PressureGradient_mmHg > 0 && &sub == m_CO) // Wants to come into the blood
  {
    DiffusedVolume_mL = PressureGradient_mmHg * DiffusingCapacityO2_mL_Per_s_mmHg * sub.GetRelativeDiffusionCoefficient().GetValue() * (1 / (5.404e-05 * vascular.GetSubstanceQuantity(*m_O2)->GetPartialPressure(PressureUnit::mmHg) + 0.02885)) * timestep_s; //Modify the relative diffusion coefficient
  }
  double DiffusedMass_ug = DiffusedVolume_mL * sub.GetDensity(MassPerVolumeUnit::ug_Per_mL);

  //Check to make sure we're not moving more than exists
  if (DiffusedVolume_mL > 0) {
    //It's moving from pulmonary to vascular
    double pVolume_mL = pSubQ->GetVolume(VolumeUnit::mL);
    if (DiffusedVolume_mL > pVolume_mL) {
      //Move all we can
      DiffusedVolume_mL = pVolume_mL;
      DiffusedMass_ug = DiffusedVolume_mL * sub.GetDensity(MassPerVolumeUnit::ug_Per_mL);
    }
  } else {
    //It's moving from vascular to pulmonary
    double vMass_ug = vSubQ->GetMass(MassUnit::ug);
    if (-DiffusedMass_ug > vMass_ug) {
      //Move all we can
      DiffusedMass_ug = -vMass_ug;
      DiffusedVolume_mL = DiffusedMass_ug / sub.GetDensity(MassPerVolumeUnit::ug_Per_mL);
    }
  }

  pSubQ->GetVolume().IncrementValue(-DiffusedVolume_mL, VolumeUnit::mL);
  sub.GetAlveolarTransfer().IncrementValue(DiffusedVolume_mL / timestep_s, VolumePerTimeUnit::mL_Per_s);
  sub.GetDiffusingCapacity().IncrementValue(DiffusingCapacityO2_mL_Per_s_mmHg * sub.GetRelativeDiffusionCoefficient().GetValue(), VolumePerTimePressureUnit::mL_Per_s_mmHg);

  vSubQ->GetMass().IncrementValue(DiffusedMass_ug, MassUnit::ug);
  if (std::abs(vSubQ->GetMass(MassUnit::ug)) < ZERO_APPROX) {
    vSubQ->GetMass().SetValue(0.0, MassUnit::ug);
  }
  vSubQ->Balance(BalanceLiquidBy::Mass);
}

/// --------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the mass transport of a substance between compartments by simple diffusion and updates other data after the mass increment
///
/// \param source: source compartment
/// \param target: target compartment
/// \param sub: substance that is diffusing
/// \param permeabilityCofficient_mL_Per_s: the area-independent constant of proportionality
/// \param timestep_s: the time step
///
/// \details
/// This method adjusts the mass in the source and target compartments using the concentration
/// gradient based on the principles of simple diffusion. The method does not automatically update the concentration,
/// molarity, partial pressure, and other data in the compartment following the mass increment.
/// ***MUST CALL BALANCE TO UPDATE CONCENTRATION, MOLARITY, ETC.***
/// Note that source and target are used to define a sign convention for readability only. the direction
/// of mass flow is controlled entirely by the concentration gradient and is independent of which compartment
/// is the source and which is the target.
//--------------------------------------------------------------------------------------------------
double Tissue::MoveMassBySimpleDiffusion(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub, double permeabilityCofficient_mL_Per_s, double timestep_s)
{
  const SELiquidSubstanceQuantity* srcQ = source.GetSubstanceQuantity(sub);
  const SELiquidSubstanceQuantity* tgtQ = target.GetSubstanceQuantity(sub);

  double amountIncrement_ug = permeabilityCofficient_mL_Per_s * (srcQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL) - tgtQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL)) * timestep_s;
  // Need to limit the increment to what is available... just in case
  if (amountIncrement_ug > 0.0) {
    if (amountIncrement_ug > srcQ->GetMass(MassUnit::ug)) {
      amountIncrement_ug = srcQ->GetMass(MassUnit::ug);
    }
    DistributeMassbyMassWeighted(source, sub, -amountIncrement_ug, MassUnit::ug);
    DistributeMassbyVolumeWeighted(target, sub, amountIncrement_ug, MassUnit::ug);
  } else // negative increment means it is coming from the target and going to the source
  {
    if (-amountIncrement_ug > tgtQ->GetMass(MassUnit::ug)) {
      amountIncrement_ug = -tgtQ->GetMass(MassUnit::ug);
    }
    DistributeMassbyVolumeWeighted(source, sub, -amountIncrement_ug, MassUnit::ug);
    DistributeMassbyMassWeighted(target, sub, amountIncrement_ug, MassUnit::ug);
  }

  // Note we are only changing mass and NOT balancing on purpose
  // We don't want the concentrations changing until we have completed our
  // diffusion methodology in its entirety

  return amountIncrement_ug;
} // End SimpleMassDiffusion

/// --------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the mass transport of a substance between compartments by instantaneous diffusion
///
/// \param source: source compartment
/// \param target: target compartment
/// \param sub: substance that is diffusing
/// \param timestep_s: the time step
///
/// \details
/// Instantaneous diffusion assumes that the entire diffusion process happens within the bounds of a time step.
//--------------------------------------------------------------------------------------------------
double Tissue::MoveMassByInstantDiffusion(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub, double timestep_s)
{
  const SELiquidSubstanceQuantity* srcQ = source.GetSubstanceQuantity(sub);
  const SELiquidSubstanceQuantity* tgtQ = target.GetSubstanceQuantity(sub);

  double sConc_ug_Per_mL = srcQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
  double tConc_ug_Per_mL = tgtQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
  double sVol_mL = source.GetVolume(VolumeUnit::mL);
  double tVol_mL = target.GetVolume(VolumeUnit::mL);

  double amountIncrement_ug = (sConc_ug_Per_mL - tConc_ug_Per_mL) * (sVol_mL * tVol_mL) / (sVol_mL + tVol_mL);

  if (amountIncrement_ug > 0.0) {
    if (amountIncrement_ug > srcQ->GetMass(MassUnit::ug)) {
      amountIncrement_ug = srcQ->GetMass(MassUnit::ug);
    }
    DistributeMassbyMassWeighted(source, sub, -amountIncrement_ug, MassUnit::ug);
    DistributeMassbyVolumeWeighted(target, sub, amountIncrement_ug, MassUnit::ug);
  } else // negative increment means it is coming from the target and going to the source
  {
    if (-amountIncrement_ug > tgtQ->GetMass(MassUnit::ug)) {
      amountIncrement_ug = -tgtQ->GetMass(MassUnit::ug);
    }
    DistributeMassbyVolumeWeighted(source, sub, -amountIncrement_ug, MassUnit::ug);
    DistributeMassbyMassWeighted(target, sub, amountIncrement_ug, MassUnit::ug);
  }

  // Note we are only changing mass and NOT balancing on purpose
  // We don't want the concentrations changing until we have completed our
  // diffusion methodology in its entirety

  return amountIncrement_ug;
}

/// --------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the mass transport of a substance between compartments by facilitated diffusion
///
/// \param source: source compartment
/// \param target: target compartment
/// \param sub: substance that is diffusing
/// \param combinedCoefficient_g_Per_s: defines the maximum mass rate
/// \param timestep_s: the time step
///
/// This method adjusts the mass in the source and target compartments using the concentration
/// gradient based on the principles of facilitated diffusion. The method does not automatically update the concentration,
/// molarity, partial pressure, and other data in the compartment following the mass increment.
/// ***MUST CALL BALANCE TO UPDATE CONCENTRATION, MOLARITY, ETC.***
/// Note that source and target are used to define a sign convention for readability only. the direction
/// of mass flow is controlled entirely by the concentration gradient and is independent of which compartment
/// is the source and which is the target.
//--------------------------------------------------------------------------------------------------
double Tissue::MoveMassByFacilitatedDiffusion(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub, double combinedCoefficient_g_Per_s, double timestep_s)
{
  const SELiquidSubstanceQuantity* sSubQ = source.GetSubstanceQuantity(sub);
  const SELiquidSubstanceQuantity* tSubQ = target.GetSubstanceQuantity(sub);

  double amountIncrement_g = combinedCoefficient_g_Per_s * (sSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL) - tSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL))
    / (sub.GetMichaelisCoefficient() + (sSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL) - tSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL))) * timestep_s;

  if (amountIncrement_g > 0.0) {
    if (amountIncrement_g > sSubQ->GetMass(MassUnit::g)) {
      amountIncrement_g = sSubQ->GetMass(MassUnit::g);
    }
    DistributeMassbyMassWeighted(source, sub, -amountIncrement_g, MassUnit::g);
    DistributeMassbyVolumeWeighted(target, sub, amountIncrement_g, MassUnit::g);
  } else // negative increment means it is coming from the target and going to the source
  {
    if (-amountIncrement_g > tSubQ->GetMass(MassUnit::g)) {
      amountIncrement_g = -tSubQ->GetMass(MassUnit::g);
    }
    DistributeMassbyVolumeWeighted(source, sub, -amountIncrement_g, MassUnit::g);
    DistributeMassbyMassWeighted(target, sub, amountIncrement_g, MassUnit::g);
  }
  // Note we are only changing mass and NOT balancing on purpose
  // We don't want the concentrations changing until we have completed our
  // diffusion methodology in its entirety

  return amountIncrement_g;
} // End FacilitatedMassDiffusion

double Tissue::MoveMassByConvection(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub, double timestep_s)
{
  const SELiquidSubstanceQuantity* sSubQ = source.GetSubstanceQuantity(sub);
  const SELiquidSubstanceQuantity* tSubQ = target.GetSubstanceQuantity(sub);

  double fluidFlow_mL_Per_s = m_LymphPaths[&source]->GetFlow(VolumePerTimeUnit::mL_Per_s);

  double amountIncrement_ug = fluidFlow_mL_Per_s * sSubQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * timestep_s;

  if (amountIncrement_ug > 0.0) {
    if (amountIncrement_ug > sSubQ->GetMass(MassUnit::ug)) {
      amountIncrement_ug = sSubQ->GetMass(MassUnit::ug);
    }
    DistributeMassbyMassWeighted(source, sub, -amountIncrement_ug, MassUnit::ug);
    DistributeMassbyVolumeWeighted(target, sub, amountIncrement_ug, MassUnit::ug);
  } else // negative increment means it is coming from the target and going to the source
  {
    amountIncrement_ug = 0.0;
  }

  return amountIncrement_ug;
}

/// --------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the mass transport of a substance between compartments by active transport
///
/// \param source: source compartment
/// \param target: target compartment
/// \param sub: substance that is diffusing
/// \param pumpRate_g_Per_s: the rate at which the pump is currently working
/// \param timestep_s: the time step
///
/// \details
/// This is a simplified pumping mechanism. This single mechanism lumps together all of the
/// biological mechanisms which "pump" a substance across a membrane against an electrochemical
/// or other gradient (all energy-requiring transport mechanisms). It computes a mass increment
/// based on a pump rate and then increments the mass. TThe method does not automatically update the concentration,
/// molarity, partial pressure, and other data in the compartment following the mass increment.
/// ***MUST CALL BALANCE TO UPDATE CONCENTRATION, MOLARITY, ETC.***
/// Note that the sign of the increment is determined by the source and target designation.
//--------------------------------------------------------------------------------------------------
double Tissue::MoveMassByActiveTransport(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub, double pumpRate_g_Per_s, double timestep_s)
{
  const SELiquidSubstanceQuantity* srcQ = source.GetSubstanceQuantity(sub);
  const SELiquidSubstanceQuantity* tgtQ = target.GetSubstanceQuantity(sub);

  double amountIncrement_g = pumpRate_g_Per_s * timestep_s;

  if (amountIncrement_g > 0.0) {
    if (amountIncrement_g > srcQ->GetMass(MassUnit::g)) {
      amountIncrement_g = srcQ->GetMass(MassUnit::g);
    }
    DistributeMassbyMassWeighted(source, sub, -amountIncrement_g, MassUnit::g);
    DistributeMassbyVolumeWeighted(target, sub, amountIncrement_g, MassUnit::g);
  } else // negative increment means it is coming from the target and going to the source
  {
    if (-amountIncrement_g > tgtQ->GetMass(MassUnit::g)) {
      amountIncrement_g = -tgtQ->GetMass(MassUnit::g);
    }
    DistributeMassbyVolumeWeighted(source, sub, -amountIncrement_g, MassUnit::g);
    DistributeMassbyMassWeighted(target, sub, amountIncrement_g, MassUnit::g);
  }

  // Note we are only changing mass and NOT balancing on purpose
  // We don't want the concentrations changing until we have completed our
  // diffusion methodology in its entirety

  return amountIncrement_g;
} // End ActiveMassTransport

/// --------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the mass transport of ionic species Na, K, Ca, Cl against their electrochemical gradient using active pumps and cotransport
///
/// \param tissue: tissue compartment
/// \param vascular: vascular space adjoining tissue compartment
/// \param extra: extracellular space in tissue compartment
/// \param intra:  intracellular space in tissue compartment
/// \param timestep_s: the time step
///
/// \details
/// This active transport moves ions to maintain their resting intracellular and extracellular concentrations using the cell membrane potential and active transport.
/// The current experience by each ion is calculated by comparing its nernst potential to the membrane potential.  Active pumps are then called which have been tuned
/// to drive ionic current to maintain homeostatic concentrations.  Since this method replaces diffusion for Na, Ca, K, and Cl, transport from the extracellular space
/// to the vascular space is also handled here and balancing of the ions is also performed.
//--------------------------------------------------------------------------------------------------
void Tissue::CoupledIonTransport(SETissueCompartment& tissue, SELiquidCompartment& extra, SELiquidCompartment& intra, SELiquidCompartment& vascular)
{
  //All terms are on the basis of a single cell using model from Yi2003Mathematical--these will be converted to macro level using reported cell volume/surface area ratio from same source
  double faradaysConstant_C_Per_mol = 96485;
  double idealGasConstant = 8.314; //Amazingly, R has same value in J/mol-K and mL-Pa/K-umol
  double temperature_K = 310.0; //Replace w/ call to body temperature
  double membranePotential_V = tissue.GetMembranePotential(ElectricPotentialUnit::V);
  double cellSurfaceArea_cm2 = 7.7e-5; //From Yi2003Mathematical
  double scaleFactor = tissue.GetTotalMass(MassUnit::kg) * 1e6 / cellSurfaceArea_cm2; //Carlson reports 1*10^6 cm^2 / kg tissue relationship

  //Get all ion concentrations
  double naExtracellular_mM = extra.GetSubstanceQuantity(*m_Sodium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  double naIntracellular_mM = intra.GetSubstanceQuantity(*m_Sodium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  double kExtracellular_mM = extra.GetSubstanceQuantity(*m_Potassium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  double kIntracellular_mM = intra.GetSubstanceQuantity(*m_Potassium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  double clExtracellular_mM = extra.GetSubstanceQuantity(*m_Chloride)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  double clIntracellular_mM = intra.GetSubstanceQuantity(*m_Chloride)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  //double calciumExtracellular_mM = extra.GetSubstanceQuantity(*m_Calcium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  //double calciumIntracellular_mM = intra.GetSubstanceQuantity(*m_Calcium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);

  /// \ @cite lindblad 1996model
  double naConductance_mC_Per_V_min = 2.925e-5;
  double kConductance_mC_Per_V_min = 7.0e-4;
  double clConductance_mC_Per_V_min = 2.81e-4;
  //double calciumConductance_S_Per_mL = 1.0 / (m_Calcium->GetMembraneResistance(ElectricResistanceUnit::Ohm));

  //Calculate current Nernst potentials for each ion (Faradays constant is multiplied by the charge of the ion--1 for Na and K, -1 for Cl)
  double coreTemp_K = m_data.GetEnergy().GetCoreTemperature(TemperatureUnit::K);
  double naNernst_V = GeneralMath::CalculateNernstPotential(extra, intra, m_Sodium, coreTemp_K);
  double kNernst_V = GeneralMath::CalculateNernstPotential(extra, intra, m_Potassium, coreTemp_K);
  double clNernst_V = GeneralMath::CalculateNernstPotential(extra, intra, m_Chloride, coreTemp_K);
  //double calciumNernst_V = CalculateNernstPotential(extra, intra, m_Calcium, coreTemp_K);

  //Calculate pump rate using information from previous time step--this will lag a time step behind everything but it
  //shouldn't make a huge difference
  double NaKPumpRate_umol_Per_min = SodiumPotassiumPump(naIntracellular_mM, naExtracellular_mM, kExtracellular_mM, membranePotential_V);

  //Calculate new membrane potential (uses previous pump rate)
  membranePotential_V = (naConductance_mC_Per_V_min * naNernst_V + kConductance_mC_Per_V_min * kNernst_V + clConductance_mC_Per_V_min * clNernst_V - faradaysConstant_C_Per_mol * NaKPumpRate_umol_Per_min / 1000.0) / (naConductance_mC_Per_V_min + kConductance_mC_Per_V_min + clConductance_mC_Per_V_min);
  tissue.GetMembranePotential().SetValue(membranePotential_V, ElectricPotentialUnit::V);

  //Calculate diffusion fluxes of each ion through channels in umol/min with respect to intracellular compartment
  //Units:  1000 factor converts mol/C to umol/mC
  double naFlux_umol_Per_min = -((1000.0 * naConductance_mC_Per_V_min / faradaysConstant_C_Per_mol) * (membranePotential_V - naNernst_V) + 3 * NaKPumpRate_umol_Per_min);
  double kFlux_umol_Per_min = -((1000.0 * kConductance_mC_Per_V_min / faradaysConstant_C_Per_mol) * (membranePotential_V - kNernst_V) - 2 * NaKPumpRate_umol_Per_min);
  double clFlux_umol_Per_min = (1000.0 * clConductance_mC_Per_V_min / faradaysConstant_C_Per_mol) * (membranePotential_V - clNernst_V);
  //double calciumDiffusion_mol_Per_mL_s = calciumConductance_S_Per_mL*(membranePotential_V - calciumNernst_V) / (faradaysConstant_C_Per_mol * 2);

  //This commented out block of code takes regulatory volume increase and decrease into account.  In essence, cells can rapidly transport
  //ions through channels to oppose a change in volume caused by a large change in osmotic environment.  It's not clear if we need this as of now
  //but the code is here if we want to turn it on.

  //These constants from Hernandez1998Modeling are used to determined regulatory volume increase/decrease.
  //Boundaries are from Hoffman1989Membrane

  /*double qNa_mL6_Per_umol_min = 0.1 * 1.0e-6 * 60.0 * cellSurfaceArea_cm2;    //Value reported = 0.1 mL^4/mol-s --> Convert to mL^6/umol-min to fit with units later on
        double qK_mL6_Per_umol_min = 10.0 * 1.0e-6 * 60.0 * cellSurfaceArea_cm2;
        double volLimitUpper_mL = 1.01*intracellularVolumeBaseline_mL;
        double volLimitLower_mL = 0.99*intracellularVolumeBaseline_mL;
        if (intracellularVolume_mL <= volLimitLower_mL)
        {
          qNa_mL6_Per_umol_min *= (volLimitLower_mL - intracellularVolume_mL) / volLimitLower_mL;
          qK_mL6_Per_umol_min = 0.0;
        }
        else if (intracellularVolume_mL >= volLimitUpper_mL)
        {
          qNa_mL6_Per_umol_min = 0.0;
          qK_mL6_Per_umol_min *= (intracellularVolume_mL - volLimitUpper_mL) / volLimitUpper_mL;
        }
        else
        {
          qNa_mL6_Per_umol_min = 0.0;
          qK_mL6_Per_umol_min = 0.0;
        }

        double psiNa_umol_Per_min = qNa_mL6_Per_umol_min*(naExtracellular_mM*clExtracellular_mM - naIntracellular_mM*clIntracellular_mM);
        double psiK_umol_Per_min = qK_mL6_Per_umol_min*(kExtracellular_mM*clExtracellular_mM - kIntracellular_mM*clIntracellular_mM);

        naFlux_umol_Per_min += psiNa_umol_Per_min;
        kFlux_umol_Per_min += psiK_umol_Per_min;
        clFlux_umol_Per_min += (psiNa_umol_Per_min + psiK_umol_Per_min);*/
  //Cl flux is coupled to both of these process to maintain electroneutrality

  //End of optional RVI/RVD functionality

  //Here is what we actually move at each time step, based on the calculated rate, and scaled up from cellular level to tissue
  double naIncrement_ug = (naFlux_umol_Per_min * m_Sodium->GetMolarMass(MassPerAmountUnit::g_Per_mol)) * (m_Dt_s / 60) * scaleFactor;
  double kIncrement_ug = (kFlux_umol_Per_min * m_Potassium->GetMolarMass(MassPerAmountUnit::g_Per_mol)) * (m_Dt_s / 60) * scaleFactor;
  double clIncrement_ug = (clFlux_umol_Per_min * m_Chloride->GetMolarMass(MassPerAmountUnit::g_Per_mol)) * (m_Dt_s / 60) * scaleFactor;

  SESubstance* ion;
  double incrementer_ug = 0.0;
  double toVascular_ug = 0.0;
  std::map<SESubstance*, double> ionIncrements = { { m_Sodium, naIncrement_ug }, { m_Potassium, kIncrement_ug }, { m_Chloride, clIncrement_ug } };
  for (auto ions : ionIncrements) {
    ion = ions.first;
    incrementer_ug = ions.second;

    if (incrementer_ug > 0) {
      //Net flux is direction extra-->intra
      if (incrementer_ug > extra.GetSubstanceQuantity(*ion)->GetMass(MassUnit::ug)) {
        //Make sure we don't take it more than is there (shouldn't happen but you never know)
        incrementer_ug = extra.GetSubstanceQuantity(*ion)->GetMass(MassUnit::ug);
      }
      DistributeMassbyMassWeighted(extra, *ion, -incrementer_ug, MassUnit::ug);
      DistributeMassbyVolumeWeighted(intra, *ion, incrementer_ug, MassUnit::ug);
    } else {
      //Net flux is direction intra-->extra
      if (-incrementer_ug > intra.GetSubstanceQuantity(*ion)->GetMass(MassUnit::ug)) {
        incrementer_ug = -intra.GetSubstanceQuantity(*ion)->GetMass(MassUnit::ug);
      }
      DistributeMassbyVolumeWeighted(extra, *ion, -incrementer_ug, MassUnit::ug);
      DistributeMassbyMassWeighted(intra, *ion, incrementer_ug, MassUnit::ug);
    }
    //Balancing will happen in CalcDiffusion method after transport from vascular to extracellular is handled for each ion.
  }
}

//Returns current generated by Na-K Pump
double Tissue::SodiumPotassiumPump(double intraNa_mM, double extraNa_mM, double extraK_mM, double potential_V)
{
  //Formulation of Luo1994Dynamic used in Yi2002Mathematical
  double maxPumpRate_umol_Per_min = 7.15e-8;
  double potassiumMichaelis_mM = 1.0;
  double sodiumMichaelis_mM = 15.0;

  double KTerm = extraK_mM / (extraK_mM + potassiumMichaelis_mM);
  double NaTerm = 1.0 / (1.0 + std::pow((sodiumMichaelis_mM / intraNa_mM), 1.5));
  double sigma = (1.0 / 7.0) * (exp(extraNa_mM / 69.73) - 1);
  double beta = potential_V * 96485.0 / (8.314 * 310.0);
  double f = 1.0 / (1.0 + 0.1245 * exp(-0.1 * beta) + 0.0365 * sigma * exp(-beta));

  double pumpFlux_umol_Per_min = maxPumpRate_umol_Per_min * f * NaTerm * KTerm;

  return pumpFlux_umol_Per_min;
}

double Tissue::CalciumPump(double intraCa_M)
{
  double maxCurrent_A_Per_mL = 0.0033;
  double calciumMichaelis_M = 2.75e-7;

  double calciumCurrent_A_Per_mL = maxCurrent_A_Per_mL * intraCa_M / (intraCa_M + calciumMichaelis_M);

  return calciumCurrent_A_Per_mL;
}

/// --------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the rate of water flow due to osmotic pressure gradient between the extracellular and intracellular spaces
///
/// \details
/// This method was originally designed to handle extra to intra transfer and lymph flow transfer (hence "TissueFluidFluxes").  However
/// the lymph system is currently not being used, so this method only calculates water movement associated with intracellular volume
/// regulation.  It takes the extracellular and intracellular concentrations of Na, K, and Cl and calculates the associated osmotic gradient,]
/// which dictates the magnitude and direction of fluid flow.  The flow value is set on the tissue extra to intra pathway

void Tissue::CalculateTissueFluidFluxes()
{
  SETissueCompartment* tissue;
  SEFluidCircuitPath* osmoticFlow;

  double naIntra_mM = 0.0;
  double naExtra_mM = 0.0;
  double kIntra_mM = 0.0;
  double kExtra_mM = 0.0;
  double clIntra_mM = 0.0;
  double clExtra_mM = 0.0;
  double mOsmIntra = 0.0;
  double mOsmExtra = 0.0;
  double hydraulicConductivity_mL_Per_min_mM = 0.0;
  double proteinIntra_mM = 112.0; //Need this to get osmotically stable cell with the desired ion concentrations
  double intracellularVolumeBaseline_mL = 0.0;
  double intracellularVolume_mL = 0.0;

  for (auto flowPath : m_ExtraToIntraPaths) {
    tissue = flowPath.first;
    osmoticFlow = flowPath.second;

    SELiquidCompartment& extracellular = m_data.GetCompartments().GetExtracellularFluid(*tissue);
    SELiquidCompartment& intracellular = m_data.GetCompartments().GetIntracellularFluid(*tissue);

    intracellularVolumeBaseline_mL = osmoticFlow->GetTargetNode().GetVolumeBaseline(VolumeUnit::mL);
    intracellularVolume_mL = intracellular.GetVolume(VolumeUnit::mL);
    hydraulicConductivity_mL_Per_min_mM = 0.3 * tissue->GetTotalMass(MassUnit::kg); //Carlson has units of mL/min-kg-mmol --> multiply by tissue mass up front

    naIntra_mM = intracellular.GetSubstanceQuantity(*m_Sodium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    naExtra_mM = extracellular.GetSubstanceQuantity(*m_Sodium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    kIntra_mM = intracellular.GetSubstanceQuantity(*m_Potassium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    kExtra_mM = extracellular.GetSubstanceQuantity(*m_Potassium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    clIntra_mM = intracellular.GetSubstanceQuantity(*m_Chloride)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
    clExtra_mM = extracellular.GetSubstanceQuantity(*m_Chloride)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);

    mOsmIntra = naIntra_mM + kIntra_mM + clIntra_mM + proteinIntra_mM * (intracellularVolumeBaseline_mL / intracellularVolume_mL);
    mOsmExtra = naExtra_mM + kExtra_mM + clExtra_mM;

    osmoticFlow->GetNextFlowSource().SetValue(hydraulicConductivity_mL_Per_min_mM * (mOsmIntra - mOsmExtra), VolumePerTimeUnit::mL_Per_min);
  }

  if (m_data.GetActions().GetPatientActions().HasBurnWound()) {
    for (auto t : m_data.GetCompartments().GetTissueCompartments()) {
      SEFluidCircuitPath* res = m_EndothelialResistancePaths[t];
      double resistanceModifier = m_data.GetBloodChemistry().GetInflammatoryResponse().GetTissueIntegrity().GetValue();
      if (t->GetName() != BGE::TissueCompartment::Brain) {
        res->GetNextResistance().SetValue(res->GetResistanceBaseline(FlowResistanceUnit::mmHg_min_Per_mL) * resistanceModifier, FlowResistanceUnit::mmHg_min_Per_mL);
      }
    }
  }
  if (m_data.GetActions().GetPatientActions().HasSepsis()) {
    for (auto t : m_data.GetCompartments().GetTissueCompartments()) {
      SEFluidCircuitPath* res = m_EndothelialResistancePaths[t];
      double resistanceModifier = m_data.GetBloodChemistry().GetInflammatoryResponse().GetTissueIntegrity().GetValue();
      if (t->GetName() != BGE::TissueCompartment::Brain) {
        res->GetNextResistance().SetValue(res->GetResistanceBaseline(FlowResistanceUnit::mmHg_min_Per_mL) * resistanceModifier, FlowResistanceUnit::mmHg_min_Per_mL);
      }
    }
  }
  ///\ToDo:  Use derivative of P-V relationship in Himeno2015Mechanisms to update lymph compliance as volume changes
}

/// --------------------------------------------------------------------------------------------------
/// \brief
/// Calculates and sets oncotic pressure on all plasma to extracellular paths
///
/// \details
/// This function takes the albumin concentrations in the vascular and extracellular spaces of each BioGears compartment (except the brain)
/// and uses the Landis-Pappenheimr approximation to determine osmotic pressure gradient.  Albumin concentrations are calculated in the
/// AlbuminTransport method
void Tissue::CalculateOncoticPressure()
{
  SETissueCompartment* tissue;
  SELiquidCompartment* vascular;
  SEFluidCircuitPath* vascularCOP;
  SEFluidCircuitPath* interstitialCOP;

  ////In calculating oncotic pressure, we assume a linear relationship between plasma albumin concentration and total plasma protein
  ////concentration.  We then use the Landis-Pappenheimer (LP) Equation for total protein. This assumes a healthy state where no other
  ////proteins are leaking across membrane.  When inflammation is present, these pressures will change as result of albumin concentration
  ////which is determined in AlbuminTransport method
  double reflectionCoefficient = 0.0;
  double vascularOncoticPressure_mmHg = 0.0;
  double interstitialOncoticPressure_mmHg = 0.0;
  double totalProteinVascular_g_Per_dL = 0.0;
  double totalProteinInterstitial_g_Per_dL = 0.0;

  for (auto tissueVascular : m_TissueToVascular) {
    tissue = tissueVascular.first;
    vascular = tissueVascular.second;
    if (tissue->GetName() == BGE::TissueCompartment::Brain)
      continue;
    if (vascular->GetName() == BGE::VascularCompartment::LeftKidney)
      vascular = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftRenalArtery);
    if (vascular->GetName() == BGE::VascularCompartment::RightKidney)
      vascular = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightRenalArtery);

    totalProteinVascular_g_Per_dL = 1.6 * vascular->GetSubstanceQuantity(*m_Albumin)->GetConcentration(MassPerVolumeUnit::g_Per_dL);
    totalProteinInterstitial_g_Per_dL = 1.6 * m_data.GetCompartments().GetExtracellularFluid(*tissue).GetSubstanceQuantity(*m_Albumin)->GetConcentration(MassPerVolumeUnit::g_Per_dL);
    vascularOncoticPressure_mmHg = 2.1 * totalProteinVascular_g_Per_dL + 0.16 * std::pow(totalProteinVascular_g_Per_dL, 2) + 0.009 * std::pow(totalProteinVascular_g_Per_dL, 3);
    interstitialOncoticPressure_mmHg = 2.1 * totalProteinInterstitial_g_Per_dL + 0.16 * std::pow(totalProteinInterstitial_g_Per_dL, 2) + 0.009 * std::pow(totalProteinInterstitial_g_Per_dL, 3);

    if (vascular->GetName() == BGE::VascularCompartment::Gut) {
      for (auto c : vascular->GetChildren()) {
        vascularCOP = m_VascularCopPaths[c];
        vascularCOP->GetNextPressureSource().SetValue(-vascularOncoticPressure_mmHg, PressureUnit::mmHg);
      }
    } else {
      vascularCOP = m_VascularCopPaths[vascular];
      vascularCOP->GetNextPressureSource().SetValue(-vascularOncoticPressure_mmHg, PressureUnit::mmHg);
    }
    interstitialCOP = m_InterstitialCopPaths[tissue];
    interstitialCOP->GetNextPressureSource().SetValue(interstitialOncoticPressure_mmHg, PressureUnit::mmHg);
  }
}

double Tissue::AlbuminTransport(SELiquidCompartment& vascular, SELiquidCompartment& extra, SETissueCompartment& tissue, double timestep_s)
{
  double diffusivityCoefficient_mL_Per_min_kg = 0.03306;
  double reflectionCoefficientSmallBase = 0.954;
  double reflectionCoefficientLargeBase = 0.097;
  double reflectionCoefficientLarge = reflectionCoefficientLargeBase;
  double reflectionCoefficientSmall = reflectionCoefficientSmallBase;
  double fluidFlux_mL_Per_min = m_InterstitialCopPaths[&tissue]->GetFlow(VolumePerTimeUnit::mL_Per_min);
  double tissueMass_kg = tissue.GetTotalMass(MassUnit::kg);

  //We need to increase albumin permeability when there is inflammation
  if (m_data.GetBloodChemistry().GetInflammatoryResponse().HasInflammationSources()) {
    double tissueIntegrity = m_data.GetBloodChemistry().GetInflammatoryResponse().GetTissueIntegrity().GetValue();
    reflectionCoefficientSmall = reflectionCoefficientSmallBase * tissueIntegrity;
  }

  BLIM(reflectionCoefficientSmall, 0.0, 1.0);
  BLIM(reflectionCoefficientLarge, 0.0, 1.0);

  double albuminVascular_ug_Per_mL = vascular.GetSubstanceQuantity(*m_Albumin)->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
  double albuminExtracellular_ug_Per_mL = extra.GetSubstanceQuantity(*m_Albumin)->GetConcentration(MassPerVolumeUnit::ug_Per_mL);

  double peclet = fluidFlux_mL_Per_min * (1.0 - reflectionCoefficientSmall) / (diffusivityCoefficient_mL_Per_min_kg * tissueMass_kg);
  double albuminFlux_ug_Per_min = fluidFlux_mL_Per_min * (1.0 - reflectionCoefficientSmall) * ((albuminVascular_ug_Per_mL - albuminExtracellular_ug_Per_mL * std::exp(-peclet)) / (1.0 - std::exp(-peclet)));

  double moved_ug = albuminFlux_ug_Per_min / 60.0 * timestep_s;

  if (moved_ug > 0) {
    if (moved_ug > vascular.GetSubstanceQuantity(*m_Albumin)->GetMass(MassUnit::ug)) {
      moved_ug = vascular.GetSubstanceQuantity(*m_Albumin)->GetMass(MassUnit::ug);
    }
    DistributeMassbyVolumeWeighted(extra, *m_Albumin, moved_ug, MassUnit::ug);
    DistributeMassbyMassWeighted(vascular, *m_Albumin, -moved_ug, MassUnit::ug);
  } else {
    moved_ug = 0.0;
  }

  return moved_ug;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// determine override requirements from user defined inputs
///
/// \details
/// User specified override outputs that are specific to the cardiovascular system are implemented here.
/// If overrides aren't present for this system then this function will not be called during preprocess.
//--------------------------------------------------------------------------------------------------
void Tissue::ProcessOverride()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();

#ifdef BIOGEARS_USE_OVERRIDE_CONTROL
  OverrideControlLoop();
#endif

  if (override->HasExtravascularFluidVolumeOverride()) {
    GetExtravascularFluidVolume().SetValue(override->GetExtravascularFluidVolumeOverride(VolumeUnit::L), VolumeUnit::L);
  }
  if (override->HasIntracellularFluidVolumeOverride()) {
    GetIntracellularFluidVolume().SetValue(override->GetIntracellularFluidVolumeOverride(VolumeUnit::L), VolumeUnit::L);
  }
  if (override->HasLiverGlycogenOverride()) {
    GetLiverGlycogen().SetValue(override->GetLiverGlycogenOverride(MassUnit::g), MassUnit::g);
  }
  if (override->HasMuscleGlycogenOverride()) {
    GetMuscleGlycogen().SetValue(override->GetMuscleGlycogenOverride(MassUnit::g), MassUnit::g);
  }
  if (override->HasStoredProteinOverride()) {
    GetStoredProtein().SetValue(override->GetStoredProteinOverride(MassUnit::g), MassUnit::g);
  }
  if (override->HasStoredFatOverride()) {
    GetStoredFat().SetValue(override->GetStoredFatOverride(MassUnit::g), MassUnit::g);
  }
}

//// Can be turned on or off (for debugging purposes) using the Biogears_USE_OVERRIDE_CONTROL external in CMake
void Tissue::OverrideControlLoop()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();

  constexpr double maxExtravascularFluidVolumeOverride = 1000.0; //L
  constexpr double minExtravascularFluidVolumeOverride = 0.0; //L
  constexpr double maxIntracellularFluidVolumeOverride = 1000.0; //L
  constexpr double minIntracellularFluidVolumeOverride = 0.0; //L
  constexpr double maxLiverGlycogenOverride = 1000.0; //g
  constexpr double minLiverGlycogenOverride = 0.0; //g
  constexpr double maxMuscleGlycogenOverride = 2000.0; //g
  constexpr double minMuscleGlycogenOverride = 0.0; //g
  constexpr double maxStoredProteinOverride = 5000.0; //g
  constexpr double minStoredProteinOverride = 0.0; //g
  constexpr double maxStoredFatOverride = 5000.0; //g
  constexpr double minStoredFatOverride = 0.0; //g // Zero will cause irreversible state

  double currentExtravascularFluidVolumeOverride = 0.0; //value gets changed in next check
  double currentIntracellularFluidVolumeOverride = 0.0; //value gets changed in next check
  double currentLiverGlycogenOverride = 0.0; //value gets changed in next check
  double currentMuscleGlycogenOverride = 0.0; //value gets changed in next check
  double currentStoredProteinOverride = 0.0; //value gets changed in next check
  double currentStoredFatOverride = 0.0; //value gets changed in next check

  if (override->HasExtravascularFluidVolumeOverride()) {
    currentExtravascularFluidVolumeOverride = override->GetExtravascularFluidVolumeOverride(VolumeUnit::L);
  }
  if (override->HasIntracellularFluidVolumeOverride()) {
    currentIntracellularFluidVolumeOverride = override->GetIntracellularFluidVolumeOverride(VolumeUnit::L);
  }
  if (override->HasLiverGlycogenOverride()) {
    currentLiverGlycogenOverride = override->GetLiverGlycogenOverride(MassUnit::g);
  }
  if (override->HasMuscleGlycogenOverride()) {
    currentMuscleGlycogenOverride = override->GetMuscleGlycogenOverride(MassUnit::g);
  }
  if (override->HasStoredProteinOverride()) {
    currentStoredProteinOverride = override->GetStoredProteinOverride(MassUnit::g);
  }
  if (override->HasStoredFatOverride()) {
    currentStoredFatOverride = override->GetStoredFatOverride(MassUnit::g);
  }

  if ((currentExtravascularFluidVolumeOverride < minExtravascularFluidVolumeOverride || currentExtravascularFluidVolumeOverride > maxExtravascularFluidVolumeOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Extravascular Fluid Volume Override (Tissue) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentIntracellularFluidVolumeOverride < minIntracellularFluidVolumeOverride || currentIntracellularFluidVolumeOverride > maxIntracellularFluidVolumeOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Intracellular Fluid Volume Override (Tissue) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentLiverGlycogenOverride < minLiverGlycogenOverride || currentLiverGlycogenOverride > maxLiverGlycogenOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Liver Glycogen Override (Tissue) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentMuscleGlycogenOverride < minMuscleGlycogenOverride || currentMuscleGlycogenOverride > maxMuscleGlycogenOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Muscle Glycogen Override (Tissue) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentStoredProteinOverride < minStoredProteinOverride || currentStoredProteinOverride > maxStoredProteinOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Stored Protein Override (Tissue) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentStoredFatOverride < minStoredFatOverride || currentStoredFatOverride > maxStoredFatOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Stored Fat Override (Tissue) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  return;
}

}