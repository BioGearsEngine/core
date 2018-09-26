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
#include <biogears/cdm/system/SESystem.h>
#include <biogears/exports.h>
#include <biogears/schema/cdm/AnesthesiaActions.hxx>

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

class BIOGEARS_API SEAnesthesiaMachine : public SESystem {
protected:
  friend SEAnesthesiaMachineConfiguration;

public:
  SEAnesthesiaMachine(SESubstanceManager& substances);
  virtual ~SEAnesthesiaMachine();

  virtual void Clear();

  virtual bool Load(const CDM::AnesthesiaMachineData& in);
  virtual CDM::AnesthesiaMachineData* Unload() const;

protected:
  virtual void Unload(CDM::AnesthesiaMachineData& data) const;

  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange(){};
  virtual void Merge(const SEAnesthesiaMachine& from);
  virtual void ProcessConfiguration(const SEAnesthesiaMachineConfiguration& config);

public:
  bool Load(const std::string& file);

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual const std::map<CDM::enumAnesthesiaMachineEvent::value, bool>& GetEventStates() const { return m_EventState; }
  virtual void SetEvent(CDM::enumAnesthesiaMachineEvent::value state, bool active, const SEScalarTime& time);
  virtual bool IsEventActive(CDM::enumAnesthesiaMachineEvent::value state) const;
  virtual double GetEventDuration(CDM::enumAnesthesiaMachineEvent::value type, const TimeUnit& unit) const;
  virtual void UpdateEvents(const SEScalarTime& timeStep);
  /** @name ForwardEvents
  *  @brief - Set a callback class to invoke when any event changes
  *  @details - Note that the handler callback can and will be called in the middle of a time step
  *             So system and compartment objects may not be completely up to date when called.
  *             Use the PhysiologyEngineInterface::SetEventHandler to ensure that all engine
  *             data is up to date at the time the callback is invoked
  */
  virtual void ForwardEvents(SEEventHandler* handler);

  virtual CDM::enumAnesthesiaMachineConnection::value GetConnection() const;
  virtual void SetConnection(CDM::enumAnesthesiaMachineConnection::value c);
  virtual bool HasConnection() const;
  virtual void InvalidateConnection();

  virtual bool HasInletFlow() const;
  virtual SEScalarVolumePerTime& GetInletFlow();
  virtual double GetInletFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasInspiratoryExpiratoryRatio() const;
  virtual SEScalar& GetInspiratoryExpiratoryRatio();
  virtual double GetInspiratoryExpiratoryRatio() const;

  virtual bool HasOxygenFraction() const;
  virtual SEScalarFraction& GetOxygenFraction();
  virtual double GetOxygenFraction() const;

  virtual CDM::enumAnesthesiaMachineOxygenSource::value GetOxygenSource() const;
  virtual void SetOxygenSource(CDM::enumAnesthesiaMachineOxygenSource::value name);
  virtual bool HasOxygenSource() const;
  virtual void InvalidateOxygenSource();

  virtual bool HasPositiveEndExpiredPressure() const;
  virtual SEScalarPressure& GetPositiveEndExpiredPressure();
  virtual double GetPositiveEndExpiredPressure(const PressureUnit& unit) const;

  virtual CDM::enumAnesthesiaMachinePrimaryGas::value GetPrimaryGas() const;
  virtual void SetPrimaryGas(CDM::enumAnesthesiaMachinePrimaryGas::value name);
  virtual bool HasPrimaryGas() const;
  virtual void InvalidatePrimaryGas();

  virtual bool HasRespiratoryRate() const;
  virtual SEScalarFrequency& GetRespiratoryRate();
  virtual double GetRespiratoryRate(const FrequencyUnit& unit) const;

  virtual bool HasReliefValvePressure() const;
  virtual SEScalarPressure& GetReliefValvePressure();
  virtual double GetReliefValvePressure(const PressureUnit& unit) const;

  virtual bool HasVentilatorPressure() const;
  virtual SEScalarPressure& GetVentilatorPressure();
  virtual double GetVentilatorPressure(const PressureUnit& unit) const;

  virtual bool HasLeftChamber() const;
  virtual SEAnesthesiaMachineChamber& GetLeftChamber();
  virtual const SEAnesthesiaMachineChamber* GetLeftChamber() const;
  virtual void RemoveLeftChamber();

  virtual bool HasRightChamber() const;
  virtual SEAnesthesiaMachineChamber& GetRightChamber();
  virtual const SEAnesthesiaMachineChamber* GetRightChamber() const;
  virtual void RemoveRightChamber();

  virtual bool HasOxygenBottleOne() const;
  virtual SEAnesthesiaMachineOxygenBottle& GetOxygenBottleOne();
  virtual const SEAnesthesiaMachineOxygenBottle* GetOxygenBottleOne() const;
  virtual void RemoveOxygenBottleOne();

  virtual bool HasOxygenBottleTwo() const;
  virtual SEAnesthesiaMachineOxygenBottle& GetOxygenBottleTwo();
  virtual const SEAnesthesiaMachineOxygenBottle* GetOxygenBottleTwo() const;
  virtual void RemoveOxygenBottleTwo();

protected:
  std::stringstream m_ss;
  SEEventHandler* m_EventHandler;
  std::map<CDM::enumAnesthesiaMachineEvent::value, bool> m_EventState;
  std::map<CDM::enumAnesthesiaMachineEvent::value, double> m_EventDuration_s;

  CDM::enumAnesthesiaMachineConnection::value m_Connection;
  SEScalarVolumePerTime* m_InletFlow;
  SEScalar* m_InspiratoryExpiratoryRatio;
  SEScalarFraction* m_OxygenFraction;
  CDM::enumAnesthesiaMachineOxygenSource::value m_OxygenSource;
  SEScalarPressure* m_PositiveEndExpiredPressure;
  CDM::enumAnesthesiaMachinePrimaryGas::value m_PrimaryGas;
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