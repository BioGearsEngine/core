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
#include <biogears/cdm/enums/SESubstanceEnums.h>
#include <biogears/exports.h>

#include <biogears/schema/cdm/Substance.hxx>

#define CDM_SUBSTANCE_MARSHALL_HELPER(in, out, func)                                 \
  if (in.m_##func && in.m_##func->IsValid()) {                                       \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Substance::Marshall(*in.m_##func, out.func());                               \
  }

#define CDM_OPTIONAL_SUBSTANCE_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func && in.m_##func->IsValid()) {                \
    io::Substance::Marshall(*in.m_##func, out.func());        \
  }
#define SE_SUBSTANCE_ENUM_MARSHALL_HELPER(in, out, func)                             \
  if (in.Has##func()) {                                                              \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Substance::Marshall(in.m_##func, out.func());                                \
  }

#define SE_OPTIONAL_SUBSTANCE_ENUM_MARSHALL_HELPER(in, out, func) \
  io::Substance::Marshall(in.m_##func, out.func());

#define CDM_SUBSTANCE_COPY(type, in, out)   \
  {                                         \
    CDM::##type##Data middle;               \
    io::Substance::Marshall(in, middle);    \
    io::Substance::UnMarshall(middle, out); \
  }
namespace biogears {

class SESubstanceManager;
class SESubstanceAerosolization;
class SESubstancePharmacokinetics;
class SESubstancePhysicochemical;
class SESubstanceTissuePharmacokinetics;
class SESubstancePharmacodynamics;
class SESubstanceClearance;
class SESubstance;
class SESubstanceCompound;
class SESubstanceFraction;
class SESubstanceConcentration;
class SEPharmacodynamicModifier;

namespace io {
  class BIOGEARS_PRIVATE_API Substance {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);

    // class SESubstanceAerosolization
    static void UnMarshall(const CDM::SubstanceAerosolizationData& in, SESubstanceAerosolization& out);
    static void Marshall(const SESubstanceAerosolization& in, CDM::SubstanceAerosolizationData& out);
    // class SESubstancePharmacokinetics
    static void UnMarshall(const CDM::SubstancePharmacokineticsData& in, SESubstancePharmacokinetics& out);
    static void Marshall(const SESubstancePharmacokinetics& in, CDM::SubstancePharmacokineticsData& out);
    // class SESubstancePhysicochemical
    static void UnMarshall(const CDM::SubstancePhysicochemicalData& in, SESubstancePhysicochemical& out);
    static void Marshall(const SESubstancePhysicochemical& in, CDM::SubstancePhysicochemicalData& out);
    // class SESubstanceTissuePharmacokinetics
    static void UnMarshall(const CDM::SubstanceTissuePharmacokineticsData& in, SESubstanceTissuePharmacokinetics& out);
    static void Marshall(const SESubstanceTissuePharmacokinetics& in, CDM::SubstanceTissuePharmacokineticsData& out);
    // class SESubstancePharmacodynamics
    static void UnMarshall(const CDM::SubstancePharmacodynamicsData& in, SESubstancePharmacodynamics& out);
    static void Marshall(const SESubstancePharmacodynamics& in, CDM::SubstancePharmacodynamicsData& out);
    // class SESubstanceClearance
    static void UnMarshall(const CDM::SubstanceClearanceData& in, SESubstanceClearance& out);
    static void Marshall(const SESubstanceClearance& in, CDM::SubstanceClearanceData& out);
    // class SESubstance
    static void UnMarshall(const CDM::SubstanceData& in, SESubstance& out);
    static void Marshall(const SESubstance& in, CDM::SubstanceData& out);
    // class SESubstanceCompound
    static void UnMarshall(const CDM::SubstanceCompoundData& in, const SESubstanceManager& subMgr, SESubstanceCompound& out);
    static void Marshall(const SESubstanceCompound& in, CDM::SubstanceCompoundData& out);
    // class SESubstanceFraction
    static void UnMarshall(const CDM::SubstanceFractionData& in, SESubstanceFraction& out);
    static void Marshall(const SESubstanceFraction& in, CDM::SubstanceFractionData& out);
    // class SESubstanceConcentration
    static void UnMarshall(const CDM::SubstanceConcentrationData& in, SESubstanceConcentration& out);
    static void Marshall(const SESubstanceConcentration& in, CDM::SubstanceConcentrationData& out);
    // class SEPharmacodynamicModifier
    static void UnMarshall(const CDM::PharmacodynamicModifierData& in, SEPharmacodynamicModifier& out);
    static void Marshall(const SEPharmacodynamicModifier& in, CDM::PharmacodynamicModifierData& out);

    static void Copy(const SESubstanceClearance& in, SESubstanceClearance& out);
    static void Copy(const SESubstance& in, SESubstance& out);
    static void Copy(const SESubstanceCompound& in, const SESubstanceManager& subMgr, SESubstanceCompound& out);
    static void Copy(const SESubstanceFraction& in, SESubstanceFraction& out);
    static void Copy(const SESubstanceConcentration& in, SESubstanceConcentration& out);

    // SESubstanceState
    static void UnMarshall(const CDM::enumSubstanceState& in, SESubstanceState& out);
    static void Marshall(const SESubstanceState& in, CDM::enumSubstanceState& out);
    // SESubstanceIonicState
    static void UnMarshall(const CDM::enumSubstanceIonicState& in, SESubstanceIonicState& out);
    static void Marshall(const SESubstanceIonicState& in, CDM::enumSubstanceIonicState& out);
    // SESubstanceBindingProtein
    static void UnMarshall(const CDM::enumSubstanceBindingProtein& in, SESubstanceBindingProtein& out);
    static void Marshall(const SESubstanceBindingProtein& in, CDM::enumSubstanceBindingProtein& out);
    //  SECharge
    static void UnMarshall(const CDM::enumCharge& in, SECharge& out);
    static void Marshall(const SECharge& in, CDM::enumCharge& out);
    //  SESubstanceClass
    static void UnMarshall(const CDM::enumSubstanceClass& in, SESubstanceClass& out);
    static void Marshall(const SESubstanceClass& in, CDM::enumSubstanceClass& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>*>
  void Substance::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present() || option_in->empty()) {
      out = SE::Invalid;
    } else {
      io::Substance::UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>*>
  void Substance::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      io::Substance::UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD>
  void Substance::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    io::Substance::Marshall(in, *item);
    option_out.set(std::move(item));
  }

} // Namespace IO

// Operators
bool operator==(CDM::enumSubstanceState const& lhs, SESubstanceState const& rhs);
bool operator==(CDM::enumSubstanceIonicState const& lhs, SESubstanceIonicState const& rhs);
bool operator==(CDM::enumSubstanceBindingProtein const& lhs, SESubstanceBindingProtein const& rhs);
bool operator==(CDM::enumCharge const& lhs, SECharge const& rhs);
bool operator==(CDM::enumSubstanceClass const& lhs, SESubstanceClass const& rhs);

inline bool operator==(SESubstanceState const& lhs, CDM::enumSubstanceState const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SESubstanceIonicState const& lhs, CDM::enumSubstanceIonicState const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SESubstanceBindingProtein const& lhs, CDM::enumSubstanceBindingProtein const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SECharge const& lhs, CDM::enumCharge const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SESubstanceClass const& lhs, CDM::enumSubstanceClass const& rhs)
{
  return rhs == lhs;
}

inline bool operator!=(CDM::enumSubstanceState const& lhs, SESubstanceState const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumSubstanceIonicState const& lhs, SESubstanceIonicState const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumSubstanceBindingProtein const& lhs, SESubstanceBindingProtein const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumCharge const& lhs, SECharge const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumSubstanceClass const& lhs, SESubstanceClass const& rhs)
{
  return !(lhs == rhs);
}

inline bool operator!=(SESubstanceState const& lhs, CDM::enumSubstanceState const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SESubstanceIonicState const& lhs, CDM::enumSubstanceIonicState const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SESubstanceBindingProtein const& lhs, CDM::enumSubstanceBindingProtein const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SECharge const& lhs, CDM::enumCharge const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SESubstanceClass const& lhs, CDM::enumSubstanceClass const& rhs)
{
  return !(rhs == lhs);
}

// Operators

} // Namespace Biogears
