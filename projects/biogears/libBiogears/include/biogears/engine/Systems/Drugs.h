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
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/engine/Controller/BioGearsSystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SETissueCompartment;
class SEFluidCircuitPath;
class SETissueCompartment;
class SESubstance;
class SESubstanceBolusState;
class SENasalState;
class SETransmucosalState;
class SELiquidCompartment;
}

namespace std {
BG_EXT template class BIOGEARS_API map<const biogears::SESubstance*, biogears::SESubstanceBolusState*>;
BG_EXT template class BIOGEARS_API map<const biogears::SESubstance*, biogears::SENasalState*>;
BG_EXT template class BIOGEARS_API map<const biogears::SESubstance*, biogears::SETransmucosalState*>;
}

namespace biogears {
class BioGears;
/**
 * @brief
 * The drug system contains the physiologically based pharmacokinetic (PBPK) model and the pharmacodynamic (PD) model.
 * @details 
 * Drug transvascular transport is modeled with a physiologically-based pharmacokinetic (PBPK) model,
 * and the physiologic effects on the body are modeled with a low-fidelity pharmacodynamic (PD) model.
 */
class BIOGEARS_API Drugs : public SEDrugSystem, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;

protected:
  static auto make_unique(BioGears& bg) -> std::unique_ptr<Drugs>;
  Drugs(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~Drugs() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); } //! Hopefully this returns a unique ID for every type
  static constexpr char const* const TypeTag() { return "Drugs"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  virtual bool Load(const CDM::BioGearsDrugSystemData& in);
  virtual CDM::BioGearsDrugSystemData* Unload() const override;

protected:
  virtual void Unload(CDM::BioGearsDrugSystemData& data) const;

  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  void AtSteadyState() override;
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;

protected:
  void AdministerSubstanceBolus();
  void AdministerSubstanceInfusion();
  void AdministerSubstanceCompoundInfusion();
  void AdministerSubstanceNasal();
  void AdministerSubstanceOral();

  void CalculatePartitionCoefficients();
  void CalculateSubstanceClearance();
  void CalculatePlasmaSubstanceConcentration();
  void CalculateDrugEffects();
  void SarinKinetics();
  double OralTransmucosalModel(const SESubstance* sub, SETransmucosalState* ot);

  // Serializable member variables (Set in Initialize and in schema)
  std::map<const SESubstance*, SESubstanceBolusState*> m_BolusAdministrations;
  std::map<const SESubstance*, SENasalState*> m_NasalStates;
  std::map<const SESubstance*, SETransmucosalState*> m_TransmucosalStates;

  double m_SarinRbcAcetylcholinesteraseComplex_nM;
  double m_AgedRbcAcetylcholinesterase_nM;

  // Stateless member variable (Set in SetUp())
  double m_dt_s;

  double m_RbcAcetylcholinesteraseFractionInhibited;
  SELiquidCompartment* m_aortaVascular;
  SELiquidCompartment* m_venaCavaVascular;
  SELiquidCompartment* m_muscleIntracellular;
  SELiquidCompartment* m_liverVascular;
  SETissueCompartment* m_liverTissue;
  SEFluidCircuitPath* m_IVToVenaCava;
  SETissueCompartment* m_fatTissue;
  SESubstance* m_Sarin;
  SESubstance* m_Pralidoxime;
  SESubstance* m_Atropine;
};
}
