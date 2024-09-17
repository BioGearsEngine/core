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
#include <biogears/cdm/substance/SESubstanceFraction.h>

#include "io/cdm/Substance.h"

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
SESubstanceFraction::SESubstanceFraction(SESubstance const& substance)
  : m_Substance(substance)
{
  m_FractionAmount = nullptr;
}
SESubstanceFraction::SESubstanceFraction(SESubstance const& substance, SEScalarFraction const& fraction)
  : m_Substance(substance)
  , m_FractionAmount(nullptr)
{
  GetFractionAmount() = fraction;
}
//-------------------------------------------------------------------------------
SESubstanceFraction::~SESubstanceFraction()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SESubstanceFraction::Clear()
{
  SAFE_DELETE(m_FractionAmount);
}
//-------------------------------------------------------------------------------
bool SESubstanceFraction::HasFractionAmount() const
{
  return m_FractionAmount == nullptr ? false : m_FractionAmount->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SESubstanceFraction::GetFractionAmount()
{
  if (m_FractionAmount == nullptr)
    m_FractionAmount = new SEScalarFraction();
  return *m_FractionAmount;
}
//-------------------------------------------------------------------------------
double SESubstanceFraction::GetFractionAmount() const
{
  if (m_FractionAmount == nullptr)
    SEScalar::dNaN();
  return m_FractionAmount->GetValue();
}
//-------------------------------------------------------------------------------
SESubstance& SESubstanceFraction::GetSubstance() const
{
  return const_cast<SESubstance&>(m_Substance);
}
//-------------------------------------------------------------------------------
bool SESubstanceFraction::operator==(const SESubstanceFraction& rhs) const
{
  bool equivilant = m_Substance == rhs.m_Substance;
  equivilant &= (m_FractionAmount && rhs.m_FractionAmount) ? m_FractionAmount->operator==(*rhs.m_FractionAmount) : m_FractionAmount == rhs.m_FractionAmount;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SESubstanceFraction::operator!=(const SESubstanceFraction& rhs) const
{
  return !(*this == rhs);
}
}