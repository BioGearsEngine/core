#include "Anesthesia.h"

#include "Property.h"
#include "System.h"

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/SESystem.h>
#include <biogears/cdm/enums/SEAnesthesiaEnums.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>

namespace biogears {
namespace io {
//----------------------------------------------------------------------------------
// class SEAnesthesiaMachine
void Anesthesia::UnMarshall(const CDM::AnesthesiaMachineData& in, SEAnesthesiaMachine& out)
{
  System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));
  
  UnMarshall(in.Connection(), out.m_Connection);
  
  if (in.InletFlow().present()) {
    io::Property::UnMarshall(in.InletFlow(), out.GetInletFlow());
  }
  if (in.InspiratoryExpiratoryRatio().present()) {
    io::Property::UnMarshall(in.InspiratoryExpiratoryRatio(), out.GetInspiratoryExpiratoryRatio());
  }
  if (in.OxygenFraction().present()) {
    io::Property::UnMarshall(in.OxygenFraction(), out.GetOxygenFraction());
  }
  
  UnMarshall(in.OxygenSource(), out.m_OxygenSource);
  
  if (in.PositiveEndExpiredPressure().present()) {
    io::Property::UnMarshall(in.PositiveEndExpiredPressure(), out.GetPositiveEndExpiredPressure());
  }
  
  UnMarshall(in.PrimaryGas(), out.m_PrimaryGas);
  
  if (in.RespiratoryRate().present()) {
    io::Property::UnMarshall(in.RespiratoryRate(), out.GetRespiratoryRate());
  }
  if (in.ReliefValvePressure().present()) {
    io::Property::UnMarshall(in.ReliefValvePressure(), out.GetReliefValvePressure());
  }
  if (in.VentilatorPressure().present()) {
    io::Property::UnMarshall(in.VentilatorPressure(), out.GetVentilatorPressure());
  }
  if (in.LeftChamber().present()) {
    UnMarshall(in.LeftChamber(), out.GetLeftChamber());
  }
  if (in.RightChamber().present()) {
    UnMarshall(in.RightChamber(), out.GetRightChamber());
  }
  if (in.OxygenBottleOne().present()) {
    UnMarshall(in.OxygenBottleOne(), out.GetOxygenBottleOne());
  }
  if (in.OxygenBottleTwo().present()) {
    UnMarshall(in.OxygenBottleTwo(), out.GetOxygenBottleTwo());
  }
  
  SEScalarTime time;
  SEAnesthesiaMachineEvent event;
  for (auto e : in.ActiveEvent()) {
    io::Property::UnMarshall(e.Duration(), time);
    UnMarshall(e.Event(), event);
    
    out.m_EventState[event] = true;
    out.m_EventDuration_s[event] = time.GetValue(TimeUnit::s);
  }
  
