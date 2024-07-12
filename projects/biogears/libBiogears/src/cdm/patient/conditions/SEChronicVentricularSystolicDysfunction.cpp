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
#include <biogears/cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.h>

#include "io/cdm/PatientConditions.h"

namespace biogears {
SEChronicVentricularSystolicDysfunction::SEChronicVentricularSystolicDysfunction()
  : SEChronicHeartFailure()
{
}
//-----------------------------------------------------------------------------
SEChronicVentricularSystolicDysfunction::~SEChronicVentricularSystolicDysfunction()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEChronicVentricularSystolicDysfunction::Clear()
{
  SEChronicHeartFailure::Clear();
}
//-----------------------------------------------------------------------------
bool SEChronicVentricularSystolicDysfunction::IsValid() const
{
  return SEChronicHeartFailure::IsValid();
}
//-----------------------------------------------------------------------------
bool SEChronicVentricularSystolicDysfunction::Load(const CDM::ChronicVentricularSystolicDysfunctionData& in)
{
  io::PatientConditions::UnMarshall(in, *this);
  return true;
}
//-----------------------------------------------------------------------------
CDM::ChronicVentricularSystolicDysfunctionData* SEChronicVentricularSystolicDysfunction::Unload() const
{
  CDM::ChronicVentricularSystolicDysfunctionData* data(new CDM::ChronicVentricularSystolicDysfunctionData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEChronicVentricularSystolicDysfunction::Unload(CDM::ChronicVentricularSystolicDysfunctionData& data) const
{
  io::PatientConditions::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
void SEChronicVentricularSystolicDysfunction::ToString(std::ostream& str) const
{
  str << "Patient Condition : Ventricular Systolic Dysfunction Heart Failure";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEChronicVentricularSystolicDysfunction::operator==(SEChronicVentricularSystolicDysfunction const& rhs) const
{
  if (this == &rhs)
    return true;

  return m_Comment == rhs.m_Comment;
}
bool SEChronicVentricularSystolicDysfunction::operator!=(SEChronicVentricularSystolicDysfunction const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool SEChronicVentricularSystolicDysfunction::operator==(SECondition const& rhs) const
{
  try {
    auto& chronicVentricularSystolicDysfunction = dynamic_cast<SEChronicVentricularSystolicDysfunction const&>(rhs);
    return this->operator==(chronicVentricularSystolicDysfunction);
  } catch (std::exception) {
    return false;
  }
  return false;
}
bool SEChronicVentricularSystolicDysfunction::operator!=(SECondition const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}