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
#include <random>

#include "biogears/cdm/CommonDataModel.h"
#include <biogears/exports.h>

#include <biogears/schema/cdm/AnesthesiaActions.hxx>

#define CDM_ANESTHESIA_ACTIONS_PTR_MARSHALL_HELPER(in, out, func)                       \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::AnesthesiaActions::Marshall(*in.m_##func, out.func());                     \
  }

#define CDM_OPTIONAL_ANESTHESIA_ACTIONS_PTR_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                                    \
    io::AnesthesiaActions::Marshall(*in.m_##func, out.func());        \
  }

namespace biogears {
class SEAction;
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
class SESubstanceManager;

namespace io {
  class BIOGEARS_PRIVATE_API AnesthesiaActions {
  public:
    //Factories
    static std::unique_ptr<CDM::AnesthesiaMachineActionData> factory(const SEAnesthesiaMachineAction*);
    static std::unique_ptr<SEAction> factory(CDM::AnesthesiaMachineActionData const* patientActionData, SESubstanceManager& substances, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEAnesthesiaMachineActionCollection& in, std::vector<std::unique_ptr<CDM::ActionData>>& out);
    //
    static void Marshall(const SEAnesthesiaMachineActionCollection&, std::vector<CDM::ActionData*>& out);
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SEAnesthesiaMachineAction
    static void UnMarshall(const CDM::AnesthesiaMachineActionData& in, SEAnesthesiaMachineAction& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEAnesthesiaMachineAction& in, CDM::AnesthesiaMachineActionData& out);
    //class SEAnesthesiaMachineConfiguration
    static void UnMarshall(const CDM::AnesthesiaMachineConfigurationData& in, SEAnesthesiaMachineConfiguration& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEAnesthesiaMachineConfiguration& in, CDM::AnesthesiaMachineConfigurationData& out);
    //class SEExpiratoryValveLeak
    static void UnMarshall(const CDM::ExpiratoryValveLeakData& in, SEExpiratoryValveLeak& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEExpiratoryValveLeak& in, CDM::ExpiratoryValveLeakData& out);
    //class SEExpiratoryValveObstruction
    static void UnMarshall(const CDM::ExpiratoryValveObstructionData& in, SEExpiratoryValveObstruction& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEExpiratoryValveObstruction& in, CDM::ExpiratoryValveObstructionData& out);
    //class SEInspiratoryValveLeak
    static void UnMarshall(const CDM::InspiratoryValveLeakData& in, SEInspiratoryValveLeak& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEInspiratoryValveLeak& in, CDM::InspiratoryValveLeakData& out);
    //class SEInspiratoryValveObstruction
    static void UnMarshall(const CDM::InspiratoryValveObstructionData& in, SEInspiratoryValveObstruction& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEInspiratoryValveObstruction& in, CDM::InspiratoryValveObstructionData& out);
    //class SEMaskLeak
    static void UnMarshall(const CDM::MaskLeakData& in, SEMaskLeak& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEMaskLeak& in, CDM::MaskLeakData& out);
    //class SESodaLimeFailure
    static void UnMarshall(const CDM::SodaLimeFailureData& in, SESodaLimeFailure& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SESodaLimeFailure& in, CDM::SodaLimeFailureData& out);
    //class SETubeCuffLeak
    static void UnMarshall(const CDM::TubeCuffLeakData& in, SETubeCuffLeak& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SETubeCuffLeak& in, CDM::TubeCuffLeakData& out);
    //class SEVaporizerFailure
    static void UnMarshall(const CDM::VaporizerFailureData& in, SEVaporizerFailure& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEVaporizerFailure& in, CDM::VaporizerFailureData& out);
    //class SEVentilatorPressureLoss    
    static void UnMarshall(const CDM::VentilatorPressureLossData& in, SEVentilatorPressureLoss& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEVentilatorPressureLoss& in, CDM::VentilatorPressureLossData& out);
    //class SEYPieceDisconnect
    static void UnMarshall(const CDM::YPieceDisconnectData& in, SEYPieceDisconnect& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEYPieceDisconnect& in, CDM::YPieceDisconnectData& out);
    //class SEOxygenWallPortPressureLoss
    static void UnMarshall(const CDM::OxygenWallPortPressureLossData& in, SEOxygenWallPortPressureLoss& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEOxygenWallPortPressureLoss& in, CDM::OxygenWallPortPressureLossData& out);
    //class SEOxygenTankPressureLoss
    static void UnMarshall(const CDM::OxygenTankPressureLossData& in, SEOxygenTankPressureLoss& out, std::default_random_engine* rd = nullptr);
    static void Marshall(const SEOxygenTankPressureLoss& in, CDM::OxygenTankPressureLossData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void AnesthesiaActions::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void AnesthesiaActions::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
