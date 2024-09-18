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
#include <biogears/cdm/system/SESystem.h>
#include <biogears/schema/cdm/ElectroCardioGram.hxx>

namespace biogears {
class SEScalarElectricPotential;
class ElectricPotentialUnit;
namespace io {
  class ElectroCardioGram;
}
class SEElectroCardioGram : public SESystem {
  friend io::ElectroCardioGram;

protected:
public:
  BIOGEARS_API SEElectroCardioGram(Logger* logger);
  BIOGEARS_API ~SEElectroCardioGram() override;

  BIOGEARS_API static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  BIOGEARS_API static constexpr char const* const TypeTag() { return "SEElectroCardioGram"; }
  BIOGEARS_API const char* classname() const override { return TypeTag(); }
  BIOGEARS_API size_t hash_code() const override { return TypeHash(); }

  BIOGEARS_API void Clear() override;

  BIOGEARS_API const SEScalar* GetScalar(const char* name) override;
  BIOGEARS_API const SEScalar* GetScalar(const std::string& name) override;

  BIOGEARS_API bool Load(const CDM::ElectroCardioGramData& in);
  BIOGEARS_API CDM::ElectroCardioGramData* Unload() const;
  BIOGEARS_API Tree<const char*> GetPhysiologyRequestGraph() const override;

  BIOGEARS_API bool operator==(SEElectroCardioGram const&) const;
  BIOGEARS_API bool operator!=(SEElectroCardioGram const&) const;

protected:
  BIOGEARS_API void Unload(CDM::ElectroCardioGramData& data) const;

public:
  BIOGEARS_API bool HasLead1ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead1ElectricPotential();
  BIOGEARS_API double GetLead1ElectricPotential(const ElectricPotentialUnit& unit) const;

  BIOGEARS_API bool HasLead2ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead2ElectricPotential();
  BIOGEARS_API double GetLead2ElectricPotential(const ElectricPotentialUnit& unit) const;

  BIOGEARS_API bool HasLead3ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead3ElectricPotential();
  BIOGEARS_API double GetLead3ElectricPotential(const ElectricPotentialUnit& unit) const;

  BIOGEARS_API bool HasLead4ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead4ElectricPotential();
  BIOGEARS_API double GetLead4ElectricPotential(const ElectricPotentialUnit& unit) const;

  BIOGEARS_API bool HasLead5ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead5ElectricPotential();
  BIOGEARS_API double GetLead5ElectricPotential(const ElectricPotentialUnit& unit) const;

  BIOGEARS_API bool HasLead6ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead6ElectricPotential();
  BIOGEARS_API double GetLead6ElectricPotential(const ElectricPotentialUnit& unit) const;

  BIOGEARS_API bool HasLead7ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead7ElectricPotential();
  BIOGEARS_API double GetLead7ElectricPotential(const ElectricPotentialUnit& unit) const;

  BIOGEARS_API bool HasLead8ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead8ElectricPotential();
  BIOGEARS_API double GetLead8ElectricPotential(const ElectricPotentialUnit& unit) const;

  BIOGEARS_API bool HasLead9ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead9ElectricPotential();
  BIOGEARS_API double GetLead9ElectricPotential(const ElectricPotentialUnit& unit) const;

  BIOGEARS_API bool HasLead10ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead10ElectricPotential();
  BIOGEARS_API double GetLead10ElectricPotential(const ElectricPotentialUnit& unit) const;

  BIOGEARS_API bool HasLead11ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead11ElectricPotential();
  BIOGEARS_API double GetLead11ElectricPotential(const ElectricPotentialUnit& unit) const;

  BIOGEARS_API bool HasLead12ElectricPotential() const;
  BIOGEARS_API SEScalarElectricPotential& GetLead12ElectricPotential();
  BIOGEARS_API double GetLead12ElectricPotential(const ElectricPotentialUnit& unit) const;

protected:
  SEScalarElectricPotential* m_Lead1ElectricPotential;
  SEScalarElectricPotential* m_Lead2ElectricPotential;
  SEScalarElectricPotential* m_Lead3ElectricPotential;
  SEScalarElectricPotential* m_Lead4ElectricPotential;
  SEScalarElectricPotential* m_Lead5ElectricPotential;
  SEScalarElectricPotential* m_Lead6ElectricPotential;
  SEScalarElectricPotential* m_Lead7ElectricPotential;
  SEScalarElectricPotential* m_Lead8ElectricPotential;
  SEScalarElectricPotential* m_Lead9ElectricPotential;
  SEScalarElectricPotential* m_Lead10ElectricPotential;
  SEScalarElectricPotential* m_Lead11ElectricPotential;
  SEScalarElectricPotential* m_Lead12ElectricPotential;

  std::stringstream m_ss;
};
}