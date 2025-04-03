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
#include <biogears/cdm/patient/actions/SESubstanceInhalation.h>

#include "io/cdm/PatientActions.h"
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SESubstanceInhalation::SESubstanceInhalation(const SESubstance& substance)
  : SESubstanceAdministration()
  , m_Substance(substance)
{
  m_Concentration = nullptr;
}
//-------------------------------------------------------------------------------
SESubstanceInhalation::~SESubstanceInhalation()
{
  Invalidate();
}
//-------------------------------------------------------------------------------
void SESubstanceInhalation::Invalidate()
{
  SESubstanceAdministration::Invalidate();
  m_Concentration = nullptr;
}
//-------------------------------------------------------------------------------
bool SESubstanceInhalation::IsValid() const
{
  return SESubstanceAdministration::IsValid() &&  HasConcentration();
}
//-------------------------------------------------------------------------------
bool SESubstanceInhalation::IsActive() const
{
  return IsValid() ? true : false;
}
//-------------------------------------------------------------------------------
bool SESubstanceInhalation::HasConcentration() const
{
  return m_Concentration == nullptr ? false : m_Concentration->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstanceInhalation::GetConcentration()
{
  if (m_Concentration == nullptr)
    m_Concentration = new SEScalarMassPerVolume();
  return *m_Concentration;
}
//-------------------------------------------------------------------------------
SESubstance& SESubstanceInhalation::GetSubstance() const
{
  return (SESubstance&)m_Substance;
}
//-------------------------------------------------------------------------------
void SESubstanceInhalation::ToString(std::ostream& str) const
{
  str << "Patient Action : Substance Inhalation";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tConcentration: ";
  HasConcentration() ? str << *m_Concentration : str << "NaN";
  str << "\n\tSubstance: " << m_Substance.GetName();
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SESubstanceInhalation::operator==(const SESubstanceInhalation& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= (m_Concentration && rhs.m_Concentration) ? m_Concentration->operator==(*rhs.m_Concentration) : m_Concentration == rhs.m_Concentration;
  equivilant &= m_Substance == rhs.m_Substance;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SESubstanceInhalation::operator!=(const SESubstanceInhalation& rhs) const
{
  return !(*this == rhs);
}
}