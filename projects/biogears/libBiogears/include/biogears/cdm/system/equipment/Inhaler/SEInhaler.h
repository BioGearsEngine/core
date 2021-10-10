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
#include <biogears/schema/biogears/BioGearsEquipment.hxx>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class Serializer;
class SESubstance;
class SESubstanceManager;
class SEInhalerConfiguration;
class SEScalarVolume;
class VolumeUnit;
class SEScalarMass;
class MassUnit;
class SEScalarFraction;

namespace io {
  class Inhaler;
}

class BIOGEARS_API SEInhaler : public SESystem {
  friend io::Inhaler;

protected:
public:
  SEInhaler(SESubstanceManager& substances);
  ~SEInhaler() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const* const TypeTag() { return "SEInhaler"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  /** @name GetScalar
  *   @brief - A reflextion type call that will return the Scalar associated
  *            with the string. ex. GetScalar("Hematocrit") will return the
  *            SEScalarPercent object associated with Hematocrit
  *   @details - Note this is an expensive call as it will string compare many times
  *              This is best used, and intended for, you to dynamically prepopulate
  *              a mapping data structure that will help access what you need
  */
  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  bool Load(const CDM::InhalerData& in);
  CDM::InhalerData* Unload() const override;
  Tree<const char*> GetPhysiologyRequestGraph() const override;

protected:
  void Unload(CDM::InhalerData& data) const;

  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange() { } //TODO: Refactor the need for this definiiton;
  void Merge(const SEInhaler& from);
  void ProcessConfiguration(const SEInhalerConfiguration& config);

public:
  bool Load(const std::string& file);

  CDM::enumOnOff::value GetState() const;
  void SetState(CDM::enumOnOff::value name);
  bool HasState() const;
  void InvalidateState();

  bool HasMeteredDose() const;
  SEScalarMass& GetMeteredDose();
  double GetMeteredDose(const MassUnit& unit) const;

  bool HasNozzleLoss() const;
  SEScalarFraction& GetNozzleLoss();
  double GetNozzleLoss() const;

  bool HasSpacerVolume() const;
  SEScalarVolume& GetSpacerVolume();
  double GetSpacerVolume(const VolumeUnit& unit) const;

  bool HasSubstance() const;
  void SetSubstance(const SESubstance* sub);
  SESubstance* GetSubstance() const;

protected:
  std::stringstream m_ss;

  CDM::enumOnOff::value m_State;
  SEScalarMass* m_MeteredDose;
  SEScalarFraction* m_NozzleLoss;
  SEScalarVolume* m_SpacerVolume;
  const SESubstance* m_Substance;

  SESubstanceManager& m_Substances;
};
}