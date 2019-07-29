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
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
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
  GetInflammatoryResponse().Initialize();
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
  m_PatientActions = &m_data.GetActions().GetPatientActions();
  m_Patient = &m_data.GetPatient();

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

  m_InflammatoryResponse = &GetInflammatoryResponse();

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
  InflammatoryResponse();
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
  GetOxygenVenousSaturation().Set(m_venaCavaO2->GetSaturation());
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

  double otherIons_mmol_Per_L = -5.4; //Na, K, and Cl baseline concentrations give SID = 45.83, we assume baseline SID = 40.5, thus "other ions" (i.e. Mg, Ca, Ketones) make up -5.3 mmol_Per_L equivalent of charge
  double strongIonDifference_mmol_Per_L = m_venaCavaSodium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + m_venaCavaPotassium->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) - (m_venaCavaChloride->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + (m_venaCavaLactate->GetMolarity(AmountPerVolumeUnit::mmol_Per_L))) + otherIons_mmol_Per_L;
  //GetStrongIonDifference().SetValue(strongIonDifference_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);

  // Calculate pH
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
  if (m_data.GetActions().GetPatientActions().HasOverride()
      && m_data.GetState() == EngineState::Active) {
    if (m_data.GetActions().GetPatientActions().GetOverride()->HasBloodChemistryOverride()) {
      ProcessOverride();
    }
  }
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
          if (!m_PatientActions->HasOverride()) {
            patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
          } else {
            if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
              patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
            }
          }
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
          if (!m_PatientActions->HasOverride()) {
            patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
          } else {
            if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
              patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
            }
          }
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
        if (!m_PatientActions->HasOverride()) {
          patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        } else {
          if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
            patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
          }
        }
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
        if (!m_PatientActions->HasOverride()) {
          patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        } else {
          if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
            patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
          }
        }
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
        if (!m_PatientActions->HasOverride()) {
          patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        } else {
          if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
            patient.SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
          }
        }
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

  //We need to signal that there is a source of inflammation--soon this will be incorporated with Acute Inflammation Model
  //Right now we are patching in Sepsis functionality and saving merge for a future story
  SEThermalCircuitPath* coreCompliance = m_data.GetCircuits().GetInternalTemperatureCircuit().GetPath(BGE::InternalTemperaturePath::InternalCoreToGround);

  //Physiological response
  double wbcBaseline_ct_Per_uL = 7000.0;
  double tissueIntegrity = m_InflammatoryResponse->GetTissueIntegrity().GetValue();
  double neutrophilActive = m_InflammatoryResponse->GetNeutrophilActive().GetValue();

  //Set pathological effects, starting with updating white blood cell count.  Scaled down to get max levels around 25-30k ct_Per_uL
  double wbcAbsolute_ct_Per_uL = wbcBaseline_ct_Per_uL * (1.0 + neutrophilActive / 0.25);
  GetWhiteBloodCellCount().SetValue(wbcAbsolute_ct_Per_uL, AmountPerVolumeUnit::ct_Per_uL);

  //Use the delta above normal white blood cell values to track other Systemic Inflammatory metrics.  These relationships were all
  //empirically determined to time symptom onset (i.e. temperature > 38 degC) with the appropriate stage of sepsis
  double sigmoidInput = 1.0 - tissueIntegrity;

  //Respiration effects--Done in Respiratory system

  //Temperature (fever) effects
  double coreTempComplianceBaseline_J_Per_K = coreCompliance->GetCapacitanceBaseline(HeatCapacitanceUnit::J_Per_K);
  double coreComplianceDeltaPercent = sigmoidInput / (sigmoidInput + 0.4);
  coreCompliance->GetNextCapacitance().SetValue(coreTempComplianceBaseline_J_Per_K * (1.0 - coreComplianceDeltaPercent / 100.0), HeatCapacitanceUnit::J_Per_K);

  //Bilirubin counts (measure of liver perfusion)
  double baselineBilirubin_mg_Per_dL = 0.70;
  double maxBilirubin_mg_Per_dL = 26.0; //Not a physiologal max, but Jones2009Sequential (SOFA score) gives max score when total bilirubin > 12 mg/dL
  double halfMaxWBC = 0.85; //White blood cell fraction that causes half-max bilirubin concentration.  Set well above 0.5 becuase this is a later sign of shock
  double shapeParam = 10.0; //Empirically determined to make sure we get above 12 mg/dL (severe liver damage) before wbc maxes out
  double totalBilirubin_mg_Per_dL = GeneralMath::LogisticFunction(maxBilirubin_mg_Per_dL, halfMaxWBC, shapeParam, sigmoidInput) + baselineBilirubin_mg_Per_dL;
  GetTotalBilirubin().SetValue(totalBilirubin_mg_Per_dL, MassPerVolumeUnit::mg_Per_dL);
}

