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

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/utils/RunningAverage.h>
#include <biogears/engine/Controller/BioGearsSystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SEArterialBloodGasAnalysis;
class SECompleteBloodCount;
class SEComprehensiveMetabolicPanel;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;
class SEPatientActionCollection;
class BioGears;
/**
 * @brief @copydoc Physiology_BloodChemistrySystemData
 * The Blood Chemistry System holds the system-level blood substance data that is computed on the compartment level by other systems.
 * @details
 * The goal of the blood chemistry system in the BioGears engine is complete analysis on the blood at any given point during the simulation.
 * The blood chemistry system houses all of the blood concentrations and compositions needed to assess a patient's health. This system is under development
 * and will be improved in future releases to include more substances that can provide clinician level details and assessments, such as a CBC and blood panel.
 */
class BIOGEARS_API BloodChemistry : public SEBloodChemistrySystem, public BioGearsSystem {
  friend BioGears;
  friend class BioGearsEngineTest;

protected:
  static auto make_unique(BioGears& bg) -> std::unique_ptr<BloodChemistry>;
  BloodChemistry(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~BloodChemistry() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); } //! Hopefully this returns a unique ID for every type
  static constexpr char const* const TypeTag() { return "BloodChemistry"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  bool Load(const CDM::BioGearsBloodChemistrySystemData& in);
  CDM::BioGearsBloodChemistrySystemData* Unload() const override;

protected:
  void Unload(CDM::BioGearsBloodChemistrySystemData& data) const;

  // Set pointers and other member variables common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  void AtSteadyState() override;
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;

  bool CalculateArterialBloodGasAnalysis(SEArterialBloodGasAnalysis& abga);
  bool CalculateCompleteBloodCount(SECompleteBloodCount& cbc);
  bool CalculateComprehensiveMetabolicPanel(SEComprehensiveMetabolicPanel& cmp);
  SEScalar& CalculateCoagulationSOFA();
  void CalculateHemolyticTransfusionReaction(bool rhMismatch = false);

protected:
  void AcuteRadiationSyndrome();
  void CheckRadiationSymptoms();
  void CheckBloodSubstanceLevels();
  void InflammatoryResponse();
  void ManageSIRS();		//SIRS = Systemic Inflammatory Response Syndrome


  //Override
  void ProcessOverride();
  void OverrideControlLoop();

  // Serializable member variables (Set in Initialize and in schema)
  RunningAverage m_ArterialOxygen_mmHg;
  RunningAverage m_ArterialCarbonDioxide_mmHg;

  // Patient
  SEPatient* m_Patient;
  SEPatientActionCollection* m_PatientActions;

  // Stateless member variable (Set in SetUp())
  double m_dt_s;
  double m_dt_hr;
  double m_redBloodCellVolume_mL;
  double m_HbLostToUrine_g;
  double m_RhTransfusionReactionVolume_mL;
  double m_HbPerRedBloodCell_ug_Per_ct;
  SEInflammatoryResponse* m_InflammatoryResponse;
  double m_RhFactorMismatch_ct;
  SELiquidCompartment* m_aorta;
  SELiquidSubstanceQuantity* m_aortaO2;
  SELiquidSubstanceQuantity* m_aortaCO2;
  SELiquidSubstanceQuantity* m_aortaCO;
  SELiquidSubstanceQuantity* m_aortaBicarbonate;
  SELiquidSubstanceQuantity* m_brainO2;
  SELiquidSubstanceQuantity* m_myocardiumO2;
  SELiquidSubstanceQuantity* m_pulmonaryArteriesO2;
  SELiquidSubstanceQuantity* m_pulmonaryArteriesCO2;
  SELiquidSubstanceQuantity* m_pulmonaryVeinsO2;
  SELiquidSubstanceQuantity* m_pulmonaryVeinsCO2;
  SELiquidCompartment* m_venaCava;
  SELiquidSubstanceQuantity* m_venaCavaO2;
  SELiquidSubstanceQuantity* m_venaCavaCO2;
  SELiquidSubstanceQuantity* m_venaCavaAlbumin;
  SELiquidSubstanceQuantity* m_venaCavaAminoAcids;
  SELiquidSubstanceQuantity* m_venaCavaBicarbonate;
  SELiquidSubstanceQuantity* m_venaCavaCalcium;
  SELiquidSubstanceQuantity* m_venaCavaChloride;
  SELiquidSubstanceQuantity* m_venaCavaCreatinine;
  SELiquidSubstanceQuantity* m_venaCavaEpinephrine;
  SELiquidSubstanceQuantity* m_venaCavaGlucagon;
  SELiquidSubstanceQuantity* m_venaCavaGlucose;
  SELiquidSubstanceQuantity* m_venaCavaInsulin;
  SELiquidSubstanceQuantity* m_venaCavaKetones;
  SELiquidSubstanceQuantity* m_venaCavaLactate;
  SELiquidSubstanceQuantity* m_venaCavaPotassium;
  SELiquidSubstanceQuantity* m_venaCavaRBC;
  SELiquidSubstanceQuantity* m_venaCavaSodium;
  SELiquidSubstanceQuantity* m_venaCavaTriacylglycerol;
  SELiquidSubstanceQuantity* m_venaCavaUrea;

  //radiation model parameters, first sets are growing within the marrow and thymus
  double m_progenitorLymphocytes_ct;
  double m_progenitorLymphocytes_wd_ct;
  double m_progenitorLymphocytes_d_ct;
  double m_progenitorLymphocytes_hd_ct;

  double m_maturingLymphocytes_ct;
  double m_maturingLymphocytes_d_ct;
  double m_maturingLymphocytes_hd_ct;

  //circulating counts
  double m_Lymphocytes_ct;
  double m_Lymphocytes_d_ct;
  double m_Lymphocytes_hd_ct;
  //track radiation
  double m_radAbsorbed_Gy;


  //Initialize HTR concentrations
  double m_donorRBC_ct;
  double m_patientRBC_ct;
  double m_2Agglutinate_ct;
  double m_p3Agglutinate_ct;
  double m_d3Agglutinate_ct;
  double m_4Agglutinate_ct;
  double m_RemovedRBC_ct;
};
}
