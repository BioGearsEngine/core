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
#include <biogears/schema/cdm/Environment.hxx>

namespace biogears {
class SESubstance;
class SESubstanceFraction;
class SESubstanceManager;
class SEEnvironmentalConditions;
class SEEnvironmentChange;
class SEInitialEnvironment;
class SEActiveHeating;
class SEActiveCooling;
class SEAppliedTemperature;

class SEScalarPower;
class PowerUnit;
class SEScalarHeatConductancePerArea;
class HeatConductancePerAreaUnit;

class BIOGEARS_API SEEnvironment : public SESystem {
public:
  SEEnvironment(SESubstanceManager& substances);
  ~SEEnvironment() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const * const  TypeTag() { return "SEEnvironment"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  bool Load(const CDM::EnvironmentData& in);
  bool Load(const char* patientFile);
  bool Load(const std::string& patientFile);

  CDM::EnvironmentData* Unload() const override;
  Tree<const char*> GetPhysiologyRequestGraph() const override;
  /** @name ProcessChange
  * @brief - Will change this class as directed by the Action
  */
  bool ProcessChange(const SEEnvironmentChange& action);

  /** @name ProcessChange
  * @brief - Will change this class as directed by the Condition
  */
  bool ProcessChange(const SEInitialEnvironment& change);

  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange() = 0;

public:
  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  std::string GetName() const;
  const char* GetName_cStr() const;
  void SetName(const char* name);
  void SetName(const std::string& name);
  bool HasName() const;
  void InvalidateName();

  bool HasActiveHeating() const;
  SEActiveHeating& GetActiveHeating();
  const SEActiveHeating* GetActiveHeating() const;
  void RemoveActiveHeating();

  bool HasActiveCooling() const;
  SEActiveCooling& GetActiveCooling();
  const SEActiveCooling* GetActiveCooling() const;
  void RemoveActiveCooling();

  bool HasAppliedTemperature() const;
  SEAppliedTemperature& GetAppliedTemperature();
  const SEAppliedTemperature* GetAppliedTemperature() const;
  void RemoveAppliedTemperature();

  bool HasConditions() const;
  SEEnvironmentalConditions& GetConditions();
  const SEEnvironmentalConditions* GetConditions() const;
  void RemoveConditions();

  bool HasConvectiveHeatLoss() const;
  SEScalarPower& GetConvectiveHeatLoss();
  double GetConvectiveHeatLoss(const PowerUnit& unit) const;

  bool HasConvectiveHeatTranferCoefficient() const;
  SEScalarHeatConductancePerArea& GetConvectiveHeatTranferCoefficient();
  double GetConvectiveHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const;

  bool HasEvaporativeHeatLoss() const;
  SEScalarPower& GetEvaporativeHeatLoss();
  double GetEvaporativeHeatLoss(const PowerUnit& unit) const;

  bool HasEvaporativeHeatTranferCoefficient() const;
  SEScalarHeatConductancePerArea& GetEvaporativeHeatTranferCoefficient();
  double GetEvaporativeHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const;

  bool HasRadiativeHeatLoss() const;
  SEScalarPower& GetRadiativeHeatLoss();
  double GetRadiativeHeatLoss(const PowerUnit& unit) const;

  bool HasRadiativeHeatTranferCoefficient() const;
  SEScalarHeatConductancePerArea& GetRadiativeHeatTranferCoefficient();
  double GetRadiativeHeatTranferCoefficient(const HeatConductancePerAreaUnit& unit) const;

  bool HasRespirationHeatLoss() const;
  SEScalarPower& GetRespirationHeatLoss();
  double GetRespirationHeatLoss(const PowerUnit& unit) const;

  bool HasSkinHeatLoss() const;
  SEScalarPower& GetSkinHeatLoss();
  double GetSkinHeatLoss(const PowerUnit& unit) const;

protected:
  void Unload(CDM::EnvironmentData& data) const;

protected:
  std::string m_Name;
  SEScalarPower* m_ConvectiveHeatLoss;
  SEScalarHeatConductancePerArea* m_ConvectiveHeatTranferCoefficient;
  SEScalarPower* m_EvaporativeHeatLoss;
  SEScalarHeatConductancePerArea* m_EvaporativeHeatTranferCoefficient;
  SEScalarPower* m_RadiativeHeatLoss;
  SEScalarHeatConductancePerArea* m_RadiativeHeatTranferCoefficient;
  SEScalarPower* m_RespirationHeatLoss;
  SEScalarPower* m_SkinHeatLoss;

  SEActiveHeating* m_ActiveHeating;
  SEActiveCooling* m_ActiveCooling;
  SEAppliedTemperature* m_AppliedTemperature;
  SEEnvironmentalConditions* m_Conditions;

  SESubstanceManager& m_Substances;
};
}