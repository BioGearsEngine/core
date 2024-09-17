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
SESubstanceAerosolization::SESubstanceAerosolization(Logger* logger)
  : Loggable(logger)
{
  m_BronchioleModifier = nullptr;
  m_InflammationCoefficient = nullptr;
  m_ParticulateSizeDistribution = nullptr;
}
//-----------------------------------------------------------------------------
SESubstanceAerosolization::~SESubstanceAerosolization()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstanceAerosolization::Clear()
{
  SAFE_DELETE(m_BronchioleModifier);
  SAFE_DELETE(m_InflammationCoefficient);
  SAFE_DELETE(m_ParticulateSizeDistribution);
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
  return (m_BronchioleModifier == nullptr) ? false : m_BronchioleModifier->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarNeg1To1& SESubstanceAerosolization::GetBronchioleModifier()
{
  if (m_BronchioleModifier == nullptr)
    m_BronchioleModifier = new SEScalarNeg1To1();
  return *m_BronchioleModifier;
}
//-----------------------------------------------------------------------------
double SESubstanceAerosolization::GetBronchioleModifier() const
{
  if (m_BronchioleModifier == nullptr)
    return SEScalar::dNaN();
  return m_BronchioleModifier->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceAerosolization::HasInflammationCoefficient() const
{
  return (m_InflammationCoefficient == nullptr) ? false : m_InflammationCoefficient->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SESubstanceAerosolization::GetInflammationCoefficient()
{
  if (m_InflammationCoefficient == nullptr)
    m_InflammationCoefficient = new SEScalar0To1();
  return *m_InflammationCoefficient;
}
//-----------------------------------------------------------------------------
double SESubstanceAerosolization::GetInflammationCoefficient() const
{
  if (m_InflammationCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_InflammationCoefficient->GetValue();
}
//-----------------------------------------------------------------------------
bool SESubstanceAerosolization::HasParticulateSizeDistribution() const
{
  return (m_ParticulateSizeDistribution == nullptr) ? false : m_ParticulateSizeDistribution->IsValid();
}
//-----------------------------------------------------------------------------
SEHistogramFractionVsLength& SESubstanceAerosolization::GetParticulateSizeDistribution()
{
  if (m_ParticulateSizeDistribution == nullptr)
    m_ParticulateSizeDistribution = new SEHistogramFractionVsLength();
  return *m_ParticulateSizeDistribution;
}
//-----------------------------------------------------------------------------
const SEHistogramFractionVsLength* SESubstanceAerosolization::GetParticulateSizeDistribution() const
{
  return m_ParticulateSizeDistribution;
}
//-------------------------------------------------------------------------------
bool SESubstanceAerosolization::operator==( const SESubstanceAerosolization& rhs) const
{
  bool equivilant = (m_BronchioleModifier && rhs.m_BronchioleModifier) ? m_BronchioleModifier->operator==(*rhs.m_BronchioleModifier) : m_BronchioleModifier == rhs.m_BronchioleModifier;
  equivilant &= (m_InflammationCoefficient && rhs.m_InflammationCoefficient) ? m_InflammationCoefficient->operator==(*rhs.m_InflammationCoefficient) : m_InflammationCoefficient == rhs.m_InflammationCoefficient;
  equivilant &= (m_ParticulateSizeDistribution && rhs.m_ParticulateSizeDistribution) ? m_ParticulateSizeDistribution->operator==(*rhs.m_ParticulateSizeDistribution) : m_ParticulateSizeDistribution == rhs.m_ParticulateSizeDistribution;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SESubstanceAerosolization::operator!=( const SESubstanceAerosolization& rhs) const
{
  return !(*this == rhs);
}
}