  out.StateChange();
  ;
}
//----------------------------------------------------------------------------------
void Anesthesia::Marshall(const SEAnesthesiaMachine& in, CDM::AnesthesiaMachineData& out)
{
  System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::AnesthesiaMachineData&>(out));
  
  SE_OPTIONAL_ANESTHESIA_ENUM_MARSHALL_HELPER(in, out, Connection)
  
  if (in.m_InletFlow != nullptr) {
    io::Property::Marshall(*in.m_InletFlow, out.InletFlow());
  }
  if (in.m_InspiratoryExpiratoryRatio != nullptr) {
    io::Property::Marshall(*in.m_InspiratoryExpiratoryRatio, out.InspiratoryExpiratoryRatio());
  }
  if (in.m_OxygenFraction != nullptr) {
    io::Property::Marshall(*in.m_OxygenFraction, out.OxygenFraction());
  }
  
  SE_OPTIONAL_ANESTHESIA_ENUM_MARSHALL_HELPER(in, out, OxygenSource)
  
  if (in.m_PositiveEndExpiredPressure != nullptr) {
    io::Property::Marshall(*in.m_PositiveEndExpiredPressure, out.PositiveEndExpiredPressure());
  }
  
  SE_OPTIONAL_ANESTHESIA_ENUM_MARSHALL_HELPER(in, out, PrimaryGas)
  
  if (in.m_RespiratoryRate != nullptr) {
    io::Property::Marshall(*in.m_RespiratoryRate, out.RespiratoryRate());
  }
  if (in.m_ReliefValvePressure != nullptr) {
    io::Property::Marshall(*in.m_ReliefValvePressure, out.ReliefValvePressure());
  }
  if (in.m_VentilatorPressure != nullptr) {
    io::Property::Marshall(*in.m_VentilatorPressure, out.VentilatorPressure());
  }
  if (in.HasLeftChamber()) {
    Marshall(*in.m_LeftChamber, out.LeftChamber());
  }
  if (in.HasRightChamber()) {
    Marshall(*in.m_RightChamber, out.RightChamber());
  }
  if (in.HasOxygenBottleOne()) {
    Marshall(*in.m_OxygenBottleOne, out.OxygenBottleOne());
  }
  if (in.HasOxygenBottleTwo()) {
    Marshall(*in.m_OxygenBottleTwo, out.OxygenBottleTwo());
  }
  
  SEScalarTime time;
  for (auto itr : in.m_EventState) {
    auto it2 = in.m_EventDuration_s.find(itr.first);
    if (it2 == in.m_EventDuration_s.end()) { // This should not happen...
      time.SetValue(0, TimeUnit::s);
    } else {
      time.SetValue(it2->second, TimeUnit::s);
    }
    
    CDM::ActiveAnesthesiaMachineEventData* eData = new CDM::ActiveAnesthesiaMachineEventData();
    
    eData->Event(std::make_unique<std::remove_reference<decltype(eData->Event())>::type>());
    io::Anesthesia::Marshall(itr.first, eData->Event());
    
    // ::mil::tatrc::physiology::datamodel::enumAnesthesiaMachineEvent
    Anesthesia::Marshall(itr.first, eData->Event());
    
    eData->Duration(std::make_unique<std::remove_reference<decltype(eData->Duration())>::type>());
    io::Property::Marshall(time, eData->Duration());
    
    out.ActiveEvent().push_back(std::unique_ptr<CDM::ActiveAnesthesiaMachineEventData>(eData));
  }
}
//----------------------------------------------------------------------------------
// class SEAnesthesiaMachineChamber
void Anesthesia::UnMarshall(const CDM::AnesthesiaMachineChamberData& in, SEAnesthesiaMachineChamber& out)
{
  if (in.State().present()) {
    auto state = out.GetState();
    Property::UnMarshall(in.State(), state);
    out.SetState(state);
  }
  if (in.SubstanceFraction().present()) {
    io::Property::UnMarshall(in.SubstanceFraction(), out.GetSubstanceFraction());
  }
  if (in.Substance().present()) {
    out.m_Substance = out.m_Substances.GetSubstance(in.Substance().get());
    if (out.m_Substance == nullptr) {
      std::stringstream ss;
      ss << "Do not have substance : " << in.Substance().get();
      throw CommonDataModelException(ss.str());
    }
  }
}
//----------------------------------------------------------------------------------
void Anesthesia::Marshall(const SEAnesthesiaMachineChamber& in, CDM::AnesthesiaMachineChamberData& out)
{ 
  io::Property::Marshall(in.GetState(), out.State());
  
  if (in.m_SubstanceFraction != nullptr) {
    io::Property::Marshall(*in.m_SubstanceFraction, out.SubstanceFraction());
  }
  if (in.HasSubstance()) {
    out.Substance(in.m_Substance->GetName());
  }
}
//----------------------------------------------------------------------------------
// class SEAnesthesiaMachineOxygenBottle
void Anesthesia::UnMarshall(const CDM::AnesthesiaMachineOxygenBottleData& in, SEAnesthesiaMachineOxygenBottle& out)
{
  if (in.Volume().present()) {
    io::Property::UnMarshall(in.Volume(), out.GetVolume());
  }
}
//----------------------------------------------------------------------------------
void Anesthesia::Marshall(const SEAnesthesiaMachineOxygenBottle& in, CDM::AnesthesiaMachineOxygenBottleData& out)
{
  if (in.m_Volume != nullptr) {
    io::Property::Marshall(*in.m_Volume, out.Volume());
  }
}
//----------------------------------------------------------------------------------
// SEAnesthesiaMachineEvent
void Anesthesia::UnMarshall(const CDM::enumAnesthesiaMachineEvent& in, SEAnesthesiaMachineEvent& out)
{
  try{
    switch (in) {
      case CDM::enumAnesthesiaMachineEvent::OxygenBottle1Exhausted:
        out = SEAnesthesiaMachineEvent::OxygenBottle1Exhausted;
        break;
      case CDM::enumAnesthesiaMachineEvent::OxygenBottle2Exhausted:
        out = SEAnesthesiaMachineEvent::OxygenBottle2Exhausted;
        break;
      case CDM::enumAnesthesiaMachineEvent::ReliefValveActive:
        out = SEAnesthesiaMachineEvent::ReliefValveActive;
        break;
      default:
        out = SEAnesthesiaMachineEvent::Invalid;
        break;
    }
  } catch ( xsd::cxx::tree::unexpected_enumerator<char> ) {
    out = SEAnesthesiaMachineEvent::Invalid;
  }
}
void Anesthesia::Marshall(const SEAnesthesiaMachineEvent& in, CDM::enumAnesthesiaMachineEvent& out)
{
  switch (in) {
    case SEAnesthesiaMachineEvent::OxygenBottle1Exhausted:
      out = CDM::enumAnesthesiaMachineEvent::OxygenBottle1Exhausted;
      break;
    case SEAnesthesiaMachineEvent::OxygenBottle2Exhausted:
      out = CDM::enumAnesthesiaMachineEvent::OxygenBottle2Exhausted;
      break;
    case SEAnesthesiaMachineEvent::ReliefValveActive:
      out = CDM::enumAnesthesiaMachineEvent::ReliefValveActive;
      break;
      
    default:
      //      out = (CDM::enumAnesthesiaMachineEvent::value)-1;
      break;
  }
}
// SEAnesthesiaMachineOxygenSource
void Anesthesia::UnMarshall(const CDM::enumAnesthesiaMachineOxygenSource& in, SEAnesthesiaMachineOxygenSource& out)
{
  try {
    switch (in) {
      case CDM::enumAnesthesiaMachineOxygenSource::BottleOne:
        out = SEAnesthesiaMachineOxygenSource::BottleOne;
        break;
      case CDM::enumAnesthesiaMachineOxygenSource::BottleTwo:
        out = SEAnesthesiaMachineOxygenSource::BottleTwo;
        break;
      case CDM::enumAnesthesiaMachineOxygenSource::Wall:
        out = SEAnesthesiaMachineOxygenSource::Wall;
        break;
      default:
        out = SEAnesthesiaMachineOxygenSource::Invalid;
        break;
    }
  } catch ( xsd::cxx::tree::unexpected_enumerator<char> ) {
    out = SEAnesthesiaMachineOxygenSource::Invalid;
  }
}
void Anesthesia::Marshall(const SEAnesthesiaMachineOxygenSource& in, CDM::enumAnesthesiaMachineOxygenSource& out)
{
  switch (in) {
    case SEAnesthesiaMachineOxygenSource::BottleOne:
      out = CDM::enumAnesthesiaMachineOxygenSource::BottleOne;
      break;
    case SEAnesthesiaMachineOxygenSource::BottleTwo:
      out = CDM::enumAnesthesiaMachineOxygenSource::BottleTwo;
      break;
    case SEAnesthesiaMachineOxygenSource::Wall:
      out = CDM::enumAnesthesiaMachineOxygenSource::Wall;
      break;
    default:
      //      out = (CDM::enumAnesthesiaMachineOxygenSource::value)-1;
      break;
  }
}
// SEAnesthesiaMachinePrimaryGas
void Anesthesia::UnMarshall(const CDM::enumAnesthesiaMachinePrimaryGas& in, SEAnesthesiaMachinePrimaryGas& out)
{
  try {
    switch (in) {
      case CDM::enumAnesthesiaMachinePrimaryGas::Air:
        out = SEAnesthesiaMachinePrimaryGas::Air;
        break;
      case CDM::enumAnesthesiaMachinePrimaryGas::Nitrogen:
        out = SEAnesthesiaMachinePrimaryGas::Nitrogen;
        break;
      default:
        out = SEAnesthesiaMachinePrimaryGas::Invalid;
        break;
    }
  } catch ( xsd::cxx::tree::unexpected_enumerator<char> ) {
    out = SEAnesthesiaMachinePrimaryGas::Invalid;
  }
}
void Anesthesia::Marshall(const SEAnesthesiaMachinePrimaryGas& in, CDM::enumAnesthesiaMachinePrimaryGas& out)
{
  switch (in) {
    case SEAnesthesiaMachinePrimaryGas::Air:
      out = CDM::enumAnesthesiaMachinePrimaryGas::Air;
      break;
    case SEAnesthesiaMachinePrimaryGas::Nitrogen:
      out = CDM::enumAnesthesiaMachinePrimaryGas::Nitrogen;
      break;
    default:
      //      out = (CDM::enumAnesthesiaMachinePrimaryGas::value)-1;
      break;
  }
}
// SEAnesthesiaMachineConnection
void Anesthesia::UnMarshall(const CDM::enumAnesthesiaMachineConnection& in, SEAnesthesiaMachineConnection& out)
{
  try {
    switch (in) {
      case CDM::enumAnesthesiaMachineConnection::Mask:
        out = SEAnesthesiaMachineConnection::Mask;
        break;
      case CDM::enumAnesthesiaMachineConnection::Off:
        out = SEAnesthesiaMachineConnection::Off;
        break;
      case CDM::enumAnesthesiaMachineConnection::Tube:
        out = SEAnesthesiaMachineConnection::Tube;
        break;
      default:
        out = SEAnesthesiaMachineConnection::Invalid;
        break;
    }
  } catch ( xsd::cxx::tree::unexpected_enumerator<char> ) {
    out = SEAnesthesiaMachineConnection::Invalid;
  }
}
void Anesthesia::Marshall(const SEAnesthesiaMachineConnection& in, CDM::enumAnesthesiaMachineConnection& out)
{
  switch (in) {
    case SEAnesthesiaMachineConnection::Off:
      out = CDM::enumAnesthesiaMachineConnection::Off;
      break;
    case SEAnesthesiaMachineConnection::Mask:
      out = CDM::enumAnesthesiaMachineConnection::Mask;
      break;
    case SEAnesthesiaMachineConnection::Tube:
      out = CDM::enumAnesthesiaMachineConnection::Tube;
      break;
    default:
      //      out = (CDM::enumAnesthesiaMachineConnection::value)-1;
      break;
  }
}
}

