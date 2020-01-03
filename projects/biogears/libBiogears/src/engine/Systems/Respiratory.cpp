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

#include <biogears/engine/Systems/Respiratory.h>

#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>
#include <biogears/cdm/patient/actions/SEBreathHold.h>
#include <biogears/cdm/patient/actions/SEConsciousRespiration.h>
#include <biogears/cdm/patient/actions/SEForcedExhale.h>
#include <biogears/cdm/patient/actions/SEForcedInhale.h>
#include <biogears/cdm/patient/actions/SEUseInhaler.h>
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>
#include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
#include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarInversePressure.h>
#include <biogears/cdm/properties/SEScalarInverseVolume.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>

namespace BGE = mil::tatrc::physiology::biogears;

#ifdef _MSC_VER
#pragma warning(disable : 4305 4244) // Disable some warning messages
#endif

namespace biogears {
//Flag for setting things constant to test
//Should be commented out, unless debugging/tuning
// #define TUNING

auto Respiratory::make_unique(BioGears& bg) -> std::unique_ptr<Respiratory>
{
  return std::unique_ptr<Respiratory>(new Respiratory(bg));
}

Respiratory::Respiratory(BioGears& bg)
  : SERespiratorySystem(bg.GetLogger())
  , m_data(bg)
  , m_Calculator(FlowComplianceUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s, FlowInertanceUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O, VolumeUnit::L, FlowResistanceUnit::cmH2O_s_Per_L, GetLogger())
  , m_GasTransporter(VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, NoUnit::unitless, GetLogger())
  , m_AerosolTransporter(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, GetLogger())
{
  Clear();
  m_TuningFile = "";
}

Respiratory::~Respiratory()
{
  Clear();
}

void Respiratory::Clear()
{
  SERespiratorySystem::Clear();
  m_Patient = nullptr;
  m_PatientActions = nullptr;
  m_Propofol = nullptr;

  m_Environment = nullptr;
  m_AerosolMouth = nullptr;
  m_AerosolTrachea = nullptr;
  m_AerosolLeftBronchi = nullptr;
  m_AerosolLeftAlveoli = nullptr;
  m_AerosolRightBronchi = nullptr;
  m_AerosolRightAlveoli = nullptr;
  m_Lungs = nullptr;
  m_LeftLungExtravascular = nullptr;
  m_RightLungExtravascular = nullptr;
  m_Trachea = nullptr;
  m_AortaO2 = nullptr;
  m_AortaCO2 = nullptr;
  m_MechanicalVentilatorConnection = nullptr;

  m_RespiratoryCircuit = nullptr;

  m_LeftAlveoli = nullptr;
  m_LeftBronchi = nullptr;
  m_LeftPleuralCavity = nullptr;
  m_RespiratoryMuscle = nullptr;
  m_RightAlveoli = nullptr;
  m_RightBronchi = nullptr;
  m_RightPleuralCavity = nullptr;

  m_TracheaToLeftBronchi = nullptr;
  m_TracheaToRightBronchi = nullptr;
  m_LeftBronchiToLeftAlveoli = nullptr;
  m_RightBronchiToRightAlveoli = nullptr;
  m_RightPleuralCavityToRespiratoryMuscle = nullptr;
  m_LeftPleuralCavityToRespiratoryMuscle = nullptr;
  m_DriverPressurePath = nullptr;
  m_LeftDriverPressurePath = nullptr;
  m_MouthToTrachea = nullptr;
  m_MouthToStomach = nullptr;
  m_EnvironmentToLeftChestLeak = nullptr;
  m_EnvironmentToRightChestLeak = nullptr;
  m_LeftAlveoliLeakToLeftPleuralCavity = nullptr;
  m_RightAlveoliLeakToRightPleuralCavity = nullptr;
  m_LeftPleuralCavityToEnvironment = nullptr;
  m_RightPleuralCavityToEnvironment = nullptr;
  m_RightAlveoliToRightPleuralConnection = nullptr;
  m_LeftAlveoliToLeftPleuralConnection = nullptr;
  m_RightPulmonaryCapillary = nullptr;
  m_LeftPulmonaryCapillary = nullptr;
  m_ConnectionToMouth = nullptr;
  m_GroundToConnection = nullptr;

  m_BloodPHRunningAverage.Reset();
  m_ArterialO2Average_mmHg.Reset();
  m_ArterialCO2Average_mmHg.Reset();

  m_OverrideRRBaseline_Per_min = 0.0;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Respiratory::Initialize()
{
  BioGearsSystem::Initialize();

  //Vital signs
  m_bNotBreathing = false;
  m_TopBreathTotalVolume_L = 0.0;
  m_TopBreathAlveoliVolume_L = 0.0;
  m_TopBreathDeadSpaceVolume_L = 0.0;
  m_TopBreathPleuralPressure_cmH2O = 0.0;
  m_LastCardiacCycleBloodPH = 7.4;

  //Driver
  //Basically a Y-shift for the driver
  m_DefaultDrivePressure_cmH2O = -5.0;
  m_MaxDriverPressure_cmH2O = -10.0 * m_Patient->GetVitalCapacity(VolumeUnit::L) / 2.0; //Max tidal volume is 50% of vital capacity, and driver is set up so that 1 cmH2O decrease in pressure -> 0.1 L increase in TV
  m_ElapsedBreathingCycleTime_min = 0.0;
  m_BreathTimeExhale_min = 0.0;
  m_BreathingCycle = false;
  m_BreathingCycleTime_s = 0.0;
  m_VentilationFrequency_Per_min = m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min);
  m_DriverPressure_cmH2O = m_DefaultDrivePressure_cmH2O;
  m_DriverPressureMin_cmH2O = m_DefaultDrivePressure_cmH2O;

  //The peak driver pressure is the pressure above the default pressure
  m_PeakRespiratoryDrivePressure_cmH2O = m_Patient->GetRespiratoryDriverAmplitudeBaseline(PressureUnit::cmH2O);
  m_ArterialO2PartialPressure_mmHg = m_AortaO2->GetPartialPressure(PressureUnit::mmHg);
  m_ArterialCO2PartialPressure_mmHg = m_AortaCO2->GetPartialPressure(PressureUnit::mmHg);
  m_AverageLocalTissueBronchodilationEffects = 0.0;

  //Asthma
  m_IEscaleFactor = 1.0;

  //PH balancing
  m_arterialPHBaseline = 0.0;

  // Conscious Respiration
  m_ConsciousRespirationPeriod_s = 0.0;
  m_ConsciousRespirationRemainingPeriod_s = 0.0;
  m_ExpiratoryReserveVolumeFraction = -1.0;
  m_InspiratoryCapacityFraction = -1.0;
  m_ConsciousStartPressure_cmH2O = 0.0;
  m_ConsciousEndPressure_cmH2O = 0.0;
  m_ConsciousBreathing = false;

  //Patient data
  m_InitialExpiratoryReserveVolume_L = m_Patient->GetExpiratoryReserveVolume(VolumeUnit::L);
  m_InitialInspiratoryCapacity_L = m_Patient->GetInspiratoryCapacity(VolumeUnit::L);
  m_InitialFunctionalResidualCapacity_L = m_Patient->GetFunctionalResidualCapacity(VolumeUnit::L);
  m_InitialResidualVolume_L = m_Patient->GetResidualVolume(VolumeUnit::L);
  m_InstantaneousFunctionalResidualCapacity_L = m_Patient->GetFunctionalResidualCapacity(VolumeUnit::L);
  m_PreviousTotalLungVolume_L = m_Patient->GetFunctionalResidualCapacity(VolumeUnit::L);

  //Action removal flags
  m_HadAirwayObstruction = false;
  m_HadBronchoconstriction = false;

  //System data
  double TidalVolume_L = m_Patient->GetTidalVolumeBaseline(VolumeUnit::L);
  double RespirationRate_Per_min = m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min);
  double DeadSpace_L = m_LeftBronchi->GetVolumeBaseline(VolumeUnit::L) + m_RightBronchi->GetVolumeBaseline(VolumeUnit::L) + m_Trachea->GetVolume(VolumeUnit::L);
  GetTotalLungVolume().SetValue(m_PreviousTotalLungVolume_L, VolumeUnit::L);
  GetTidalVolume().SetValue(TidalVolume_L, VolumeUnit::L);
  GetRespirationRate().SetValue(RespirationRate_Per_min, FrequencyUnit::Per_min);
  GetRespirationDriverFrequency().SetValue(RespirationRate_Per_min, FrequencyUnit::Per_min);
  GetRespirationDriverPressure().SetValue(m_PeakRespiratoryDrivePressure_cmH2O, PressureUnit::cmH2O);
  GetCarricoIndex().SetValue(452.0, PressureUnit::mmHg);
  GetInspiratoryExpiratoryRatio().SetValue(0.5);
  GetTotalAlveolarVentilation().SetValue(RespirationRate_Per_min * (TidalVolume_L - DeadSpace_L), VolumePerTimeUnit::L_Per_min);
  GetTotalPulmonaryVentilation().SetValue(RespirationRate_Per_min * TidalVolume_L, VolumePerTimeUnit::L_Per_min);
  GetTotalDeadSpaceVentilation().SetValue(DeadSpace_L * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);
  GetPulmonaryCompliance().SetValue(0.2, FlowComplianceUnit::L_Per_cmH2O);
  GetSpecificVentilation().SetValue(0.21);
  GetEndTidalCarbonDioxideFraction().SetValue(0.0827);
  GetEndTidalCarbonDioxidePressure().SetValue(0.0, PressureUnit::mmHg);

  //Get the fluid mechanics to a good starting point
  TuneCircuit();
}

