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
namespace io {
  class BiogearsEquipment;
}
/**
* @brief 
* Generic ECG machine to assess the heart rhythm.
*/

class ECG : public SEElectroCardioGram, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;
  friend io::BiogearsEquipment;

protected:
  BIOGEARS_API static auto make_unique(BioGears& bg) -> std::unique_ptr<ECG>;
  BIOGEARS_API ECG(BioGears& bg);
  BioGears& m_data;

public:
  BIOGEARS_API virtual ~ECG() override;

  BIOGEARS_API static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  BIOGEARS_API static constexpr char const* const TypeTag() { return "ECG"; }
  BIOGEARS_API const char* classname() const override { return TypeTag(); }
  BIOGEARS_API size_t hash_code() const override { return TypeHash(); }

  BIOGEARS_API void Clear() override;

  // Set members to a stable homeostatic state
  BIOGEARS_API void Initialize() override;

  // Load a state
  virtual bool Load(const CDM::BioGearsElectroCardioGramData& in);
  virtual CDM::BioGearsElectroCardioGramData* Unload() const;

protected:
  virtual void Unload(CDM::BioGearsElectroCardioGramData& data) const;

  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  BIOGEARS_API void SetUp() override;

public:
  // main driver function responsible for calling the various ECG functions:
  BIOGEARS_API void PreProcess() override;
  BIOGEARS_API void Process() override;
  BIOGEARS_API void PostProcess() override;

protected:
  // Serializable member variables (Set in Initialize and in schema)
  SEScalarTime m_HeartRhythmTime;
  SEScalarTime m_HeartRhythmPeriod;
  SEElectroCardioGramInterpolator m_Waveforms;

  // Stateless member variable (Set in SetUp())
  double m_dt_s;
};
}