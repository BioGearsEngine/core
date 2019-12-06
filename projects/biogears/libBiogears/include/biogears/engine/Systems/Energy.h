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

#pragma once
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/cdm/circuit/thermal/SEThermalCircuitCalculator.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/utils/RunningAverage.h>
#include <biogears/engine/Controller/BioGearsSystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SEPatient;
class SEPatientActionCollection;
class SELiquidSubstanceQuantity;
class SEThermalCircuitNode;
class SEThermalCircuitPath;
class SEFluidCircuitPath;
class SEThermalCircuit;
class SEThermalCircuitCalculator;
class BioGears;

/**
 * @brief @copydoc Physiology_EnergySystemData
 */
class BIOGEARS_API Energy : public SEEnergySystem, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;

protected:
  Energy(BioGears& bg);
  BioGears& m_data;

public:
  ~Energy(void) override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "Energy"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  virtual bool Load(const CDM::BioGearsEnergySystemData& in);
  virtual CDM::BioGearsEnergySystemData* Unload() const override;

protected:
  virtual void Unload(CDM::BioGearsEnergySystemData& data) const;

  // Set pointers and other member variables common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  void AtSteadyState() override;
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;

private:
  // Preprocess Methods
  void CalculateMetabolicHeatGeneration();
  void CalculateSweatRate();
  void UpdateHeatResistance();
  // Actions/Conditions
  void Exercise();

protected:
  void ManageEnergyDeficit();
  void ProcessOverride();
  void OverrideControlLoop();

private:
  // These are both part of the consume meal condition.
  //void Dehydration(double time); // Need to revisit

  // Used in Reset & Starvation
  void CalculateBasalMetabolicRate();

  // Process Methods
  void CalculateVitalSigns();

  // Serializable member variables (Set in Initialize and in schema)
  RunningAverage m_BloodpH;
  RunningAverage m_BicarbonateMolarity_mmol_Per_L;

  // Stateless member variable (Set in SetUp())
  double m_dT_s;
  SEPatient* m_Patient;
  SEPatientActionCollection* m_PatientActions;
  // Cmpts,Substance, and nodes
  SELiquidSubstanceQuantity* m_AortaHCO3;
  SELiquidSubstanceQuantity* m_SkinSodium;
  SELiquidSubstanceQuantity* m_SkinChloride;
  SELiquidSubstanceQuantity* m_SkinPotassium;
  //Nodes
  SEThermalCircuitNode* m_coreNode;
  SEThermalCircuitNode* m_skinNode;
  //Paths
  SEThermalCircuitPath* m_temperatureGroundToCorePath;
  SEThermalCircuitPath* m_coreToSkinPath;
  SEFluidCircuitPath* m_skinExtravascularToSweatingGroundPath;
  //Circuits
  SEThermalCircuit* m_InternalTemperatureCircuit;
  SEThermalCircuit* m_TemperatureCircuit;
  SEThermalCircuitCalculator m_circuitCalculator;

};
}
