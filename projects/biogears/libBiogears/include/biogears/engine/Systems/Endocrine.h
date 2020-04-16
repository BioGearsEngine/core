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
#include <biogears/cdm/system/physiology/SEEndocrineSystem.h>
#include <biogears/engine/Controller/BioGearsSystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SELiquidSubstanceQuantity;
class BioGears;
/**
 * @brief @copydoc Physiology_EndocrineSystemData
 * @details
 * The BioGears® endocrine system is a rudimentary system with only one stimulus (increased carbon dioxide partial pressure in the blood stream)
 * and two hormones (epinephrine and norepinephrine). The release of the hormones in response to the stimuli to represent the response of the sympathetic nervous system.
 * In the future, additional stimuli and additional hormones will be added.
 */
class BIOGEARS_API Endocrine : public SEEndocrineSystem, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;

protected:
  static auto make_unique(BioGears& bg) -> std::unique_ptr<Endocrine>;
  Endocrine(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~Endocrine() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "Endocrine"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  virtual bool Load(const CDM::BioGearsEndocrineSystemData& in);
  virtual CDM::BioGearsEndocrineSystemData* Unload() const override;

protected:
  virtual void Unload(CDM::BioGearsEndocrineSystemData& data) const;

  // Set pointers and other member variables common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  void AtSteadyState() override;
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;

private:
  void SynthesizeInsulin();
  void SynthesizeGlucagon();
  void ReleaseEpinephrine();

protected:  
  //Override
  void ProcessOverride();
  void OverrideControlLoop();

private:
  // Serializable member variables (Set in Initialize and in schema)

  // Stateless member variable (Set in SetUp())
  double m_dt_s;
  double m_insulinMolarMass_g_Per_mol;
  double m_averageBiologicalDebt;
  double m_glucagonMolarMass_g_Per_mol;
  SELiquidSubstanceQuantity* m_aortaGlucose = nullptr;
  SELiquidSubstanceQuantity* m_aortaEpinephrine = nullptr;
  SELiquidSubstanceQuantity* m_rKidneyEpinephrine = nullptr;
  SELiquidSubstanceQuantity* m_lKidneyEpinephrine = nullptr;
  SELiquidSubstanceQuantity* m_splanchnicInsulin = nullptr;
  SELiquidSubstanceQuantity* m_splanchnicGlucagon = nullptr;
};
}