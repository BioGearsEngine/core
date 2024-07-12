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
#include <biogears/cdm/patient/conditions/SEChronicAnemia.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include "io/cdm/PatientConditions.h"

namespace biogears {
SEChronicAnemia::SEChronicAnemia()
  : SEPatientCondition()
{
  m_ReductionFactor = nullptr;
}
//-------------------------------------------------------------------------------
SEChronicAnemia::~SEChronicAnemia()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEChronicAnemia::Clear()
{
  SEPatientCondition::Clear();
  SAFE_DELETE(m_ReductionFactor);
}
//-------------------------------------------------------------------------------
bool SEChronicAnemia::IsValid() const
{
  return SEPatientCondition::IsValid() && HasReductionFactor();
}
//-------------------------------------------------------------------------------
bool SEChronicAnemia::Load(const CDM::ChronicAnemiaData& in)
{
  io::PatientConditions::UnMarshall(in, *this);
  return true;
}
//-------------------------------------------------------------------------------
CDM::ChronicAnemiaData* SEChronicAnemia::Unload() const
{
  CDM::ChronicAnemiaData* data(new CDM::ChronicAnemiaData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEChronicAnemia::Unload(CDM::ChronicAnemiaData& data) const
{
  io::PatientConditions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool SEChronicAnemia::HasReductionFactor() const
{
  return m_ReductionFactor == nullptr ? false : m_ReductionFactor->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar0To1& SEChronicAnemia::GetReductionFactor()
{
  if (m_ReductionFactor == nullptr)
    m_ReductionFactor = new SEScalar0To1();
  return *m_ReductionFactor;
}
//-------------------------------------------------------------------------------
void SEChronicAnemia::ToString(std::ostream& str) const
{
  str << "Patient Condition : Anemia";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tReduction Factor: ";
  HasReductionFactor() ? str << m_ReductionFactor : str << "NaN";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEChronicAnemia::operator==(SEChronicAnemia const& rhs) const
{
  if (this == &rhs)
    return true;

  return ((m_ReductionFactor && rhs.m_ReductionFactor) ? m_ReductionFactor->operator==(*rhs.m_ReductionFactor)
                                                       : (m_ReductionFactor == rhs.m_ReductionFactor))
    && (m_Comment == rhs.m_Comment);
}
bool SEChronicAnemia::operator!=(SEChronicAnemia const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SEChronicAnemia::operator==(SECondition const& rhs) const
{
  try {
    auto& chronicAnemaRhs = dynamic_cast<SEChronicAnemia const&>(rhs);
    return this->operator==(chronicAnemaRhs);
  } catch (std::exception) {
    return false;
  }
  return false;
}
bool SEChronicAnemia::operator!=(SECondition const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}