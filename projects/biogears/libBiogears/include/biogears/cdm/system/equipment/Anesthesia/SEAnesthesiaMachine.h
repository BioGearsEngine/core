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
#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/system/SESystem.h>
#include <biogears/cdm/enums/SEAnesthesiaEnums.h>

namespace biogears {

class SEEventHandler;
class SESubstanceManager;
class SEAnesthesiaMachineChamber;
class SEAnesthesiaMachineOxygenBottle;
class SEAnesthesiaMachineConfiguration;
class Serializer;
class TimeUnit;
class SEScalarVolumePerTime;
class VolumePerTimeUnit;
class SEScalarPressure;
class PressureUnit;
class SEScalarFrequency;
class FrequencyUnit;
class SEScalarFraction;

namespace io {
  class Anesthesia;
}
} // namespace biogears
#pragma warning(disable : 4661)

namespace std {
extern template class map<biogears::SEAnesthesiaMachineEvent, bool>;
extern template class map<biogears::SEAnesthesiaMachineEvent, double>;
}
#pragma warning(default : 4661)

namespace biogears {
class BIOGEARS_API SEAnesthesiaMachine : public SESystem {
  friend SEAnesthesiaMachineConfiguration;
  friend io::Anesthesia;

public:
  SEAnesthesiaMachine(SESubstanceManager& substances);
  ~SEAnesthesiaMachine() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const* const TypeTag() { return "SEAnesthesiaMachine"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  Tree<const char*> GetPhysiologyRequestGraph() const override;


  /** @name StateChange
   *   @brief - This method is called when ever there is a state change
   *            Specically a new file has been loaded, configuration action, or the system reset
   *            Engine specific methodology can then update their logic.
   */
  virtual void StateChange();
  void Merge(const SEAnesthesiaMachine& from);
  void ProcessConfiguration(const SEAnesthesiaMachineConfiguration& config);


  bool Load(const std::string& file);

  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  const std::map<SEAnesthesiaMachineEvent, bool>& GetEventStates() const { return m_EventState; }
  void SetEvent(SEAnesthesiaMachineEvent state, bool active, const SEScalarTime& time);
  bool IsEventActive(SEAnesthesiaMachineEvent state) const;
  double GetEventDuration(SEAnesthesiaMachineEvent type, const TimeUnit& unit) const;
  void UpdateEvents(const SEScalarTime& timeStep);
  /** @name ForwardEvents
   *  @brief - Set a callback class to invoke when any event changes
   *  @details - Note that the handler callback can and will be called in the middle of a time step
   *             So system and compartment objects may not be completely up to date when called.
   *             Use the PhysiologyEngineInterface::SetEventHandler to ensure that all engine
   *             data is up to date at the time the callback is invoked
   */
  void ForwardEvents(SEEventHandler* handler);

  virtual SEAnesthesiaMachineConnection GetConnection() const;
  virtual void SetConnection(SEAnesthesiaMachineConnection c);
  virtual bool HasConnection() const;
  virtual void InvalidateConnection();

  bool HasInletFlow() const;
  SEScalarVolumePerTime& GetInletFlow();
  double GetInletFlow(const VolumePerTimeUnit& unit) const;

  bool HasInspiratoryExpiratoryRatio() const;
  SEScalar& GetInspiratoryExpiratoryRatio();
  double GetInspiratoryExpiratoryRatio() const;

  bool HasOxygenFraction() const;
  SEScalarFraction& GetOxygenFraction();
  double GetOxygenFraction() const;

  SEAnesthesiaMachineOxygenSource GetOxygenSource() const;
  void SetOxygenSource(SEAnesthesiaMachineOxygenSource name);
  bool HasOxygenSource() const;
  void InvalidateOxygenSource();

  bool HasPositiveEndExpiredPressure() const;
  SEScalarPressure& GetPositiveEndExpiredPressure();
  double GetPositiveEndExpiredPressure(const PressureUnit& unit) const;

  SEAnesthesiaMachinePrimaryGas GetPrimaryGas() const;
  void SetPrimaryGas(SEAnesthesiaMachinePrimaryGas name);
  bool HasPrimaryGas() const;
  void InvalidatePrimaryGas();

  bool HasRespiratoryRate() const;
  SEScalarFrequency& GetRespiratoryRate();
  double GetRespiratoryRate(const FrequencyUnit& unit) const;

  bool HasReliefValvePressure() const;
  SEScalarPressure& GetReliefValvePressure();
  double GetReliefValvePressure(const PressureUnit& unit) const;

  bool HasVentilatorPressure() const;
  SEScalarPressure& GetVentilatorPressure();
  double GetVentilatorPressure(const PressureUnit& unit) const;

  bool HasLeftChamber() const;
  SEAnesthesiaMachineChamber& GetLeftChamber();
  const SEAnesthesiaMachineChamber* GetLeftChamber() const;
  void RemoveLeftChamber();

  bool HasRightChamber() const;
  SEAnesthesiaMachineChamber& GetRightChamber();
  const SEAnesthesiaMachineChamber* GetRightChamber() const;
  void RemoveRightChamber();

  bool HasOxygenBottleOne() const;
  SEAnesthesiaMachineOxygenBottle& GetOxygenBottleOne();
  const SEAnesthesiaMachineOxygenBottle* GetOxygenBottleOne() const;
  void RemoveOxygenBottleOne();

  bool HasOxygenBottleTwo() const;
  SEAnesthesiaMachineOxygenBottle& GetOxygenBottleTwo();
  const SEAnesthesiaMachineOxygenBottle* GetOxygenBottleTwo() const;
  void RemoveOxygenBottleTwo();

  bool operator==(SEAnesthesiaMachine const& rhs) const;
  bool operator!=(SEAnesthesiaMachine const& rhs) const;

protected:
  std::stringstream m_ss;
  SEEventHandler* m_EventHandler;
  std::map<SEAnesthesiaMachineEvent, bool> m_EventState;
  std::map<SEAnesthesiaMachineEvent, double> m_EventDuration_s;

  SEAnesthesiaMachineConnection m_Connection;
  SEScalarVolumePerTime* m_InletFlow;
  SEScalar* m_InspiratoryExpiratoryRatio;
  SEScalarFraction* m_OxygenFraction;
  SEAnesthesiaMachineOxygenSource m_OxygenSource;
  SEScalarPressure* m_PositiveEndExpiredPressure;
  SEAnesthesiaMachinePrimaryGas m_PrimaryGas;
  SEScalarFrequency* m_RespiratoryRate;
  SEScalarPressure* m_ReliefValvePressure;

  SEScalarPressure* m_VentilatorPressure;

  SEAnesthesiaMachineChamber* m_LeftChamber;
  SEAnesthesiaMachineChamber* m_RightChamber;

  SEAnesthesiaMachineOxygenBottle* m_OxygenBottleOne;
  SEAnesthesiaMachineOxygenBottle* m_OxygenBottleTwo;

  SESubstanceManager& m_Substances;
};
}