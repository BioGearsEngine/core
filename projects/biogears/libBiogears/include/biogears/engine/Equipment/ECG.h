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

#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolator.h>
#include <biogears/schema/biogears/BioGearsEnvironment.hxx>
#include <biogears/engine/Controller/BioGears.h>  
#include <biogears/engine/Controller/BioGearsSystem.h>

namespace biogears {
/**
* @brief 
* Generic ECG machine to assess the heart rhythm.
*/

class BIOGEARS_API ECG : public SEElectroCardioGram, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;

protected:
  static auto make_unique(BioGears& bg) -> std::unique_ptr<ECG>;
  ECG(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~ECG() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "ECG"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  virtual bool Load(const CDM::BioGearsElectroCardioGramData& in);
  virtual CDM::BioGearsElectroCardioGramData* Unload() const override;

protected:
  virtual void Unload(CDM::BioGearsElectroCardioGramData& data) const;

  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  // main driver function responsible for calling the various ECG functions:
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;

protected:
  // Serializable member variables (Set in Initialize and in schema)
  SEScalarTime m_heartRhythmTime;
  SEScalarTime m_heartRhythmPeriod;
  SEElectroCardioGramInterpolator m_interpolator;

  // Stateless member variable (Set in SetUp())
  double m_dt_s;
};
}