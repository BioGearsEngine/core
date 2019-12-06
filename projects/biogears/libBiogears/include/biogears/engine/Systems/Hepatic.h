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

#include <biogears/cdm/system/physiology/SEHepaticSystem.h>
#include <biogears/engine/Controller/BioGearsSystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SETissueSystem;
class SELiquidSubstanceQuantity;
class SELiquidSubstanceQuantity;
class SESubstance;
class SETissueSystem;
class SEEnergySystem;
class SEPatient;
class BioGears;

/**
 * @copydoc Physiology_HepaticystemData
 */
class BIOGEARS_API Hepatic : public SEHepaticSystem, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;

protected:
  Hepatic(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~Hepatic() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "Hepatic"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  virtual bool Load(const CDM::BioGearsHepaticSystemData& in);
  virtual CDM::BioGearsHepaticSystemData* Unload() const override;

protected:
  virtual void Unload(CDM::BioGearsHepaticSystemData& data) const;

  // Set pointers and other member variables common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  void AtSteadyState() override;
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;
  static double CalculateRelativeHormoneChange(double insulinSetPoint_pmol_Per_L, double glucagonSetPoint_pg_Per_mL, SELiquidSubstanceQuantity* currentInsulin, SELiquidSubstanceQuantity* currentGlucagon, BioGears& m_data);

protected:
  //Pre-Process methods
  void ProduceAlbumin(double duration_s);

  //Process methods
  void Glycogenesis();
  void Glycogenolysis();
  void Lipogenesis();
  void Gluconeogenesis();

  //Post-Process methods

  // Serializable member variables (Set in Initialize and in schema)

  // Stateless member variable (Set in SetUp())
  std::stringstream m_ss;
  double m_dt_s;
  double m_maxLiverGlycogen_g;
  double m_maxMuscleGlycogen_g;
  double m_AlbuminProdutionRate_g_Per_s;
  SELiquidSubstanceQuantity* m_liverInsulin = nullptr;
  SELiquidSubstanceQuantity* m_liverGlucagon = nullptr;
  SELiquidSubstanceQuantity* m_liverVascularGlucose = nullptr;
  SELiquidSubstanceQuantity* m_liverExtracellularGlucose = nullptr;
  SELiquidSubstanceQuantity* m_liverExtracellularAA = nullptr;
  SELiquidSubstanceQuantity* m_liverExtracellularTAG = nullptr;
  SELiquidSubstanceQuantity* m_liverExtracellularUrea = nullptr;
  SELiquidSubstanceQuantity* m_liverExtracellularLactate = nullptr;
  SELiquidSubstanceQuantity* m_liverExtracellularO2 = nullptr;
  SELiquidSubstanceQuantity* m_liverExtracellularCO2 = nullptr;
  SELiquidSubstanceQuantity* m_liverExtracellularKetones = nullptr;
  SELiquidSubstanceQuantity* m_muscleInsulin = nullptr;
  SELiquidSubstanceQuantity* m_muscleGlucagon = nullptr;
  SELiquidSubstanceQuantity* m_muscleVascularGlucose = nullptr;
  SELiquidSubstanceQuantity* m_LiverTissueAlbumin = nullptr;
  SESubstance* m_Glucagon;
  SESubstance* m_Insulin;
  SESubstance* m_Glucose;
  SESubstance* m_AminoAcids;
  SESubstance* m_Triacylglycerol;
  SESubstance* m_Urea;
  SESubstance* m_Lactate;
  SESubstance* m_O2;
  SESubstance* m_CO2;
  SESubstance* m_Ketones;
  SESubstance* m_Albumin;
  SETissueSystem* m_tsu;
  SEEnergySystem* m_energy;
  SEPatient* m_Patient;
};
}