bool Respiratory::Load(const CDM::BioGearsRespiratorySystemData& in)
{
  if (!SERespiratorySystem::Load(in))
    return false;

  m_InitialExpiratoryReserveVolume_L = in.InitialExpiratoryReserveVolume_L();
  m_InitialFunctionalResidualCapacity_L = in.InitialFunctionalResidualCapacity_L();
  m_InitialInspiratoryCapacity_L = in.InitialInspiratoryCapacity_L();
  m_InitialResidualVolume_L = in.InitialResidualVolume_L();

  m_bNotBreathing = in.NotBreathing();
  m_TopBreathTotalVolume_L = in.TopBreathTotalVolume_L();
  m_TopBreathAlveoliVolume_L = in.TopBreathAlveoliVolume_L();
  m_TopBreathDeadSpaceVolume_L = in.TopBreathDeadSpaceVolume_L();
  m_TopBreathPleuralPressure_cmH2O = in.TopBreathPleuralPressure_cmH2O();
  m_LastCardiacCycleBloodPH = in.LastCardiacCycleBloodPH();
  m_PreviousTotalLungVolume_L = in.PreviousTotalLungVolume_L();
  m_BloodPHRunningAverage.Load(in.BloodPHRunningAverage());

  m_BreathingCycle = in.BreathingCycle();
  m_ArterialO2PartialPressure_mmHg = in.ArterialOxygenPressure_mmHg();
  m_ArterialCO2PartialPressure_mmHg = in.ArterialCarbonDioxidePressure_mmHg();
  m_BreathingCycleTime_s = in.BreathingCycleTime_s();
  m_BreathTimeExhale_min = in.BreathTimeExhale_min();
  m_DefaultDrivePressure_cmH2O = in.DefaultDrivePressure_cmH2O();
  m_DriverPressure_cmH2O = in.DriverPressure_cmH2O();
  m_DriverPressureMin_cmH2O = in.DriverPressureMin_cmH2O();
  m_ElapsedBreathingCycleTime_min = in.ElapsedBreathingCycleTime_min();
  m_IEscaleFactor = in.IEscaleFactor();
  m_InstantaneousFunctionalResidualCapacity_L = in.InstantaneousFunctionalResidualCapacity_L();
  m_MaxDriverPressure_cmH2O = in.MaxDriverPressure_cmH2O();
  m_PeakRespiratoryDrivePressure_cmH2O = in.PeakRespiratoryDrivePressure_cmH2O();

  m_VentilationFrequency_Per_min = in.VentilationFrequency_Per_min();
  m_ArterialO2Average_mmHg.Load(in.ArterialOxygenAverage_mmHg());
  m_ArterialCO2Average_mmHg.Load(in.ArterialCarbonDioxideAverage_mmHg());

  m_ConsciousBreathing = in.ConsciousBreathing();
  m_ConsciousRespirationPeriod_s = in.ConsciousRespirationPeriod_s();
  m_ConsciousRespirationRemainingPeriod_s = in.ConsciousRespirationRemainingPeriod_s();
  m_ExpiratoryReserveVolumeFraction = in.ExpiratoryReserveVolumeFraction();
  m_InspiratoryCapacityFraction = in.InspiratoryCapacityFraction();
  m_ConsciousStartPressure_cmH2O = in.ConsciousStartPressure_cmH2O();
  m_ConsciousEndPressure_cmH2O = in.ConsciousEndPressure_cmH2O();

  m_HadAirwayObstruction = in.HadAirwayObstruction();
  m_HadBronchoconstriction = in.HadBronchoconstriction();

  BioGearsSystem::LoadState();
  return true;
}
CDM::BioGearsRespiratorySystemData* Respiratory::Unload() const
{
  CDM::BioGearsRespiratorySystemData* data = new CDM::BioGearsRespiratorySystemData();
  Unload(*data);
  return data;
}
void Respiratory::Unload(CDM::BioGearsRespiratorySystemData& data) const
{
  SERespiratorySystem::Unload(data);

  data.InitialExpiratoryReserveVolume_L(m_InitialExpiratoryReserveVolume_L);
  data.InitialFunctionalResidualCapacity_L(m_InitialFunctionalResidualCapacity_L);
  data.InitialInspiratoryCapacity_L(m_InitialInspiratoryCapacity_L);
  data.InitialResidualVolume_L(m_InitialResidualVolume_L);

  data.NotBreathing(m_bNotBreathing);
  data.TopBreathTotalVolume_L(m_TopBreathTotalVolume_L);
  data.TopBreathAlveoliVolume_L(m_TopBreathAlveoliVolume_L);
  data.TopBreathDeadSpaceVolume_L(m_TopBreathDeadSpaceVolume_L);
  data.TopBreathPleuralPressure_cmH2O(m_TopBreathPleuralPressure_cmH2O);
  data.LastCardiacCycleBloodPH(m_LastCardiacCycleBloodPH);
  data.PreviousTotalLungVolume_L(m_PreviousTotalLungVolume_L);
  data.BloodPHRunningAverage(std::unique_ptr<CDM::RunningAverageData>(m_BloodPHRunningAverage.Unload()));

  data.BreathingCycle(m_BreathingCycle);
  data.ArterialOxygenPressure_mmHg(m_ArterialO2PartialPressure_mmHg);
  data.ArterialCarbonDioxidePressure_mmHg(m_ArterialCO2PartialPressure_mmHg);
  data.BreathingCycleTime_s(m_BreathingCycleTime_s);
  data.BreathTimeExhale_min(m_BreathTimeExhale_min);
  data.DefaultDrivePressure_cmH2O(m_DefaultDrivePressure_cmH2O);
  data.DriverPressure_cmH2O(m_DriverPressure_cmH2O);
  data.DriverPressureMin_cmH2O(m_DriverPressureMin_cmH2O);
  data.ElapsedBreathingCycleTime_min(m_ElapsedBreathingCycleTime_min);
  data.IEscaleFactor(m_IEscaleFactor);
  data.InstantaneousFunctionalResidualCapacity_L(m_InstantaneousFunctionalResidualCapacity_L);
  data.MaxDriverPressure_cmH2O(m_MaxDriverPressure_cmH2O);
  data.PeakRespiratoryDrivePressure_cmH2O(m_PeakRespiratoryDrivePressure_cmH2O);

  data.VentilationFrequency_Per_min(m_VentilationFrequency_Per_min);
  data.ArterialOxygenAverage_mmHg(std::unique_ptr<CDM::RunningAverageData>(m_ArterialO2Average_mmHg.Unload()));
  data.ArterialCarbonDioxideAverage_mmHg(std::unique_ptr<CDM::RunningAverageData>(m_ArterialCO2Average_mmHg.Unload()));

  data.ConsciousBreathing(m_ConsciousBreathing);
  data.ConsciousRespirationPeriod_s(m_ConsciousRespirationPeriod_s);
  data.ConsciousRespirationRemainingPeriod_s(m_ConsciousRespirationRemainingPeriod_s);
  data.ExpiratoryReserveVolumeFraction(m_ExpiratoryReserveVolumeFraction);
  data.InspiratoryCapacityFraction(m_InspiratoryCapacityFraction);
  data.ConsciousStartPressure_cmH2O(m_ConsciousStartPressure_cmH2O);
  data.ConsciousEndPressure_cmH2O(m_ConsciousEndPressure_cmH2O);

  data.HadAirwayObstruction(m_HadAirwayObstruction);
  data.HadBronchoconstriction(m_HadBronchoconstriction);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for Respiratory Class
///
///  \details
///  Initializes member variables and system level values on the common data model.
//--------------------------------------------------------------------------------------------------
void Respiratory::SetUp()
{
  //Time Step
  m_dt_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  m_dt_min = m_data.GetTimeStep().GetValue(TimeUnit::min);
  m_hadApnea = false;
  //Patient
  m_Patient = &m_data.GetPatient();
  m_PatientActions = &m_data.GetActions().GetPatientActions();
  m_OverrideRRBaseline_Per_min = m_Patient->GetRespirationRateBaseline().GetValue(FrequencyUnit::Per_min);
  // Substance
  m_Propofol = m_data.GetSubstances().GetSubstance("Propofol");
  //Configuration parameters
  m_dDefaultOpenResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetDefaultOpenFlowResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  m_dDefaultClosedResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetDefaultClosedFlowResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  m_dRespOpenResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetRespiratoryOpenResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  m_dRespClosedResistance_cmH2O_s_Per_L = m_data.GetConfiguration().GetRespiratoryClosedResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  m_VentilatoryOcclusionPressure_cmH2O = m_data.GetConfiguration().GetVentilatoryOcclusionPressure(PressureUnit::cmH2O); //This increases the absolute max driver pressure
  m_PleuralComplianceSensitivity_Per_L = m_data.GetConfiguration().GetPleuralComplianceSensitivity(InverseVolumeUnit::Inverse_L);
  //Compartments
  m_Environment = m_data.GetCompartments().GetGasCompartment(BGE::EnvironmentCompartment::Ambient);
  m_AerosolMouth = m_data.GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::Mouth);
  m_AerosolTrachea = m_data.GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::Trachea);
  m_AerosolLeftBronchi = m_data.GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::LeftBronchi);
  m_AerosolLeftAlveoli = m_data.GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::LeftAlveoli);
  m_AerosolRightBronchi = m_data.GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::RightBronchi);
  m_AerosolRightAlveoli = m_data.GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::RightAlveoli);
  m_Lungs = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::Lungs);
  m_LeftLungExtravascular = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LeftLungIntracellular);
  m_RightLungExtravascular = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::RightLungIntracellular);
  m_Trachea = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::Trachea);
  m_TracheaO2 = m_Trachea->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_TracheaCO2 = m_Trachea->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());
  SELiquidCompartment* Aorta = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Aorta);
  m_AortaO2 = Aorta->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_AortaCO2 = Aorta->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());
  m_LeftAlveoliO2 = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::LeftAlveoli)->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_RightAlveoliO2 = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::RightAlveoli)->GetSubstanceQuantity(m_data.GetSubstances().GetO2());
  m_LeftAlveoliCO2 = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::LeftAlveoli)->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());
  m_RightAlveoliCO2 = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::RightAlveoli)->GetSubstanceQuantity(m_data.GetSubstances().GetCO2());
  m_MechanicalVentilatorConnection = m_data.GetCompartments().GetGasCompartment(BGE::MechanicalVentilatorCompartment::Connection);
  // Compartments we will process aerosol effects on
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::Trachea));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::LeftAlveoli));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::LeftBronchi));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::RightAlveoli));
  m_AerosolEffects.push_back(m_data.GetCompartments().GetLiquidCompartment(BGE::PulmonaryCompartment::RightBronchi));
  //Circuits
  m_RespiratoryCircuit = &m_data.GetCircuits().GetRespiratoryCircuit();
  //Nodes
  m_LeftAlveoli = m_RespiratoryCircuit->GetNode(BGE::RespiratoryNode::LeftAlveoli);
  m_LeftBronchi = m_RespiratoryCircuit->GetNode(BGE::RespiratoryNode::LeftBronchi);
  m_LeftPleuralCavity = m_RespiratoryCircuit->GetNode(BGE::RespiratoryNode::LeftPleuralCavity);
  m_RespiratoryMuscle = m_RespiratoryCircuit->GetNode(BGE::RespiratoryNode::RespiratoryMuscle);
  m_RightAlveoli = m_RespiratoryCircuit->GetNode(BGE::RespiratoryNode::RightAlveoli);
  m_RightBronchi = m_RespiratoryCircuit->GetNode(BGE::RespiratoryNode::RightBronchi);
  m_RightPleuralCavity = m_RespiratoryCircuit->GetNode(BGE::RespiratoryNode::RightPleuralCavity);
  m_Ambient = m_RespiratoryCircuit->GetNode(BGE::EnvironmentNode::Ambient);
  m_Stomach = m_RespiratoryCircuit->GetNode(BGE::RespiratoryNode::Stomach);
  //Paths
  m_TracheaToLeftBronchi = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::TracheaToLeftBronchi);
  m_TracheaToRightBronchi = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::TracheaToRightBronchi);
  m_LeftBronchiToLeftAlveoli = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::LeftBronchiToLeftAlveoli);
  m_RightBronchiToRightAlveoli = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::RightBronchiToRightAlveoli);
  m_RightPleuralCavityToRespiratoryMuscle = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::RightPleuralCavityToRespiratoryMuscle);
  m_LeftPleuralCavityToRespiratoryMuscle = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::LeftPleuralCavityToRespiratoryMuscle);
  m_DriverPressurePath = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::EnvironmentToRespiratoryMuscle);
  m_MouthToTrachea = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::MouthToTrachea);
  m_MouthToStomach = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::MouthToStomach);
  m_EnvironmentToLeftChestLeak = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::EnvironmentToLeftChestLeak);
  m_EnvironmentToRightChestLeak = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::EnvironmentToRightChestLeak);
  m_LeftAlveoliLeakToLeftPleuralCavity = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::LeftAlveoliLeakToLeftPleuralCavity);
  m_RightAlveoliLeakToRightPleuralCavity = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::RightAlveoliLeakToRightPleuralCavity);
  m_LeftPleuralCavityToEnvironment = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::LeftPleuralCavityToEnvironment);
  m_RightPleuralCavityToEnvironment = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::RightPleuralCavityToEnvironment);
  m_RightAlveoliToRightPleuralConnection = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::RightAlveoliToRightPleuralConnection);
  m_LeftAlveoliToLeftPleuralConnection = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::LeftAlveoliToLeftPleuralConnection);
  m_RightBronchiToRightPleuralConnection = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::RightBronchiToRightPleuralConnection);
  m_LeftBronchiToLeftPleuralConnection = m_RespiratoryCircuit->GetPath(BGE::RespiratoryPath::LeftBronchiToLeftPleuralConnection);
  m_ConnectionToMouth = m_data.GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPath(BGE::MechanicalVentilatorPath::ConnectionToMouth);
  m_GroundToConnection = m_data.GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().GetPath(BGE::MechanicalVentilatorPath::GroundToConnection);

  /// \todo figure out how to modify these resistances without getting the cv circuit - maybe add a parameter, like baroreceptors does
  m_RightPulmonaryCapillary = m_data.GetCircuits().GetCardiovascularCircuit().GetPath(BGE::CardiovascularPath::RightPulmonaryCapillariesToRightPulmonaryVeins);
  m_LeftPulmonaryCapillary = m_data.GetCircuits().GetCardiovascularCircuit().GetPath(BGE::CardiovascularPath::LeftPulmonaryCapillariesToLeftPulmonaryVeins);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system at steady state
