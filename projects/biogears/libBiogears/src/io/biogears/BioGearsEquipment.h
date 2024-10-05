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

#include <biogears/schema/biogears/BioGearsEquipment.hxx>

// Question: To Serialize Invalid units or not to Serialize?
//           TO Throw an exception when a member is invalid?
#define CDM_BIOGEARS_EQUIPMENT_PTR_MARSHALL_HELPER(in, out, func)                                \
  if (in.m_##func && in.m_##func->IsValid()) {                                               \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>());         \
    io::BiogearsEquipment::Marshall(*in.m_##func, out.func());                               \
  } /* else if (in.m_##func) {                                                               \
     throw biogears::CommonDataModelException("func is InValid and cannot be Unmarshalled"); \
   }*/

#define CDM_OPTIONAL_BIOGEARS_EQUIPMENT_PTR_MARSHALL_HELPER(in, out, func)                      \
  if (in.m_##func && in.m_##func->IsValid()) {                                              \
    io::BiogearsEquipment::Marshall(*in.m_##func, out.func());                              \
  } /*else if (in.m_##func) {                                                               \
    throw biogears::CommonDataModelException("func is InValid and cannot be Unmarshalled"); \
  }*/

namespace biogears {
class AnesthesiaMachine;
class ECG;
class Inhaler;
class SESystem;
class BioGears;
class SESubstanceManager;

namespace io {
  class BIOGEARS_PRIVATE_API BiogearsEquipment {
  public:
    // class Factories;
    static std::unique_ptr<SESystem> factory(CDM::SystemData const* systemData, biogears::BioGears& bgData);

    static std::unique_ptr<CDM::SystemData> factory(const SESystem* data);

    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class SEAnesthesiaMachine
    static void UnMarshall(const CDM::BioGearsAnesthesiaMachineData& in, AnesthesiaMachine& out);
    static void Marshall(const AnesthesiaMachine& in, CDM::BioGearsAnesthesiaMachineData& out);
    // class SEAnesthesiaMachineChamber
    static void UnMarshall(const CDM::BioGearsElectroCardioGramData& in, ECG& out);
    static void Marshall(const ECG& in, CDM::BioGearsElectroCardioGramData& out);
    // class SEAnesthesiaMachineOxygenBottle
    static void UnMarshall(const CDM::BioGearsInhalerData& in, biogears::Inhaler& out);
    static void Marshall(const biogears::Inhaler& in, CDM::BioGearsInhalerData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void BiogearsEquipment::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void BiogearsEquipment::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} // Namespace Biogears
