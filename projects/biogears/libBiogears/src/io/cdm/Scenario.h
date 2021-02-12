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

#include "biogears/cdm/CommonDataModel.h"
#include <biogears/exports.h>

#include "biogears/cdm/substance/SESubstanceManager.h"
#include <biogears/schema/cdm/Scenario.hxx>

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

#define CDM_SCENARIO_UNMARSHAL_HELPER(xsd, func)                                     \
  if (m_##func) {                                                                    \
    xsd.func(std::make_unique<std::remove_reference<decltype(xsd.func())>::type>()); \
    io::Property::UnMarshall(*m_##func, xsd.func());                                 \
  }

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
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SECondition;
    static void Marshall(const CDM::ConditionData& in, SECondition& out);
    static void UnMarshall(const SECondition& in, CDM::ConditionData& out);
    //class SEAction;
    static void Marshall(const CDM::ActionData& in, SEAction& out);
    static void UnMarshall(const SEAction& in, CDM::ActionData& out);
    //class SEDataRequestManager
    static void Marshall(const CDM::DataRequestsData& in, const SESubstanceManager& subMgr, SEDataRequestManager& out);
    static void UnMarshall(const SEDataRequestManager& in, CDM::DataRequestsData& out);
    //class SEDataRequest;
    static void Marshall(const CDM::DataRequestData& in, SEDataRequest& out);
    static void UnMarshall(const SEDataRequest& in, CDM::DataRequestData& out);
    //class SEPatientDataRequest;
    static void Marshall(const CDM::PatientDataRequestData& in, SEPatientDataRequest& out);
    static void UnMarshall(const SEPatientDataRequest& in, CDM::PatientDataRequestData& out);
    //class SEPhysiologyDataRequest;
    static void Marshall(const CDM::PhysiologyDataRequestData& in, SEPhysiologyDataRequest& out);
    static void UnMarshall(const SEPhysiologyDataRequest& in, CDM::PhysiologyDataRequestData& out);
    //class SEEnvironmentDataRequest;
    static void Marshall(const CDM::EnvironmentDataRequestData& in, SEEnvironmentDataRequest& out);
    static void UnMarshall(const SEEnvironmentDataRequest& in, CDM::EnvironmentDataRequestData& out);
    //class SEEquipmentDataRequest;
    static void Marshall(const CDM::EquipmentDataRequestData& in, SEEquipmentDataRequest& out);
    static void UnMarshall(const SEEquipmentDataRequest& in, CDM::EquipmentDataRequestData& out);
    //class SECompartmentDataRequest;
    static void Marshall(const CDM::CompartmentDataRequestData& in, SECompartmentDataRequest& out);
    static void UnMarshall(const SECompartmentDataRequest& in, CDM::CompartmentDataRequestData& out);
    //class SECompartmentSubstanceDataRequest;
    static void Marshall(const CDM::CompartmentSubstanceDataRequestData& in, SESubstanceManager const& substances, SECompartmentSubstanceDataRequest& out);
    static void UnMarshall(const SECompartmentSubstanceDataRequest& in, CDM::CompartmentSubstanceDataRequestData& out);
    //class SEGasCompartmentDataRequest;
    static void Marshall(const CDM::GasCompartmentDataRequestData& in, SESubstanceManager const& substances, SEGasCompartmentDataRequest& out);
    static void UnMarshall(const SEGasCompartmentDataRequest& in, CDM::GasCompartmentDataRequestData& out);
    //class SELiquidCompartmentDataRequest;
    static void Marshall(const CDM::LiquidCompartmentDataRequestData& in, SESubstanceManager const& substances, SELiquidCompartmentDataRequest& out);
    static void UnMarshall(const SELiquidCompartmentDataRequest& in, CDM::LiquidCompartmentDataRequestData& out);
    //class SEThermalCompartmentDataRequest;
    static void Marshall(const CDM::ThermalCompartmentDataRequestData& in, SEThermalCompartmentDataRequest& out);
    static void UnMarshall(const SEThermalCompartmentDataRequest& in, CDM::ThermalCompartmentDataRequestData& out);
    //class SETissueCompartmentDataRequest;
    static void Marshall(const CDM::TissueCompartmentDataRequestData& in, SETissueCompartmentDataRequest& out);
    static void UnMarshall(const SETissueCompartmentDataRequest& in, CDM::TissueCompartmentDataRequestData& out);
    //class SESubstanceDataRequest;
    static void Marshall(const CDM::SubstanceDataRequestData& in, SESubstanceManager const& substances, SESubstanceDataRequest& out);
    static void UnMarshall(const SESubstanceDataRequest& in, CDM::SubstanceDataRequestData& out);
    //class SEScenario;
    static void Marshall(const CDM::ScenarioData& in, SEScenario& out);
    static void UnMarshall(const SEScenario& in, CDM::ScenarioData& out);
    //class SEScenarioInitialParameters;
    static void Marshall(const CDM::ScenarioInitialParametersData& in, SEScenarioInitialParameters& out);
    static void UnMarshall(const SEScenarioInitialParameters& in, CDM::ScenarioInitialParametersData& out);
    //class SEScenarioAutoSerialization;
    static void Marshall(const CDM::ScenarioAutoSerializationData& in, SEScenarioAutoSerialization& out);
    static void UnMarshall(const SEScenarioAutoSerialization& in, CDM::ScenarioAutoSerializationData& out);
    //class SEAdvanceTime;
    static void Marshall(const CDM::AdvanceTimeData& in, SEAdvanceTime& out);
    static void UnMarshall(const SEAdvanceTime& in, CDM::AdvanceTimeData& out);
    //class SESerializeState;
    static void Marshall(const CDM::SerializeStateData& in, SESerializeState& out);
    static void UnMarshall(const SESerializeState& in, CDM::SerializeStateData& out);

    static std::unique_ptr<SEDataRequest> factory(const CDM::DataRequestData& in, SESubstanceManager const& substances, const SEDecimalFormat* default = nullptr);
    static std::unique_ptr<CDM::DataRequestData> factory(const SEDataRequest* in);
  };

  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Scenario::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Scenario::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
}
}
