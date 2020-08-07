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
#include <biogears/engine/Systems/Energy.h>

#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarEnergy.h>
#include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>
#include <biogears/cdm/properties/SEScalarEnergyPerMass.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>
#include <biogears/cdm/properties/SEScalarHeatConductance.h>
#include <biogears/cdm/properties/SEScalarHeatResistance.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>
#include <biogears/cdm/utils/GeneralMath.h>
#include <biogears/schema/cdm/Properties.hxx>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
auto Energy::make_unique(BioGears& bg) -> std::unique_ptr<Energy>
{
  return std::unique_ptr<Energy>(new Energy(bg));
}

Energy::Energy(BioGears& bg)
  : SEEnergySystem(bg.GetLogger())
  , m_data(bg)
  , m_circuitCalculator(GetLogger())
{
  Clear();
}

Energy::~Energy()
{
  Clear();
}

void Energy::Clear()
{
  SEEnergySystem::Clear();
  m_Patient = nullptr;
  m_PatientActions = nullptr;
  m_AortaHCO3 = nullptr;
  m_SkinSodium = nullptr;
  m_SkinChloride = nullptr;
  m_SkinPotassium = nullptr;
  m_coreNode = nullptr;
  m_skinNode = nullptr;
  m_temperatureGroundToCorePath = nullptr;
  m_coreToSkinPath = nullptr;
  m_skinExtravascularToSweatingGroundPath = nullptr;
  m_InternalTemperatureCircuit = nullptr;
  m_TemperatureCircuit = nullptr;

  m_BloodpH.Reset();
  m_BicarbonateMolarity_mmol_Per_L.Reset();
  m_previousWeightPack_kg = 0.0;
  m_packOn = false;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Energy::Initialize()
{
  BioGearsSystem::Initialize();

  GetTotalMetabolicRate().Set(m_Patient->GetBasalMetabolicRate());
  //Initialization of other system variables
  /// \cite herman2008physics
  GetCoreTemperature().SetValue(37.0, TemperatureUnit::C);
  GetSkinTemperature().SetValue(33.0, TemperatureUnit::C);
  /// \cite phypers2006lactate
  GetLactateProductionRate().SetValue(1.3, AmountPerTimeUnit::mol_Per_day);
  /// \cite guyton2006medical
  GetEnergyDeficit().SetValue(0.0, PowerUnit::W);
  GetExerciseEnergyDemand().SetValue(0.0, PowerUnit::W);
  GetExerciseMeanArterialPressureDelta().SetValue(0.0, PressureUnit::mmHg);
  GetTotalWorkRateLevel().SetValue(0.0);
  GetAchievedExerciseLevel().SetValue(0.0);
  GetFatigueLevel().SetValue(0.0);
  GetSweatRate().SetValue(0.0, MassPerTimeUnit::kg_Per_s);
  GetChlorideLostToSweat().SetValue(0.0, MassUnit::mg);
  GetPotassiumLostToSweat().SetValue(0.0, MassUnit::mg);
  GetSodiumLostToSweat().SetValue(0.0, MassUnit::mg);

  //Running average quantities used to trigger events
  m_BloodpH.Sample(7.4); //Initialize
  m_BicarbonateMolarity_mmol_Per_L.Sample(24.0); //Initialize

  m_previousWeightPack_kg = 0.0;
  m_packOn = false;
}

bool Energy::Load(const CDM::BioGearsEnergySystemData& in)
{
  if (!SEEnergySystem::Load(in)) {
    return false;
  }

  m_BloodpH.Load(in.BloodpH());
  m_BicarbonateMolarity_mmol_Per_L.Load(in.BicarbonateMolarity_mmol_Per_L());
  m_packOn = in.PackOn();
  m_previousWeightPack_kg = in.PreviousWeightPack_kg();
  BioGearsSystem::LoadState();
  return true;
}
CDM::BioGearsEnergySystemData* Energy::Unload() const
{
  CDM::BioGearsEnergySystemData* data = new CDM::BioGearsEnergySystemData();
  Unload(*data);
  return data;
}
void Energy::Unload(CDM::BioGearsEnergySystemData& data) const
{
  SEEnergySystem::Unload(data);

  data.BloodpH(std::unique_ptr<CDM::RunningAverageData>(m_BloodpH.Unload()));
  data.BicarbonateMolarity_mmol_Per_L(std::unique_ptr<CDM::RunningAverageData>(m_BicarbonateMolarity_mmol_Per_L.Unload()));
  data.PackOn(m_packOn);
  data.PreviousWeightPack_kg(m_previousWeightPack_kg);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes the energy specific quantities
///
/// \details
/// The energy local member variables, system level quantities, and metabolic-specific substance
/// quantities are initialized here. These variables are used in the numerous metabolic and thermal regulation
/// functions contained in the energy system. Encompassing them in this function allows for easy initialization
/// either at the beginning of a simulation or after a system crash
//--------------------------------------------------------------------------------------------------
void Energy::SetUp()
{
  m_dT_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  m_PatientActions = &m_data.GetActions().GetPatientActions();
  m_Patient = &m_data.GetPatient();

  m_AortaHCO3 = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Aorta)->GetSubstanceQuantity(m_data.GetSubstances().GetHCO3());
  m_SkinSodium = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::SkinExtracellular)->GetSubstanceQuantity(m_data.GetSubstances().GetSodium());
  m_SkinChloride = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::SkinExtracellular)->GetSubstanceQuantity(m_data.GetSubstances().GetChloride());
  m_SkinPotassium = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::SkinExtracellular)->GetSubstanceQuantity(m_data.GetSubstances().GetPotassium());
  //Circuit elements
  //Circuits
  m_TemperatureCircuit = &m_data.GetCircuits().GetTemperatureCircuit();
  m_InternalTemperatureCircuit = &m_data.GetCircuits().GetInternalTemperatureCircuit();
  //Nodes
  m_coreNode = m_InternalTemperatureCircuit->GetNode(BGE::InternalTemperatureNode::InternalCore);
  m_skinNode = m_InternalTemperatureCircuit->GetNode(BGE::InternalTemperatureNode::InternalSkin);
  //Paths
  m_temperatureGroundToCorePath = m_InternalTemperatureCircuit->GetPath(BGE::InternalTemperaturePath::GroundToInternalCore);
  m_coreToSkinPath = m_InternalTemperatureCircuit->GetPath(BGE::InternalTemperaturePath::InternalCoreToInternalSkin);
  m_skinExtravascularToSweatingGroundPath = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::SkinSweating);
}

