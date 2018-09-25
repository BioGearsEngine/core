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
#include <biogears/engine/Systems/BloodChemistry.h>

#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>
#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

#pragma warning(disable : 4786)
#pragma warning(disable : 4275)
namespace biogears {
BloodChemistry::BloodChemistry(BioGears& bg)
  : SEBloodChemistrySystem(bg.GetLogger())
  , m_data(bg)
{
  Clear();
}

BloodChemistry::~BloodChemistry()
{
  Clear();
}

void BloodChemistry::Clear()
{
  SEBloodChemistrySystem::Clear();
  m_aorta = nullptr;
  m_aortaO2 = nullptr;
  m_aortaCO2 = nullptr;
  m_aortaCO = nullptr;
  m_aortaBicarbonate = nullptr;
  m_brainO2 = nullptr;
  m_myocardiumO2 = nullptr;
  m_pulmonaryArteriesO2 = nullptr;
  m_pulmonaryArteriesCO2 = nullptr;
  m_pulmonaryVeinsO2 = nullptr;
  m_pulmonaryVeinsCO2 = nullptr;
  m_venaCava = nullptr;
  m_venaCavaO2 = nullptr;
  m_venaCavaCO2 = nullptr;
  m_venaCavaKetones = nullptr;
  m_venaCavaAlbumin = nullptr;
  m_venaCavaBicarbonate = nullptr;
  m_venaCavaCalcium = nullptr;
  m_venaCavaChloride = nullptr;
  m_venaCavaCreatinine = nullptr;
  m_venaCavaEpinephrine = nullptr;
  m_venaCavaGlucose = nullptr;
  m_venaCavaInsulin = nullptr;
  m_venaCavaLactate = nullptr;
  m_venaCavaPotassium = nullptr;
  m_venaCavaSodium = nullptr;
  m_venaCavaTriacylglycerol = nullptr;
  m_venaCavaUrea = nullptr;
  m_ArterialOxygen_mmHg.Reset();
  m_ArterialCarbonDioxide_mmHg.Reset();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties.
//--------------------------------------------------------------------------------------------------
void BloodChemistry::Initialize()
{
  BioGearsSystem::Initialize();
  GetBloodDensity().SetValue(1050, MassPerVolumeUnit::kg_Per_m3);
  GetBloodSpecificHeat().SetValue(3617, HeatCapacitancePerMassUnit::J_Per_K_kg);
  GetVolumeFractionNeutralLipidInPlasma().SetValue(0.0023);
  GetVolumeFractionNeutralPhospholipidInPlasma().SetValue(0.0013);
  GetWhiteBloodCellCount().SetValue(7000, AmountPerVolumeUnit::ct_Per_uL);
  GetPhosphate().SetValue(1.1, AmountPerVolumeUnit::mmol_Per_L);
  GetStrongIonDifference().SetValue(40.5, AmountPerVolumeUnit::mmol_Per_L);
  GetTotalBilirubin().SetValue(0.70, MassPerVolumeUnit::mg_Per_dL); //Reference range is 0.2-1.0
  //Note that RedBloodCellAcetylcholinesterase is initialized in Drugs file because Drugs is processed before Blood Chemistry

  m_ArterialOxygen_mmHg.Sample(m_aortaO2->GetPartialPressure(PressureUnit::mmHg));
  m_ArterialCarbonDioxide_mmHg.Sample(m_aortaCO2->GetPartialPressure(PressureUnit::mmHg));
  GetCarbonMonoxideSaturation().SetValue(0);
  Process(); // Calculate the initial system values
}

bool BloodChemistry::Load(const CDM::BioGearsBloodChemistrySystemData& in)
{
  if (!SEBloodChemistrySystem::Load(in))
    return false;
  m_ArterialOxygen_mmHg.Load(in.ArterialOxygenAverage_mmHg());
  m_ArterialCarbonDioxide_mmHg.Load(in.ArterialCarbonDioxideAverage_mmHg());
  BioGearsSystem::LoadState();
  return true;
}
CDM::BioGearsBloodChemistrySystemData* BloodChemistry::Unload() const
{
  CDM::BioGearsBloodChemistrySystemData* data = new CDM::BioGearsBloodChemistrySystemData();
  Unload(*data);
  return data;
}
void BloodChemistry::Unload(CDM::BioGearsBloodChemistrySystemData& data) const
{
  SEBloodChemistrySystem::Unload(data);
  data.ArterialOxygenAverage_mmHg(std::unique_ptr<CDM::RunningAverageData>(m_ArterialOxygen_mmHg.Unload()));
  data.ArterialCarbonDioxideAverage_mmHg(std::unique_ptr<CDM::RunningAverageData>(m_ArterialCarbonDioxide_mmHg.Unload()));
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for BloodChemistry Class
///
///  \details
///  Initializes member variables and system level values on the common data model.
//--------------------------------------------------------------------------------------------------
void BloodChemistry::SetUp()
{
  const BioGearsConfiguration& ConfigData = m_data.GetConfiguration();
  m_redBloodCellVolume_mL = ConfigData.GetMeanCorpuscularVolume(VolumeUnit::mL);
  m_HbPerRedBloodCell_ug_Per_ct = ConfigData.GetMeanCorpuscularHemoglobin(MassPerAmountUnit::ug_Per_ct);

  //Substance
  SESubstance* albumin = &m_data.GetSubstances().GetAlbumin();
  SESubstance* aminoAcids = &m_data.GetSubstances().GetAminoAcids();
  SESubstance* bicarbonate = &m_data.GetSubstances().GetBicarbonate();
  SESubstance* calcium = &m_data.GetSubstances().GetCalcium();
  SESubstance* chloride = &m_data.GetSubstances().GetChloride();
  SESubstance* creatinine = &m_data.GetSubstances().GetCreatinine();
  SESubstance* epinephrine = &m_data.GetSubstances().GetEpi();
  SESubstance* glucagon = &m_data.GetSubstances().GetGlucagon();
  SESubstance* glucose = &m_data.GetSubstances().GetGlucose();
  SESubstance* insulin = &m_data.GetSubstances().GetInsulin();
  SESubstance* ketones = &m_data.GetSubstances().GetKetones();
  SESubstance* lactate = &m_data.GetSubstances().GetLactate();
  SESubstance* potassium = &m_data.GetSubstances().GetPotassium();
  SESubstance* sodium = &m_data.GetSubstances().GetSodium();
  SESubstance* triaclyglycerol = &m_data.GetSubstances().GetTriacylglycerol();
  SESubstance* urea = &m_data.GetSubstances().GetUrea();

  m_aorta = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Aorta);
  m_aortaO2 = m_aorta->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_aortaCO2 = m_aorta->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());
  m_aortaBicarbonate = m_aorta->GetSubstanceQuantity(m_data.GetSubstances().GetHCO3());

  m_brainO2 = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Brain)->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_myocardiumO2 = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Myocardium)->GetSubstanceQuantity(m_data.GetSubstances().GetO2());

  m_venaCava = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::VenaCava);
  m_venaCavaO2 = m_venaCava->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_venaCavaCO2 = m_venaCava->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());
  m_venaCavaAlbumin = m_venaCava->GetSubstanceQuantity(*albumin);
  m_venaCavaAminoAcids = m_venaCava->GetSubstanceQuantity(*aminoAcids);
  m_venaCavaBicarbonate = m_venaCava->GetSubstanceQuantity(*bicarbonate);
  m_venaCavaCalcium = m_venaCava->GetSubstanceQuantity(*calcium);
  m_venaCavaChloride = m_venaCava->GetSubstanceQuantity(*chloride);
  m_venaCavaCreatinine = m_venaCava->GetSubstanceQuantity(*creatinine);
  m_venaCavaEpinephrine = m_venaCava->GetSubstanceQuantity(*epinephrine);
  m_venaCavaGlucagon = m_venaCava->GetSubstanceQuantity(*glucagon);
  m_venaCavaGlucose = m_venaCava->GetSubstanceQuantity(*glucose);
  m_venaCavaInsulin = m_venaCava->GetSubstanceQuantity(*insulin);
  m_venaCavaKetones = m_venaCava->GetSubstanceQuantity(*ketones);
  m_venaCavaLactate = m_venaCava->GetSubstanceQuantity(*lactate);
  m_venaCavaPotassium = m_venaCava->GetSubstanceQuantity(*potassium);
  m_venaCavaSodium = m_venaCava->GetSubstanceQuantity(*sodium);
  m_venaCavaTriacylglycerol = m_venaCava->GetSubstanceQuantity(*triaclyglycerol);
  m_venaCavaUrea = m_venaCava->GetSubstanceQuantity(*urea);

  SELiquidCompartment* pulmonaryArteries = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::PulmonaryArteries);
  m_pulmonaryArteriesO2 = pulmonaryArteries->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_pulmonaryArteriesCO2 = pulmonaryArteries->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());

  SELiquidCompartment* pulmonaryVeins = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::PulmonaryVeins);
  m_pulmonaryVeinsO2 = pulmonaryVeins->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_pulmonaryVeinsCO2 = pulmonaryVeins->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());

  double dT_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  m_PatientActions = &m_data.GetActions().GetPatientActions();
}

