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

#define CDM_BIOGEARS_ENGINE_CONFIGURATION_PTR_MARSHALL_HELPER(in, out, func)             \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::BiogearsEngineConfiguration::Marshall(*in.m_##func, out.func());             \
  }

#define CDM_OPTIONAL_BIOGEARS_ENGINE_CONFIGURATION_PTR_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                                              \
    io::BiogearsEngineConfiguration::Marshall(*in.m_##func, out.func());          \
  }

#define CDM_BIOGEARS_ENGINE_CONFIGURATION_COPY(type, in, out) \
  {                                                           \
    CDM::##type##Data middle;                                 \
    io::BiogearsEngineConfiguration::Marshall(in, middle);    \
    io::BiogearsEngineConfiguration::UnMarshall(middle, out); \
  }
namespace biogears {

class BioGearsConfiguration;

namespace io {
  class BIOGEARS_PRIVATE_API BiogearsEngineConfiguration {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class BloodChemistryConfiguration
    static void UnMarshall(const CDM::BloodChemistryConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::BloodChemistryConfigurationData& out);
    // class CardiovascularConfiguration
    static void UnMarshall(const CDM::CardiovascularConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::CardiovascularConfigurationData& out);
    // class CircuitConfiguration
    static void UnMarshall(const CDM::CircuitConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::CircuitConfigurationData& out);
    // class ConstantCOnfiguration
    static void UnMarshall(const CDM::ConstantsConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::ConstantsConfigurationData& out);
    // class DrugsConfiguration
    static void UnMarshall(const CDM::DrugsConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::DrugsConfigurationData& out);
    // class EnergyConfiguration
    static void UnMarshall(const CDM::EnergyConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::EnergyConfigurationData& out);
    // class EnvironmentConfiguration
    static void UnMarshall(const CDM::EnvironmentConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::EnvironmentConfigurationData& out);
    // class GastrointestinalConfiguratio
    static void UnMarshall(const CDM::GastrointestinalConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::GastrointestinalConfigurationData& out);
    // class BioGearsConfiguration
    static void UnMarshall(const CDM::NervousConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::NervousConfigurationData& out);
    // class RenalConfiguration
    static void UnMarshall(const CDM::RenalConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::RenalConfigurationData& out);
    // class RespiratoryConfiguration
    static void UnMarshall(const CDM::RespiratoryConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::RespiratoryConfigurationData& out);
    // class TissueConfiguration
    static void UnMarshall(const CDM::TissueConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::TissueConfigurationData& out);
    // class BioGearsConfiguration
    static void UnMarshall(const CDM::BioGearsConfigurationData& in, BioGearsConfiguration& out);
    static void Marshall(const BioGearsConfiguration& in, CDM::BioGearsConfigurationData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void BiogearsEngineConfiguration::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present() && !out.m_Merge) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void BiogearsEngineConfiguration::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} // Namespace Biogears
