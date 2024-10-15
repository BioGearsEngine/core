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
#include <biogears/cdm/substance/SESubstanceConcentration.h>

#include "io/cdm/Substance.h"

#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
SESubstanceConcentration::SESubstanceConcentration(SESubstanceDefinition substance)
  : Loggable(substance.Aerosolization.GetLogger())
  , m_Substance(substance)
{
}
//-------------------------------------------------------------------------------
SESubstanceConcentration::SESubstanceConcentration(SESubstanceDefinition substance, SEScalarMassPerVolume const& concentration)
  : Loggable(substance.Aerosolization.GetLogger())
  , m_Substance(substance)
  , m_Concentration(concentration)
{
}
//-------------------------------------------------------------------------------
SESubstanceConcentration::SESubstanceConcentration(SESubstanceDefinition substance, double concentration, const MassPerVolumeUnit& unit)
  : Loggable(substance.Aerosolization.GetLogger())
  , m_Substance(substance)
  , m_Concentration(concentration, unit)
{
}
//-------------------------------------------------------------------------------
SESubstanceConcentration::~SESubstanceConcentration()
{
}
//-------------------------------------------------------------------------------
void SESubstanceConcentration::Clear()
{
  m_Concentration.Clear();
}

//-------------------------------------------------------------------------------
bool SESubstanceConcentration::HasConcentration() const
{
  return m_Concentration.IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume SESubstanceConcentration::GetConcentration() const
{
  return m_Concentration;
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume& SESubstanceConcentration::GetConcentration()
{
  return m_Concentration;
}
//-------------------------------------------------------------------------------
double SESubstanceConcentration::GetConcentration(const MassPerVolumeUnit& unit) const
{
  return m_Concentration.GetValue(unit);
}
//-------------------------------------------------------------------------------
SESubstanceDefinition const& SESubstanceConcentration::GetSubstance() const
{
  return m_Substance;
}

//-------------------------------------------------------------------------------
bool SESubstanceConcentration::operator==(SESubstanceConcentration const& rhs) const
{
  return m_Substance == rhs.m_Substance
    && m_Concentration == rhs.m_Concentration;
}
//-------------------------------------------------------------------------------
bool SESubstanceConcentration::operator!=(SESubstanceConcentration const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
SESubstanceConcentration& SESubstanceConcentration::operator=(SESubstanceConcentration const& rhs)
{
  m_Substance = rhs.m_Substance;
  m_Concentration = rhs.m_Concentration;
  return *this;
}

}