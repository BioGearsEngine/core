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

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

#include <biogears/engine/Controller/BioGearsEnums.h>

#include <biogears/schema/biogears/BioGears.hxx>
#include <biogears/schema/cdm/Circuit.hxx>
#include <biogears/schema/cdm/Compartment.hxx>

#define CDM_BIOGEARS_MARSHALL_HELPER(in, out, func)                                  \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::BioGears::Marshall(*in.m_##func, out.func());                                \
  }

#define CDM_OPTIONAL_BIOGEARS_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                         \
  }

#define SE_BIOGEARS_ENUM_MARSHALL_HELPER(in, out, func)                              \
  if (in.Has##func()) {                                                              \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::BioGears::Marshall(in.m_##func, out.func());                                 \
  }

#define SE_BIOGEARS_PROPERTY_ENUM_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func != decltype(in.m_##func)::Invalid) {           \
    io::BioGears::Marshall(in.m_##func, out.func());             \
  }

#define CDM_BIOGEARS_COPY(type, in, out)   \
  {                                        \
    CDM::##type##Data middle;              \
    io::BioGears::Marshall(in, middle);    \
    io::BioGears::UnMarshall(middle, out); \
  }
namespace biogears {
class BiogearsPhysiologyEngine;

class BioGearsEngine;
class BioGearsCircuits;
class BioGearsCompartments;
class SECircuitManager;
class BioGears;
class SESystem;

namespace io {
  class BIOGEARS_PRIVATE_API BioGears {
  public:
    // class Factories;
    static std::unique_ptr<SESystem> factory(CDM::SystemData const* systemData, biogears::BioGears& bgData);

    static std::unique_ptr<CDM::SystemData> factory(const SESystem* data);

    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd = nullptr);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);

    // class BioGears
    static void UnMarshall(const CDM::BioGearsStateData& in, BioGearsEngine& out, const SEScalarTime* simTime = nullptr);
    static void Marshall(const BioGearsEngine& in, CDM::BioGearsStateData& out);

    // class BioGearsCompartments
    static void UnMarshall(const CDM::CompartmentManagerData& in, BioGearsCompartments& out, SECircuitManager* circuits);
    static void Marshall(const BioGearsCompartments& in, CDM::CompartmentManagerData& out);

    // class BioGearsCircuits
    static void UnMarshall(const CDM::CircuitManagerData& in, BioGearsCircuits& ou);
    static void Marshall(const BioGearsCircuits& in, CDM::CircuitManagerData& out);

    //  SEErrorType
    static void UnMarshall(const CDM::enumBioGearsAirwayMode& in, SEBioGearsAirwayMode& out);
    static void Marshall(const SEBioGearsAirwayMode& in, CDM::enumBioGearsAirwayMode& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>*>
  void BioGears::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out = SE::Invalid;
    } else {
      UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>*>
  void BioGears::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out, std::default_random_engine* rd)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      UnMarshall(option_in.get(), out, rd);
    }
  }

  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void BioGears::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO

// Operators
bool operator==(CDM::enumBioGearsAirwayMode const& lhs, SEBioGearsAirwayMode const& rhs);

inline bool operator==(SEBioGearsAirwayMode const& lhs, CDM::enumBioGearsAirwayMode const& rhs)
{
  return rhs == lhs;
}

inline bool operator!=(CDM::enumBioGearsAirwayMode const& lhs, SEBioGearsAirwayMode const& rhs)
{
  return !(lhs == rhs);
}

inline bool operator!=(SEBioGearsAirwayMode const& lhs, CDM::enumBioGearsAirwayMode const& rhs)
{
  return !(rhs == lhs);
}
} // Namespace Biogears
