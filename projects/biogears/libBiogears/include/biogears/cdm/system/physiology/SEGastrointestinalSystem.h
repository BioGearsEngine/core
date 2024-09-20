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
#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/enums/SEPatientActionsEnums.h>
#include <biogears/cdm/enums/SESubstanceEnums.h>
#include <biogears/cdm/system/SESystem.h>
#include <biogears/cdm/enums/SEPhysiologyEnums.h>


#include <map>

namespace biogears {
class SEScalarMass;
class MassUnit;
class SEScalarVolumePerTime;
class VolumePerTimeUnit;
class SESubstance;
class SEDrugTransitState;

namespace io {
  class Physiology;
}
} // namespace biogears
#pragma warning(disable : 4661)

namespace std {
extern template class map<const biogears::SESubstance*, biogears::SEDrugTransitState*>;
}
#pragma warning(default : 4661)

namespace biogears {
class BIOGEARS_API SEGastrointestinalSystem : public SESystem {
  friend io::Physiology;

public:
  SEGastrointestinalSystem(Logger* logger);
  ~SEGastrointestinalSystem() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); } //! Hopefully this returns a unique ID for every type
  static constexpr char const* const TypeTag() { return "SEGastrointestinalSystem"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override; // Deletes all members

  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  bool operator==(SEGastrointestinalSystem const&) const;
  bool operator!=(SEGastrointestinalSystem const&) const;

  bool HasChymeAbsorptionRate() const;
  SEScalarVolumePerTime& GetChymeAbsorptionRate();
  double GetChymeAbsorptionRate(const VolumePerTimeUnit& unit) const;

  bool HasStomachContents() const;
  SENutrition& GetStomachContents();
  const SENutrition* GetStomachContents() const;
  void RemoveStomachContents();

  std::map<const SESubstance*, SEDrugTransitState*> GetDrugTransitStates();
  SEDrugTransitState* GetDrugTransitState(const SESubstance* sub);
  SEDrugTransitState* NewDrugTransitState(const SESubstance* sub);

  Tree<const char*> GetPhysiologyRequestGraph() const override;

protected:
  SEScalarVolumePerTime* m_ChymeAbsorptionRate;
  SENutrition* m_StomachContents;
  std::map<const SESubstance*, SEDrugTransitState*> m_DrugTransitStates;
};

class BIOGEARS_API SEDrugTransitState {
  friend io::Physiology;
public:
  SEDrugTransitState(const SESubstance& sub);
  ~SEDrugTransitState();

  virtual void Clear();

  bool Initialize(SEScalarMass& dose, SEOralAdministrationType route);

  std::vector<double> GetLumenSolidMasses(const MassUnit& unit);
  std::vector<double> GetLumenDissolvedMasses(const MassUnit& unit);
  std::vector<double> GetEnterocyteMasses(const MassUnit& unit);

  bool SetLumenSolidMasses(std::vector<double>& tsolid, const MassUnit& unit);
  bool SetLumenDissolvedMasses(std::vector<double>& tdis, const MassUnit& unit);
  bool SetEnterocyteMasses(std::vector<double>& esolid, const MassUnit& unit);

  void IncrementStomachDissolvedMass(double value, const MassUnit& unit);
  void IncrementStomachSolidMass(double value, const MassUnit& unit);

  double GetTotalSolidMassInLumen(const MassUnit& unit);
  double GetTotalDissolvedMassInLumen(const MassUnit& unit);
  double GetTotalMassInEnterocytes(const MassUnit& unit);

  SEScalarMass& GetTotalMassMetabolized();
  SEScalarMass& GetTotalMassExcreted();

  bool operator==(SEDrugTransitState const&) const;
  bool operator!=(SEDrugTransitState const&) const;

protected:

  const SESubstance* m_Substance;
  std::vector<SEScalarMass> m_LumenSolidMasses;
  std::vector<SEScalarMass> m_LumenDissolvedMasses;
  std::vector<SEScalarMass> m_EnterocyteMasses;
  SEScalarMass* m_TotalMassMetabolized;
  SEScalarMass* m_TotalMassExcreted;
  size_t m_NumTransitMasses;
};

}