//void BloodChemistry::AcuteInflammatoryResponse()
//{
//  //Handle all inflammation actions here instead of creating one action for each.  We can filter based on "InflammationSource"
//  std::vector<CDM::enumInflammationSource> sources = m_InflammatoryResponse->GetInflammationSources();
//  //Values that we will change depending on what the source of the inflammation is
//  double scaleFactor = 1.0; //Scale Factor to speed up during debug/testing (and probably for some scenarios)
//  double burnTotalBodySurfaceArea = 0.0;
//  double pathogenGrowthRate = 0.0; //Note bifurcation at approximately kpg = 1.6
//  double damageRecovery = 0.0;
//
//  if (m_data.GetActions().GetPatientActions().HasBurnWound()) {
//    burnTotalBodySurfaceArea = m_data.GetActions().GetPatientActions().GetBurnWound()->GetTotalBodySurfaceArea().GetValue();
//    scaleFactor = 1.0;
//    damageRecovery = 0.01;
//    if (std::find(sources.begin(), sources.end(), CDM::enumInflammationSource::Burn) == sources.end()) {
//      GetAcuteInflammatoryResponse().GetTrauma().SetValue(burnTotalBodySurfaceArea); //This causes inflammatory mediators (particulalary IL-6) to peak around 4 hrs at levels similar to those induced by pathogen
//      GetAcuteInflammatoryResponse().GetInflammationSources().push_back(CDM::enumInflammationSource::Burn);
//    }
//  }
//  if (m_data.GetActions().GetPatientActions().HasSepsis()) {
//    pathogenGrowthRate = 3.5;
//    scaleFactor = 5.0 * m_data.GetActions().GetPatientActions().GetSepsis()->GetSeverity().GetValue();
//    damageRecovery = 0.05;
//    if (std::find(sources.begin(), sources.end(), CDM::enumInflammationSource::Pathogen) == sources.end()) {
//      GetAcuteInflammatoryResponse().GetPathogen().SetValue(1.0);
//      GetAcuteInflammatoryResponse().GetInflammationSources().push_back(CDM::enumInflammationSource::Pathogen);
//    }
//    Sepsis(); //Process additional sepsis effects
//    //Check for presence of antibiotic and scale pathogen growth accordingly
//    if (m_data.GetSubstances().GetSubstance("Piperacillin")->HasPlasmaConcentration()) {
//      double minimumInhibitoryConcentration_ug_Per_mL = 16.0;
//      double antibioticEMax = 3.0;
//      double antibioticShapeParam = 2.0;
//      double antibioticEC50 = 3.0;
//      double piperacillin_ug_Per_mL = m_data.GetSubstances().GetSubstance("Piperacillin")->GetPlasmaConcentration(MassPerVolumeUnit::ug_Per_mL);
//      double antibioticEffect_Per_h = antibioticEMax * std::pow(piperacillin_ug_Per_mL / minimumInhibitoryConcentration_ug_Per_mL, antibioticShapeParam) / (std::pow(antibioticEC50, antibioticShapeParam) + std::pow(piperacillin_ug_Per_mL / minimumInhibitoryConcentration_ug_Per_mL, antibioticShapeParam));
//      pathogenGrowthRate = pathogenGrowthRate - antibioticEffect_Per_h;
//    }
//  }
//  //Perform this check after looking for inflammatory actions (otherwise we'll never process)
//  if (!GetAcuteInflammatoryResponse().HasInflammationSources()) {
//    return;
//  }
//  //Unit scales--TNF, nitrate, IL6, and IL10 are scaled in Chow2005Acute so that there outputs are in units of pg/L.
//  //We will store them with those units (for outputs), but they need to be scaled back down to dimensionless values to use in model equations
//  double tnfScale = 35000.0;
//  double nitrateScale = 1000.0;
//  double il6Scale = 17000.0;
//  double il10Scale = 8000.0;
//
//  //Get inflammation state variables
//  double pathogen = GetAcuteInflammatoryResponse().GetPathogen().GetValue();
//  double trauma = GetAcuteInflammatoryResponse().GetTrauma().GetValue();
//  double macrophageResting = GetAcuteInflammatoryResponse().GetMacrophageResting().GetValue();
//  double macrophageActive = GetAcuteInflammatoryResponse().GetMacrophageActive().GetValue();
//  double neutrophilResting = GetAcuteInflammatoryResponse().GetNeutrophilResting().GetValue();
//  double neutrophilActive = GetAcuteInflammatoryResponse().GetNeutrophilActive().GetValue();
//  double iNOS = GetAcuteInflammatoryResponse().GetInducibleNOSynthase().GetValue();
//  double iNOSpre = GetAcuteInflammatoryResponse().GetInducibleNOSynthasePre().GetValue();
//  double eNOS = GetAcuteInflammatoryResponse().GetConstitutiveNOSynthase().GetValue();
//  double nitricOxide = GetAcuteInflammatoryResponse().GetNitricOxide().GetValue();
//  double NO3 = GetAcuteInflammatoryResponse().GetNitrate().GetValue() / nitrateScale;
//  double TNF = GetAcuteInflammatoryResponse().GetTumorNecrosisFactor().GetValue() / tnfScale;
//  double IL6 = GetAcuteInflammatoryResponse().GetInterleukin6().GetValue() / il6Scale;
//  double IL10 = GetAcuteInflammatoryResponse().GetInterleukin10().GetValue() / il10Scale;
//  double IL12 = GetAcuteInflammatoryResponse().GetInterleukin12().GetValue();
//  double catecholamines = GetAcuteInflammatoryResponse().GetCatecholamines().GetValue();
//  double tissueIntegrity = GetAcuteInflammatoryResponse().GetTissueIntegrity().GetValue();
//
//  //Time step
//  double dt_hr = m_data.GetTimeStep().GetValue(TimeUnit::hr);
//
//  //Non-serialized variables
//  double fB = 0.0;
//  double autonomic = 0.0;
//
//  //Model Parameters
//  //Source terms
//  double sM = 1.0, sN = 1.0, s6 = 0.001, s10 = 0.01;
//  //Pathogen parameters
//  double kPG = pathogenGrowthRate;
//  double kPN = 4.0; //Phagocytic effect of activated neutrophils on pathogen, determined empirically
//  double sB = 0.0075; //Source of non-specific immune response
//  double kPB = 0.461; //Rate that non-specific response eliminates pathogen
//  double uB = 0.0023; //Decay rate of non-specific immune response
//  double kBP = 0.0001; //Rate at which non-specific response exhausted by pathogen
//  double maxPathogen = 20.0; //Maximum pathogen population size
//  //Trauma decay
//  double kTr = 0.85; //Determined empirically to give good results
//  //Macrophage interaction
//  double kML = 1.01, kMTR = 0.04, kM6 = 0.1, kMB = 0.0495, kMR = 0.05, kMD = 0.05, xML = 10.0, xMD = 1.0, xMTNF = 0.4, xM6 = 1.0, xM10 = 0.297, xMCA = 0.9; //Note xMD was 1.0 for burn, see if this messes things up
//  //Activate macrophage interactions
//  double kMANO = 0.2, kMA = 0.2;
//  //Neutrophil interactions
//  double kNL = 0.15, kNTNF = 0.2, kN6 = 0.557, kNB = 0.1, kND = 0.05, kNTR = 0.02, kNTGF = 0.1, kNR = 0.05, kNNO = 0.4, kNA = 0.5, xNL = 15.0, xNTNF = 2.0, xN6 = 1.0, xND = 0.4, xN10 = 0.2, xNNO = 0.5; //xND was 0.4 for burn
//  //Inducible NOS
//  double kINOSN = 1.5, kINOSM = 0.1, kINOSEC = 0.1, kINOS6 = 2.0, kINOSd = 0.05, kINOS = 0.101, xINOS10 = 0.1, xINOSTNF = 0.05, xINOS6 = 0.1, xINOSNO = 0.3;
//  //E NOS
//  double kENOS = 4.0, kENOSEC = 0.05, xENOSTNF = 0.4, xENOSL = 1.015, xENOSTR = 0.1;
//  //Nitric oxide
//  double kN = 0.5, kNO3 = 0.46, kNOMA = 2.0;
//  //TNF
//  double kTNFN = 2.97, kTNFM = 0.1, kTNF = 1.4, xTNF6 = 0.059, xTNF10 = 0.079;
//  //IL6
//  double k6M = 3.03, k6TNF = 1.0, k62 = 3.4, k6NO = 2.97, k6 = 0.7, k6N = 0.2, x610 = 0.1782, x6TNF = 0.1, x66 = 0.2277, x6NO = 0.4;
//  //IL10
//  double k10MA = 0.1, k10N = 0.1, k10A = 62.87, k10TNF = 1.485, k106 = 0.051, k10 = 0.35, k10R = 0.1, x10TNF = 0.05, x1012 = 0.049, x106 = 0.08;
//  //CA
//  double kCA = 0.1, kCATR = 0.16;
//  //IL12
//  double k12M = 0.303, k12 = 0.05, x12TNF = 0.2, x126 = 0.2, x1210 = 0.2525;
//  //Blood pressure
//  double kB = 4.0, kBNO = 0.2, xBNO = 0.05;
//  //Damage --- changed kDB from 0.02, changed xD6 from 0.25, changed kDTR from 0.05,
//  double kDB = 0.005, kD6 = 0.3, kD = damageRecovery, xD6 = 0.25, xDNO = 0.5;
//  double kDTR = 0.0; //This is a base value that will be adjusted as a function of type and severity of trauma
//  double kDP = 0.001; //Pathogen causes a small amount of damage by itself
//  //Temperature parameters
//  double kT = 1.0, kTTnf = 1.5, nTTnf = 0.2, hTTnf = 0.75, TMax = 39.5, TMin = 37.0, kT6 = 1.5, nT6 = 0.5, hT6 = 0.75, kT10 = 0.0625, nT10 = 0.2, hT10 = 1.0;
//  //Heart rate parameters
//  double kH = 0.2, nHT = 1.0, hHT = 2.0, HMax = 192.0, HBase = 72.0, tau2 = 0.003;
//  //Pain threshold parameters
//  double kPTP = 0.025, kPT = 0.011, PTM = 1.0;
//
//  //Adjust parameters depending on inflammation source
//  if (burnTotalBodySurfaceArea != 0) {
//    //Rate at which burn causes damage varies on the severity of the burn.  A larger burn causes a bigger initial hit to tissue damage
//    kDTR = 5.0 * burnTotalBodySurfaceArea;
//  }
//
//  double dPathogen = kPG * pathogen * (1.0 - pathogen / maxPathogen) - pathogen * kPN * neutrophilActive - sB * kPB * pathogen / (uB + kBP * pathogen); //This is assumed to be the driving force for infection / sepsis.
//  if (m_data.GetSubstances().GetSubstance("Piperacillin")->HasPlasmaConcentration() && pathogen < 0.001) {
//    //Since pathogen decreases exponentially it will never actually hit 0.  Make sure it can't rebound when population becomes 0.1% of initial pop
//    dPathogen = 0.0;
//  }
//  double dTrauma = -kTr * trauma; //This is assumed to be the driving force for burn
//  double dMacrophageResting = -((kML * std::pow(xML, 2.0) * GeneralMath::HillActivation(pathogen, xML, 2.0) + kMD * GeneralMath::HillActivation(1.0 - tissueIntegrity, xMD, 4.0)) * (GeneralMath::HillActivation(TNF, xMTNF, 2.0) + kM6 * GeneralMath::HillActivation(IL6, xM6, 2.0)) + kMTR * trauma + kMB * fB) * macrophageResting * GeneralMath::HillInhibition(IL10 + catecholamines, xM10, 2.0) - kMR * (macrophageResting - sM);
//  double dMacrophageActive = ((kML * std::pow(xML, 2.0) * GeneralMath::HillActivation(pathogen, xML, 2.0) + kMD * GeneralMath::HillActivation(1.0 - tissueIntegrity, xMD, 4.0)) * (GeneralMath::HillActivation(TNF, xMTNF, 2.0) + kM6 * GeneralMath::HillActivation(IL6, xM6, 2.0)) + kMTR * trauma + kMB * fB) * macrophageResting * GeneralMath::HillInhibition(IL10 + catecholamines, xM10, 2.0) - kMA * macrophageActive;
//  double dNeutrophilResting = -(kNL * xNL * GeneralMath::HillActivation(pathogen, xNL, 1.0) + kNTNF * xNTNF * GeneralMath::HillActivation(TNF, xNTNF, 1.0) + kN6 * std::pow(xN6, 2.0) * GeneralMath::HillActivation(IL6, xN6, 2.0) + kND * std::pow(xND, 2.0) * GeneralMath::HillActivation(1.0 - tissueIntegrity, xND, 2.0) + kNB * fB * kNTR * trauma) * GeneralMath::HillInhibition(IL10 + catecholamines, xN10, 2.0) * neutrophilResting - kNR * (neutrophilResting - sN);
//  double dNeutrophilActive = (kNL * xNL * GeneralMath::HillActivation(pathogen, xNL, 1.0) + kNTNF * xNTNF * GeneralMath::HillActivation(TNF, xNTNF, 1.0) + kN6 * std::pow(xN6, 2.0) * GeneralMath::HillActivation(IL6, xN6, 2.0) + kND * std::pow(xND, 2.0) * GeneralMath::HillActivation(1.0 - tissueIntegrity, xND, 2.0) + kNB * fB + kNTR * trauma) * GeneralMath::HillInhibition(IL10 + catecholamines, xN10, 2.0) * neutrophilResting - kNA * neutrophilActive;
//  double dINOS = kINOS * (iNOSpre - iNOS);
//  double dINOSpre = (kINOSN * neutrophilActive + kINOSM * macrophageActive + kINOSEC * (std::pow(xINOSTNF, 2.0) * GeneralMath::HillActivation(TNF, xINOSTNF, 2.0) + kINOS6 * std::pow(xINOS6, 2.0) * GeneralMath::HillActivation(IL6, xINOS6, 2.0))) * GeneralMath::HillInhibition(IL10, xINOS10, 2.0) * GeneralMath::HillInhibition(nitricOxide, xINOSNO, 2.0) - kINOSd * iNOSpre;
//  double dENOS = kENOSEC * GeneralMath::HillInhibition(TNF, xENOSTNF, 1.0) * GeneralMath::HillInhibition(pathogen, xENOSL, 1.0) * GeneralMath::HillInhibition(trauma, xENOSTR, 4.0) - kENOS * eNOS;
//  double dNO3 = kNO3 * (nitricOxide - NO3);
//  double dTNF = (kTNFN * neutrophilActive + kTNFM * macrophageActive) * GeneralMath::HillInhibition(IL10 + catecholamines, xTNF10, 2.0) * GeneralMath::HillInhibition(IL6, xTNF6, 3.0) - kTNF * TNF;
//  double dIL6 = (k6N * neutrophilActive + macrophageActive) * (k6M + k6TNF * GeneralMath::HillActivation(TNF, x6TNF, 2.0) + k6NO * GeneralMath::HillActivation(nitricOxide, x6NO, 2.0)) * GeneralMath::HillInhibition(IL10 + catecholamines, x610, 2.0) * GeneralMath::HillInhibition(IL6, x66, 4.0) + k6 * (s6 - IL6);
//  double dIL10 = (k10N * neutrophilActive + macrophageActive * (1 + k10A * autonomic)) * (k10MA + k10TNF * GeneralMath::HillActivation(TNF, x10TNF, 4.0) + k106 * GeneralMath::HillActivation(IL6, x106, 4.0)) * ((1 - k10R) * GeneralMath::HillInhibition(IL12, x1012, 4.0) + k10R) - k10 * (IL10 - s10);
//  double dIL12 = k12M * macrophageActive * GeneralMath::HillInhibition(IL10, x1210, 2.0) - k12 * IL12;
//  double dCa = kCATR * autonomic - kCA * catecholamines;
//  double dTissueIntegrity = kD * (1.0 - tissueIntegrity) - tissueIntegrity * (kDB * fB + kD6 * GeneralMath::HillActivation(IL6, xD6, 2.0) + kDTR * trauma + kDP * pathogen) * (1.0 / (std::pow(xDNO, 2.0) + std::pow(nitricOxide, 2.0)));
//
//  //Increment state values--make sure to scale nitrate, tnf, il6, and il10 back up
//  GetAcuteInflammatoryResponse().GetPathogen().IncrementValue(dPathogen * dt_hr * scaleFactor);
//  GetAcuteInflammatoryResponse().GetTrauma().IncrementValue(dTrauma * dt_hr * scaleFactor);
//  GetAcuteInflammatoryResponse().GetMacrophageResting().IncrementValue(dMacrophageResting * dt_hr * scaleFactor);
//  GetAcuteInflammatoryResponse().GetMacrophageActive().IncrementValue(dMacrophageActive * dt_hr * scaleFactor);
//  GetAcuteInflammatoryResponse().GetNeutrophilResting().IncrementValue(dNeutrophilResting * dt_hr * scaleFactor);
//  GetAcuteInflammatoryResponse().GetNeutrophilActive().IncrementValue(dNeutrophilActive * dt_hr * scaleFactor);
//  GetAcuteInflammatoryResponse().GetInducibleNOSynthase().IncrementValue(dINOS * dt_hr * scaleFactor);
//  GetAcuteInflammatoryResponse().GetInducibleNOSynthasePre().IncrementValue(dINOSpre * dt_hr * scaleFactor);
//  GetAcuteInflammatoryResponse().GetConstitutiveNOSynthase().IncrementValue(dENOS * dt_hr * scaleFactor);
//  GetAcuteInflammatoryResponse().GetNitrate().IncrementValue(dNO3 * dt_hr * scaleFactor * nitrateScale);
//  GetAcuteInflammatoryResponse().GetTumorNecrosisFactor().IncrementValue(dTNF * dt_hr * scaleFactor * tnfScale);
//  GetAcuteInflammatoryResponse().GetInterleukin6().IncrementValue(dIL6 * dt_hr * scaleFactor * il6Scale);
//  GetAcuteInflammatoryResponse().GetInterleukin10().IncrementValue(dIL10 * dt_hr * scaleFactor * il10Scale);
//  GetAcuteInflammatoryResponse().GetInterleukin12().IncrementValue(dIL12 * dt_hr * scaleFactor);
//  GetAcuteInflammatoryResponse().GetCatecholamines().IncrementValue(dCa * dt_hr * scaleFactor);
//  GetAcuteInflammatoryResponse().GetTissueIntegrity().IncrementValue(dTissueIntegrity * dt_hr * scaleFactor);
//  //Nitric oxide is an algebraic relationship--update it here using new macrophage and neutrophil values
//  nitricOxide = iNOS * (1.0 + kNOMA * (GetAcuteInflammatoryResponse().GetMacrophageActive().GetValue() + GetAcuteInflammatoryResponse().GetNeutrophilActive().GetValue())) + eNOS;
//  GetAcuteInflammatoryResponse().GetNitricOxide().SetValue(nitricOxide);
//}

