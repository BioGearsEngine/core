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
  SEChronicHeartFailure::Load(in);
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
  SEChronicHeartFailure::Unload(data);
}
//-----------------------------------------------------------------------------
void SEChronicVentricularSystolicDysfunction::ToString(std::ostream& str) const
{
  str << "Patient Condition : Ventricular Systolic Dysfunction Heart Failure";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << std::flush;
}
//-----------------------------------------------------------------------------
}