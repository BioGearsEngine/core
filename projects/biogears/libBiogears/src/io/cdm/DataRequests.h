/**************************************************************************************
Copyright 2019 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
DATA_REQUESTS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <random>
#include <type_traits>

#include "DataRequests.h"

#include <biogears/cdm/enums/SEActionEnums.h>
#include <biogears/schema/cdm/DataRequests.hxx>

// Question: To Serialize Invalid units or not to Serialize?
//           TO Throw an exception when a member is invalid?
#define CDM_DATA_REQUESTS_MARSHALL_HELPER(in, out, func)                                     \
  if (in.m_##func && in.m_##func->IsValid()) {                                               \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>());         \
    io::DataRequests::Marshall(*in.m_##func, out.func());                                    \
  } /* else if (in.m_##func) {                                                               \
     throw biogears::CommonDataModelException("func is InValid and cannot be Unmarshalled"); \
   }*/

#define CDM_OPTIONAL_DATA_REQUESTS_MARSHALL_HELPER(in, out, func)                           \
  if (in.m_##func && in.m_##func->IsValid()) {                                              \
    io::DataRequests::Marshall(*in.m_##func, out.func());                                   \
  } /*else if (in.m_##func) {                                                               \
    throw biogears::CommonDataModelException("func is InValid and cannot be Unmarshalled"); \
  }*/

#define SE_DATA_REQUESTS_ENUM_MARSHALL_HELPER(in, out, func)                         \
  if (in.Has##func()) {                                                              \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::DataRequests::Marshall(in.m_##func, out.func());                             \
  }

#define SE_OPTIONAL_DATA_REQUESTS_ENUM_MARSHALL_HELPER(in, out, func) \
  io::DataRequests::Marshall(in.m_##func, out.func());

namespace biogears {

class SEDecimalFormat;

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
class SESubstanceManager;

namespace io {
  class BIOGEARS_PRIVATE_API DataRequests {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd = nullptr);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);

    // class SEDataRequestManager
    static void UnMarshall(const CDM::DataRequestManagerData& in, const SESubstanceManager& subMgr, SEDataRequestManager& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SEDataRequestManager& in, CDM::DataRequestManagerData& out);

    // class SEDataRequest;
    static void UnMarshall(const CDM::DataRequestData& in, SEDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SEDataRequest& in, CDM::DataRequestData& out);

    // class SEPatientDataRequest;
    static void UnMarshall(const CDM::PatientDataRequestData& in, SEPatientDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SEPatientDataRequest& in, CDM::PatientDataRequestData& out);
    // class SEPhysiologyDataRequest;
    static void UnMarshall(const CDM::PhysiologyDataRequestData& in, SEPhysiologyDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SEPhysiologyDataRequest& in, CDM::PhysiologyDataRequestData& out);
    // class SEEnvironmentDataRequest;
    static void UnMarshall(const CDM::EnvironmentDataRequestData& in, SEEnvironmentDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SEEnvironmentDataRequest& in, CDM::EnvironmentDataRequestData& out);
    // class SEEquipmentDataRequest;
    static void UnMarshall(const CDM::EquipmentDataRequestData& in, SEEquipmentDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SEEquipmentDataRequest& in, CDM::EquipmentDataRequestData& out);
    // class SECompartmentDataRequest;
    static void UnMarshall(const CDM::CompartmentDataRequestData& in, SECompartmentDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SECompartmentDataRequest& in, CDM::CompartmentDataRequestData& out);
    // class SECompartmentSubstanceDataRequest;
    static void UnMarshall(const CDM::CompartmentSubstanceDataRequestData& in, SESubstanceManager const& substances, SECompartmentSubstanceDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SECompartmentSubstanceDataRequest& in, CDM::CompartmentSubstanceDataRequestData& out);
    // class SEGasCompartmentDataRequest;
    static void UnMarshall(const CDM::GasCompartmentDataRequestData& in, SESubstanceManager const& substances, SEGasCompartmentDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SEGasCompartmentDataRequest& in, CDM::GasCompartmentDataRequestData& out);
    // class SELiquidCompartmentDataRequest;
    static void UnMarshall(const CDM::LiquidCompartmentDataRequestData& in, SESubstanceManager const& substances, SELiquidCompartmentDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SELiquidCompartmentDataRequest& in, CDM::LiquidCompartmentDataRequestData& out);
    // class SEThermalCompartmentDataRequest;
    static void UnMarshall(const CDM::ThermalCompartmentDataRequestData& in, SEThermalCompartmentDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SEThermalCompartmentDataRequest& in, CDM::ThermalCompartmentDataRequestData& out);
    // class SETissueCompartmentDataRequest;
    static void UnMarshall(const CDM::TissueCompartmentDataRequestData& in, SETissueCompartmentDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SETissueCompartmentDataRequest& in, CDM::TissueCompartmentDataRequestData& out);
    // class SESubstanceDataRequest;
    static void UnMarshall(const CDM::SubstanceDataRequestData& in, SESubstanceManager const& substances, SESubstanceDataRequest& out, std::default_random_engine* re = nullptr);
    static void Marshall(const SESubstanceDataRequest& in, CDM::SubstanceDataRequestData& out);

    static std::unique_ptr<SEDataRequest> factory(const CDM::DataRequestData& in, SESubstanceManager const& substances, const SEDecimalFormat* df = nullptr);
    static std::unique_ptr<CDM::DataRequestData> factory(const SEDataRequest* in);
  };

  //----------------------------------------------------------------------------------

  template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>*>
  void DataRequests::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present() || option_in->empty()) {
      out = SE::Invalid;
    } else {
      UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>*>
  void DataRequests::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      io::DataRequests::UnMarshall(option_in.get(), out, rd);
    }
  }

  template <typename SE, typename XSD>
  void DataRequests::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    io::DataRequests::Marshall(in, *item);
    option_out.set(std::move(item));
  }
} // namespace io
} // namespace biogears
