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

#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

#define CDM_BIOGEARS_PHYSIOLOGY_MARSHALL_HELPER(in, out, func)                       \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::BiogearsPhysiology::Marshall(*in.m_##func, out.func());                      \
  }

#define CDM_OPTIONAL_BIOGEARS_PHYSIOLOGY_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                                    \
    io::BiogearsPhysiology::Marshall(*in.m_##func, out.func());         \
  }

namespace biogears {
class SESubstanceManager;
class SESystem;
class BioGears;
class BioGearsSystem;
class BloodChemistry;
class Cardiovascular;
class Drugs;
class Endocrine;
class Energy;
class Gastrointestinal;
class Hepatic;
class Nervous;
class Renal;
class Respiratory;
class Tissue;

namespace io {
  class BIOGEARS_PRIVATE_API BiogearsPhysiology {
  public:
    // class Factories;
    static std::unique_ptr<SESystem> factory(CDM::SystemData const* systemData, biogears::BioGears& bgData, SESubstanceManager& substances);

    static std::unique_ptr<CDM::SystemData> factory(const SESystem* data);

    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    // class BloodChemistry
    static void UnMarshall(const CDM::BioGearsBloodChemistrySystemData& in, const SESubstanceManager& substances, BloodChemistry& out);
    static void Marshall(const BloodChemistry& in, CDM::BioGearsBloodChemistrySystemData& out);
    // class Cardiovascular
    static void UnMarshall(const CDM::BioGearsCardiovascularSystemData& in, const SESubstanceManager& substances, Cardiovascular& out);
    static void Marshall(const Cardiovascular& in, CDM::BioGearsCardiovascularSystemData& out);
    // class Drugs
    static void UnMarshall(const CDM::BioGearsDrugSystemData& in, const SESubstanceManager& substances, Drugs& out);
    static void Marshall(const Drugs& in, CDM::BioGearsDrugSystemData& out);
    // class Endocrine
    static void UnMarshall(const CDM::BioGearsEndocrineSystemData& in, const SESubstanceManager& substances, Endocrine& out);
    static void Marshall(const Endocrine& in, CDM::BioGearsEndocrineSystemData& out);
    // class Energy
    static void UnMarshall(const CDM::BioGearsEnergySystemData& in, const SESubstanceManager& substances, Energy& out);
    static void Marshall(const Energy& in, CDM::BioGearsEnergySystemData& out);
    // class Gastrointestinal
    static void UnMarshall(const CDM::BioGearsGastrointestinalSystemData& in, const SESubstanceManager& substances, Gastrointestinal& out);
    static void Marshall(const Gastrointestinal& in, CDM::BioGearsGastrointestinalSystemData& out);
    // class Hepatic
    static void UnMarshall(const CDM::BioGearsHepaticSystemData& in, const SESubstanceManager& substances, Hepatic& out);
    static void Marshall(const Hepatic& in, CDM::BioGearsHepaticSystemData& out);
    // class Nervous
    static void UnMarshall(const CDM::BioGearsNervousSystemData& in, const SESubstanceManager& substances, Nervous& out);
    static void Marshall(const Nervous& in, CDM::BioGearsNervousSystemData& out);
    // class Renal
    static void UnMarshall(const CDM::BioGearsRenalSystemData& in, const SESubstanceManager& substances, Renal& out);
    static void Marshall(const Renal& in, CDM::BioGearsRenalSystemData& out);
    // class Respiratory
    static void UnMarshall(const CDM::BioGearsRespiratorySystemData& in, const SESubstanceManager& substances, Respiratory& out);
    static void Marshall(const Respiratory& in, CDM::BioGearsRespiratorySystemData& out);
    // class Tissue
    static void UnMarshall(const CDM::BioGearsTissueSystemData& in, const SESubstanceManager& substances, Tissue& out);
    static void Marshall(const Tissue& in, CDM::BioGearsTissueSystemData& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void BiogearsPhysiology::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Clear();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void BiogearsPhysiology::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} // Namespace Biogears
