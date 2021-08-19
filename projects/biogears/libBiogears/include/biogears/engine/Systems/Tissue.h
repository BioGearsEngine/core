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

#include <map>
#include <vector>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>
#include <biogears/cdm/system/physiology/SETissueSystem.h>
#include <biogears/cdm/utils/RunningAverage.h>
#include <biogears/engine/Controller/BioGearsSystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SESubstance;
class SEGasCompartment;
class SETissueCompartment;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;
class SEPatientActionCollection;
class SEPatient;
class SEEnergySystem;
class BioGears;
}

namespace std {
BG_EXT template class BIOGEARS_API map<biogears::SETissueCompartment*, biogears::SELiquidCompartment*>;
BG_EXT template class BIOGEARS_API map<biogears::SELiquidCompartment*, biogears::SEFluidCircuitPath*>;
BG_EXT template class BIOGEARS_API map<biogears::SETissueCompartment*, biogears::SEFluidCircuitPath*>;
BG_EXT template class BIOGEARS_API vector<biogears::SETissueCompartment*>;
}

namespace biogears {
/**
 * @brief This class encapsulates logic necessary to connect independent systems together.
 * @details Each system calculates the behavior that occurs within its individual physiology function; 
 * however, several processes extend between two systems, i.e., alveoli transfer links the %Respiratory and %Anesthesia Machine Systems. 
 * To capture this behavior, the System Interactions methodology was introduced. 
 * The primary function of this system is to capture the substance transport that occurs between systems.
 */
class BIOGEARS_API Tissue : public SETissueSystem, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;

protected:
  static auto make_unique(BioGears& bg) -> std::unique_ptr<Tissue>;
  Tissue(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~Tissue() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const* const TypeTag() { return "Tissue"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  virtual bool Load(const CDM::BioGearsTissueSystemData& in);
  virtual CDM::BioGearsTissueSystemData* Unload() const override;

protected:
  virtual void Unload(CDM::BioGearsTissueSystemData& data) const;
  // Set pointers and other member variables common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  void AtSteadyState() override;
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;

  // Used to add Hepatic O2/CO2 changes to Tissue outputs (there's probably a better way to transfer this data)
  static double m_hepaticO2Consumed_mol;
  static double m_hepaticCO2Produced_mol;

protected:
  // Preprocess Methods
  void ProteinStorageAndRelease();
  void FatStorageAndRelease();

  // Process Methods
  void CalculateCompartmentalBurn();
  void CalculateDiffusion();
  void CalculatePulmonaryCapillarySubstanceTransfer();
  void AlveolarPartialPressureGradientDiffusion(SEGasCompartment& pulmonary, SELiquidCompartment& vascular, SESubstance& sub, double DiffusingCapacityO2_mL_Per_s_mmHg, double timestep_s);
  void CalculateVitals();
  void CheckGlycogenLevels();
  void ManageSubstancesAndSaturation();

  //conditions
  void Dehydrate();

  // Tissue Nutrients
  void CalculateMetabolicConsumptionAndProduction(double time);
  void SetStarvationState();

  //Tissue fluid exchange
  void CalculateTissueFluidFluxes();
  void CalculateOncoticPressure();

  //Override
  void ProcessOverride();
  void OverrideControlLoop();

  // Serializable member variables (Set in Initialize and in schema)
  double m_RestingPatientMass_kg;
  double m_RestingFluidMass_kg;
  RunningAverage m_O2ConsumedRunningAverage_mL_Per_s;
  RunningAverage m_CO2ProducedRunningAverage_mL_Per_s;
  RunningAverage m_RespiratoryQuotientRunningAverage;
  RunningAverage m_FatigueRunningAverage;

  // Stateless member variable (Set in SetUp())
  double m_Dt_s;
  double m_maxProteinStorage_g;
  double m_lastFatigueTime;

  double m_leftArmDeltaResistance_mmHg_s_Per_mL;
  double m_rightArmDeltaResistance_mmHg_s_Per_mL;
  double m_leftLegDeltaResistance_mmHg_s_Per_mL;
  double m_rightLegDeltaResistance_mmHg_s_Per_mL;
  double m_trunkDeltaResistance_mmHg_s_Per_mL;

  double m_compartmentSyndromeCount;
  double m_baselineECFluidVolume_mL;

  bool m_trunkEscharotomy = false;
  bool m_leftArmEscharotomy = false;
  bool m_leftLegEscharotomy = false;
  bool m_rightArmEscharotomy = false;
  bool m_rightLegEscharotomy = false;

  std::stringstream m_ss;
  SESubstance* m_Albumin;
  SESubstance* m_AminoAcids;
  SESubstance* m_Glucagon;
  SESubstance* m_Glucose;
  SESubstance* m_Triacylglycerol;
  SESubstance* m_O2;
  SESubstance* m_CO2;
  SESubstance* m_CO;
  SESubstance* m_Lactate;
  SESubstance* m_Ketones;
  SESubstance* m_Creatinine;
  SESubstance* m_Sodium;
  SESubstance* m_Potassium;
  SESubstance* m_Calcium;
  SESubstance* m_Chloride;
  SESubstance* m_Insulin;
  SESubstance* m_Urea;

  SEFluidCircuitNode* m_GutE3;
  SEFluidCircuitPath* m_GutE3ToGround;
  SEFluidCircuitPath* m_SmallIntestineToGut;
  SEFluidCircuitPath* m_SplanchnicToGut;

  SELiquidSubstanceQuantity* m_MuscleInsulin;
  SELiquidSubstanceQuantity* m_MuscleGlucagon;
  SELiquidSubstanceQuantity* m_MuscleAA;
  SELiquidSubstanceQuantity* m_FatInsulin;
  SELiquidSubstanceQuantity* m_FatGlucagon;
  SELiquidSubstanceQuantity* m_FatTAG;
  SETissueCompartment* m_LeftLungTissue;
  SETissueCompartment* m_RightLungTissue;
  SETissueCompartment* m_MuscleTissue;
  SELiquidCompartment* m_MuscleIntracellular;
  SETissueCompartment* m_LiverTissue;
  SELiquidCompartment* m_LiverIntracellular;
  SELiquidCompartment* m_LiverExtracellular;
  SETissueCompartment* m_FatTissue;
  SELiquidCompartment* m_FatIntracellular;
  SETissueCompartment* m_BrainTissue;

  SELiquidCompartment* m_FatVascular;
  SELiquidSubstanceQuantity* m_FatVascularLipid;
  SELiquidSubstanceQuantity* m_LiverVascularGlucose;
  SELiquidCompartment* m_MuscleVascular;
  SELiquidSubstanceQuantity* m_MuscleVascularGlucose;

  SEGasCompartment* m_LeftAlveoli;
  SEGasCompartment* m_RightAlveoli;
  SELiquidCompartment* m_LeftPulmonaryCapillaries;
  SELiquidCompartment* m_RightPulmonaryCapillaries;

  SEPatientActionCollection* m_PatientActions;
  SEPatient* m_Patient;
  SEEnergySystem* m_energy;

  std::map<SETissueCompartment*, SELiquidCompartment*> m_TissueToVascular;
  std::map<SELiquidCompartment*, SEFluidCircuitPath*> m_VascularCopPaths;
  std::map<SETissueCompartment*, SEFluidCircuitPath*> m_InterstitialCopPaths;
  std::map<SETissueCompartment*, SEFluidCircuitPath*> m_ExtraToIntraPaths;
  std::map<SETissueCompartment*, SEFluidCircuitPath*> m_EndothelialResistancePaths;
  std::vector<SETissueCompartment*> m_ConsumptionProdutionTissues;
  std::string m_AnaerobicTissues;
};
}