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
  void AnesthesiaActions::Marshall(const SEAnesthesiaMachineActionCollection& in, std::vector<CDM::ActionData*>& out)
  {
    if (in.HasConfiguration()) {
      auto data = std::make_unique<CDM::AnesthesiaMachineConfigurationData>();
      io::AnesthesiaActions::Marshall(*in.GetConfiguration(), *data);
      out.push_back(data.release());
    }
    if (in.HasOxygenTankPressureLoss()) {
      auto data = std::make_unique<CDM::OxygenTankPressureLossData>();
      io::AnesthesiaActions::Marshall(*in.GetOxygenTankPressureLoss(), *data);
      out.push_back(data.release());
    }
    if (in.HasOxygenWallPortPressureLoss()) {
      auto data = std::make_unique<CDM::OxygenWallPortPressureLossData>();
      io::AnesthesiaActions::Marshall(*in.GetOxygenWallPortPressureLoss(), *data);
      out.push_back(data.release());
    }
    if (in.HasExpiratoryValveLeak()) {
      auto data = std::make_unique<CDM::ExpiratoryValveLeakData>();
      io::AnesthesiaActions::Marshall(*in.GetExpiratoryValveLeak(), *data);
      out.push_back(data.release());
    }
    if (in.HasExpiratoryValveObstruction()) {
      auto data = std::make_unique<CDM::ExpiratoryValveObstructionData>();
      io::AnesthesiaActions::Marshall(*in.GetExpiratoryValveObstruction(), *data);
      out.push_back(data.release());
    }
    if (in.HasInspiratoryValveLeak()) {
      auto data = std::make_unique<CDM::InspiratoryValveLeakData>();
      io::AnesthesiaActions::Marshall(*in.GetInspiratoryValveLeak(), *data);
      out.push_back(data.release());
    }
    if (in.HasInspiratoryValveObstruction()) {
      auto data = std::make_unique<CDM::InspiratoryValveObstructionData>();
      io::AnesthesiaActions::Marshall(*in.GetInspiratoryValveObstruction(), *data);
      out.push_back(data.release());
    }
    if (in.HasMaskLeak()) {
      auto data = std::make_unique<CDM::MaskLeakData>();
      io::AnesthesiaActions::Marshall(*in.GetMaskLeak(), *data);
      out.push_back(data.release());
    }
    if (in.HasSodaLimeFailure()) {
      auto data = std::make_unique<CDM::SodaLimeFailureData>();
      io::AnesthesiaActions::Marshall(*in.GetSodaLimeFailure(), *data);
      out.push_back(data.release());
    }
    if (in.HasTubeCuffLeak()) {
      auto data = std::make_unique<CDM::TubeCuffLeakData>();
      io::AnesthesiaActions::Marshall(*in.GetTubeCuffLeak(), *data);
      out.push_back(data.release());
    }
    if (in.HasVaporizerFailure()) {
      auto data = std::make_unique<CDM::VaporizerFailureData>();
      io::AnesthesiaActions::Marshall(*in.GetVaporizerFailure(), *data);
      out.push_back(data.release());
    }
    if (in.HasVentilatorPressureLoss()) {
      auto data = std::make_unique<CDM::VentilatorPressureLossData>();
      io::AnesthesiaActions::Marshall(*in.GetVentilatorPressureLoss(), *data);
      out.push_back(data.release());
    }
    if (in.HasYPieceDisconnect()) {
      auto data = std::make_unique<CDM::YPieceDisconnectData>();
      io::AnesthesiaActions::Marshall(*in.GetYPieceDisconnect(), *data);
      out.push_back(data.release());
    }
  }
  // class SEAnesthesiaMachineAction
  void AnesthesiaActions::UnMarshall(const CDM::AnesthesiaMachineActionData& in, SEAnesthesiaMachineAction& out, std::default_random_engine* rd)
  {
    io::Actions::UnMarshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SEAnesthesiaMachineAction& in, CDM::AnesthesiaMachineActionData& out)
  {
    io::Actions::Marshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SEAnesthesiaMachineConfiguration
  void AnesthesiaActions::UnMarshall(const CDM::AnesthesiaMachineConfigurationData& in, SEAnesthesiaMachineConfiguration& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    if (in.ConfigurationFile().present())
      out.SetConfigurationFile(in.ConfigurationFile().get());
    io::Anesthesia::UnMarshall(in.Configuration(), out.GetConfiguration());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SEAnesthesiaMachineConfiguration& in, CDM::AnesthesiaMachineConfigurationData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));

    if (in.HasConfiguration())
      io::Anesthesia::Marshall(*in.GetConfiguration(), out.Configuration());
    else if (in.HasConfigurationFile())
      out.ConfigurationFile(in.GetConfigurationFile());
  }
  //----------------------------------------------------------------------------------
  // class SEExpiratoryValveLeak
  void AnesthesiaActions::UnMarshall(const CDM::ExpiratoryValveLeakData& in, SEExpiratoryValveLeak& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SEExpiratoryValveLeak& in, CDM::ExpiratoryValveLeakData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEExpiratoryValveObstruction
  void AnesthesiaActions::UnMarshall(const CDM::ExpiratoryValveObstructionData& in, SEExpiratoryValveObstruction& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SEExpiratoryValveObstruction& in, CDM::ExpiratoryValveObstructionData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEInspiratoryValveLeak
  void AnesthesiaActions::UnMarshall(const CDM::InspiratoryValveLeakData& in, SEInspiratoryValveLeak& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SEInspiratoryValveLeak& in, CDM::InspiratoryValveLeakData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEInspiratoryValveObstruction
  void AnesthesiaActions::UnMarshall(const CDM::InspiratoryValveObstructionData& in, SEInspiratoryValveObstruction& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SEInspiratoryValveObstruction& in, CDM::InspiratoryValveObstructionData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEMaskLeak
  void AnesthesiaActions::UnMarshall(const CDM::MaskLeakData& in, SEMaskLeak& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SEMaskLeak& in, CDM::MaskLeakData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SESodaLimeFailure
  void AnesthesiaActions::UnMarshall(const CDM::SodaLimeFailureData& in, SESodaLimeFailure& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SESodaLimeFailure& in, CDM::SodaLimeFailureData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SETubeCuffLeak
  void AnesthesiaActions::UnMarshall(const CDM::TubeCuffLeakData& in, SETubeCuffLeak& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SETubeCuffLeak& in, CDM::TubeCuffLeakData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEVaporizerFailure
  void AnesthesiaActions::UnMarshall(const CDM::VaporizerFailureData& in, SEVaporizerFailure& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SEVaporizerFailure& in, CDM::VaporizerFailureData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEVentilatorPressureLoss
  void AnesthesiaActions::UnMarshall(const CDM::VentilatorPressureLossData& in, SEVentilatorPressureLoss& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SEVentilatorPressureLoss& in, CDM::VentilatorPressureLossData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEYPieceDisconnect
  void AnesthesiaActions::UnMarshall(const CDM::YPieceDisconnectData& in, SEYPieceDisconnect& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    io::Property::UnMarshall(in.Severity(), out.GetSeverity(), rd);
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SEYPieceDisconnect& in, CDM::YPieceDisconnectData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Severity)
  }
  //----------------------------------------------------------------------------------
  // class SEOxygenWallPortPressureLoss
  void AnesthesiaActions::UnMarshall(const CDM::OxygenWallPortPressureLossData& in, SEOxygenWallPortPressureLoss& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    out.SetActive(in.State() == SEOnOff::On ? true : false);
  }
  //----------------------------------------------------------------------------------
  void AnesthesiaActions::Marshall(const SEOxygenWallPortPressureLoss& in, CDM::OxygenWallPortPressureLossData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    
    out.State(in.IsActive() ? CDM::enumOnOff::On : CDM::enumOnOff::Off);
  }
  //----------------------------------------------------------------------------------
  // class SEOxygenTankPressureLoss
  void AnesthesiaActions::UnMarshall(const CDM::OxygenTankPressureLossData& in, SEOxygenTankPressureLoss& out, std::default_random_engine* rd)
  {
    UnMarshall(static_cast<const CDM::AnesthesiaMachineActionData&>(in), static_cast<SEAnesthesiaMachineAction&>(out));
    out.SetActive(in.State() == SEOnOff::On ? true : false);
  }
  //----------------------------------------------------------------------------------

  void AnesthesiaActions::Marshall(const SEOxygenTankPressureLoss& in, CDM::OxygenTankPressureLossData& out)
  {
    Marshall(static_cast<const SEAnesthesiaMachineAction&>(in), static_cast<CDM::AnesthesiaMachineActionData&>(out));
    out.State(std::make_unique<std::remove_reference<decltype(out.State())>::type>()); 
    io::Property::Marshall(in.m_State, out.State());
  }

  //----------------------------------------------------------------------------------
  std::unique_ptr<CDM::AnesthesiaMachineActionData> AnesthesiaActions::factory(const SEAnesthesiaMachineAction* anesthesiaMachineAction)
  {
    if (auto oxygenTankPressureLoss = dynamic_cast<SEOxygenTankPressureLoss const*>(anesthesiaMachineAction); oxygenTankPressureLoss) {
      auto oxygenTankPressureLossData = std::make_unique<CDM::OxygenTankPressureLossData>();
      Marshall(*oxygenTankPressureLoss, *oxygenTankPressureLossData);
      return std::move(oxygenTankPressureLossData);
    }

    if (auto oxygenWallPortPressureLoss = dynamic_cast<SEOxygenWallPortPressureLoss const*>(anesthesiaMachineAction); oxygenWallPortPressureLoss) {
      auto oxygenWallPortPressureLossData = std::make_unique<CDM::OxygenWallPortPressureLossData>();
      Marshall(*oxygenWallPortPressureLoss, *oxygenWallPortPressureLossData);
      return std::move(oxygenWallPortPressureLossData);
    }

    if (auto yPieceDisconnect = dynamic_cast<SEYPieceDisconnect const*>(anesthesiaMachineAction); yPieceDisconnect) {
      auto yPieceDisconnectData = std::make_unique<CDM::YPieceDisconnectData>();
      Marshall(*yPieceDisconnect, *yPieceDisconnectData);
      return std::move(yPieceDisconnectData);
    }

    if (auto ventilatorPressureLoss = dynamic_cast<SEVentilatorPressureLoss const*>(anesthesiaMachineAction); ventilatorPressureLoss) {
      auto ventilatorPressureLossData = std::make_unique<CDM::VentilatorPressureLossData>();
      Marshall(*ventilatorPressureLoss, *ventilatorPressureLossData);
      return std::move(ventilatorPressureLossData);
    }

    if (auto vaporizerFailure = dynamic_cast<SEVaporizerFailure const*>(anesthesiaMachineAction); vaporizerFailure) {
      auto vaporizerFailureData = std::make_unique<CDM::VaporizerFailureData>();
      Marshall(*vaporizerFailure, *vaporizerFailureData);
      return std::move(vaporizerFailureData);
    }

    if (auto tubeCuffLeak = dynamic_cast<SETubeCuffLeak const*>(anesthesiaMachineAction); tubeCuffLeak) {
      auto tubeCuffLeakData = std::make_unique<CDM::TubeCuffLeakData>();
      Marshall(*tubeCuffLeak, *tubeCuffLeakData);
      return std::move(tubeCuffLeakData);
    }

    if (auto sodaLimeFailure = dynamic_cast<SESodaLimeFailure const*>(anesthesiaMachineAction); sodaLimeFailure) {
      auto sodaLimeFailureData = std::make_unique<CDM::SodaLimeFailureData>();
      Marshall(*sodaLimeFailure, *sodaLimeFailureData);
      return std::move(sodaLimeFailureData);
    }

    if (auto maskLeak = dynamic_cast<SEMaskLeak const*>(anesthesiaMachineAction); maskLeak) {
      auto maskLeakData = std::make_unique<CDM::MaskLeakData>();
      Marshall(*maskLeak, *maskLeakData);
      return std::move(maskLeakData);
    }

    if (auto inspiratoryValveObstruction = dynamic_cast<SEInspiratoryValveObstruction const*>(anesthesiaMachineAction); inspiratoryValveObstruction) {
      auto inspiratoryValveObstructionData = std::make_unique<CDM::InspiratoryValveObstructionData>();
      Marshall(*inspiratoryValveObstruction, *inspiratoryValveObstructionData);
      return std::move(inspiratoryValveObstructionData);
    }

    if (auto inspiratoryValveLeak = dynamic_cast<SEInspiratoryValveLeak const*>(anesthesiaMachineAction); inspiratoryValveLeak) {
      auto inspiratoryValveLeakData = std::make_unique<CDM::InspiratoryValveLeakData>();
      Marshall(*inspiratoryValveLeak, *inspiratoryValveLeakData);
      return std::move(inspiratoryValveLeakData);
    }

    if (auto expiratoryValveObstruction = dynamic_cast<SEExpiratoryValveObstruction const*>(anesthesiaMachineAction); expiratoryValveObstruction) {
      auto expiratoryValveObstructionData = std::make_unique<CDM::ExpiratoryValveObstructionData>();
      Marshall(*expiratoryValveObstruction, *expiratoryValveObstructionData);
      return std::move(expiratoryValveObstructionData);
    }

    if (auto expiratoryValveLeak = dynamic_cast<SEExpiratoryValveLeak const*>(anesthesiaMachineAction); expiratoryValveLeak) {
      auto expiratoryValveLeakData = std::make_unique<CDM::ExpiratoryValveLeakData>();
      Marshall(*expiratoryValveLeak, *expiratoryValveLeakData);
      return std::move(expiratoryValveLeakData);
    }

    if (auto anesthesiaMachineConfiguration = dynamic_cast<SEAnesthesiaMachineConfiguration const*>(anesthesiaMachineAction); anesthesiaMachineConfiguration) {
      auto anesthesiaMachineConfigurationData = std::make_unique<CDM::AnesthesiaMachineConfigurationData>();
      Marshall(*anesthesiaMachineConfiguration, *anesthesiaMachineConfigurationData);
      return std::move(anesthesiaMachineConfigurationData);
    }
    throw biogears::CommonDataModelException("AnesthesiaActions::factory does not support the derived SEAnesthesiaMachineAction. If you are not a developer contact upstream for support.");
  }
}
}