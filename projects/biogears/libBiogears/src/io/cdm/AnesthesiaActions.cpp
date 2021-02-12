#include "AnesthesiaActions.h"

#include "Anesthesia.h"
#include "Property.h"
#include "Scenario.h"

#include <biogears/cdm/properties/SEScalar0To1.h>

#include <biogears/cdm/scenario/SEAnesthesiaMachineActionCollection.h>

#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineAction.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEExpiratoryValveLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEExpiratoryValveObstruction.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEInspiratoryValveLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEInspiratoryValveObstruction.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEMaskLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenTankPressureLoss.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenWallPortPressureLoss.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SESodaLimeFailure.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SETubeCuffLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEVaporizerFailure.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEVentilatorPressureLoss.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEYPieceDisconnect.h>
namespace biogears {
namespace io {
  void AnesthesiaActions::UnMarshall(const SEAnesthesiaMachineActionCollection& in, std::vector<CDM::ActionData*>& out)
  {
    if (in.HasConfiguration()) {
      auto data = std::make_unique<CDM::AnesthesiaMachineConfigurationData>();
      io::AnesthesiaActions::UnMarshall(*in.GetConfiguration(), *data);
      out.push_back(data.release());
    }
    if (in.HasOxygenTankPressureLoss()) {
      auto data = std::make_unique<CDM::OxygenTankPressureLossData>();
      io::AnesthesiaActions::UnMarshall(*in.GetOxygenTankPressureLoss(), *data);
      out.push_back(data.release());
    }
    if (in.HasOxygenWallPortPressureLoss()) {
      auto data = std::make_unique<CDM::OxygenWallPortPressureLossData>();
      io::AnesthesiaActions::UnMarshall(*in.GetOxygenWallPortPressureLoss(), *data);
      out.push_back(data.release());
    }
    if (in.HasExpiratoryValveLeak()) {
      auto data = std::make_unique<CDM::ExpiratoryValveLeakData>();
      io::AnesthesiaActions::UnMarshall(*in.GetExpiratoryValveLeak(), *data);
      out.push_back(data.release());
    }
    if (in.HasExpiratoryValveObstruction()) {
      auto data = std::make_unique<CDM::ExpiratoryValveObstructionData>();
      io::AnesthesiaActions::UnMarshall(*in.GetExpiratoryValveObstruction(), *data);
      out.push_back(data.release());
    }
    if (in.HasInspiratoryValveLeak()) {
      auto data = std::make_unique<CDM::InspiratoryValveLeakData>();
      io::AnesthesiaActions::UnMarshall(*in.GetInspiratoryValveLeak(), *data);
      out.push_back(data.release());
    }
    if (in.HasInspiratoryValveObstruction()) {
      auto data = std::make_unique<CDM::InspiratoryValveObstructionData>();
      io::AnesthesiaActions::UnMarshall(*in.GetInspiratoryValveObstruction(), *data);
      out.push_back(data.release());
    }
    if (in.HasMaskLeak()) {
      auto data = std::make_unique<CDM::MaskLeakData>();
      io::AnesthesiaActions::UnMarshall(*in.GetMaskLeak(), *data);
      out.push_back(data.release());
    }
    if (in.HasSodaLimeFailure()) {
      auto data = std::make_unique<CDM::SodaLimeFailureData>();
      io::AnesthesiaActions::UnMarshall(*in.GetSodaLimeFailure(), *data);
      out.push_back(data.release());
    }
    if (in.HasTubeCuffLeak()) {
      auto data = std::make_unique<CDM::TubeCuffLeakData>();
      io::AnesthesiaActions::UnMarshall(*in.GetTubeCuffLeak(), *data);
      out.push_back(data.release());
    }
    if (in.HasVaporizerFailure()) {
      auto data = std::make_unique<CDM::VaporizerFailureData>();
      io::AnesthesiaActions::UnMarshall(*in.GetVaporizerFailure(), *data);
      out.push_back(data.release());
    }
    if (in.HasVentilatorPressureLoss()) {
      auto data = std::make_unique<CDM::VentilatorPressureLossData>();
      io::AnesthesiaActions::UnMarshall(*in.GetVentilatorPressureLoss(), *data);
      out.push_back(data.release());
    }
    if (in.HasYPieceDisconnect()) {
      auto data = std::make_unique<CDM::YPieceDisconnectData>();
      io::AnesthesiaActions::UnMarshall(*in.GetYPieceDisconnect(), *data);
      out.push_back(data.release());
    }
  }
  //class SEAnesthesiaMachineAction
  void AnesthesiaActions::Marshall(const CDM::AnesthesiaMachineActionData& in, SEAnesthesiaMachineAction& out)
  {
    io::Scenario::Marshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEAnesthesiaMachineAction& in, CDM::AnesthesiaMachineActionData& out)
  {
    io::Scenario::UnMarshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  //class SEAnesthesiaMachineConfiguration
  void AnesthesiaActions::Marshall(const CDM::AnesthesiaMachineConfigurationData& in, SEAnesthesiaMachineConfiguration& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    if (in.ConfigurationFile().present())
      out.SetConfigurationFile(in.ConfigurationFile().get());
    io::Anesthesia::Marshall(in.Configuration(), out.GetConfiguration());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEAnesthesiaMachineConfiguration& in, CDM::AnesthesiaMachineConfigurationData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));

