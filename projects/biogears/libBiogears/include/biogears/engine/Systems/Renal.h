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

#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/system/physiology/SERenalSystem.h>
#include <biogears/cdm/utils/RunningAverage.h>
#include <biogears/engine/Controller/BioGearsSystem.h>


namespace biogears {
class SEUrinalysis;
class SEPatient;
class SEFluidCircuit;
class SEFluidCircuitNode;
class SEFluidCircuitPath;
class SESubstance;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;
class BioGears;
class SETissueCompartment;
namespace io {
  class BiogearsPhysiology;
}
/**
 * @brief @copydoc Physiology_RenalSystemData
 */
class BIOGEARS_API Renal : public SERenalSystem, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;
  friend class io::BiogearsPhysiology;

protected:
  static auto make_unique(BioGears& bg) -> std::unique_ptr<Renal>;
  Renal(BioGears& bg);
  BioGears& m_data;

  double m_dt;

public:
  virtual ~Renal() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "Renal"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

protected:
   // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  void AtSteadyState() override;
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;

  // Assessments
  bool CalculateUrinalysis(SEUrinalysis& u);
  SEScalar& CalculateRenalSOFA();

protected:
  struct ActiveTransport {
  public:
    double leftGlucoseReabsorptionMass_mg;
    double rightGlucoseReabsorptionMass_mg;
    double leftLactateExcretedMass_mg;
    double rightLactateExcretedMass_mg;
  };

  // Initialization
  void CalculateFilterability(SESubstance& sub);
  

  //Preprocess
  void CalculateUltrafiltrationFeedback();
  void CalculateColloidOsmoticPressure(SEScalarMassPerVolume& albuminConcentration, SEScalarPressure& osmoticPressure);
  void CalculateReabsorptionFeedback();
  void CalculateOsmoreceptorFeedback();
  void CalculateFluidPermeability();
  void CalculateTubuloglomerularFeedback();
  void UpdateBladderVolume();
  void ProcessActions();
  void Urinate();

  //Process
  void CalculateActiveTransport();
  void CalculateGlomerularTransport(SESubstance& sub);
  void CalculateGolmerularReabsorption(SESubstance& sub);
  void CalculateReabsorptionTransport(SESubstance& sub);
  void CalculateSecretion();
  void CalculateExcretion(SESubstance& sub);
  void CalculateAutomaticClearance(SESubstance& sub);
  void CalculateGluconeogenesis();
  void CalculateVitalSigns();

  //Override
  void ProcessOverride();
  void OverrideControlLoop();


  // Serializable member variables (Set in Initialize and in schema)
  bool m_Urinating;
  //Tubuloglomerular Feedback
  double m_LeftAfferentResistance_mmHg_s_Per_mL;
  double m_RightAfferentResistance_mmHg_s_Per_mL;
  double m_LeftSodiumFlowSetPoint_mg_Per_s;
  double m_RightSodiumFlowSetPoint_mg_Per_s;
  //Events
  RunningAverage m_UrineProductionRate_mL_Per_min;
  RunningAverage m_UrineOsmolarity_mOsm_Per_L;
  RunningAverage m_SodiumConcentration_mg_Per_mL;
  RunningAverage m_SodiumExcretionRate_mg_Per_min;
  RunningAverage m_LeftSodiumFlow_mg_Per_s;
  RunningAverage m_RightSodiumFlow_mg_Per_s;
  RunningAverage m_LeftRenalArterialPressure_mmHg;
  RunningAverage m_RightRenalArterialPressure_mmHg;

  // Stateless member variable (Set in SetUp())
  SEPatient* m_Patient;
  //Circuits
  SEFluidCircuit* m_RenalCircuit;
  //Nodes
  SEFluidCircuitNode* m_LeftGlomerularNode;
  SEFluidCircuitNode* m_LeftBowmansNode;
  SEFluidCircuitNode* m_LeftPeritubularNode;
  SEFluidCircuitNode* m_LeftTubulesNode;
  SEFluidCircuitNode* m_LeftRenalArteryNode;
  SEFluidCircuitNode* m_RightGlomerularNode;
  SEFluidCircuitNode* m_RightBowmansNode;
  SEFluidCircuitNode* m_RightPeritubularNode;
  SEFluidCircuitNode* m_RightTubulesNode;
  SEFluidCircuitNode* m_RightRenalArteryNode;
  SEFluidCircuitNode* m_BladderNode;
  SEFluidCircuitNode* m_LeftNetGlomerularCapillariesNode;
  SEFluidCircuitNode* m_LeftNetBowmansCapsulesNode;
  SEFluidCircuitNode* m_LeftNetPeritubularCapillariesNode;
  SEFluidCircuitNode* m_LeftNetTubulesNode;
  SEFluidCircuitNode* m_RightNetGlomerularCapillariesNode;
  SEFluidCircuitNode* m_RightNetBowmansCapsulesNode;
  SEFluidCircuitNode* m_RightNetPeritubularCapillariesNode;
  SEFluidCircuitNode* m_RightNetTubulesNode;
  //Paths
  SEFluidCircuitPath* m_LeftGlomerularOsmoticSourcePath;
  SEFluidCircuitPath* m_LeftBowmansOsmoticSourcePath;
  SEFluidCircuitPath* m_LeftReabsorptionResistancePath;
  SEFluidCircuitPath* m_LeftTubulesOsmoticSourcePath;
  SEFluidCircuitPath* m_LeftPeritubularOsmoticSourcePath;
  SEFluidCircuitPath* m_LeftUreterPath;
  SEFluidCircuitPath* m_LeftGlomerularFilterResistancePath;
  SEFluidCircuitPath* m_LeftAfferentArteriolePath;
  SEFluidCircuitPath* m_RightReabsorptionResistancePath;
  SEFluidCircuitPath* m_RightGlomerularOsmoticSourcePath;
  SEFluidCircuitPath* m_RightBowmansOsmoticSourcePath;
  SEFluidCircuitPath* m_RightTubulesOsmoticSourcePath;
  SEFluidCircuitPath* m_RightPeritubularOsmoticSourcePath;
  SEFluidCircuitPath* m_RightUreterPath;
  SEFluidCircuitPath* m_RightGlomerularFilterResistancePath;
  SEFluidCircuitPath* m_RightAfferentArteriolePath;
  SEFluidCircuitPath* m_BladderToGroundPressurePath;
  SEFluidCircuitPath* m_UrethraPath;
  SEFluidCircuitPath* m_LeftTubulesPath;
  SEFluidCircuitPath* m_RightTubulesPath;
  SEFluidCircuitPath* m_LeftEfferentArteriolePath;
  SEFluidCircuitPath* m_RightEfferentArteriolePath;

