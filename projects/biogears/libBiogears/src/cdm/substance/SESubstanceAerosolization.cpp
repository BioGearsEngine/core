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
#include <biogears/cdm/substance/SESubstanceAerosolization.h>

#include "io/cdm/Substance.h"

#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SESubstanceAerosolization::SESubstanceAerosolization(SESubstanceAerosolization const& obj)
  : Loggable(obj.GetLogger())
  , m_BronchioleModifier(obj.m_BronchioleModifier)
  , m_InflammationCoefficient(obj.m_InflammationCoefficient)
  , m_ParticulateSizeDistribution(obj.m_ParticulateSizeDistribution)

{
}

SESubstanceAerosolization::SESubstanceAerosolization(Logger* logger)
  : Loggable(logger)
  , m_BronchioleModifier()
  , m_InflammationCoefficient()
  , m_ParticulateSizeDistribution()
{
}
//-----------------------------------------------------------------------------
SESubstanceAerosolization::~SESubstanceAerosolization()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstanceAerosolization::Clear()
{
  (m_BronchioleModifier.Clear());
  (m_InflammationCoefficient.Clear());
  (m_ParticulateSizeDistribution.Clear());
}
//-----------------------------------------------------------------------------
bool SESubstanceAerosolization::IsValid() const
{
  if (!HasBronchioleModifier())
    return false;
  if (!HasInflammationCoefficient())
    return false;
  if (!HasParticulateSizeDistribution())
    return false;
  return true;
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstanceAerosolization::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstanceAerosolization::GetScalar(const std::string& name)
{
  if (name.compare("BronchioleModifier") == 0)
    return &GetBronchioleModifier();
  if (name.compare("InflammationCoefficient") == 0)
    return &GetInflammationCoefficient();
  return nullptr;
}

//-----------------------------------------------------------------------------
bool SESubstanceAerosolization::HasBronchioleModifier() const
{
  return m_BronchioleModifier.IsValid();
}
//-----------------------------------------------------------------------------
SEScalarNeg1To1& SESubstanceAerosolization::GetBronchioleModifier()
{
  return m_BronchioleModifier;
}
//-----------------------------------------------------------------------------
double SESubstanceAerosolization::GetBronchioleModifier() const
{
  return m_BronchioleModifier.GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceAerosolization::HasInflammationCoefficient() const
{
  return m_InflammationCoefficient.IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SESubstanceAerosolization::GetInflammationCoefficient()
{
  return m_InflammationCoefficient;
}
//-----------------------------------------------------------------------------
double SESubstanceAerosolization::GetInflammationCoefficient() const
{
  return m_InflammationCoefficient.GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceAerosolization::HasParticulateSizeDistribution() const
{
  return m_ParticulateSizeDistribution.IsValid();
}
//-----------------------------------------------------------------------------
SEHistogramFractionVsLength& SESubstanceAerosolization::GetParticulateSizeDistribution()
{
  return m_ParticulateSizeDistribution;
}
//-----------------------------------------------------------------------------
const SEHistogramFractionVsLength SESubstanceAerosolization::GetParticulateSizeDistribution() const
{
  return m_ParticulateSizeDistribution;
}
//-------------------------------------------------------------------------------
bool SESubstanceAerosolization::operator==(const SESubstanceAerosolization& rhs) const
{
  bool equivilant = m_BronchioleModifier == rhs.m_BronchioleModifier;
  equivilant &= m_InflammationCoefficient == rhs.m_InflammationCoefficient;
  equivilant &= m_ParticulateSizeDistribution == rhs.m_ParticulateSizeDistribution;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SESubstanceAerosolization::operator!=(const SESubstanceAerosolization& rhs) const
{
  return !(*this == rhs);
}
}