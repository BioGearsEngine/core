/**************************************************************************************
Copyright 2019 Applied Research Associates, Inc.
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
#include <memory>

#include "biogears/cdm/CommonDataModel.h"
#include <biogears/exports.h>

#include <biogears/schema/cdm/AnesthesiaActions.hxx>

namespace biogears {
class SEAnesthesiaMachineActionCollection;
class SEAnesthesiaMachineAction;
class SEAnesthesiaMachineConfiguration;
class SEExpiratoryValveLeak;
class SEExpiratoryValveObstruction;
class SEInspiratoryValveLeak;
class SEInspiratoryValveObstruction;
class SEMaskLeak;
class SESodaLimeFailure;
class SETubeCuffLeak;
class SEVaporizerFailure;
class SEVentilatorPressureLoss;
class SEYPieceDisconnect;
class SEOxygenWallPortPressureLoss;
class SEOxygenTankPressureLoss;

#define CDM_ANESTHESIA_ACTIONS_UNMARSHAL_HELPER(xsd, func)                              \
  if (m_##func) {                                                                    \
    xsd.func(std::make_unique<std::remove_reference<decltype(xsd.func())>::type>()); \
    io::Property::UnMarshall(*m_##func, xsd.func());                       \
  }
namespace io {
  class BIOGEARS_PRIVATE_API AnesthesiaActions {
  public:
    //
    static void UnMarshall(const SEAnesthesiaMachineActionCollection&, std::vector<CDM::ActionData*>& out);
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SEAnesthesiaMachineAction
    static void Marshall(const CDM::AnesthesiaMachineActionData& in, SEAnesthesiaMachineAction& out);
    static void UnMarshall(const SEAnesthesiaMachineAction& in, CDM::AnesthesiaMachineActionData& out);
    //class SEAnesthesiaMachineConfiguration
    static void Marshall(const CDM::AnesthesiaMachineConfigurationData& in, SEAnesthesiaMachineConfiguration& out);
    static void UnMarshall(const SEAnesthesiaMachineConfiguration& in, CDM::AnesthesiaMachineConfigurationData& out);
    //class SEExpiratoryValveLeak
    static void Marshall(const CDM::ExpiratoryValveLeakData& in, SEExpiratoryValveLeak& out);
    static void UnMarshall(const SEExpiratoryValveLeak& in, CDM::ExpiratoryValveLeakData& out);
    //class SEExpiratoryValveObstruction
    static void Marshall(const CDM::ExpiratoryValveObstructionData& in, SEExpiratoryValveObstruction& out);
    static void UnMarshall(const SEExpiratoryValveObstruction& in, CDM::ExpiratoryValveObstructionData& out);
    //class SEInspiratoryValveLeak
    static void Marshall(const CDM::InspiratoryValveLeakData& in, SEInspiratoryValveLeak& out);
    static void UnMarshall(const SEInspiratoryValveLeak& in, CDM::InspiratoryValveLeakData& out);
    //class SEInspiratoryValveObstruction
    static void Marshall(const CDM::InspiratoryValveObstructionData& in, SEInspiratoryValveObstruction& out);
    static void UnMarshall(const SEInspiratoryValveObstruction& in, CDM::InspiratoryValveObstructionData& out);
    //class SEMaskLeak
    static void Marshall(const CDM::MaskLeakData& in, SEMaskLeak& out);
    static void UnMarshall(const SEMaskLeak& in, CDM::MaskLeakData& out);
    //class SESodaLimeFailure
    static void Marshall(const CDM::SodaLimeFailureData& in, SESodaLimeFailure& out);
    static void UnMarshall(const SESodaLimeFailure& in, CDM::SodaLimeFailureData& out);
    //class SETubeCuffLeak
    static void Marshall(const CDM::TubeCuffLeakData& in, SETubeCuffLeak& out);
    static void UnMarshall(const SETubeCuffLeak& in, CDM::TubeCuffLeakData& out);
    //class SEVaporizerFailure
    static void Marshall(const CDM::VaporizerFailureData& in, SEVaporizerFailure& out);
    static void UnMarshall(const SEVaporizerFailure& in, CDM::VaporizerFailureData& out);
    //class SEVentilatorPressureLoss    
    static void Marshall(const CDM::VentilatorPressureLossData& in, SEVentilatorPressureLoss& out);
    static void UnMarshall(const SEVentilatorPressureLoss& in, CDM::VentilatorPressureLossData& out);
    //class SEYPieceDisconnect
    static void Marshall(const CDM::YPieceDisconnectData& in, SEYPieceDisconnect& out);
    static void UnMarshall(const SEYPieceDisconnect& in, CDM::YPieceDisconnectData& out);
    //class SEOxygenWallPortPressureLoss
    static void Marshall(const CDM::OxygenWallPortPressureLossData& in, SEOxygenWallPortPressureLoss& out);
    static void UnMarshall(const SEOxygenWallPortPressureLoss& in, CDM::OxygenWallPortPressureLossData& out);
    //class SEOxygenTankPressureLoss
    static void Marshall(const CDM::OxygenTankPressureLossData& in, SEOxygenTankPressureLoss& out);
    static void UnMarshall(const SEOxygenTankPressureLoss& in, CDM::OxygenTankPressureLossData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void AnesthesiaActions::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void AnesthesiaActions::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
