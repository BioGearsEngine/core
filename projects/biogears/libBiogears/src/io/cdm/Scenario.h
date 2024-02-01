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

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/schema/cdm/Scenario.hxx>

#define CDM_SCENARIO_MARSHALL_HELPER(in, out, func)                                 \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Scenario::Marshall(*in.m_##func, out.func());                              \
  }

#define CDM_OPTIONAL_SCENARIO_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                          \
    io::Scenario::Marshall(*in.m_##func, out.func());       \
  }
namespace biogears {

class SEDecimalFormat;
class SESubstanceManager;
class SECondition;
class SEAction;
class SEDataRequest;
class SEPatientDataRequest;
class SEPhysiologyDataRequest;
class SEEnvironmentDataRequest;
class SEEquipmentDataRequest;
class SECompartmentDataRequest;
class SECompartmentSubstanceDataRequest;
class SEGasCompartmentDataRequest;
class SELiquidCompartmentDataRequest;
class SEThermalCompartmentDataRequest;
class SETissueCompartmentDataRequest;
class SESubstanceDataRequest;
class SEDataRequestManager;
class SEScenario;
class SEScenarioInitialParameters;
class SEScenarioAutoSerialization;
class SEAdvanceTime;
class SESerializeState;
class SEAnesthesiaMachineActionCollection;
class SEEnvironmentActionCollection;
class SEInhalerActionCollection;
class SEPatientActionCollection;

namespace io {
  class BIOGEARS_PRIVATE_API Scenario {
  public:
    //ActionCollection

    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SECondition;
    static void UnMarshall(const CDM::ConditionData& in, SECondition& out);
    static void Marshall(const SECondition& in, CDM::ConditionData& out);
    //class SEAction;
    static void UnMarshall(const CDM::ActionData& in, SEAction& out);
    static void Marshall(const SEAction& in, CDM::ActionData& out);
    //class SEDataRequestManager
    static void UnMarshall(const CDM::DataRequestManagerData& in, const SESubstanceManager& subMgr, SEDataRequestManager& out);
    static void Marshall(const SEDataRequestManager& in, CDM::DataRequestManagerData& out);
    //class SEDataRequest;
    static void UnMarshall(const CDM::DataRequestData& in, SEDataRequest& out);
    static void Marshall(const SEDataRequest& in, CDM::DataRequestData& out);
    //class SEPatientDataRequest;
    static void UnMarshall(const CDM::PatientDataRequestData& in, SEPatientDataRequest& out);
    static void Marshall(const SEPatientDataRequest& in, CDM::PatientDataRequestData& out);
    //class SEPhysiologyDataRequest;
    static void UnMarshall(const CDM::PhysiologyDataRequestData& in, SEPhysiologyDataRequest& out);
    static void Marshall(const SEPhysiologyDataRequest& in, CDM::PhysiologyDataRequestData& out);
    //class SEEnvironmentDataRequest;
    static void UnMarshall(const CDM::EnvironmentDataRequestData& in, SEEnvironmentDataRequest& out);
    static void Marshall(const SEEnvironmentDataRequest& in, CDM::EnvironmentDataRequestData& out);
    //class SEEquipmentDataRequest;
    static void UnMarshall(const CDM::EquipmentDataRequestData& in, SEEquipmentDataRequest& out);
    static void Marshall(const SEEquipmentDataRequest& in, CDM::EquipmentDataRequestData& out);
    //class SECompartmentDataRequest;
    static void UnMarshall(const CDM::CompartmentDataRequestData& in, SECompartmentDataRequest& out);
    static void Marshall(const SECompartmentDataRequest& in, CDM::CompartmentDataRequestData& out);
    //class SECompartmentSubstanceDataRequest;
    static void UnMarshall(const CDM::CompartmentSubstanceDataRequestData& in, SESubstanceManager const& substances, SECompartmentSubstanceDataRequest& out);
    static void Marshall(const SECompartmentSubstanceDataRequest& in, CDM::CompartmentSubstanceDataRequestData& out);
    //class SEGasCompartmentDataRequest;
    static void UnMarshall(const CDM::GasCompartmentDataRequestData& in, SESubstanceManager const& substances, SEGasCompartmentDataRequest& out);
    static void Marshall(const SEGasCompartmentDataRequest& in, CDM::GasCompartmentDataRequestData& out);
    //class SELiquidCompartmentDataRequest;
    static void UnMarshall(const CDM::LiquidCompartmentDataRequestData& in, SESubstanceManager const& substances, SELiquidCompartmentDataRequest& out);
    static void Marshall(const SELiquidCompartmentDataRequest& in, CDM::LiquidCompartmentDataRequestData& out);
    //class SEThermalCompartmentDataRequest;
    static void UnMarshall(const CDM::ThermalCompartmentDataRequestData& in, SEThermalCompartmentDataRequest& out);
    static void Marshall(const SEThermalCompartmentDataRequest& in, CDM::ThermalCompartmentDataRequestData& out);
    //class SETissueCompartmentDataRequest;
    static void UnMarshall(const CDM::TissueCompartmentDataRequestData& in, SETissueCompartmentDataRequest& out);
    static void Marshall(const SETissueCompartmentDataRequest& in, CDM::TissueCompartmentDataRequestData& out);
    //class SESubstanceDataRequest;
    static void UnMarshall(const CDM::SubstanceDataRequestData& in, SESubstanceManager const& substances, SESubstanceDataRequest& out);
    static void Marshall(const SESubstanceDataRequest& in, CDM::SubstanceDataRequestData& out);
    //class SEScenario;
    static void UnMarshall(const CDM::ScenarioData& in, SEScenario& out);
    static void Marshall(const SEScenario& in, CDM::ScenarioData& out);
    //class SEScenarioInitialParameters;
    static void UnMarshall(const CDM::ScenarioInitialParametersData& in, SEScenarioInitialParameters& out);
    static void Marshall(const SEScenarioInitialParameters& in, CDM::ScenarioInitialParametersData& out);
    //class SEScenarioAutoSerialization;
    static void UnMarshall(const CDM::ScenarioAutoSerializationData& in, SEScenarioAutoSerialization& out);
    static void Marshall(const SEScenarioAutoSerialization& in, CDM::ScenarioAutoSerializationData& out);
    //class SEAdvanceTime;
    static void UnMarshall(const CDM::AdvanceTimeData& in, SEAdvanceTime& out);
    static void Marshall(const SEAdvanceTime& in, CDM::AdvanceTimeData& out);
    //class SESerializeState;
    static void UnMarshall(const CDM::SerializeStateData& in, SESerializeState& out);
    static void Marshall(const SESerializeState& in, CDM::SerializeStateData& out);

    static std::unique_ptr<SEDataRequest> factory(const CDM::DataRequestData& in, SESubstanceManager const& substances,  SEDecimalFormat const* df = nullptr);

    static std::unique_ptr<CDM::DataRequestData> factory(const SEDataRequest* in);
    static std::unique_ptr<CDM::ActionData> factory(const SEAction* in);
  };

  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Scenario::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Scenario::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
}
}
