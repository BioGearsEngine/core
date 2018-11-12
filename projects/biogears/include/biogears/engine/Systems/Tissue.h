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
  Tissue(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~Tissue();

  void Clear();

  // Set members to a stable homeostatic state
  void Initialize();

  // Load a state
  virtual bool Load(const CDM::BioGearsTissueSystemData& in);
  virtual CDM::BioGearsTissueSystemData* Unload() const;

protected:
  virtual void Unload(CDM::BioGearsTissueSystemData& data) const;
  // Set pointers and other member variables common to both homeostatic initialization and loading a state
  void SetUp();

public:
  void AtSteadyState();
  void PreProcess();
  void Process();
  void PostProcess();

  // Used to add Hepatic O2/CO2 changes to Tissue outputs (there's probably a better way to transfer this data)
  static double m_hepaticO2Consumed_mol;
  static double m_hepaticCO2Produced_mol;

protected:
  // Preprocess Methods
  void ProteinStorageAndRelease();
  void FatStorageAndRelease();

  // Process Methods
  void CalculateDiffusion();
  void CalculatePulmonaryCapillarySubstanceTransfer();
  void CalculateVitals();
  void CheckGlycogenLevels();
  void ManageSubstancesAndSaturation();


  // Postprocess Methods

  //conditions
  void Dehydrate();

  // Tissue System
  void CalculateMetabolicConsumptionAndProduction(double time);
  void SetStarvationState();

  // Diffusion Utilities
  void DistributeMassbyVolumeWeighted(SELiquidCompartment& cmpt, const SESubstance& sub, double mass, const MassUnit& unit);
  void DistributeMassbyMassWeighted(SELiquidCompartment& cmpt, const SESubstance& sub, double mass, const MassUnit& unit);

  double PerfusionLimitedDiffusion(SETissueCompartment& tissue, SELiquidCompartment& vascular, const SESubstance& sub, double partitionCoeff, double timestep_s);
  void AlveolarPartialPressureGradientDiffusion(SEGasCompartment& pulmonary, SELiquidCompartment& vascular, SESubstance& sub, double DiffusingCapacityO2_mL_Per_s_mmHg, double timestep_s);

  double MoveMassByInstantDiffusion(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub, double timestep_s);
  double MoveMassBySimpleDiffusion(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub, double permeabilityCofficient_mL_Per_s, double timestep_s);
  double MoveMassByFacilitatedDiffusion(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub, double combinedCoefficient_g_Per_s, double timestep_s);
  double MoveMassByActiveTransport(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub, double pumpRate_g_Per_s, double timestep_s);
  double MoveMassByConvection(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub, double timestep_s);
  void CoupledIonTransport(SETissueCompartment& tissue, SELiquidCompartment& extra, SELiquidCompartment& intra, SELiquidCompartment& vascular);
  double AlbuminTransport(SELiquidCompartment& vascular, SELiquidCompartment& extra, SETissueCompartment& tissue, double timestep_s);
  void CalculateTissueFluidFluxes();

  void CalculateOncoticPressure();

  double SodiumPotassiumPump(double intraNa_mM, double extraNa_mM, double extraK_mM, double potential_V);
  double CalciumPump(double intraCa_M);

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
  std::map<SELiquidCompartment*, SEFluidCircuitPath*> m_LymphPaths;
  std::vector<SETissueCompartment*> m_ConsumptionProdutionTissues;
  std::string m_AnaerobicTissues;
};
}