void BloodChemistry::AtSteadyState()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Blood Chemistry Preprocess method
///
/// \details
/// The current BioGears implementation only checks for active Sepsis event.
//--------------------------------------------------------------------------------------------------
void BloodChemistry::PreProcess()
{
  Sepsis(); //Need this here because "next" values are set on circuit
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Blood Chemistry process method
///
/// \details
/// The system data (blood concentrations, blood gases, and other blood properties) that are calculated
/// or changed by other systems are set on the blood chemistry system data objects. Events
/// are triggered at specific blood concentrations of certain substances in CheckBloodGasLevels().
//--------------------------------------------------------------------------------------------------
void BloodChemistry::Process()
{
  //Push the compartment values of O2 and CO2 partial pressures on the corresponding system data.
  GetOxygenSaturation().Set(m_aortaO2->GetSaturation());
  GetCarbonDioxideSaturation().Set(m_aortaCO2->GetSaturation());
  if (m_aortaCO == nullptr && m_data.GetSubstances().IsActive(m_data.GetSubstances().GetCO()))
    m_aortaCO = m_aorta->GetSubstanceQuantity(m_data.GetSubstances().GetCO());
  if (m_aortaCO != nullptr) {
    GetCarbonMonoxideSaturation().Set(m_aortaCO->GetSaturation());
    GetPulseOximetry().SetValue(GetOxygenSaturation().GetValue() + GetCarbonMonoxideSaturation().GetValue());
  } else {
    GetPulseOximetry().Set(GetOxygenSaturation());
  }

  // This Hemoglobin Content is the mass of the hemoglobin only, not the hemoglobin and bound gas.
  // So we have to take our 4 Hb species masses and remove the mass of the gas.
  // Step 1) Get the mass of the bound species, which includes the mass of the bound gas.
  // Step 2) Convert to moles using the molar weight of the bound species (molar mass of bound species includes the mass of the bound gas and the mass of the unbound species).
  // Step 3) Covert moles of the bound species to moles of the unbound species. i.e.multiply by 1 (this step is implied)
  // Step 4) Convert moles of the unbound species to mass in order to get a total mass of hemoglobin (as opposed to a total mass of hemoglobin plus the bound gases).
  double totalHb_g = m_data.GetSubstances().GetSubstanceMass(m_data.GetSubstances().GetHb(), m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::g);
  double totalHbO2_g = (m_data.GetSubstances().GetSubstanceMass(m_data.GetSubstances().GetHbO2(), m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::g) / m_data.GetSubstances().GetHbO2().GetMolarMass(MassPerAmountUnit::g_Per_mol)) * m_data.GetSubstances().GetHb().GetMolarMass(MassPerAmountUnit::g_Per_mol);
  double totalHbCO2_g = (m_data.GetSubstances().GetSubstanceMass(m_data.GetSubstances().GetHbCO2(), m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::g) / m_data.GetSubstances().GetHbCO2().GetMolarMass(MassPerAmountUnit::g_Per_mol)) * m_data.GetSubstances().GetHb().GetMolarMass(MassPerAmountUnit::g_Per_mol);
  double totalHBO2CO2_g = (m_data.GetSubstances().GetSubstanceMass(m_data.GetSubstances().GetHbO2CO2(), m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::g) / m_data.GetSubstances().GetHbO2CO2().GetMolarMass(MassPerAmountUnit::g_Per_mol)) * m_data.GetSubstances().GetHb().GetMolarMass(MassPerAmountUnit::g_Per_mol);
  double totalHBCO_g = 0.0;
  if (m_aortaCO != nullptr)
    totalHBCO_g = (m_data.GetSubstances().GetSubstanceMass(m_data.GetSubstances().GetHbCO(), m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::g) / m_data.GetSubstances().GetHbCO().GetMolarMass(MassPerAmountUnit::g_Per_mol)) * m_data.GetSubstances().GetHb().GetMolarMass(MassPerAmountUnit::g_Per_mol);

  double totalHemoglobinO2Hemoglobin_g = totalHb_g + totalHbO2_g + totalHbCO2_g + totalHBO2CO2_g + totalHBCO_g;
  GetHemoglobinContent().SetValue(totalHemoglobinO2Hemoglobin_g, MassUnit::g);

  // Calculate Blood Cell Counts
  double RedBloodCellCount_ct = GetHemoglobinContent(MassUnit::ug) / m_HbPerRedBloodCell_ug_Per_ct;
  double RedBloodCellVolume_mL = RedBloodCellCount_ct * m_redBloodCellVolume_mL;
  double TotalBloodVolume_mL = m_data.GetCardiovascular().GetBloodVolume(VolumeUnit::mL);
  GetHematocrit().SetValue((RedBloodCellVolume_mL / TotalBloodVolume_mL));
  // Yes, we are giving GetRedBloodCellCount a concentration, because that is what it is, but clinically, it is known as RedBloodCellCount
  GetRedBloodCellCount().SetValue(RedBloodCellCount_ct / m_data.GetCardiovascular().GetBloodVolume(VolumeUnit::L), AmountPerVolumeUnit::ct_Per_L);
  GetPlasmaVolume().SetValue(TotalBloodVolume_mL - RedBloodCellVolume_mL, VolumeUnit::mL);

  // Concentrations
  double albuminConcentration_ug_Per_mL = m_venaCavaAlbumin->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
  m_data.GetSubstances().GetAlbumin().GetBloodConcentration().Set(m_venaCavaAlbumin->GetConcentration());
  m_data.GetSubstances().GetAminoAcids().GetBloodConcentration().Set(m_venaCavaAminoAcids->GetConcentration());
  m_data.GetSubstances().GetBicarbonate().GetBloodConcentration().Set(m_venaCavaBicarbonate->GetConcentration());
  GetBloodUreaNitrogenConcentration().SetValue(m_venaCavaUrea->GetConcentration(MassPerVolumeUnit::ug_Per_mL) / 2.14, MassPerVolumeUnit::ug_Per_mL);
  m_data.GetSubstances().GetCalcium().GetBloodConcentration().Set(m_venaCavaCalcium->GetConcentration());
  m_data.GetSubstances().GetChloride().GetBloodConcentration().Set(m_venaCavaChloride->GetConcentration());
  m_data.GetSubstances().GetCreatinine().GetBloodConcentration().Set(m_venaCavaCreatinine->GetConcentration());
  m_data.GetSubstances().GetEpi().GetBloodConcentration().Set(m_venaCavaEpinephrine->GetConcentration());
  m_data.GetSubstances().GetGlobulin().GetBloodConcentration().SetValue(albuminConcentration_ug_Per_mL * 1.6 - albuminConcentration_ug_Per_mL, MassPerVolumeUnit::ug_Per_mL); // 1.6 comes from reading http://www.drkaslow.com/html/proteins_-_albumin-_globulins-_etc.html
  m_data.GetSubstances().GetGlucagon().GetBloodConcentration().Set(m_venaCavaGlucagon->GetConcentration());
  m_data.GetSubstances().GetGlucose().GetBloodConcentration().Set(m_venaCavaGlucose->GetConcentration());
  double HemoglobinConcentration = totalHemoglobinO2Hemoglobin_g / TotalBloodVolume_mL;
  m_data.GetSubstances().GetHb().GetBloodConcentration().SetValue(HemoglobinConcentration, MassPerVolumeUnit::g_Per_mL);
  m_data.GetSubstances().GetInsulin().GetBloodConcentration().Set(m_venaCavaInsulin->GetConcentration());
  m_data.GetSubstances().GetKetones().GetBloodConcentration().Set(m_venaCavaKetones->GetConcentration());
  m_data.GetSubstances().GetLactate().GetBloodConcentration().Set(m_venaCavaLactate->GetConcentration());
  m_data.GetSubstances().GetPotassium().GetBloodConcentration().Set(m_venaCavaPotassium->GetConcentration());
  m_data.GetSubstances().GetSodium().GetBloodConcentration().Set(m_venaCavaSodium->GetConcentration());
  GetTotalProteinConcentration().SetValue(albuminConcentration_ug_Per_mL * 1.6, MassPerVolumeUnit::ug_Per_mL);
  m_data.GetSubstances().GetTriacylglycerol().GetBloodConcentration().Set(m_venaCavaTriacylglycerol->GetConcentration());
  m_data.GetSubstances().GetUrea().GetBloodConcentration().Set(m_venaCavaUrea->GetConcentration());

  // Calculate pH
  /// \todo Change system data so that we have ArterialBloodPH (from aorta) and VenousBloodPH (from vena cava)
  GetBloodPH().Set(m_aorta->GetPH());
  GetArterialBloodPH().Set(m_aorta->GetPH());
  GetVenousBloodPH().Set(m_venaCava->GetPH());

  // Pressures
  // arterial gas partial pressures -
  GetArterialOxygenPressure().Set(m_aortaO2->GetPartialPressure());
  GetArterialCarbonDioxidePressure().Set(m_aortaCO2->GetPartialPressure());
  // pulmonary arteries
  GetPulmonaryArterialOxygenPressure().Set(m_pulmonaryArteriesO2->GetPartialPressure());
  GetPulmonaryArterialCarbonDioxidePressure().Set(m_pulmonaryArteriesCO2->GetPartialPressure());
  // pulmonary vein gas partial pressures -  the average of right and left pulmonary vein gas pressures
  GetPulmonaryVenousOxygenPressure().Set(m_pulmonaryVeinsO2->GetPartialPressure());
  GetPulmonaryVenousCarbonDioxidePressure().Set(m_pulmonaryVeinsCO2->GetPartialPressure());
  // venous gas partial pressures
  GetVenousOxygenPressure().Set(m_venaCavaO2->GetPartialPressure());
  GetVenousCarbonDioxidePressure().Set(m_venaCavaCO2->GetPartialPressure());

  double totalFlow_mL_Per_min = m_data.GetCardiovascular().GetCardiacOutput(VolumePerTimeUnit::mL_Per_min);
  double shuntFlow_mL_Per_min = m_data.GetCardiovascular().GetPulmonaryMeanShuntFlow(VolumePerTimeUnit::mL_Per_min);
  double shunt = shuntFlow_mL_Per_min / totalFlow_mL_Per_min;
  GetShuntFraction().SetValue(shunt);

  //Throw events if levels are low/high
  CheckBloodSubstanceLevels();

  // Total up all active substances
  double bloodMass_ug;
  double tissueMass_ug;
  for (SESubstance* sub : m_data.GetSubstances().GetActiveSubstances()) {
    bloodMass_ug = m_data.GetSubstances().GetSubstanceMass(*sub, m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::ug);
    tissueMass_ug = m_data.GetSubstances().GetSubstanceMass(*sub, m_data.GetCompartments().GetTissueLeafCompartments(), MassUnit::ug);
    sub->GetMassInBody().SetValue(bloodMass_ug + tissueMass_ug, MassUnit::ug);
    sub->GetMassInBlood().SetValue(bloodMass_ug, MassUnit::ug);
    sub->GetMassInTissue().SetValue(tissueMass_ug, MassUnit::ug);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Blood Chemistry postprocess method
///
/// \details
/// The current BioGears implementation has no specific postprocess functionality.
//--------------------------------------------------------------------------------------------------
void BloodChemistry::PostProcess()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Checks the blood substance (oxygen, carbon dioxide, glucose, etc.) levels and sets events.
///
/// \details
/// Checks the oxygen, carbon dioxide, glucose, ketones, and lactate levels in specific compartments of the body.
/// Set events for Hypercapnia, Hypoxia, Brain and MyoCardium Oxygen Deficit, Hypo/hyperglycemia and related events,
/// ketoacidosis, and lactic acidosis based on the levels.
//--------------------------------------------------------------------------------------------------
void BloodChemistry::CheckBloodSubstanceLevels()
{
  SEPatient& patient = m_data.GetPatient();
  double hypoxiaFlag = 65.0; //Arterial O2 Partial Pressure in mmHg \cite Pierson2000Pathophysiology
  double hypoxiaIrreversible = 15.0; // \cite hobler1973HypoxemiaCardiacOutput
  double hypoglycemiaLevel_mg_Per_dL = 70;
  double hypoglycemicShockLevel_mg_Per_dL = 50;
  double hypoglycemicComaLevel_mg_Per_dL = 20;
  double hyperglycemiaLevel_mg_Per_dL = 200;
  double lacticAcidosisLevel_mg_Per_dL = 44;
  double ketoacidosisLevel_mg_Per_dL = 122;

  m_ArterialOxygen_mmHg.Sample(m_aortaO2->GetPartialPressure(PressureUnit::mmHg));
  m_ArterialCarbonDioxide_mmHg.Sample(m_aortaCO2->GetPartialPressure(PressureUnit::mmHg));
  //Only check these at the end of a cardiac cycle and reset at start of cardiac cycle
  if (patient.IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
    double arterialOxygen_mmHg = m_ArterialOxygen_mmHg.Value();
    double arterialCarbonDioxide_mmHg = m_ArterialCarbonDioxide_mmHg.Value();

    if (m_data.GetState() > EngineState::InitialStabilization) { // Don't throw events if we are initializing
      // hypercapnia check
      double hypercapniaFlag = 60.0; // \cite Guyton11thEd p.531
      double carbonDioxideToxicity = 80.0; // \cite labertsen1971CarbonDioxideToxicity
      if (arterialCarbonDioxide_mmHg >= hypercapniaFlag) {
        /// \event Patient: Hypercapnia. The carbon dioxide partial pressure has risen above 60 mmHg. The patient is now hypercapnic.
        patient.SetEvent(CDM::enumPatientEvent::Hypercapnia, true, m_data.GetSimulationTime());

        if (arterialCarbonDioxide_mmHg > carbonDioxideToxicity) {
          m_ss << "Arterial Carbon Dioxide partial pressure is " << arterialCarbonDioxide_mmHg << ". This is beyond 80 mmHg triggering extreme Hypercapnia, patient is in an irreversible state.";
          Warning(m_ss);
          /// \irreversible The carbon dioxide partial pressure is greater than 80 mmHg.
          patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        }
      } else if (patient.IsEventActive(CDM::enumPatientEvent::Hypercapnia) && arterialCarbonDioxide_mmHg < (hypercapniaFlag - 3)) {
        /// \event Patient: End Hypercapnia. The carbon dioxide partial pressure has fallen below 57 mmHg. The patient is no longer considered to be hypercapnic.
        /// This event is triggered if the patient was hypercapnic and is now considered to be recovered.
        patient.SetEvent(CDM::enumPatientEvent::Hypercapnia, false, m_data.GetSimulationTime());
      }

      // hypoxia check
      if (arterialOxygen_mmHg <= hypoxiaFlag) {
        /// \event Patient: Hypoxia Event. The oxygen partial pressure has fallen below 65 mmHg, indicating that the patient is hypoxic.
        patient.SetEvent(CDM::enumPatientEvent::Hypoxia, true, m_data.GetSimulationTime());

        if (arterialOxygen_mmHg < hypoxiaIrreversible) {
          m_ss << "Arterial Oxygen partial pressure is " << arterialOxygen_mmHg << ". This is below 15 mmHg triggering extreme Hypoxia, patient is in an irreversible state.";
          Warning(m_ss);
          /// \irreversible Arterial oxygen partial pressure has been critically reduced to below 15 mmHg.
          patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        }
      } else if (arterialOxygen_mmHg > (hypoxiaFlag + 3)) {
        /// \event Patient: End Hypoxia Event. The oxygen partial pressure has rise above 68 mmHg. If this occurs when the patient is hypoxic, it will reverse the hypoxic event.
        /// The patient is no longer considered to be hypoxic.
        patient.SetEvent(CDM::enumPatientEvent::Hypoxia, false, m_data.GetSimulationTime());
      }

      //glucose checks

      //hypoglycemia
      if (m_venaCavaGlucose->GetConcentration(MassPerVolumeUnit::mg_Per_dL) < hypoglycemiaLevel_mg_Per_dL) {
        patient.SetEvent(CDM::enumPatientEvent::Hypoglycemia, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::Hypoglycemia) && m_venaCavaGlucose->GetConcentration(MassPerVolumeUnit::mg_Per_dL) > hypoglycemiaLevel_mg_Per_dL + 5) {
        patient.SetEvent(CDM::enumPatientEvent::Hypoglycemia, false, m_data.GetSimulationTime());
      }

      //hypoglycemic shock
      if (m_venaCavaGlucose->GetConcentration(MassPerVolumeUnit::mg_Per_dL) < hypoglycemicShockLevel_mg_Per_dL) {
        patient.SetEvent(CDM::enumPatientEvent::HypoglycemicShock, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::HypoglycemicShock) && m_venaCavaGlucose->GetConcentration(MassPerVolumeUnit::mg_Per_dL) > hypoglycemicShockLevel_mg_Per_dL + 5) {
        patient.SetEvent(CDM::enumPatientEvent::HypoglycemicShock, false, m_data.GetSimulationTime());
      }

      //hypoglycemic coma
      if (m_venaCavaGlucose->GetConcentration(MassPerVolumeUnit::mg_Per_dL) < hypoglycemicComaLevel_mg_Per_dL) {
        patient.SetEvent(CDM::enumPatientEvent::HypoglycemicComa, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::HypoglycemicComa) && m_venaCavaGlucose->GetConcentration(MassPerVolumeUnit::mg_Per_dL) > hypoglycemicComaLevel_mg_Per_dL + 3) {
        patient.SetEvent(CDM::enumPatientEvent::HypoglycemicComa, false, m_data.GetSimulationTime());
      }

      //hyperglycemia
      if (m_venaCavaGlucose->GetConcentration(MassPerVolumeUnit::mg_Per_dL) > hyperglycemiaLevel_mg_Per_dL) {
        patient.SetEvent(CDM::enumPatientEvent::Hyperglycemia, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::Hyperglycemia) && m_venaCavaGlucose->GetConcentration(MassPerVolumeUnit::mg_Per_dL) < hypoglycemicShockLevel_mg_Per_dL - 10) {
        patient.SetEvent(CDM::enumPatientEvent::Hyperglycemia, false, m_data.GetSimulationTime());
      }

      //lactate check
      if (m_venaCavaLactate->GetConcentration(MassPerVolumeUnit::mg_Per_dL) > lacticAcidosisLevel_mg_Per_dL) {
        patient.SetEvent(CDM::enumPatientEvent::LacticAcidosis, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::LacticAcidosis) && m_venaCavaLactate->GetConcentration(MassPerVolumeUnit::mg_Per_dL) < lacticAcidosisLevel_mg_Per_dL - 5) {
        patient.SetEvent(CDM::enumPatientEvent::LacticAcidosis, false, m_data.GetSimulationTime());
      }

      //ketones check
      if (m_venaCavaKetones->GetConcentration(MassPerVolumeUnit::mg_Per_dL) > ketoacidosisLevel_mg_Per_dL) {
        patient.SetEvent(CDM::enumPatientEvent::Ketoacidosis, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::Ketoacidosis) && m_venaCavaKetones->GetConcentration(MassPerVolumeUnit::mg_Per_dL) < ketoacidosisLevel_mg_Per_dL - 5) {
        patient.SetEvent(CDM::enumPatientEvent::Ketoacidosis, false, m_data.GetSimulationTime());
      }

      //sodium check
      //Mild and severe hyponatremia
      if (m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) < 136.0) {
        patient.SetEvent(CDM::enumPatientEvent::MildHyponatremia, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::MildHyponatremia) && m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) > 137.0) {
        patient.SetEvent(CDM::enumPatientEvent::MildHyponatremia, false, m_data.GetSimulationTime());
      }

      if (m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) < 120.0) {
        patient.SetEvent(CDM::enumPatientEvent::SevereHyponatremia, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::SevereHyponatremia) && m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) > 121.0) {
        patient.SetEvent(CDM::enumPatientEvent::SevereHyponatremia, false, m_data.GetSimulationTime());
      }
      //Mild and severe hypernatremia
      if (m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) > 148.0) {
        patient.SetEvent(CDM::enumPatientEvent::MildHypernatremia, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::MildHypernatremia) && m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) < 147.0) {
        patient.SetEvent(CDM::enumPatientEvent::MildHypernatremia, false, m_data.GetSimulationTime());
      }
      if (m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) > 160.0) {
        patient.SetEvent(CDM::enumPatientEvent::SevereHypernatremia, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::SevereHypernatremia) && m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) < 159.0) {
        patient.SetEvent(CDM::enumPatientEvent::SevereHypernatremia, false, m_data.GetSimulationTime());
      }

      //potassium check
      //mild and severe hypokalemia
      if (m_venaCavaPotassium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) < 3.2) {
        patient.SetEvent(CDM::enumPatientEvent::MildHypokalemia, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::MildHypokalemia) && m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) > 3.3) {
        patient.SetEvent(CDM::enumPatientEvent::MildHypokalemia, false, m_data.GetSimulationTime());
      }
      if (m_venaCavaPotassium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) < 2.5) {
        patient.SetEvent(CDM::enumPatientEvent::SevereHypokalemia, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::SevereHypokalemia) && m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) > 2.6) {
        patient.SetEvent(CDM::enumPatientEvent::SevereHypokalemia, false, m_data.GetSimulationTime());
      }
      //mild and severe hyperkalemia
      if (m_venaCavaPotassium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) > 5.5) {
        patient.SetEvent(CDM::enumPatientEvent::MildHyperkalemia, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::MildHyperkalemia) && m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) < 5.6) {
        patient.SetEvent(CDM::enumPatientEvent::MildHyperkalemia, false, m_data.GetSimulationTime());
      }
      if (m_venaCavaPotassium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) > 6.2) {
        patient.SetEvent(CDM::enumPatientEvent::SevereHyperkalemia, true, m_data.GetSimulationTime());
      } else if (patient.IsEventActive(CDM::enumPatientEvent::SevereHyperkalemia) && m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) < 6.1) {
        patient.SetEvent(CDM::enumPatientEvent::SevereHyperkalemia, false, m_data.GetSimulationTime());
      }
    }

    m_ArterialOxygen_mmHg.Reset();
    m_ArterialCarbonDioxide_mmHg.Reset();
  }

  if (m_data.GetState() > EngineState::InitialStabilization) { // Don't throw events if we are initializing

    // When the brain oxygen partial pressure is too low, events are triggered and event duration is tracked.
    // If the oxygen tension in the brain remains below the thresholds for the specified amount of time, the body
    // will go into an irreversible state. The threshold values are chosen based on empirical data reviewed in summary in @cite dhawan2011neurointensive
    // and from data presented in @cite purins2012brain and @cite doppenberg1998determination.
    if (m_brainO2->GetPartialPressure(PressureUnit::mmHg) < 21.0) {
      /// \event Patient: Brain Oxygen Deficit Event. The oxygen partial pressure in the brain has dropped to a dangerously low level.
      patient.SetEvent(CDM::enumPatientEvent::BrainOxygenDeficit, true, m_data.GetSimulationTime());

      // Irreversible damage occurs if the deficit has gone on too long
      if (patient.GetEventDuration(CDM::enumPatientEvent::BrainOxygenDeficit, TimeUnit::s) > 1800) {
        m_ss << "Brain Oxygen partial pressure is " << m_brainO2->GetPartialPressure(PressureUnit::mmHg) << " and has been below the danger threshold for " << patient.GetEventDuration(CDM::enumPatientEvent::BrainOxygenDeficit, TimeUnit::s) << " seconds. Damage is irreversible.";
        Warning(m_ss);
        /// \irreversible Brain oxygen pressure has been dangerously low for more than 30 minutes.
        patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
      }

      // If the O2 tension is below a critical threshold, the irreversible damage occurs more quickly
      if (m_brainO2->GetPartialPressure(PressureUnit::mmHg) < 10.0) {
        /// \event Patient: Critical Brain Oxygen Deficit Event. The oxygen partial pressure in the brain has dropped to a critically low level.
        patient.SetEvent(CDM::enumPatientEvent::CriticalBrainOxygenDeficit, true, m_data.GetSimulationTime());
      } else if (m_brainO2->GetPartialPressure(PressureUnit::mmHg) > 12.0) {
        /// \event Patient: End Brain Oxygen Deficit Event. The oxygen partial pressure has risen above 12 mmHg in the brain. If this occurs when the patient has a critical brain oxygen deficit event, it will reverse the event.
        /// The brain is not in a critical oxygen deficit.
        patient.SetEvent(CDM::enumPatientEvent::CriticalBrainOxygenDeficit, false, m_data.GetSimulationTime());
      }

      // Irreversible damage occurs if the critical deficit has gone on too long
      if (patient.GetEventDuration(CDM::enumPatientEvent::CriticalBrainOxygenDeficit, TimeUnit::s) > 600) {
        m_ss << "Brain Oxygen partial pressure is " << m_brainO2->GetPartialPressure(PressureUnit::mmHg) << " and has been below the critical threshold for " << patient.GetEventDuration(CDM::enumPatientEvent::BrainOxygenDeficit, TimeUnit::s) << " seconds. Damage is irreversible.";
        Warning(m_ss);
        /// \irreversible Brain oxygen pressure has been critically low for more than 10 minutes.
        patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
      }
    } else if (m_brainO2->GetPartialPressure(PressureUnit::mmHg) > 25.0) {
      /// \event Patient: End Brain Oxygen Deficit Event. The oxygen partial pressure has risen above 25 mmHg in the brain. If this occurs when the patient has a brain oxygen deficit event, it will reverse the event.
      /// The brain is getting oxygen.
      patient.SetEvent(CDM::enumPatientEvent::BrainOxygenDeficit, false, m_data.GetSimulationTime());
      // The critical deficit event is also set to false just in case there is an unrealistically rapid transition in oxygen partial pressure.
      patient.SetEvent(CDM::enumPatientEvent::CriticalBrainOxygenDeficit, false, m_data.GetSimulationTime());
    }

    //Myocardium Oxygen Check
    if (m_myocardiumO2->GetPartialPressure(PressureUnit::mmHg) < 5) {
      /// \event Patient: The heart is not receiving enough oxygen. Coronary arteries should dilate to increase blood flow to the heart.
      patient.SetEvent(CDM::enumPatientEvent::MyocardiumOxygenDeficit, true, m_data.GetSimulationTime());

      if (patient.GetEventDuration(CDM::enumPatientEvent::MyocardiumOxygenDeficit, TimeUnit::s) > 2400) // \cite murry1986preconditioning
      {
        m_ss << "Myocardium oxygen partial pressure is  " << m_myocardiumO2->GetPartialPressure(PressureUnit::mmHg) << " and has been sustained for " << patient.GetEventDuration(CDM::enumPatientEvent::MyocardiumOxygenDeficit, TimeUnit::s) << "patient heart muscle has experienced necrosis and is in an irreversible state.";
        Warning(m_ss);
        /// \irreversible Heart has not been receiving enough oxygen for more than 40 min.
        patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
      }
    } else if (m_myocardiumO2->GetPartialPressure(PressureUnit::mmHg) > 8) {
      /// \event Patient: End Myocardium Oxygen Event. The heart is now receiving enough oxygen. If this occurs when the patient has a heart oxygen deficit event, it will reverse the event.
      /// The brain is getting oxygen.
      patient.SetEvent(CDM::enumPatientEvent::MyocardiumOxygenDeficit, false, m_data.GetSimulationTime());
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets data on the metabolic panel object.
///
/// \details
/// Sets data on the metabolic panel object to create the metabolic panel.
/// Uses information from the chem 14 substances that are in %BioGears (see @ref bloodchemistry-assessments)
//--------------------------------------------------------------------------------------------------
bool BloodChemistry::CalculateComprehensiveMetabolicPanel(SEComprehensiveMetabolicPanel& cmp)
{
  cmp.Reset();
  cmp.GetAlbumin().Set(m_data.GetSubstances().GetAlbumin().GetBloodConcentration());
  //cmp.GetALP().SetValue();
  //cmp.GetALT().SetValue();
  //cmp.GetAST().SetValue();
  cmp.GetBUN().Set(GetBloodUreaNitrogenConcentration());
  cmp.GetCalcium().Set(m_data.GetSubstances().GetCalcium().GetBloodConcentration());
  double CL_mmol_Per_L = m_data.GetSubstances().GetChloride().GetBloodConcentration(MassPerVolumeUnit::g_Per_L) / m_data.GetSubstances().GetChloride().GetMolarMass(MassPerAmountUnit::g_Per_mmol);
  cmp.GetChloride().SetValue(CL_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  // CO2 is predominantly Bicarbonate, so going to put that in this slot
  double HCO3_mmol_Per_L = m_data.GetSubstances().GetBicarbonate().GetBloodConcentration(MassPerVolumeUnit::g_Per_L) / m_data.GetSubstances().GetHCO3().GetMolarMass(MassPerAmountUnit::g_Per_mmol);
  cmp.GetCO2().SetValue(HCO3_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  cmp.GetCreatinine().Set(m_data.GetSubstances().GetCreatinine().GetBloodConcentration());
  cmp.GetGlucose().Set(m_data.GetSubstances().GetGlucose().GetBloodConcentration());
  double K_mmol_Per_L = m_data.GetSubstances().GetPotassium().GetBloodConcentration(MassPerVolumeUnit::g_Per_L) / m_data.GetSubstances().GetPotassium().GetMolarMass(MassPerAmountUnit::g_Per_mmol);
  cmp.GetPotassium().SetValue(K_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  double Sodium_mmol_Per_L = m_data.GetSubstances().GetSodium().GetBloodConcentration(MassPerVolumeUnit::g_Per_L) / m_data.GetSubstances().GetSodium().GetMolarMass(MassPerAmountUnit::g_Per_mmol);
  cmp.GetSodium().SetValue(Sodium_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  //cmp.GetTotalBelirubin().SetValue();
  cmp.GetTotalProtein().Set(GetTotalProteinConcentration());
  return true;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets data on the complete blood count object.
///
/// \details
/// Sets data on the complete blood count object to create the [CBC](@ref bloodchemistry-assessments).
//--------------------------------------------------------------------------------------------------
bool BloodChemistry::CalculateCompleteBloodCount(SECompleteBloodCount& cbc)
{
  cbc.Reset();
  cbc.GetHematocrit().Set(GetHematocrit());
  cbc.GetHemoglobin().Set(m_data.GetSubstances().GetHb().GetBloodConcentration());
  cbc.GetPlateletCount().SetValue(325000, AmountPerVolumeUnit::ct_Per_uL); // Hardcoded for now, don't support PlateletCount yet
  cbc.GetMeanCorpuscularHemoglobin().SetValue(m_data.GetConfiguration().GetMeanCorpuscularHemoglobin(MassPerAmountUnit::pg_Per_ct), MassPerAmountUnit::pg_Per_ct);
  cbc.GetMeanCorpuscularHemoglobinConcentration().SetValue(m_data.GetSubstances().GetHb().GetBloodConcentration(MassPerVolumeUnit::g_Per_dL) / GetHematocrit().GetValue(), MassPerVolumeUnit::g_Per_dL); //Average range should be 32-36 g/dL. (https://en.wikipedia.org/wiki/Mean_corpuscular_hemoglobin_concentration)
  cbc.GetMeanCorpuscularVolume().SetValue(m_data.GetConfiguration().GetMeanCorpuscularVolume(VolumeUnit::uL), VolumeUnit::uL);
  cbc.GetRedBloodCellCount().Set(GetRedBloodCellCount());
  cbc.GetWhiteBloodCellCount().Set(GetWhiteBloodCellCount());
  return true;
}

void BloodChemistry::Sepsis()
{
  if (!m_PatientActions->HasSepsis()) {
    return;
  }

  SESepsis* sep = m_PatientActions->GetSepsis();
  std::map<std::string, std::string> tissueResistors = sep->GetTissueResistorMap();
  std::string sepComp = sep->GetCompartment() + "Tissue";
  SEThermalCircuitPath* coreCompliance = m_data.GetCircuits().GetInternalTemperatureCircuit().GetPath(BGE::InternalTemperaturePath::InternalCoreToGround);

  //Get state parameter values
  double pathogen = sep->GetPathogen().GetValue();
  double whiteBloodCell = sep->GetEarlyMediator().GetValue();
  double lateMediator = sep->GetLateMediator().GetValue();
  double antibiotic_g = m_data.GetDrugs().GetAntibioticMassInBody(MassUnit::g);

  //Calculate pathogen activation of early immune response from severity (totally empirical relationship)
  double logKmp = 0.4618 * exp(2.4525 * sep->GetSeverity().GetValue());
  double wbcFractionMax = 0.0387 * logKmp + 0.7418; //We need to scale response as function of maximum possible white blood cell value
  double kmp_Per_hr = exp(logKmp);

  //Constants needed for Kumar Model
  double kp_Per_hr = 3.0; //Pathogen growth rate
  double minimumInhibitory_g = 0.75; //Antibiotic threshold mass to have an effect on pathogen
  double kpm_Per_hr = 3.0; //Pathogen susceptibility to immune response
  double klm_Per_hr = 15.0; //Activation rate of late immune response
  double kl_Per_hr = 1.0; //Immune response decay rate
  double activationWidth = 0.5; //Attenuates coupling function between early and late immune response
  double activationThreshold = 1.25; //Level of initial immune response that triggers late immune response
  double dt_hr = m_data.GetTimeStep().GetValue(TimeUnit::hr);
  double wbcBaseline_ct_Per_uL = 7000.0;
  double wbcFractionInitial = 0.05;

  //Other values we need to evaluate the system
  double couplingFunction = 1.0 + tanh((whiteBloodCell - activationThreshold) / activationWidth);
  double effectiveAntibiotic_g = 0;
  if (antibiotic_g > minimumInhibitory_g)
    effectiveAntibiotic_g = (antibiotic_g - minimumInhibitory_g) / sep->GetSeverity().GetValue();

  //Calculate rates of change using Kumar model (@ cite Kumar2004Dynamics)
  double dPathogen_Per_hr = kp_Per_hr * pathogen * (1.0 - pathogen) / (1 + effectiveAntibiotic_g) - kpm_Per_hr * whiteBloodCell * pathogen;
  double dWhiteBloodCell_Per_hr = (kmp_Per_hr * pathogen + lateMediator) * whiteBloodCell * (1.0 - whiteBloodCell) - whiteBloodCell;
  double dLateMediator_Per_hr = klm_Per_hr * couplingFunction - kl_Per_hr * lateMediator;

  /*Adjust model to account for presence of antibiotics.  This is very hand-wavy to get behavior desired by AMM.  If for some reason they (or someone
		else) want to increase/decrease amount of time for return to normal function, first adjust the minimum inhibitory concentration (higher for longer
		recovery, lower for shorter recovery). If still not satisfactory, adjust decay of late mediator in presence of antibiotic (below).  But this
		second knob is pretty sensitive.
		*/
  double nextPathogen = pathogen + dPathogen_Per_hr;
  if ((nextPathogen > pathogen) && antibiotic_g > 0) {
    //This means that we're getting into some oscillatory behavior after antibiotic administration (i.e. we hit a minimum pathogen concentration)
    //Stop pathogen growth and allow late mediators to decay.  This will start to bring white blood cells back to normal
    dPathogen_Per_hr = 0.0;
    dLateMediator_Per_hr = -0.25 * sep->GetSeverity().GetValue(); //Scale with severity because low-severity scenarios have lower max wbc count
  }
  if (lateMediator <= 0) {
    //Because of monkeying around above we might get a negative late mediator value.  Don't let this happen
    dLateMediator_Per_hr = 0.0;
  }

  sep->GetPathogen().IncrementValue(dPathogen_Per_hr * dt_hr);
  sep->GetEarlyMediator().IncrementValue(dWhiteBloodCell_Per_hr * dt_hr);
  sep->GetLateMediator().IncrementValue(dLateMediator_Per_hr * dt_hr);

  if (sep->GetEarlyMediator().GetValue() < wbcFractionInitial) {
    sep->GetSeverity().SetValue(0.0); //Our White blood cell count has return to normal.  This will cause the Sepsis action to be inactivated
  }

  //Use the change in white blood cell count to scale down the resistance of the vascular -> tissue paths
  //The circuit needs continuous values to solve, so we cannot change reflection coefficient values (which are mapped to oncotic pressure
  //sources) to arbitrary values.  Thus we use the linear interpolator to move coefficients from initial value of 1.0 as function of
  //white blood cell fraction.
  SEFluidCircuitPath* vascularLeak = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(tissueResistors[sepComp]);
  double resistanceBase_mmHg_s_Per_mL = vascularLeak->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
  double nextResistance_mmHg_s_Per_mL = resistanceBase_mmHg_s_Per_mL * std::pow(10.0, -10 * (whiteBloodCell - wbcFractionInitial));
  vascularLeak->GetNextResistance().SetValue(nextResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  double nextReflectionCoefficient = GeneralMath::LinearInterpolator(wbcFractionInitial, wbcFractionMax, 1.0, 0.05, whiteBloodCell);
  BLIM(nextReflectionCoefficient, 0.0, 1.0); //Make sure the interpolator doesn't extrapolate a bad value and give us a fraction outside range [0,1]
  m_data.GetCompartments().GetTissueCompartment(sep->GetCompartment() + "Tissue")->GetReflectionCoefficient().SetValue(nextReflectionCoefficient);

  //Adjust the other tissue resistances and permeabilities as sepsis worsens
  //Do this in two stages so that blood volume loss doesn't drop off all at once but is more of a progression
  double severeThreshold = 0.35 * wbcFractionMax;
  double modsThreshold = 0.85 * wbcFractionMax;

  for (auto comp : m_data.GetCompartments().GetTissueLeafCompartments()) {
    if (comp->GetName().compare(BGE::TissueCompartment::Brain) == 0 || comp->GetName().compare(sepComp) == 0)
      continue; //Don't mess with the brain and don't repeat the compartment the infection started in
    if (whiteBloodCell > severeThreshold && whiteBloodCell <= modsThreshold) {
      nextReflectionCoefficient = GeneralMath::LinearInterpolator(severeThreshold, modsThreshold, 1.0, 0.5, whiteBloodCell);
      BLIM(nextReflectionCoefficient, 0.0, 1.0);
      comp->GetReflectionCoefficient().SetValue(nextReflectionCoefficient);
      vascularLeak = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(tissueResistors[comp->GetName()]);
      resistanceBase_mmHg_s_Per_mL = vascularLeak->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
      nextResistance_mmHg_s_Per_mL = resistanceBase_mmHg_s_Per_mL * std::pow(10.0, -10 * (whiteBloodCell - modsThreshold));
      vascularLeak->GetNextResistance().SetValue(nextResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);

    } else if (whiteBloodCell > modsThreshold) {
      nextReflectionCoefficient = GeneralMath::LinearInterpolator(modsThreshold, wbcFractionMax, 0.5, 0.05, whiteBloodCell);
      BLIM(nextReflectionCoefficient, 0.0, 1.0);
      comp->GetReflectionCoefficient().SetValue(nextReflectionCoefficient);
      vascularLeak = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(tissueResistors[comp->GetName()]);
      resistanceBase_mmHg_s_Per_mL = vascularLeak->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
      nextResistance_mmHg_s_Per_mL = resistanceBase_mmHg_s_Per_mL * std::pow(10.0, -10 * (whiteBloodCell - modsThreshold));
      vascularLeak->GetNextResistance().SetValue(nextResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    }
  }

  //Set pathological effects, starting with updating white blood cell count.  Scaled down to get max levels around 25-30k ct_Per_uL
  double wbcAbsolute_ct_Per_uL = wbcBaseline_ct_Per_uL * (1.0 + (sep->GetEarlyMediator().GetValue() - wbcFractionInitial) / (5.0 * wbcFractionInitial));
  GetWhiteBloodCellCount().SetValue(wbcAbsolute_ct_Per_uL, AmountPerVolumeUnit::ct_Per_uL);

  //Use the delta above normal white blood cell values to track other Systemic Inflammatory metrics.  These relationships were all
  //empirically determined to time symptom onset (i.e. temperature > 38 degC) with the appropriate stage of sepsis
  double sigmoidInput = whiteBloodCell - wbcFractionInitial;

  //Respiration effects--Done in Respiratory system

  //Temperature (fever) effects
  double coreTempComplianceBaseline_J_Per_K = coreCompliance->GetCapacitanceBaseline(HeatCapacitanceUnit::J_Per_K);
  double coreComplianceDeltaPercent = sigmoidInput / (sigmoidInput + 0.15); //was *0.75, then *0.85 for max value
  coreCompliance->GetNextCapacitance().SetValue(coreTempComplianceBaseline_J_Per_K * (1.0 - coreComplianceDeltaPercent / 100.0), HeatCapacitanceUnit::J_Per_K);

  //Blood pressure effects (accomplish by dulling baroreceptor resistance scale)
  double baroreceptorScaleDelta = 0.6 * sigmoidInput / (sigmoidInput + 0.25);
  m_data.GetNervous().GetBaroreceptorResistanceScale().SetValue(1.0 - baroreceptorScaleDelta);

  //Bilirubin counts (measure of liver perfusion)
  double baselineBilirubin_mg_Per_dL = 0.70;
  double maxBilirubin_mg_Per_dL = 26.0; //Not a physiologal max, but Jones2009Sequential (SOFA score) gives max score when total bilirubin > 12 mg/dL
  double halfMaxWBC = wbcFractionMax; //White blood cell fraction that causes half-max bilirubin concentration.  Set above 0.5 because bilirubin is a later sign of shock
  double shapeParam = 10.0; //Empirically determined to make sure we get above 12 mg/dL (severe liver damage) before wbc maxes out
  double totalBilirubin_mg_Per_dL = GeneralMath::LogisticFunction(maxBilirubin_mg_Per_dL, halfMaxWBC, shapeParam, sigmoidInput) + baselineBilirubin_mg_Per_dL;
  GetTotalBilirubin().SetValue(totalBilirubin_mg_Per_dL, MassPerVolumeUnit::mg_Per_dL);
}
}