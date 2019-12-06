/**************************************************************************************
C:opyright 2015 Applied Research Associates, Inc.
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

#include <biogears/cdm/system/equipment/Inhaler/SEInhaler.h>
#include <biogears/engine/Controller/BioGearsSystem.h>
#include <biogears/schema/biogears/BioGearsEquipment.hxx>

namespace biogears {
/**
* @brief 
* Generic inhaler for substance administration.
*/

class SEGasCompartment;
class SELiquidCompartment;
class SEGasCompartment;
class SELiquidSubstanceQuantity;
class BioGears;

class BIOGEARS_API Inhaler : public SEInhaler, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;

protected:
  Inhaler(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~Inhaler() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "Inhaler"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  bool Load(const CDM::BioGearsInhalerData& in);
  CDM::BioGearsInhalerData* Unload() const override;

protected:
  void Unload(CDM::BioGearsInhalerData& data) const;

  // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  // main driver function responsible for calling the various ECG functions:
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;

protected:
  void StateChange() override;
  void Administer();

  // Serializable member variables (Set in Initialize and in schema)

  // Stateless member variable (Set in SetUp())
  double m_dt_s;
  SEGasCompartment* m_Mouthpiece;
  SELiquidCompartment* m_AerosolMouthpiece;
  SEGasCompartment* m_AmbientEnv;
  SELiquidSubstanceQuantity* m_InhalerDrug;
};
}