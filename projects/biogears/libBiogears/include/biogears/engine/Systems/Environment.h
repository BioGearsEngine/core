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

#include <biogears/cdm/system/environment/SEEnvironment.h>
#include <biogears/engine/Controller/BioGearsSystem.h>
#include <biogears/schema/biogears/BioGearsEnvironment.hxx>

namespace biogears {
class SEPatient;
class SEPatientActionCollection;
class SEEnvironmentActionCollection;
class SEGasCompartment;
class SELiquidCompartment;
class SEThermalCircuit;
class SEThermalCircuitNode;
class SEThermalCircuitPath;
class BioGears;
/**
 * @brief The %Environment class characterizes the environment and manages interactions between the body its surroundings.
 */
class BIOGEARS_API Environment : public SEEnvironment, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;

protected:
  Environment(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~Environment() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "Environment"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  virtual bool Load(const CDM::BioGearsEnvironmentData& in);
  virtual CDM::BioGearsEnvironmentData* Unload() const override;

protected:
  virtual void Unload(CDM::BioGearsEnvironmentData& data) const;

  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  void AtSteadyState() override;
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;

  void StateChange() override;

protected:
  void ProcessActions();
  void CalculateSupplementalValues();
  void CalculateRadiation();
  void CalculateConvection();
  void CalculateEvaporation();
  void CalculateRespiration();

  // Serializable member variables (Set in Initialize and in schema)

  // Stateless member variable (Calculated in Supplemental Method and used in other methods)
  double m_dLewisRelation;
  double m_dHeatOfVaporizationOfWater_J_Per_kg;
  double m_dWaterVaporPressureInAmbientAir_Pa;
  double m_dWaterVaporPressureAtSkin_Pa;
  //Prandtl number calculation
  double m_WaterSpecificHeat_J_Per_kg_K;
  double m_WaterViscosity_N_s_Per_m2;
  double m_WaterThermalConductivity_W_Per_m_K;
  //Grashof number calculation
  double m_ThermalExpansion_Per_K;
  // For Convection calculations
  double m_PatientEquivalentDiameter_m;

  // Stateless member variable (Set in SetUp())
  // Patient and Actions
  SEPatient* m_Patient;
  SEPatientActionCollection* m_PatientActions;
  SEEnvironmentActionCollection* m_EnvironmentActions;
  // Compartments
  SEGasCompartment* m_AmbientGases;
  SELiquidCompartment* m_AmbientAerosols;
  //Circuits
  SEThermalCircuit* m_EnvironmentCircuit;
  //Nodes
  SEThermalCircuitNode* m_ThermalEnvironment;
  SEThermalCircuitNode* m_SkinNode;
  SEThermalCircuitNode* m_ClothingNode;
  SEThermalCircuitNode* m_EnclosureNode;
  //Paths
  SEThermalCircuitPath* m_SkinToClothing;
  SEThermalCircuitPath* m_ActiveHeatTransferRatePath;
  SEThermalCircuitPath* m_ActiveTemperaturePath;
  SEThermalCircuitPath* m_ActiveSwitchPath;
  SEThermalCircuitPath* m_ClothingToEnclosurePath;
  SEThermalCircuitPath* m_GroundToEnclosurePath;
  SEThermalCircuitPath* m_ClothingToEnvironmentPath;
  SEThermalCircuitPath* m_GroundToEnvironmentPath;
  SEThermalCircuitPath* m_EnvironmentSkinToGroundPath;
  SEThermalCircuitPath* m_EnvironmentCoreToGroundPath;
};
}