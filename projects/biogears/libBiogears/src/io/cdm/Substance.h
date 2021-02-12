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

#include <biogears/schema/cdm/Substance.hxx>

namespace biogears {

class SESubstanceManager;
class SESubstanceAerosolization;
class SESubstancePharmacokinetics;
class SESubstancePhysicochemicals;
class SESubstanceTissuePharmacokinetics;
class SESubstancePharmacodynamics;
class SESubstanceClearance;
class SESubstance;
class SESubstanceCompound;
class SESubstanceFraction;
class SESubstanceConcentration;
class SEPharmacodynamicModifier;

#define CDM_SUBSTANCE_UNMARSHAL_HELPER(xsd, func)                                    \
  if (m_##func) {                                                                    \
    xsd.func(std::make_unique<std::remove_reference<decltype(xsd.func())>::type>()); \
    io::Property::UnMarshall(*m_##func, xsd.func());                                 \
  }
namespace io {
  class BIOGEARS_PRIVATE_API Substance {
  public:
    //Enums
    //class enumSubstanceState;
    //class enumSubstanceIonicState;
    //class enumSubstanceBindingProtein;
    //class enumCharge;
    //class enumSubstanceClass;
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SESubstanceAerosolization
    static void Marshall(const CDM::SubstanceAerosolizationData& in, SESubstanceAerosolization& out);
    static void UnMarshall(const SESubstanceAerosolization& in, CDM::SubstanceAerosolizationData& out);
    //class SESubstancePharmacokinetics
    static void Marshall(const CDM::SubstancePharmacokineticsData& in, SESubstancePharmacokinetics& out);
    static void UnMarshall(const SESubstancePharmacokinetics& in, CDM::SubstancePharmacokineticsData& out);
    //class SESubstancePhysicochemical
    static void Marshall(const CDM::SubstancePhysicochemicalData& in, SESubstancePhysicochemicals& out);
    static void UnMarshall(const SESubstancePhysicochemicals& in, CDM::SubstancePhysicochemicalData& out);
    //class SESubstanceTissuePharmacokinetics
    static void Marshall(const CDM::SubstanceTissuePharmacokineticsData& in, SESubstanceTissuePharmacokinetics& out);
    static void UnMarshall(const SESubstanceTissuePharmacokinetics& in, CDM::SubstanceTissuePharmacokineticsData& out);
    //class SESubstancePharmacodynamics
    static void Marshall(const CDM::SubstancePharmacodynamicsData& in, SESubstancePharmacodynamics& out);
    static void UnMarshall(const SESubstancePharmacodynamics& in, CDM::SubstancePharmacodynamicsData& out);
    //class SESubstanceClearance
    static void Marshall(const CDM::SubstanceClearanceData& in, SESubstanceClearance& out);
    static void UnMarshall(const SESubstanceClearance& in, CDM::SubstanceClearanceData& out);
    //class SESubstance
    static void Marshall(const CDM::SubstanceData& in, SESubstance& out);
    static void UnMarshall(const SESubstance& in, CDM::SubstanceData& out);
    //class SESubstanceCompound
    static void Marshall(const CDM::SubstanceCompoundData& in, const SESubstanceManager& subMgr, SESubstanceCompound& out);
    static void UnMarshall(const SESubstanceCompound& in, CDM::SubstanceCompoundData& out);
    //class SESubstanceFraction
    static void Marshall(const CDM::SubstanceFractionData& in, SESubstanceFraction& out);
    static void UnMarshall(const SESubstanceFraction& in, CDM::SubstanceFractionData& out);
    //class SESubstanceConcentration
    static void Marshall(const CDM::SubstanceConcentrationData& in, SESubstanceConcentration& out);
    static void UnMarshall(const SESubstanceConcentration& in, CDM::SubstanceConcentrationData& out);
    //class SEPharmacodynamicModifier
    static void Marshall(const CDM::PharmacodynamicModifierData& in, SEPharmacodynamicModifier& out);
    static void UnMarshall(const SEPharmacodynamicModifier& in, CDM::PharmacodynamicModifierData& out);

    static void Copy(const SESubstanceClearance& in, SESubstanceClearance& out);
    static void Copy(const SESubstance& in, SESubstance& out);
    static void Copy(const SESubstanceCompound& in, const SESubstanceManager& subMgr, SESubstanceCompound& out);
    static void Copy(const SESubstanceFraction& in, SESubstanceFraction& out);
    static void Copy(const SESubstanceConcentration& in, SESubstanceConcentration& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Substance::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Substance::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }

} // Namespace IO
} //Namespace Biogears
