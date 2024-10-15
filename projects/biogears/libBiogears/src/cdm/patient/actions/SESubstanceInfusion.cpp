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
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>

#include "io/cdm/PatientActions.h"
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SESubstanceInfusion::SESubstanceInfusion(const SESubstance& substance)
  : SESubstanceAdministration()
  , m_Substance(substance)
{
  m_Rate = nullptr;
  m_Concentration = nullptr;
}
//-------------------------------------------------------------------------------
SESubstanceInfusion::~SESubstanceInfusion()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SESubstanceInfusion::Clear()
{
  SESubstanceAdministration::Clear();
  m_Rate = nullptr;
  m_Concentration = nullptr;
}
//-------------------------------------------------------------------------------
bool SESubstanceInfusion::IsValid() const
{
  return SESubstanceAdministration::IsValid() && HasRate() && HasConcentration();
}
//-------------------------------------------------------------------------------
bool SESubstanceInfusion::IsActive() const
{
  return IsValid() ? !m_Rate->IsZero() : false;
}
//-------------------------------------------------------------------------------
bool SESubstanceInfusion::HasRate() const
{
  return m_Rate == nullptr ? false : m_Rate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SESubstanceInfusion::GetRate()
{
  if (m_Rate == nullptr)
    m_Rate = new SEScalarVolumePerTime();
  return *m_Rate;
}
//-------------------------------------------------------------------------------
bool SESubstanceInfusion::HasConcentration() const
{
  return m_Concentration == nullptr ? false : m_Concentration->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstanceInfusion::GetConcentration()
{
  if (m_Concentration == nullptr)
    m_Concentration = new SEScalarMassPerVolume();
  return *m_Concentration;
}
//-------------------------------------------------------------------------------
SESubstance& SESubstanceInfusion::GetSubstance() const
{
  return (SESubstance&)m_Substance;
}
//-------------------------------------------------------------------------------
void SESubstanceInfusion::ToString(std::ostream& str) const
{
  str << "Patient Action : Substance Infusion";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tRate: ";
  HasRate() ? str << *m_Rate : str << "NaN";
  str << "\n\tConcentration: ";
  HasConcentration() ? str << *m_Concentration : str << "NaN";
  str << "\n\tSubstance: " << m_Substance.GetName();
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SESubstanceInfusion::operator==(const SESubstanceInfusion& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= (m_Concentration && rhs.m_Concentration) ? m_Concentration->operator==(*rhs.m_Concentration) : m_Concentration == rhs.m_Concentration;
  equivilant &= (m_Rate && rhs.m_Rate) ? m_Rate->operator==(*rhs.m_Rate) : m_Rate == rhs.m_Rate;
  equivilant &= m_Substance == rhs.m_Substance;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SESubstanceInfusion::operator!=(const SESubstanceInfusion& rhs) const
{
  return !(*this == rhs);
}
}