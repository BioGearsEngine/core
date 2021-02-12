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
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SEScalarAmountPerTime;
class AmountPerTimeUnit;
class SEScalarMassPerTime;
class MassPerTimeUnit;
namespace io {
  class Physiology;
}
class BIOGEARS_API SEHepaticSystem : public SESystem {
  friend io::Physiology;

public:
  SEHepaticSystem(Logger* logger);
  ~SEHepaticSystem() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const* const TypeTag() { return "SEHepaticSystem"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override; // Deletes all members

  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  bool Load(const CDM::HepaticSystemData& in);
  CDM::HepaticSystemData* Unload() const override;

  Tree<const char*> GetPhysiologyRequestGraph() const override;

protected:
  void Unload(CDM::HepaticSystemData& data) const;

public:
  bool HasKetoneProductionRate() const;
  SEScalarAmountPerTime& GetKetoneProductionRate();
  double GetKetoneProductionRate(const AmountPerTimeUnit& unit) const;

  bool HasHepaticGluconeogenesisRate() const;
  SEScalarMassPerTime& GetHepaticGluconeogenesisRate();
  double GetHepaticGluconeogenesisRate(const MassPerTimeUnit& unit) const;

protected:
  SEScalarAmountPerTime* m_KetoneProductionRate;
  SEScalarMassPerTime* m_HepaticGluconeogenesisRate;
};
}