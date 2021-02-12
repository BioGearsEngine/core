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
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
namespace biogears {
namespace io {
  //----------------------------------------------------------------------------------
  //class SEAnesthesiaMachine
  void Anesthesia::Marshall(const CDM::AnesthesiaMachineData& in, SEAnesthesiaMachine& out)
  {
    System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));

    if (in.Connection().present()) {
      out.m_Connection = in.Connection().get();
    }
    if (in.InletFlow().present()) {
      io::Property::Marshall(in.InletFlow(), out.GetInletFlow());
    }
    if (in.InspiratoryExpiratoryRatio().present()) {
      io::Property::Marshall(in.InspiratoryExpiratoryRatio(), out.GetInspiratoryExpiratoryRatio());
    }
    if (in.OxygenFraction().present()) {
      io::Property::Marshall(in.OxygenFraction(), out.GetOxygenFraction());
    }

    if (in.OxygenSource().present()) {
      out.SetOxygenSource(in.OxygenSource().get());
    }
    if (in.PositiveEndExpiredPressure().present()) {
      io::Property::Marshall(in.PositiveEndExpiredPressure(), out.GetPositiveEndExpiredPressure());
    }
    if (in.PrimaryGas().present()) {
      out.SetPrimaryGas(in.PrimaryGas().get());
    }

    if (in.RespiratoryRate().present()) {
      io::Property::Marshall(in.RespiratoryRate(), out.GetRespiratoryRate());
    }
    if (in.ReliefValvePressure().present()) {
      io::Property::Marshall(in.ReliefValvePressure(), out.GetReliefValvePressure());
    }
    if (in.VentilatorPressure().present()) {
      io::Property::Marshall(in.VentilatorPressure(), out.GetVentilatorPressure());
    }
    if (in.LeftChamber().present()) {
      Marshall(in.LeftChamber(), out.GetLeftChamber());
    }
    if (in.RightChamber().present()) {
      Marshall(in.RightChamber(), out.GetRightChamber());
    }
    if (in.OxygenBottleOne().present()) {
      Marshall(in.OxygenBottleOne(), out.GetOxygenBottleOne());
    }
    if (in.OxygenBottleTwo().present()) {
      Marshall(in.OxygenBottleTwo(), out.GetOxygenBottleTwo());
    }

    SEScalarTime time;
    for (auto e : in.ActiveEvent()) {
      io::Property::Marshall(e.Duration(), time);
      out.m_EventState[e.Event()] = true;
      out.m_EventDuration_s[e.Event()] = time.GetValue(TimeUnit::s);
    }

    out.StateChange();
    ;
  }
  //----------------------------------------------------------------------------------
  void Anesthesia::UnMarshall(const SEAnesthesiaMachine& in, CDM::AnesthesiaMachineData& out)
  {
    System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::AnesthesiaMachineData&>(out));
    if (in.HasConnection()) {
      out.Connection(in.m_Connection);
    }
    if (in.m_InletFlow != nullptr) {
      io::Property::UnMarshall(*in.m_InletFlow, out.InletFlow());
    }
    if (in.m_InspiratoryExpiratoryRatio != nullptr) {
      io::Property::UnMarshall(*in.m_InspiratoryExpiratoryRatio, out.InspiratoryExpiratoryRatio());
    }
    if (in.m_OxygenFraction != nullptr) {
      io::Property::UnMarshall(*in.m_OxygenFraction, out.OxygenFraction());
    }
    if (in.HasOxygenSource()) {
      out.OxygenSource(in.m_OxygenSource);
    }
    if (in.m_PositiveEndExpiredPressure != nullptr) {
      io::Property::UnMarshall(*in.m_PositiveEndExpiredPressure, out.PositiveEndExpiredPressure());
    }
    if (in.HasPrimaryGas()) {
      out.PrimaryGas(in.m_PrimaryGas);
    }

    if (in.m_RespiratoryRate != nullptr) {
      io::Property::UnMarshall(*in.m_RespiratoryRate, out.RespiratoryRate());
    }
    if (in.m_ReliefValvePressure != nullptr) {
      io::Property::UnMarshall(*in.m_ReliefValvePressure, out.ReliefValvePressure());
    }
    if (in.m_VentilatorPressure != nullptr) {
      io::Property::UnMarshall(*in.m_VentilatorPressure, out.VentilatorPressure());
    }
    if (in.HasLeftChamber()) {
      UnMarshall(*in.m_LeftChamber, out.LeftChamber());
    }
    if (in.HasRightChamber()) {
      UnMarshall(*in.m_RightChamber, out.RightChamber());
    }
    if (in.HasOxygenBottleOne()) {
      UnMarshall(*in.m_OxygenBottleOne, out.OxygenBottleOne());
    }
    if (in.HasOxygenBottleTwo()) {
      UnMarshall(*in.m_OxygenBottleTwo, out.OxygenBottleTwo());
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
      eData->Event(itr.first);
      io::Property::UnMarshall(time, eData->Duration());
      out.ActiveEvent().push_back(std::unique_ptr<CDM::ActiveAnesthesiaMachineEventData>(eData));
    }
  }
  //----------------------------------------------------------------------------------
  //class SEAnesthesiaMachineChamber
  void Anesthesia::Marshall(const CDM::AnesthesiaMachineChamberData& in, SEAnesthesiaMachineChamber& out)
  {
    if (in.State().present()) {
      out.SetState(in.State().get());
    }
    if (in.SubstanceFraction().present()) {
      io::Property::Marshall(in.SubstanceFraction(), out.GetSubstanceFraction());
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
  void Anesthesia::UnMarshall(const SEAnesthesiaMachineChamber& in, CDM::AnesthesiaMachineChamberData& out)
  {
    if (in.HasState()) {
      out.State(in.m_State);
    }
    if (in.m_SubstanceFraction != nullptr) {
      io::Property::UnMarshall(*in.m_SubstanceFraction, out.SubstanceFraction());
    }
    if (in.HasSubstance()) {
      out.Substance(in.m_Substance->GetName());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEAnesthesiaMachineOxygenBottle
  void Anesthesia::Marshall(const CDM::AnesthesiaMachineOxygenBottleData& in, SEAnesthesiaMachineOxygenBottle& out)
  {
    if (in.Volume().present()) {
      io::Property::Marshall(in.Volume(), out.GetVolume());
    }
  }
  //----------------------------------------------------------------------------------
  void Anesthesia::UnMarshall(const SEAnesthesiaMachineOxygenBottle& in, CDM::AnesthesiaMachineOxygenBottleData& out)
  {
    if (in.m_Volume != nullptr) {
      io::Property::UnMarshall(*in.m_Volume, out.Volume());
    }
  }
  //----------------------------------------------------------------------------------
}
}