void Energy::AtSteadyState()
{
  if (m_data.GetState() == EngineState::AtInitialStableState) {
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Prepares the energy system for the circuit solver
///
/// \details
/// The PreProcess function performs function in preparation to update the thermal circuit.
/// It also processes energy-related actions (exercise) and computes a sweat rate.
//--------------------------------------------------------------------------------------------------
void Energy::PreProcess()
{
  CalculateMetabolicHeatGeneration();
  CalculateSweatRate();
  UpdateHeatResistance();
  //ManageEnergyDeficit();
  Exercise();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// The exercise function updates the patient's metabolic rate if the exercise action is present
///
/// \details
/// The exercise function adds to the body's basal metabolic rate a value that is specified by
/// the exercise action. The actual metabolic rate is ramped up to this value. The body's actual
/// work rate is determined by the nutrients and oxygen available. Exercise-related outputs are
/// set in Tissue::CalculateMetabolicConsumptionAndProduction().
//--------------------------------------------------------------------------------------------------
void Energy::Exercise()
{
  double exerciseIntensity = 0.0;
  double DesiredWorkRate_W = 0.0;
  const double currentMetabolicRate_kcal_Per_day = GetTotalMetabolicRate().GetValue(PowerUnit::kcal_Per_day);
  const double basalMetabolicRate_kcal_Per_day = m_Patient->GetBasalMetabolicRate().GetValue(PowerUnit::kcal_Per_day);
  const double maxWorkRate_W = m_Patient->GetMaxWorkRate().GetValue(PowerUnit::W);
  const double kcal_Per_day_Per_Watt = 20.6362855;
  double cadenceCycle_Per_Min;
  double powerCycle_W;
  double speedRun_m_Per_s;
  double inclineRun_percent;
  double weightStrength_kg;
  double repetitionsStrength_number;
  double addedWeight_kg = 0.0;
  bool packOn = m_packOn;

  // Check for exercise call and only try to get intensity/desired work rate if the exercise action is active.
  auto exercise = m_PatientActions->GetExercise();
  if (m_PatientActions->HasExercise()) {
    switch (exercise->GetExerciseType()) {
    case SEExercise::GENERIC: {
      //Model Code
      SEExercise::SEGeneric genericEx = exercise->GetGenericExercise();
      if (genericEx.Intensity.IsValid()) {
        exerciseIntensity = genericEx.Intensity.GetValue();
      } else if (genericEx.DesiredWorkRate.IsValid()) {
        DesiredWorkRate_W = genericEx.DesiredWorkRate.GetValue(PowerUnit::W);
        exerciseIntensity = DesiredWorkRate_W / maxWorkRate_W;
        if (exerciseIntensity > 1) {
          exerciseIntensity = 1;
          Warning("Desired work rate over max work rate. Desired work rate can be a value between 0 and 1200 W. Proceeding with max work rate.");
        }
        genericEx.DesiredWorkRate.Clear();
      } else {
        Warning("Generic Exercise call with no severity. Action ignored.");
      }
      //
      break;
    }
    case SEExercise::RUNNING: {
      //Model Code
      SEExercise::SERunning runningEx = exercise->GetRunningExercise();
      speedRun_m_Per_s = runningEx.SpeedRun.GetValue(LengthPerTimeUnit::m_Per_s);
      const double speedRun_mph = speedRun_m_Per_s * 2.23694;
      inclineRun_percent = runningEx.InclineRun.GetValue();
      double inclineRun_fraction = inclineRun_percent / 100.0;
      addedWeight_kg = runningEx.AddedWeight.GetValue(MassUnit::kg);
      // Convert into intensity value
      const double C0 = 0.0;
      const double C1 = 0.0847;
      const double C2 = 4.34;
      const double C3 = -0.0019;
      const double C4 = -49;
      const double C5 = 1.16;
      exerciseIntensity = C0 + (C1 * speedRun_mph) + (C2 * inclineRun_fraction) + (C3 * std::pow(speedRun_mph, 2.0)) + (C4 * std::pow(inclineRun_fraction, 2.0)) + (C5 * speedRun_mph * inclineRun_fraction);
      if (speedRun_m_Per_s > 0.0) {
        BLIM(exerciseIntensity, 0.1, 1.0);
      } else {
        BLIM(exerciseIntensity, 0.0, 1.0);
      }
      //
      break;
    }
    case SEExercise::CYCLING: {
      //Model Code
      SEExercise::SECycling cyclingEx = exercise->GetCyclingExercise();
      cadenceCycle_Per_Min = cyclingEx.CadenceCycle.GetValue(FrequencyUnit::Per_min);
      powerCycle_W = cyclingEx.PowerCycle.GetValue(PowerUnit::W);
      addedWeight_kg = cyclingEx.AddedWeight.GetValue(MassUnit::kg);
      // Convert into intensity value
      const double C0 = -0.62;
      const double C1 = 0.0129;
      const double C2 = -0.0005;
      const double C3 = -0.00001;
      const double C4 = 0.00008;
      const double C5 = -0.00009;
      exerciseIntensity = C0 + (C1 * cadenceCycle_Per_Min) + (C2 * powerCycle_W) + (C3 * std::pow(cadenceCycle_Per_Min, 2.0)) + (C4 * std::pow(powerCycle_W, 2.0)) + (C5 * cadenceCycle_Per_Min * powerCycle_W);
      BLIM(exerciseIntensity, 0.0, 1.0);
      if (cadenceCycle_Per_Min > 0.0) {
        BLIM(exerciseIntensity, 0.05, 1.0);
      } else {
        BLIM(exerciseIntensity, 0.0, 1.0);
      }
      //
      break;
    }
    case SEExercise::STRENGTH_TRAINING: {
      //Model Code
      SEExercise::SEStrengthTraining strengthEx = exercise->GetStrengthExercise();
      weightStrength_kg = strengthEx.WeightStrength.GetValue(MassUnit::kg);
      repetitionsStrength_number = strengthEx.RepsStrength.GetValue();
      // Convert into intensity value
      exerciseIntensity = ((1 + (weightStrength_kg / m_Patient->GetWeight(MassUnit::kg))) * (0.0068 * repetitionsStrength_number) + 0.9864);
      BLIM(exerciseIntensity, 0.0, 1.0);
      //
      break;
    }
    case SEExercise::NONE:
      //No actual exercise - included to extend "full range of switch possibilities"
      break;
    }

    double previousWeightPack_kg = m_previousWeightPack_kg;
    if (addedWeight_kg != previousWeightPack_kg) {
      if (addedWeight_kg > previousWeightPack_kg)
        m_Patient->GetWeight().IncrementValue(addedWeight_kg - previousWeightPack_kg, MassUnit::kg);
      else if (addedWeight_kg < previousWeightPack_kg)
        m_Patient->GetWeight().DecrementValue(previousWeightPack_kg - addedWeight_kg, MassUnit::kg);

      m_previousWeightPack_kg = addedWeight_kg;
      packOn = true;
    }
  } else {
    if (packOn && m_previousWeightPack_kg > 0.0) {
      m_Patient->GetWeight().DecrementValue(m_previousWeightPack_kg, MassUnit::kg);
      packOn = false;
    }
    return;
  }
  exercise->GetGenericExercise().Intensity.SetValue(exerciseIntensity);
  // The MetabolicRateGain is used to ramp the metabolic rate to the value specified by the user's exercise intensity.
  const double MetabolicRateGain = m_dT_s;
  const double workRateDesired_W = exerciseIntensity * maxWorkRate_W;
  const double TotalMetabolicRateSetPoint_kcal_Per_day = basalMetabolicRate_kcal_Per_day + workRateDesired_W * kcal_Per_day_Per_Watt;
  const double exerciseEnergyIncrement_kcal_Per_day = MetabolicRateGain * (TotalMetabolicRateSetPoint_kcal_Per_day - currentMetabolicRate_kcal_Per_day);

  GetExerciseEnergyDemand().IncrementValue(exerciseEnergyIncrement_kcal_Per_day, PowerUnit::kcal_Per_day);
  GetTotalMetabolicRate().IncrementValue(exerciseEnergyIncrement_kcal_Per_day, PowerUnit::kcal_Per_day);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Processes the temperature circuit and calculates extravascular transport
///
/// \details
/// The Process function solves the temperature circuit for all path heat transfer rates and nodal temperatures.
/// Additionally, the process function also initiates the flow transport of the metabolic substance quantities
/// from the vascular to extravascular space. After this is complete, the vital signs of the patient are updated
/// with regards to the new state.
//--------------------------------------------------------------------------------------------------
void Energy::Process()
{
  m_circuitCalculator.Process(*m_TemperatureCircuit, m_dT_s);
  CalculateVitalSigns();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Updates the parameters for the temperature circuit
///
/// \details
/// Updates the resulting heat transfer rates, temperatures and nodal heat values; effectively moving
/// the next values to the current ones before time can be advanced.
//--------------------------------------------------------------------------------------------------
void Energy::PostProcess()
{
  if (m_data.GetActions().GetPatientActions().HasOverride()
      && m_data.GetState() == EngineState::Active) {
    if (m_data.GetActions().GetPatientActions().GetOverride()->HasEnergyOverride()) {
      ProcessOverride();
    }
  }
  m_circuitCalculator.PostProcess(*m_TemperatureCircuit);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Checks the patient's thermal and metabolic state
///
/// \details
/// The core and skin temperatures are recorded in this function. In addition, the current metabolic
/// state of the patient may trigger the following events: hypothermia, hyperthermia, and metabolic
/// acidosis/alkalosis. These events are only triggered if the current state falls within the criteria of the specific event
//--------------------------------------------------------------------------------------------------
void Energy::CalculateVitalSigns()
{
  double coreTemperature_degC = m_coreNode->GetTemperature(TemperatureUnit::C);
  double skinTemperature_degC = m_skinNode->GetTemperature(TemperatureUnit::C);
  if (m_data.GetDrugs().HasFeverChange() && GetCoreTemperature().GetValue(TemperatureUnit::C) > 37.0) { // Modifier for current drugs should not be able to increase core temperature
    coreTemperature_degC += m_data.GetDrugs().GetFeverChange().GetValue(TemperatureUnit::C);
    LLIM(coreTemperature_degC, 36.5); // Tylenol will not lower your basal core temperature
  }
  GetCoreTemperature().SetValue(coreTemperature_degC, TemperatureUnit::C);
  GetSkinTemperature().SetValue(skinTemperature_degC, TemperatureUnit::C);
  std::stringstream ss;

  //Hypothermia check
  double coreTempIrreversible_degC = 20.0; /// \cite Stocks2004HumanPhysiologicalResponseCold
  if (coreTemperature_degC < 35.0) /// \cite mallet2001hypothermia
  {
    /// \event Patient: Core temperature has fallen below 35 degrees Celsius. Patient is hypothermic.
    m_Patient->SetEvent(CDM::enumPatientEvent::Hypothermia, true, m_data.GetSimulationTime());

    /// \irreversible State: Core temperature has fallen below 20 degrees Celsius.
    if (coreTemperature_degC < coreTempIrreversible_degC) {
      ss << "Core temperature is " << coreTemperature_degC << ". This is below 20 degrees C, patient is experiencing extreme hypothermia.";
      Warning(ss);
      if (!m_PatientActions->HasOverride()) {
        m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
      } else {
        if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
          m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        }
      }
    }

  } else if (m_Patient->IsEventActive(CDM::enumPatientEvent::Hypothermia) && coreTemperature_degC > 35.2) {
    m_Patient->SetEvent(CDM::enumPatientEvent::Hypothermia, false, m_data.GetSimulationTime());
  }
  //Hyperthermia check
  if (coreTemperature_degC > 38.8) // Note: Hyperthermia threshold varies; we'll use 38.8
  {
    /// \event Patient: Core temperature has exceeded 38.8 degrees Celsius. Patient is hyperthermic.
    m_Patient->SetEvent(CDM::enumPatientEvent::Hyperthermia, true, m_data.GetSimulationTime());
  } else if (m_Patient->IsEventActive(CDM::enumPatientEvent::Hyperthermia) && coreTemperature_degC < 38.0) {
    m_Patient->SetEvent(CDM::enumPatientEvent::Hyperthermia, false, m_data.GetSimulationTime());
  }

  /// \todo Move to blood chemistry
  // Although it is called metabolic acidosis/alkalosis, the detection and event setting is actually a part of the @ref BloodChemistrySystem
  // The terms "metabolic" and "respiratory" refer to the origin of the acid-base disturbance
  // The hydrogen ion concentration is a property of the blood
  // The events related to blood concentrations should be detected and set in blood chemistry.
  double highPh = 8.5;
  double lowPh = 6.5; // \cite Edge2006AcidosisConscious
  m_BloodpH.Sample(m_data.GetBloodChemistry().GetArterialBloodPH().GetValue());
  m_BicarbonateMolarity_mmol_Per_L.Sample(m_AortaHCO3->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
  //Only check these at the end of a cardiac cycle and reset at start of cardiac cycle
  if (m_Patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
    double bloodPH = m_BloodpH.Value();
    double bloodBicarbonate_mmol_Per_L = m_BicarbonateMolarity_mmol_Per_L.Value();

    if (m_data.GetState() > EngineState::InitialStabilization) { // Don't throw events if we are initializing
      if (bloodPH < 7.35 && bloodBicarbonate_mmol_Per_L < 22.0) {
        /// \event The patient is in a state of metabolic acidosis
      }

      /// \irreversible State: arterial blood pH has dropped below 6.5.
      if (bloodPH < lowPh) {
        ss << " Arterial blood PH is " << bloodPH << ". This is below 6.5, patient is experiencing extreme metabolic acidosis and is in an irreversible state.";
        Warning(ss);
        if (!m_PatientActions->HasOverride()) {
          m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        } else {
          if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
            m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
          }
        }
      } else if (bloodPH > 7.38 && bloodBicarbonate_mmol_Per_L > 23.0) {
        /// \event The patient has exited the state state of metabolic acidosis
        m_Patient->SetEvent(CDM::enumPatientEvent::MetabolicAcidosis, false, m_data.GetSimulationTime());
      }

      if (bloodPH > 7.45 && bloodBicarbonate_mmol_Per_L > 26.0) {
        /// \event The patient is in a state of metabolic alkalosis
        m_Patient->SetEvent(CDM::enumPatientEvent::MetabolicAlkalosis, true, m_data.GetSimulationTime());
      }

      /// \irreversible State: arterial blood pH has increased above 8.5.
      if (bloodPH > highPh) {
        ss << " Arterial blood PH is " << bloodPH << ". This is above 8.5, patient is experiencing extreme metabolic Alkalosis and is in an irreversible state.";
        Warning(ss);
        if (!m_PatientActions->HasOverride()) {
          m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
        } else {
          if (m_PatientActions->GetOverride()->GetOverrideConformance() == CDM::enumOnOff::On) {
            m_Patient->SetEvent(CDM::enumPatientEvent::IrreversibleState, true, m_data.GetSimulationTime());
          }
        }
      }

      else if (bloodPH < 7.42 && bloodBicarbonate_mmol_Per_L < 25.0) {
        /// \event The patient has exited the state of metabolic alkalosis
        m_Patient->SetEvent(CDM::enumPatientEvent::MetabolicAlkalosis, false, m_data.GetSimulationTime());
      }
    }
    // Reset the running averages. Why do we need running averages here? Does the aorta pH fluctuate that much?
    m_BloodpH.Reset();
    m_BicarbonateMolarity_mmol_Per_L.Reset();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the metabolic rate, depending on the current state of the patient
///
/// \details
/// The metabolic rate is dictated by the current state of the core temperature. This function includes
/// states of increased metabolic rate due to shivering or severe hyperthermia. Additionally, an extreme
/// drop in core temperature leads to decreasing metabolic rate. If the core temperature does not meet any
/// of the criteria for increases/decreases, the metabolic rate will be calculated as the basal metabolic rate.
//--------------------------------------------------------------------------------------------------
void Energy::CalculateMetabolicHeatGeneration()
{

  double coreTemperature_degC = m_coreNode->GetTemperature(TemperatureUnit::C);
  double coreTemperatureLow_degC = m_data.GetConfiguration().GetCoreTemperatureLow(TemperatureUnit::C);
  double coreTemperatureLowDelta_degC = m_data.GetConfiguration().GetDeltaCoreTemperatureLow(TemperatureUnit::C);
  double coreTemperatureHigh_degC = 40.0; //TODO: This should use the config property why doesn't it?
  double totalMetabolicRateNew_Kcal_Per_day = 0.0;
  double totalMetabolicRateNew_W = 0.0;
  //The summit metabolism is the maximum amount of power the human body can generate due to shivering/response to the cold.
  double summitMetabolism_W = 21.0 * std::pow(m_Patient->GetWeight(MassUnit::kg), 0.75); /// \cite herman2008physics
  double currentMetabolicRate_kcal_Per_day = GetTotalMetabolicRate().GetValue(PowerUnit::kcal_Per_day);
  double basalMetabolicRate_kcal_Per_day = m_Patient->GetBasalMetabolicRate().GetValue(PowerUnit::kcal_Per_day);

  if (coreTemperature_degC < 34.0) //Hypothermic state inducing metabolic depression (decline of metabolic heat generation)
  {
    totalMetabolicRateNew_W = summitMetabolism_W * std::pow(0.94, 34.0 - coreTemperature_degC); //The metabolic heat generated will drop by 6% for every degree below 34 C
    GetTotalMetabolicRate().SetValue(totalMetabolicRateNew_W, PowerUnit::W); /// \cite mallet2002hypothermia
  } else if (coreTemperature_degC >= 34.0 && coreTemperature_degC < 36.8) //Patient is increasing heat generation via shivering. This caps out at the summit metabolism
  {
    //Todo: Add an event for shivering
    double basalMetabolicRate_W = m_Patient->GetBasalMetabolicRate(PowerUnit::W);
    totalMetabolicRateNew_W = basalMetabolicRate_W + (summitMetabolism_W - basalMetabolicRate_W) * (coreTemperatureLow_degC - coreTemperature_degC) / coreTemperatureLowDelta_degC;
    totalMetabolicRateNew_W = std::min(totalMetabolicRateNew_W, summitMetabolism_W); //Bounded at the summit metabolism so further heat generation doesn't continue for continue drops below 34 C.
    GetTotalMetabolicRate().SetValue(totalMetabolicRateNew_W, PowerUnit::W);
  } else if (coreTemperature_degC >= 36.8 && coreTemperature_degC < 40 && !m_PatientActions->HasExercise()) //Basic Metabolic rate
  {
    double TotalMetabolicRateSetPoint_kcal_Per_day = basalMetabolicRate_kcal_Per_day;
    double MetabolicRateGain = 0.0001; //Used to ramp the metabolic rate from its current value to the basal value if the patient meets the basal criteria
    double TotalMetabolicRateProduced_kcal_Per_day = currentMetabolicRate_kcal_Per_day + MetabolicRateGain * (TotalMetabolicRateSetPoint_kcal_Per_day - currentMetabolicRate_kcal_Per_day);
    GetTotalMetabolicRate().SetValue(TotalMetabolicRateProduced_kcal_Per_day, PowerUnit::kcal_Per_day);
  } else if (coreTemperature_degC > 40.0 && !m_PatientActions->HasExercise()) //Core temperature greater than 40.0. If not exercising, then the hyperthermia leads to increased metabolism
  {
    totalMetabolicRateNew_Kcal_Per_day = basalMetabolicRate_kcal_Per_day * std::pow(1.11, coreTemperature_degC - coreTemperatureHigh_degC); //The metabolic heat generated will increase by 11% for every degree above 40.0 C
    GetTotalMetabolicRate().SetValue(totalMetabolicRateNew_Kcal_Per_day, PowerUnit::kcal_Per_day); /// \cite pate2001thermal
  }

  m_temperatureGroundToCorePath->GetNextHeatSource().SetValue(GetTotalMetabolicRate(PowerUnit::W), PowerUnit::W);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the sweat rate if the core temperature is too high
///
/// \details
/// The sweat rate is calculated from a core temperature control function. The mass lost due to sweating is accounted for
/// and a flow source from the skin extravascular to ground path is updated to ensure fluid loss
//--------------------------------------------------------------------------------------------------
void Energy::CalculateSweatRate()
{
  double coreTemperature_degC = m_coreNode->GetTemperature(TemperatureUnit::C);
  double coreTemperatureHigh_degC = m_data.GetConfiguration().GetCoreTemperatureHigh(TemperatureUnit::C);
  double sweatHeatTranferCoefficient_W_Per_K = m_data.GetConfiguration().GetSweatHeatTransfer(HeatConductanceUnit::W_Per_K);

  /// Determine the maximum evaporative capacity to limit the amount of cooling due to sweat on the patient
  /// \cite potter2017mathematical
  double effectiveClothingEvaporation_im_Per_clo = std::pow(0.1, 0.29);
  if ((m_data.GetEnvironment().GetConditions().GetAirVelocity(LengthPerTimeUnit::m_Per_s)) > 0.1) {
    effectiveClothingEvaporation_im_Per_clo = std::pow((m_data.GetEnvironment().GetConditions().GetAirVelocity(LengthPerTimeUnit::m_Per_s)), 0.29);
  }

  double dAirTemperature_C = m_data.GetEnvironment().GetConditions().GetAmbientTemperature(TemperatureUnit::C);
  double dWaterVaporPressureInAmbientAir_mmHg = GeneralMath::AntoineEquation(dAirTemperature_C);
  double m_dWaterVaporPressureInAmbientAir_Pa = Convert(dWaterVaporPressureInAmbientAir_mmHg, PressureUnit::mmHg, PressureUnit::Pa);
  // double ambientAtmosphericPressure_Pa = m_data.GetEnvironment().GetConditions().GetAtmosphericPressure().GetValue(PressureUnit::Pa);
  double maximumEvaporativeCapacity_W = 14.21 * (m_Patient->GetSkinSurfaceArea().GetValue(AreaUnit::m2)) * effectiveClothingEvaporation_im_Per_clo * (133.322 * (std::pow(10, (8.1076 - (1750.286 / (235.0 + (m_skinNode->GetTemperature(TemperatureUnit::C))))))) - ((m_dWaterVaporPressureInAmbientAir_Pa))); //Still needs effective clothing evaporation

  double vaporizationEnergy_J_Per_kg = m_data.GetConfiguration().GetVaporizationEnergy(EnergyPerMassUnit::J_Per_kg);
  double sweatSodiumConcentration_mM = 51.0; /// \cite shirreffs1997whole
  double sweatPotassiumConcentration_mM = 6.0; /// \cite shirreffs1997whole
  double sweatChlorideConcentration_mM = 48.0; /// \cite shirreffs1997whole
    // static double totalSweatLost_mL = 0; --Used to figure out total sweat loss during exercise scenario during debugging

  double currentEvaporativeCapacity_W = sweatHeatTranferCoefficient_W_Per_K * (m_skinNode->GetTemperature(TemperatureUnit::K));
  if (currentEvaporativeCapacity_W > maximumEvaporativeCapacity_W) {

    sweatHeatTranferCoefficient_W_Per_K = maximumEvaporativeCapacity_W / (m_skinNode->GetTemperature(TemperatureUnit::K));
  }

  /// \todo Convert to sweat density once specific gravity calculation is in
  SEScalarMassPerVolume sweatDensity;
  GeneralMath::CalculateWaterDensity(m_skinNode->GetTemperature(), sweatDensity);
  double dehydrationFraction = m_data.GetTissue().GetDehydrationFraction().GetValue();

  //m_data.GetDataTrack().Probe("DehydrationPercent", dehydrationFraction*100);

  //Calculate sweat rate (in kg/s) from core temperature feedback.
  //The sweat rate heat transfer is determined from a control equation that attempts to keep the core temperature in line
  /// \cite herman2008physics
  //Sweat rate decreases as dehydration becomes more severe, with max reduction seen at 10% dehydration
  double dehydrationScalingFactor = GeneralMath::LinearInterpolator(0, .1, 1, 0, dehydrationFraction);
  BLIM(dehydrationScalingFactor, 0, 1);
  double sweatRate_kg_Per_s = dehydrationScalingFactor * (0.25 * sweatHeatTranferCoefficient_W_Per_K / vaporizationEnergy_J_Per_kg) * (coreTemperature_degC - coreTemperatureHigh_degC);

  //The Sweat Scaling Factor is caused by changes in the Hyperhidrosis patient parameter to invoke either hyperhidrosis or hypohidrosis
  /// \cite shih1983autonomic
  double sweatScalingFactor = 0.0;
  if (m_Patient->HasHyperhidrosis()) {
    sweatScalingFactor = m_Patient->GetHyperhidrosis().GetValue();
    sweatRate_kg_Per_s = (1 + sweatScalingFactor) * sweatRate_kg_Per_s;
  }

  double maxSweatRate_kg_Per_s = 12.5 * m_Patient->GetSkinSurfaceArea().GetValue(AreaUnit::m2) / 60.0 / 1000.0; //10 - 15 g/min/m2
  BLIM(sweatRate_kg_Per_s, 0.0, maxSweatRate_kg_Per_s);

  //Account for mass lost by subtracting from the current patient mass
  double massLost_kg = sweatRate_kg_Per_s * m_dT_s;
  m_Patient->GetWeight().IncrementValue(-massLost_kg, MassUnit::kg);
  GetSweatRate().SetValue(sweatRate_kg_Per_s, MassPerTimeUnit::kg_Per_s);

  //Calculate mass of ions lost in sweat (sodium, potassium, and chloride):  Converts kg sweat lost -> L sweat lost -> mmol ion lost -> mg ion lost
  double sodiumLost_mg = massLost_kg / sweatDensity.GetValue(MassPerVolumeUnit::kg_Per_L) * sweatSodiumConcentration_mM * m_data.GetSubstances().GetSodium().GetMolarMass(MassPerAmountUnit::mg_Per_mmol);
  double potassiumLost_mg = massLost_kg / sweatDensity.GetValue(MassPerVolumeUnit::kg_Per_L) * sweatPotassiumConcentration_mM * m_data.GetSubstances().GetPotassium().GetMolarMass(MassPerAmountUnit::mg_Per_mmol);
  double chlorideLost_mg = massLost_kg / sweatDensity.GetValue(MassPerVolumeUnit::kg_Per_L) * sweatChlorideConcentration_mM * m_data.GetSubstances().GetChloride().GetMolarMass(MassPerAmountUnit::mg_Per_mmol);

  //Decrement amount of each ion in the skin extracellular compartment, track the cumulative amount removed for output, and balance (i.e. update concentration) remaining levels
  m_SkinSodium->GetMass().IncrementValue(-sodiumLost_mg, MassUnit::mg);
  GetSodiumLostToSweat().IncrementValue(sodiumLost_mg, MassUnit::mg);
  m_SkinPotassium->GetMass().IncrementValue(-potassiumLost_mg, MassUnit::mg);
  GetPotassiumLostToSweat().IncrementValue(potassiumLost_mg, MassUnit::mg);
  m_SkinChloride->GetMass().IncrementValue(-chlorideLost_mg, MassUnit::mg);
  GetChlorideLostToSweat().IncrementValue(chlorideLost_mg, MassUnit::mg);
  m_SkinSodium->Balance(BalanceLiquidBy::Mass);
  m_SkinPotassium->Balance(BalanceLiquidBy::Mass);
  m_SkinChloride->Balance(BalanceLiquidBy::Mass);

  //Set the flow source on the extravascular circuit to begin removing the fluid that is excreted
  double sweatRate_mL_Per_s = sweatRate_kg_Per_s / sweatDensity.GetValue(MassPerVolumeUnit::kg_Per_mL);
  m_skinExtravascularToSweatingGroundPath->GetNextFlowSource().SetValue(sweatRate_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

  //totalSweatLost_mL += sweatRate_mL_Per_s * m_dT_s;
  //m_data.GetDataTrack().Probe("CumulativeSweatLost_mL", totalSweatLost_mL);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Updates the variable core to skin heat transfer resistance
///
/// \details
/// The variable core to skin heat transfer resistance is updated here according to the  inverse
/// of the skin blood flow.
//--------------------------------------------------------------------------------------------------
void Energy::UpdateHeatResistance()
{
  double skinBloodFlow_m3_Per_s = m_data.GetCardiovascular().GetMeanSkinFlow().GetValue(VolumePerTimeUnit::m3_Per_s);
  double bloodDensity_kg_Per_m3 = m_data.GetBloodChemistry().GetBloodDensity().GetValue(MassPerVolumeUnit::kg_Per_m3);
  double bloodSpecificHeat_J_Per_K_kg = m_data.GetBloodChemistry().GetBloodSpecificHeat().GetValue(HeatCapacitancePerMassUnit::J_Per_K_kg);

  double alphaScale = 0.5; //Scaling factor for convective heat transfer from core to skin (35 seems to be near the upper limit before non-stabilization)
  if (m_data.GetBloodChemistry().GetInflammatoryResponse().HasInflammationSource(CDM::enumInflammationSource::Burn)) {
    const double burnSurfaceAreaFraction = m_data.GetActions().GetPatientActions().GetBurnWound()->GetTotalBodySurfaceArea().GetValue();
    const double resInput = std::min(2.0 * burnSurfaceAreaFraction, 1.0); //Make >50% burn the worse case scenario
    const double targetAlpha = GeneralMath::LinearInterpolator(0.0, resInput, alphaScale, 20.0, resInput);
    const double lastAlpha = 1.0 / (m_coreToSkinPath->GetResistance(HeatResistanceUnit::K_Per_W) * bloodDensity_kg_Per_m3 * bloodSpecificHeat_J_Per_K_kg * skinBloodFlow_m3_Per_s);
    const double rampGain = 1.0e-5;
    alphaScale = lastAlpha + rampGain * (targetAlpha - lastAlpha);
  }

  //The heat transfer resistance from the core to the skin is inversely proportional to the skin blood flow.
  //When skin blood flow increases, then heat transfer resistance decreases leading to more heat transfer from core to skin.
  //The opposite occurs for skin blood flow decrease.
  double coreToSkinResistance_K_Per_W = 1.0 / (alphaScale * bloodDensity_kg_Per_m3 * bloodSpecificHeat_J_Per_K_kg * skinBloodFlow_m3_Per_s);

  coreToSkinResistance_K_Per_W = BLIM(coreToSkinResistance_K_Per_W, 0.0001, 20.0);

  m_coreToSkinPath->GetNextResistance().SetValue(coreToSkinResistance_K_Per_W, HeatResistanceUnit::K_Per_W);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the basal metabolic rate from the Harris-Benedict formula
///
///
/// \details
/// The Harris-Benedict formula uses the patient height, weight age and sex to determine
/// the basal metabolic requirements.
//--------------------------------------------------------------------------------------------------
void Energy::CalculateBasalMetabolicRate()
{
  SEPatient& patient = m_data.GetPatient();
  double PatientMass_kg = patient.GetWeight(MassUnit::kg);
  double PatientAge_yr = patient.GetAge(TimeUnit::yr);
  double PatientHeight_cm = patient.GetHeight(LengthUnit::cm);

  //The basal metabolic rate is determined from the Harris-Benedict formula, with differences dependent on sex, age, height and mass
  /// \cite roza1984metabolic
  double patientBMR_kcal_Per_day = 0.0;
  if (patient.GetGender() == CDM::enumSex::Male) {
    patientBMR_kcal_Per_day = 88.632 + 13.397 * PatientMass_kg + 4.799 * PatientHeight_cm - 5.677 * PatientAge_yr;
  } else {
    patientBMR_kcal_Per_day = 447.593 + 9.247 * PatientMass_kg + 3.098 * PatientHeight_cm - 4.330 * PatientAge_yr;
  }
  // Systems do their math with MetabolicRate in Watts, so let's make these consistent
  patient.GetBasalMetabolicRate().SetValue(patientBMR_kcal_Per_day, PowerUnit::kcal_Per_day);

  std::stringstream ss;
  ss << "Conditions applied homeostasis: "
     << "Patient basal metabolic rate = " << patientBMR_kcal_Per_day << " kcal/day";
  Info(ss);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Tracks gap in energy supply / demand due to pathophysiological states
///
///
/// \details
/// Pathophysiology such as hypoperfusion secondary to hemorrhage or sepsis cause the tissues to
/// extract more oxygen from the bloodstream to make up for mitochondrial dysfunction and increased
/// capillary diffusion distance.  Since we do not currently model this mechanistically, we track a
/// pathophysiological energy debt that we use to force the tissue to consume more O2 and pull it
/// from the vascular space, giving symptoms such as decreased pH and O2 saturation.
/// Currently, only triggered by hemorrhage.  Will add sepsis, burn wound.
//--------------------------------------------------------------------------------------------------
void Energy::ManageEnergyDeficit()
{
  if (m_PatientActions->HasHemorrhage()) {
    double basalTissueEnergyDemand_W = m_Patient->GetBasalMetabolicRate(PowerUnit::W) * 0.8; //We say in tissue that say brain takes up 20%, and we just care about the other tissues for this process
    double volFraction = m_data.GetCardiovascular().GetBloodVolume(VolumeUnit::mL) / m_Patient->GetBloodVolumeBaseline(VolumeUnit::mL);
    ULIM(volFraction, 1.0);
    double minVolFraction = 0.5; //i.e. half of blood volume lost
    double maxDeficitMultiplier = 0.5;
    double energyDeficit_W = basalTissueEnergyDemand_W * GeneralMath::LinearInterpolator(1.0, minVolFraction, 0.0, maxDeficitMultiplier, volFraction);
    GetEnergyDeficit().SetValue(energyDeficit_W, PowerUnit::W);
  } else if (GetEnergyDeficit(PowerUnit::W) > ZERO_APPROX) {
    double restoreFactor_Per_s = 0.001;
    double nextEnergyDeficit_W = -restoreFactor_Per_s * m_dT_s * GetEnergyDeficit(PowerUnit::W) + GetEnergyDeficit(PowerUnit::W);
    GetEnergyDeficit().SetValue(nextEnergyDeficit_W, PowerUnit::W);
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
void Energy::ProcessOverride()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();

#ifdef BIOGEARS_USE_OVERRIDE_CONTROL
  OverrideControlLoop();
#endif

  if (override->HasAchievedExerciseLevelOverride()) {
    GetAchievedExerciseLevel().SetValue(override->GetAchievedExerciseLevelOverride().GetValue());
  }
  if (override->HasCoreTemperatureOverride()) {
    GetCoreTemperature().SetValue(override->GetCoreTemperatureOverride(TemperatureUnit::C), TemperatureUnit::C);
  }
  if (override->HasCreatinineProductionRateOverride()) {
    GetCreatinineProductionRate().SetValue(override->GetCreatinineProductionRateOverride(AmountPerTimeUnit::mol_Per_s), AmountPerTimeUnit::mol_Per_s);
  }
  if (override->HasExerciseMeanArterialPressureDeltaOverride()) {
    GetExerciseMeanArterialPressureDelta().SetValue(override->GetExerciseMeanArterialPressureDeltaOverride(PressureUnit::mmHg), PressureUnit::mmHg);
  }
  if (override->HasFatigueLevelOverride()) {
    GetFatigueLevel().SetValue(override->GetFatigueLevelOverride().GetValue());
  }
  if (override->HasLactateProductionRateOverride()) {
    GetLactateProductionRate().SetValue(override->GetLactateProductionRateOverride(AmountPerTimeUnit::mol_Per_s), AmountPerTimeUnit::mol_Per_s);
  }
  if (override->HasSkinTemperatureOverride()) {
    GetSkinTemperature().SetValue(override->GetSkinTemperatureOverride(TemperatureUnit::C), TemperatureUnit::C);
  }
  if (override->HasSweatRateOverride()) {
    GetSweatRate().SetValue(override->GetSweatRateOverride(MassPerTimeUnit::g_Per_s), MassPerTimeUnit::g_Per_s);
  }
  if (override->HasTotalMetabolicOverride()) {
    GetTotalMetabolicRate().SetValue(override->GetTotalMetabolicOverride(PowerUnit::kcal_Per_day), PowerUnit::kcal_Per_day);
  }
  if (override->HasTotalWorkRateLevelOverride()) {
    GetTotalWorkRateLevel().SetValue(override->GetTotalWorkRateLevelOverride().GetValue());
  }
  if (override->HasSodiumLostToSweatOverride()) {
    GetSodiumLostToSweat().SetValue(override->GetSodiumLostToSweatOverride(MassUnit::g), MassUnit::g);
  }
  if (override->HasPotassiumLostToSweatOverride()) {
    GetPotassiumLostToSweat().SetValue(override->GetPotassiumLostToSweatOverride(MassUnit::g), MassUnit::g);
  }
  if (override->HasChlorideLostToSweatOverride()) {
    GetChlorideLostToSweat().SetValue(override->GetChlorideLostToSweatOverride(MassUnit::g), MassUnit::g);
  }
}

//// Can be turned on or off (for debugging purposes) using the Biogears_USE_OVERRIDE_CONTROL external in CMake
void Energy::OverrideControlLoop()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();

  constexpr double maxAcheivedExerciseOverride = 100;
  constexpr double minAcheivedExerciseOverride = 0;
  constexpr double maxCoreTempOverride = 200.0; //degC
  constexpr double minCoreTempOverride = 0.0; //degC
  constexpr double maxCreatinineOverride = 100.0; // mol_Per_s
  constexpr double minCreatinineOverride = 0.0; // mol_Per_s
  constexpr double maxExerciseMAPOverride = 200.0; //mmHg
  constexpr double minExerciseMAPOverride = 0.0; //mmHg
  constexpr double maxFatigueOverride = 100.0;
  constexpr double minFatigueOverride = 0.0;
  constexpr double maxLactateOverride = 200.0; //mol per s
  constexpr double minLactateOverride = 0.0; //mol per s
  constexpr double maxSkinTempOverride = 200.0; //degC
  constexpr double minSkinTempOverride = 0.0; //degC
  constexpr double maxSweatRateOverride = 50.0; // g per s
  constexpr double minSweatRateOverride = 0.0; // g per s
  constexpr double maxTotalMetabolicOverride = 5000.0; //kcal/day
  constexpr double minTotalMetabolicOverride = 0.0; //kcal/day
  constexpr double maxTotalWorkOverride = 100.0;
  constexpr double minTotalWorkOverride = 0.0;
  constexpr double maxSodiumSweatOverride = 500.0; // g
  constexpr double minSodiumSweatOverride = 0.0; // g
  constexpr double maxPotassiumSweatOverride = 500.0; // g
  constexpr double minPotassiumSweatOverride = 0.0; //  g
  constexpr double maxChlorideSweatOverride = 500.0; // g
  constexpr double minChlorideSweatOverride = 0.0; // g

  double currentAcheivedExerciseOverride = 0; //gets changed in next step
  double currentCoreTempOverride = 0; //gets changed in next step
  double currentCreatinineOverride = 0; //gets changed in next step
  double currentExerciseMAPOverride = 0; //gets changed in next step
  double currentFatigueOverride = 0; //gets changed in next step
  double currentLactateOverride = 0; //gets changed in next step
  double currentSkinTempOverride = 0; //gets changed in next step
  double currentSweatRateOverride = 0; //gets changed in next step
  double currentTotalMetabolicOverride = 0; //gets changed in next step
  double currentTotalWorkOverride = 0; //gets changed in next step
  double currentSodiumSweatOverride = 0.0; //gets changed in next step
  double currentPotassiumSweatOverride = 0.0; //gets changed in next step

  double currentChlorideSweatOverride = 0.0; //gets changed in next step

  if (override->HasAchievedExerciseLevelOverride()) {
    currentAcheivedExerciseOverride = override->GetAchievedExerciseLevelOverride().GetValue();
  }
  if (override->HasCoreTemperatureOverride()) {
    currentCoreTempOverride = override->GetCoreTemperatureOverride(TemperatureUnit::C);
  }
  if (override->HasCreatinineProductionRateOverride()) {
    currentCreatinineOverride = override->GetCreatinineProductionRateOverride(AmountPerTimeUnit::mol_Per_s);
  }
  if (override->HasExerciseMeanArterialPressureDeltaOverride()) {
    currentExerciseMAPOverride = override->GetExerciseMeanArterialPressureDeltaOverride(PressureUnit::mmHg);
  }
  if (override->HasFatigueLevelOverride()) {
    currentFatigueOverride = override->GetFatigueLevelOverride().GetValue();
  }
  if (override->HasLactateProductionRateOverride()) {
    currentLactateOverride = override->GetLactateProductionRateOverride(AmountPerTimeUnit::mol_Per_s);
  }
  if (override->HasSkinTemperatureOverride()) {
    currentSkinTempOverride = override->GetSkinTemperatureOverride(TemperatureUnit::C);
  }
  if (override->HasSweatRateOverride()) {
    currentSweatRateOverride = override->GetSweatRateOverride(MassPerTimeUnit::g_Per_s);
  }
  if (override->HasTotalMetabolicOverride()) {
    currentTotalMetabolicOverride = override->GetTotalMetabolicOverride(PowerUnit::kcal_Per_day);
  }
  if (override->HasTotalWorkRateLevelOverride()) {
    currentTotalWorkOverride = override->GetTotalWorkRateLevelOverride().GetValue();
  }
  if (override->HasSodiumLostToSweatOverride()) {
    currentSodiumSweatOverride = override->GetSodiumLostToSweatOverride(MassUnit::g);
  }
  if (override->HasPotassiumLostToSweatOverride()) {
    currentPotassiumSweatOverride = override->GetPotassiumLostToSweatOverride(MassUnit::g);
  }
  if (override->HasChlorideLostToSweatOverride()) {
    currentChlorideSweatOverride = override->GetChlorideLostToSweatOverride(MassUnit::g);
  }

  if ((currentAcheivedExerciseOverride < minAcheivedExerciseOverride
       || currentAcheivedExerciseOverride > maxAcheivedExerciseOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Achieved Exercise Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentCoreTempOverride < minCoreTempOverride
       || currentCoreTempOverride > maxCoreTempOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Core Temperature Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentCreatinineOverride < minCreatinineOverride
       || currentCreatinineOverride > maxCreatinineOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Creatinine Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentExerciseMAPOverride < minExerciseMAPOverride
       || currentExerciseMAPOverride > maxExerciseMAPOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Exercise Mean Arterial Pressure Delta Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentFatigueOverride < minFatigueOverride
       || currentFatigueOverride > maxFatigueOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Fatigue Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentLactateOverride < minLactateOverride
       || currentLactateOverride > maxLactateOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Lactate Production Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentSkinTempOverride < minSkinTempOverride
       || currentSkinTempOverride > maxSkinTempOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Skin Temperature Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentSweatRateOverride < minSweatRateOverride
       || currentSweatRateOverride > maxSweatRateOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Sweat Rate Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentTotalMetabolicOverride < minTotalMetabolicOverride
       || currentTotalMetabolicOverride > maxTotalMetabolicOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Total Metabolic Rate Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentTotalWorkOverride < minTotalWorkOverride
       || currentTotalWorkOverride > maxTotalWorkOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Total Work Rate Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentSodiumSweatOverride < minSodiumSweatOverride
       || currentSodiumSweatOverride > maxSodiumSweatOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Sodium Lost to Sweat Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentPotassiumSweatOverride < minPotassiumSweatOverride
       || currentPotassiumSweatOverride > maxPotassiumSweatOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Potassium Lost to Sweat Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  if ((currentChlorideSweatOverride < minChlorideSweatOverride
       || currentChlorideSweatOverride > maxChlorideSweatOverride)
      && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
    m_ss << "Chloride Lost to Sweat Override (Energy) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
    Info(m_ss);
    override->SetOverrideConformance(CDM::enumOnOff::Off);
  }
  return;
}
}