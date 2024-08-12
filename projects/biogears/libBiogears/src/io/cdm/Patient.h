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
#include <biogears/cdm/enums/SEPatientEnums.h>
#include <biogears/exports.h>

#include <biogears/schema/cdm/Patient.hxx>

#define CDM_PATIENT_MARSHALL_HELPER(in, out, func)                                   \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Patient::Marshall(*in.m_##func, out.func());                                 \
  }

#define CDM_OPTIONAL_PATIENT_MARSHALL_HELPER(in, out, func) \
  if (in.m_##func) {                                        \
    io::Patient::Marshall(*in.m_##func, out.func());        \
  }

#define SE_PATIENT_ENUM_MARSHALL_HELPER(in, out, func)                               \
  if (in.Has##func()) {                                                              \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::Patient::Marshall(in.m_##func, out.func());                                  \
  }

#define SE_OPTIONAL_PATIENT_ENUM_MARSHALL_HELPER(in, out, func) \
  io::Patient::Marshall(in.m_##func, out.func());

#define CDM_PATIENT_COPY(type, in, out)   \
  {                                       \
    CDM::##type##Data middle;             \
    io::Patient::Marshall(in, middle);    \
    io::Patient::UnMarshall(middle, out); \
  }

namespace biogears {
class SEPatient;

namespace io {
  class BIOGEARS_PRIVATE_API Patient {
  public:
    // template <typename SE, typename XSD>  option
    template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>* = nullptr>
    static void UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);

    template <typename SE, typename XSD>
    static void Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);

    // class SEPatient
    static void UnMarshall(const CDM::PatientData& in, SEPatient& out);
    static void Marshall(const SEPatient& in, CDM::PatientData& out);

    // SESex
    static void UnMarshall(const CDM::enumSex& in, SESex& out);
    static void Marshall(const SESex& in, CDM::enumSex& out);
    // SEBloodType
    static void UnMarshall(const CDM::enumBloodType& in, SEBloodType& out);
    static void Marshall(const SEBloodType& in, CDM::enumBloodType& out);
    //  SEPatientEventType
    static void UnMarshall(const CDM::enumPatientEvent& in, SEPatientEventType& out);
    static void Marshall(const SEPatientEventType& in, CDM::enumPatientEvent& out);

    template <typename SE, typename XSD>
    static typename std::enable_if<std::is_enum<SE>::type>::type
    UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
    {
      if (!option_in.present() || option_in->empty()) {
        out = SE::Invalid;
      } else {
        UnMarshall(option_in.get(), out);
      }
    }
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD, std::enable_if_t<std::is_enum<SE>::value>*>
  void Patient::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present() || option_in->empty()) {
      out = SE::Invalid;
    } else {
      UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD, std::enable_if_t<!std::is_enum<SE>::value>*>
  void Patient::UnMarshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      UnMarshall(option_in.get(), out);
    }
  }

  template <typename SE, typename XSD>
  void Patient::Marshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    Marshall(in, *item);
    option_out.set(std::move(item));
  }
} // Namespace IO

// Operators
// Operators
bool operator==(CDM::enumSex const& lhs, SESex const& rhs);
bool operator==(CDM::enumBloodType const& lhs, SEBloodType const& rhs);
bool operator==(CDM::enumPatientEvent const& lhs, SEPatientEventType const& rhs);

inline bool operator==(SESex const& lhs, CDM::enumSex const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEBloodType const& lhs, CDM::enumBloodType const& rhs)
{
  return rhs == lhs;
}
inline bool operator==(SEPatientEventType const& lhs, CDM::enumPatientEvent const& rhs)
{
  return rhs == lhs;
}

inline bool operator!=(CDM::enumSex const& lhs, SESex const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumBloodType const& lhs, SEBloodType const& rhs)
{
  return !(lhs == rhs);
}
inline bool operator!=(CDM::enumPatientEvent const& lhs, SEPatientEventType const& rhs)
{
  return !(lhs == rhs);
}

inline bool operator!=(SESex const& lhs, CDM::enumSex const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEBloodType const& lhs, CDM::enumBloodType const& rhs)
{
  return !(rhs == lhs);
}
inline bool operator!=(SEPatientEventType const& lhs, CDM::enumPatientEvent const& rhs)
{
  return !(rhs == lhs);
}

} // Namespace Biogears