    if (in.HasConfiguration())
      io::Anesthesia::UnMarshall(*in.GetConfiguration(), out.Configuration());
    else if (in.HasConfigurationFile())
      out.ConfigurationFile(in.GetConfigurationFile());
  }
  //----------------------------------------------------------------------------------
  //class SEExpiratoryValveLeak
  void AnesthesiaActions::Marshall(const CDM::ExpiratoryValveLeakData& in, SEExpiratoryValveLeak& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEExpiratoryValveLeak& in, CDM::ExpiratoryValveLeakData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    if (in.HasSeverity())
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
  }
  //----------------------------------------------------------------------------------
  //class SEExpiratoryValveObstruction
  void AnesthesiaActions::Marshall(const CDM::ExpiratoryValveObstructionData& in, SEExpiratoryValveObstruction& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEExpiratoryValveObstruction& in, CDM::ExpiratoryValveObstructionData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    if (in.HasSeverity())
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
  }
  //----------------------------------------------------------------------------------
  //class SEInspiratoryValveLeak
  void AnesthesiaActions::Marshall(const CDM::InspiratoryValveLeakData& in, SEInspiratoryValveLeak& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEInspiratoryValveLeak& in, CDM::InspiratoryValveLeakData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    if (in.HasSeverity())
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
  }
  //----------------------------------------------------------------------------------
  //class SEInspiratoryValveObstruction
  void AnesthesiaActions::Marshall(const CDM::InspiratoryValveObstructionData& in, SEInspiratoryValveObstruction& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEInspiratoryValveObstruction& in, CDM::InspiratoryValveObstructionData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    if (in.HasSeverity())
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
  }
  //----------------------------------------------------------------------------------
  //class SEMaskLeak
  void AnesthesiaActions::Marshall(const CDM::MaskLeakData& in, SEMaskLeak& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEMaskLeak& in, CDM::MaskLeakData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    if (in.HasSeverity())
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
  }
  //----------------------------------------------------------------------------------
  //class SESodaLimeFailure
  void AnesthesiaActions::Marshall(const CDM::SodaLimeFailureData& in, SESodaLimeFailure& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SESodaLimeFailure& in, CDM::SodaLimeFailureData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    if (in.HasSeverity())
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
  }
  //----------------------------------------------------------------------------------
  //class SETubeCuffLeak
  void AnesthesiaActions::Marshall(const CDM::TubeCuffLeakData& in, SETubeCuffLeak& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SETubeCuffLeak& in, CDM::TubeCuffLeakData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    if (in.HasSeverity())
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
  }
  //----------------------------------------------------------------------------------
  //class SEVaporizerFailure
  void AnesthesiaActions::Marshall(const CDM::VaporizerFailureData& in, SEVaporizerFailure& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEVaporizerFailure& in, CDM::VaporizerFailureData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    if (in.HasSeverity())
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
  }
  //----------------------------------------------------------------------------------
  //class SEVentilatorPressureLoss
  void AnesthesiaActions::Marshall(const CDM::VentilatorPressureLossData& in, SEVentilatorPressureLoss& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEVentilatorPressureLoss& in, CDM::VentilatorPressureLossData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    if (in.HasSeverity())
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
  }
  //----------------------------------------------------------------------------------
  //class SEYPieceDisconnect
  void AnesthesiaActions::Marshall(const CDM::YPieceDisconnectData& in, SEYPieceDisconnect& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEYPieceDisconnect& in, CDM::YPieceDisconnectData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    if (in.HasSeverity())
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
  }
  //----------------------------------------------------------------------------------
  //class SEOxygenWallPortPressureLoss
  void AnesthesiaActions::Marshall(const CDM::OxygenWallPortPressureLossData& in, SEOxygenWallPortPressureLoss& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    out.SetActive(in.State() == CDM::enumOnOff::On ? true : false);
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEOxygenWallPortPressureLoss& in, CDM::OxygenWallPortPressureLossData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    out.State(in.IsActive() ? CDM::enumOnOff::On : CDM::enumOnOff::Off);
  }
  //----------------------------------------------------------------------------------
  //class SEOxygenTankPressureLoss
  void AnesthesiaActions::Marshall(const CDM::OxygenTankPressureLossData& in, SEOxygenTankPressureLoss& out)
  {
    Marshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    out.SetActive(in.State() == CDM::enumOnOff::On ? true : false);
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::UnMarshall(const SEOxygenTankPressureLoss& in, CDM::OxygenTankPressureLossData& out)
  {
    UnMarshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    out.State(in.IsActive() ? CDM::enumOnOff::On : CDM::enumOnOff::Off);
  }
  //----------------------------------------------------------------------------------
}
}