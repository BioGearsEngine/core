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

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/schema/biogears/BioGearsEngineConfiguration.hxx>

#define CDM_BIOGEARS_ENGINE_CONFIGURATION_UNMARSHAL_HELPER(in, out, func)            \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::BiogearsEngineConfiguration::UnMarshall(*in.m_##func, out.func());           \
  }

#define CDM_OPTIONAL_BIOGEARS_ENGINE_CONFIGURATION_UNMARSHAL_HELPER(in, out, func) \
  if (in.m_##func) {                                                               \
    io::BiogearsEngineConfiguration::UnMarshall(*in.m_##func, out.func());         \
  }
namespace biogears {

class BioGearsConfiguration;

namespace io {
  class BIOGEARS_PRIVATE_API BiogearsEngineConfiguration {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class BloodChemistryConfiguration
    static void Marshall(const CDM::BloodChemistryConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::BloodChemistryConfigurationData& out);
    // class CardiovascularConfiguration
    static void Marshall(const CDM::CardiovascularConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::CardiovascularConfigurationData& out);
    // class CircuitConfiguration
    static void Marshall(const CDM::CircuitConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::CircuitConfigurationData& out);
    // class ConstantCOnfiguration
    static void Marshall(const CDM::ConstantsConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::ConstantsConfigurationData& out);
    // class DrugsConfiguration
    static void Marshall(const CDM::DrugsConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::DrugsConfigurationData& out);
    // class EnergyConfiguration
    static void Marshall(const CDM::EnergyConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::EnergyConfigurationData& out);
    // class EnvironmentConfiguration
    static void Marshall(const CDM::EnvironmentConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::EnvironmentConfigurationData& out);
    // class GastrointestinalConfiguratio
    static void Marshall(const CDM::GastrointestinalConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::GastrointestinalConfigurationData& out);
    // class BioGearsConfiguration
    static void Marshall(const CDM::NervousConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::NervousConfigurationData& out);
    // class RenalConfiguration
    static void Marshall(const CDM::RenalConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::RenalConfigurationData& out);
    // class RespiratoryConfiguration
    static void Marshall(const CDM::RespiratoryConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::RespiratoryConfigurationData& out);
    // class TissueConfiguration
    static void Marshall(const CDM::TissueConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::TissueConfigurationData& out);
    // class BioGearsConfiguration
    static void Marshall(const CDM::BioGearsConfigurationData& in, BioGearsConfiguration& out);
    static void UnMarshall(const BioGearsConfiguration& in, CDM::BioGearsConfigurationData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void BiogearsEngineConfiguration::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void BiogearsEngineConfiguration::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} // Namespace Biogears