//Will replace AcuteInflammatoryResponse function when tested
void BloodChemistry::InflammatoryResponse()
{
  std::vector<CDM::enumInflammationSource> sources = m_InflammatoryResponse->GetInflammationSources();
  double burnTotalBodySurfaceArea = 0.0;
  double damageRecovery = 0.0;

  if (m_data.GetActions().GetPatientActions().HasInfection()) {
    if (std::find(sources.begin(), sources.end(), CDM::enumInflammationSource::Pathogen) == sources.end()) {
      double initialPathogen = 0.0;
      switch (m_data.GetActions().GetPatientActions().GetInfection()->GetSeverity()) {
      case CDM::enumInfectionSeverity::Mild:
        initialPathogen = 0.5;
        break;
      case CDM::enumInfectionSeverity::Moderate:
        initialPathogen = 2.5;
        break;
      case CDM::enumInfectionSeverity::Severe:
        initialPathogen = 10.0;
        break;
      default:
        initialPathogen = 0.2; //Default to very mild infection
      }

      m_InflammatoryResponse->GetPathogen().SetValue(initialPathogen); //This causes inflammatory mediators (particulalary IL-6) to peak around 4 hrs at levels similar to those induced by pathogen
      m_InflammatoryResponse->GetInflammationSources().push_back(CDM::enumInflammationSource::Pathogen);
    }
  }
    if (m_data.GetActions().GetPatientActions().HasBurnWound()) {
      burnTotalBodySurfaceArea = m_data.GetActions().GetPatientActions().GetBurnWound()->GetTotalBodySurfaceArea().GetValue();
      damageRecovery = 0.01;
      if (std::find(sources.begin(), sources.end(), CDM::enumInflammationSource::Burn) == sources.end()) {
       m_InflammatoryResponse->GetTrauma().SetValue(burnTotalBodySurfaceArea); //This causes inflammatory mediators (particulalary IL-6) to peak around 4 hrs at levels similar to those induced by pathogen
       m_InflammatoryResponse->GetInflammationSources().push_back(CDM::enumInflammationSource::Burn);
      }
    }

  //Perform this check after looking for inflammatory actions (otherwise we'll never process)
  if (!m_InflammatoryResponse->HasInflammationSources()) {
    return;
  }

  //Time step
  double dt_hr = m_data.GetTimeStep().GetValue(TimeUnit::hr);
  //Model Parameters
  //Source terms
  double sM = 1.0, sN = 1.0, s6 = 0.001, s10 = 0.01;
  //Pathogen parameters
  double kPG = 1.25;
  double kPN = 5.8; //Phagocytic effect of activated neutrophils on pathogen, determined empirically
  double xPN = 10.0; //Level of pathogen that brings elimination of P by neutrophils to 50% of max
  double sB = 0.0075; //Source of non-specific immune response
  double kPB = 0.461; //Rate that non-specific response eliminates pathogen
  double uB = 0.0023; //Decay rate of non-specific immune response
  double kBP = 0.0001; //Rate at which non-specific response exhausted by pathogen
  double maxPathogen = 100.0; //Maximum pathogen population size
  //Trauma decay
  double kTr = 0.85; //Determined empirically to give good results
  //Macrophage interaction
  double kML = 1.01, kMTR = 0.04, kM6 = 0.1, kMB = 0.0495, kMR = 0.05, kMD = 0.05, xML = 10.0, xMD = 0.5, xMTNF = 0.4, xM6 = 1.0, xM10 = 0.297, xMCA = 0.9; //Note xMD was 1.0 for burn, see if this messes things up
  //Activate macrophage interactions
  double kMANO = 0.2, kMA = 0.2;
  //Neutrophil interactions
  double kNL = 0.15, kNTNF = 0.2, kN6 = 0.557, kNB = 0.1, kND = 0.05, kNTR = 0.02, kNTGF = 0.1, kNR = 0.05, kNNO = 0.4, kNA = 0.5, xNL = 15.0, xNTNF = 2.0, xN6 = 1.0, xND = 0.4, xN10 = 0.2, xNNO = 0.5; //xND was 0.4 for burn
  //Inducible NOS
  double kINOSN = 1.5, kINOSM = 0.1, kINOSEC = 0.1, kINOS6 = 2.0, kINOSd = 0.05, kINOS = 0.101, xINOS10 = 0.1, xINOSTNF = 0.05, xINOS6 = 0.1, xINOSNO = 0.3;
  //E NOS
  double kENOS = 4.0, kENOSEC = 0.05, xENOSTNF = 0.4, xENOSL = 1.015, xENOSTR = 0.1;
  //Nitric oxide
  double kN = 0.5, kNO3 = 0.46, kNOMA = 2.0;
  //TNF
  double kTNFN = 2.97, kTNFM = 0.1, kTNF = 1.4, xTNF6 = 0.059, xTNF10 = 0.079;
  //IL6
  double k6M = 3.03, k6TNF = 1.0, k62 = 3.4, k6NO = 2.97, k6 = 0.7, k6N = 0.2, x610 = 0.1782, x6TNF = 0.1, x66 = 0.5, x6NO = 0.4; //x66 = 0.2277
  //IL10
  double k10MA = 0.1, k10N = 0.1, k10A = 62.87, k10TNF = 1.485, k106 = 0.051, k10 = 0.35, k10R = 0.1, x10TNF = 0.05, x1012 = 0.049, x106 = 0.08;
  //CA
  double kCA = 0.1, kCATR = 0.16;
  //IL12
  double k12M = 0.303, k12 = 0.05, x12TNF = 0.2, x126 = 0.2, x1210 = 0.2525;
  //Blood pressure
  double kB = 4.0, kBNO = 0.2, xBNO = 0.05;
  //Damage --- changed kDB from 0.02, changed xD6 from 0.25, changed kDTR from 0.05,
  double kDB = 0.005, kD6 = 0.4, kD = 0.05, xD6 = 0.5, xDNO = 0.7;
  double kDTR = 0.0; //This is a base value that will be adjusted as a function of type and severity of trauma
  double kDP = 0.001; //Pathogen causes a small amount of damage by itself
  //Containers to hold states
  double P = 0.0, MR = 0.0, MA = 0.0, NR = 0.0, NA = 0.0, ER = 0.0, EA = 0.0, eNOS = 0.0, iNOSd = 0.0, iNOS = 0.0, NO3 = 0.0, NO = 0.0, I6 = 0.0, I10 = 0.0, I12 = 0.0, TNF = 0.0, TI = 0.0, TR = 0.0;
  //Differential containers
  double dP = 0.0, dMR = 0.0, dMA = 0.0, dNR = 0.0, dNA = 0.0, dER = 0.0, dEA = 0.0, dENOS = 0.0, dINOSd = 0.0, dINOS = 0.0, dNO3 = 0.0, dI6 = 0.0, dI10 = 0.0, dI12 = 0.0, dTNF = 0.0, dTI = 0.0, dTR = 0.0;

  //Previous state
  P = m_InflammatoryResponse->GetPathogen().GetValue();
  MR = m_InflammatoryResponse->GetMacrophageResting().GetValue();
  MA = m_InflammatoryResponse->GetMacrophageActive().GetValue();
  NR = m_InflammatoryResponse->GetNeutrophilResting().GetValue();
  NA = m_InflammatoryResponse->GetNeutrophilActive().GetValue();
  eNOS = m_InflammatoryResponse->GetConstitutiveNOS().GetValue();
  iNOSd = m_InflammatoryResponse->GetInducibleNOSPre().GetValue();
  iNOS = m_InflammatoryResponse->GetInducibleNOS().GetValue();
  NO3 = m_InflammatoryResponse->GetNitrate().GetValue();
  NO = m_InflammatoryResponse->GetNitricOxide().GetValue();
  I6 = m_InflammatoryResponse->GetInterleukin6().GetValue();
  I10 = m_InflammatoryResponse->GetInterleukin10().GetValue();
  I12 = m_InflammatoryResponse->GetInterleukin12().GetValue();
  TNF = m_InflammatoryResponse->GetTumorNecrosisFactor().GetValue();
  TI = m_InflammatoryResponse->GetTissueIntegrity().GetValue();
  //Antibiotic effects
  double antibacterialEffect = m_data.GetDrugs().GetAntibioticActivity().GetValue();

  //Process equations
  dP = (kPG-antibacterialEffect) * P * (1.0 - P / maxPathogen) - P * kPN * NA * GeneralMath::HillActivation(P, xPN, 2.0) - sB * kPB * P / (uB + kBP * P); //This is assumed to be the driving force for infection / sepsis.
  if (P < 0.001) {
    //Since pathogen decreases exponentially it will never actually hit 0.  Make sure it can't rebound when population becomes 0.1% of initial pop
    dP = 0.0;
  }
  dTR = -kTr * TR; //This is assumed to be the driving force for burn
  dMR = -((kML * std::pow(xML, 2.0) * GeneralMath::HillActivation(P, xML, 2.0) + kMD * GeneralMath::HillActivation(1.0 - TI, xMD, 4.0)) * (GeneralMath::HillActivation(TNF, xMTNF, 2.0) + kM6 * GeneralMath::HillActivation(I6, xM6, 2.0)) + kMTR * TR) * MR * GeneralMath::HillInhibition(I10, xM10, 2.0) - kMR * (MR - sM);
  dMA = ((kML * std::pow(xML, 2.0) * GeneralMath::HillActivation(P, xML, 2.0) + kMD * GeneralMath::HillActivation(1.0 - TI, xMD, 4.0)) * (GeneralMath::HillActivation(TNF, xMTNF, 2.0) + kM6 * GeneralMath::HillActivation(I6, xM6, 2.0)) + kMTR * TR) * MR * GeneralMath::HillInhibition(I10, xM10, 2.0) - kMA * MA;
  dNR = -(kNL * xNL * GeneralMath::HillActivation(P, xNL, 1.0) + kNTNF * xNTNF * GeneralMath::HillActivation(TNF, xNTNF, 1.0) + kN6 * std::pow(xN6, 2.0) * GeneralMath::HillActivation(I6, xN6, 2.0) + kND * std::pow(xND, 2.0) * GeneralMath::HillActivation(1.0 - TI, xND, 2.0) + kNTR * TR) * GeneralMath::HillInhibition(I10, xN10, 2.0) * NR - kNR * (NR - sN);
  dNA = (kNL * xNL * GeneralMath::HillActivation(P, xNL, 1.0) + kNTNF * xNTNF * GeneralMath::HillActivation(TNF, xNTNF, 1.0) + kN6 * std::pow(xN6, 2.0) * GeneralMath::HillActivation(I6, xN6, 2.0) + kND * std::pow(xND, 2.0) * GeneralMath::HillActivation(1.0 - TI, xND, 2.0) + kNTR * TR) * GeneralMath::HillInhibition(I10, xN10, 2.0) * NR - kNA * NA;
  dINOS = kINOS * (iNOSd - iNOS);
  dINOSd = (kINOSN * NA + kINOSM * MA + kINOSEC * (std::pow(xINOSTNF, 2.0) * GeneralMath::HillActivation(TNF, xINOSTNF, 2.0) + kINOS6 * std::pow(xINOS6, 2.0) * GeneralMath::HillActivation(I6, xINOS6, 2.0))) * GeneralMath::HillInhibition(I10, xINOS10, 2.0) * GeneralMath::HillInhibition(NO, xINOSNO, 2.0) - kINOSd * iNOSd;
  dENOS = kENOSEC * GeneralMath::HillInhibition(TNF, xENOSTNF, 1.0) * GeneralMath::HillInhibition(P, xENOSL, 1.0) * GeneralMath::HillInhibition(TR, xENOSTR, 4.0) - kENOS * eNOS;
  dNO3 = kNO3 * (NO - NO3);
  dTNF = (kTNFN * NA + kTNFM * MA) * GeneralMath::HillInhibition(I10, xTNF10, 2.0) * GeneralMath::HillInhibition(I6, xTNF6, 3.0) - kTNF * TNF;
  dI6 = (k6N * NA + MA) * (k6M + k6TNF * GeneralMath::HillActivation(TNF, x6TNF, 2.0) + k6NO * GeneralMath::HillActivation(NO, x6NO, 2.0)) * GeneralMath::HillInhibition(I10, x610, 2.0) * GeneralMath::HillInhibition(I6, x66, 4.0) + k6 * (s6 - I6);
  dI10 = (k10N * NA + MA) * (k10MA + k10TNF * GeneralMath::HillActivation(TNF, x10TNF, 4.0) + k106 * GeneralMath::HillActivation(I6, x106, 4.0)) * ((1 - k10R) * GeneralMath::HillInhibition(I12, x1012, 4.0) + k10R) - k10 * (I10 - s10);
  dI12 = k12M * MA * GeneralMath::HillInhibition(I10, x1210, 2.0) - k12 * I12;
  dTI += kD * (1.0 - TI) - TI * (kD6 * GeneralMath::HillActivation(I6, xD6, 2.0) + kDTR * TR + kDP * P) * (1.0 / (std::pow(xDNO, 2.0) + std::pow(NO, 2.0)));


  double scale = 60.0;
  //Increment state values--make sure to scale nitrate, tnf, il6, and il10 back up
  m_InflammatoryResponse->GetPathogen().IncrementValue(dP * dt_hr * scale);
  m_InflammatoryResponse->GetTrauma().IncrementValue(dTR * dt_hr * scale);
  m_InflammatoryResponse->GetMacrophageResting().IncrementValue(dMR * dt_hr * scale);
  m_InflammatoryResponse->GetMacrophageActive().IncrementValue(dMA * dt_hr * scale);
  m_InflammatoryResponse->GetNeutrophilResting().IncrementValue(dNR * dt_hr * scale);
  m_InflammatoryResponse->GetNeutrophilActive().IncrementValue(dNA * dt_hr * scale);
  m_InflammatoryResponse->GetInducibleNOS().IncrementValue(dINOS * dt_hr * scale);
  m_InflammatoryResponse->GetInducibleNOSPre().IncrementValue(dINOSd * dt_hr * scale);
  m_InflammatoryResponse->GetConstitutiveNOS().IncrementValue(dENOS * dt_hr * scale);
  m_InflammatoryResponse->GetNitrate().IncrementValue(dNO3 * dt_hr * scale);
  m_InflammatoryResponse->GetTumorNecrosisFactor().IncrementValue(dTNF * dt_hr * scale);
  m_InflammatoryResponse->GetInterleukin6().IncrementValue(dI6 * dt_hr * scale);
  m_InflammatoryResponse->GetInterleukin10().IncrementValue(dI10 * dt_hr * scale);
  m_InflammatoryResponse->GetInterleukin12().IncrementValue(dI12 * dt_hr * scale);
  m_InflammatoryResponse->GetTissueIntegrity().IncrementValue(dTI * dt_hr * scale);
  //Nitric oxide is an algebraic relationship--update it here using new macrophage and neutrophil values
  NO = iNOS * (1.0 + kNOMA * (m_InflammatoryResponse->GetMacrophageActive().GetValue() + m_InflammatoryResponse->GetNeutrophilActive().GetValue())) + eNOS;
  m_InflammatoryResponse->GetNitricOxide().SetValue(NO);
 
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// determine override requirements from user defined inputs
///
/// \details
/// User specified override outputs that are specific to the cardiovascular system are implemented here.
/// If overrides aren't present for this system then this function will not be called during preprocess.
//--------------------------------------------------------------------------------------------------
void BloodChemistry::ProcessOverride()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();

#ifdef BIOGEARS_USE_OVERRIDE_CONTROL
  OverrideControlLoop();
#endif

  if (override->HasArterialPHOverride()) {
    GetArterialBloodPH().SetValue(override->GetArterialPHOverride().GetValue());
  }
  if (override->HasVenousPHOverride()) {
    GetVenousBloodPH().SetValue(override->GetVenousPHOverride().GetValue());
  }
  if (override->HasCO2SaturationOverride()) {
    GetCarbonDioxideSaturation().SetValue(override->GetCO2SaturationOverride().GetValue());
  }
  if (override->HasCOSaturationOverride()) {
    GetCarbonMonoxideSaturation().SetValue(override->GetCOSaturationOverride().GetValue());
  }
  if (override->HasO2SaturationOverride()) {
    GetOxygenSaturation().SetValue(override->GetO2SaturationOverride().GetValue());
  }
  if (override->HasPhosphateOverride()) {
    GetPhosphate().SetValue(override->GetPhosphateOverride(AmountPerVolumeUnit::mmol_Per_mL), AmountPerVolumeUnit::mmol_Per_mL);
  }
  if (override->HasWBCCountOverride()) {
    GetWhiteBloodCellCount().SetValue(override->GetWBCCountOverride(AmountPerVolumeUnit::ct_Per_uL), AmountPerVolumeUnit::ct_Per_uL);
  }
  if (override->HasTotalBilirubinOverride()) {
    GetTotalBilirubin().SetValue(override->GetTotalBilirubinOverride(MassPerVolumeUnit::mg_Per_mL), MassPerVolumeUnit::mg_Per_mL);
  }
  if (override->HasCalciumConcentrationOverride()) {
    m_data.GetSubstances().GetCalcium().GetBloodConcentration().SetValue(override->GetCalciumConcentrationOverride(MassPerVolumeUnit::mg_Per_mL), MassPerVolumeUnit::mg_Per_mL);
  }
  if (override->HasGlucoseConcentrationOverride()) {
    m_data.GetSubstances().GetGlucose().GetBloodConcentration().SetValue(override->GetGlucoseConcentrationOverride(MassPerVolumeUnit::mg_Per_mL), MassPerVolumeUnit::mg_Per_mL);
  }
  if (override->HasLactateConcentrationOverride()) {
    m_data.GetSubstances().GetLactate().GetBloodConcentration().SetValue(override->GetLactateConcentrationOverride(MassPerVolumeUnit::mg_Per_mL), MassPerVolumeUnit::mg_Per_mL);
  }
  if (override->HasPotassiumConcentrationOverride()) {
    m_data.GetSubstances().GetPotassium().GetBloodConcentration().SetValue(override->GetPotassiumConcentrationOverride(MassPerVolumeUnit::mg_Per_mL), MassPerVolumeUnit::mg_Per_mL);
  }
  if (override->HasSodiumConcentrationOverride()) {
    m_data.GetSubstances().GetSodium().GetBloodConcentration().SetValue(override->GetSodiumConcentrationOverride(MassPerVolumeUnit::mg_Per_mL), MassPerVolumeUnit::mg_Per_mL);
  }
}

