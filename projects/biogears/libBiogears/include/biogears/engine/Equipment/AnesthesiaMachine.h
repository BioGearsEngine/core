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

#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/engine/Controller/BioGearsSystem.h>
#include <biogears/schema/biogears/BioGears.hxx>
#include <biogears/schema/biogears/BioGearsEquipment.hxx>

namespace biogears {
class BioGears;
class SEAnesthesiaMachineActionCollection;
class SEGasCompartment;
class SEGasSubstanceQuantity;
class SEFluidCircuitNode;
class SEFluidCircuitPath;

/**
 * @brief 
 * Generic anesthesia machine for positive pressure ventilation.
 */
class BIOGEARS_API AnesthesiaMachine : public SEAnesthesiaMachine, public BioGearsSystem {
  friend BioGears;
  friend class BioGearsEngineTest;

protected:
  AnesthesiaMachine(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~AnesthesiaMachine();

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "AnesthesiaMachine"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  virtual bool Load(const CDM::BioGearsAnesthesiaMachineData& in);
  virtual CDM::BioGearsAnesthesiaMachineData* Unload() const override;

protected:
  virtual void Unload(CDM::BioGearsAnesthesiaMachineData& data) const;

  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  void StateChange() override;

  void PreProcess() override;
  void Process() override;
  void PostProcess() override;

  void CalculateScrubber();

  // Extending some functionality to these base class methods
  // We will update the BioGears Airway mode when these are called
  void SetConnection(CDM::enumAnesthesiaMachineConnection::value c) override;
  void InvalidateConnection() override;

private:
  void CalculateSourceStatus();
  void CalculateEquipmentLeak();
  void SetConnection();
  void CalculateValveResistances();
  void CalculateVentilator();
  void CalculateGasSource();
  void CalculateCyclePhase();
  void CheckReliefValve();

  // Serializable member variables (Set in Initialize and in schema)
  bool m_inhaling;
  SEScalarTime m_inspirationTime;
  SEScalar m_O2InletVolumeFraction;
  SEScalarTime m_currentbreathingCycleTime;
  SEScalarTime m_totalBreathingCycleTime;

  // Stateless member variable (Set in SetUp())
  double m_dt_s;
  double m_dValveOpenResistance_cmH2O_s_Per_L;
  double m_dValveClosedResistance_cmH2O_s_Per_L;
  double m_dSwitchOpenResistance_cmH2O_s_Per_L;
  double m_dSwitchClosedResistance_cmH2O_s_Per_L;
  SEAnesthesiaMachineActionCollection* m_actions;
  SEGasCompartment* m_ambient;
  SEGasSubstanceQuantity* m_ambientCO2;
  SEGasSubstanceQuantity* m_ambientN2;
  SEGasSubstanceQuantity* m_ambientO2;
  SEGasCompartment* m_gasSource;
  SEGasSubstanceQuantity* m_gasSourceCO2;
  SEGasSubstanceQuantity* m_gasSourceN2;
  SEGasSubstanceQuantity* m_gasSourceO2;
  SEGasCompartment* m_scrubber;
  SEGasSubstanceQuantity* m_scubberCO2;
  SEGasSubstanceQuantity* m_scrubberN2;
  SEFluidCircuitNode* m_nVentilator;
  SEFluidCircuitNode* m_nSelector;
  SEFluidCircuitPath* m_pAnesthesiaConnectionToEnvironment;
  SEFluidCircuitPath* m_pYPieceToExpiratoryLimb;
  SEFluidCircuitPath* m_pGasSourceToGasInlet;
  SEFluidCircuitPath* m_pInspiratoryLimbToYPiece;
  SEFluidCircuitPath* m_pSelectorToReliefValve;
  SEFluidCircuitPath* m_pEnvironmentToReliefValve;
  SEFluidCircuitPath* m_pSelectorToEnvironment;
  SEFluidCircuitPath* m_pEnvironmentToVentilator;
  SEFluidCircuitPath* m_pExpiratoryLimbToSelector;
  SEFluidCircuitPath* m_pSelectorToScrubber;
};
}