bool operator==(CDM::enumAnesthesiaMachineEvent const& lhs, SEAnesthesiaMachineEvent const& rhs)
{
    switch (rhs) {
      case SEAnesthesiaMachineEvent::OxygenBottle1Exhausted:
        return (CDM::enumAnesthesiaMachineEvent::OxygenBottle1Exhausted == lhs);
      case SEAnesthesiaMachineEvent::OxygenBottle2Exhausted:
        return (CDM::enumAnesthesiaMachineEvent::OxygenBottle2Exhausted == lhs);
      case SEAnesthesiaMachineEvent::ReliefValveActive:
        return (CDM::enumAnesthesiaMachineEvent::ReliefValveActive == lhs);
      case SEAnesthesiaMachineEvent::Invalid:
        return ((CDM::enumAnesthesiaMachineEvent::value)-1 == lhs);
      default:
        return false;
    }
}
bool operator==(CDM::enumAnesthesiaMachineOxygenSource const& lhs, SEAnesthesiaMachineOxygenSource const& rhs)
{
  switch (rhs) {
    case SEAnesthesiaMachineOxygenSource::BottleOne:
      return (CDM::enumAnesthesiaMachineOxygenSource::BottleOne == lhs);
    case SEAnesthesiaMachineOxygenSource::BottleTwo:
      return (CDM::enumAnesthesiaMachineOxygenSource::BottleTwo == lhs);
    case SEAnesthesiaMachineOxygenSource::Wall:
      return (CDM::enumAnesthesiaMachineOxygenSource::Wall == lhs);
    case SEAnesthesiaMachineOxygenSource::Invalid:
      return ((CDM::enumAnesthesiaMachineOxygenSource::value)-1 == lhs);
    default:
      return false;
  }
}
bool operator==(CDM::enumAnesthesiaMachinePrimaryGas const& lhs, SEAnesthesiaMachinePrimaryGas const& rhs)
{
  switch (rhs) {
    case SEAnesthesiaMachinePrimaryGas::Air:
      return (CDM::enumAnesthesiaMachinePrimaryGas::Air == lhs);
    case SEAnesthesiaMachinePrimaryGas::Nitrogen:
      return (CDM::enumAnesthesiaMachinePrimaryGas::Nitrogen == lhs);
    case SEAnesthesiaMachinePrimaryGas::Invalid:
      return ((CDM::enumAnesthesiaMachinePrimaryGas::value)-1 == lhs);
    default:
      return false;
  }
}
bool operator==(CDM::enumAnesthesiaMachineConnection const& lhs, SEAnesthesiaMachineConnection const& rhs)
{
  switch (rhs) {
    case SEAnesthesiaMachineConnection::Off:
      return (CDM::enumAnesthesiaMachineConnection::Off == lhs);
    case SEAnesthesiaMachineConnection::Mask:
      return (CDM::enumAnesthesiaMachineConnection::Mask == lhs);
    case SEAnesthesiaMachineConnection::Tube:
      return (CDM::enumAnesthesiaMachineConnection::Tube == lhs);
    case SEAnesthesiaMachineConnection::Invalid:
      return ((CDM::enumAnesthesiaMachineConnection::value)-1 == lhs);
    default:
      return false;
  }
}

}
