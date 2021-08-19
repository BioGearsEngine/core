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

#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>
#include <biogears/engine/Controller/BioGearsSystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SELiquidCompartment;
class SELiquidSubstanceQuantity;
class SEFluidCircuitPath;
class BioGears;


BG_EXT template class BIOGEARS_API std::map<SELiquidSubstanceQuantity*, double>;
/**
* @brief @copydoc Physiology_GastrointestinalSystemData
*/
class BIOGEARS_API Gastrointestinal : public SEGastrointestinalSystem, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;

protected:
  static auto make_unique(BioGears& bg) -> std::unique_ptr<Gastrointestinal>;
  Gastrointestinal(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~Gastrointestinal() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); } //! Hopefully this returns a unique ID for every type
  static constexpr char const* const TypeTag() { return "Gastrointestinal"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  virtual bool Load(const CDM::BioGearsGastrointestinalSystemData& in);
  virtual CDM::BioGearsGastrointestinalSystemData* Unload() const override;

protected:
  virtual void Unload(CDM::BioGearsGastrointestinalSystemData& data) const;

  // Set pointers and other member variables common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  void AtSteadyState() override;
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;

protected:
  void GastricSecretion(double duration_s);
  void AbsorbNutrients();
  void DigestNutrient();
  void AbsorbMeal(double duration_s);
  void AbsorbNutrients(double duration_s);

  void ProcessDrugCAT(); //Compartment, absorption, and transit model for oral drug dosing

  void ChymeSecretion();
  // Serializable member variables (Set in Initialize and in schema)

  // Stateless member variable (Set in SetUp())
  SELiquidCompartment* m_SmallIntestineChyme;
  SELiquidCompartment* m_vSmallIntestine;
  SELiquidCompartment* m_Lymph;
  SELiquidSubstanceQuantity* m_SmallIntestineChymeAlbumin;
  SELiquidSubstanceQuantity* m_SmallIntestineChymeAminoAcids;
  SELiquidSubstanceQuantity* m_SmallIntestineChymeGlucose;
  SELiquidSubstanceQuantity* m_SmallIntestineChymeTriacylglycerol;
  SELiquidSubstanceQuantity* m_SmallIntestineChymeCalcium;
  SELiquidSubstanceQuantity* m_SmallIntestineChymeSodium;
  SELiquidSubstanceQuantity* m_SmallIntestineChymeUrea;
  SELiquidSubstanceQuantity* m_SmallIntestineVascularSodium;
  SELiquidSubstanceQuantity* m_smallIntestineVascularGlucose;
  SELiquidSubstanceQuantity* m_smallIntestineVascularAminoAcids;
  SELiquidSubstanceQuantity* m_SmallIntestineVascularCalcium;
  SELiquidSubstanceQuantity* m_LymphTriacylglycerol;
  SELiquidSubstanceQuantity* m_smallintestineVAscularTriacylglycerol;

  SEFluidCircuitPath* m_GItoCVPath;
  SEFluidCircuitPath* m_GutE3ToGroundPath;
  SEScalarMassPerTime m_CalciumDigestionRate;
  SEScalarVolumePerTime m_WaterDigestionRate;

  //Drug absoprtion member variables
  std::vector<double> m_TransitPH;
  std::vector<double> m_TransitVolume_mL;
  std::vector<double> m_TransitSurfaceArea_cm2;
  std::vector<double> m_EnterocyteVolumeFraction;
  std::vector<double> m_TransitRate_Per_s;
  std::vector<double> m_TransitBileSalts_mM;

  // We want to start simulation time 0 (after stabilization) with the initial gut mass from the meal specified in the config file
  std::map<SELiquidSubstanceQuantity*, double> m_InitialSubstanceMasses_ug; // Used only during the final AtSteadyState

  bool m_ConsumeRate; // Some substance digestion rates are specified by the user, some by configuration,
  //if the stomach runs out of a substance with a user provided rate, we invalidate the rate (true)
  //if the rate is a configuration rate, we don't want to invalidate it if we run out of the substance (false)
  bool m_DecrementNutrients; // True  Signals that we decrement stomach nutrients when we digest them
  // We do not decrement stomach contents during stabilization
  double m_secretionRate_mL_Per_s;
  double m_dT_s;
};
}