  //Substances
  SESubstance* m_Albumin;
  SESubstance* m_Sodium;
  SESubstance* m_Urea;
  SESubstance* m_Glucose;
  SESubstance* m_Lactate;
  SESubstance* m_Potassium;
  //Compartments
  SELiquidCompartment* m_Aorta;
  SELiquidCompartment* m_VenaCava;
  SELiquidCompartment* m_Bladder;
  SETissueCompartment* m_RightKidneyTissue;
  SETissueCompartment* m_LeftKidneyTissue;
  SELiquidCompartment* m_LeftUreter;
  SELiquidCompartment* m_LeftPeritubular;
  SELiquidCompartment* m_RightUreter;
  SELiquidCompartment* m_RightPeritubular;
  SELiquidCompartment* m_LeftGlomerular;
  SELiquidCompartment* m_LeftBowmans;
  SELiquidCompartment* m_RightGlomerular;
  SELiquidCompartment* m_RightBowmans;
  SELiquidCompartment* m_LeftTubules;
  SELiquidCompartment* m_RightTubules;
  SELiquidCompartment* m_Ground;
  // Compartment Substance Quantites
  SELiquidSubstanceQuantity* m_AortaLactate;

  SELiquidSubstanceQuantity* m_LeftPeritubularGlucose;
  SELiquidSubstanceQuantity* m_LeftPeritubularPotassium;
  SELiquidSubstanceQuantity* m_LeftUreterPotassium;
  SELiquidSubstanceQuantity* m_LeftUreterLactate;

  SELiquidSubstanceQuantity* m_RightPeritubularGlucose;
  SELiquidSubstanceQuantity* m_RightPeritubularPotassium;
  SELiquidSubstanceQuantity* m_RightUreterPotassium;
  SELiquidSubstanceQuantity* m_RightUreterLactate;

  SELiquidSubstanceQuantity* m_BladderGlucose;
  SELiquidSubstanceQuantity* m_BladderPotassium;
  SELiquidSubstanceQuantity* m_BladderAlbumin;
  SELiquidSubstanceQuantity* m_BladderSodium;
  SELiquidSubstanceQuantity* m_BladderUrea;
  SELiquidSubstanceQuantity* m_LeftTubulesSodium;
  SELiquidSubstanceQuantity* m_RightTubulesSodium;

  SELiquidSubstanceQuantity* m_LeftKidneyIntracellularLactate;
  SELiquidSubstanceQuantity* m_RightKidneyIntracellularLactate;

  // Configuration
  double m_DefaultOpenResistance_mmHg_s_Per_mL;
  double m_DefaultClosedResistance_mmHg_s_Per_mL;
  //   Ultrafiltration
  double m_CVOpenResistance_mmHg_s_Per_mL;
  //   Tubuloglomerular Feedback
  double m_MaxLeftAfferentResistance_mmHg_s_Per_mL;
  double m_MinLeftAfferentResistance_mmHg_s_Per_mL;
  double m_MaxRightAfferentResistance_mmHg_s_Per_mL;
  double m_MinRightAfferentResistance_mmHg_s_Per_mL;
  //   Osmoreceptor Feedback
  double m_LeftReabsorptionPermeabilityModificationFactor;
  double m_RightReabsorptionPermeabilityModificationFactor;
  double m_SodiumPlasmaConcentrationSetpoint_mg_Per_mL;
  //   Secretion
  double m_BaselinePotassiumConcentration_g_Per_dL;
  //   Reabsorption
  double m_LeftReabsorptionPermeabilitySetpoint_mL_Per_s_mmHg_m2;
  double m_RightReabsorptionPermeabilitySetpoint_mL_Per_s_mmHg_m2;

  // Utility/ScratchPads
  SEScalarMass m_SPCleared;
  ActiveTransport m_SubstanceTransport;

};
}