///
/// \details
/// Initializes respiratory conditions if any are present.
///  <UL>
///   <LI>COPD</LI>
///   <LI>Lobar Pneumonia</LI>
///   <LI>ImpairedAlveolarExchange</LI>
///  </UL>
///
//--------------------------------------------------------------------------------------------------
void Respiratory::AtSteadyState()
{
  double respirationRate_Per_min = GetRespirationRate(FrequencyUnit::Per_min);
  double tidalVolume_L = GetTidalVolume(VolumeUnit::L);
  double totalLungCapacity_L = m_Patient->GetTotalLungCapacity(VolumeUnit::L);
  double residualVolume_L = m_Patient->GetResidualVolume(VolumeUnit::L);
  double vitalCapacity_L = totalLungCapacity_L - residualVolume_L;
  double expiratoryReserveVolume_L = m_InstantaneousFunctionalResidualCapacity_L - residualVolume_L;
  double inspiratoryReserveVolume_L = totalLungCapacity_L - m_InstantaneousFunctionalResidualCapacity_L - tidalVolume_L;
  double inspiratoryCapacity_L = totalLungCapacity_L - m_InstantaneousFunctionalResidualCapacity_L;
  m_Patient->GetRespirationRateBaseline().SetValue(respirationRate_Per_min, FrequencyUnit::Per_min);
  m_Patient->GetTidalVolumeBaseline().SetValue(tidalVolume_L, VolumeUnit::L);
  m_Patient->GetFunctionalResidualCapacity().SetValue(m_InstantaneousFunctionalResidualCapacity_L, VolumeUnit::L);
  m_Patient->GetVitalCapacity().SetValue(vitalCapacity_L, VolumeUnit::L);
  m_Patient->GetExpiratoryReserveVolume().SetValue(expiratoryReserveVolume_L, VolumeUnit::L);
  m_Patient->GetInspiratoryReserveVolume().SetValue(inspiratoryReserveVolume_L, VolumeUnit::L);
  m_Patient->GetInspiratoryCapacity().SetValue(inspiratoryCapacity_L, VolumeUnit::L);
  m_Patient->GetRespiratoryDriverAmplitudeBaseline().Set(GetRespirationDriverPressure());

  std::string typeString = "Initial Stabilization Homeostasis: ";
  if (m_data.GetState() == EngineState::AtSecondaryStableState)
    typeString = "Secondary Stabilization Homeostasis: ";

  std::stringstream ss;
  ss << typeString << "Patient respiration rate = " << respirationRate_Per_min << " bpm.";
  Info(ss);
  ss << typeString << "Patient tidal volume = " << tidalVolume_L << " L.";
  Info(ss);
  ss << typeString << "Patient functional residual capacity = " << m_InstantaneousFunctionalResidualCapacity_L << " L.";
  Info(ss);
  ss << typeString << "Patient vital capacity = " << vitalCapacity_L << " L.";
  Info(ss);
  ss << typeString << "Patient expiratory reserve volume = " << expiratoryReserveVolume_L << " L.";
  Info(ss);
  ss << typeString << "Patient inspiratory reserve volume = " << inspiratoryReserveVolume_L << " L.";
  Info(ss);
  ss << typeString << "Patient inspiratory capacity = " << inspiratoryCapacity_L << " L.";
  Info(ss);

  if (m_data.GetState() == EngineState::AtInitialStableState) { // At Resting State, apply conditions if we have them
    COPD();
    LobarPneumonia();
    //These conditions stack effects
    //If combined, it will be a fraction of the already affected alveolar surface area
    ImpairedAlveolarExchange();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system preprocess function
///
/// \details
/// Calculates the muscle pressure source that drives the respiratory system.
/// Updates airway resistance to account for changes arising from factors
/// like drugs and respiratory insults and interventions.
//// Updates the chest wall variable compliance. Handles all respiratory
/// insults and actions.
//--------------------------------------------------------------------------------------------------
void Respiratory::PreProcess()
{
  UpdatePleuralCompliance();
  ProcessAerosolSubstances();
  AirwayObstruction();
  UpdateObstructiveResistance();
  BronchoConstriction();
  BronchoDilation();
  Intubation();
  Pneumothorax();
  ConsciousRespiration();

  MechanicalVentilation();

  RespiratoryDriver();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system process function
///
/// \details
/// Ensures the selection of the respiratory system with or without the anesthesia machine.
/// Handles the integration of the anesthesia machine to the respiratory system when the anesthesia machine is turned on.
/// The integration of the anesthesia machine to the respiratory system is handled at run time by constructing a combined circuit of
/// the respiratory and anesthesia machine.
/// Handles lung volume changes during alveolar gas transfer.
/// Calculates physiological parameters such as respiration rate, tidal volume and others that belonging to the respiratory system.
//--------------------------------------------------------------------------------------------------
void Respiratory::Process()
{
  // Respiration circuit changes based on if Anesthesia Machine is on or off
  // When dynamic intercircuit connections work, we can stash off the respiration circuit in a member variable
  SEFluidCircuit& RespirationCircuit = m_data.GetCircuits().GetActiveRespiratoryCircuit();
  // Calc the circuits
  m_Calculator.Process(RespirationCircuit, m_dt_s);
  SEGasCompartmentGraph& RespirationGraph = m_data.GetCompartments().GetActiveRespiratoryGraph();
  SELiquidCompartmentGraph& AerosolGraph = m_data.GetCompartments().GetActiveAerosolGraph();
  // Transport substances
  m_GasTransporter.Transport(RespirationGraph, m_dt_s);
  if (m_AerosolMouth->HasSubstanceQuantities())
    m_AerosolTransporter.Transport(AerosolGraph, m_dt_s);
  //Update system data
  CalculateVitalSigns();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory system postprocess function
///
/// \details
/// Updates the current values of the gas volume fraction and gas volumes for the nodes in the respiratory circuit
/// or the nodes in the combined (respiratory + anesthesia machine) circuit when the anesthesia machine is turned on.
//--------------------------------------------------------------------------------------------------
void Respiratory::PostProcess()
{
  if (m_data.GetActions().GetPatientActions().HasOverride()
      && m_data.GetState() == EngineState::Active) {
    if (m_data.GetActions().GetPatientActions().GetOverride()->HasRespiratoryOverride()) {
      ProcessOverride();
    }
  }
  // Respiration circuit changes based on if Anesthesia Machine is on or off
  // When dynamic intercircuit connections work, we can stash off the respiration circuit in a member variable
  SEFluidCircuit& RespirationCircuit = m_data.GetCircuits().GetActiveRespiratoryCircuit();
  m_Calculator.PostProcess(RespirationCircuit);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Chest wall compliance modifier
///
/// \details
/// Adjusts the chest wall compliance based on the pleural volume. The variable compliance allows some level of
/// tolerance to mechanical stresses that may arise from events like airway obstruction. Currently, a
/// linear relation with an upper bound has been used.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdatePleuralCompliance()
{
  double dRightPleuralCompliance = m_RightPleuralCavityToRespiratoryMuscle->GetNextCompliance().GetValue(FlowComplianceUnit::L_Per_cmH2O);
  double dLeftPleuralCompliance = m_LeftPleuralCavityToRespiratoryMuscle->GetNextCompliance().GetValue(FlowComplianceUnit::L_Per_cmH2O);
  double dRightPleuralVolumeBaseline = m_RightPleuralCavity->GetVolumeBaseline().GetValue(VolumeUnit::L);
  double dLeftPleuralVolumeBaseline = m_LeftPleuralCavity->GetVolumeBaseline().GetValue(VolumeUnit::L);
  double dRightPleuralVolume = m_RightPleuralCavity->GetNextVolume().GetValue(VolumeUnit::L);
  double dLeftPleuralVolume = m_LeftPleuralCavity->GetNextVolume().GetValue(VolumeUnit::L);

  dRightPleuralCompliance = (dRightPleuralVolume - dRightPleuralVolumeBaseline) * m_PleuralComplianceSensitivity_Per_L * dRightPleuralCompliance + dRightPleuralCompliance;
  dLeftPleuralCompliance = (dLeftPleuralVolume - dLeftPleuralVolumeBaseline) * m_PleuralComplianceSensitivity_Per_L * dLeftPleuralCompliance + dLeftPleuralCompliance;

  dRightPleuralCompliance = LIMIT(dRightPleuralCompliance, 0.024, 24.);
  dLeftPleuralCompliance = LIMIT(dLeftPleuralCompliance, 0.024, 24.);

  //m_RightPleuralCavityToRespiratoryMuscle->GetNextCompliance().SetValue(dRightPleuralCompliance, FlowComplianceUnit::L_Per_cmH2O);
  //m_LeftPleuralCavityToRespiratoryMuscle->GetNextCompliance().SetValue(dLeftPleuralCompliance, FlowComplianceUnit::L_Per_cmH2O);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Compute deposited mass, update localized PD effects
///
/// \details
/// For each aerosol get the SIDE coefficient to determine deposited mass in each respiratory compartment.
/// Adjust the resistances between compartments as a function of deposited mass to reach validated data.
/// Liquid and solid aerosols are handled here.
//--------------------------------------------------------------------------------------------------
void Respiratory::ProcessAerosolSubstances()
{
  m_AverageLocalTissueBronchodilationEffects = 0.0; //No effect

  size_t numAerosols = m_AerosolMouth->GetSubstanceQuantities().size();
  if (numAerosols == 0)
    return;

  double inflammationCoefficient;

  // For this time step
  double mouthDepositied_ug = 0;
  double TracheaDepositied_ug = 0;
  double LeftBronchiDepositied_ug = 0;
  double leftAlveoliDepositied_ug = 0;
  double RightBronchiDepositied_ug = 0;
  double rightAlveoliDepositied_ug = 0;

  // Total amount deposited (including this time step)
  double mouthTotalDepositied_ug = 0;
  double TracheaTotalDepositied_ug = 0;
  double LeftBronchiTotalDepositied_ug = 0;
  double leftAlveoliTotalDepositied_ug = 0;
  double RightBronchiTotalDepositied_ug = 0;
  double rightAlveoliTotalDepositied_ug = 0;

  // Resistance Modifier Sum
  double mouthResistanceModifier = 1;
  double TracheaResistanceModifier = 1;
  double LeftBronchiResistanceModifier = 1;
  double leftAlveoliResistanceModifier = 1;
  double RightBronchiResistanceModifier = 1;
  double rightAlveoliResistanceModifier = 1;

  // Currently, there is no way to clear deposited particulate out of the respiratory system
  // Maybe we could have it to cough or some other excretion related method...

  SELiquidSubstanceQuantity* subQ;
  SELiquidSubstanceQuantity* tSubQ;
  const SizeIndependentDepositionEfficencyCoefficient* SIDECoeff;
  double combinedRightBronchodilationEffects = 0.0;
  double combinedLeftBronchodilationEffects = 0.0;
  for (size_t i = 0; i < numAerosols; i++) {
    //initialize substance
    subQ = m_AerosolMouth->GetSubstanceQuantities()[i];
    //Adjust inflammation coefficient (scaled down):
    inflammationCoefficient = subQ->GetSubstance().GetAerosolization().GetInflammationCoefficient().GetValue(); // Once for each subQ
    inflammationCoefficient *= 0.01;
    //Mouth
    SIDECoeff = &m_data.GetSubstances().GetSizeIndependentDepositionEfficencyCoefficient(subQ->GetSubstance()); // Once for each subQ
    mouthDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * m_AerosolMouth->GetInFlow(VolumePerTimeUnit::mL_Per_s) * m_dt_s * SIDECoeff->GetMouth();
    if (mouthDepositied_ug > subQ->GetMass(MassUnit::ug)) {
      mouthDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    } else
      subQ->GetMass().IncrementValue(-mouthDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    mouthTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(mouthDepositied_ug, MassUnit::ug).GetValue(MassUnit::ug);
    mouthResistanceModifier += mouthTotalDepositied_ug * inflammationCoefficient;
    //Trachea
    subQ = m_AerosolTrachea->GetSubstanceQuantities()[i];
    TracheaDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * m_AerosolTrachea->GetInFlow(VolumePerTimeUnit::mL_Per_s) * m_dt_s * SIDECoeff->GetTrachea();
    if (TracheaDepositied_ug > subQ->GetMass(MassUnit::ug)) {
      TracheaDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    } else
      subQ->GetMass().IncrementValue(-TracheaDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    TracheaTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(TracheaDepositied_ug, MassUnit::ug).GetValue(MassUnit::ug);
    TracheaResistanceModifier += TracheaTotalDepositied_ug * inflammationCoefficient;
    //Left DeadSpace
    subQ = m_AerosolLeftBronchi->GetSubstanceQuantities()[i];
    LeftBronchiDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * m_AerosolLeftBronchi->GetInFlow(VolumePerTimeUnit::mL_Per_s) * m_dt_s * SIDECoeff->GetBronchi();
    if (LeftBronchiDepositied_ug > subQ->GetMass(MassUnit::ug)) {
      LeftBronchiDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    } else
      subQ->GetMass().IncrementValue(-LeftBronchiDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    LeftBronchiTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(LeftBronchiDepositied_ug, MassUnit::ug).GetValue(MassUnit::ug);
    LeftBronchiResistanceModifier += LeftBronchiTotalDepositied_ug * inflammationCoefficient;
    //Left Alveoli
    subQ = m_AerosolLeftAlveoli->GetSubstanceQuantities()[i];
    leftAlveoliDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * m_AerosolLeftAlveoli->GetInFlow(VolumePerTimeUnit::mL_Per_s) * m_dt_s * SIDECoeff->GetAlveoli();
    if (leftAlveoliDepositied_ug > subQ->GetMass(MassUnit::ug)) {
      leftAlveoliDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    } else
      subQ->GetMass().IncrementValue(-leftAlveoliDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    leftAlveoliTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(leftAlveoliDepositied_ug, MassUnit::ug).GetValue(MassUnit::ug);
    leftAlveoliResistanceModifier += leftAlveoliTotalDepositied_ug * inflammationCoefficient;
    //Right DeadSpace
    subQ = m_AerosolRightBronchi->GetSubstanceQuantities()[i];
    RightBronchiDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * m_AerosolRightBronchi->GetInFlow(VolumePerTimeUnit::mL_Per_s) * m_dt_s * SIDECoeff->GetBronchi();
    if (RightBronchiDepositied_ug > subQ->GetMass(MassUnit::ug)) {
      RightBronchiDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    } else
      subQ->GetMass().IncrementValue(-RightBronchiDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    RightBronchiTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(RightBronchiDepositied_ug, MassUnit::ug).GetValue(MassUnit::ug);
    RightBronchiResistanceModifier += RightBronchiTotalDepositied_ug * inflammationCoefficient;
    //Right Alveoli
    subQ = m_AerosolRightAlveoli->GetSubstanceQuantities()[i];
    rightAlveoliDepositied_ug = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * m_AerosolRightAlveoli->GetInFlow(VolumePerTimeUnit::mL_Per_s) * m_dt_s * SIDECoeff->GetAlveoli();
    if (rightAlveoliDepositied_ug > subQ->GetMass(MassUnit::ug)) {
      rightAlveoliDepositied_ug = subQ->GetMass(MassUnit::ug);
      subQ->GetMass().SetValue(0, MassUnit::ug);
    } else
      subQ->GetMass().IncrementValue(-rightAlveoliDepositied_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    rightAlveoliTotalDepositied_ug = subQ->GetMassDeposited().IncrementValue(rightAlveoliDepositied_ug, MassUnit::ug).GetValue(MassUnit::ug);
    rightAlveoliResistanceModifier += rightAlveoliTotalDepositied_ug * inflammationCoefficient;

    // Apply the BronchioleModifier dilation effect
    // This is all just tuned to Albuterol - it'll work for other substances, and can be tuned using the other parameters (especially BronchioleModifier)
    if (subQ->GetSubstance().GetState() == CDM::enumSubstanceState::Liquid) {
      // Sum the Bronchiole Effects
      // Must be positive
      double bronchioleModifier = subQ->GetSubstance().GetAerosolization().GetBronchioleModifier().GetValue();

      // Diffuse into Tissues
      // We only process mass deposited on the lungs (dead space and alveoli)
      // We do not currently do anything with the mass in the mouth and Trachea
      // Could possibly let it go into the stomach somehow...
      tSubQ = m_LeftLungExtravascular->GetSubstanceQuantity(subQ->GetSubstance());
      tSubQ->GetMass().IncrementValue(LeftBronchiDepositied_ug + leftAlveoliDepositied_ug, MassUnit::ug);
      tSubQ->Balance(BalanceLiquidBy::Mass);
      combinedLeftBronchodilationEffects += bronchioleModifier * tSubQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);

      tSubQ = m_RightLungExtravascular->GetSubstanceQuantity(subQ->GetSubstance());
      tSubQ->GetMass().IncrementValue(RightBronchiDepositied_ug + rightAlveoliDepositied_ug, MassUnit::ug);
      tSubQ->Balance(BalanceLiquidBy::Mass);
      combinedRightBronchodilationEffects += bronchioleModifier * tSubQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
    }
  }

  //We're going to make the bronchodilation effects be based off of Albuterol.
  //Experimentally, 1mg of Albuterol given via a spacer device on an endotracheal tube caused a pulmonary resistance change of ~20% @cite mancebo1991effects.
  //The bronchi are ~30% of the total pulmonary resistance, so we'll make a dilation effect really strong with a normal dose.
  //This was tuned using a standard inhaler dose of 90ug
  m_AverageLocalTissueBronchodilationEffects = (combinedLeftBronchodilationEffects + combinedRightBronchodilationEffects) / 2.0;
  combinedLeftBronchodilationEffects = exp(-32894.0 * combinedLeftBronchodilationEffects);
  combinedRightBronchodilationEffects = exp(-32894.0 * combinedRightBronchodilationEffects);

  // Change resistances due to deposition
  double dTracheaResistance = m_MouthToTrachea->GetNextResistance(FlowResistanceUnit::cmH2O_s_Per_L) * TracheaResistanceModifier;
  double dLeftAlveoliResistance = m_LeftBronchiToLeftAlveoli->GetNextResistance(FlowResistanceUnit::cmH2O_s_Per_L) * leftAlveoliResistanceModifier;
  double dRightAlveoliResistance = m_RightBronchiToRightAlveoli->GetNextResistance(FlowResistanceUnit::cmH2O_s_Per_L) * rightAlveoliResistanceModifier;

  double dLeftBronchiResistance = m_TracheaToLeftBronchi->GetNextResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  double dRightBronchiResistance = m_TracheaToRightBronchi->GetNextResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  dLeftBronchiResistance *= LeftBronchiResistanceModifier * combinedLeftBronchodilationEffects;
  dRightBronchiResistance *= RightBronchiResistanceModifier * combinedRightBronchodilationEffects;
  dLeftBronchiResistance = LIMIT(dLeftBronchiResistance, m_dRespClosedResistance_cmH2O_s_Per_L, m_dRespOpenResistance_cmH2O_s_Per_L);
  dRightBronchiResistance = LIMIT(dRightBronchiResistance, m_dRespClosedResistance_cmH2O_s_Per_L, m_dRespOpenResistance_cmH2O_s_Per_L);

  m_MouthToTrachea->GetNextResistance().SetValue(dTracheaResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  m_TracheaToLeftBronchi->GetNextResistance().SetValue(dLeftBronchiResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  m_TracheaToRightBronchi->GetNextResistance().SetValue(dRightBronchiResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  m_LeftBronchiToLeftAlveoli->GetNextResistance().SetValue(dLeftAlveoliResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  m_RightBronchiToRightAlveoli->GetNextResistance().SetValue(dRightAlveoliResistance, FlowResistanceUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Modifies the pressure and/or flow at the mouth
///
/// \details
/// Handles the mechanical ventilation action that adds a flow and pressure source to instantaneously
/// set the respiratory connection (mouth) to user specified values.
//--------------------------------------------------------------------------------------------------
void Respiratory::MechanicalVentilation()
{
  if (m_data.GetActions().GetPatientActions().HasMechanicalVentilation()) {
    SEMechanicalVentilation* mv = m_data.GetActions().GetPatientActions().GetMechanicalVentilation();
    // You only get here if action is On
    m_data.SetAirwayMode(CDM::enumBioGearsAirwayMode::MechanicalVentilator);

    //Set the substance volume fractions ********************************************
    std::vector<SESubstanceFraction*> gasFractions = mv->GetGasFractions();

    //Reset the substance quantities at the connection
    for (SEGasSubstanceQuantity* subQ : m_MechanicalVentilatorConnection->GetSubstanceQuantities())
      subQ->SetToZero();

    //If no gas fractions specified, assume ambient
    if (gasFractions.empty()) {
      for (auto s : m_Environment->GetSubstanceQuantities()) {
        m_MechanicalVentilatorConnection->GetSubstanceQuantity(s->GetSubstance())->GetVolumeFraction().Set(s->GetVolumeFraction());
      }
    } else {
      //Has fractions defined
      for (auto f : gasFractions) {
        SESubstance& sub = f->GetSubstance();
        double fraction = f->GetFractionAmount().GetValue();

        //Do this, just in case it's something new
        m_data.GetSubstances().AddActiveSubstance(sub);

        //Now set it on the connection compartment
        //It has a NaN volume, so this will keep the same volume fraction no matter what's going on around it
        m_MechanicalVentilatorConnection->GetSubstanceQuantity(sub)->GetVolumeFraction().SetValue(fraction);
      }
    }

    //Apply the instantaneous flow ********************************************
    if (mv->HasFlow()) {
      m_ConnectionToMouth->GetNextFlowSource().Set(mv->GetFlow());
      //It may or may not be there
      if (!m_ConnectionToMouth->HasFlowSource()) {
        m_data.GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().StateChange();
      }
    } else {
      //If there's no flow specified, we need to remove the flow source
      if (m_ConnectionToMouth->HasNextFlowSource()) {
        m_ConnectionToMouth->GetNextFlowSource().Invalidate();
        m_data.GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit().StateChange();
      }
    }

    //Apply the instantaneous pressure ********************************************
    if (mv->HasPressure()) {
      //This is the pressure above ambient
      m_GroundToConnection->GetNextPressureSource().Set(mv->GetPressure());
    } else {
      //Pressure is same as ambient
      m_GroundToConnection->GetNextPressureSource().SetValue(0.0, PressureUnit::cmH2O);
    }
  } else if (m_data.GetAirwayMode() == CDM::enumBioGearsAirwayMode::MechanicalVentilator) {
    // Was just turned off
    m_data.SetAirwayMode(CDM::enumBioGearsAirwayMode::Free);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Respiratory driver pressure source
///
/// \details
/// Calculates the muscle pressure source pressure by using the chemical stimuli as feedback control mechanism.
/// The method reads the arterial O2 and CO2 partial pressures. Using these partial pressures, the method calculates the
/// alveolar ventilation from which the method calculates the target breathing frequency. The target breathing frequency is
/// used in the equation for muscle pressure source. The muscle pressure source is used as a driver for ventilation.
/// This method also calculates the drug modifiers that adjusts the depth and frequency of respiration.
//--------------------------------------------------------------------------------------------------
void Respiratory::RespiratoryDriver()
{
  ///\ToDo:  Running averages were mostly used for chemoreceptors that have been moved to Nervous.
  /// ToDo:  However, they are still used in some calculations in CalculateVitals.  Should we move/consolidate these?
  //Keep a running average of the Arterial Partial Pressures
  m_ArterialO2Average_mmHg.Sample(m_AortaO2->GetPartialPressure(PressureUnit::mmHg));
  m_ArterialCO2Average_mmHg.Sample(m_AortaCO2->GetPartialPressure(PressureUnit::mmHg));
  //Reset at start of cardiac cycle
  if (m_Patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
    m_ArterialO2PartialPressure_mmHg = m_ArterialO2Average_mmHg.Value();
    m_ArterialCO2PartialPressure_mmHg = m_ArterialCO2Average_mmHg.Value();
    m_ArterialO2Average_mmHg.Reset();
    m_ArterialCO2Average_mmHg.Reset();
  }

#ifdef TUNING
  m_ArterialO2PartialPressure_mmHg = 95.0;
  m_ArterialCO2PartialPressure_mmHg = 40.0;
#endif

  m_BreathingCycleTime_s += m_dt_s;
  if (m_ConsciousBreathing) //Conscious breathing
  {
    SEConsciousRespiration* cr = m_data.GetActions().GetPatientActions().GetConsciousRespiration();
    SEConsciousRespirationCommand* cmd = cr->GetActiveCommand();
    SEUseInhaler* ui = dynamic_cast<SEUseInhaler*>(cmd);
    if (ui != nullptr) {
      //We're using the inhaler, so just wait at this driver pressure
      m_DriverPressure_cmH2O = m_DriverPressurePath->GetPressureSource(PressureUnit::cmH2O);
      m_ConsciousBreathing = false;
    } else {
      //Just increase/decrease the driver pressure linearly to achieve the desired
      //pressure (attempting to reach a specific volume) at the end of the user specified period
      double Time_s = m_ConsciousRespirationPeriod_s - m_ConsciousRespirationRemainingPeriod_s;
      double Slope = (m_ConsciousEndPressure_cmH2O - m_ConsciousStartPressure_cmH2O) / m_ConsciousRespirationPeriod_s;
      //Form of y=mx+b
      m_DriverPressure_cmH2O = Slope * Time_s + m_ConsciousStartPressure_cmH2O;

      //Make it so we start a fresh cycle when we go back to spontaneous breathing
      //Adding 2.0 * timestamp just makes it greater than the TotalBreathingCycleTime_s
      m_VentilationFrequency_Per_min = m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min);
      m_BreathingCycleTime_s = 60.0 / m_VentilationFrequency_Per_min + 2.0;
    }
  } else //Spontaneous (i.e. not conscious) breathing
  {
    double TotalBreathingCycleTime_s = 0.0;
    if (m_VentilationFrequency_Per_min < 1.0) {
      TotalBreathingCycleTime_s = 0.0;
    } else {
      TotalBreathingCycleTime_s = 60.0 / m_VentilationFrequency_Per_min; //Total time of one breathing cycle
    }

    //Prepare for the next cycle -------------------------------------------------------------------------------
    if (m_BreathingCycleTime_s > TotalBreathingCycleTime_s) //End of the cycle or currently not breathing
    {
      m_PeakRespiratoryDrivePressure_cmH2O = GetRespirationDriverPressure(PressureUnit::cmH2O);
      m_VentilationFrequency_Per_min = GetRespirationDriverFrequency(FrequencyUnit::Per_min);
      //All actions that effect the target tidal volume and respiration rate are processed in the function below.  The
      //ventilation frequency (m_VentilationFrequency_Per_min) is calculated in this function.
      ProcessDriverActions();
      m_BreathingCycleTime_s = 0.0;
    }

    //Run the driver based on the waveform used in
    ///\@cite Albanese2015Integrated-----------------------------------------------------------------------------
    UpdateIERatio();
    double IERatio = m_IEscaleFactor * GetInspiratoryExpiratoryRatio().GetValue();
    double driverInspirationTime_s = (IERatio / (1.0 + IERatio) * TotalBreathingCycleTime_s);
    double driverExpirationTime_s = TotalBreathingCycleTime_s - driverInspirationTime_s;
    double tauExpiration_s = 0.2;

    //New driver
    if (m_BreathingCycleTime_s < driverInspirationTime_s) {
      //Inspiration
      m_DriverPressure_cmH2O = m_DefaultDrivePressure_cmH2O + (-m_PeakRespiratoryDrivePressure_cmH2O * std::pow(m_BreathingCycleTime_s, 2) / (driverInspirationTime_s * driverExpirationTime_s) + (m_PeakRespiratoryDrivePressure_cmH2O * TotalBreathingCycleTime_s * m_BreathingCycleTime_s / (driverInspirationTime_s * driverExpirationTime_s)));
    } else if (m_BreathingCycleTime_s < TotalBreathingCycleTime_s) {
      //Expiration
      m_DriverPressure_cmH2O = m_DefaultDrivePressure_cmH2O + m_PeakRespiratoryDrivePressure_cmH2O / (1.0 - std::exp(-driverExpirationTime_s / tauExpiration_s)) * (std::exp(-(m_BreathingCycleTime_s - driverInspirationTime_s) / tauExpiration_s) - std::exp(-driverExpirationTime_s / tauExpiration_s));
    } else {
      m_DriverPressure_cmH2O = m_DefaultDrivePressure_cmH2O;
    }

    Apnea();

    if (m_bNotBreathing) {
      m_DriverPressure_cmH2O = m_DefaultDrivePressure_cmH2O;
    }
  }

  //Push Driving Data to the Circuit -------------------------------------------------------------------------------
  m_DriverPressurePath->GetNextPressureSource().SetValue(m_DriverPressure_cmH2O, PressureUnit::cmH2O);
}
//-------------------------------------------------------------------------------------------------
///\brief
/// Process actions that affect respiratory driver
///
///\details
/// The tidal volume calculated from the target ventilation returned by the Chemoreceptor feedback can attenuated by many
/// factors.  Currently, we have a cardiac arrest effect, drug effects in the form of neuromuscular blockers and sedatives,
/// sepsis effects, and pain effects.  Process these actions here instead of in the RespiratoryDriver to clean up the code
/// and have a dedicated place for future actions that change driver output.
//--------------------------------------------------------------------------------------------------
void Respiratory::ProcessDriverActions()
{
  // Process Cardiac Arrest action
  double cardiacArrestEffect = 1.0;
  if (m_Patient->IsEventActive(CDM::enumPatientEvent::CardiacArrest)) {
    cardiacArrestEffect = 0.0;
  }
  //Process drug effects--adjust them based on neuromuscular block level
  SEDrugSystem& Drugs = m_data.GetDrugs();
  double DrugRRChange_Per_min = Drugs.GetRespirationRateChange(FrequencyUnit::Per_min);
  double DrugsTVChange_L = Drugs.GetTidalVolumeChange(VolumeUnit::L);
  double NMBModifier = 1.0;
  double SedationModifier = 1.0;

  // Check drug modifiers for effect on driver actions
  // \todo The propofol check needs to be investigated for all anethesia/sedative type drugs
  if (Drugs.GetNeuromuscularBlockLevel().GetValue() > 0.135) {
    NMBModifier = 0.0;
    DrugRRChange_Per_min = 0.0;
    DrugsTVChange_L = 0.0;
  } else if (Drugs.GetNeuromuscularBlockLevel().GetValue() > 0.11) {
    NMBModifier = 0.5;
    DrugRRChange_Per_min = 0.0;
    DrugsTVChange_L = 0.0;
  } else if (Drugs.GetSedationLevel().GetValue() > 0.15 && std::abs(m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min) + DrugRRChange_Per_min) < 1.0 && !m_data.GetSubstances().IsActive(*m_Propofol)) {
    SedationModifier = 0.0;
    DrugRRChange_Per_min = 0.0;
    DrugsTVChange_L = 0.0;
  } else if (Drugs.GetSedationLevel().GetValue() > 0.5 && std::abs(m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min) + DrugRRChange_Per_min) < 1.0 && m_data.GetSubstances().IsActive(*m_Propofol)) {
    SedationModifier = 0.0;
    DrugRRChange_Per_min = 0.0;
    DrugsTVChange_L = 0.0;
  }

  //Process Pain effects
  double painVAS = 0.1 * m_data.GetNervous().GetPainVisualAnalogueScale().GetValue(); //already processed pain score from nervous [0,10]
  double painModifier = 1.0 + 0.75 * (painVAS / (painVAS + 0.2));

  //Process infection effects--this won't lead to large change until infection tends towards sepsis
  double infectionModifier = 0.0;
  if (m_data.GetBloodChemistry().GetInflammatoryResponse().HasInflammationSource(CDM::enumInflammationSource::Infection)) {
    double baselineRR_Per_min = m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min);
    double sigmoidInput = 1.0 - m_data.GetBloodChemistry().GetInflammatoryResponse().GetTissueIntegrity().GetValue();
    infectionModifier = baselineRR_Per_min * sigmoidInput / (sigmoidInput + 0.5);
  }

  //Apply tidal volume modifiers by adjusting the peak driver pressure
  m_PeakRespiratoryDrivePressure_cmH2O *= cardiacArrestEffect;
  m_PeakRespiratoryDrivePressure_cmH2O *= NMBModifier;
  m_PeakRespiratoryDrivePressure_cmH2O *= (1.0 + DrugsTVChange_L / m_Patient->GetTidalVolumeBaseline(VolumeUnit::L));

  //update PH during stabilization
  if (m_data.GetState() < EngineState::Active) {
    m_arterialPHBaseline = m_data.GetBloodChemistry().GetArterialBloodPH().GetValue();
    m_data.GetBloodChemistry().GetArterialBloodPHBaseline().SetValue(m_arterialPHBaseline);
  }

  //That tidal volume cannot exceed 1/2 * Vital Capacity after modifications. m_MaxDriverPressure is set up to correspond to this maximum tidal volume
  m_PeakRespiratoryDrivePressure_cmH2O = std::max(m_PeakRespiratoryDrivePressure_cmH2O, m_MaxDriverPressure_cmH2O); //"Max" somewhat of a misnomer, since driver pressures are negative
  //Maximum allowable respiration frequency
  double maximumVentilationFrequency_Per_min = m_data.GetConfiguration().GetPulmonaryVentilationRateMaximum(VolumePerTimeUnit::L_Per_min) / (m_Patient->GetVitalCapacity(VolumeUnit::L) / 2.0);

  m_VentilationFrequency_Per_min += infectionModifier;
  m_VentilationFrequency_Per_min *= painModifier;
  m_VentilationFrequency_Per_min *= NMBModifier * SedationModifier;
  m_VentilationFrequency_Per_min += DrugRRChange_Per_min;

  //Make sure the the ventilation frequency is not negative or greater than maximum achievable based on ventilation
  m_VentilationFrequency_Per_min = BLIM(m_VentilationFrequency_Per_min, 0.0, maximumVentilationFrequency_Per_min);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Airway obstruction
///
/// \details
/// Various factors can trigger upper airway obstruction. The Biogears engine handles airway obstruction that arise from
/// the partial or complete obstruction of the upper airways by foreign objects. The model accounts for such obstruction by setting the
/// flow resistance of the trachea compartment in proportion to the severity of the obstruction.
/// The function updates the tracheal flow resistance by modifying the resistance across the airway to Trachea node path.
//--------------------------------------------------------------------------------------------------
void Respiratory::AirwayObstruction()
{
  if (m_PatientActions->HasAirwayObstruction()) {
    double Severity = m_PatientActions->GetAirwayObstruction()->GetSeverity().GetValue();
    double AirwayResistance = m_MouthToTrachea->GetNextResistance().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
    double dClosedResistance = AirwayResistance;
    AirwayResistance = GeneralMath::ResistanceFunction(5.0, m_dRespOpenResistance_cmH2O_s_Per_L, dClosedResistance, Severity);
    m_MouthToTrachea->GetNextResistance().SetValue(AirwayResistance, FlowResistanceUnit::cmH2O_s_Per_L);

    //This flag tells this function to ramp down resistors once the Airway obstruction has been deactivated
    m_HadAirwayObstruction = true;
  } else {
    if (m_HadAirwayObstruction) {
      //This block is entered when an airway obstruction action has been deactivated.
      //We need to gradually scale back resistances--returning to baseline in one time step causes from high severities circuit instabilities.
      //Since other functions use these resistors (like Aerosol Deposition), we check to make sure that an obstruction
      //was previously active so that we don't overwrite a different action's circuit changes
      const double scale = 0.02;
      const double lastAirwayResistance = m_MouthToTrachea->GetResistance().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
      const double baselineAirwayResistance = m_MouthToTrachea->GetResistanceBaseline().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
      const double nextAirwayResistance = lastAirwayResistance + scale * (baselineAirwayResistance - lastAirwayResistance);
      m_MouthToTrachea->GetNextResistance().SetValue(nextAirwayResistance, FlowResistanceUnit::cmH2O_s_Per_L);
      //We'll say we're back to normal if there's < 1% difference from baseline.  When that happens, set HadBronchonstriction back to false
      //to reflect return to baseline physiology
      if (std::abs((lastAirwayResistance - baselineAirwayResistance) / baselineAirwayResistance) < 0.01) {
        m_HadAirwayObstruction = false;
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Bronchoconstriction
///
/// \details
/// Bronchoconstriction involves the tightening of smooth muscles surrounding bronchi. The effect of such airway constriction is the
/// reduction of air flow or increase of flow resistance of the lower airways. The Biogears model handles bronchoconstriction by increasing
/// the flow resistances of the bronchi compartments. The function updates the bronchial resistances in proportion to the severity of the
/// bronchoconstriction.
//--------------------------------------------------------------------------------------------------
void Respiratory::BronchoConstriction()
{
  if (m_PatientActions->HasBronchoconstriction()) {
    double LeftBronchiResistance = m_TracheaToLeftBronchi->GetNextResistance().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
    double RightBronchiResistance = m_TracheaToRightBronchi->GetNextResistance().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
    const double dSeverity = m_PatientActions->GetBronchoconstriction()->GetSeverity().GetValue();
    double dClosedResistance = LeftBronchiResistance;

    //Constrictive effects stack more rapidly in new driver, so base of resistance function has been changed from 70 to 10.
    LeftBronchiResistance = GeneralMath::ResistanceFunction(10.0, m_dRespOpenResistance_cmH2O_s_Per_L, dClosedResistance, dSeverity);
    dClosedResistance = RightBronchiResistance;
    RightBronchiResistance = GeneralMath::ResistanceFunction(10.0, m_dRespOpenResistance_cmH2O_s_Per_L, dClosedResistance, dSeverity);

    m_TracheaToLeftBronchi->GetNextResistance().SetValue(LeftBronchiResistance, FlowResistanceUnit::cmH2O_s_Per_L);
    m_TracheaToRightBronchi->GetNextResistance().SetValue(RightBronchiResistance, FlowResistanceUnit::cmH2O_s_Per_L);

    //This flag tell this function to ramp resistors back down to baseline once Bronchoconstriction action is inactivated
    m_HadBronchoconstriction = true;
  } else {
    if (m_HadBronchoconstriction) {
      //This block is entered when a bronchoconstriction action has been deactivated.
      //We need to gradually scale back resistances--returning to baseline in one time step causes from high severities circuit instabilities.
      //Since other functions use these resistors (like Asthma), we check to make sure that a bronchoconstriction
      // was previously active so that we don't overwrite a different action's circuit changes
      const double scale = 0.02;
      const double lastLeftBronchiResistance = m_TracheaToLeftBronchi->GetResistance().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
      const double baselineLeftBronchiResistance = m_TracheaToLeftBronchi->GetResistanceBaseline().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
      const double lastRightBronchiResistance = m_TracheaToRightBronchi->GetResistance().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
      const double baselineRightBronchiResistance = m_TracheaToRightBronchi->GetResistanceBaseline().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
      const double nextLeftBronchiResistance = lastLeftBronchiResistance + scale * (baselineLeftBronchiResistance - lastLeftBronchiResistance);
      const double nextRightBronchiResistance = lastRightBronchiResistance + scale * (baselineRightBronchiResistance - lastRightBronchiResistance);
      m_TracheaToLeftBronchi->GetNextResistance().SetValue(nextLeftBronchiResistance, FlowResistanceUnit::cmH2O_s_Per_L);
      m_TracheaToRightBronchi->GetNextResistance().SetValue(nextRightBronchiResistance, FlowResistanceUnit::cmH2O_s_Per_L);
      //We'll say we're back to normal if there's < 1% difference from baseline.  When that happens, set HadBronchonstriction back to false
      //to reflect return to baseline physiology
      if (std::abs((lastLeftBronchiResistance - baselineLeftBronchiResistance) / baselineLeftBronchiResistance) < 0.01 && std::abs((lastRightBronchiResistance - baselineRightBronchiResistance) / baselineRightBronchiResistance) < 0.01) {
        m_HadBronchoconstriction = false;
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Bronchodilation PD effects.
///
/// \details
/// This reduces the bronchi resitances based on drug PD effects (i.e., plasma concentrations).
//--------------------------------------------------------------------------------------------------
void Respiratory::BronchoDilation()
{
  //Note: this could constrict with a negative number from PD

  //We're going to make the bronchodilation effects be based off of Albuterol.
  //A value of 1.0 will be the effective Albuterol EMax (with the current, non-overdose implementation), and -1.0 will be a bronchconstriction the same percentage in the other direction.
  //Experimentally, 1mg of Albuterol given via a spacer device on an endotracheal tube caused a pulmonary resistance change of ~20% @cite mancebo1991effects.
  //The bronchi are ~30% of the total pulmonary resistance, so we'll make a dilation effect of 1.0 be at the respiratory open resistance.
  //Dilation effect values have max effect at 1 and below -1, so anything outside of that will maintain that effect.
  double bronchoDilationEffect = m_data.GetDrugs().GetBronchodilationLevel().GetValue();
  double resTrack = 0.0;

  if (bronchoDilationEffect != 0.0) {
    //Note: It'll pretty much always get in here because there's epinephrine present
    // Get the path resistances
    double dLeftBronchiResistance = m_TracheaToLeftBronchi->GetNextResistance().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
    double dRightBronchiResistance = m_TracheaToRightBronchi->GetNextResistance().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
    if (bronchoDilationEffect >= 0.0) // positive, therefore dilation
    {
      bronchoDilationEffect = std::min(bronchoDilationEffect, 1.0);
      dLeftBronchiResistance = GeneralMath::ResistanceFunction(10.0, m_dRespClosedResistance_cmH2O_s_Per_L, dLeftBronchiResistance, bronchoDilationEffect);
      dRightBronchiResistance = GeneralMath::ResistanceFunction(10.0, m_dRespClosedResistance_cmH2O_s_Per_L, dRightBronchiResistance, bronchoDilationEffect);
    } else //negative, therefore constriction
    {
      bronchoDilationEffect = std::min(-bronchoDilationEffect, 1.0);
      dLeftBronchiResistance = GeneralMath::ResistanceFunction(10.0, m_dRespOpenResistance_cmH2O_s_Per_L, dLeftBronchiResistance, bronchoDilationEffect);
      dRightBronchiResistance = GeneralMath::ResistanceFunction(10.0, m_dRespOpenResistance_cmH2O_s_Per_L, dRightBronchiResistance, bronchoDilationEffect);
      resTrack = dRightBronchiResistance;
    }
    m_TracheaToLeftBronchi->GetNextResistance().SetValue(dLeftBronchiResistance, FlowResistanceUnit::cmH2O_s_Per_L);
    m_TracheaToRightBronchi->GetNextResistance().SetValue(dRightBronchiResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Esophageal Intubation
///
/// \details
/// During mechanical ventilation, one of the clinical complications of endotracheal intubation is esophageal intubation. This involves the
/// misplacement of the tube down the esophagus. Such event prohibits air flow into or out of the lungs. The Biogers circuit handles
/// this respiratory distress by manipulating the tracheal resistance. When esophageal intubation incidence is triggered, significantly large
/// resistance is assigned to the trachea compartment. Otherwise, the esophageal compartment resistance is set to be significantly
/// large value under normal condition.
//--------------------------------------------------------------------------------------------------
void Respiratory::Intubation()
{
  //Don't modify the stomach on environment changes
  if (m_Ambient->GetNextPressure(PressureUnit::cmH2O) != m_Ambient->GetPressure(PressureUnit::cmH2O)) {
    //The environment just changed
    //Keep volume the same by changing the pressure equally on both sides
    double pressureChange_cmH2O = m_Ambient->GetNextPressure(PressureUnit::cmH2O) - m_Ambient->GetPressure(PressureUnit::cmH2O);
    m_Stomach->GetNextPressure().IncrementValue(pressureChange_cmH2O, PressureUnit::cmH2O);
  }

  if (m_PatientActions->HasIntubation()) {
    m_data.SetIntubation(CDM::enumOnOff::On);
    SEIntubation* intubation = m_PatientActions->GetIntubation();
    switch (intubation->GetType()) {
    case CDM::enumIntubationType::Tracheal: {
      // The proper way to intubate
      // Airway mode handles this case by default
      break;
    }
    case CDM::enumIntubationType::Esophageal: {
      // Allow air flow between Airway and Stomach
      ///\todo Make this a modifier (i.e. multiplier), instead of setting it directly
      m_MouthToStomach->GetNextResistance().SetValue(1.2, FlowResistanceUnit::cmH2O_s_Per_L);
      // Stop air flow between the Airway and Trachea
      //This is basically an open switch.  We don't need to worry about anyone else modifying it if this action is on.
      m_MouthToTrachea->GetNextResistance().SetValue(m_dDefaultOpenResistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
      break;
    }
    case CDM::enumIntubationType::RightMainstem: {
      m_TracheaToLeftBronchi->GetNextResistance().SetValue(m_dRespOpenResistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
      break;
    }
    case CDM::enumIntubationType::LeftMainstem: {
      m_TracheaToRightBronchi->GetNextResistance().SetValue(m_dRespOpenResistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
      break;
    }
    default:
      break;
    }
  } else {
    m_data.SetIntubation(CDM::enumOnOff::Off);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Tension pneumothorax
///
/// \details
/// Pneumothorax is an abnormal accumulation of air in the pleural cavity. Tension pneumothorax occurs when
/// air escapes into the pleural space on inspiration, but cannot return on expiration. Different types of pneumothorces are known.
/// The Biogears engine models  open (sucking chest wound) and closed (intact thoracic cage) pnumothoraces whereas occlusive dressing
/// and needle decompression are modeled as interventions.
/// The function models pneumothorax and needle decompression by opening respiratory circuits that allow air flows into
/// and out of the pleural cavity. Under normal condition, the path between the lung node or the outside environment and the intrapleural cavity
/// is blocked via significantly high resistances that serve as open switches. When open or closed pneumothorax is triggered, these resistances
/// are reduced in proportion to the severity of the pneumothorax. This reduction permits air flow into the pleural cavity a
/// and cause respiratory and cardiovascular distress.
/// The function allows the selection of tension pneumothorax and/or needle decompression events on the left or right lung side. The effects on
/// the cardiovascular system parameters is accounted through modifiers that scale in proportion to the accumulated gas.
//--------------------------------------------------------------------------------------------------
void Respiratory::Pneumothorax()
{
  if (m_PatientActions->HasTensionPneumothorax()) {
    // Minimum flow resistance for the chest cavity or alveoli leak
    double dPneumoMinFlowResistance_cmH2O_s_Per_L = 10.0;
    // Maximum flow resistance for the chest cavity or alveoli leak
    double dPneumoMaxFlowResistance_cmH2O_s_Per_L = m_dDefaultOpenResistance_cmH2O_s_Per_L;
    // Flow resistance for the decompression needle, if used
    double dNeedleFlowResistance_cmH2O_s_Per_L = 15.0;

    if (m_PatientActions->HasLeftOpenTensionPneumothorax()) {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetLeftOpenTensionPneumothorax()->GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = dPneumoMaxFlowResistance_cmH2O_s_Per_L;
      if (severity > 0.0 && !m_PatientActions->HasLeftChestOcclusiveDressing()) {
        resistance_cmH2O_s_Per_L = dPneumoMinFlowResistance_cmH2O_s_Per_L / std::pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = std::min(resistance_cmH2O_s_Per_L, dPneumoMaxFlowResistance_cmH2O_s_Per_L);
      m_EnvironmentToLeftChestLeak->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
      if (severity == 0) {
        m_EnvironmentToLeftChestLeak->SetNextValve(CDM::enumOpenClosed::Open);
      }
      if (m_PatientActions->HasLeftNeedleDecompression()) {
        DoLeftNeedleDecompression(dNeedleFlowResistance_cmH2O_s_Per_L);
      }
    }

    if (m_PatientActions->HasRightOpenTensionPneumothorax()) {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetRightOpenTensionPneumothorax()->GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = dPneumoMaxFlowResistance_cmH2O_s_Per_L;
      if (severity > 0.0 && !m_PatientActions->HasRightChestOcclusiveDressing()) {
        resistance_cmH2O_s_Per_L = dPneumoMinFlowResistance_cmH2O_s_Per_L / std::pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = std::min(resistance_cmH2O_s_Per_L, dPneumoMaxFlowResistance_cmH2O_s_Per_L);
      m_EnvironmentToRightChestLeak->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
      if (severity == 0) {
        m_EnvironmentToRightChestLeak->SetNextValve(CDM::enumOpenClosed::Open);
      }

      if (m_PatientActions->HasRightNeedleDecompression()) {
        DoRightNeedleDecompression(dNeedleFlowResistance_cmH2O_s_Per_L);
      }
    }

    if (m_PatientActions->HasLeftClosedTensionPneumothorax()) {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetLeftClosedTensionPneumothorax()->GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = dPneumoMaxFlowResistance_cmH2O_s_Per_L;
      if (severity > 0.0) {
        resistance_cmH2O_s_Per_L = dPneumoMinFlowResistance_cmH2O_s_Per_L / std::pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = std::min(resistance_cmH2O_s_Per_L, dPneumoMaxFlowResistance_cmH2O_s_Per_L);
      m_LeftAlveoliLeakToLeftPleuralCavity->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
      if (severity == 0) {
        m_LeftAlveoliLeakToLeftPleuralCavity->SetNextValve(CDM::enumOpenClosed::Open);
      }

      if (m_PatientActions->HasLeftNeedleDecompression()) {
        DoLeftNeedleDecompression(dNeedleFlowResistance_cmH2O_s_Per_L);
      }
    }

    if (m_PatientActions->HasRightClosedTensionPneumothorax()) {
      // Scale the flow resistance through the chest opening based on severity
      double severity = m_PatientActions->GetRightClosedTensionPneumothorax()->GetSeverity().GetValue();
      double resistance_cmH2O_s_Per_L = dPneumoMaxFlowResistance_cmH2O_s_Per_L;
      if (severity > 0.0) {
        resistance_cmH2O_s_Per_L = dPneumoMinFlowResistance_cmH2O_s_Per_L / std::pow(severity, 2.0);
      }
      resistance_cmH2O_s_Per_L = std::min(resistance_cmH2O_s_Per_L, dPneumoMaxFlowResistance_cmH2O_s_Per_L);
      m_RightAlveoliLeakToRightPleuralCavity->GetNextResistance().SetValue(resistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
      if (severity == 0) {
        m_RightAlveoliLeakToRightPleuralCavity->SetNextValve(CDM::enumOpenClosed::Open);
      }

      if (m_PatientActions->HasRightNeedleDecompression()) {
        DoRightNeedleDecompression(dNeedleFlowResistance_cmH2O_s_Per_L);
      }
    }

    //Check for interventions without insult
    if (!m_PatientActions->HasLeftClosedTensionPneumothorax() && !m_PatientActions->HasLeftOpenTensionPneumothorax()) {
      if (m_PatientActions->HasLeftChestOcclusiveDressing() || m_PatientActions->HasLeftNeedleDecompression()) {
        /// \error Patient: Cannot perform an intervention if Tension Pneumothorax is not present on that side.
        Error("Cannot perform an intervention if Tension Pneumothorax is not present on that side.");
        m_PatientActions->RemoveLeftChestOcclusiveDressing();
        m_PatientActions->RemoveLeftNeedleDecompression();
        return;
      }
    }
    if (!m_PatientActions->HasRightClosedTensionPneumothorax() && !m_PatientActions->HasRightOpenTensionPneumothorax()) {
      if (m_PatientActions->HasRightChestOcclusiveDressing() || m_PatientActions->HasRightNeedleDecompression()) {
        /// \error Patient: Cannot perform an intervention if Tension Pneumothorax is not present on that side.
        Error("Cannot perform an intervention if Tension Pneumothorax is not present on that side.");
        m_PatientActions->RemoveRightChestOcclusiveDressing();
        m_PatientActions->RemoveRightNeedleDecompression();
        return;
      }
    }
  } else {
    // Check for cases where the needle decompression or occlusive dressing actions are called without having
    // initiated a pneumothorax action
    if (m_PatientActions->HasNeedleDecompression()) {
      /// \error Patient: can't process needle decompression if no pneumothorax is present
      Error("Cannot perform a Needle Decompression intervention if Tension Pneumothorax is not present");
      m_PatientActions->RemoveLeftNeedleDecompression();
      m_PatientActions->RemoveRightNeedleDecompression();
      return;
    }
    if (m_PatientActions->HasChestOcclusiveDressing()) {
      /// \error Patient: can't process a chest occlusive dressing if no pneumothorax is present
      Error("Cannot perform a Chest Occlusive Dressing intervention if Tension Pneumothorax is not present");
      m_PatientActions->RemoveLeftChestOcclusiveDressing();
      m_PatientActions->RemoveRightChestOcclusiveDressing();
      return;
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Conscious respiration
///
/// \param  None
///
/// \return void
///
/// \details
/// This method determines when to process a conscious respiration command and removes the command
/// from the stack.
//--------------------------------------------------------------------------------------------------
void Respiratory::ConsciousRespiration()
{
  if (m_ConsciousBreathing && m_ConsciousRespirationRemainingPeriod_s >= 0.0) {
    m_ConsciousRespirationRemainingPeriod_s -= m_dt_s;

    if (m_ConsciousRespirationRemainingPeriod_s <= 0.0) { // We are done with this command
      m_ConsciousRespirationRemainingPeriod_s = 0.0;
      m_ConsciousRespirationPeriod_s = 0.0;
      m_ConsciousStartPressure_cmH2O = 0.0;
      m_ConsciousEndPressure_cmH2O = 0.0;
      m_ExpiratoryReserveVolumeFraction = -1.0;
      m_InspiratoryCapacityFraction = -1.0;
      m_ConsciousBreathing = false;
      SEConsciousRespiration* cr = m_PatientActions->GetConsciousRespiration();
      m_ss << "Completed Conscious Respiration Command : " << *cr->GetActiveCommand();
      Info(m_ss);
      cr->RemoveActiveCommand();
    }
  }

  //We'll wait for any current commands to complete first - only one at a time
  if (!m_ConsciousBreathing && m_PatientActions->HasConsciousRespiration()) {
    SEConsciousRespiration* cr = m_PatientActions->GetConsciousRespiration();
    SEConsciousRespirationCommand* cmd = cr->GetActiveCommand();
    ProcessConsciousRespiration(*cmd);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Process conscious respiration
///
/// \param  cmd - Conscious respiration command
///
/// \details
/// Processes conscious respiration commands.
//--------------------------------------------------------------------------------------------------
void Respiratory::ProcessConsciousRespiration(SEConsciousRespirationCommand& cmd)
{
  if (m_ConsciousRespirationRemainingPeriod_s > 0) {
    Error("Already processing a Conscious Respiration Command, ignoring this command");
    return;
  }

  m_ConsciousBreathing = true;
  m_ConsciousStartPressure_cmH2O = m_DriverPressurePath->GetPressureSource(PressureUnit::cmH2O);

  SEBreathHold* bh = dynamic_cast<SEBreathHold*>(&cmd);
  if (bh != nullptr) {
    m_ConsciousRespirationRemainingPeriod_s = bh->GetPeriod().GetValue(TimeUnit::s);
    m_ConsciousRespirationPeriod_s = m_ConsciousRespirationRemainingPeriod_s;
    m_ConsciousEndPressure_cmH2O = m_ConsciousStartPressure_cmH2O;

    return;
  }
  SEForcedExhale* fe = dynamic_cast<SEForcedExhale*>(&cmd);
  if (fe != nullptr) {
    m_ExpiratoryReserveVolumeFraction = fe->GetExpiratoryReserveVolumeFraction().GetValue();
    m_ConsciousRespirationRemainingPeriod_s = fe->GetPeriod().GetValue(TimeUnit::s);
    m_ConsciousRespirationPeriod_s = m_ConsciousRespirationRemainingPeriod_s;

    //Pressure effects
    double lastVolume_L = GetTotalLungVolume(VolumeUnit::L);
    double TargetVolume_L = m_InitialResidualVolume_L + m_InitialExpiratoryReserveVolume_L * (1.0 - m_ExpiratoryReserveVolumeFraction);
    m_ConsciousEndPressure_cmH2O = m_DriverPressure_cmH2O - 10.0 * (TargetVolume_L - lastVolume_L);

    return;
  }
  SEForcedInhale* fi = dynamic_cast<SEForcedInhale*>(&cmd);
  if (fi != nullptr) {
    m_InspiratoryCapacityFraction = fi->GetInspiratoryCapacityFraction().GetValue();
    m_ConsciousRespirationRemainingPeriod_s = fi->GetPeriod().GetValue(TimeUnit::s);
    m_ConsciousRespirationPeriod_s = m_ConsciousRespirationRemainingPeriod_s;

    //Pressure effects
    double lastVolume_L = GetTotalLungVolume(VolumeUnit::L);
    double TargetVolume_L = m_InitialFunctionalResidualCapacity_L + m_InitialInspiratoryCapacity_L * m_InspiratoryCapacityFraction;
    m_ConsciousEndPressure_cmH2O = m_DriverPressure_cmH2O - 10.0 * (TargetVolume_L - lastVolume_L);

    return;
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Left Side Needle Decompression
///
/// \param  dFlowResistance - Resistance value for air flow through the needle
///
/// \details
/// Used for left side needle decompression. this is an intervention (action) used to treat left
/// side tension pneumothorax
//--------------------------------------------------------------------------------------------------
void Respiratory::DoLeftNeedleDecompression(double dFlowResistance)
{
  //Leak flow resistance that is scaled in proportion to Lung resistance, depending on severity
  double dScalingFactor = 0.5; //Tuning parameter to allow gas flow due to needle decompression using lung resistance as reference
  double dFlowResistanceLeftNeedle = dScalingFactor * dFlowResistance;
  m_LeftPleuralCavityToEnvironment->GetNextResistance().SetValue(dFlowResistanceLeftNeedle, FlowResistanceUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Right Side Needle Decompression
///
/// \param  dFlowResistance - Resistance value for air flow through the needle
///
/// \details
/// Used for right side needle decompression. this is an intervention (action) used to treat right
/// side tension pneumothorax
//--------------------------------------------------------------------------------------------------
void Respiratory::DoRightNeedleDecompression(double dFlowResistance)
{
  //Leak flow resistance that is scaled in proportion to Lung resistance, depending on severity
  double dScalingFactor = 0.5; //Tuning parameter to allow gas flow due to needle decompression using lung resistance as reference
  double dFlowResistanceRightNeedle = dScalingFactor * dFlowResistance;
  m_RightPleuralCavityToEnvironment->GetNextResistance().SetValue(dFlowResistanceRightNeedle, FlowResistanceUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Chronic Obstructive Pulmonary Disease (COPD)
///
/// \param  None
///
/// \return void
///
/// \details
/// This method handles the COPD condition. It determines if the patient has COPD, and if so,
/// calculates a set of multipliers to model the various symptoms. This method should only
/// run once per simulation. Note that the bronchitis symptom (airway obstruction) is handled by
/// another method that is called every time-step in pre-process.
//--------------------------------------------------------------------------------------------------
void Respiratory::COPD()
{
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease()) {
    double dBronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetBronchitisSeverity().GetValue();
    double dEmphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetEmphysemaSeverity().GetValue();
    double dLeftLungFraction = 1.0;
    double dRightLungFraction = 1.0;

    // Calculate Pulmonary Capillary Resistance Multiplier based on severities
    double dMaxSeverity = std::max(dBronchitisSeverity, dEmphysemaSeverity);
    // Resistance is based on a a simple linear regression, with maximum of 3x the pulmonary resistance baseline
    double dPulmonaryResistanceMultiplier = GeneralMath::LinearInterpolator(0.0, 1.0, 1.0, 3, dMaxSeverity);
    // Call UpdatePulmonaryCapillaryResistance
    UpdatePulmonaryCapillaryResistance(dPulmonaryResistanceMultiplier, dLeftLungFraction, dRightLungFraction);

    // Calculate Alveoli Compliance Multiplier based on severities.  Setting both lung fractions to 1 (as above) will have large impact on
    // compliances, so we don't need to make the severity too high to get the changes we want.  Scale from 0 to 0.35
    double dCompilanceScalingFactor = GeneralMath::LinearInterpolator(0, 1.0, 0, 0.35, dEmphysemaSeverity);
    // Call UpdateAlveoliCompliance
    UpdateAlveoliCompliance(dCompilanceScalingFactor, dLeftLungFraction, dRightLungFraction);

    // Calculate Gas Diffusion Surface Area scaling factor in the same ways as alveoli compliance
    double dGasDiffScalingFactor = GeneralMath::LinearInterpolator(0, 1.0, 0, 0.3, dEmphysemaSeverity);
    // Call UpdateGasDiffusionSurfaceArea
    UpdateGasDiffusionSurfaceArea(dGasDiffScalingFactor, dLeftLungFraction, dRightLungFraction);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Impaired Alveolar Exchange
///
/// \param  None
///
/// \return void
///
/// \details
/// This method handles the Impaired Alveolar Exchange condition.
//--------------------------------------------------------------------------------------------------
void Respiratory::ImpairedAlveolarExchange()
{
  if (!m_data.GetConditions().HasImpairedAlveolarExchange()) {
    return;
  }

  if (!m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedSurfaceArea() && !m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedFraction()) {
    /// \error Fatal: The Impaired Alveolar Exchange action must include either a surface area of fraction.
    Fatal("The Impaired Alveolar Exchange action must include either a surface area of fraction.");
  }

  double alveoliDiffusionArea_cm2 = m_Patient->GetAlveoliSurfaceArea(AreaUnit::cm2);
  if (m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedSurfaceArea() && m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedFraction()) {
    /// \error Error: The Impaired Alveolar Exchange action cannot have both an impaired surface area and impaired fraction defined. Defaulting to the surface area value.
    Warning("The Impaired Alveolar Exchange action cannot have both an impaired surface area and impaired fraction defined. Defaulting to the surface area value.");
    alveoliDiffusionArea_cm2 = alveoliDiffusionArea_cm2 - m_data.GetConditions().GetImpairedAlveolarExchange()->GetImpairedSurfaceArea(AreaUnit::cm2);
  } else if (m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedSurfaceArea()) {
    alveoliDiffusionArea_cm2 = alveoliDiffusionArea_cm2 - m_data.GetConditions().GetImpairedAlveolarExchange()->GetImpairedSurfaceArea(AreaUnit::cm2);
  } else if (m_data.GetConditions().GetImpairedAlveolarExchange()->HasImpairedFraction()) {
    alveoliDiffusionArea_cm2 = (1.0 - m_data.GetConditions().GetImpairedAlveolarExchange()->GetImpairedFraction().GetValue()) * alveoliDiffusionArea_cm2;
  }

  //This is a conditions, so we change it pertinently
  m_Patient->GetAlveoliSurfaceArea().SetValue(alveoliDiffusionArea_cm2, AreaUnit::cm2);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Lobar Pneumonia
///
/// \param  None
///
/// \return void
///
/// \details
/// This method handles the lobar pneumonia condition. It determines if the patient has lobar pneumonia and
/// calculates a set of multipliers to model the various symptoms. This method should only run once per simulation.
//--------------------------------------------------------------------------------------------------
void Respiratory::LobarPneumonia()
{
  // Check to see if we have Lobar Pneumonia
  if (m_data.GetConditions().HasLobarPneumonia()) {
    // Get Lobar Pneumonia Severity
    double dLobarPneumoniaSeverity = m_data.GetConditions().GetLobarPneumonia()->GetSeverity().GetValue();

    // Get lung fractions
    double dLeftLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetLeftLungAffected().GetValue();
    double dRightLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetRightLungAffected().GetValue();

    // Call UpdateAlveoliCompliance -- the interaction of severity and lung fraction is accounted for in this function
    UpdateAlveoliCompliance(dLobarPneumoniaSeverity, dLeftLungFraction, dRightLungFraction);

    // Call UpdateGasDiffusionSurfaceArea -- the interaction of severity and lung fraction is accounted for in this function
    UpdateGasDiffusionSurfaceArea(dLobarPneumoniaSeverity, dLeftLungFraction, dRightLungFraction);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Apply apnea action.
///
/// \param  None
///
/// \return void
///
/// \details
/// This reduces the respiratory driver pressure source amplitude by the percentage defined by the
/// action severity.
//--------------------------------------------------------------------------------------------------
void Respiratory::Apnea()
{
  if (m_PatientActions->HasApnea()) {
    double apneaSeverity = m_PatientActions->GetApnea()->GetSeverity().GetValue();
    m_hadApnea = true;
    if (1.0 - apneaSeverity < ZERO_APPROX) {
      m_bNotBreathing = true;
    }
    //Just reduce the tidal volume by the percentage given
    m_DriverPressure_cmH2O = m_DefaultDrivePressure_cmH2O + (m_DriverPressure_cmH2O - m_DefaultDrivePressure_cmH2O) * (1 - apneaSeverity);
  } else {
    if (m_hadApnea) {
      m_hadApnea = false;
      m_bNotBreathing = false;
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates key respiratory physiological parameters
///
/// \details
/// Calculates the respiration rate and tidal volume. For each breathing cycle, the tidal volume is calculated by identifying
/// the peaks in the total lung volume.
/// The tidal volume of a particular breathing cycle is then calculated by taking the difference between the
/// maximum and minimum values of the total lung volume.
/// The respiration rate is calculated by measuring the duration for one complete breathing cycle and then
/// converting the duration to the number of breaths per minute.
// The method also checks for bradypnea and tachypnea events and reports to the use.
//--------------------------------------------------------------------------------------------------
void Respiratory::CalculateVitalSigns()
{
  std::stringstream ss;
  //Total Respiratory Volume - this should not include the Pleural Space
  GetTotalLungVolume().Set(m_Lungs->GetVolume());

  //Record values each time-step
  double tracheaFlow_L_Per_s = m_MouthToTrachea->GetNextFlow().GetValue(VolumePerTimeUnit::L_Per_s);
  GetInspiratoryFlow().SetValue(tracheaFlow_L_Per_s, VolumePerTimeUnit::L_Per_s);
  GetExpiratoryFlow().SetValue(-tracheaFlow_L_Per_s, VolumePerTimeUnit::L_Per_s);

  double dEnvironmentPressure = m_Environment->GetPressure(PressureUnit::cmH2O);
  double dAlveolarPressure = (m_LeftAlveoli->GetNextPressure().GetValue(PressureUnit::cmH2O) + m_RightAlveoli->GetNextPressure().GetValue(PressureUnit::cmH2O)) / 2.0; //Average of L and R
  GetPulmonaryResistance().SetValue((dEnvironmentPressure - dAlveolarPressure) / tracheaFlow_L_Per_s, FlowResistanceUnit::cmH2O_s_Per_L);

  double dPleuralPressure_cmH2O = (m_LeftPleuralCavity->GetNextPressure().GetValue(PressureUnit::cmH2O) + m_RightPleuralCavity->GetNextPressure().GetValue(PressureUnit::cmH2O)) / 2.0; //Average of L and R
  GetTranspulmonaryPressure().SetValue(dAlveolarPressure - dPleuralPressure_cmH2O, PressureUnit::cmH2O);

  GetRespirationMusclePressure().SetValue(m_RespiratoryMuscle->GetNextPressure(PressureUnit::cmH2O) - 1033.23, PressureUnit::cmH2O);

  double avgAlveoliO2PP_mmHg = (m_LeftAlveoliO2->GetPartialPressure(PressureUnit::mmHg) + m_RightAlveoliO2->GetPartialPressure(PressureUnit::mmHg)) / 2.0;
  GetAlveolarArterialGradient().SetValue(avgAlveoliO2PP_mmHg - m_AortaO2->GetPartialPressure(PressureUnit::mmHg), PressureUnit::mmHg);

  /// \event Patient: Start of exhale/inhale
  if (m_Patient->IsEventActive(CDM::enumPatientEvent::StartOfExhale))
    m_Patient->SetEvent(CDM::enumPatientEvent::StartOfExhale, false, m_data.GetSimulationTime());
  if (m_Patient->IsEventActive(CDM::enumPatientEvent::StartOfInhale))
    m_Patient->SetEvent(CDM::enumPatientEvent::StartOfInhale, false, m_data.GetSimulationTime());

  //Record values at the breathing inflection points (i.e. switch between inhale and exhale)
  // Temporal tolerance to avoid accidental entry in the the inhalation and exhalation code blocks
  // dTimeTol = One fourth of the smallest possible period between breaths for a human. 60 breaths per
  // minute is a typical upper limit, so dTimeTol = 1 / (60 *4) = 0.004167 minutes.
  double dTimeTol = 0.004167;
  m_ElapsedBreathingCycleTime_min += m_dt_min;
 
  if (m_BreathingCycle && ((GetTotalLungVolume(VolumeUnit::L) - m_PreviousTotalLungVolume_L) > ZERO_APPROX)
      && (m_ElapsedBreathingCycleTime_min > dTimeTol)) {
    m_Patient->SetEvent(CDM::enumPatientEvent::StartOfInhale, true, m_data.GetSimulationTime());
    // Calculate Respiration Rate and track time and update cycle flag
    double RespirationRate_Per_min = 0.0;
    RespirationRate_Per_min = 1.0 / m_ElapsedBreathingCycleTime_min;
    if (m_data.GetActions().GetPatientActions().HasOverride()
        && m_data.GetActions().GetPatientActions().GetOverride()->HasRespirationRateOverride()
        && m_data.GetActions().GetPatientActions().GetOverride()->GetOverrideConformance() == CDM::enumOnOff::Off) {
      RespirationRate_Per_min = m_data.GetActions().GetPatientActions().GetOverride()->GetRespirationRateOverride(FrequencyUnit::Per_min);
    }
    GetRespirationRate().SetValue(RespirationRate_Per_min, FrequencyUnit::Per_min);

    double dExpirationTime = m_ElapsedBreathingCycleTime_min - m_BreathTimeExhale_min;
    double dInspirationTime = m_BreathTimeExhale_min;
    double dieratio = dInspirationTime / dExpirationTime;
    GetInspiratoryExpiratoryRatio().SetValue(dieratio);

    m_ElapsedBreathingCycleTime_min = 0.0;
    m_BreathingCycle = false;

    // Calculate the Tidal Volume from the last peak
    double dBottomBreathTotalVolume_L = GetTotalLungVolume(VolumeUnit::L);
    m_InstantaneousFunctionalResidualCapacity_L = dBottomBreathTotalVolume_L;
    double dBottomBreathAlveoliVolume_L = m_RightAlveoli->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftAlveoli->GetNextVolume().GetValue(VolumeUnit::L);
    double dBottomBreathDeadSpaceVolume_L = m_RightBronchi->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftBronchi->GetNextVolume().GetValue(VolumeUnit::L) + m_Trachea->GetVolume(VolumeUnit::L);
    double dBottomBreathPleuralPressure_cmH2O = dPleuralPressure_cmH2O;

    double TidalVolume_L = std::abs(m_TopBreathTotalVolume_L - dBottomBreathTotalVolume_L);
    double AlveoliDeltaVolume_L = std::abs(m_TopBreathAlveoliVolume_L - dBottomBreathAlveoliVolume_L);
    double DeadSpaceDeltaVolume_L = std::abs(m_TopBreathDeadSpaceVolume_L - dBottomBreathDeadSpaceVolume_L);
    double PleuralDeltaPressure_cmH2O = dBottomBreathPleuralPressure_cmH2O - m_TopBreathPleuralPressure_cmH2O;

    GetTidalVolume().SetValue(TidalVolume_L, VolumeUnit::L);

    // Set the End Tidal Concentration for our gases at the end of a respiration cycle
    for (SEGasSubstanceQuantity* subQ : m_Trachea->GetSubstanceQuantities()) {
      subQ->GetSubstance().GetEndTidalFraction().Set(subQ->GetVolumeFraction());
      subQ->GetSubstance().GetEndTidalPressure().Set(subQ->GetPartialPressure());
    }
    GetEndTidalCarbonDioxideFraction().Set(m_data.GetSubstances().GetCO2().GetEndTidalFraction());
    GetEndTidalCarbonDioxidePressure().Set(m_LeftAlveoliCO2->GetPartialPressure());

    // Calculate Ventilationss
    GetTotalAlveolarVentilation().SetValue(AlveoliDeltaVolume_L * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);
    GetTotalPulmonaryVentilation().SetValue(TidalVolume_L * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);
    GetTotalDeadSpaceVentilation().SetValue(DeadSpaceDeltaVolume_L * RespirationRate_Per_min, VolumePerTimeUnit::L_Per_min);
    GetSpecificVentilation().SetValue(TidalVolume_L / m_InstantaneousFunctionalResidualCapacity_L);

    // Calculate Total Circuit Values
    GetPulmonaryCompliance().SetValue(TidalVolume_L / PleuralDeltaPressure_cmH2O, FlowComplianceUnit::L_Per_cmH2O);
  } else if (!m_BreathingCycle
             && (m_PreviousTotalLungVolume_L - GetTotalLungVolume(VolumeUnit::L) > ZERO_APPROX)
             && (m_ElapsedBreathingCycleTime_min > dTimeTol)) {
    m_Patient->SetEvent(CDM::enumPatientEvent::StartOfExhale, true, m_data.GetSimulationTime());
    m_BreathTimeExhale_min = m_ElapsedBreathingCycleTime_min;
    m_BreathingCycle = true;
    m_TopBreathTotalVolume_L = GetTotalLungVolume(VolumeUnit::L);
    m_TopBreathAlveoliVolume_L = m_RightAlveoli->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftAlveoli->GetNextVolume().GetValue(VolumeUnit::L);
    m_TopBreathDeadSpaceVolume_L = m_RightBronchi->GetNextVolume().GetValue(VolumeUnit::L) + m_LeftBronchi->GetNextVolume().GetValue(VolumeUnit::L) + m_Trachea->GetVolume(VolumeUnit::L);
    m_TopBreathPleuralPressure_cmH2O = dPleuralPressure_cmH2O;

    //We want the peak Trachea O2 value - this should be the inspired value
    GetCarricoIndex().SetValue(m_ArterialO2PartialPressure_mmHg / m_TracheaO2->GetVolumeFraction().GetValue(), PressureUnit::mmHg);

    if (m_data.GetState() > EngineState::InitialStabilization) { // Don't throw events if we are initializing
      //Check for ARDS - 3 different levels
      if (GetCarricoIndex().GetValue(PressureUnit::mmHg) < 100.0) {
        /// \event Patient: Severe ARDS: Carrico Index is below 100 mmHg
        m_Patient->SetEvent(CDM::enumPatientEvent::SevereAcuteRespiratoryDistress, true, m_data.GetSimulationTime()); /// \cite ranieriacute
        m_Patient->SetEvent(CDM::enumPatientEvent::ModerateAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
        m_Patient->SetEvent(CDM::enumPatientEvent::MildAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
      } else if (GetCarricoIndex().GetValue(PressureUnit::mmHg) < 200.0) {
        /// \event Patient: Moderate ARDS: Carrico Index is below 200 mmHg
        m_Patient->SetEvent(CDM::enumPatientEvent::ModerateAcuteRespiratoryDistress, true, m_data.GetSimulationTime()); /// \cite ranieriacute
        m_Patient->SetEvent(CDM::enumPatientEvent::SevereAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
        m_Patient->SetEvent(CDM::enumPatientEvent::MildAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
      } else if (GetCarricoIndex().GetValue(PressureUnit::mmHg) < 300.0) {
        /// \event Patient: Mild ARDS: Carrico Index is below 300 mmHg
        m_Patient->SetEvent(CDM::enumPatientEvent::MildAcuteRespiratoryDistress, true, m_data.GetSimulationTime()); /// \cite ranieriacute
        m_Patient->SetEvent(CDM::enumPatientEvent::SevereAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
        m_Patient->SetEvent(CDM::enumPatientEvent::ModerateAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
      } else {
        /// \event Patient: End ARDS: Carrico Index is above 305 mmHg
        m_Patient->SetEvent(CDM::enumPatientEvent::SevereAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
        m_Patient->SetEvent(CDM::enumPatientEvent::ModerateAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
        m_Patient->SetEvent(CDM::enumPatientEvent::MildAcuteRespiratoryDistress, false, m_data.GetSimulationTime());
      }
    }
  }

  //Zero out if waiting longer than 30 s -- this means that our lowest supported respiration rate is ~1.3/min
  if (m_ElapsedBreathingCycleTime_min > 0.75) {
    GetRespirationRate().SetValue(0.0, FrequencyUnit::Per_min);
    GetTotalAlveolarVentilation().SetValue(0.0, VolumePerTimeUnit::L_Per_min);
    GetTotalPulmonaryVentilation().SetValue(0.0, VolumePerTimeUnit::L_Per_min);
  }

  /// \todo Move to blood chemistry
  // Although it is called respiratory acidosis/alkalosis, the detection and event setting is actually a part of the @ref BloodChemistrySystem
  // The terms "metabolic" and "respiratory" refer to the origin of the acid-base disturbance
  // The hydrogen ion concentration is a property of the blood
  // The events related to blood concentrations should be detected and set in blood chemistry.
  //Keep a running average of the pH
  m_BloodPHRunningAverage.Sample(m_data.GetBloodChemistry().GetArterialBloodPH().GetValue());
  //Reset at start of cardiac cycle
  if (m_Patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
    m_LastCardiacCycleBloodPH = m_BloodPHRunningAverage.Value();
    m_BloodPHRunningAverage.Reset();
  }

  if (m_data.GetState() > EngineState::InitialStabilization) { // Don't throw events if we are initializing

    //Bradypnea
    if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) < 8) {
      /// \event Patient: Bradypnea: Respiration rate is below 10 breaths per minute
      /// The patient has bradypnea.
      m_Patient->SetEvent(CDM::enumPatientEvent::Bradypnea, true, m_data.GetSimulationTime()); /// \cite overdyk2007continuous
    } else if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) >= 10) // offset by 2
    {
      /// \event Patient: End Bradypnea Event. The respiration rate has risen above 10.
      /// The patient is no longer considered to have bradypnea.
      m_Patient->SetEvent(CDM::enumPatientEvent::Bradypnea, false, m_data.GetSimulationTime());
    }

    //Tachypnea
    if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) > 20) {
      /// \event Patient: Tachypnea: Respiration rate is above 20 breaths per minute.
      /// The patient has tachypnea.
      m_Patient->SetEvent(CDM::enumPatientEvent::Tachypnea, true, m_data.GetSimulationTime()); /// \cite
    } else if (GetRespirationRate().GetValue(FrequencyUnit::Per_min) <= 18) // offset by 2 // && m_Patient->GetEventDuration(CDM::enumPatientEvent::Tachypnea, TimeUnit::s) > 5 for time based segmentation
    {
      /// \event Patient: End Tachypnea Event. The respiration rate has fallen below 19.5.
      /// The patient is no longer considered to have tachypnea.
      m_Patient->SetEvent(CDM::enumPatientEvent::Tachypnea, false, m_data.GetSimulationTime());
    }

    double highPh = 8.5;
    double lowPh = 6.5; // \cite Edge2006AcidosisConscious
    //// Respiratory Acidosis
    if (m_LastCardiacCycleBloodPH < 7.35 && m_ArterialCO2PartialPressure_mmHg > 47.0) {
      /// \event Patient: Respiratory Acidosis: event is triggered when blood pH is below 7.36
      /// The patient has respiratory acidosis.
      m_Patient->SetEvent(CDM::enumPatientEvent::RespiratoryAcidosis, true, m_data.GetSimulationTime());

      /// \event Patient: arterial blood ph has dropped below 6.5.
      if (m_LastCardiacCycleBloodPH < lowPh) {
        ss << "Arterial blood pH is  " << m_LastCardiacCycleBloodPH << ". This is below 6.5, Patient is experiencing extreme respiratory Acidosis and is in an irreversible state.";
        Warning(ss);
        /// \irreversible Extreme respiratory Acidosis: blood pH below 6.5.
        if (!m_PatientActions->HasOverride()) {
          m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        } else {
          if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
            m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
          }
        }
      }
    } else if (m_LastCardiacCycleBloodPH >= 7.38 && m_ArterialCO2PartialPressure_mmHg < 44.0) {
      /// \event Patient: End Respiratory Acidosis Event. The pH value has risen above 7.38.
      /// The patient is no longer considered to have respiratory acidosis.
      m_Patient->SetEvent(CDM::enumPatientEvent::RespiratoryAcidosis, false, m_data.GetSimulationTime());
    }

    ////Respiratory Alkalosis
    if (m_LastCardiacCycleBloodPH > 7.45 && m_ArterialCO2PartialPressure_mmHg < 37.0) {
      /// \event Patient: Respiratory Alkalosis: event is triggered when blood pH is above 7.45
      /// The patient has respiratory alkalosis.
      m_Patient->SetEvent(CDM::enumPatientEvent::RespiratoryAlkalosis, true, m_data.GetSimulationTime());

      /// \event Patient: arterial blood ph has gotten above 8.5.
      if (m_LastCardiacCycleBloodPH > highPh) {
        ss << "Arterial blood pH is  " << m_LastCardiacCycleBloodPH << ". This is above 8.5, Patient is experiencing extreme respiratory Alkalosis and is in an irreversible state.";
        Warning(ss);
        /// \irreversible Extreme respiratory Alkalosis: blood pH above 8.5.
        if (!m_PatientActions->HasOverride()) {
          m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        } else {
          if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
            m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
          }
        }
      }
    } else if (m_LastCardiacCycleBloodPH <= 7.43 && m_ArterialCO2PartialPressure_mmHg > 39.0) {
      /// \event Patient: End Respiratory Alkalosis Event. The pH value has has fallen below 7.45.
      /// The patient is no longer considered to have respiratory alkalosis.
      m_Patient->SetEvent(CDM::enumPatientEvent::RespiratoryAlkalosis, false, m_data.GetSimulationTime());
    }
  }

  m_PreviousTotalLungVolume_L = GetTotalLungVolume(VolumeUnit::L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update obstructive (airway) resistance
///
/// \return void
///
/// \details
/// This method takes a resistance scaling factor and lung percentages (left and right) as input
/// variables.  It updates the Trachea to Dead Space path resistances in order to model airflow
/// blockage through the bronchi and bronchioles.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateObstructiveResistance()
{
  if ((!m_PatientActions->HasAsthmaAttack() && !m_data.GetConditions().HasChronicObstructivePulmonaryDisease()) || GetExpiratoryFlow(VolumePerTimeUnit::L_Per_s) < 0.0)
  {
    return;
  }
  double combinedResistanceScalingFactor = 1.0;
  //Asthma attack on
  if (m_PatientActions->HasAsthmaAttack()) {
    double dSeverity = m_PatientActions->GetAsthmaAttack()->GetSeverity().GetValue();
    // Resistance function: Base = 10, Min = 10, Max = 5000 (increasing with severity)
    double dResistanceScalingFactor = GeneralMath::ResistanceFunction(10.0, 5000, 10.0, dSeverity);
    combinedResistanceScalingFactor = dResistanceScalingFactor;
  }
  //COPD on
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease()) {
    double dSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetBronchitisSeverity().GetValue();
    // Resistance function: Base = 10, Min = 10, Max = 500 (increasing with severity)
    double dResistanceScalingFactor = GeneralMath::ResistanceFunction(10.0, 500.0, 10.0, dSeverity);
    combinedResistanceScalingFactor = std::max(combinedResistanceScalingFactor, dResistanceScalingFactor);
  }

  // Get the path resistances
  double dLeftBronchiResistance = m_TracheaToLeftBronchi->GetNextResistance().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
  double dRightBronchiResistance = m_TracheaToRightBronchi->GetNextResistance().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);

  dLeftBronchiResistance *= combinedResistanceScalingFactor;
  m_TracheaToLeftBronchi->GetNextResistance().SetValue(dLeftBronchiResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  dRightBronchiResistance *= combinedResistanceScalingFactor;
  m_TracheaToRightBronchi->GetNextResistance().SetValue(dRightBronchiResistance, FlowResistanceUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update the inspiratory-expiratory ratio
///
/// \return void
///
/// \details
/// The inspiratory-expiratory ratio is modified based on certain actions and conditions, as well as
/// local bronchodilation effects from aerosols.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateIERatio()
{
  // Asthma, Chronic Bronchitis, and Lobar Pneumonia
  //  Adjust the inspiration/expiration ratio based on severity
  double combinedSeverity = 0.0;
  m_IEscaleFactor = 1.0;
  if (m_PatientActions->HasAsthmaAttack()) {
    combinedSeverity = m_PatientActions->GetAsthmaAttack()->GetSeverity().GetValue();
  }
  if (m_data.GetConditions().HasChronicObstructivePulmonaryDisease()) {
    double dBronchitisSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetBronchitisSeverity().GetValue();
    double dEmphysemaSeverity = m_data.GetConditions().GetChronicObstructivePulmonaryDisease()->GetEmphysemaSeverity().GetValue();
    combinedSeverity = std::max(combinedSeverity, dEmphysemaSeverity);
    combinedSeverity = std::max(combinedSeverity, dBronchitisSeverity);
  }
  if (m_data.GetConditions().HasLobarPneumonia()) {
    double severity = m_data.GetConditions().GetLobarPneumonia()->GetSeverity().GetValue();
    // Get lung fractions
    double dRightLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetRightLungAffected().GetValue();
    double dLeftLungFraction = m_data.GetConditions().GetLobarPneumonia()->GetLeftLungAffected().GetValue();

    // Get the right and left lung ratios
    double dRightLungRatio = m_Patient->GetRightLungRatio().GetValue();
    double dLeftLungRatio = 1.0 - dRightLungRatio;

    double dLP_ScaledSeverity = (0.75 * severity) + (severity * dLeftLungFraction * dLeftLungRatio) + (severity * dRightLungFraction * dRightLungRatio);
    combinedSeverity = std::max(combinedSeverity, dLP_ScaledSeverity);
  }

  if (combinedSeverity > 0.0) {
    //The respiratory driver is very sensitive to the IEScaleFactor.  It does not take to decrease output IE Ratio significanlty.  Found that
    //bounding at 0.85 worked well.
    m_IEscaleFactor = 1.0 - 0.15 * combinedSeverity;
    //When albuterol is administered, the bronchodilation also causes the IE ratio to correct itself
    m_IEscaleFactor *= exp(7728.4 * m_AverageLocalTissueBronchodilationEffects);

    // IE scale factor is constrained to a minimum of 0.1 and a maximum 1.0. Lower than 0.1 causes simulation instability.
    // Greater than 1.0 is not possible for patients with these conditions
    m_IEscaleFactor = LIMIT(m_IEscaleFactor, 0.1, 1.0);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update Alveoli Compliance
///
/// \param  dScalingFactor      Multiplier based on the severity of the condition (lobar pneumonia, COPD, etc)
/// \param  dLeftLungFraction     Fraction of left lung affected by change in surface area (0 to 1)
/// \param  dRightLungFraction      Fraction of right lung affected by change in surface area (0 to 1)
///
/// \return void
///
/// \details
/// This method takes a scaling factor and lung percentages (left and right) as input variables
/// The scaling factor and lung percentages are combined to produce a measure of deviation from compliance baseline.
/// The Alveoli to Pleural compliances are updated to model alveolus membrane damage orchanges to alveolus fluid content
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateAlveoliCompliance(double dScalingFactor, double dLeftLungFraction, double dRightLungFraction)
{
  //Testing shows that patient will die of extreme hypercapnia in most cases when the sum of the lung fractions and scaling factor is greater
  //than 2.35.  While not a perfect cut off (i.e. severity = 1.0, Left = Right = 0.675 does not produce exact same result as severity = 0.35,
  //Left = Right = 0.35), it allows for extreme scenarios (RR > 35, PaO2 < 40) without crashing engine.
  if (dScalingFactor + dLeftLungFraction + dRightLungFraction > 2.35) {
    dScalingFactor = 1.0;
    dLeftLungFraction = 0.675;
    dRightLungFraction = 0.657;
  }
  // Get path compliances
  double dRightAlveoliBaselineCompliance = m_RightAlveoliToRightPleuralConnection->GetComplianceBaseline().GetValue(FlowComplianceUnit::L_Per_cmH2O);
  double dLeftAlveoliBaselineCompliance = m_LeftAlveoliToLeftPleuralConnection->GetComplianceBaseline().GetValue(FlowComplianceUnit::L_Per_cmH2O);

  // Left lung alveoli
  double dLeftScalingFactor = GeneralMath::ResistanceFunction(10, 1.000, 0.005, 1.0 - dScalingFactor * dLeftLungFraction);
  dLeftAlveoliBaselineCompliance *= dLeftScalingFactor;
  m_LeftAlveoliToLeftPleuralConnection->GetComplianceBaseline().SetValue(dLeftAlveoliBaselineCompliance, FlowComplianceUnit::L_Per_cmH2O);

  // Right lung alveoli
  double dRightScalingFactor = GeneralMath::ResistanceFunction(10, 1.000, 0.005, 1.0 - dScalingFactor * dRightLungFraction);
  dRightAlveoliBaselineCompliance *= dRightScalingFactor;
  m_RightAlveoliToRightPleuralConnection->GetComplianceBaseline().SetValue(dRightAlveoliBaselineCompliance, FlowComplianceUnit::L_Per_cmH2O);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update Pulmonary Capillary Resistance
///
/// \param  dResistanceScalingFactor  Pulmonary capillary resistance multiplier
/// \param  dLeftLungFraction     Fraction of left lung affected by change in surface area (0 to 1)
/// \param  dRightLungFraction      Fraction of right lung affected by change in surface area (0 to 1)
///
/// \return void
///
/// \details
/// This method takes a resistance scaling factor and lung fractions (left and right) as input
/// variables.  It updates the pulmonary capillary to pulmonary vein resistance in order to model the
/// destruction of capillaries in the alveolus membrane.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdatePulmonaryCapillaryResistance(double dResistanceScalingFactor, double dLeftLungPercent, double dRightLungPercent)
{
  // Get path resistances
  double dRightPulmonaryCapillaryResistance = m_RightPulmonaryCapillary->GetResistanceBaseline().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);
  double dLeftPulmonaryCapillaryResistance = m_LeftPulmonaryCapillary->GetResistanceBaseline().GetValue(FlowResistanceUnit::cmH2O_s_Per_L);

  dLeftPulmonaryCapillaryResistance = (dLeftPulmonaryCapillaryResistance * (1.0 - dLeftLungPercent)) + (dLeftPulmonaryCapillaryResistance * dResistanceScalingFactor * dLeftLungPercent);
  m_LeftPulmonaryCapillary->GetResistanceBaseline().SetValue(dLeftPulmonaryCapillaryResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  dRightPulmonaryCapillaryResistance = (dRightPulmonaryCapillaryResistance * (1.0 - dRightLungPercent)) + (dRightPulmonaryCapillaryResistance * dResistanceScalingFactor * dRightLungPercent);
  m_RightPulmonaryCapillary->GetResistanceBaseline().SetValue(dRightPulmonaryCapillaryResistance, FlowResistanceUnit::cmH2O_s_Per_L);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Update Gas Diffusion Surface Area
///
/// \param  dFractionArea   Fractional change in gas diffusion surface area (0 to 1)
/// \param  dLeftLungFraction Fraction of left lung affected by change in surface area (0 to 1)
/// \param  dRightLungFraction  Fraction of right lung affected by change in surface area (0 to 1)
///
/// \return void
///
/// \details
/// This method takes a percent valve and lung percentages (left and right) as input
/// variables.  It updates the gas diffusion surface area in the lungs in order to model the
/// destruction of alveoli membranes and/or lung consolidation.
//--------------------------------------------------------------------------------------------------
void Respiratory::UpdateGasDiffusionSurfaceArea(double dFractionArea, double dLeftLungFraction, double dRightLungFraction)
{
  double AlveoliDiffusionArea_cm2 = m_Patient->GetAlveoliSurfaceArea(AreaUnit::cm2);

  // Get the right and left lung ratios
  double RightLungRatio = m_Patient->GetRightLungRatio().GetValue();
  double LeftLungRatio = 1.0 - RightLungRatio;

  double dLeftScaleFactor = GeneralMath::ResistanceFunction(10, 0.15, 1.0, dFractionArea * dLeftLungFraction);
  double dRightScaleFactor = GeneralMath::ResistanceFunction(10, 0.15, 1.0, dFractionArea * dRightLungFraction);

  // Calculate the total surface area
  AlveoliDiffusionArea_cm2 = dLeftScaleFactor * LeftLungRatio * AlveoliDiffusionArea_cm2 + dRightScaleFactor * RightLungRatio * AlveoliDiffusionArea_cm2;

  m_Patient->GetAlveoliSurfaceArea().SetValue(AlveoliDiffusionArea_cm2, AreaUnit::cm2);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Populate the Pulmonary Function Test Assessment
///
/// \param  pft       pft data
///
/// \return success       true, if everything worked out
//--------------------------------------------------------------------------------------------------
bool Respiratory::CalculatePulmonaryFunctionTest(SEPulmonaryFunctionTest& pft)
{
  pft.Reset();
  pft.GetExpiratoryReserveVolume().Set(m_Patient->GetExpiratoryReserveVolume());
  pft.GetFunctionalResidualCapacity().Set(m_Patient->GetFunctionalResidualCapacity());
  pft.GetInspiratoryCapacity().Set(m_Patient->GetInspiratoryCapacity());
  pft.GetInspiratoryReserveVolume().Set(m_Patient->GetInspiratoryReserveVolume());
  pft.GetResidualVolume().Set(m_Patient->GetResidualVolume());
  pft.GetTotalLungCapacity().Set(m_Patient->GetTotalLungCapacity());
  pft.GetVitalCapacity().Set(m_Patient->GetVitalCapacity());

  double rr_Hz = GetRespirationRate(FrequencyUnit::Hz);
  double tv_L = GetTidalVolume(VolumeUnit::L);
  double waveRespirationRate = rr_Hz;
  double pi = 3.14159265359;
  double magnitude = 0.5 * tv_L;
  double superPosition = m_Patient->GetFunctionalResidualCapacity(VolumeUnit::L) + magnitude;
  double waveTime = 0.0;
  double currentTime = 0.0;

  double numPlotPoint = pft.GetNumberOfPlotPoints();
  SEFunctionVolumeVsTime& plot = pft.GetLungVolumePlot();
  plot.Invalidate();
  std::vector<double>& time = plot.GetTime();
  std::vector<double>& volume = plot.GetVolume();
  plot.SetTimeUnit(TimeUnit::s);
  plot.SetVolumeUnit(VolumeUnit::L);
  if (numPlotPoint > 0) {
    double dt = 60.0 / numPlotPoint;
    for (unsigned int i = 0; i < numPlotPoint; i++) {
      time.push_back(currentTime);
      if (waveTime >= 12.0 && waveTime < 13.0) { //Period dilation due to forced inspiration
        magnitude = 0.5 * tv_L + pft.GetInspiratoryReserveVolume().GetValue(VolumeUnit::L);
        waveRespirationRate = rr_Hz
          / (1 + pft.GetExpiratoryReserveVolume().GetValue(VolumeUnit::L) / tv_L);
      } else if (waveTime >= 13.0 && waveTime < 14.0) { //Period dilation due to forced expiration
        magnitude = 0.5 * tv_L + pft.GetExpiratoryReserveVolume().GetValue(VolumeUnit::L);
        waveRespirationRate = rr_Hz
          / (1 + pft.GetExpiratoryReserveVolume().GetValue(VolumeUnit::L) / tv_L);
      } else {
        magnitude = 0.5 * tv_L;
        waveRespirationRate = rr_Hz;
      }

      volume.push_back(magnitude * sin(pi * waveTime) + superPosition);
      waveTime += 2.0 * waveRespirationRate * dt;
      currentTime += dt;
    }
  }

  /// \todo Insert more PFT calculations here
  /*
  All Known:
  VitalCapacity=TotaLungCapacity-ResidualVolume
  InspiratoryCapacity=TotaLungCapacity-FunctionalResidualCapacity
  InspiratoryReserveVolume=InspiratoryCapacity-TidalVolume
  ExpiratoryReserveVolume=FunctionalResidualCapacity-ResidualVolume

  Need to define:
  ForcedVitalCapacity       | Increase the pressure driver in the lungs
  ForcedExpiratoryVolume      | for forced PFT constituents?
  ForcedExpiratoryFlow      |
  PeakExpiratoryFlow        |
  MaximumVoluntaryVentilation   | The same pressure driver increase should be used here. Calculate as a time integration of lung volume over 1 min.

  SlowVitalCapacity       |

  */
  return true;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Run the circuit standalone for a little to get it into a good starting state
/// - keep volume fractions constant
//--------------------------------------------------------------------------------------------------
void Respiratory::TuneCircuit()
{
  DataTrack circuitTrk;
  std::ofstream circuitFile;
  SEFluidCircuit& RespiratoryCircuit = m_data.GetCircuits().GetRespiratoryCircuit();
  double time_s = 0.0;
  double timeInCycle_s = 0.0;
  double cycleTime_s = 5.0; //12 /min
  double nextDriverPressure_cmH2O;
  double driveAmplitude_cmH2O = -5.0;
  double ieRatio = 0.6;
  double expTime_s = cycleTime_s / (1.0 + ieRatio);
  double inTime_s = expTime_s * ieRatio;
  double tau_s = expTime_s / 5.0;
  //This is precharge everything to help stabilization
  for (unsigned int loops = 0; loops < static_cast<unsigned int>(60.0 / m_dt_s); loops++) {
    if (timeInCycle_s < inTime_s) {
      nextDriverPressure_cmH2O = m_DefaultDrivePressure_cmH2O + (-driveAmplitude_cmH2O / (inTime_s * expTime_s) * std::pow(timeInCycle_s, 2.0) + driveAmplitude_cmH2O * cycleTime_s / (inTime_s * expTime_s) * timeInCycle_s);
    } else {
      nextDriverPressure_cmH2O = m_DefaultDrivePressure_cmH2O + driveAmplitude_cmH2O / (1.0 - std::exp(-expTime_s / tau_s)) * (std::exp(-(timeInCycle_s - inTime_s) / tau_s) - std::exp(-expTime_s / tau_s));
    }
    m_DriverPressurePath->GetNextPressureSource().SetValue(nextDriverPressure_cmH2O, PressureUnit::cmH2O);
    m_Calculator.Process(RespiratoryCircuit, m_dt_s);
    m_Calculator.PostProcess(RespiratoryCircuit);
    if (!m_TuningFile.empty()) {
      circuitTrk.Track(time_s, RespiratoryCircuit);
      if (time_s == 0)
        circuitTrk.CreateFile(m_TuningFile.c_str(), circuitFile);
      circuitTrk.StreamTrackToFile(circuitFile);
    }
    time_s += m_dt_s;
    timeInCycle_s += m_dt_s;
    if (timeInCycle_s >= cycleTime_s) {
      timeInCycle_s = 0.0;
    }
  }

  //Make sure the new volumes are accounted for with all the substance stuff
  //Keep the same volume fraction originally initialized
  SEGasCompartmentGraph* RespiratoryGraph = &m_data.GetCompartments().GetRespiratoryGraph();
  for (SEGasCompartment* compartment : RespiratoryGraph->GetCompartments()) {
    if (compartment->HasVolume())
      compartment->Balance(BalanceGasBy::VolumeFraction);
  }

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// determine override requirements from user defined inputs
///
/// \details
/// User specified override outputs that are specific to the cardiovascular system are implemented here.
/// If overrides aren't present for this system then this function will not be called during preprocess.
//--------------------------------------------------------------------------------------------------
void Respiratory::ProcessOverride()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();

#ifdef BIOGEARS_USE_OVERRIDE_CONTROL
  OverrideControlLoop();
#endif

  if (override->HasExpiratoryFlowOverride()) {
    GetExpiratoryFlow().SetValue(override->GetExpiratoryFlowOverride(VolumePerTimeUnit::L_Per_min), VolumePerTimeUnit::L_Per_min);
  }
  if (override->HasInspiratoryFlowOverride()) {
    GetInspiratoryFlow().SetValue(override->GetInspiratoryFlowOverride(VolumePerTimeUnit::L_Per_min), VolumePerTimeUnit::L_Per_min);
  }
  if (override->HasPulmonaryComplianceOverride()) {
    GetPulmonaryCompliance().SetValue(override->GetPulmonaryComplianceOverride(FlowComplianceUnit::L_Per_cmH2O), FlowComplianceUnit::L_Per_cmH2O);
  }
  if (override->HasPulmonaryResistanceOverride()) {
    GetPulmonaryResistance().SetValue(override->GetPulmonaryResistanceOverride(FlowResistanceUnit::cmH2O_s_Per_L), FlowResistanceUnit::cmH2O_s_Per_L);
  }
  if (override->HasRespirationRateOverride()) {
    if (override->GetOverrideConformance() == CDM::enumOnOff::Off) {
      GetRespirationRate().SetValue(override->GetRespirationRateOverride(FrequencyUnit::Per_min), FrequencyUnit::Per_min);
    }
  }
  if (override->HasTidalVolumeOverride()) {
    GetTidalVolume().SetValue(override->GetTidalVolumeOverride(VolumeUnit::mL), VolumeUnit::mL);
  }
  if (override->HasTargetPulmonaryVentilationOverride()) {
    GetTargetPulmonaryVentilation().SetValue(override->GetTargetPulmonaryVentilationOverride(VolumePerTimeUnit::L_Per_min), VolumePerTimeUnit::L_Per_min);
  }
  if (override->HasTotalAlveolarVentilationOverride()) {
    GetTotalAlveolarVentilation().SetValue(override->GetTotalAlveolarVentilationOverride(VolumePerTimeUnit::L_Per_min), VolumePerTimeUnit::L_Per_min);
  }
  if (override->HasTotalLungVolumeOverride()) {
    GetTotalLungVolume().SetValue(override->GetTotalLungVolumeOverride(VolumeUnit::L), VolumeUnit::L);
  }
  if (override->HasTotalPulmonaryVentilationOverride()) {
    GetTotalPulmonaryVentilation().SetValue(override->GetTotalPulmonaryVentilationOverride(VolumePerTimeUnit::L_Per_min), VolumePerTimeUnit::L_Per_min);
  }
  //m_Patient->GetRespirationRateBaseline().SetValue(rr_per_min, FrequencyUnit::Per_min);
}

//// Can be turned on or off (for debugging purposes) using the Biogears_USE_OVERRIDE_CONTROL external in CMake
void Respiratory::OverrideControlLoop()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();
  constexpr double maxExpiratoryFlowOverride = 1000.0; // L/min
  constexpr double minExpiratoryFlowOverride = 0.0; // L/min
  constexpr double maxInspiratoryFlowOverride = 1000.0; // L/min
  constexpr double minInspiratoryFlowOverride = 0.0; // L/min
  constexpr double maxPulmonaryComplianceOverride = 1000.0; // L_Per_cmH2O
  constexpr double minPulmonaryComplianceOverride = 0.0; // L_Per_cmH2O
  constexpr double maxPulmonaryResistanceOverride = 1000.0; // cmH2O_s_Per_L
  constexpr double minPulmonaryResistanceOverride = 0.0; // cmH2O_s_Per_L
  constexpr double maxRROverride = 60.0; //respiration rate in breaths per min
  constexpr double minRROverride = 0.0; //respiration rate in breaths per min
  constexpr double maxTVOverride = 10000.0; //Tidal volume in mL
  constexpr double minTVOverride = 0.0; //Tidal volume in mL
  constexpr double maxTargetPulmonaryVentilationOverride = 1000.0; // L/min
  constexpr double minTargetPulmonaryVentilationOverride = 0.0; // L/min
  constexpr double maxTotalAlveolarVentilationOverride = 1000.0; // L/min
  constexpr double minTotalAlveolarVentilationOverride = 0.0; // L/min
  constexpr double maxTotalLungVolumeOverride = 500.0; // L
  constexpr double minTotalLungVolumeOverride = 0.0; // L
  constexpr double maxTotalPulmonaryVentilationOverride = 1000.0; // L/min
  constexpr double minTotalPulmonaryVentilationOverride = 0.0; // L/min

  double currentExpiratoryFlowOverride = 0.0; // value gets changed in next check
  double currentInspiratoryFlowOverride = 0.0; // value gets changed in next check
  double currentPulmonaryComplianceOverride = 0.0; // value gets changed in next check
  double currentPulmonaryResistanceOverride = 0.0; // value gets changed in next check
  double currentRROverride = 12.0; //Average RR, value gets changed in next check
  double currentTVOverride = m_data.GetRespiratory().GetTidalVolume().GetValue(VolumeUnit::mL); //Current Tidal Volume, value gets changed in next check
  double currentTargetPulmonaryVentilationOverride = 0.0; // value gets changed in next check
  double currentTotalAlveolarVentilationOverride = 0.0; // value gets changed in next check
  double currentTotalLungVolumeOverride = 0.0; // value gets changed in next check
  double currentTotalPulmonaryVentilationOverride = 0.0; // value gets changed in next check

  if (override->HasExpiratoryFlowOverride()) {
    currentExpiratoryFlowOverride = override->GetExpiratoryFlowOverride(VolumePerTimeUnit::L_Per_min);
  }
  if (override->HasInspiratoryFlowOverride()) {
    currentInspiratoryFlowOverride = override->GetInspiratoryFlowOverride(VolumePerTimeUnit::L_Per_min);
  }
  if (override->HasPulmonaryComplianceOverride()) {
    currentPulmonaryComplianceOverride = override->GetPulmonaryComplianceOverride(FlowComplianceUnit::L_Per_cmH2O);
  }
  if (override->HasPulmonaryResistanceOverride()) {
    currentPulmonaryResistanceOverride = override->GetPulmonaryResistanceOverride(FlowResistanceUnit::cmH2O_s_Per_L);
  }
  if (override->HasRespirationRateOverride()) {
    currentRROverride = override->GetRespirationRateOverride(FrequencyUnit::Per_min);
  }
  if (override->HasTidalVolumeOverride()) {
    currentTVOverride = override->GetTidalVolumeOverride(VolumeUnit::mL);
  }
  if (override->HasTargetPulmonaryVentilationOverride()) {
    currentTargetPulmonaryVentilationOverride = override->GetTargetPulmonaryVentilationOverride(VolumePerTimeUnit::L_Per_min);
  }
  if (override->HasTotalAlveolarVentilationOverride()) {
    currentTotalAlveolarVentilationOverride = override->GetTotalAlveolarVentilationOverride(VolumePerTimeUnit::L_Per_min);
  }
  if (override->HasTotalLungVolumeOverride()) {
    currentTotalLungVolumeOverride = override->GetTotalLungVolumeOverride(VolumeUnit::L);
  }
  if (override->HasTotalPulmonaryVentilationOverride()) {
    currentTotalPulmonaryVentilationOverride = override->GetTotalPulmonaryVentilationOverride(VolumePerTimeUnit::L_Per_min);
  }

  if ((currentExpiratoryFlowOverride < minExpiratoryFlowOverride || currentExpiratoryFlowOverride > maxExpiratoryFlowOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Expiratory Flow Override (Respiratory) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentInspiratoryFlowOverride < minInspiratoryFlowOverride || currentInspiratoryFlowOverride > maxInspiratoryFlowOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Inspiratory Flow Override (Respiratory) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentPulmonaryComplianceOverride < minPulmonaryComplianceOverride || currentPulmonaryComplianceOverride > maxPulmonaryComplianceOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Pulmonary Compliance Override (Respiratory) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentPulmonaryResistanceOverride < minPulmonaryResistanceOverride || currentPulmonaryResistanceOverride > maxPulmonaryResistanceOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Pulmonary Resistance Override (Respiratory) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentRROverride < minRROverride || currentRROverride > maxRROverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Respiration Rate Override (Respiratory) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentTVOverride < minTVOverride || currentTVOverride > maxTVOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Tidal Volume (Respiratory) Override set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentTargetPulmonaryVentilationOverride < minTargetPulmonaryVentilationOverride || currentTargetPulmonaryVentilationOverride > maxTargetPulmonaryVentilationOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Target Pulmonary Ventilation Override (Respiratory) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentTotalAlveolarVentilationOverride < minTotalAlveolarVentilationOverride || currentTotalAlveolarVentilationOverride > maxTotalAlveolarVentilationOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Total Alveolar Ventilation Override (Respiratory) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentTotalLungVolumeOverride < minTotalLungVolumeOverride || currentTotalLungVolumeOverride > maxTotalLungVolumeOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Total Lung Volume Override (Respiratory) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentTotalPulmonaryVentilationOverride < minTotalPulmonaryVentilationOverride || currentTotalPulmonaryVentilationOverride > maxTotalPulmonaryVentilationOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Total Pulmonary Ventilation Override (Respiratory) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  return;
}
}