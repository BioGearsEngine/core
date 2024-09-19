#include "AnesthesiaActions.h"

#include "Anesthesia.h"
#include "Property.h"
#include "Scenario.h"

#include <biogears/cdm/properties/SEScalar0To1.h>

#include <biogears/cdm/scenario/SEAnesthesiaMachineActionCollection.h>

#include <biogears/cdm/scenario/SEAction.h>
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

#define POLYMORPHIC_MARSHALL(paramName, typeName)                               \
  if (auto typeName = dynamic_cast<SE##typeName const*>(paramName); typeName) { \
    auto typeName##Data = std::make_unique<CDM::typeName##Data>();              \
    Marshall(*typeName, *typeName##Data);                                       \
    return std::move(typeName##Data);                                           \
  }

#define POLYMORPHIC_UNMARSHALL(paramName, typeName, schema)                                        \
  if (auto typeName##Data = dynamic_cast<CDM::typeName##Data const*>(paramName); typeName##Data) { \
    auto typeName = std::make_unique<SE##typeName>();                                              \
    schema::UnMarshall(*typeName##Data, *typeName);                                                \
    return std::move(typeName);                                                                    \
  }

#define STOCASTIC_POLYMORPHIC_UNMARSHALL(paramName, typeName, schema, engine)                      \
  if (auto typeName##Data = dynamic_cast<CDM::typeName##Data const*>(paramName); typeName##Data) { \
    auto typeName = std::make_unique<SE##typeName>();                                              \
    schema::UnMarshall(*typeName##Data, *typeName, engine);                                        \
    return std::move(typeName);                                                                    \
  }
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
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, OxygenTankPressureLoss)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, OxygenWallPortPressureLoss)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, YPieceDisconnect)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, VentilatorPressureLoss)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, VaporizerFailure)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, TubeCuffLeak)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, SodaLimeFailure)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, MaskLeak)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, InspiratoryValveObstruction)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, InspiratoryValveLeak)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, ExpiratoryValveObstruction)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, ExpiratoryValveLeak)
    POLYMORPHIC_MARSHALL(anesthesiaMachineAction, AnesthesiaMachineConfiguration)

    throw biogears::CommonDataModelException("AnesthesiaActions::factory does not support the derived SEAnesthesiaMachineAction. If you are not a developer contact upstream for support.");
  }

  std::unique_ptr<SEAction> AnesthesiaActions::factory(CDM::AnesthesiaMachineActionData const* anesthesiaMachineActionData, SESubstanceManager& substances, std::default_random_engine* rd)
  {

    if (auto AnesthesiaMachineConfigurationData = dynamic_cast<CDM::AnesthesiaMachineConfigurationData const*>(anesthesiaMachineActionData); AnesthesiaMachineConfigurationData) {
      auto AnesthesiaMachineConfiguration = std::make_unique<SEAnesthesiaMachineConfiguration>(substances);
      AnesthesiaActions::UnMarshall(*AnesthesiaMachineConfigurationData, *AnesthesiaMachineConfiguration, rd);
      return std::move(AnesthesiaMachineConfiguration);
    }

    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, OxygenWallPortPressureLoss, AnesthesiaActions, rd)
    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, OxygenTankPressureLoss, AnesthesiaActions, rd)
    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, ExpiratoryValveLeak, AnesthesiaActions, rd)
    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, ExpiratoryValveObstruction, AnesthesiaActions, rd)
    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, InspiratoryValveLeak, AnesthesiaActions, rd)
    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, InspiratoryValveObstruction, AnesthesiaActions, rd)
    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, MaskLeak, AnesthesiaActions, rd)
    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, SodaLimeFailure, AnesthesiaActions, rd)
    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, TubeCuffLeak, AnesthesiaActions, rd)
    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, VaporizerFailure, AnesthesiaActions, rd)
    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, VentilatorPressureLoss, AnesthesiaActions, rd)
    STOCASTIC_POLYMORPHIC_UNMARSHALL(anesthesiaMachineActionData, YPieceDisconnect, AnesthesiaActions, rd)
    throw biogears::CommonDataModelException("PatientActions:Factory - Unsupported Anesthesia Machine Action Received.");
  }

  void AnesthesiaActions::Marshall(const SEAnesthesiaMachineActionCollection& in, std::vector<std::unique_ptr<CDM::ActionData>>& out)
  {
    if (in.m_Configuration) {
      out.push_back(AnesthesiaActions::factory(in.m_Configuration));
    }
    // Anesthesia Machine Incidents
    if (in.m_OxygenTankPressureLoss) {
      out.push_back(AnesthesiaActions::factory(in.m_OxygenTankPressureLoss));
    }
    if (in.m_OxygenWallPortPressureLoss) {
      out.push_back(AnesthesiaActions::factory(in.m_OxygenWallPortPressureLoss));
    }
    // Anesthesia Machine Failures
    if (in.m_ExpiratoryValveLeak) {
      out.push_back(AnesthesiaActions::factory(in.m_ExpiratoryValveLeak));
    }
    if (in.m_ExpiratoryValveObstruction) {
      out.push_back(AnesthesiaActions::factory(in.m_ExpiratoryValveObstruction));
    }
    if (in.m_InspiratoryValveLeak) {
      out.push_back(AnesthesiaActions::factory(in.m_InspiratoryValveLeak));
    }
    if (in.m_InspiratoryValveObstruction) {
      out.push_back(AnesthesiaActions::factory(in.m_InspiratoryValveObstruction));
    }
    if (in.m_MaskLeak) {
      out.push_back(AnesthesiaActions::factory(in.m_MaskLeak));
    }
    if (in.m_SodaLimeFailure) {
      out.push_back(AnesthesiaActions::factory(in.m_SodaLimeFailure));
    }
    if (in.m_TubeCuffLeak) {
      out.push_back(AnesthesiaActions::factory(in.m_TubeCuffLeak));
    }
    if (in.m_VaporizerFailure) {
      out.push_back(AnesthesiaActions::factory(in.m_VaporizerFailure));
    }
    if (in.m_VentilatorPressureLoss) {
      out.push_back(AnesthesiaActions::factory(in.m_VentilatorPressureLoss));
    }
    if (in.m_YPieceDisconnect) {
      out.push_back(AnesthesiaActions::factory(in.m_YPieceDisconnect));
    }
  }
}
}