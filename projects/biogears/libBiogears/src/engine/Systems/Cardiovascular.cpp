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
#include <biogears/engine/Systems/Cardiovascular.h>

#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/conditions/SEChronicAnemia.h>
#include <biogears/cdm/patient/conditions/SEChronicHeartFailure.h>
#include <biogears/cdm/patient/conditions/SEChronicPericardialEffusion.h>
#include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarElectricPotential.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFlowElastance.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarForce.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarPressurePerVolume.h>
#include <biogears/cdm/properties/SEScalarPressureTimePerVolumeArea.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeArea.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/engine/Systems/BloodChemistry.h>
#include <biogears/engine/Systems/Drugs.h>
#include <biogears/engine/Systems/Energy.h>
#include <biogears/engine/Systems/Nervous.h>
#include <biogears/engine/Systems/Renal.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
auto Cardiovascular::make_unique(BioGears& bg) -> std::unique_ptr<Cardiovascular>
{
  return std::unique_ptr<Cardiovascular>(new Cardiovascular(bg));
}

Cardiovascular::Cardiovascular(BioGears& bg)
  : SECardiovascularSystem(bg.GetLogger())
  , m_data(bg)
  , m_circuitCalculator(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, bg.GetLogger())
  , m_transporter(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, bg.GetLogger())
{
  Clear();
  m_TuningFile = "";
}

Cardiovascular::~Cardiovascular()
{
  Clear();
}

void Cardiovascular::Clear()
{
  SECardiovascularSystem::Clear();
  m_patient = nullptr;

  m_CirculatoryCircuit = nullptr;
  m_CirculatoryGraph = nullptr;

  m_AortaCompliance = nullptr;
  m_AortaResistance = nullptr;
  m_VenaCavaCompliance = nullptr;
  m_RightHeartResistance = nullptr;

  m_MainPulmonaryArteries = nullptr;
  m_LeftAtrium1 = nullptr;

  m_LeftPulmonaryArteriesToVeins = nullptr;
  m_LeftPulmonaryArteriesToCapillaries = nullptr;
  m_RightPulmonaryArteriesToVeins = nullptr;
  m_RightPulmonaryArteriesToCapillaries = nullptr;

  m_pAortaToBone = nullptr;
  m_pAortaToBrain = nullptr;
  m_pBrainToVenaCava = nullptr;
  m_pAortaToLiver = nullptr;
  m_pAortaToLeftKidney = nullptr;
  m_pAortaToLargeIntestine = nullptr;
  m_pAortaToMuscle = nullptr;
  m_pMuscleToVenaCava = nullptr;
  m_pAortaToMyocardium = nullptr;
  m_pMyocardiumToVenaCava = nullptr;
  m_pAortaToRightKidney = nullptr;
  m_pAortaToSkin = nullptr;
  m_pAortaToSmallIntestine = nullptr;
  m_pAortaToSplanchnic = nullptr;
  m_pAortaToSpleen = nullptr;

  m_pVenaCavaHemorrhage = nullptr;
  m_pGndToPericardium = nullptr;
  m_pPericardiumToGnd = nullptr;
  m_pRightHeartToGnd = nullptr;
  m_pRightHeart = nullptr;
  m_pLeftHeartToGnd = nullptr;
  m_pLeftHeart = nullptr;
  m_LeftHeartToAorta = nullptr;

  m_leftRenalArteryPath = nullptr;
  m_rightRenalArteryPath = nullptr;

  m_Aorta = nullptr;
  m_AortaCO2 = nullptr;
  m_LeftHeart = nullptr;
  m_LeftPulmonaryCapillaries = nullptr;
  m_LeftPulmonaryArteries = nullptr;
  m_LeftPulmonaryVeins = nullptr;
  m_Pericardium = nullptr;
  m_RightHeart = nullptr;
  m_RightPulmonaryCapillaries = nullptr;
  m_RightPulmonaryArteries = nullptr;
  m_RightPulmonaryVeins = nullptr;
  m_VenaCava = nullptr;

  m_CardiacCycleArterialPressure_mmHg.Reset();
  m_CardiacCycleArterialCO2PartialPressure_mmHg.Reset();
  m_CardiacCyclePulmonaryCapillariesWedgePressure_mmHg.Reset();
  m_CardiacCyclePulmonaryCapillariesFlow_mL_Per_s.Reset();
  m_CardiacCyclePulmonaryShuntFlow_mL_Per_s.Reset();
  m_CardiacCyclePulmonaryArteryPressure_mmHg.Reset();
  m_CardiacCycleCentralVenousPressure_mmHg.Reset();
  m_CardiacCycleSkinFlow_mL_Per_s.Reset();

  m_OverrideHR_Conformant_Per_min = 0.0;
  m_OverrideLHEMin_Conformant_mmHg = 0.0;
  m_OverrideLHEMax_Conformant_mmHg = 0.0;
  m_OverrideRHEMin_Conformant_mmHg = 0.0;
  m_OverrideRHEMax_Conformant_mmHg = 0.0;

  m_overrideTime_s = 0.0;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::Initialize()
{
  BioGearsSystem::Initialize();

  m_HeartRhythm = CDM::enumHeartRhythm::NormalSinus;

  m_StartSystole = true;
  m_HeartFlowDetected = false;
  m_CardiacCyclePeriod_s = 0.8; //seconds per beat
  m_CardiacCycleDiastolicVolume_mL = 0.0;
  m_CardiacCycleStrokeVolume_mL = 0;
  m_CurrentCardiacCycleDuration_s = 0;

  //Heart Elastance Parameters
  m_LeftHeartElastance_mmHg_Per_mL = 0.0;
  m_LeftHeartElastanceMax_mmHg_Per_mL = m_data.GetConfiguration().GetLeftHeartElastanceMaximum(FlowElastanceUnit::mmHg_Per_mL);
  m_LeftHeartElastanceMin_mmHg_Per_mL = m_data.GetConfiguration().GetLeftHeartElastanceMinimum(FlowElastanceUnit::mmHg_Per_mL);
  m_LeftHeartElastanceModifier = 1.0; //Utilized for reducing the maximum elastance to represent left ventricular systolic dysfunction
  m_RightHeartElastance_mmHg_Per_mL = 0.0;
  m_RightHeartElastanceMax_mmHg_Per_mL = m_data.GetConfiguration().GetRightHeartElastanceMaximum(FlowElastanceUnit::mmHg_Per_mL);
  m_RightHeartElastanceMin_mmHg_Per_mL = m_data.GetConfiguration().GetRightHeartElastanceMinimum(FlowElastanceUnit::mmHg_Per_mL);

  // CPR and Cardiac Arrest control
  m_EnterCardiacArrest = false;
  m_CompressionTime_s = 0.0;
  m_CompressionRatio = 0.0;
  m_CompressionPeriod_s = 0.0;

  //Initialize system data based on patient file inputs
  GetBloodVolume().Set(m_patient->GetBloodVolumeBaseline());
  m_CardiacCycleAortaPressureHigh_mmHg = m_patient->GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
  m_CardiacCycleAortaPressureLow_mmHg = m_patient->GetDiastolicArterialPressureBaseline(PressureUnit::mmHg);
  GetMeanArterialPressure().SetValue((2. / 3. * m_CardiacCycleAortaPressureLow_mmHg) + (1. / 3. * m_CardiacCycleAortaPressureHigh_mmHg), PressureUnit::mmHg);
  m_CardiacCycleArterialPressure_mmHg.Sample(GetMeanArterialPressure().GetValue(PressureUnit::mmHg));
  m_CardiacCyclePulmonaryArteryPressureHigh_mmHg = 26;
  m_CardiacCyclePulmonaryArteryPressureLow_mmHg = 9;
  GetPulmonaryMeanArterialPressure().SetValue(15, PressureUnit::mmHg);
  GetHeartRate().Set(m_patient->GetHeartRateBaseline());
  RecordAndResetCardiacCycle();

  // Set system data based on physiology norms
  GetMeanCentralVenousPressure().SetValue(5.0, PressureUnit::mmHg);
  m_CardiacCycleArterialCO2PartialPressure_mmHg.Sample(60.0);
  m_LastCardiacCycleMeanArterialCO2PartialPressure_mmHg = 60.0;
  GetMeanArterialCarbonDioxidePartialPressure().SetValue(60, PressureUnit::mmHg);
  GetMeanArterialCarbonDioxidePartialPressureDelta().SetValue(0.0, PressureUnit::mmHg);
  GetPulmonaryCapillariesWedgePressure().SetValue(0, PressureUnit::mmHg);
  GetPulmonaryMeanArterialPressure().SetValue(90, PressureUnit::mmHg);
  GetPulmonaryArterialPressure().SetValue(90, PressureUnit::mmHg);
  GetPulmonaryMeanCapillaryFlow().SetValue(0, VolumePerTimeUnit::mL_Per_s);
  GetPulmonaryMeanShuntFlow().SetValue(0, VolumePerTimeUnit::mL_Per_s);

  GetMeanSkinFlow().SetValue(0, VolumePerTimeUnit::mL_Per_s);
  GetCardiacOutput().SetValue(5600, VolumePerTimeUnit::mL_Per_min);
  GetHeartStrokeVolume().SetValue(78, VolumeUnit::mL);
  GetHeartEjectionFraction().SetValue(0.55);
  GetCardiacIndex().SetValue(3.0, VolumePerTimeAreaUnit::mL_Per_min_m2);
  GetPulmonaryVascularResistance().SetValue(0.14, FlowResistanceUnit::mmHg_min_Per_mL);
  GetPulmonaryVascularResistanceIndex().SetValue(0.082, PressureTimePerVolumeAreaUnit::mmHg_min_Per_mL_m2);

  m_CurrentCardiacCycleTime_s = 0.0;

  CalculateHeartElastance();

  double systemicVascularResistance_mmHg_s_Per_mL = (GetMeanArterialPressure().GetValue(PressureUnit::mmHg) - GetMeanCentralVenousPressure().GetValue(PressureUnit::mmHg)) / GetCardiacOutput().GetValue(VolumePerTimeUnit::mL_Per_s);
  GetSystemicVascularResistance().SetValue(systemicVascularResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  //This is not part of stabilization due to not knowing when we hit the patient parameters with a circuit configuration
  TuneCircuit();
  systemicVascularResistance_mmHg_s_Per_mL = (GetMeanArterialPressure().GetValue(PressureUnit::mmHg) - GetMeanCentralVenousPressure().GetValue(PressureUnit::mmHg)) / GetCardiacOutput().GetValue(VolumePerTimeUnit::mL_Per_s);
  GetSystemicVascularResistance().SetValue(systemicVascularResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  m_LeftHeartElastanceMax_mmHg_Per_mL = m_data.GetConfiguration().GetLeftHeartElastanceMaximum(FlowElastanceUnit::mmHg_Per_mL);
  m_RightHeartElastanceMax_mmHg_Per_mL = m_data.GetConfiguration().GetRightHeartElastanceMaximum(FlowElastanceUnit::mmHg_Per_mL);
  m_patient->GetBloodVolumeBaseline().Set(GetBloodVolume());

  m_OverrideLHEMin_Conformant_mmHg = m_data.GetConfiguration().GetLeftHeartElastanceMinimum(FlowElastanceUnit::mmHg_Per_mL); //m_patient->GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
  m_OverrideRHEMin_Conformant_mmHg = m_data.GetConfiguration().GetRightHeartElastanceMinimum(FlowElastanceUnit::mmHg_Per_mL); //m_patient->GetDiastolicArterialPressureBaseline(PressureUnit::mmHg);
  m_OverrideLHEMax_Conformant_mmHg = m_LeftHeartElastanceMax_mmHg_Per_mL;
  m_OverrideRHEMax_Conformant_mmHg = m_RightHeartElastanceMax_mmHg_Per_mL;

  m_overrideTime_s = 0.0;
}

bool Cardiovascular::Load(const CDM::BioGearsCardiovascularSystemData& in)
{
  if (!SECardiovascularSystem::Load(in))
    return false;

  m_StartSystole = in.StartSystole();
  m_HeartFlowDetected = in.HeartFlowDetected();
  m_EnterCardiacArrest = in.EnterCardiacArrest();
  m_CardiacCyclePeriod_s = in.CardiacCyclePeriod_s();
  m_CurrentCardiacCycleDuration_s = in.CurrentCardiacCycleDuration_s();
  m_LeftHeartElastanceModifier = in.LeftHeartElastanceModifier();
  m_LeftHeartElastance_mmHg_Per_mL = in.LeftHeartElastance_mmHg_Per_mL();
  m_LeftHeartElastanceMax_mmHg_Per_mL = in.LeftHeartElastanceMax_mmHg_Per_mL();
  m_LeftHeartElastanceMin_mmHg_Per_mL = in.LeftHeartElastanceMin_mmHg_Per_mL();
  m_RightHeartElastance_mmHg_Per_mL = in.RightHeartElastance_mmHg_Per_mL();
  m_RightHeartElastanceMax_mmHg_Per_mL = in.RightHeartElastanceMax_mmHg_Per_mL();
  m_RightHeartElastanceMin_mmHg_Per_mL = in.RightHeartElastanceMin_mmHg_Per_mL();

  m_CompressionTime_s = in.CompressionTime_s();
  m_CompressionRatio = in.CompressionRatio();
  m_CompressionPeriod_s = in.CompressionPeriod_s();

  m_CurrentCardiacCycleTime_s = in.CurrentCardiacCycleTime_s();
  m_CardiacCycleDiastolicVolume_mL = in.CardiacCycleDiastolicVolume_mL();
  m_CardiacCycleAortaPressureLow_mmHg = in.CardiacCycleAortaPressureLow_mmHg();
  m_CardiacCycleAortaPressureHigh_mmHg = in.CardiacCycleAortaPressureHigh_mmHg();
  m_CardiacCyclePulmonaryArteryPressureLow_mmHg = in.CardiacCyclePulmonaryArteryPressureLow_mmHg();
  m_CardiacCyclePulmonaryArteryPressureHigh_mmHg = in.CardiacCyclePulmonaryArteryPressureHigh_mmHg();
  m_LastCardiacCycleMeanArterialCO2PartialPressure_mmHg = in.LastCardiacCycleMeanArterialCO2PartialPressure_mmHg();
  m_CardiacCycleStrokeVolume_mL = in.CardiacCycleStrokeVolume_mL();

  m_CardiacCycleArterialPressure_mmHg.Load(in.CardiacCycleArterialPressure_mmHg());
  m_CardiacCycleArterialCO2PartialPressure_mmHg.Load(in.CardiacCycleArterialCO2PartialPressure_mmHg());
  m_CardiacCyclePulmonaryCapillariesWedgePressure_mmHg.Load(in.CardiacCyclePulmonaryCapillariesWedgePressure_mmHg());
  m_CardiacCyclePulmonaryCapillariesFlow_mL_Per_s.Load(in.CardiacCyclePulmonaryCapillariesFlow_mL_Per_s());
  m_CardiacCyclePulmonaryShuntFlow_mL_Per_s.Load(in.CardiacCyclePulmonaryShuntFlow_mL_Per_s());
  m_CardiacCyclePulmonaryArteryPressure_mmHg.Load(in.CardiacCyclePulmonaryArteryPressure_mmHg());
  m_CardiacCycleCentralVenousPressure_mmHg.Load(in.CardiacCycleCentralVenousPressure_mmHg());
  m_CardiacCycleSkinFlow_mL_Per_s.Load(in.CardiacCycleSkinFlow_mL_Per_s());

  BioGearsSystem::LoadState();
  return true;
}
CDM::BioGearsCardiovascularSystemData* Cardiovascular::Unload() const
{
  CDM::BioGearsCardiovascularSystemData* data = new CDM::BioGearsCardiovascularSystemData();
  Unload(*data);
  return data;
}
void Cardiovascular::Unload(CDM::BioGearsCardiovascularSystemData& data) const
{
  SECardiovascularSystem::Unload(data);

  data.StartSystole(m_StartSystole);
  data.HeartFlowDetected(m_HeartFlowDetected);
  data.EnterCardiacArrest(m_EnterCardiacArrest);
  data.CardiacCyclePeriod_s(m_CardiacCyclePeriod_s);
  data.CurrentCardiacCycleDuration_s(m_CurrentCardiacCycleDuration_s);
  data.LeftHeartElastance_mmHg_Per_mL(m_LeftHeartElastance_mmHg_Per_mL);
  data.LeftHeartElastanceModifier(m_LeftHeartElastanceModifier);
  data.LeftHeartElastanceMax_mmHg_Per_mL(m_LeftHeartElastanceMax_mmHg_Per_mL);
  data.LeftHeartElastanceMin_mmHg_Per_mL(m_LeftHeartElastanceMin_mmHg_Per_mL);
  data.RightHeartElastance_mmHg_Per_mL(m_RightHeartElastance_mmHg_Per_mL);
  data.RightHeartElastanceMax_mmHg_Per_mL(m_RightHeartElastanceMax_mmHg_Per_mL);
  data.RightHeartElastanceMin_mmHg_Per_mL(m_RightHeartElastanceMin_mmHg_Per_mL);

  data.CompressionTime_s(m_CompressionTime_s);
  data.CompressionRatio(m_CompressionRatio);
  data.CompressionPeriod_s(m_CompressionPeriod_s);

  data.CurrentCardiacCycleTime_s(m_CurrentCardiacCycleTime_s);
  data.CardiacCycleDiastolicVolume_mL(m_CardiacCycleDiastolicVolume_mL);
  data.CardiacCycleAortaPressureLow_mmHg(m_CardiacCycleAortaPressureLow_mmHg);
  data.CardiacCycleAortaPressureHigh_mmHg(m_CardiacCycleAortaPressureHigh_mmHg);
  data.CardiacCyclePulmonaryArteryPressureLow_mmHg(m_CardiacCyclePulmonaryArteryPressureLow_mmHg);
  data.CardiacCyclePulmonaryArteryPressureHigh_mmHg(m_CardiacCyclePulmonaryArteryPressureHigh_mmHg);
  data.LastCardiacCycleMeanArterialCO2PartialPressure_mmHg(m_LastCardiacCycleMeanArterialCO2PartialPressure_mmHg);
  data.CardiacCycleStrokeVolume_mL(m_CardiacCycleStrokeVolume_mL);

  data.CardiacCycleArterialPressure_mmHg(std::unique_ptr<CDM::RunningAverageData>(m_CardiacCycleArterialPressure_mmHg.Unload()));
  data.CardiacCycleArterialCO2PartialPressure_mmHg(std::unique_ptr<CDM::RunningAverageData>(m_CardiacCycleArterialCO2PartialPressure_mmHg.Unload()));
  data.CardiacCyclePulmonaryCapillariesWedgePressure_mmHg(std::unique_ptr<CDM::RunningAverageData>(m_CardiacCyclePulmonaryCapillariesWedgePressure_mmHg.Unload()));
  data.CardiacCyclePulmonaryCapillariesFlow_mL_Per_s(std::unique_ptr<CDM::RunningAverageData>(m_CardiacCyclePulmonaryCapillariesFlow_mL_Per_s.Unload()));
  data.CardiacCyclePulmonaryShuntFlow_mL_Per_s(std::unique_ptr<CDM::RunningAverageData>(m_CardiacCyclePulmonaryShuntFlow_mL_Per_s.Unload()));
  data.CardiacCyclePulmonaryArteryPressure_mmHg(std::unique_ptr<CDM::RunningAverageData>(m_CardiacCyclePulmonaryArteryPressure_mmHg.Unload()));
  data.CardiacCycleCentralVenousPressure_mmHg(std::unique_ptr<CDM::RunningAverageData>(m_CardiacCycleCentralVenousPressure_mmHg.Unload()));
  data.CardiacCycleSkinFlow_mL_Per_s(std::unique_ptr<CDM::RunningAverageData>(m_CardiacCycleSkinFlow_mL_Per_s.Unload()));
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for Cardiovascular Class
///
///  \details
///  Initializes member variables and system level values on the common data model.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::SetUp()
{
  m_dT_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  m_patient = &m_data.GetPatient();
  m_minIndividialSystemicResistance__mmHg_s_Per_mL = 0.1;
  m_OverrideHR_Conformant_Per_min = m_patient->GetHeartRateBaseline(FrequencyUnit::Per_min);

  //Circuits
  m_CirculatoryCircuit = &m_data.GetCircuits().GetActiveCardiovascularCircuit();
  m_CirculatoryGraph = &m_data.GetCompartments().GetActiveCardiovascularGraph();
  //Compartments
  m_Aorta = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Aorta);
  m_AortaCO2 = m_Aorta->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());
  m_Brain = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Brain);
  m_LeftPulmonaryArteries = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryArteries);
  m_RightPulmonaryArteries = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightPulmonaryArteries);
  m_LeftPulmonaryVeins = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryVeins);
  m_RightPulmonaryVeins = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightPulmonaryVeins);
  m_LeftPulmonaryCapillaries = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryCapillaries);
  m_RightPulmonaryCapillaries = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightPulmonaryCapillaries);
  m_VenaCava = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::VenaCava);
  m_Pericardium = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Pericardium);
  m_LeftHeart = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftHeart);
  m_RightHeart = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightHeart);
  //Nodes
  m_MainPulmonaryArteries = m_CirculatoryCircuit->GetNode(BGE::CardiovascularNode::MainPulmonaryArteries);
  m_LeftAtrium1 = m_CirculatoryCircuit->GetNode(BGE::CardiovascularNode::LeftAtrium1);
  //Paths
  m_LeftPulmonaryArteriesToVeins = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::LeftPulmonaryArteriesToLeftPulmonaryVeins);
  m_LeftPulmonaryArteriesToCapillaries = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::LeftPulmonaryArteriesToLeftPulmonaryCapillaries);
  m_RightPulmonaryArteriesToVeins = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::RightPulmonaryArteriesToRightPulmonaryVeins);
  m_RightPulmonaryArteriesToCapillaries = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::RightPulmonaryArteriesToRightPulmonaryCapillaries);

  m_pAortaToBone = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToBone1);
  m_pAortaToBrain = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToBrain1);
  m_pBrainToVenaCava = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Brain1ToBrain2);
  m_pAortaToLiver = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToLiver1);
  m_pAortaToLeftKidney = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToLeftKidney1);
  m_pAortaToLargeIntestine = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToLargeIntestine);
  m_pAortaToMuscle = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToMuscle1);
  m_pMuscleToVenaCava = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Muscle1ToMuscle2);
  m_pAortaToMyocardium = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToMyocardium1);
  m_pMyocardiumToVenaCava = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Myocardium1ToMyocardium2);
  m_pAortaToRightKidney = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToRightKidney1);
  m_pAortaToSkin = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToSkin1);
  m_pAortaToSmallIntestine = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToSmallIntestine);
  m_pAortaToSplanchnic = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToSplanchnic);
  m_pAortaToSpleen = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToSpleen);

  m_pBrainResistanceDownstream = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Brain1ToBrain2);
  m_pBrainResistanceUpstream = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToBrain1);
  if (m_data.GetConfiguration().IsCerebralEnabled()) {
    m_pBrainResistanceUpstream = m_CirculatoryCircuit->GetPath(BGE::CerebralPath::NeckArteriesToCerebralArteries1);
    m_pBrainResistanceDownstream = m_CirculatoryCircuit->GetPath(BGE::CerebralPath::CerebralVeins1ToCerebralVeins2);
  }

  m_pVenaCavaHemorrhage = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::VenaCavaBleed);
  m_pGndToPericardium = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::GroundToPericardium1);
  m_pPericardiumToGnd = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Pericardium1ToGround);
  m_pRightHeartToGnd = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::RightVentricle2ToGround);
  m_pRightHeart = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::RightVentricle1ToRightVentricle2);
  m_pLeftHeartToGnd = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::LeftVentricle2ToGround);
  m_pLeftHeart = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::LeftVentricle1ToLeftVentricle2);
  m_LeftHeartToAorta = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::LeftVentricle1ToAorta2);

  /// \todo We are assuming that the complex renal system is connected. Make it agnostic.
  m_leftRenalArteryPath = m_CirculatoryCircuit->GetPath(BGE::RenalPath::LeftRenalArteryToAfferentArteriole);
  m_rightRenalArteryPath = m_CirculatoryCircuit->GetPath(BGE::RenalPath::RightRenalArteryToAfferentArteriole);

  m_systemicResistancePaths.clear();
  m_systemicCompliancePaths.clear();
  m_tissueResistancePaths.clear();
  m_cerebralResistancePaths.clear();
  m_extrasplanchnicResistancePaths.clear();
  m_splanchnicResistancePaths.clear();
  m_myocardiumResistancePaths.clear();
  m_muscleResistancePaths.clear();

  std::vector<SEFluidCircuitNode*> venousNodes;
  SEFluidCircuitNode* aorta = m_CirculatoryCircuit->GetNode(BGE::CardiovascularNode::Aorta1);
  SEFluidCircuitNode* ground = m_CirculatoryCircuit->GetNode(BGE::CardiovascularNode::Ground);

  //Cache resistance paths that will be affected by nervous feedback.  We exclude cerebral because we do not want those paths pushed on to systemic paths vector.  This way, cerebral
  //paths won't be affected by drugs.
  for (SEFluidCircuitPath* path : m_CirculatoryCircuit->GetPaths()) {
    if (&path->GetSourceNode() == aorta && path->HasResistanceBaseline()) {
      if (&path->GetTargetNode() != ground && path->HasCardiovascularRegion()) {
        CDM::enumResistancePathType region = path->GetCardiovascularRegion();
        switch (region) {
        case CDM::enumResistancePathType::Cerebral:
          continue;
        case CDM::enumResistancePathType::Extrasplanchnic:
          m_extrasplanchnicResistancePaths.push_back(path);
          break;
        case CDM::enumResistancePathType::Muscle:
          m_muscleResistancePaths.push_back(path);
          break;
        case CDM::enumResistancePathType::Myocardium:
          m_myocardiumResistancePaths.push_back(path);
          break;
        case CDM::enumResistancePathType::Splanchnic:
          m_splanchnicResistancePaths.push_back(path);
          break;
        }
        m_systemicResistancePaths.push_back(path);
        venousNodes.push_back(&path->GetTargetNode());
      }
    }
  }
  for (SEFluidCircuitPath* path : m_CirculatoryCircuit->GetPaths()) {
    for (SEFluidCircuitNode* node : venousNodes) {
      if (&path->GetSourceNode() == node) {
        if (path->HasResistanceBaseline() && &path->GetTargetNode() != ground) {
          if (path->HasCardiovascularRegion()) {
            CDM::enumResistancePathType region = path->GetCardiovascularRegion();
            switch (region) {
            case CDM::enumResistancePathType::Extrasplanchnic:
              m_extrasplanchnicResistancePaths.push_back(path);
              break;
            case CDM::enumResistancePathType::Muscle:
              m_muscleResistancePaths.push_back(path);
              break;
            case CDM::enumResistancePathType::Myocardium:
              m_myocardiumResistancePaths.push_back(path);
              break;
            case CDM::enumResistancePathType::Splanchnic:
              m_splanchnicResistancePaths.push_back(path);
              break;
            }
          }
          m_systemicResistancePaths.push_back(path);
        }
        if (path->HasComplianceBaseline()) {
          m_systemicCompliancePaths.push_back(path);
        }
      }
    }
  }

  // Add the portal vein!
  SEFluidCircuitPath* path = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::PortalVeinToLiver1);
  if (!Contains(m_systemicResistancePaths, (*path))) {
    m_systemicResistancePaths.push_back(path);
    m_splanchnicResistancePaths.push_back(path);
  }

  //Since we have cerbral circuit defined separately, we can add cerebral resistance paths here
  if (m_data.GetConfiguration().IsCerebralEnabled()) {
    for (auto path : m_data.GetCircuits().GetCerebralCircuit().GetPaths()) {
      if (path->HasCardiovascularRegion()) {
        m_cerebralResistancePaths.push_back(path);
      }
    }
  }

  m_AortaCompliance = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta1ToGround);
  m_AortaResistance = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::Aorta3ToAorta1);
  m_VenaCavaCompliance = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::VenaCavaToGround);
  m_RightHeartResistance = m_CirculatoryCircuit->GetPath(BGE::CardiovascularPath::VenaCavaToRightAtrium1);

  if (m_data.GetConfiguration().IsTissueEnabled()) {
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::GutE1ToGutE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::BoneE1ToBoneE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::BrainE1ToBrainE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::FatE1ToFatE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::LeftKidneyE1ToLeftKidneyE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::LeftLungE1ToLeftLungE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::LiverE1ToLiverE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::MuscleE1ToMuscleE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::MyocardiumE1ToMyocardiumE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::RightKidneyE1ToRightKidneyE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::RightLungE1ToRightLungE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::SkinE1ToSkinE2));
    m_tissueResistancePaths.push_back(m_CirculatoryCircuit->GetPath(BGE::TissuePath::SpleenE1ToSpleenE2));
  }

  m_PatientActions = &m_data.GetActions().GetPatientActions();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Cardiovascular system at steady state
