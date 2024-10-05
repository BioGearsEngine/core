/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>

#include "io/cdm/Substance.h"

#include <biogears/cdm/properties/SEScalarFraction.h>

namespace std {
template class map<string, biogears::SESubstanceTissuePharmacokinetics*>;
}

namespace biogears {
SESubstancePharmacokinetics::SESubstancePharmacokinetics(SESubstancePharmacokinetics const& obj)
  : Loggable(obj.GetLogger())
  , m_Physicochemicals(std::make_unique<SESubstancePhysicochemical>(*obj.m_Physicochemicals).release())
{
  for (auto& [key, value] : obj.m_TissueKinetics) {
    m_TissueKinetics[key] = std::make_unique<SESubstanceTissuePharmacokinetics>(*value).release();
  }
}
SESubstancePharmacokinetics::SESubstancePharmacokinetics(Logger* logger)
  : Loggable(logger)
  , m_Physicochemicals(std::make_unique<SESubstancePhysicochemical>(logger).release())
  , m_TissueKinetics()
{
}
//-----------------------------------------------------------------------------
SESubstancePharmacokinetics::~SESubstancePharmacokinetics()
{
  Clear();
}
//-----------------------------------------------------------------------------
#pragma optimize("", off)
void SESubstancePharmacokinetics::Clear()
{
  m_Physicochemicals->Clear();

  for (auto& [key, value] : m_TissueKinetics) {
    delete value;
    value = nullptr;
  }
  m_TissueKinetics.clear();
}
#pragma optimize("", on)
//-----------------------------------------------------------------------------
bool SESubstancePharmacokinetics::IsValid() const
{
  if (HasPhysicochemicals())
    return true;
  if (HasTissueKinetics())
    return true;
  return false;
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstancePharmacokinetics::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstancePharmacokinetics::GetScalar(const std::string& name)
{
  if (HasPhysicochemicals())
    return GetPhysicochemicals().GetScalar(name);
  // I did not support for getting a specific tissue kinetic scalar due to lack of coffee
  return nullptr;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacokinetics::HasPhysicochemicals() const
{
  return m_Physicochemicals == nullptr ? false : m_Physicochemicals->IsValid();
}
//-----------------------------------------------------------------------------
SESubstancePhysicochemical& SESubstancePharmacokinetics::GetPhysicochemicals()
{
  if (m_Physicochemicals == nullptr)
    m_Physicochemicals = new SESubstancePhysicochemical(GetLogger());
  return *m_Physicochemicals;
}
//-----------------------------------------------------------------------------
const SESubstancePhysicochemical* SESubstancePharmacokinetics::GetPhysicochemicals() const
{
  return m_Physicochemicals;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacokinetics::HasTissueKinetics() const
{
  return m_TissueKinetics.size() > 0;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacokinetics::HasTissueKinetics(const char* name) const
{
  return HasTissueKinetics(std::string { name });
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacokinetics::HasTissueKinetics(const std::string& name) const
{
  auto idx = m_TissueKinetics.find(name);
  if (idx != m_TissueKinetics.end())
    return true;
  return false;
}
//-----------------------------------------------------------------------------
SESubstanceTissuePharmacokinetics& SESubstancePharmacokinetics::GetTissueKinetics(const char* name)
{
  return GetTissueKinetics(std::string { name });
}
//-----------------------------------------------------------------------------
SESubstanceTissuePharmacokinetics& SESubstancePharmacokinetics::GetTissueKinetics(const std::string& name)
{
  auto idx = m_TissueKinetics.find(name);
  if (idx != m_TissueKinetics.end())
    return *(idx->second);
  SESubstanceTissuePharmacokinetics* fx = new SESubstanceTissuePharmacokinetics(name, GetLogger());
  m_TissueKinetics[name] = fx;
  return *fx;
}
//-----------------------------------------------------------------------------
const SESubstanceTissuePharmacokinetics* SESubstancePharmacokinetics::GetTissueKinetics(const char* name) const
{
  return GetTissueKinetics(std::string { name });
}
//-----------------------------------------------------------------------------
const SESubstanceTissuePharmacokinetics* SESubstancePharmacokinetics::GetTissueKinetics(const std::string& name) const
{
  auto idx = m_TissueKinetics.find(name);
  if (idx != m_TissueKinetics.end())
    return (idx->second);
  return nullptr;
}
//-----------------------------------------------------------------------------
void SESubstancePharmacokinetics::RemoveTissueKinetics(const char* name)
{
  m_TissueKinetics.erase(name);
}
//-----------------------------------------------------------------------------
void SESubstancePharmacokinetics::RemoveTissueKinetics(const std::string& name)
{
  m_TissueKinetics.erase(name);
}
//-----------------------------------------------------------------------------
SESubstancePharmacokinetics& SESubstancePharmacokinetics::operator=(SESubstancePharmacokinetics const& rhs)
{
  if (this != &rhs) {
    return *this;
  }

  GetPhysicochemicals() = *rhs.GetPhysicochemicals();
  for (auto& [key, value] : rhs.m_TissueKinetics) {

      auto tissueKinetic = std::make_unique<SESubstanceTissuePharmacokinetics>(*value);
  m_TissueKinetics[key] = tissueKinetic.release();
  }

  return *this;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacokinetics::operator==(const SESubstancePharmacokinetics& rhs) const
{
  bool equivilant = (m_Physicochemicals && rhs.m_Physicochemicals) ? m_Physicochemicals->operator==(*rhs.m_Physicochemicals) : m_Physicochemicals == rhs.m_Physicochemicals;
  equivilant &= m_TissueKinetics.size() == rhs.m_TissueKinetics.size();
  if (equivilant) {
    for (auto& pair : m_TissueKinetics) {
      // auto lh = m_TissueKinetics.find(pair.first);
      auto rh = rhs.m_TissueKinetics.find(pair.first);
      if (rh != rhs.m_TissueKinetics.end()) {
        equivilant &= (pair.second && rh->second) ? *pair.second == *rh->second : pair.second == rh->second;
      }
    }
  }
  return equivilant;
}
//-----------------------------------------------------------------------------
bool SESubstancePharmacokinetics::operator!=(const SESubstancePharmacokinetics& rhs) const
{
  return !(*this == rhs);
}
}