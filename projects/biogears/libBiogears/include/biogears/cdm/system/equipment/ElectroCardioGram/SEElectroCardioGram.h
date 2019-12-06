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

class BIOGEARS_API SEElectroCardioGram : public SESystem {
protected:
public:
  SEElectroCardioGram(Logger* logger);
  ~SEElectroCardioGram() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "SEElectroCardioGram"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  bool Load(const CDM::ElectroCardioGramData& in);
  CDM::ElectroCardioGramData* Unload() const override;
  Tree<const char*> GetPhysiologyRequestGraph() const override;
protected:
  void Unload(CDM::ElectroCardioGramData& data) const;

public:
  bool HasLead1ElectricPotential() const;
  SEScalarElectricPotential& GetLead1ElectricPotential();
  double GetLead1ElectricPotential(const ElectricPotentialUnit& unit) const;

  bool HasLead2ElectricPotential() const;
  SEScalarElectricPotential& GetLead2ElectricPotential();
  double GetLead2ElectricPotential(const ElectricPotentialUnit& unit) const;

  bool HasLead3ElectricPotential() const;
  SEScalarElectricPotential& GetLead3ElectricPotential();
  double GetLead3ElectricPotential(const ElectricPotentialUnit& unit) const;

  bool HasLead4ElectricPotential() const;
  SEScalarElectricPotential& GetLead4ElectricPotential();
  double GetLead4ElectricPotential(const ElectricPotentialUnit& unit) const;

  bool HasLead5ElectricPotential() const;
  SEScalarElectricPotential& GetLead5ElectricPotential();
  double GetLead5ElectricPotential(const ElectricPotentialUnit& unit) const;

  bool HasLead6ElectricPotential() const;
  SEScalarElectricPotential& GetLead6ElectricPotential();
  double GetLead6ElectricPotential(const ElectricPotentialUnit& unit) const;

  bool HasLead7ElectricPotential() const;
  SEScalarElectricPotential& GetLead7ElectricPotential();
  double GetLead7ElectricPotential(const ElectricPotentialUnit& unit) const;

  bool HasLead8ElectricPotential() const;
  SEScalarElectricPotential& GetLead8ElectricPotential();
  double GetLead8ElectricPotential(const ElectricPotentialUnit& unit) const;

  bool HasLead9ElectricPotential() const;
  SEScalarElectricPotential& GetLead9ElectricPotential();
  double GetLead9ElectricPotential(const ElectricPotentialUnit& unit) const;

  bool HasLead10ElectricPotential() const;
  SEScalarElectricPotential& GetLead10ElectricPotential();
  double GetLead10ElectricPotential(const ElectricPotentialUnit& unit) const;

  bool HasLead11ElectricPotential() const;
  SEScalarElectricPotential& GetLead11ElectricPotential();
  double GetLead11ElectricPotential(const ElectricPotentialUnit& unit) const;

  bool HasLead12ElectricPotential() const;
  SEScalarElectricPotential& GetLead12ElectricPotential();
  double GetLead12ElectricPotential(const ElectricPotentialUnit& unit) const;

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