///
/// \details
/// Initializes Cardiovascular conditions if any are present.
///  <UL>
///   <LI>Anemia</LI>
///   <LI>Renal Stenosis</LI>
///   <LI>Heart Failure</LI>
///   <LI>Pericardial Effusion</LI>
///  </UL>
///
//--------------------------------------------------------------------------------------------------
void Cardiovascular::AtSteadyState()
{
  m_patient->GetHeartRateBaseline().Set(GetHeartRate());
  m_patient->GetDiastolicArterialPressureBaseline().Set(GetDiastolicArterialPressure());
  m_patient->GetSystolicArterialPressureBaseline().Set(GetSystolicArterialPressure());
  m_patient->GetMeanArterialPressureBaseline().Set(GetMeanArterialPressure());

  std::string typeString = "Initial Stabilization Homeostasis: ";
  if (m_data.GetState() == EngineState::AtSecondaryStableState)
    typeString = "Secondary Stabilization Homeostasis: ";

  m_ss << typeString << "Patient heart rate = " << GetHeartRate();
  Info(m_ss);
  m_ss << typeString << "Patient diastolic arterial pressure = " << GetDiastolicArterialPressure();
  Info(m_ss);
  m_ss << typeString << "Patient systolic arterial pressure = " << GetSystolicArterialPressure();
  Info(m_ss);
  m_ss << typeString << "Patient mean arterial pressure = " << GetMeanArterialPressure();
  Info(m_ss);

  if (m_data.GetState() == EngineState::AtInitialStableState) { // At Resting State, apply conditions if we have them
    if (m_data.GetConditions().HasChronicAnemia())
      ChronicAnemia();
    if (m_data.GetConditions().HasChronicRenalStenosis())
      ChronicRenalStenosis();
    if (m_data.GetConditions().HasChronicVentricularSystolicDysfunction())
      ChronicHeartFailure();
    if (m_data.GetConditions().HasChronicPericardialEffusion())
      ChronicPericardialEffusion();
  }

  m_LeftHeartElastanceMax_mmHg_Per_mL = m_data.GetConfiguration().GetLeftHeartElastanceMaximum(FlowElastanceUnit::mmHg_Per_mL);
  m_RightHeartElastanceMax_mmHg_Per_mL = m_data.GetConfiguration().GetRightHeartElastanceMaximum(FlowElastanceUnit::mmHg_Per_mL);
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
void Cardiovascular::ChronicAnemia()
{
  SEChronicAnemia* anemia = m_data.GetConditions().GetChronicAnemia();
  double rf = anemia->GetReductionFactor().GetValue();

  // Maximum 30% reduction
  if (rf > 0.3) {
    /// \error if too much hemoglobin is removed, we will no longer meet validation, so set to maximum amount that can be removed.
    Error("Cannot remove more than 30% of hemoglobin in anemia in the BioGears Engine. Setting value to 30% and continuing.", "Cardiovascular::Anemia");
    rf = 0.3;
  }
  // Empirical resistance modification
  double viscousModifier = 1.0 - (0.15 * rf);

  std::vector<SESubstance*> hemoglobinSubs;
  hemoglobinSubs.push_back(&m_data.GetSubstances().GetHb());
  hemoglobinSubs.push_back(&m_data.GetSubstances().GetHbO2());
  hemoglobinSubs.push_back(&m_data.GetSubstances().GetHbCO2());
  hemoglobinSubs.push_back(&m_data.GetSubstances().GetHbO2CO2());

  double newMass_g;
  SELiquidSubstanceQuantity* subQ;
  // Reduce all hemoglobin mass in all compartments
  for (SELiquidCompartment* cmpt : m_data.GetCompartments().GetVascularLeafCompartments()) {
    if (!cmpt->HasVolume())
      continue;

    for (SESubstance* subst : hemoglobinSubs) {
      subQ = cmpt->GetSubstanceQuantity(*subst);
      newMass_g = subQ->GetMass(MassUnit::g) * (1 - rf);
      subQ->GetMass().SetValue(newMass_g, MassUnit::g);
      subQ->Balance(BalanceLiquidBy::Mass);
    }
  }

  // Only the cardiovascular paths are adjusted. This is obviously an inconsistency, but
  // other vascular paths may contain non-blood fluids for which viscosity is unaffected by
  // anemia. This gets us close enough.
  for (SEFluidCircuitPath* path : m_data.GetCircuits().GetCardiovascularCircuit().GetPaths()) {
    if (path->HasResistanceBaseline()) {
      path->GetResistanceBaseline().SetValue(path->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL) * viscousModifier, FlowResistanceUnit::mmHg_s_Per_mL);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Establishes the chronic heart failure condition.
///
/// \details
/// Heart failure is a chronic condition that is modeled by a permanent
/// reduction in the left heart contractility. The user may scale the severity of this action
/// with a fraction from 0 to 1, with 0 being non-existent to 1 being severe heart failure.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::ChronicHeartFailure()
{
  //Decrease left heart contractility
  m_LeftHeartElastanceModifier = 0.2;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Establishes the pericardial effusion condition.
///
/// \details
/// Pericardial effusion can be either chronic (slow) or acute (fast).
/// Chronic effusion will eventually lead to tamponade, acute effusion leads
/// immediately to tamponade and imminent death. The chronic effusion parameters
/// are set in the BioGears engine so that life-threatening tamponade will occur
/// in about 30 minutes after the insult.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::ChronicPericardialEffusion()
{
  double deltaVolume_mL = m_data.GetConditions().GetChronicPericardialEffusion()->GetAccumulatedVolume().GetValue(VolumeUnit::mL);
  if (deltaVolume_mL > 1000.0) {
    Error("Cannot specify volume accumulation greater than 1000 mL. Accumulated volume is now set at 1000 mL.");
    /// \error Cannot specify volume accumulation greater than 1000 mL. Accumulated volume is now set at 1000 mL.
    deltaVolume_mL = 1000.0;
  } else if (deltaVolume_mL < 0.0) {
    Error("Cannot specify volume accumulation less than 0 mL. Accumulated volume is now set at 0 mL.");
    /// \error Cannot specify volume accumulation less than 0 mL. Accumulated volume is now set at 0 mL.
    deltaVolume_mL = 0.0;
  }

  //Just throw this all on at once
  //Only do this for a single time-step!
  m_pGndToPericardium->GetNextFlowSource().SetValue(deltaVolume_mL / m_dT_s, VolumePerTimeUnit::mL_Per_s);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Establishes the renal stenosis condition in one or both of the renal arteries.
///
/// \details
/// Stenosed renal arteries are slightly occluded, which increases the resistance from the aorta to the kidney.
/// This is implemented as a condition, which specifies what percent of the artery is occluded and which artery
/// or arteries are being affected.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::ChronicRenalStenosis()
{
  ///\todo move this to CV
  double LeftOcclusionFraction = m_data.GetConditions().GetChronicRenalStenosis()->GetLeftKidneySeverity().GetValue();
  double RightOcclusionFraction = m_data.GetConditions().GetChronicRenalStenosis()->GetRightKidneySeverity().GetValue();

  if (LeftOcclusionFraction < 0.0) {
    /// \error Cannot specify left occlusion fraction less than zero
    Error("Cannot specify left occlusion fraction less than zero. Renal resistances remain unchanged."); //Specify resistance is the same in error
    return;
  }

  if (RightOcclusionFraction < 0.0) {
    /// \error Cannot specify right occlusion fraction less than zero
    Error("Cannot specify right occlusion fraction less than zero. Renal resistances remain unchanged.");
    return;
  }

  if (LeftOcclusionFraction > 1.0) {
    /// \error Cannot specify left occlusion fraction greater than one
    Error("Cannot specify left occlusion fraction greater than one. Renal resistances remain unchanged.");
    return;
  }

  if (RightOcclusionFraction > 1.0) {
    /// \error Cannot specify right occlusion fraction greater than one
    Error("Cannot specify right occlusion fraction greater than  one. Renal resistances remain unchanged.");
    return;
  }

  //Aorta1ToAfferentArteriole paths are equivalent to the renal artery in BioGears. Resistance increases on these paths to represent renal arterial stenosis
  double currentLeftResistance_mmHg_s_Per_mL = m_leftRenalArteryPath->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
  double currentRightResistance_mmHg_s_Per_mL = m_rightRenalArteryPath->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);

  //The base resistance is a tuned parameter that allows for adequate flow reduction to the kidneys with the logarithmic functional form chosen
  double baseResistance_mmHg_s_Per_mL = 10.0;
  //Open resistance indicates a completely occluded artery. This value is 100 mmHg/mL/s for the cardiovascular circuit.
  double openResistance_mmHg_s_Per_mL = m_data.GetConfiguration().GetCardiovascularOpenResistance(FlowResistanceUnit::mmHg_s_Per_mL);

  double newLeftResistance_mmHg_s_Per_mL = GeneralMath::ResistanceFunction(baseResistance_mmHg_s_Per_mL, openResistance_mmHg_s_Per_mL, currentLeftResistance_mmHg_s_Per_mL, LeftOcclusionFraction);
  double newRightResistance_mmHg_s_Per_mL = GeneralMath::ResistanceFunction(baseResistance_mmHg_s_Per_mL, openResistance_mmHg_s_Per_mL, currentRightResistance_mmHg_s_Per_mL, RightOcclusionFraction);

  m_leftRenalArteryPath->GetResistanceBaseline().SetValue(newLeftResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  m_rightRenalArteryPath->GetResistanceBaseline().SetValue(newRightResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Preprocess prepares the cardiovascular system for the circuit solver
///
/// \details
/// This function calculates the appropriate modifications to the cardiovascular
/// circuit for the time within the cardiac cycle and it processes the actions on the
/// cardiovascular system.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::PreProcess()
{
  // Locate the cardiac cycle in time (systole, diastole)
  // and do the appropriate calculations based on the time location.
  HeartDriver();
  ProcessActions();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Process solves the cardiovascular circuit
///
/// \details
/// Modifications to the cardiovascular system are made during the preprocess
/// step of the cardiovascular and other systems. The new state of the circuit
/// is solved using %BioGears @ref CircuitMethodology. Advective substance transport
/// is computed using the %BioGears @ref SubstanceTransportMethodology.
/// Finally, vitals sign data is computed and system data is populated in the
/// CalculateVitalSigns method.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::Process()
{
  m_circuitCalculator.Process(*m_CirculatoryCircuit, m_dT_s);
  m_transporter.Transport(*m_CirculatoryGraph, m_dT_s);
  CalculateVitalSigns();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update the cardiovascular circuit
///
/// \details
/// The current time-step's circuit solution is set to the next time-step when it is passed to PostProcess.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::PostProcess()
{
  if (m_data.GetActions().GetPatientActions().HasOverride()
      && m_data.GetState() == EngineState::Active) {
    SEOverride* override = m_data.GetActions().GetPatientActions().GetOverride();
    if (override->HasCardiovascularOverride()
        && override->GetOverrideConformance() == CDM::enumOnOff::Off) {
      ProcessOverride();
    }
  }
  m_circuitCalculator.PostProcess(*m_CirculatoryCircuit);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// CalculateVitalSigns computes and/or updates cardiovascular system level data
///
/// \details
/// Calculate vital signs obtains the pressures in the aorta, pulmonary arteries, pulmonary veins and vena cava.
/// Waveform data for the system, such as arterial pressure, is set every at every time slice. Mean data, such
/// as mean arterial pressure, is set using a running average. Data that are more useful filtered are also set
/// from a running mean.
/// Several events and irreversible states are detected and set by this method.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::CalculateVitalSigns()
{
  // Grab data from the circuit in order to calculate a running mean
  double AortaNodePressure_mmHg = m_Aorta->GetPressure(PressureUnit::mmHg);
  double AortaNodeCO2PartialPressure_mmHg = m_AortaCO2 == nullptr ? 0 : m_AortaCO2->GetPartialPressure(PressureUnit::mmHg); // This is here so we can Tune circuit w/o substances
  double LeftPulmonaryArteryVolume_mL = m_LeftPulmonaryArteries->GetVolume(VolumeUnit::mL);
  double RightPulmonaryArteryVolume_mL = m_RightPulmonaryArteries->GetVolume(VolumeUnit::mL);
  double TotalPulmonaryArteryVolume_mL = LeftPulmonaryArteryVolume_mL + RightPulmonaryArteryVolume_mL;
  double LeftPulmonaryArteryPressure_mmHg = m_LeftPulmonaryArteries->GetPressure(PressureUnit::mmHg);
  double RightPulmonaryArteryPressure_mmHg = m_RightPulmonaryArteries->GetPressure(PressureUnit::mmHg);

  double LeftPulmonaryVeinVolume_mL = m_LeftPulmonaryVeins->GetVolume(VolumeUnit::mL);
  double RightPulmonaryVeinVolume_mL = m_RightPulmonaryVeins->GetVolume(VolumeUnit::mL);
  double TotalPulmonaryVeinVolume_mL = LeftPulmonaryVeinVolume_mL + RightPulmonaryVeinVolume_mL;
  double LeftPulmonaryVeinPressure_mmHg = m_LeftPulmonaryVeins->GetPressure(PressureUnit::mmHg);
  double RightPulmonaryVeinPressure_mmHg = m_RightPulmonaryVeins->GetPressure(PressureUnit::mmHg);

  double PulmonaryArteryNodePressure_mmHg = (LeftPulmonaryArteryVolume_mL * LeftPulmonaryArteryPressure_mmHg + RightPulmonaryArteryVolume_mL * RightPulmonaryArteryPressure_mmHg) / TotalPulmonaryArteryVolume_mL;
  double PulmVeinNodePressure_mmHg = (LeftPulmonaryVeinVolume_mL * LeftPulmonaryVeinPressure_mmHg + RightPulmonaryVeinVolume_mL * RightPulmonaryVeinPressure_mmHg) / TotalPulmonaryVeinVolume_mL;
  double PulmCapFlow_mL_Per_s = m_LeftPulmonaryArteriesToCapillaries->GetNextFlow(VolumePerTimeUnit::mL_Per_s)
    + m_RightPulmonaryArteriesToCapillaries->GetNextFlow(VolumePerTimeUnit::mL_Per_s);
  double PulmShuntFlow_mL_Per_s = m_LeftPulmonaryArteriesToVeins->GetNextFlow(VolumePerTimeUnit::mL_Per_s)
    + m_RightPulmonaryArteriesToVeins->GetNextFlow(VolumePerTimeUnit::mL_Per_s);

  double VenaCavaPressure_mmHg = m_VenaCava->GetPressure(PressureUnit::mmHg);

  double SkinFlow_mL_Per_s = m_pAortaToSkin->GetNextFlow(VolumePerTimeUnit::mL_Per_s);
  double LHeartFlow_mL_Per_s = m_LeftHeartToAorta->GetNextFlow(VolumePerTimeUnit::mL_Per_s);
  double LHeartVolume_mL = m_LeftHeart->GetVolume(VolumeUnit::mL);

  double muscleFlow_mL_Per_s = m_pAortaToMuscle->GetNextFlow(VolumePerTimeUnit::mL_Per_s);

  double gutFlow_mL_Per_s = m_pAortaToLargeIntestine->GetNextFlow(VolumePerTimeUnit::mL_Per_s) + m_pAortaToSmallIntestine->GetNextFlow(VolumePerTimeUnit::mL_Per_s) + m_pAortaToSplanchnic->GetNextFlow(VolumePerTimeUnit::mL_Per_s);

  // Calculate heart rate - Threshold of 0.1 is empirically determined. Approximate zero makes it too noisy.
  if (LHeartFlow_mL_Per_s > 0.1 && !m_HeartFlowDetected) {
    m_HeartFlowDetected = true;
    CalculateHeartRate();
    RecordAndResetCardiacCycle();
  }
  if (LHeartFlow_mL_Per_s < 0.1 && m_HeartFlowDetected)
    m_HeartFlowDetected = false;

  // Record high and low values to compute for systolic and diastolic pressures:
  if (AortaNodePressure_mmHg > m_CardiacCycleAortaPressureHigh_mmHg)
    m_CardiacCycleAortaPressureHigh_mmHg = AortaNodePressure_mmHg;
  if (AortaNodePressure_mmHg < m_CardiacCycleAortaPressureLow_mmHg)
    m_CardiacCycleAortaPressureLow_mmHg = AortaNodePressure_mmHg;
  if (PulmonaryArteryNodePressure_mmHg > m_CardiacCyclePulmonaryArteryPressureHigh_mmHg)
    m_CardiacCyclePulmonaryArteryPressureHigh_mmHg = PulmonaryArteryNodePressure_mmHg;
  if (PulmonaryArteryNodePressure_mmHg < m_CardiacCyclePulmonaryArteryPressureLow_mmHg)
    m_CardiacCyclePulmonaryArteryPressureLow_mmHg = PulmonaryArteryNodePressure_mmHg;
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Get Max of Left Ventricle Volume over the course of a heart beat for end diastolic volume
  if (LHeartVolume_mL > m_CardiacCycleDiastolicVolume_mL)
    m_CardiacCycleDiastolicVolume_mL = LHeartVolume_mL;

  // Increment stroke volume. Get samples for running means
  m_CardiacCycleStrokeVolume_mL += LHeartFlow_mL_Per_s * m_dT_s;
  m_CardiacCycleArterialPressure_mmHg.Sample(AortaNodePressure_mmHg);
  m_CardiacCycleArterialCO2PartialPressure_mmHg.Sample(AortaNodeCO2PartialPressure_mmHg);
  m_CardiacCyclePulmonaryCapillariesWedgePressure_mmHg.Sample(PulmVeinNodePressure_mmHg);
  m_CardiacCyclePulmonaryCapillariesFlow_mL_Per_s.Sample(PulmCapFlow_mL_Per_s);
  m_CardiacCyclePulmonaryShuntFlow_mL_Per_s.Sample(PulmShuntFlow_mL_Per_s);
  m_CardiacCyclePulmonaryArteryPressure_mmHg.Sample(PulmonaryArteryNodePressure_mmHg);
  m_CardiacCycleCentralVenousPressure_mmHg.Sample(VenaCavaPressure_mmHg);
  m_CardiacCycleSkinFlow_mL_Per_s.Sample(SkinFlow_mL_Per_s);

  /// \todo Make sure irreversible state is hit before we get here.
  if (m_CardiacCycleAortaPressureLow_mmHg < -2.0) {
    Fatal("Diastolic pressure has fallen below zero.");
    /// \error Fatal: Diastolic pressure has fallen below -2
  }
  if (m_CardiacCycleAortaPressureHigh_mmHg > 700.0) {
    Fatal("Systolic pressure has exceeded physiologic range.");
    /// \error Fatal: Systolic pressure has exceeded 700
  }

  // Pressures\Flows from circuit
  GetArterialPressure().SetValue(AortaNodePressure_mmHg, PressureUnit::mmHg);
  GetPulmonaryArterialPressure().SetValue(PulmonaryArteryNodePressure_mmHg, PressureUnit::mmHg);
  GetCentralVenousPressure().SetValue(VenaCavaPressure_mmHg, PressureUnit::mmHg);
  GetCerebralBloodFlow().Set(m_CirculatoryCircuit->GetPath(BGE::CerebralPath::CerebralCapillariesToCerebralVeins1)->GetFlow());
  if (m_data.GetConfiguration().IsCerebralEnabled()) {
    GetIntracranialPressure().Set(m_CirculatoryCircuit->GetNode(BGE::CerebralNode::SpinalFluid)->GetPressure());
    GetCerebralPerfusionPressure().SetValue(GetMeanArterialPressure(PressureUnit::mmHg) - GetIntracranialPressure(PressureUnit::mmHg), PressureUnit::mmHg);
  } else {
    GetIntracranialPressure().Set(m_Brain->GetPressure());
    GetCerebralPerfusionPressure().SetValue(GetMeanArterialPressure(PressureUnit::mmHg) - GetIntracranialPressure(PressureUnit::mmHg), PressureUnit::mmHg);
  }

  if (m_data.GetState() > EngineState::InitialStabilization) { // Don't throw events if we are initializing

    // Check for hypovolemic shock
    /// \event Patient: Hypovolemic Shock: blood volume below 70% of its normal value
    if (GetBloodVolume().GetValue(VolumeUnit::mL) <= (m_data.GetConfiguration().GetMinimumBloodVolumeFraction() * m_patient->GetBloodVolumeBaseline(VolumeUnit::mL))) {
      m_patient->SetEvent(CDM::enumPatientEvent::HypovolemicShock, true, m_data.GetSimulationTime());

      /// \event Patient: blood loss below 60%, irreversible state enacted
      // @cite champion2003profile
      if (GetBloodVolume().GetValue(VolumeUnit::mL) <= 0.5 * m_patient->GetBloodVolumeBaseline(VolumeUnit::mL)) {
        m_ss << "50% of the patient's blood volume has been lost. The patient is now in an irreversible state.";
        Warning(m_ss);
        /// \irreversible Over half the patients blood volume has been lost.
        if (!m_PatientActions->HasOverride()) {
          m_patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        } else {
          if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
            m_patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
          }
        }
      }
    } else {
      m_patient->SetEvent(CDM::enumPatientEvent::HypovolemicShock, false, m_data.GetSimulationTime());
    }

    //Check for cardiogenic shock
    if (GetCardiacIndex().GetValue(VolumePerTimeAreaUnit::L_Per_min_m2) < 2.2 && GetSystolicArterialPressure(PressureUnit::mmHg) < 90.0 && GetPulmonaryCapillariesWedgePressure(PressureUnit::mmHg) > 15.0) {
      /// \event Patient: Cardiogenic Shock: Cardiac Index has fallen below 2.2 L/min-m^2, Systolic Arterial Pressure is below 90 mmHg, and Pulmonary Capillary Wedge Pressure is above 15.0.
      /// \cite dhakam2008review
      m_patient->SetEvent(CDM::enumPatientEvent::CardiogenicShock, true, m_data.GetSimulationTime());
    } else {
      m_patient->SetEvent(CDM::enumPatientEvent::CardiogenicShock, false, m_data.GetSimulationTime());
    }

    //Check for Tachycardia, Bradycardia, and asystole
    /// \event Patient: Tachycardia: heart rate exceeds 100 beats per minute.  This state is alleviated if it decreases below 90.
    if (GetHeartRate().GetValue(FrequencyUnit::Per_min) < 90)
      m_patient->SetEvent(CDM::enumPatientEvent::Tachycardia, false, m_data.GetSimulationTime());
    if (GetHeartRate().GetValue(FrequencyUnit::Per_min) > 100)
      m_patient->SetEvent(CDM::enumPatientEvent::Tachycardia, true, m_data.GetSimulationTime());
    /// \event Patient: Bradycardia: heart rate falls below 60 beats per minute.  This state is alleviated if it increases above 65.
    if (GetHeartRate().GetValue(FrequencyUnit::Per_min) < 60)
      m_patient->SetEvent(CDM::enumPatientEvent::Bradycardia, true, m_data.GetSimulationTime());
    if (GetHeartRate().GetValue(FrequencyUnit::Per_min) > 65)
      m_patient->SetEvent(CDM::enumPatientEvent::Bradycardia, false, m_data.GetSimulationTime());
    if (GetHeartRate().GetValue(FrequencyUnit::Per_min) > 30) {
      if (GetHeartRhythm() != CDM::enumHeartRhythm::Asystole) {
        m_patient->SetEvent(CDM::enumPatientEvent::Asystole, false, m_data.GetSimulationTime());
      }
    }
    ///\event Patient: Asystole: Heart Rate has fallen below minimum value and is being set to 0.
    // @cite guinness2005lowest
    if (GetHeartRate().GetValue(FrequencyUnit::Per_min) < 27) {
      if (!m_PatientActions->HasOverride()) {
        m_patient->SetEvent(CDM::enumPatientEvent::Asystole, true, m_data.GetSimulationTime());
        SetHeartRhythm(CDM::enumHeartRhythm::Asystole);
      } else {
        if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
          m_patient->SetEvent(CDM::enumPatientEvent::Asystole, true, m_data.GetSimulationTime());
          SetHeartRhythm(CDM::enumHeartRhythm::Asystole);
        }
      }
    }
  }

  // Irreversible state if asystole persists.
  if (GetHeartRhythm() == CDM::enumHeartRhythm::Asystole) {
    m_patient->SetEvent(CDM::enumPatientEvent::Asystole, true, m_data.GetSimulationTime());

    /// \event Patient: Irreversible State: heart has been in asystole for over 45 min:
    if (m_patient->GetEventDuration(CDM::enumPatientEvent::Asystole, TimeUnit::s) > 2700.0) // \cite: Zijlmans2002EpilepticSeizuresAsystole
    {
      m_ss << "Asystole has occurred for " << m_patient->GetEventDuration(CDM::enumPatientEvent::Asystole, TimeUnit::s) << " seconds, patient is in irreversible state.";
      Warning(m_ss);
      /// \irreversible Heart has been in asystole for over 45 min
      if (!m_PatientActions->HasOverride()) {
        m_patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
      } else {
        if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
          m_patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        }
      }
    }
  }

  // Compute blood volume
  double blood_mL = 0;
  for (SELiquidCompartment* cmpt : m_data.GetCompartments().GetVascularLeafCompartments()) {
    if (cmpt->HasVolume() && cmpt != m_Pericardium) //Don't include pericardium
    {
      blood_mL += cmpt->GetVolume(VolumeUnit::mL);
    }
  }
  GetBloodVolume().SetValue(blood_mL, VolumeUnit::mL);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets the systolic and diastolic pressures.
///
/// \details
/// The systemic arterial and pulmonary arterial systolic and diastolic pressures are set here.
/// The variables used to track the maximum and minimum pressures are then reset for the next cardiac cycle.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::RecordAndResetCardiacCycle()
{

  GetSystolicArterialPressure().SetValue(m_CardiacCycleAortaPressureHigh_mmHg, PressureUnit::mmHg);
  GetDiastolicArterialPressure().SetValue(m_CardiacCycleAortaPressureLow_mmHg, PressureUnit::mmHg);
  GetPulmonarySystolicArterialPressure().SetValue(m_CardiacCyclePulmonaryArteryPressureHigh_mmHg, PressureUnit::mmHg);
  GetPulmonaryDiastolicArterialPressure().SetValue(m_CardiacCyclePulmonaryArteryPressureLow_mmHg, PressureUnit::mmHg);
  GetPulsePressure().SetValue(m_CardiacCycleAortaPressureHigh_mmHg - m_CardiacCycleAortaPressureLow_mmHg, PressureUnit::mmHg);

  m_data.GetCardiovascular().GetHeartStrokeVolume().SetValue(m_CardiacCycleStrokeVolume_mL, VolumeUnit::mL);
  double ejectionFraction = 0.;
  if (m_CardiacCycleDiastolicVolume_mL > ZERO_APPROX)
    ejectionFraction = m_CardiacCycleStrokeVolume_mL / m_CardiacCycleDiastolicVolume_mL;
  GetHeartEjectionFraction().SetValue(ejectionFraction);
  GetCardiacOutput().SetValue(m_CardiacCycleStrokeVolume_mL * GetHeartRate().GetValue(FrequencyUnit::Per_min), VolumePerTimeUnit::mL_Per_min);
  GetCardiacIndex().SetValue(GetCardiacOutput().GetValue(VolumePerTimeUnit::mL_Per_min) / m_patient->GetSkinSurfaceArea(AreaUnit::m2), VolumePerTimeAreaUnit::mL_Per_min_m2);

  // Running means
  GetMeanArterialPressure().SetValue(m_CardiacCycleArterialPressure_mmHg.Value(), PressureUnit::mmHg);

  m_CardiacCycleArterialPressure_mmHg.Reset();

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  GetMeanArterialCarbonDioxidePartialPressure().SetValue(m_CardiacCycleArterialCO2PartialPressure_mmHg.Value(), PressureUnit::mmHg);
  // Mean Aterial CO2 Partial Pressure Delta
  GetMeanArterialCarbonDioxidePartialPressureDelta().SetValue(m_CardiacCycleArterialCO2PartialPressure_mmHg.Value() - m_LastCardiacCycleMeanArterialCO2PartialPressure_mmHg, PressureUnit::mmHg);
  m_LastCardiacCycleMeanArterialCO2PartialPressure_mmHg = m_CardiacCycleArterialCO2PartialPressure_mmHg.Value();
  m_CardiacCycleArterialCO2PartialPressure_mmHg.Reset();
  // Pulmonary Capillary Wedge Pressure
  GetPulmonaryCapillariesWedgePressure().SetValue(m_CardiacCyclePulmonaryCapillariesWedgePressure_mmHg.Value(), PressureUnit::mmHg);
  m_CardiacCyclePulmonaryCapillariesWedgePressure_mmHg.Reset();
  // Pulmonary Capillary Mean Flow
  GetPulmonaryMeanCapillaryFlow().SetValue(m_CardiacCyclePulmonaryCapillariesFlow_mL_Per_s.Value(), VolumePerTimeUnit::mL_Per_s);
  m_CardiacCyclePulmonaryCapillariesFlow_mL_Per_s.Reset();
  // Pulmonary Shunt Mean Flow
  GetPulmonaryMeanShuntFlow().SetValue(m_CardiacCyclePulmonaryShuntFlow_mL_Per_s.Value(), VolumePerTimeUnit::mL_Per_s);
  m_CardiacCyclePulmonaryShuntFlow_mL_Per_s.Reset();
  // Mean Pulmonary Artery Pressure
  GetPulmonaryMeanArterialPressure().SetValue(m_CardiacCyclePulmonaryArteryPressure_mmHg.Value(), PressureUnit::mmHg);
  m_CardiacCyclePulmonaryArteryPressure_mmHg.Reset();
  // Mean Central Venous Pressure
  GetMeanCentralVenousPressure().SetValue(m_CardiacCycleCentralVenousPressure_mmHg.Value(), PressureUnit::mmHg);
  m_CardiacCycleCentralVenousPressure_mmHg.Reset();
  // Mean Skin Flow
  GetMeanSkinFlow().SetValue(m_CardiacCycleSkinFlow_mL_Per_s.Value(), VolumePerTimeUnit::mL_Per_s);
  m_CardiacCycleSkinFlow_mL_Per_s.Reset();

  // Computed systemic Vascular Resistance
  double cardiacOutput_mL_Per_s = GetCardiacOutput().GetValue(VolumePerTimeUnit::mL_Per_s);
  double systemicVascularResistance_mmHg_s_Per_mL = 0.0;
  if (cardiacOutput_mL_Per_s > ZERO_APPROX)
    systemicVascularResistance_mmHg_s_Per_mL = (GetMeanArterialPressure().GetValue(PressureUnit::mmHg) - GetMeanCentralVenousPressure().GetValue(PressureUnit::mmHg)) / cardiacOutput_mL_Per_s;
  GetSystemicVascularResistance().SetValue(systemicVascularResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);

  // Computed pulmonary Vascular Resistances
  if (cardiacOutput_mL_Per_s == 0.0) {
    GetPulmonaryVascularResistance().SetValue(0.0, FlowResistanceUnit::mmHg_min_Per_mL);
    GetPulmonaryVascularResistanceIndex().SetValue(0.0, PressureTimePerVolumeAreaUnit::mmHg_min_Per_mL_m2);
  } else {
    GetPulmonaryVascularResistance().SetValue((m_MainPulmonaryArteries->GetNextPressure(PressureUnit::mmHg) - m_LeftAtrium1->GetNextPressure(PressureUnit::mmHg)) / cardiacOutput_mL_Per_s, FlowResistanceUnit::mmHg_s_Per_mL);
    GetPulmonaryVascularResistanceIndex().SetValue(GetPulmonaryVascularResistance(FlowResistanceUnit::mmHg_min_Per_mL) / m_patient->GetSkinSurfaceArea(AreaUnit::m2), PressureTimePerVolumeAreaUnit::mmHg_min_Per_mL_m2);
  }

  m_CardiacCycleAortaPressureHigh_mmHg = 0.0;
  m_CardiacCycleAortaPressureLow_mmHg = 10000.0;
  m_CardiacCyclePulmonaryArteryPressureHigh_mmHg = 0.0;
  m_CardiacCyclePulmonaryArteryPressureLow_mmHg = 10000.0;
  m_CardiacCycleDiastolicVolume_mL = 0;
  m_CardiacCycleStrokeVolume_mL = 0;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Holds the action methods for the cardiovascular system.
///
/// \details
/// This method holds the actions for the CV system so that only one
/// method need be called in preprocess.
/// <ul>
///   <li> Hemorrhage </li>
///   <li> Pericardial Effusion </li>
///   <li> CPR </li>
///   <li> CardiacArrest </li>
/// </ul>
//--------------------------------------------------------------------------------------------------
void Cardiovascular::ProcessActions()
{
  TraumaticBrainInjury();
  Hemorrhage();
  PericardialEffusion();
  CPR();
  CardiacArrest();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// The traumatic brain injury action increases flow resistance in the brain
///
/// \details
/// The user may specify a brain injury of varying severity to apply during runtime. The brain resistance
/// is scaled based on severity, which impacts flow and pressure in the brain, simulating the effects of
/// a non-localized brain injury.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::TraumaticBrainInjury()
{
  if (!m_data.GetActions().GetPatientActions().HasBrainInjury())
    return;

  //Grab info about the injury
  SEBrainInjury* b = m_data.GetActions().GetPatientActions().GetBrainInjury();
  double severity = b->GetSeverity().GetValue();

  SEFluidCircuitPath* brainToSpinal = m_CirculatoryCircuit->GetPath(BGE::CerebralPath::CerebralArteries2ToSpinalFluid);

  //Interpolate linearly between multipliers of 1 (for severity of 0) to max (for severity of 1)
  //These multipliers are chosen to result in ICP > 25 mmHg and CBF < 1.8 mL/s
  double usMult = GeneralMath::LinearInterpolator(0, 1, 1, 4.87814, severity);
  double dsMult = GeneralMath::LinearInterpolator(0, 1, 1, 30.7993, severity);

  //We don't want to shoot the resistances up to their new values in a single time step because this causes very sharp changes in cerebral metrics
  //Use a simple proportional control to ramp up to target resistances.  Time constant determined empirically based on previous baselines
  double timeConstant_s = 300.0 * severity; //Otherwise the low severity scenarios go too slowly
  if (severity < ZERO_APPROX) {
    timeConstant_s = 300.0; //This function should not get evaluated once the TBI is removed, but put this here just in case something changes so we don't get a divide by 0 NaN
  }
  double targetUpstream = m_pBrainResistanceUpstream->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL) * usMult;
  double lastUpstream = m_pBrainResistanceUpstream->GetResistance(FlowResistanceUnit::mmHg_s_Per_mL);
  double upstreamDelta = (targetUpstream - lastUpstream) * m_dT_s / timeConstant_s;
  double targetDownstream = m_pBrainResistanceDownstream->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL) * dsMult;
  double lastDownstream = m_pBrainResistanceDownstream->GetResistance(FlowResistanceUnit::mmHg_s_Per_mL);
  double downstreamDelta = (targetDownstream - lastDownstream) * m_dT_s / timeConstant_s;

  double intracranialBaseline = brainToSpinal->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
  double targetSpinal = GeneralMath::ResistanceFunction(10.0, intracranialBaseline / 5.0, intracranialBaseline, severity);
  double lastSpinal = brainToSpinal->GetResistance(FlowResistanceUnit::mmHg_s_Per_mL);
  double spinalDelta = (targetSpinal - lastSpinal) * m_dT_s / timeConstant_s;

  m_pBrainResistanceDownstream->GetNextResistance().SetValue(lastDownstream + downstreamDelta, FlowResistanceUnit::mmHg_s_Per_mL);
  m_pBrainResistanceUpstream->GetNextResistance().SetValue(lastUpstream + upstreamDelta, FlowResistanceUnit::mmHg_s_Per_mL);
  brainToSpinal->GetNextResistance().SetValue(lastSpinal + spinalDelta, FlowResistanceUnit::mmHg_s_Per_mL);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// The hemorrhage function simulates bleeding from a specified compartment
/// \details
/// The cardiovascular circuit has paths from each compartment to ground (initially open) that represent
/// bleeding sites.  When a hemorrhage is activated, the current pressure at the location and the
/// user specified bleed rate are used to determine a resistance for the appropriate bleeding path.
/// A resistance is set (rather than a flow source) so that pulsatile flow characteristics can be
/// observed and so that bleeding rate diminishes with blood volume.
/// Tourniquets can be applied to the extremities, which increase the resistance on the paths into and
/// out of the heomrrhage compartment.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::Hemorrhage()
{
  /// \todo Enforce limits and remove fatal errors.
  SEHemorrhage* targetHemorrhage;
  SETourniquet* tourniquet;
  std::string hemCmpt;
  std::string tournCmpt;
  SEFluidCircuitPath* targetPath = nullptr;

  //Values for tracking physiological metrics
  double resistance = 0.0;
  double locationPressure_mmHg = 0.0;
  double TotalLossRate_mL_Per_s = 0.0;
  double TotalLoss_mL = 0;
  double probabilitySurvival = 0.0;
  double bleedoutTime = 0.0;
  double bleedRate_mL_Per_s = 0.0;
  double drugFlowResistance = 0.0;

  const double bloodVolume_mL = GetBloodVolume(VolumeUnit::mL);
  const double baselineBloodVolume_mL = m_patient->GetBloodVolumeBaseline(VolumeUnit::mL);

  const std::map<std::string, SEHemorrhage*>& hems = m_data.GetActions().GetPatientActions().GetHemorrhages();
  const std::map<std::string, SETourniquet*>& tourniquets = m_data.GetActions().GetPatientActions().GetTourniquets();
  for (auto hem : hems) {
    targetHemorrhage = hem.second;
    hemCmpt = targetHemorrhage->GetCompartment();

    targetPath = m_CirculatoryCircuit->GetPath(hemCmpt + "Bleed");
    locationPressure_mmHg = targetPath->GetSourceNode().GetPressure(PressureUnit::mmHg);
    bleedRate_mL_Per_s = targetHemorrhage->GetInitialRate().GetValue(VolumePerTimeUnit::mL_Per_s);

    //enforce strict restictions on bleed rate for circuit stability
    if (bleedRate_mL_Per_s > m_data.GetCardiovascular().GetCardiacOutput(VolumePerTimeUnit::mL_Per_s)) {
      std::stringstream err;
      err << "bleed rate (mL/s): " << bleedRate_mL_Per_s << " is higher than the cardiac output (mL/s): ("
          << m_data.GetCardiovascular().GetCardiacOutput(VolumePerTimeUnit::mL_Per_s) << ")";
      Info(err);
      Fatal("Cannot set a bleed rate higher than the cardiac output");
      return;
    }

    if (!targetHemorrhage->HasBleedResistance()) {
      if (targetHemorrhage->GetCompartment() == "Aorta") {
        targetHemorrhage->GetBleedResistance().SetValue((locationPressure_mmHg / bleedRate_mL_Per_s) * 1.1, FlowResistanceUnit::mmHg_s_Per_mL);
      } else {
        targetHemorrhage->GetBleedResistance().SetValue((locationPressure_mmHg / bleedRate_mL_Per_s), FlowResistanceUnit::mmHg_s_Per_mL);
      }
    }
    resistance = targetHemorrhage->GetBleedResistance().GetValue(FlowResistanceUnit::mmHg_s_Per_mL);

    // Use hemorrhage flow modifier to affect hemorrhage resistance path, negative modifier INCREASES resistance thus DECREASES flow out of body
    // Then set to resistance path AND next resistance to ensure stacked effect over time
    drugFlowResistance = resistance * (1 - m_data.GetDrugs().GetHemorrhageChange().GetValue());
    targetHemorrhage->GetBleedResistance().SetValue(drugFlowResistance, FlowResistanceUnit::mmHg_s_Per_mL);
    targetPath->GetNextResistance().SetValue(drugFlowResistance, FlowResistanceUnit::mmHg_s_Per_mL);

    TotalLossRate_mL_Per_s += targetPath->GetFlow(VolumePerTimeUnit::mL_Per_s);
    TotalLoss_mL += targetPath->GetFlow(VolumePerTimeUnit::mL_Per_s) * m_dT_s;
    bleedoutTime = (bloodVolume_mL - (0.5 * baselineBloodVolume_mL)) / TotalLossRate_mL_Per_s * (1.0 / 60.0);
  }

  //We already filtered tourniquet actions during Action Loading to make sure that we do not have a tourniquet that A) Aligns with a non-existent hemorrhage
  //or B) refers to an incompatible compartments.  We are therefore safe to loop through this map without further checks.
  
  for (auto tPair : tourniquets) {
    tournCmpt = tPair.first;
    tourniquet = tPair.second;
    CDM::enumTourniquetApplicationLevel::value tLevel = tourniquet->GetTourniquetLevel();
    //Take advantage of the fact that extremities are all named as Aorta1ToLeftArm1 and LeftArm1ToLeftArm2
    std::string supplyPathName = "Aorta1To" + tournCmpt + "1";
    std::string returnPathName = tournCmpt + "1To" + tournCmpt + "2";
    SEFluidCircuitPath* supplyPath = m_CirculatoryCircuit->GetPath(supplyPathName);
    const double supplyBaseResistance = supplyPath->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
    SEFluidCircuitPath* returnPath = m_CirculatoryCircuit->GetPath(returnPathName);
    const double returnBaseResistance = returnPath->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
    double tResModifier;
    switch (tLevel) {
    case CDM::enumTourniquetApplicationLevel::Applied:
      tResModifier = 100.0;
      break;
    case CDM::enumTourniquetApplicationLevel::Misapplied:
      tResModifier = 3.0; //Slow blood flow, but don't stop entirely
      break;
    case CDM::enumTourniquetApplicationLevel::None:
      //This case shouldn't get hit because "None" deactivates tourniquet, but account for it just in case
    default:
      tResModifier = 1.0;
    }
    supplyPath->GetNextResistance().SetValue(tResModifier * supplyBaseResistance, FlowResistanceUnit::mmHg_s_Per_mL);
    returnPath->GetNextResistance().SetValue(tResModifier * returnBaseResistance, FlowResistanceUnit::mmHg_s_Per_mL);
  }

  /*
  Stub to try to calculate a probability of survival based on the bleeding rate and approximate time to bleed out.
  if (bleedoutTime!=0)
  probabilitySurvival = 100.0-100.0*(0.9127*exp(-0.008*bleedoutTime));  //relationship from Table 5 in champion2003profile
  */

  double bloodDensity_kg_Per_mL = m_data.GetBloodChemistry().GetBloodDensity(MassPerVolumeUnit::kg_Per_mL);
  double massLost_kg = TotalLossRate_mL_Per_s * bloodDensity_kg_Per_mL * m_dT_s;
  double patientMass_kg = m_patient->GetWeight(MassUnit::kg);
  patientMass_kg -= massLost_kg;

  m_patient->GetWeight().SetValue(patientMass_kg, MassUnit::kg);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// The function initiates a flow source on the pericardium. It is used by both the action and condition.
///
/// \details
/// The pericardial effusion action may be called during run time. It initiates a flow source on the pericardium
/// which leads to increased pericardium volume. As the volume increases, a pressure source is applied to
/// the left and right heart nodes, simulating the restriction of the swelling pericardium.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::PericardialEffusion()
{
  //We need to do this here because the circuit needs to be processed to modify the compliance pressure based on the volume change
  if (m_data.GetConditions().HasChronicPericardialEffusion() && !m_data.GetActions().GetPatientActions().HasPericardialEffusion()) {
    PericardialEffusionPressureApplication();
  }

  if (!m_data.GetActions().GetPatientActions().HasPericardialEffusion())
    return;

  double complianceSlopeParameter = 0.0;
  double complianceCurveParameter = 0.0;
  double flowToPericardium_mL_per_s = 0.0;
  double flowCubed_mL3_Per_s3 = 0.0;
  double compliance_mL_Per_mmHg = 0.0;
  double intrapericardialVolume_mL = m_Pericardium->GetVolume(VolumeUnit::mL);
  double intrapericardialPressure_mmHg = m_Pericardium->GetPressure(PressureUnit::mmHg);

  double effusionRate_mL_Per_s = m_data.GetActions().GetPatientActions().GetPericardialEffusion()->GetEffusionRate().GetValue(VolumePerTimeUnit::mL_Per_s);
  if (effusionRate_mL_Per_s <= 0.1 && effusionRate_mL_Per_s > 0.0) {
    //Slow effusion
    complianceSlopeParameter = 0.4;
    complianceCurveParameter = 0.55;
  } else if (effusionRate_mL_Per_s > 0.1 && effusionRate_mL_Per_s < 1.0) {
    complianceSlopeParameter = 50;
    complianceCurveParameter = 0.1;
  } else if (effusionRate_mL_Per_s > 1.0) {
    Error("Effusion rate is out of physiologic bounds. Effusion rate is reset to 1.0 milliliters per second.");
    /// \error Effusion rate is out of physiologic bounds. Effusion rate is reset to 1.0 milliliters per second.
    effusionRate_mL_Per_s = 1.0;
    complianceSlopeParameter = 50;
    complianceCurveParameter = 0.1;
  } else if (effusionRate_mL_Per_s < 0.0) {
    Error("Cannot specify effusion rate less than zero. Effusion rate is now set to 0.0.");
    /// \error Cannot specify effusion rate less than zero. Effusion rate is now set to 0.0.
    effusionRate_mL_Per_s = 0.0;
    complianceSlopeParameter = 0.4;
    complianceCurveParameter = 0.55;
  }

  m_pGndToPericardium->GetNextFlowSource().SetValue(effusionRate_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  flowToPericardium_mL_per_s = m_pGndToPericardium->GetNextFlow(VolumePerTimeUnit::mL_Per_s);
  flowCubed_mL3_Per_s3 = flowToPericardium_mL_per_s * flowToPericardium_mL_per_s * flowToPericardium_mL_per_s;

  //Variable compliance calculation
  if (flowCubed_mL3_Per_s3 < 0.0001) {
    compliance_mL_Per_mmHg = m_pPericardiumToGnd->GetNextCompliance().GetValue(FlowComplianceUnit::mL_Per_mmHg);
  } else {
    compliance_mL_Per_mmHg = complianceSlopeParameter / flowCubed_mL3_Per_s3 - complianceCurveParameter * intrapericardialVolume_mL;
  }

  m_pPericardiumToGnd->GetNextCompliance().SetValue(compliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);

  PericardialEffusionPressureApplication();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// The CPR function controls the force applied during a chest compression action.
///
/// \details
/// The user may apply a chest compression to continue blood circulation if the heart no longer has an effective rhythm.
/// The compression can either be defined by an explicit force or by a fraction of the maximum allowable force.
/// If the compression input is a force scale then the method controls the shape of the force pulse and converts the force
/// pressure for application to the heart.
/// If the compression input is force, then the raw force is converted to pressure and applied to
/// the heart. The pressure is applied at the pressure source on the LeftHeart3ToGround and
/// RightHeart3ToGround paths in the cardiovascular circuit.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::CPR()
{
  // If a compression has started, finish it.
  if (m_CompressionRatio > 0.0) {
    if (m_data.GetActions().GetPatientActions().HasChestCompressionForceScale()) {
      Warning("Attempt to start a new compression during a previous compression. Allow more time between compressions or shorten the compression period.");
      m_data.GetActions().GetPatientActions().RemoveChestCompression();
      return;
    }

    if (m_data.GetActions().GetPatientActions().HasChestCompressionForce()) {
      Warning("Attempt to switch to explicit force from force scale during CPR compression. CPR actions will be ignored until current compression ends.");
      m_data.GetActions().GetPatientActions().RemoveChestCompression();
      return;
    }

    CalculateAndSetCPRcompressionForce();
    return;
  }
  // If there is no chest compression action and we are not currently compressing, return to ProcessActions
  if (!m_data.GetActions().GetPatientActions().HasChestCompression())
    return;

  // Call for chest compression with an effective heart rhythm
  // In the future we may allow compressions on a beating heart, but that will require extensive testing
  // to evaluate the hemodynamic stability.
  if (!m_patient->IsEventActive(CDM::enumPatientEvent::CardiacArrest)) {
    Warning("CPR attempted on beating heart. Action ignored.");
    m_data.GetActions().GetPatientActions().RemoveChestCompression();
    return;
  }

  // Have a new call for a chest compression
  if (m_data.GetActions().GetPatientActions().HasChestCompressionForceScale()) {
    m_CompressionRatio = m_data.GetActions().GetPatientActions().GetChestCompressionForceScale()->GetForceScale().GetValue();
    /// \error Warning: CPR compression ratio must be a positive value between 0 and 1 inclusive.
    if (m_CompressionRatio < 0.0)
      Warning("CPR compression ratio must be a positive value between 0 and 1 inclusive.");
    if (m_CompressionRatio > 1.0)
      Warning("CPR compression ratio must be a positive value between 0 and 1 inclusive.");

    BLIM(m_CompressionRatio, 0., 1.);
    // If no period was assigned by the user, then use the default - 0.4s
    if (m_data.GetActions().GetPatientActions().GetChestCompressionForceScale()->HasForcePeriod()) {
      m_CompressionPeriod_s = m_data.GetActions().GetPatientActions().GetChestCompressionForceScale()->GetForcePeriod().GetValue(TimeUnit::s);
    } else {
      m_CompressionPeriod_s = 0.4;
    }

    m_data.GetActions().GetPatientActions().RemoveChestCompression();
  }

  CalculateAndSetCPRcompressionForce();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates and sets the pressure on the heart pressure sources when a CPR compression is applied.
///
/// \details
/// Calculates and sets the pressure on the heart pressure sources when a CPR compression is applied.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::CalculateAndSetCPRcompressionForce()
{

  double compressionForce_N = 0.0;
  double compressionForceMax_N = 500.0; // The maximum allowed compression force (corresponds to 1.0 when force scale is used)
  double compressionForceMin_N = 0.0; // The minimum allowed compression force

  if (m_CompressionRatio > 0.0) //Force scale
  {
    // Bell curve shaping parameters
    double c = -10; // Defines the start and stop of the force bell curve given the period
    double a = 4 * c / (m_CompressionPeriod_s * m_CompressionPeriod_s);
    double b = -a * m_CompressionPeriod_s;

    compressionForce_N = std::pow(2, a * m_CompressionTime_s * m_CompressionTime_s + b * m_CompressionTime_s + c) * m_CompressionRatio * compressionForceMax_N;

    // 2 second max compression time is arbitrary. I just put it in to make sure it doesn't get stuck if
    // we accidentally make a really wide bell curve. Note that the bell curve parameters are currently hardcoded above.
    // If compression force has decayed to less than some amount or the time is above some amount, end the compression
    if (m_CompressionTime_s > m_CompressionPeriod_s) {
      compressionForce_N = 0.0;
      m_CompressionTime_s = 0.0;
      m_CompressionRatio = 0.0;
      m_CompressionPeriod_s = 0.0;
    }
  } else //Explicit force
  {
    compressionForce_N = m_data.GetActions().GetPatientActions().GetChestCompressionForce()->GetForce().GetValue(ForceUnit::N);
  }

  m_CompressionTime_s += m_dT_s;

  if (compressionForce_N > compressionForceMax_N) {
    compressionForce_N = compressionForceMax_N;
    Warning("The compression force exceeded the maximum compression force. Compression force limited to 500N.");
  }

  if (compressionForce_N < compressionForceMin_N) {
    compressionForce_N = compressionForceMin_N;
    Warning("The compression force was less than the required minimum. Compression force limited to 0N.");
  }

  double leftHeartForceToPressureFactor = 0.1; // Tuning parameter to translate compression force in N to left heart pressure in mmHg
  double rightHeartForceToPressureFactor = 0.1; // Tuning parameter to translate compression force in N to right heart pressure in mmHg
  double nextLeftPressure_mmHg = leftHeartForceToPressureFactor * compressionForce_N;
  double nextRightPressure_mmHg = rightHeartForceToPressureFactor * compressionForce_N;

  m_pRightHeartToGnd->GetNextPressureSource().SetValue(nextRightPressure_mmHg, PressureUnit::mmHg);
  m_pLeftHeartToGnd->GetNextPressureSource().SetValue(nextLeftPressure_mmHg, PressureUnit::mmHg);

  // The action is removed when the force is set to 0.
  if (compressionForce_N == 0)
    m_data.GetActions().GetPatientActions().RemoveChestCompression();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// The cardiac arrest action causes the sudden loss of heart function and breathing.
///
/// \details
/// Cardiac arrest is the sudden loss of effective blood circulation. When the cardiac arrest
/// action is active, the heart will not beat effectively and breathing will not occur.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::CardiacArrest()
{
  // If there is no call for a cardiac arrest, return to ProcessActions
  if (!m_data.GetActions().GetPatientActions().HasCardiacArrest())
    return;
  // Flip the cardiac arrest switch
  // This tells the CV system that a cardiac arrest has been initiated.
  // The cardiac arrest event will be triggered by CardiacCycleCalculations() at the end of the cardiac cycle.
  m_EnterCardiacArrest = true;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// The pericardial effusion pressure application function calculates the pressure applied to the heart due to a pericardial effusion.
///
/// \details
/// The pressure applied to the left and right heart is dictated by the pericardium pressure. The response is tuned to 40% of this value
/// to achieve the correct physiologic response.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::PericardialEffusionPressureApplication()
{
  double intrapericardialPressure_mmHg = m_Pericardium->GetPressure(PressureUnit::mmHg);

  double pressureResponseFraction = 0.4; //Tuning the pressure applied to the heart

  //Set the pressure on the right and left heart from the pericardium pressure
  m_pRightHeartToGnd->GetPressureSourceBaseline().SetValue(pressureResponseFraction * intrapericardialPressure_mmHg, PressureUnit::mmHg);
  m_pLeftHeartToGnd->GetPressureSourceBaseline().SetValue(pressureResponseFraction * intrapericardialPressure_mmHg, PressureUnit::mmHg);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the contraction and relaxation of the left and right heart during the cardiac cycle
///
/// \details
/// This function tracks the progress of the current cardiac cycle, and modifies the compliance of the left
/// and right heart to drive the cardiovascular circuit. The reduced compliance at the beginning of the cycle
/// acts to increase the pressure, driving flow out of the heart. The compliance is then reduced allowing flow into
/// the heart. This represents the systolic and diastolic portion of the cardiac cycle. The compliance is
/// driven by an elastance equation.
/// This function also keeps track of the cardiac cycle time and calls BeginCardiacCycle() at the start of
/// systole portion of each cycle. Modifications to heart rate and heart compliance are calculated by
/// BeginCardiacCycle() and applied for the remained of the current cardiac cycle. Changes to things like
/// heart rate and heart contractility can only occur at the top of the current cardiac cycle, after the last cardiac
/// cycle has completed. This helps to avoid discontinuous behavior such as the complete cessation of heart function
/// mid contraction.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::HeartDriver()
{
  // Reset start cardiac cycle event if it was activated by BeginCardiacCycle() last time step
  if (m_patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle))
    m_patient->SetEvent(CDM::enumPatientEvent::StartOfCardiacCycle, false, m_data.GetSimulationTime());

  // m_StartSystole is set to true at the end of a cardiac cycle in order to setup the next cardiac cycle.
  // After the next cycle is prepared in BeginCardiacCycle, m_StartSystole is seet back to false.
  if (m_StartSystole)
    BeginCardiacCycle();

  // If any system set the rhythm to asystole (or other rhythms in the future) then trip the cardiac arrest flag so that we can deal with it at the top of the next cardiac cycle
  // This prevents the heart from stopping in the middle of a contraction.
  if (GetHeartRhythm() == CDM::enumHeartRhythm::Asystole)
    m_EnterCardiacArrest = true;

  if (!m_patient->IsEventActive(CDM::enumPatientEvent::CardiacArrest)) {
    if (m_CurrentCardiacCycleTime_s + m_dT_s > m_CardiacCyclePeriod_s) {
      m_StartSystole = true; // A new cardiac cycle will begin next time step
      m_CurrentCardiacCycleDuration_s += (m_CardiacCyclePeriod_s - m_CurrentCardiacCycleTime_s); //Add leftover time to current duration so Calc Heart Rate has an accuracte notion of how long this cycle lasted
    }
    AdjustVascularTone();
    CalculateHeartElastance();
  }

  m_pRightHeart->GetNextCompliance().SetValue(1.0 / m_RightHeartElastance_mmHg_Per_mL, FlowComplianceUnit::mL_Per_mmHg);
  m_pLeftHeart->GetNextCompliance().SetValue(1.0 / m_LeftHeartElastance_mmHg_Per_mL, FlowComplianceUnit::mL_Per_mmHg);

  // Now that the math is done we can increment the cardiac cycle time
  // Note that the cardiac cycle time (m_CurrentCardiacCycleTime_s) continues to increment until a cardiac cycle begins (a beat happens)
  // So for a normal sinus rhythm, the maximum cardiac cycle time is equal to the cardiac cycle period (m_CardiacCyclePeriod_s).
  // For any ineffective rhythm (no heart beat) the cardiac cycle time will be as long as it has been since the last time there was an effective beat.
  // The variable m_CurrentCardiacCycleDuratoin_s tracks the time between beats and is used in CalculateHeartRate.  We use a separate counter
  // for heart rate because the conditions that trigger CalcHeartRate are different than those that trigger BeginCardiacCycle
  m_CurrentCardiacCycleTime_s += m_dT_s;
  m_CurrentCardiacCycleDuration_s += m_dT_s;
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets up the evolution of the next cardiac cycle.
///
/// \details
/// This function is directed from Cardiovascular::HeartDriver. It set's up the evolution of the proceeding cardiac
/// cycle. It is used to apply the effects of drugs or exercise on the cardiovascular system.
/// These effects will persist for the remainder of the cardiac cycle, at which point this function
/// is called again if a new heart beat is warranted (i.e. not in cardiac arrest).
//--------------------------------------------------------------------------------------------------
void Cardiovascular::BeginCardiacCycle()
{
  m_patient->SetEvent(CDM::enumPatientEvent::StartOfCardiacCycle, true, m_data.GetSimulationTime());

  // Changes to the heart rate and other hemodynamic parameters are applied at the top of the cardiac cycle.
  // Parameters cannot change during the cardiac cycle because the heart beat is modeled as a changing compliance.

  // Apply baroreceptor reflex effects
  /// \todo need to reset the heart elastance min and max at the end of each stabilization period in AtSteadyState()
  m_LeftHeartElastanceMax_mmHg_Per_mL = m_data.GetConfiguration().GetLeftHeartElastanceMaximum(FlowElastanceUnit::mmHg_Per_mL);
  m_RightHeartElastanceMax_mmHg_Per_mL = m_data.GetConfiguration().GetRightHeartElastanceMaximum(FlowElastanceUnit::mmHg_Per_mL);
  if (m_data.GetNervous().HasHeartElastanceScale()) {
    m_LeftHeartElastanceMax_mmHg_Per_mL *= m_data.GetNervous().GetHeartElastanceScale().GetValue();
  }
  if (m_data.GetNervous().HasHeartElastanceScale()) {
    m_RightHeartElastanceMax_mmHg_Per_mL *= m_data.GetNervous().GetHeartElastanceScale().GetValue();
  }

  // Current cardiovascular metrics that could be subject to overrides
  double meanArterialPressureOverride_mmHg = GetMeanArterialPressure().GetValue(PressureUnit::mmHg);
  double meanArterialPressureChange_pct = 0.0;
  double systolicOverride_mmHg = GetSystolicArterialPressure().GetValue(PressureUnit::mmHg);
  double diastolicOverride_mmHg = GetDiastolicArterialPressure().GetValue(PressureUnit::mmHg);
  double HeartDriverFrequency_Per_Min = m_patient->GetHeartRateBaseline(FrequencyUnit::Per_min);


  if (m_data.GetActions().GetPatientActions().HasOverride() && m_data.GetActions().GetPatientActions().GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
    SEOverride* override = m_data.GetActions().GetPatientActions().GetOverride();
    m_OverrideOnOffCheck = true;
    m_overrideTime_s += m_data.GetTimeStep().GetValue(TimeUnit::s);
    //Blood pressure override processing
    if (override->HasSystolicArterialPressureOverride()) {
      systolicOverride_mmHg = override->GetSystolicArterialPressureOverride().GetValue(PressureUnit::mmHg);
    }
    if (override->HasDiastolicArterialPressureOverride()) {
      diastolicOverride_mmHg = override->GetDiastolicArterialPressureOverride().GetValue(PressureUnit::mmHg);
    }
    if (systolicOverride_mmHg > GetSystolicArterialPressure().GetValue(PressureUnit::mmHg) && diastolicOverride_mmHg < GetDiastolicArterialPressure().GetValue(PressureUnit::mmHg)) {
      Warning("Cannot drive systolic and diastolic values away from each other and remain conformant. Pushing forward with systolic override only.");
      diastolicOverride_mmHg = GetDiastolicArterialPressure().GetValue(PressureUnit::mmHg);
      override->GetDiastolicArterialPressureOverride().Invalidate();
    }
    if (override->HasMAPOverride()) {
      meanArterialPressureOverride_mmHg = override->GetMAPOverride().GetValue(PressureUnit::mmHg);
      meanArterialPressureChange_pct = 1. + ((meanArterialPressureOverride_mmHg - GetMeanArterialPressure().GetValue(PressureUnit::mmHg)) / GetMeanArterialPressure().GetValue(PressureUnit::mmHg));
      systolicOverride_mmHg = GetSystolicArterialPressure().GetValue(PressureUnit::mmHg) * meanArterialPressureChange_pct;
      diastolicOverride_mmHg = GetDiastolicArterialPressure().GetValue(PressureUnit::mmHg) * meanArterialPressureChange_pct;
      override->GetSystolicArterialPressureOverride().SetValue(systolicOverride_mmHg, PressureUnit::mmHg);
      override->GetDiastolicArterialPressureOverride().SetValue(diastolicOverride_mmHg, PressureUnit::mmHg);
    }
    if (systolicOverride_mmHg < diastolicOverride_mmHg) {
      Fatal("Systolic and Diastolic Pressure Override Values Do Not Make Sense!");
    }
    //Apply blood pressure overrides incrementally
    const double bpChange_Per_timestep = 0.005;
    if (systolicOverride_mmHg > GetSystolicArterialPressure(PressureUnit::mmHg)) {
      m_LeftHeartElastanceMin_mmHg_Per_mL *= 1. - (0.25 * bpChange_Per_timestep);
      m_RightHeartElastanceMin_mmHg_Per_mL *= 1. - (0.5 * bpChange_Per_timestep);
      m_LeftHeartElastanceMax_mmHg_Per_mL *= 1. + (0.25 * bpChange_Per_timestep);
      m_RightHeartElastanceMax_mmHg_Per_mL *= 1. - (0.5 * bpChange_Per_timestep);
    } else if (systolicOverride_mmHg < GetSystolicArterialPressure(PressureUnit::mmHg)) {
      m_LeftHeartElastanceMin_mmHg_Per_mL *= 1. + bpChange_Per_timestep;
      m_LeftHeartElastanceMax_mmHg_Per_mL *= 1. - bpChange_Per_timestep;
    }
    if (diastolicOverride_mmHg > GetDiastolicArterialPressure(PressureUnit::mmHg)) {
      m_LeftHeartElastanceMin_mmHg_Per_mL *= 1. - (0.25 * bpChange_Per_timestep);
      m_RightHeartElastanceMin_mmHg_Per_mL *= 1. - (0.5 * bpChange_Per_timestep);
      m_LeftHeartElastanceMax_mmHg_Per_mL *= 1. + (0.25 * bpChange_Per_timestep);
      m_RightHeartElastanceMax_mmHg_Per_mL *= 1. - (0.5 * bpChange_Per_timestep);
    } else if (diastolicOverride_mmHg < GetDiastolicArterialPressure(PressureUnit::mmHg)) {
      m_RightHeartElastanceMin_mmHg_Per_mL *= 1. + bpChange_Per_timestep;
      m_RightHeartElastanceMax_mmHg_Per_mL *= 1. - bpChange_Per_timestep;
    }
    //Heart rate override
    if (override->HasHeartRateOverride()) {
      HeartDriverFrequency_Per_Min = m_OverrideHR_Conformant_Per_min;
      const double HRoverride_Per_min = m_data.GetActions().GetPatientActions().GetOverride()->GetHeartRateOverride(FrequencyUnit::Per_min);
      const double HeartDriverIncrease_Per_min = std::abs((HRoverride_Per_min - HeartDriverFrequency_Per_Min) / HeartDriverFrequency_Per_Min) * (HRoverride_Per_min - HeartDriverFrequency_Per_Min);
      HeartDriverFrequency_Per_Min += HeartDriverIncrease_Per_min;
    }
  } else {
    //No conformant override present
    if (m_OverrideOnOffCheck) {
      //Was there an override that was deactivated?  If so, gradually move back to cv model output
      if (m_overrideTime_s > 0.0) {
        m_LeftHeartElastanceMin_mmHg_Per_mL = ((m_LeftHeartElastanceMin_mmHg_Per_mL) + m_OverrideLHEMin_Conformant_mmHg) / 2.;
        m_RightHeartElastanceMin_mmHg_Per_mL = ((m_RightHeartElastanceMin_mmHg_Per_mL) + m_OverrideRHEMin_Conformant_mmHg) / 2.;
        m_LeftHeartElastanceMax_mmHg_Per_mL = ((m_LeftHeartElastanceMax_mmHg_Per_mL) + m_OverrideLHEMax_Conformant_mmHg) / 2.;
        m_RightHeartElastanceMax_mmHg_Per_mL = ((m_RightHeartElastanceMax_mmHg_Per_mL) + m_OverrideRHEMax_Conformant_mmHg) / 2.;
        m_overrideTime_s -= m_data.GetTimeStep().GetValue(TimeUnit::s);
      } else {
        m_OverrideOnOffCheck = false;
      }
    }
    //Only apply nervous heart rate scale if no override is present
    if (m_data.GetNervous().HasHeartRateScale()) {
      HeartDriverFrequency_Per_Min *= (m_data.GetNervous().GetHeartRateScale().GetValue());
    }
  }

  // Apply drug effects
  if (m_data.GetDrugs().HasHeartRateChange()) {
    HeartDriverFrequency_Per_Min += m_data.GetDrugs().GetHeartRateChange(FrequencyUnit::Per_min);
  }

  BLIM(HeartDriverFrequency_Per_Min, m_data.GetPatient().GetHeartRateMinimum(FrequencyUnit::Per_min), m_data.GetPatient().GetHeartRateMaximum(FrequencyUnit::Per_min));
  m_OverrideHR_Conformant_Per_min = HeartDriverFrequency_Per_Min;
  //Apply heart failure effects
  m_LeftHeartElastanceMax_mmHg_Per_mL *= m_LeftHeartElastanceModifier;

  // Now set the cardiac cycle period and the cardiac arrest event if applicable
  if (m_EnterCardiacArrest) {
    m_patient->SetEvent(CDM::enumPatientEvent::CardiacArrest, true, m_data.GetSimulationTime());
    m_CardiacCyclePeriod_s = 1.0e9;
    RecordAndResetCardiacCycle();
    GetHeartRate().SetValue(0.0, FrequencyUnit::Per_min);
  } else {
    if (HeartDriverFrequency_Per_Min == 0) {
      m_CardiacCyclePeriod_s = 1.0e9; // Cannot divide by zero so set the period to a large number (1.0e9 sec = 31.7 years)
    } else {
      m_CardiacCyclePeriod_s = 60.0 / HeartDriverFrequency_Per_Min;
      if ((m_CardiacCyclePeriod_s < (1.0 / m_patient->GetHeartRateBaseline(FrequencyUnit::Per_s)) + m_dT_s) && (m_data.GetState() <= EngineState::AtSecondaryStableState)) {
        // A deviation of < 1 time step in cycle period can introduce undamped oscillations in HR during stabilization.  Make sure that any changes to HeartDriverFrequency
        // are greater than 1 time step during stabilization -- this will still allow conditions to affect change
        m_CardiacCyclePeriod_s = 1.0 / m_patient->GetHeartRateBaseline(FrequencyUnit::Per_s);
      }
    }
  }

  // Reset the systole flag and the cardiac cycle time
  m_StartSystole = false;
  m_CurrentCardiacCycleTime_s = 0.0;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculation of the left and right ventricular elastance
///
/// \details
/// This function calculates the left and right ventricular elastance at the current time in the cardiac cycle.
/// The elastance takes the form of a double hill function with a period equivalent to the cardiac cycle length.
/// \cite stergiopulos1996elastance
//--------------------------------------------------------------------------------------------------
void Cardiovascular::CalculateHeartElastance()
{
  //Shape parameters, used to define double hill functional form of the elastance
  double alpha1 = 0.303;
  double alpha2 = 0.508;
  double n1 = 1.32;
  double n2 = 21.9;
  double maxShape = 0.598;

  double normalizedCardiacTime = m_CurrentCardiacCycleTime_s / m_CardiacCyclePeriod_s;
  double elastanceShapeFunction = (std::pow(normalizedCardiacTime / alpha1, n1) / (1.0 + std::pow(normalizedCardiacTime / alpha1, n1))) * (1.0 / (1.0 + std::pow(normalizedCardiacTime / alpha2, n2))) / maxShape;

  m_LeftHeartElastance_mmHg_Per_mL = (m_LeftHeartElastanceMax_mmHg_Per_mL - m_LeftHeartElastanceMin_mmHg_Per_mL) * elastanceShapeFunction + m_LeftHeartElastanceMin_mmHg_Per_mL;
  m_RightHeartElastance_mmHg_Per_mL = (m_RightHeartElastanceMax_mmHg_Per_mL - m_RightHeartElastanceMin_mmHg_Per_mL) * elastanceShapeFunction + m_RightHeartElastanceMin_mmHg_Per_mL;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates metabolic and thermal feedback on the systemic vascular tone
///
/// \details
/// This method controls the vascular tone response to metabolic changes. As the metabolic rate increases,
/// the resistances to non-vital organs is increased while the resistance to the muscle and skin is reduced,
/// thus increasing heat convective rejection (see Energy::UpdateHeatResistance()). In the case of decreasing core temperature
/// the resistance to the skin is increased to shunt blood flow and reduce heat transfer from the core to skin.
//--------------------------------------------------------------------------------------------------
/// \todo Possibly add a thermoreception model to nervous and move the computation there.
void Cardiovascular::MetabolicToneResponse()
{
  double metabolicFraction = 1.0;
  if (m_data.GetEnergy().HasTotalMetabolicRate()) {
    double TMR_kcal_Per_day = m_data.GetEnergy().GetTotalMetabolicRate(PowerUnit::kcal_Per_day);
    metabolicFraction = TMR_kcal_Per_day / m_patient->GetBasalMetabolicRate(PowerUnit::kcal_Per_day);
  }

  //We skip this functionality when we are at basal conditions or there is a burn present.  Burns cause hypothermia,
  //but that should be accompanied by an increase in systemic resistance.  The code below is more applicable to exercise
  //conditions (in which systemic resistance decreases) and as such causes poor behavior when modeling burns.  Future work
  //should address the difference between metabolic disturbances caused by exercise and those caused by hypothermia.
  if (metabolicFraction == 1.0 || m_data.GetActions().GetPatientActions().HasBurnWound())
    return;

  double coreTemp_degC = m_data.GetEnergy().GetCoreTemperature(TemperatureUnit::C); //Resting: 37.0 degC
  double coreTempSet_degC = m_data.GetConfiguration().GetCoreTemperatureHigh(TemperatureUnit::C); //37.1 degC
  double coreTempDelta_degC = std::max(coreTemp_degC - coreTempSet_degC, 0.0);
  coreTempDelta_degC = std::min(coreTempDelta_degC, 1.0); //A 1 degree increase in core temperature is the where the cardiovascular response on resistances is capped
  //The skin multiplier is used to increase the skin blood flow resistance, effectively reducing the skin blood flow leading to less heat transfered from core to skin.
  double skinMultiplier = 1.0 / std::max((coreTemp_degC - 35.0), 0.001);
  double coreTempLow_degC = m_data.GetConfiguration().GetCoreTemperatureLow(TemperatureUnit::C); //36.8 degC
  /// \cite talebipour2006sauna
  double tempMultiplier = 1.0 - 0.4 * std::min(coreTempDelta_degC, 1.0); //Approximate 40% reduction in peripheral resistance due to core temperature rise of 1 degree.
  double exerciseModifier = 1.0;
  //The metabolic modifier is used as a tuned response to represent cardiovascular resistance effects during exercise
  double sp0 = 1.5;
  double divisor = 7.0;

  if (m_data.GetActions().GetPatientActions().HasExercise()) {
    //Only change this value if exercise is active (per comment above) -- otherwise this modifier can increase during hypothermia, causing incorrect decease in peripheral resistance
    exerciseModifier = (sp0 * metabolicFraction + (divisor - sp0)) / divisor;
  }
  // Max delta approx. 20% of baseline \cite christie1997cardiac \cite foster1999left
  double metabolicRateMeanArterialPressureDelta_mmHg = (0.05 * metabolicFraction - 0.05) * m_data.GetPatient().GetMeanArterialPressureBaseline(PressureUnit::mmHg);
  m_data.GetEnergy().GetExerciseMeanArterialPressureDelta().SetValue(metabolicRateMeanArterialPressureDelta_mmHg, PressureUnit::mmHg);

  //Reducing resistances scaling with metabolic rate increase and changes in core temperature
  double resistanceNew__mmHg_s_Per_mL = 0.0;
  double complianceNew_mL_Per_mmHg = 0.0;

  for (SEFluidCircuitPath* musclePath : m_muscleResistancePaths) {
    resistanceNew__mmHg_s_Per_mL = (1.0 / exerciseModifier) * (1.0 / exerciseModifier) * musclePath->GetNextResistance(FlowResistanceUnit::mmHg_s_Per_mL); //biggest change in muscle
    if (resistanceNew__mmHg_s_Per_mL < m_minIndividialSystemicResistance__mmHg_s_Per_mL) {
      resistanceNew__mmHg_s_Per_mL = m_minIndividialSystemicResistance__mmHg_s_Per_mL;
    }
    musclePath->GetNextResistance().SetValue(resistanceNew__mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  }
  for (SEFluidCircuitPath* extrasplanchnicPath : m_extrasplanchnicResistancePaths) {
    resistanceNew__mmHg_s_Per_mL = (1.0 / exerciseModifier) * extrasplanchnicPath->GetNextResistance(FlowResistanceUnit::mmHg_s_Per_mL);
    if (resistanceNew__mmHg_s_Per_mL < m_minIndividialSystemicResistance__mmHg_s_Per_mL) {
      resistanceNew__mmHg_s_Per_mL = m_minIndividialSystemicResistance__mmHg_s_Per_mL;
    }
    extrasplanchnicPath->GetNextResistance().SetValue(resistanceNew__mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  }
  for (SEFluidCircuitPath* splanchnicPath : m_splanchnicResistancePaths) {
    resistanceNew__mmHg_s_Per_mL = (1.0 / exerciseModifier) * splanchnicPath->GetNextResistance(FlowResistanceUnit::mmHg_s_Per_mL);
    if (resistanceNew__mmHg_s_Per_mL < m_minIndividialSystemicResistance__mmHg_s_Per_mL) {
      resistanceNew__mmHg_s_Per_mL = m_minIndividialSystemicResistance__mmHg_s_Per_mL;
    }
    splanchnicPath->GetNextResistance().SetValue(resistanceNew__mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  }
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Tune the cardiovascular circuit during stabilization
///
/// \details
/// Tunes the resistors and capacitors associated with tissue compartments during stabilization to achieve the requested patient parameters
//--------------------------------------------------------------------------------------------------
void Cardiovascular::TuneCircuit()
{
  DataTrack circuitTrk;
  std::ofstream circuitFile;

  DataTrack cerebralTrk;
  std::ofstream cerebralFile;

  bool success = false;
  double systolicTarget_mmHg = m_patient->GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
  double diastolicTarget_mmHg = m_patient->GetDiastolicArterialPressureBaseline(PressureUnit::mmHg);
  double heartRateTarget_bpm = m_patient->GetHeartRateBaseline(FrequencyUnit::Per_min);

  m_ss << "Tuning to patient parameters : HeartRate(bpm):" << heartRateTarget_bpm << " Systolic(mmHg):" << systolicTarget_mmHg << " Diastolic(mmHg):" << diastolicTarget_mmHg;
  Info(m_ss);

  // Tuning variables
  double pressuretolerance = 0.01;
  double stabPercentTolerance = 0.25;
  double stabCheckTime_s = 15.0;
  double srGain = 0.01; //Systemic is sensitive
  double acGain1 = 0.06; //Gains are empirical
  double acGain2 = 0.02; //Gains are empirical
  double vrGain = 0.06; //Gains are empirical

  bool stable;
  double map_mmHg = 0, tgt_map_mmHg = 0;
  double systolic_mmHg = 0, tgt_systolic_mmHg = 0;
  double diastolic_mmHg = 0, tgt_diastolic_mmHg = 0;
  double cardiacOutput_mL_Per_min = 0, tgt_cardiacOutput_mL_Per_min = 0;
  double meanCVP_mmHg = 0, tgt_meanCVP_mmHg = 0;
  double blood_mL = 0, tgt_blood_mL = 0;

  double time_s = 0;
  double timeStep_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  double stableTime_s;
  double maxTime_s = 2000;
  double maxConfigurations = 50;
  for (int i = 0; i < maxConfigurations; i++) {
    stable = false;
    stableTime_s = 0;
    while (!stable) {
      HeartDriver();
      m_circuitCalculator.Process(*m_CirculatoryCircuit, m_dT_s);
      CalculateVitalSigns();
      m_circuitCalculator.PostProcess(*m_CirculatoryCircuit);
      //return; //Skip stabelization for debugging

      map_mmHg = GetMeanArterialPressure(PressureUnit::mmHg);
      systolic_mmHg = GetSystolicArterialPressure(PressureUnit::mmHg);
      diastolic_mmHg = GetDiastolicArterialPressure(PressureUnit::mmHg);
      cardiacOutput_mL_Per_min = GetCardiacOutput(VolumePerTimeUnit::mL_Per_min);
      meanCVP_mmHg = GetMeanCentralVenousPressure(PressureUnit::mmHg);
      blood_mL = GetBloodVolume(VolumeUnit::mL);

      stableTime_s += timeStep_s;
      bool stableMAP = true;
      if (GeneralMath::PercentDifference(tgt_map_mmHg, map_mmHg) > stabPercentTolerance) {
        stableTime_s = 0;
        tgt_map_mmHg = map_mmHg;
        stableMAP = false;
      }
      bool stableSystolic = true;
      if (GeneralMath::PercentDifference(tgt_systolic_mmHg, systolic_mmHg) > stabPercentTolerance) {
        stableTime_s = 0;
        tgt_systolic_mmHg = systolic_mmHg;
        stableSystolic = false;
      }
      bool stableDiastolic = true;
      if (GeneralMath::PercentDifference(tgt_diastolic_mmHg, diastolic_mmHg) > stabPercentTolerance) {
        stableTime_s = 0;
        tgt_diastolic_mmHg = diastolic_mmHg;
        stableDiastolic = false;
      }
      bool stableCO = true;
      if (GeneralMath::PercentDifference(tgt_cardiacOutput_mL_Per_min, cardiacOutput_mL_Per_min) > stabPercentTolerance) {
        stableTime_s = 0;
        tgt_cardiacOutput_mL_Per_min = cardiacOutput_mL_Per_min;
        stableCO = false;
      }
      //bool stableMeanCVP = true;
      //if (GeneralMath::PercentDifference(tgt_meanCVP_mmHg, meanCVP_mmHg) > 0.25)
      //  { stableTime_s = 0; tgt_meanCVP_mmHg = meanCVP_mmHg; stableMeanCVP = false; }
      bool stableBloodVol = true;
      if (GeneralMath::PercentDifference(tgt_blood_mL, blood_mL) > stabPercentTolerance) {
        stableTime_s = 0;
        tgt_blood_mL = blood_mL;
        stableBloodVol = false;
      }

      if (stableTime_s > stabCheckTime_s) {
        stable = true;
        m_ss << "We are stable at " << time_s;
        Info(m_ss);
      }
      if (time_s > maxTime_s) {
        Error("Could not stabilize this configuration");
        break;
      }

      if (!m_TuningFile.empty()) {
        circuitTrk.Track(time_s, *m_CirculatoryCircuit);
        circuitTrk.Track("MAP_mmHg", time_s, map_mmHg);
        circuitTrk.Track("Systolic_mmHg", time_s, systolic_mmHg);
        circuitTrk.Track("Diastolilc_mmHg", time_s, diastolic_mmHg);
        circuitTrk.Track("MeanCVP_mmHg", time_s, meanCVP_mmHg);
        circuitTrk.Track("CardiacOutput_mL_per_s", time_s, cardiacOutput_mL_Per_min);
        circuitTrk.Track("BloodVolume_mL", time_s, blood_mL);

        if (time_s == 0) {
          circuitTrk.CreateFile(m_TuningFile.c_str(), circuitFile);
        }

        circuitTrk.StreamTrackToFile(circuitFile);
      }
      time_s += m_dT_s;
    }
    if (!m_TuneCircuit) {
      Info("Not tuning circuit");
      success = true; // Assume this is what you want
      break;
    }

    double systolicError_mmHg = systolicTarget_mmHg - systolic_mmHg;
    double diastolicError_mmHg = diastolicTarget_mmHg - diastolic_mmHg;
    if (stable) {
      // Compute the pressure errors
      if (std::abs(systolicError_mmHg / systolicTarget_mmHg) < pressuretolerance && std::abs(diastolicError_mmHg / diastolicTarget_mmHg) < pressuretolerance) //relative error check
      {
        success = true;
        break; // We met our patient parameters
      }
    }

    // This configuration did not meet the requests patient baselines, Tune the paths
    double systemicResistanceScale = 1;
    double systemicComplianceScale = 1;
    double aortaResistanceScale = 1;
    double aortaComplianceScale = 1;
    double rightHeartResistanceScale = 1;
    double venaCavaComplianceScale = 1;
    if ((systolicError_mmHg > 0 && diastolicError_mmHg > 0) || (systolicError_mmHg < 0 && diastolicError_mmHg < 0)) // Same direction
    {
      if (cardiacOutput_mL_Per_min > 4000.0) {
        systemicResistanceScale += srGain * (systolicError_mmHg + diastolicError_mmHg);
        BLIM(systemicResistanceScale, 0.5, 1.5);
      } else {
        rightHeartResistanceScale -= vrGain * (systolicError_mmHg + diastolicError_mmHg);
        systemicResistanceScale += srGain * (systolicError_mmHg + diastolicError_mmHg);
        BLIM(rightHeartResistanceScale, 0.5, 1.5);
        BLIM(systemicResistanceScale, 0.5, 1.5);
        Info("Using vena cava resistance gain.");
      }
    } else if ((systolicError_mmHg > 0 && diastolicError_mmHg < 0) || (systolicError_mmHg < 0 && diastolicError_mmHg > 0)) // Opposite directions
    {
      // Widen or narrow based on both errors
      if (meanCVP_mmHg < 2.0 || meanCVP_mmHg > 6.0) // Go ahead and do it but warn the user that CVP is out of normal range.
        Warning("We're out of CVP range");
      aortaComplianceScale -= acGain1 * systolicError_mmHg - acGain2 * diastolicError_mmHg;
      BLIM(aortaComplianceScale, 0.5, 1.5);
    } else {
      // This means that both errors are zero, which means you shouldn't have called me
      Error("Something is wrong with tuning logic");
    }
    m_ss << "Tuning paths, Current values : HeartRate(bpm):" << GetHeartRate(FrequencyUnit::Per_min) << " Systolic(mmHg):" << systolic_mmHg << " Diastolic(mmHg):" << diastolic_mmHg << " Cardiac Output(mL/min):" << cardiacOutput_mL_Per_min << " Mean CVP(mmHg):" << meanCVP_mmHg << " MAP(mmHg):" << GetMeanArterialPressure(PressureUnit::mmHg) << " BloodVolume(mL): " << blood_mL;
    Info(m_ss);
    TunePaths(systemicResistanceScale, systemicComplianceScale, aortaResistanceScale, aortaComplianceScale, rightHeartResistanceScale, venaCavaComplianceScale);
  }

  if (!success) {
    m_ss << "Unable to tune circuit to desired patient parameters. Final values : HeartRate(bpm):" << GetHeartRate(FrequencyUnit::Per_min) << " Systolic(mmHg):" << GetSystolicArterialPressure(PressureUnit::mmHg) << " Diastolic(mmHg):" << GetDiastolicArterialPressure(PressureUnit::mmHg) << " Cardiac Output(mL/min):" << GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << " Mean CVP(mmHg):" << GetMeanCentralVenousPressure(PressureUnit::mmHg) << " MAP(mmHg):" << GetMeanArterialPressure(PressureUnit::mmHg) << " BloodVolume(mL): " << blood_mL;
    Fatal(m_ss);
  } else {
    m_ss << "Successfully tuned circuit. Final values : HeartRate(bpm):" << GetHeartRate(FrequencyUnit::Per_min) << " Systolic(mmHg):" << GetSystolicArterialPressure(PressureUnit::mmHg) << " Diastolic(mmHg):" << GetDiastolicArterialPressure(PressureUnit::mmHg) << " Cardiac Output(mL/min):" << GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << " Mean CVP(mmHg):" << GetMeanCentralVenousPressure(PressureUnit::mmHg) << " MAP(mmHg):" << GetMeanArterialPressure(PressureUnit::mmHg) << " BloodVolume(mL): " << blood_mL;
    Info(m_ss);
    // Reset our substance masses to the new volumes
    double bloodVolumeBaseline_mL = 0.0;
    for (SELiquidCompartment* c : m_data.GetCompartments().GetVascularLeafCompartments()) {
      if (!c->HasVolume())
        continue;
      bloodVolumeBaseline_mL += c->GetVolume(VolumeUnit::mL);
      c->Balance(BalanceLiquidBy::Concentration);
      if (m_CirculatoryGraph->GetCompartment(c->GetName()) == nullptr)
        Info(std::string { "Cardiovascular Graph does not have cmpt " } + c->GetName());
      if (c->HasSubstanceQuantity(m_data.GetSubstances().GetHb())) // Unit testing does not have any Hb
        m_data.GetSaturationCalculator().CalculateBloodGasDistribution(*c); //so don't do this if we don't have Hb
    }
    //We need to reset the blood volume baseline because it might have changed during circuit stabilization
    m_data.GetPatient().GetBloodVolumeBaseline().SetValue(bloodVolumeBaseline_mL, VolumeUnit::mL);
    for (SELiquidCompartment* c : m_data.GetCompartments().GetUrineLeafCompartments()) {
      if (!c->HasVolume())
        continue;
      c->Balance(BalanceLiquidBy::Concentration);
    }
    SELiquidCompartment* extra;
    for (SETissueCompartment* c : m_data.GetCompartments().GetTissueCompartments()) {
      extra = &m_data.GetCompartments().GetExtracellularFluid(*c);
      if (!extra->HasVolume())
        continue;
      extra->Balance(BalanceLiquidBy::Concentration);
    }
    m_patient->GetHeartRateBaseline().Set(GetHeartRate());
    m_patient->GetDiastolicArterialPressureBaseline().Set(GetDiastolicArterialPressure());
    m_patient->GetSystolicArterialPressureBaseline().Set(GetSystolicArterialPressure());
    m_patient->GetMeanArterialPressureBaseline().Set(GetMeanArterialPressure());
  }
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Method that changes path parameters during circuit tuning.
///
/// \details
/// Method that changes path parameters during circuit tuning.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::TunePaths(double systemicResistanceScale, double systemicComplianceScale, double aortaResistanceScale, double aortaComplianceScale, double rightHeartResistanceScale, double venaCavaComplianceScale)
{
  double sp1_mmHg_s_Per_mL;
  if (systemicComplianceScale != 1.0) {
    for (SEFluidCircuitPath* p : m_systemicCompliancePaths) {
      sp1_mmHg_s_Per_mL = p->GetCapacitanceBaseline().GetValue(FlowComplianceUnit::mL_Per_mmHg) * systemicComplianceScale;
      p->GetCapacitanceBaseline().SetValue(sp1_mmHg_s_Per_mL, FlowComplianceUnit::mL_Per_mmHg);
      p->GetCapacitance().SetValue(sp1_mmHg_s_Per_mL, FlowComplianceUnit::mL_Per_mmHg);
      p->GetNextCapacitance().SetValue(sp1_mmHg_s_Per_mL, FlowComplianceUnit::mL_Per_mmHg);
    }
  }
  if (systemicResistanceScale != 1.0) {
    for (SEFluidCircuitPath* p : m_systemicResistancePaths) {
      sp1_mmHg_s_Per_mL = p->GetResistanceBaseline().GetValue(FlowResistanceUnit::mmHg_s_Per_mL) * systemicResistanceScale;

      p->GetResistanceBaseline().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
      p->GetResistance().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
      p->GetNextResistance().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    }
  }
  if (systemicResistanceScale != 1.0) {
    for (SEFluidCircuitPath* p : m_tissueResistancePaths) {
      sp1_mmHg_s_Per_mL = p->GetResistanceBaseline().GetValue(FlowResistanceUnit::mmHg_min_Per_mL) / systemicResistanceScale;

      p->GetResistanceBaseline().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
      p->GetResistance().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
      p->GetNextResistance().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
    }
  }

  if (aortaComplianceScale != 1) {
    sp1_mmHg_s_Per_mL = m_AortaCompliance->GetCapacitanceBaseline().GetValue(FlowComplianceUnit::mL_Per_mmHg) * aortaComplianceScale;
    m_AortaCompliance->GetCapacitanceBaseline().SetValue(sp1_mmHg_s_Per_mL, FlowComplianceUnit::mL_Per_mmHg);
    m_AortaCompliance->GetCapacitance().SetValue(sp1_mmHg_s_Per_mL, FlowComplianceUnit::mL_Per_mmHg);
    m_AortaCompliance->GetNextCapacitance().SetValue(sp1_mmHg_s_Per_mL, FlowComplianceUnit::mL_Per_mmHg);
  }
  if (aortaResistanceScale != 1) {
    sp1_mmHg_s_Per_mL = m_AortaResistance->GetResistanceBaseline().GetValue(FlowResistanceUnit::mmHg_s_Per_mL) * aortaResistanceScale;
    m_AortaResistance->GetResistanceBaseline().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    m_AortaResistance->GetResistance().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    m_AortaResistance->GetNextResistance().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  }
  if (venaCavaComplianceScale != 1) {
    sp1_mmHg_s_Per_mL = m_VenaCavaCompliance->GetCapacitanceBaseline().GetValue(FlowComplianceUnit::mL_Per_mmHg) * venaCavaComplianceScale;
    m_VenaCavaCompliance->GetCapacitanceBaseline().SetValue(sp1_mmHg_s_Per_mL, FlowComplianceUnit::mL_Per_mmHg);
    m_VenaCavaCompliance->GetCapacitance().SetValue(sp1_mmHg_s_Per_mL, FlowComplianceUnit::mL_Per_mmHg);
    m_VenaCavaCompliance->GetNextCapacitance().SetValue(sp1_mmHg_s_Per_mL, FlowComplianceUnit::mL_Per_mmHg);
  }
  if (rightHeartResistanceScale != 1) {
    sp1_mmHg_s_Per_mL = m_RightHeartResistance->GetResistanceBaseline().GetValue(FlowResistanceUnit::mmHg_s_Per_mL) * rightHeartResistanceScale;
    m_RightHeartResistance->GetResistanceBaseline().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    m_RightHeartResistance->GetResistance().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    m_RightHeartResistance->GetNextResistance().SetValue(sp1_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Adjusts vascular tone due to Efferent reflex, drug, and other effects
///
/// \details
/// Adjusts vascular tone due to Efferent reflex, drug, and other effects
//--------------------------------------------------------------------------------------------------
void Cardiovascular::AdjustVascularTone()
{
  /// \todo Add dilation (decreased resistance) to myocardium supply path if myocardium oxygen deficit event is active.

  //The Efferent response adjusts the systemic resistances and compliances according to the multiplier calculated in Nervous.cpp
  double UpdatedResistance_mmHg_s_Per_mL = 0.0;
  double UpdatedCompliance_mL_Per_mmHg = 0.0;
  double totalComplianceChange_mL_Per_mmHg = 0.0;
  double ResistanceScale = 0.0;

  if (m_data.GetNervous().HasResistanceScaleExtrasplanchnic()) {
    ResistanceScale = m_data.GetNervous().GetResistanceScaleExtrasplanchnic().GetValue();
    for (SEFluidCircuitPath* ePath : m_extrasplanchnicResistancePaths) {
      UpdatedResistance_mmHg_s_Per_mL = ePath->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
      UpdatedResistance_mmHg_s_Per_mL *= ResistanceScale;
      if (UpdatedResistance_mmHg_s_Per_mL < m_minIndividialSystemicResistance__mmHg_s_Per_mL) {
        UpdatedResistance_mmHg_s_Per_mL = m_minIndividialSystemicResistance__mmHg_s_Per_mL;
      }
      ePath->GetNextResistance().SetValue(UpdatedResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    }
  }
  if (m_data.GetNervous().HasResistanceScaleMuscle()) {
    ResistanceScale = m_data.GetNervous().GetResistanceScaleMuscle().GetValue();
    for (SEFluidCircuitPath* mPath : m_muscleResistancePaths) {
      UpdatedResistance_mmHg_s_Per_mL = mPath->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
      UpdatedResistance_mmHg_s_Per_mL *= ResistanceScale;
      if (UpdatedResistance_mmHg_s_Per_mL < m_minIndividialSystemicResistance__mmHg_s_Per_mL) {
        UpdatedResistance_mmHg_s_Per_mL = m_minIndividialSystemicResistance__mmHg_s_Per_mL;
      }
      mPath->GetNextResistance().SetValue(UpdatedResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    }
  }
  if (m_data.GetNervous().HasResistanceScaleMyocardium()) {
    ResistanceScale = m_data.GetNervous().GetResistanceScaleMyocardium().GetValue();
    for (SEFluidCircuitPath* mPath : m_myocardiumResistancePaths) {
      UpdatedResistance_mmHg_s_Per_mL = mPath->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
      UpdatedResistance_mmHg_s_Per_mL *= ResistanceScale;
      if (UpdatedResistance_mmHg_s_Per_mL < m_minIndividialSystemicResistance__mmHg_s_Per_mL) {
        UpdatedResistance_mmHg_s_Per_mL = m_minIndividialSystemicResistance__mmHg_s_Per_mL;
      }
      mPath->GetNextResistance().SetValue(UpdatedResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    }
  }
  if (m_data.GetNervous().HasResistanceScaleSplanchnic()) {
    ResistanceScale = m_data.GetNervous().GetResistanceScaleSplanchnic().GetValue();
    for (SEFluidCircuitPath* sPath : m_splanchnicResistancePaths) {
      UpdatedResistance_mmHg_s_Per_mL = sPath->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
      UpdatedResistance_mmHg_s_Per_mL *= ResistanceScale;
      if (UpdatedResistance_mmHg_s_Per_mL < m_minIndividialSystemicResistance__mmHg_s_Per_mL) {
        UpdatedResistance_mmHg_s_Per_mL = m_minIndividialSystemicResistance__mmHg_s_Per_mL;
      }
      sPath->GetNextResistance().SetValue(UpdatedResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    }
  }

  if (m_data.GetNervous().HasComplianceScale()) {
    for (SEFluidCircuitPath* Path : m_systemicCompliancePaths) {
      UpdatedCompliance_mL_Per_mmHg = m_data.GetNervous().GetComplianceScale().GetValue() * Path->GetComplianceBaseline(FlowComplianceUnit::mL_Per_mmHg);
      Path->GetNextCompliance().SetValue(UpdatedCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
    }
  }

  //The drug response adjusts the systemic resistances according to the mean arterial pressure change calculated in Drugs.cpp
  double ResistanceChange = 0.0;
  if (m_data.GetDrugs().HasMeanBloodPressureChange()) {
    double TuningParameter = 5.0;
    double CardiacOutput_mL_Per_s = GetCardiacOutput(VolumePerTimeUnit::mL_Per_s);
    if (CardiacOutput_mL_Per_s != 0.0)
      ResistanceChange = m_data.GetDrugs().GetMeanBloodPressureChange(PressureUnit::mmHg) / GetCardiacOutput(VolumePerTimeUnit::mL_Per_s);
    if (ResistanceChange < 0.0)
      TuningParameter = 0.8; //1.2;
    ResistanceChange *= TuningParameter;
  }
  
  //Drug effects on arterial pressure occur by increasing the systemic vascular resistance. This occurs every time step by updating the next flow resistance.
  //These effects are applied in HeartDriver() since its functionality is called every time step.
  if (std::abs(ResistanceChange) > ZERO_APPROX) {
    for (SEFluidCircuitPath* Path : m_systemicResistancePaths) {
      if (!Path->HasNextResistance())
        continue;
      UpdatedResistance_mmHg_s_Per_mL = Path->GetNextResistance(FlowResistanceUnit::mmHg_s_Per_mL);
      UpdatedResistance_mmHg_s_Per_mL += ResistanceChange * UpdatedResistance_mmHg_s_Per_mL / GetSystemicVascularResistance(FlowResistanceUnit::mmHg_s_Per_mL);
      LLIM(UpdatedResistance_mmHg_s_Per_mL, m_minIndividialSystemicResistance__mmHg_s_Per_mL);
      Path->GetNextResistance().SetValue(UpdatedResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    }

    MetabolicToneResponse();
  }
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the heart rate from the period.
///
/// \details
/// When flow is detected, the heart rate is computed from the cardiac cycle duration. Because a
/// time step is added right before the flow detection (in case the cardiac cycle is continuing)
/// we must peel off the time step here.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::CalculateHeartRate()
{
  // The time that the flow actually decreased below the threshold was last time slice (when m_HeartFlowDetected
  // was set back to false), so we need to subtract one time step from the interval.
  double HeartRate_Per_s = 0.0;
  if (m_data.GetActions().GetPatientActions().HasOverride()
      && m_data.GetActions().GetPatientActions().GetOverride()->HasHeartRateOverride()
      && m_data.GetActions().GetPatientActions().GetOverride()->GetOverrideConformance() == CDM::enumOnOff::Off) {
    HeartRate_Per_s = m_data.GetActions().GetPatientActions().GetOverride()->GetHeartRateOverride(FrequencyUnit::Per_s);
  } else {
    HeartRate_Per_s = 1.0 / (m_CurrentCardiacCycleDuration_s - m_dT_s);
  }
  GetHeartRate().SetValue(HeartRate_Per_s * 60.0, FrequencyUnit::Per_min);
  m_CurrentCardiacCycleDuration_s = 0.0; //Incremented each time step in HeartDriver
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// determine override requirements from user defined inputs
///
/// \details
/// User specified override outputs that are specific to the cardiovascular system are implemented here.
/// If overrides aren't present for this system then this function will not be called during preprocess.
//--------------------------------------------------------------------------------------------------
void Cardiovascular::ProcessOverride()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();

#ifdef BIOGEARS_USE_OVERRIDE_CONTROL
  OverrideControlLoop();
#endif

  if (override->HasBloodVolumeOverride()) {
    GetBloodVolume().SetValue(override->GetBloodVolumeOverride(VolumeUnit::L), VolumeUnit::L);
  }
  if (override->HasCardiacOutputOverride()) {
    GetCardiacOutput().SetValue(override->GetCardiacOutputOverride(VolumePerTimeUnit::L_Per_min), VolumePerTimeUnit::L_Per_min);
  }
  if (override->HasDiastolicArterialPressureOverride()) {
    GetDiastolicArterialPressure().SetValue(override->GetDiastolicArterialPressureOverride(PressureUnit::mmHg), PressureUnit::mmHg);
  }
  if (override->HasMAPOverride()) {
    GetMeanArterialPressure().SetValue(override->GetMAPOverride(PressureUnit::mmHg), PressureUnit::mmHg);
  }
  if (override->HasHeartRateOverride()) {
    if (override->GetOverrideConformance() == CDM::enumOnOff::Off) {
      GetHeartRate().SetValue(override->GetHeartRateOverride(FrequencyUnit::Per_min), FrequencyUnit::Per_min);
    }
  }
  if (override->HasHeartStrokeVolumeOverride()) {
    GetHeartStrokeVolume().SetValue(override->GetHeartStrokeVolumeOverride(VolumeUnit::mL), VolumeUnit::mL);
  }
  if (override->HasSystolicArterialPressureOverride()) {
    GetSystolicArterialPressure().SetValue(override->GetSystolicArterialPressureOverride(PressureUnit::mmHg), PressureUnit::mmHg);
  }
}

//// Can be turned on or off (for debugging purposes) using the Biogears_USE_OVERRIDE_CONTROL external in CMake
void Cardiovascular::OverrideControlLoop()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();

  constexpr double maxBloodVolumeOverride = 25.0; //L
  constexpr double minBloodVolumeOverride = 0.0; //L
  constexpr double maxCardiacOutput = 100.0; //L/min
  constexpr double minCardiacOutput = 0.0; //L/min
  constexpr double maxDiastolicArtPressureOverride = 200.0; //mmHg
  constexpr double minDiastolicArtPressureOverride = 0.0; //mmHg
  constexpr double maxMAPOverride = 105.0; //mmHg
  constexpr double minMAPOverride = 60.0; //mmHg
  const double maxHROverride = m_data.GetPatient().GetHeartRateMaximum(FrequencyUnit::Per_min); //bpm, max estimate of patient's max age related heart is [220-age_yr] so override provides a huge buffer above this estimate
  const double minHROverride = m_data.GetPatient().GetHeartRateMinimum(FrequencyUnit::Per_min); //bpm
  constexpr double maxHeartStrokeVolumeOverride = 5000.0; //mL
  constexpr double minHeartStrokeVolumeOverride = 0.0; //mL
  constexpr double maxSystolicArtPressureOverride = 300.0; //mmHg
  constexpr double minSystolicArtPressureOverride = 0.0; //mmHg

  double currentBloodVolumeOverride = 0.0; //value gets changed in next check
  double currentCardiacOutput = 0.0; //value gets changed in next check
  double currentDiastolicArtPressureOverride = 0.0; //value gets changed in next check
  double currentMAPOverride = 85.0; //Average MAP, value gets changed in next check
  double currentHROverride = 80.0; //Average HR, value gets changed in next check
  double currentHeartStrokeVolumeOverride = 0.0; //value gets changed in next check
  double currentSystolicArtPressureOverride = 0.0; //value gets changed in next check

  if (override->HasBloodVolumeOverride()) {
    currentBloodVolumeOverride = override->GetBloodVolumeOverride(VolumeUnit::L);
  }
  if (override->HasCardiacOutputOverride()) {
    currentCardiacOutput = override->GetCardiacOutputOverride(VolumePerTimeUnit::L_Per_min);
  }
  if (override->HasDiastolicArterialPressureOverride()) {
    currentDiastolicArtPressureOverride = override->GetDiastolicArterialPressureOverride(PressureUnit::mmHg);
  }
  if (override->HasMAPOverride()) {
    currentMAPOverride = override->GetMAPOverride(PressureUnit::mmHg);
  }
  if (override->HasHeartRateOverride()) {
    currentHROverride = override->GetHeartRateOverride(FrequencyUnit::Per_min);
  }
  if (override->HasHeartStrokeVolumeOverride()) {
    currentHeartStrokeVolumeOverride = override->GetHeartStrokeVolumeOverride(VolumeUnit::mL);
  }
  if (override->HasSystolicArterialPressureOverride()) {
    currentSystolicArtPressureOverride = override->GetSystolicArterialPressureOverride(PressureUnit::mmHg);
  }

  if ((currentBloodVolumeOverride < minBloodVolumeOverride || currentBloodVolumeOverride > maxBloodVolumeOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Blood Volume Override (Cardiovascular) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentCardiacOutput < minCardiacOutput || currentCardiacOutput > maxCardiacOutput) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Cardiac Output Override (Cardiovascular) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentDiastolicArtPressureOverride < minDiastolicArtPressureOverride || currentDiastolicArtPressureOverride > maxDiastolicArtPressureOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Diastolic Arterial Pressure Override (Cardiovascular) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentMAPOverride < minMAPOverride || currentMAPOverride > maxMAPOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Mean Arterial Pressure Override (Cardiovascular) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentHROverride < minHROverride || currentHROverride > maxHROverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Heart Rate (Cardiovascular) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentHeartStrokeVolumeOverride < minHeartStrokeVolumeOverride || currentHeartStrokeVolumeOverride > maxHeartStrokeVolumeOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Heart Stroke Volume Override (Cardiovascular) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentSystolicArtPressureOverride < minSystolicArtPressureOverride || currentSystolicArtPressureOverride > maxSystolicArtPressureOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Systolic Arterial Pressure Override (Cardiovascular) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  return;
}
}
