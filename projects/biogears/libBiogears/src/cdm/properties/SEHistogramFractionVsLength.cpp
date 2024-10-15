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
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
// Standard Includes
#include <iostream>
#include <numeric>
// Project Includes

#include "io/cdm/Property.h"

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarLength.h>

namespace biogears {
SEHistogramFractionVsLength::SEHistogramFractionVsLength(SEHistogramFractionVsLength const& obj)
  : SEHistogram(obj)
  , m_LengthUnit(obj.m_LengthUnit)
{

}
SEHistogramFractionVsLength::SEHistogramFractionVsLength()
  : SEHistogram()
  , m_LengthUnit(nullptr)
{
}

SEHistogramFractionVsLength::~SEHistogramFractionVsLength()
{
  Clear();
}

void SEHistogramFractionVsLength::Clear()
{
  SEHistogram::Clear();
  m_LengthUnit = nullptr;
}

bool SEHistogramFractionVsLength::IsVaild() const
{
  if (!SEHistogram::IsValid())
    return false;

  double binned = std::accumulate(m_Dependent.begin(), m_Dependent.end(), 0.);
  if (!SEScalar::IsValue(1, binned)) {
    std::cerr << "Particle distribution histogram: Failed to correct a distribution error. Check distribution histogram." << std::endl; // todo properties need a logger
    return false;
  }
  return true;
}

double SEHistogramFractionVsLength::GetLengthValue(unsigned int index, const LengthUnit& unit) const
{
  if (m_LengthUnit == nullptr)
    throw CommonDataModelException("No length units have been set");
  if (index >= m_Independent.size())
    throw CommonDataModelException("Independent index out of bounds");
  return Convert(m_Independent[index], *m_LengthUnit, unit);
}
std::vector<double>& SEHistogramFractionVsLength::GetLength()
{
  return m_Independent;
}
const std::vector<double>& SEHistogramFractionVsLength::GetLength() const
{
  return m_Independent;
}
const LengthUnit* SEHistogramFractionVsLength::GetLengthUnit() const
{
  return m_LengthUnit;
}
void SEHistogramFractionVsLength::SetLengthUnit(const LengthUnit& unit)
{
  m_LengthUnit = &unit;
}

double SEHistogramFractionVsLength::GetFractionValue(unsigned int index) const
{
  if (index >= m_Dependent.size())
    throw CommonDataModelException("Dependent index out of bounds");
  return m_Dependent[index];
}
std::vector<double>& SEHistogramFractionVsLength::GetFraction()
{
  return m_Dependent;
}
const std::vector<double>& SEHistogramFractionVsLength::GetFraction() const
{
  return m_Dependent;
}
//-------------------------------------------------------------------------------
SEHistogramFractionVsLength& SEHistogramFractionVsLength::operator=(const SEHistogramFractionVsLength& rhs)
{
  if (this != &rhs) {
    m_LengthUnit = rhs.m_LengthUnit;
    SEHistogram::operator=(rhs);
  }

  return *this;
}
//-------------------------------------------------------------------------------
bool SEHistogramFractionVsLength::operator==(const SEHistogramFractionVsLength& rhs) const
{
  bool equivilant = (m_LengthUnit && rhs.m_LengthUnit) ? m_LengthUnit->operator==(*rhs.m_LengthUnit) : m_LengthUnit == rhs.m_LengthUnit;
  equivilant &= SEHistogram::operator==(rhs);
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEHistogramFractionVsLength::operator!=(const SEHistogramFractionVsLength& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}