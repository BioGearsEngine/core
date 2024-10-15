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
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>

#include "io/cdm/Anesthesia.h"
#include "io/cdm/AnesthesiaActions.h"

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/container/Tree.tci.h>
#include <biogears/io/io-manager.h>

namespace std {
template class map<biogears::SEAnesthesiaMachineEvent, bool>;
template class map<biogears::SEAnesthesiaMachineEvent, double>;
}
namespace biogears {

SEAnesthesiaMachine::SEAnesthesiaMachine(SESubstanceManager& substances)
  : SESystem(substances.GetLogger())
  , m_Substances(substances)
{
  m_Connection = (SEAnesthesiaMachineConnection)-1;
  m_InletFlow = nullptr;
  m_InspiratoryExpiratoryRatio = nullptr;
  m_OxygenFraction = nullptr;
  m_OxygenSource = (SEAnesthesiaMachineOxygenSource)-1;
  m_PositiveEndExpiredPressure = nullptr;
  m_PrimaryGas = (SEAnesthesiaMachinePrimaryGas)-1;
  m_RespiratoryRate = nullptr;
  m_ReliefValvePressure = nullptr;
  m_VentilatorPressure = nullptr;
  m_LeftChamber = nullptr;
  m_RightChamber = nullptr;
  m_OxygenBottleOne = nullptr;
  m_OxygenBottleTwo = nullptr;
  m_EventHandler = nullptr;
}
//-----------------------------------------------------------------------------
SEAnesthesiaMachine::~SEAnesthesiaMachine()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::Clear()
{
  SESystem::Clear();

  m_EventHandler = nullptr;
  m_EventState.clear();
  m_EventDuration_s.clear();
  m_Connection = (SEAnesthesiaMachineConnection)-1;
  SAFE_DELETE(m_InletFlow);
  SAFE_DELETE(m_InspiratoryExpiratoryRatio);
  SAFE_DELETE(m_OxygenFraction);
  m_OxygenSource = (SEAnesthesiaMachineOxygenSource)-1;
  SAFE_DELETE(m_PositiveEndExpiredPressure);
  m_PrimaryGas = (SEAnesthesiaMachinePrimaryGas)-1;
  SAFE_DELETE(m_RespiratoryRate);
  SAFE_DELETE(m_ReliefValvePressure);
  SAFE_DELETE(m_VentilatorPressure);
  SAFE_DELETE(m_LeftChamber);
  SAFE_DELETE(m_RightChamber);
  SAFE_DELETE(m_OxygenBottleOne);
  SAFE_DELETE(m_OxygenBottleTwo);
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::StateChange() { }
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::Merge(const SEAnesthesiaMachine& from)
{
  if (from.HasConnection())
    SetConnection(from.m_Connection);
  // Copy EventHandler? I don't think so...
  for (auto e : from.m_EventState)
    m_EventState[e.first] = e.second;
  for (auto e : from.m_EventDuration_s)
    m_EventDuration_s[e.first] = e.second;
  COPY_PROPERTY(InletFlow);
  COPY_PROPERTY(InspiratoryExpiratoryRatio);
  COPY_PROPERTY(OxygenFraction);
  if (from.HasOxygenSource())
    SetOxygenSource(from.m_OxygenSource);
  COPY_PROPERTY(PositiveEndExpiredPressure);
  if (from.HasPrimaryGas())
    SetPrimaryGas(from.m_PrimaryGas);
  COPY_PROPERTY(RespiratoryRate);
  COPY_PROPERTY(ReliefValvePressure);
  COPY_PROPERTY(VentilatorPressure);

  MERGE_CHILD(LeftChamber);
  MERGE_CHILD(RightChamber);

  MERGE_CHILD(OxygenBottleOne);
  MERGE_CHILD(OxygenBottleTwo);
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::ProcessConfiguration(const SEAnesthesiaMachineConfiguration& config)
{
  if (config.HasConfiguration())
    Merge(*config.GetConfiguration());
  else if (config.HasConfigurationFile())
    if (!Load(config.GetConfigurationFile())) // Does NOT merge file in data, Should we ?
      Error("Unable to load configuration file", "SEAnesthesiaMachine::ProcessConfiguration");
  StateChange();
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::Load(const std::string& file)
{
  CDM::AnesthesiaMachineData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  auto io = m_Logger->GetIoManager().lock();
  auto possible_path = io->find_resource_file(file.c_str());
  if (possible_path.empty()) {
#ifdef BIOGEARS_IO_PRESENT
    size_t content_size;
    auto content = io->get_embedded_resource_file(file.c_str(), content_size);
    data = Serializer::ReadBuffer((XMLByte*)content, content_size, m_Logger);
#endif
  } else {
    data = Serializer::ReadFile(possible_path, m_Logger);
  }

  pData = dynamic_cast<CDM::AnesthesiaMachineData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Anesthesia Machine file could not be read : " << file << std::endl;
    Error(ss);
    return false;
  }
  io::Anesthesia::UnMarshall(*pData, *this);
  return true;
}



//-----------------------------------------------------------------------------
const SEScalar* SEAnesthesiaMachine::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-----------------------------------------------------------------------------
const SEScalar* SEAnesthesiaMachine::GetScalar(const std::string& name)
{
  if (name == "InletFlow")
    return &GetInletFlow();
  if (name == "InspiratoryExpiratoryRatio")
    return &GetInspiratoryExpiratoryRatio();
  if (name == "OxygenFraction")
    return &GetOxygenFraction();
  if (name == "PositiveEndExpiredPressure")
    return &GetPositiveEndExpiredPressure();
  if (name == "ReliefValvePressure")
    return &GetReliefValvePressure();
  if (name == "RespiratoryRate")
    return &GetRespiratoryRate();
  if (name == "VentilatorPressure")
    return &GetVentilatorPressure();

  size_t split = name.find('-');
  if (split != name.npos) {
    std::string child = name.substr(0, split);
    std::string prop = name.substr(split + 1, name.npos);
    if (child == "LeftChamber")
      return GetLeftChamber().GetScalar(prop);
    if (child == "RightChamber")
      return GetRightChamber().GetScalar(prop);
    if (child == "OxygenBottleOne")
      return GetOxygenBottleOne().GetScalar(prop);
    if (child == "OxygenBottleTwo")
      return GetOxygenBottleTwo().GetScalar(prop);
  }

  return nullptr;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::SetEvent(SEAnesthesiaMachineEvent type, bool active, const SEScalarTime& time)
{
  bool b = false; // Default is off
  if (m_EventState.find(type) != m_EventState.end())
    b = m_EventState[type];
  if (b == active)
    return; // No Change
  if (active != b) {
    m_ss.str("");
    m_ss << "[Event] " << time << ", ";
    if (active) {
      switch (type) {
      case SEAnesthesiaMachineEvent::OxygenBottle1Exhausted:
        m_ss << "Oxygen Bottle 1 has been exhausted";
        break;
      case SEAnesthesiaMachineEvent::OxygenBottle2Exhausted:
        m_ss << "Oxygen Bottle 2 has been exhausted";
        break;
      case SEAnesthesiaMachineEvent::ReliefValveActive:
        m_ss << "Relief valve active - pressure exceeded";
        break;
      default:
        m_ss << "Anesthesia Machine Event On : " << type; 
      }
    } else {
      switch (type) {
      case SEAnesthesiaMachineEvent::OxygenBottle1Exhausted:
        m_ss << "Oxygen Bottle 1 has been replenished";
        break;
      case SEAnesthesiaMachineEvent::OxygenBottle2Exhausted:
        m_ss << "Oxygen Bottle 2 has been replenished";
        break;
      case SEAnesthesiaMachineEvent::ReliefValveActive:
        m_ss << "Relief valve inactive - pressure below setting";
        break;
      default:
        m_ss << "Anesthesia Machine Event Off : " << type;
      }
    }
    Info(m_ss);
  }
  m_EventState[type] = active;
  m_EventDuration_s[type] = 0;

  if (m_EventHandler != nullptr)
    m_EventHandler->HandleAnesthesiaMachineEvent(type, active, &time);
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::IsEventActive(SEAnesthesiaMachineEvent type) const
{
  auto b = m_EventState.find(type);
  if (b == m_EventState.end())
    return false;
  return b->second;
}
//-----------------------------------------------------------------------------
double SEAnesthesiaMachine::GetEventDuration(SEAnesthesiaMachineEvent type, const TimeUnit& unit) const
{
  auto i = m_EventDuration_s.find(type);
  if (i == m_EventDuration_s.end())
    return 0;
  return Convert(i->second, TimeUnit::s, unit);
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::UpdateEvents(const SEScalarTime& timeStep)
{
  for (auto itr : m_EventDuration_s)
    itr.second += timeStep.GetValue(TimeUnit::s);
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::ForwardEvents(SEEventHandler* handler)
{
  m_EventHandler = handler;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::SetConnection(SEAnesthesiaMachineConnection c)
{
  m_Connection = c;
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasConnection() const
{
  return m_Connection == ((SEAnesthesiaMachineConnection)-1) ? false : true;
}
//-----------------------------------------------------------------------------
SEAnesthesiaMachineConnection SEAnesthesiaMachine::GetConnection() const
{
  return m_Connection;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::InvalidateConnection()
{
  m_Connection = (SEAnesthesiaMachineConnection)-1;
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasInletFlow() const
{
  return m_InletFlow == nullptr ? false : m_InletFlow->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolumePerTime& SEAnesthesiaMachine::GetInletFlow()
{
  if (m_InletFlow == nullptr)
    m_InletFlow = new SEScalarVolumePerTime();
  return *m_InletFlow;
}
//-----------------------------------------------------------------------------
double SEAnesthesiaMachine::GetInletFlow(const VolumePerTimeUnit& unit) const
{
  if (m_InletFlow == nullptr)
    return SEScalar::dNaN();
  return m_InletFlow->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasInspiratoryExpiratoryRatio() const
{
  return m_InspiratoryExpiratoryRatio == nullptr ? false : m_InspiratoryExpiratoryRatio->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SEAnesthesiaMachine::GetInspiratoryExpiratoryRatio()
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    m_InspiratoryExpiratoryRatio = new SEScalar();
  return *m_InspiratoryExpiratoryRatio;
}
//-----------------------------------------------------------------------------
double SEAnesthesiaMachine::GetInspiratoryExpiratoryRatio() const
{
  if (m_InspiratoryExpiratoryRatio == nullptr)
    return SEScalar::dNaN();
  return m_InspiratoryExpiratoryRatio->GetValue();
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasOxygenFraction() const
{
  return m_OxygenFraction == nullptr ? false : m_OxygenFraction->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFraction& SEAnesthesiaMachine::GetOxygenFraction()
{
  if (m_OxygenFraction == nullptr)
    m_OxygenFraction = new SEScalarFraction();
  return *m_OxygenFraction;
}
//-----------------------------------------------------------------------------
double SEAnesthesiaMachine::GetOxygenFraction() const
{
  if (m_OxygenFraction == nullptr)
    return SEScalar::dNaN();
  return m_OxygenFraction->GetValue();
}
//-----------------------------------------------------------------------------
SEAnesthesiaMachineOxygenSource SEAnesthesiaMachine::GetOxygenSource() const
{
  return m_OxygenSource;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::SetOxygenSource(SEAnesthesiaMachineOxygenSource src)
{
  m_OxygenSource = src;
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasOxygenSource() const
{
  return m_OxygenSource == ((SEAnesthesiaMachineOxygenSource)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::InvalidateOxygenSource()
{
  m_OxygenSource = (SEAnesthesiaMachineOxygenSource)-1;
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasPositiveEndExpiredPressure() const
{
  return m_PositiveEndExpiredPressure == nullptr ? false : m_PositiveEndExpiredPressure->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SEAnesthesiaMachine::GetPositiveEndExpiredPressure()
{
  if (m_PositiveEndExpiredPressure == nullptr)
    m_PositiveEndExpiredPressure = new SEScalarPressure();
  return *m_PositiveEndExpiredPressure;
}
//-----------------------------------------------------------------------------
double SEAnesthesiaMachine::GetPositiveEndExpiredPressure(const PressureUnit& unit) const
{
  if (m_PositiveEndExpiredPressure == nullptr)
    return SEScalar::dNaN();
  return m_PositiveEndExpiredPressure->GetValue(unit);
}
//-----------------------------------------------------------------------------
SEAnesthesiaMachinePrimaryGas SEAnesthesiaMachine::GetPrimaryGas() const
{
  return m_PrimaryGas;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::SetPrimaryGas(SEAnesthesiaMachinePrimaryGas gas)
{
  m_PrimaryGas = gas;
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasPrimaryGas() const
{
  return m_PrimaryGas == ((SEAnesthesiaMachinePrimaryGas)-1) ? false : true;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::InvalidatePrimaryGas()
{
  m_PrimaryGas = (SEAnesthesiaMachinePrimaryGas)-1;
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasRespiratoryRate() const
{
  return m_RespiratoryRate == nullptr ? false : m_RespiratoryRate->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFrequency& SEAnesthesiaMachine::GetRespiratoryRate()
{
  if (m_RespiratoryRate == nullptr)
    m_RespiratoryRate = new SEScalarFrequency();
  return *m_RespiratoryRate;
}
//-----------------------------------------------------------------------------
double SEAnesthesiaMachine::GetRespiratoryRate(const FrequencyUnit& unit) const
{
  if (m_RespiratoryRate == nullptr)
    return SEScalar::dNaN();
  return m_RespiratoryRate->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasReliefValvePressure() const
{
  return m_ReliefValvePressure == nullptr ? false : m_ReliefValvePressure->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SEAnesthesiaMachine::GetReliefValvePressure()
{
  if (m_ReliefValvePressure == nullptr)
    m_ReliefValvePressure = new SEScalarPressure();
  return *m_ReliefValvePressure;
}
//-----------------------------------------------------------------------------
double SEAnesthesiaMachine::GetReliefValvePressure(const PressureUnit& unit) const
{
  if (m_ReliefValvePressure == nullptr)
    return SEScalar::dNaN();
  return m_ReliefValvePressure->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasVentilatorPressure() const
{
  return m_VentilatorPressure == nullptr ? false : m_VentilatorPressure->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarPressure& SEAnesthesiaMachine::GetVentilatorPressure()
{
  if (m_VentilatorPressure == nullptr)
    m_VentilatorPressure = new SEScalarPressure();
  return *m_VentilatorPressure;
}
//-----------------------------------------------------------------------------
double SEAnesthesiaMachine::GetVentilatorPressure(const PressureUnit& unit) const
{
  if (m_VentilatorPressure == nullptr)
    return SEScalar::dNaN();
  return m_VentilatorPressure->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasLeftChamber() const
{
  return m_LeftChamber == nullptr ? false : true;
}
//-----------------------------------------------------------------------------
SEAnesthesiaMachineChamber& SEAnesthesiaMachine::GetLeftChamber()
{
  if (m_LeftChamber == nullptr)
    m_LeftChamber = new SEAnesthesiaMachineChamber(m_Substances);
  return *m_LeftChamber;
}
//-----------------------------------------------------------------------------
const SEAnesthesiaMachineChamber* SEAnesthesiaMachine::GetLeftChamber() const
{
  return m_LeftChamber;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::RemoveLeftChamber()
{
  SAFE_DELETE(m_LeftChamber);
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasRightChamber() const
{
  return m_RightChamber == nullptr ? false : true;
}
//-----------------------------------------------------------------------------
SEAnesthesiaMachineChamber& SEAnesthesiaMachine::GetRightChamber()
{
  if (m_RightChamber == nullptr)
    m_RightChamber = new SEAnesthesiaMachineChamber(m_Substances);
  return *m_RightChamber;
}
//-----------------------------------------------------------------------------
const SEAnesthesiaMachineChamber* SEAnesthesiaMachine::GetRightChamber() const
{
  return m_RightChamber;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::RemoveRightChamber()
{
  SAFE_DELETE(m_RightChamber);
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasOxygenBottleOne() const
{
  return m_OxygenBottleOne == nullptr ? false : true;
}
//-----------------------------------------------------------------------------
SEAnesthesiaMachineOxygenBottle& SEAnesthesiaMachine::GetOxygenBottleOne()
{
  if (m_OxygenBottleOne == nullptr)
    m_OxygenBottleOne = new SEAnesthesiaMachineOxygenBottle(GetLogger());
  return *m_OxygenBottleOne;
}
//-----------------------------------------------------------------------------
const SEAnesthesiaMachineOxygenBottle* SEAnesthesiaMachine::GetOxygenBottleOne() const
{
  return m_OxygenBottleOne;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::RemoveOxygenBottleOne()
{
  SAFE_DELETE(m_OxygenBottleOne);
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::HasOxygenBottleTwo() const
{
  return m_OxygenBottleTwo == nullptr ? false : true;
}
//-----------------------------------------------------------------------------
SEAnesthesiaMachineOxygenBottle& SEAnesthesiaMachine::GetOxygenBottleTwo()
{
  if (m_OxygenBottleTwo == nullptr)
    m_OxygenBottleTwo = new SEAnesthesiaMachineOxygenBottle(GetLogger());
  return *m_OxygenBottleTwo;
}
//-----------------------------------------------------------------------------
const SEAnesthesiaMachineOxygenBottle* SEAnesthesiaMachine::GetOxygenBottleTwo() const
{
  return m_OxygenBottleTwo;
}
//-----------------------------------------------------------------------------
void SEAnesthesiaMachine::RemoveOxygenBottleTwo()
{
  SAFE_DELETE(m_OxygenBottleTwo);
}
//-----------------------------------------------------------------------------
Tree<const char*> SEAnesthesiaMachine::GetPhysiologyRequestGraph() const
{
  return { "" };
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::operator==(SEAnesthesiaMachine const& rhs) const
{
  // return m_EventState == rhs.m_EventState
  //   && m_EventDuration_s == rhs.m_EventDuration_s
  //   && m_Connection == rhs.m_Connection
  //   && ((m_InletFlow && rhs.m_InletFlow)
  //         ? m_InletFlow->operator==(*rhs.m_InletFlow)
  //         : m_InletFlow == rhs.m_InletFlow)
  //   && ((m_InspiratoryExpiratoryRatio && rhs.m_InspiratoryExpiratoryRatio)
  //         ? m_InspiratoryExpiratoryRatio->operator==(*rhs.m_InspiratoryExpiratoryRatio)
  //         : (m_InspiratoryExpiratoryRatio == rhs.m_InspiratoryExpiratoryRatio))
  //   && m_OxygenSource == rhs.m_OxygenSource
  //   && ((m_PositiveEndExpiredPressure && rhs.m_PositiveEndExpiredPressure)
  //         ? m_PositiveEndExpiredPressure->operator==(*rhs.m_PositiveEndExpiredPressure)
  //         : (m_PositiveEndExpiredPressure == rhs.m_PositiveEndExpiredPressure))
  //   && m_PrimaryGas == rhs.m_PrimaryGas
  //   && ((m_RespiratoryRate && rhs.m_RespiratoryRate)
  //         ? m_RespiratoryRate->operator==(*rhs.m_RespiratoryRate)
  //         : (m_RespiratoryRate == rhs.m_RespiratoryRate))
  //   && ((m_ReliefValvePressure && rhs.m_ReliefValvePressure)
  //         ? m_ReliefValvePressure->operator==(*rhs.m_ReliefValvePressure)
  //         : (m_ReliefValvePressure == rhs.m_ReliefValvePressure))
  //   && ((m_VentilatorPressure && rhs.m_VentilatorPressure)
  //         ? m_VentilatorPressure->operator==(*rhs.m_VentilatorPressure)
  //         : (m_VentilatorPressure == rhs.m_VentilatorPressure))
  //   && ((m_LeftChamber && rhs.m_LeftChamber)
  //         ? m_LeftChamber->operator==(*rhs.m_LeftChamber)
  //         : (m_LeftChamber == rhs.m_LeftChamber))
  //   && ((m_RightChamber && rhs.m_RightChamber)
  //         ? m_RightChamber->operator==(*rhs.m_RightChamber)
  //         : (m_RightChamber == rhs.m_RightChamber))
  //   && ((m_OxygenBottleOne && rhs.m_OxygenBottleOne)
  //         ? m_OxygenBottleOne->operator==(*rhs.m_OxygenBottleOne)
  //         : (m_OxygenBottleOne == rhs.m_OxygenBottleOne))
  //   && ((m_OxygenBottleTwo && rhs.m_OxygenBottleTwo)
  //         ? m_OxygenBottleTwo->operator==(*rhs.m_OxygenBottleTwo)
  //         : (m_OxygenBottleTwo == rhs.m_OxygenBottleTwo))
  //   && (m_Substances == rhs.m_Substances);

  bool equivilant = m_EventState == rhs.m_EventState;
  equivilant &= m_EventDuration_s == rhs.m_EventDuration_s;
  equivilant &= m_Connection == rhs.m_Connection;
  equivilant &= ((m_InletFlow && rhs.m_InletFlow)
                   ? m_InletFlow->operator==(*rhs.m_InletFlow)
                   : m_InletFlow == rhs.m_InletFlow);
  equivilant &= ((m_InspiratoryExpiratoryRatio && rhs.m_InspiratoryExpiratoryRatio)
                   ? m_InspiratoryExpiratoryRatio->operator==(*rhs.m_InspiratoryExpiratoryRatio)
                   : (m_InspiratoryExpiratoryRatio == rhs.m_InspiratoryExpiratoryRatio));
  equivilant &= m_OxygenSource == rhs.m_OxygenSource;
  equivilant &= ((m_PositiveEndExpiredPressure && rhs.m_PositiveEndExpiredPressure)
                   ? m_PositiveEndExpiredPressure->operator==(*rhs.m_PositiveEndExpiredPressure)
                   : (m_PositiveEndExpiredPressure == rhs.m_PositiveEndExpiredPressure));
  equivilant &= m_PrimaryGas == rhs.m_PrimaryGas;
  equivilant &= ((m_RespiratoryRate && rhs.m_RespiratoryRate)
                   ? m_RespiratoryRate->operator==(*rhs.m_RespiratoryRate)
                   : (m_RespiratoryRate == rhs.m_RespiratoryRate));
  equivilant &= ((m_ReliefValvePressure && rhs.m_ReliefValvePressure)
                   ? m_ReliefValvePressure->operator==(*rhs.m_ReliefValvePressure)
                   : (m_ReliefValvePressure == rhs.m_ReliefValvePressure));
  equivilant &= ((m_VentilatorPressure && rhs.m_VentilatorPressure)
                   ? m_VentilatorPressure->operator==(*rhs.m_VentilatorPressure)
                   : (m_VentilatorPressure == rhs.m_VentilatorPressure));
  equivilant &= ((m_LeftChamber && rhs.m_LeftChamber)
                   ? m_LeftChamber->operator==(*rhs.m_LeftChamber)
                   : (m_LeftChamber == rhs.m_LeftChamber));
  equivilant &= ((m_RightChamber && rhs.m_RightChamber)
                   ? m_RightChamber->operator==(*rhs.m_RightChamber)
                   : (m_RightChamber == rhs.m_RightChamber));
  equivilant &= ((m_OxygenBottleOne && rhs.m_OxygenBottleOne)
                   ? m_OxygenBottleOne->operator==(*rhs.m_OxygenBottleOne)
                   : (m_OxygenBottleOne == rhs.m_OxygenBottleOne));
  equivilant &= ((m_OxygenBottleTwo && rhs.m_OxygenBottleTwo)
                   ? m_OxygenBottleTwo->operator==(*rhs.m_OxygenBottleTwo)
                   : (m_OxygenBottleTwo == rhs.m_OxygenBottleTwo));
  equivilant &= (m_Substances == rhs.m_Substances);
  return equivilant;
}
//-----------------------------------------------------------------------------
bool SEAnesthesiaMachine::operator!=(SEAnesthesiaMachine const& rhs) const
{
  return !(*this == rhs);
}
//-----------------------------------------------------------------------------

}