//// Can be turned on or off (for debugging purposes) using the Biogears_USE_OVERRIDE_CONTROL external in CMake
void BloodChemistry::OverrideControlLoop()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();
  constexpr double maxArtPHOverride = 14.0; //Arterial pH
  constexpr double minArtPHOverride = 0.0; //Arterial pH
  constexpr double maxVenPHOverride = 14.0; //Venous pH
  constexpr double minVenPHOverride = 0.0; //Venous pH
  constexpr double maxCO2SaturationOverride = 1.0; //Carbon Dioxide Saturation
  constexpr double minCO2SaturationOverride = 0.0; //Carbon Dioxide Saturation
  constexpr double maxCOSaturationOverride = 1.0; //Carbon Monoxide Saturation
  constexpr double minCOSaturationOverride = 0.0; //Carbon Monoxide Saturation
  constexpr double maxO2SaturationOverride = 1.0; //Oxygen Saturation
  constexpr double minO2SaturationOverride = 0.0; //Oxygen Saturation
  constexpr double maxPhosphateOverride = 1000.0; // mmol/mL
  constexpr double minPhosphateOverride = 0.0; // mmol/mL
  constexpr double maxWBCCountOverride = 50000.0; // ct/uL
  constexpr double minWBCCountOverride = 0.0; // ct/uL
  constexpr double maxTotalBilirubinOverride = 500.0; // mg/dL
  constexpr double minTotalBilirubinOverride = 0.0; // mg/dL
  constexpr double maxCalciumConcentrationOverride = 500.0; // mg/dL
  constexpr double minCalciumConcentrationOverride = 0.0; // mg/dL
  constexpr double maxGlucoseConcentrationOverride = 1000.0; // mg/dL
  constexpr double minGlucoseConcentrationOverride = 0.0; // mg/dL
  constexpr double maxLactateConcentrationOverride = 1000.0; // mg/dL
  constexpr double minLactateConcentrationOverride = 0.0; // mg/dL
  constexpr double maxPotassiumConcentrationOverride = 500.0; // mg/mL
  constexpr double minPotassiumConcentrationOverride = 0.0; // mg/mL
  constexpr double maxSodiumConcentrationOverride = 500.0; // mg/mL
  constexpr double minSodiumConcentrationOverride = 0.0; // mg/mL

  double currentArtPHOverride = GetArterialBloodPH().GetValue(); //Current Arterial pH, value gets changed in next check
  double currentVenPHOverride = GetVenousBloodPH().GetValue(); //Current Venous pH, value gets changed in next check
  double currentCO2SaturationOverride = 0.0; //value gets changed in next check
  double currentCOSaturationOverride = 0.0; //value gets changed in next check
  double currentO2SaturationOverride = 0.0; //value gets changed in next check
  double currentPhosphateOverride = 0.0; //value gets changed in next check
  double currentWBCCountOverride = 0.0; //value gets changed in next check
  double currentTotalBilirubinOverride = 0.0; //value gets changed in next check
  double currentCalciumConcentrationOverride = 0.0; //value gets changed in next check
  double currentGlucoseConcentrationOverride = 0.0; //value gets changed in next check
  double currentLactateConcentrationOverride = 0.0; //value gets changed in next check
  double currentPotassiumConcentrationOverride = 0.0; //value gets changed in next check
  double currentSodiumConcentrationOverride = 0.0; //value gets changed in next check

  if (override->HasArterialPHOverride()) {
    currentArtPHOverride = override->GetArterialPHOverride().GetValue();
  }
  if (override->HasVenousPHOverride()) {
    currentVenPHOverride = override->GetVenousPHOverride().GetValue();
  }
  if (override->HasCO2SaturationOverride()) {
    currentCO2SaturationOverride = override->GetCO2SaturationOverride().GetValue();
  }
  if (override->HasCOSaturationOverride()) {
    currentCOSaturationOverride = override->GetCOSaturationOverride().GetValue();
  }
  if (override->HasO2SaturationOverride()) {
    currentO2SaturationOverride = override->GetO2SaturationOverride().GetValue();
  }
  if (override->HasPhosphateOverride()) {
    currentPhosphateOverride = override->GetPhosphateOverride(AmountPerVolumeUnit::mmol_Per_mL);
  }
  if (override->HasWBCCountOverride()) {
    currentWBCCountOverride = override->GetWBCCountOverride(AmountPerVolumeUnit::mmol_Per_mL);
  }
  if (override->HasTotalBilirubinOverride()) {
    currentTotalBilirubinOverride = override->GetTotalBilirubinOverride(MassPerVolumeUnit::mg_Per_mL);
  }
  if (override->HasCalciumConcentrationOverride()) {
    currentCalciumConcentrationOverride = override->GetCalciumConcentrationOverride(MassPerVolumeUnit::mg_Per_mL);
  }
  if (override->HasGlucoseConcentrationOverride()) {
    currentGlucoseConcentrationOverride = override->GetGlucoseConcentrationOverride(MassPerVolumeUnit::mg_Per_mL);
  }
  if (override->HasLactateConcentrationOverride()) {
    currentLactateConcentrationOverride = override->GetLactateConcentrationOverride(MassPerVolumeUnit::mg_Per_mL);
  }
  if (override->HasPotassiumConcentrationOverride()) {
    currentPotassiumConcentrationOverride = override->GetPotassiumConcentrationOverride(MassPerVolumeUnit::mg_Per_mL);
  }
  if (override->HasSodiumConcentrationOverride()) {
    currentSodiumConcentrationOverride = override->GetSodiumConcentrationOverride(MassPerVolumeUnit::mg_Per_mL);
  }

  if ((currentArtPHOverride < minArtPHOverride || currentArtPHOverride > maxArtPHOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Arterial Blood pH Override (BloodChemistry) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentVenPHOverride < minVenPHOverride || currentVenPHOverride > maxVenPHOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Venous Blood pH (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentCO2SaturationOverride < minCO2SaturationOverride || currentCO2SaturationOverride > maxCO2SaturationOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "CO2 Saturation (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentCOSaturationOverride < minCOSaturationOverride || currentCOSaturationOverride > maxCOSaturationOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "CO Saturation (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentO2SaturationOverride < minO2SaturationOverride || currentO2SaturationOverride > maxO2SaturationOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Oxygen Saturation (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentPhosphateOverride < minPhosphateOverride || currentPhosphateOverride > maxPhosphateOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Phosphate (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentWBCCountOverride < minWBCCountOverride || currentWBCCountOverride > maxWBCCountOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "White Blood Cell Count (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentTotalBilirubinOverride < minTotalBilirubinOverride || currentTotalBilirubinOverride > maxTotalBilirubinOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Total Bilirubin (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentCalciumConcentrationOverride < minCalciumConcentrationOverride || currentCalciumConcentrationOverride > maxCalciumConcentrationOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Calcium Concentration (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentGlucoseConcentrationOverride < minGlucoseConcentrationOverride || currentGlucoseConcentrationOverride > maxGlucoseConcentrationOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Glucose Concentration (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentLactateConcentrationOverride < minLactateConcentrationOverride || currentLactateConcentrationOverride > maxLactateConcentrationOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Lactate Concentration (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentPotassiumConcentrationOverride < minPotassiumConcentrationOverride || currentPotassiumConcentrationOverride > maxPotassiumConcentrationOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Potassium Concentration (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentSodiumConcentrationOverride < minSodiumConcentrationOverride || currentSodiumConcentrationOverride > maxSodiumConcentrationOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Sodium Concentration (BloodChemistry) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  return;
}
}
