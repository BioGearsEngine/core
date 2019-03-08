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
#include <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.h>

#include <biogears/cdm/properties/SEScalarElectricPotential.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/utils/GeneralMath.h>
#include <biogears/cdm/utils/Logger.h>

namespace biogears {
SEFunctionElectricPotentialVsTime::SEFunctionElectricPotentialVsTime()
  : SEFunction()
{
}

SEFunctionElectricPotentialVsTime::~SEFunctionElectricPotentialVsTime()
{
  Clear();
}

void SEFunctionElectricPotentialVsTime::Clear()
{
  SEFunction::Clear();
  m_TimeUnit = nullptr;
  m_ElectricPotentialUnit = nullptr;
}

bool SEFunctionElectricPotentialVsTime::Load(const CDM::FunctionElectricPotentialVsTimeData& in)
{
  if (!SEFunction::Load(in))
    return false;
  m_TimeUnit = &TimeUnit::GetCompoundUnit(in.IndependentUnit().get());
  m_ElectricPotentialUnit = &ElectricPotentialUnit::GetCompoundUnit(in.DependentUnit().get());
  return IsValid();
}

CDM::FunctionElectricPotentialVsTimeData* SEFunctionElectricPotentialVsTime::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::FunctionElectricPotentialVsTimeData* data(new CDM::FunctionElectricPotentialVsTimeData());
  Unload(*data);
  return data;
}

void SEFunctionElectricPotentialVsTime::Unload(CDM::FunctionElectricPotentialVsTimeData& data) const
{
  SEFunction::Unload(data);
  data.IndependentUnit(m_TimeUnit->GetString());
  data.DependentUnit(m_ElectricPotentialUnit->GetString());
}

double SEFunctionElectricPotentialVsTime::GetTimeValue(unsigned int index, const TimeUnit& unit)
{
  if (m_TimeUnit == nullptr)
    throw CommonDataModelException("No time units have been set");
  if (index >= m_Independent.size())
    throw CommonDataModelException("Independent index out of bounds");
  return Convert(m_Independent[index], *m_TimeUnit, unit);
}
std::vector<double>& SEFunctionElectricPotentialVsTime::GetTime()
{
  return m_Independent;
}
const TimeUnit* SEFunctionElectricPotentialVsTime::GetTimeUnit()
{
  return m_TimeUnit;
}
void SEFunctionElectricPotentialVsTime::SetTimeUnit(const TimeUnit& unit)
{
  m_TimeUnit = &unit;
}

double SEFunctionElectricPotentialVsTime::GetElectricPotentialValue(unsigned int index, const ElectricPotentialUnit& unit)
{
  if (m_ElectricPotentialUnit == nullptr)
    throw CommonDataModelException("No electric potential units have been set");
  if (index >= m_Dependent.size())
    throw CommonDataModelException("Dependent index out of bounds");
  return Convert(m_Dependent[index], *m_ElectricPotentialUnit, unit);
}
std::vector<double>& SEFunctionElectricPotentialVsTime::GetElectricPotential()
{
  return m_Dependent;
}
const ElectricPotentialUnit* SEFunctionElectricPotentialVsTime::GetElectricPotentialUnit()
{
  return m_ElectricPotentialUnit;
}
void SEFunctionElectricPotentialVsTime::SetElectricPotentialUnit(const ElectricPotentialUnit& unit)
{
  m_ElectricPotentialUnit = &unit;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Interpolates the original data to match the engine time step
///
/// \param  newTime  vector of the new time values
/// \param  unit    the unit to use when creating the output
///
/// \return the new waveform data
///
/// \details
/// This function creates the new waveform data for the ECG output by interpolating the data from the
/// original file. It inherits m_Independent and m_Dependent from the original data and calls
/// GeneralMath::LinearInterpolator to create the new vector of voltage points that correspond to the
/// time points in newTime. It is then assigned the unit of the original data and output as the new
/// waveform.
//--------------------------------------------------------------------------------------------------
SEFunctionElectricPotentialVsTime* SEFunctionElectricPotentialVsTime::InterpolateToTime(std::vector<double>& newTime, const TimeUnit& unit)
{
  if (!IsValid())
    return nullptr;

  SEFunctionElectricPotentialVsTime* newFunction = new SEFunctionElectricPotentialVsTime();
  std::vector<double>& fTime = newFunction->GetTime();
  std::vector<double>& fEleP = newFunction->GetElectricPotential();

  for (double t : newTime)
    fTime.push_back(t);
  newFunction->SetTimeUnit(unit);

  //m_Independent;// Original X (Time)
  //m_Dependent;// Original Y (ElectricPotential)
  double x1, x2, y1, y2, xPrime, yPrime;
  unsigned int newTimeIterator = 0;
  for (unsigned int i = 0; i < m_Independent.size(); i++) {
    x1 = GetTimeValue(i, unit); // get the points needed for interpolation.
    x2 = GetTimeValue(i + 1, unit);
    y1 = m_Dependent[i];
    y2 = m_Dependent[i + 1];

    while (newTime[newTimeIterator] >= x1 && newTime[newTimeIterator] < x2) {
      xPrime = newTime[newTimeIterator]; // new time point
      yPrime = GeneralMath::LinearInterpolator(x1, x2, y1, y2, xPrime); // call general math function LinearInterpolator to find yPrime at xPrime, xPrime must be between x1 and x2
      fEleP.push_back(yPrime); // populate the voltage vector
      newTimeIterator++;
      if (newTimeIterator >= newTime.size())
        break;
    }
    if (newTimeIterator >= newTime.size())
      break;
  }
  newFunction->SetElectricPotentialUnit(*m_ElectricPotentialUnit); //

  if (!newFunction->IsValid())
    throw new CommonDataModelException("Could not Interpolate to provided time");
  return newFunction;
}
}