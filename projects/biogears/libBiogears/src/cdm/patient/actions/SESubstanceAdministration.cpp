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
#include <biogears/cdm/patient/actions/SESubstanceAdministration.h>

#include <biogears/cdm/properties/SEScalarVolume.h>

// Private Includes
#include <io/cdm/PatientActions.h>

namespace biogears {
SESubstanceAdministration::SESubstanceAdministration()
  : SEPatientAction()
{
}
SESubstanceAdministration::~SESubstanceAdministration()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SESubstanceAdministration::Clear()
{
  SEPatientAction::Clear();
}
//-------------------------------------------------------------------------------
bool SESubstanceAdministration::IsValid() const
{
  return SEPatientAction::IsValid();
}
//-------------------------------------------------------------------------------
bool SESubstanceAdministration::IsActive() const
{
  return IsValid();
}
//-------------------------------------------------------------------------------
bool SESubstanceAdministration::Load(const CDM::SubstanceAdministrationData& in)
{
  io::PatientActions::UnMarshall(in, *this);
  return true;
}
//-------------------------------------------------------------------------------
void SESubstanceAdministration::Unload(CDM::SubstanceAdministrationData& data) const
{
  io::PatientActions::Marshall(*this, data);
}
//-------------------------------------------------------------------------------
bool SESubstanceAdministration::operator==(const SESubstanceAdministration& rhs) const
{
  return m_Comment == rhs.m_Comment;
}
//-------------------------------------------------------------------------------
bool SESubstanceAdministration::operator!=(const SESubstanceAdministration& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}