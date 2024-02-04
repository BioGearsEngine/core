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
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolationWaveform.h>
#include <biogears/schema/cdm/Physiology.hxx>
#include <biogears/cdm/properties/SEScalarTime.h>

//Private Includes
#include <io/cdm/ElectroCardioGram.h>

namespace biogears {
SEElectroCardioGramInterpolationWaveform::SEElectroCardioGramInterpolationWaveform(Logger* logger)
  : Loggable(logger)
{
  m_TimeStep = nullptr;
  m_Data = nullptr;
  m_Rhythm = (CDM::enumHeartRhythm::value)-1;
  m_LeadNumber = 0;
}
//-----------------------------------------------------------------------------
SEElectroCardioGramInterpolationWaveform::~SEElectroCardioGramInterpolationWaveform()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEElectroCardioGramInterpolationWaveform::Clear()
{
  m_LeadNumber = 0;
  m_Rhythm = (CDM::enumHeartRhythm::value)-1;
  SAFE_DELETE(m_TimeStep);
  SAFE_DELETE(m_Data);
  m_ActiveIndicies.clear();
}
//-----------------------------------------------------------------------------
bool SEElectroCardioGramInterpolationWaveform::Load(const CDM::ElectroCardioGramInterpolationWaveformData& in)
{
  io::ElectroCardioGram::UnMarshall(in, *this);
  return true;
}
//-----------------------------------------------------------------------------
CDM::ElectroCardioGramInterpolationWaveformData* SEElectroCardioGramInterpolationWaveform::Unload() const
{
  CDM::ElectroCardioGramInterpolationWaveformData* data(new CDM::ElectroCardioGramInterpolationWaveformData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEElectroCardioGramInterpolationWaveform::Unload(CDM::ElectroCardioGramInterpolationWaveformData& data) const
{
  io::ElectroCardioGram::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
CDM::ElectroCardioGramWaveformLeadNumberData SEElectroCardioGramInterpolationWaveform::GetLeadNumber() const
{
  return m_LeadNumber;
}
void SEElectroCardioGramInterpolationWaveform::SetLeadNumber(CDM::ElectroCardioGramWaveformLeadNumberData n)
{
  m_LeadNumber = n;
}
bool SEElectroCardioGramInterpolationWaveform::HasLeadNumber() const
{
  return m_LeadNumber == 0 ? false : true;
}
void SEElectroCardioGramInterpolationWaveform::InvalidateLeadNumber()
{
  m_LeadNumber = 0;
}
//-----------------------------------------------------------------------------
CDM::enumHeartRhythm::value SEElectroCardioGramInterpolationWaveform::GetRhythm() const
{
  return m_Rhythm;
}
void SEElectroCardioGramInterpolationWaveform::SetRhythm(CDM::enumHeartRhythm::value rhythm)
{
  m_Rhythm = rhythm;
}
bool SEElectroCardioGramInterpolationWaveform::HasRhythm() const
{
  return m_Rhythm == ((CDM::enumHeartRhythm::value)-1) ? false : true;
}
void SEElectroCardioGramInterpolationWaveform::InvalidateRhythm()
{
  m_Rhythm = (CDM::enumHeartRhythm::value)-1;
}
//-----------------------------------------------------------------------------
bool SEElectroCardioGramInterpolationWaveform::HasData() const
{
  return m_Data == nullptr ? false : m_Data->IsValid();
}
SEFunctionElectricPotentialVsTime& SEElectroCardioGramInterpolationWaveform::GetData()
{
  if (m_Data == nullptr)
    m_Data = new SEFunctionElectricPotentialVsTime();
  return *m_Data;
}
const SEFunctionElectricPotentialVsTime* SEElectroCardioGramInterpolationWaveform::GetData() const
{
  return m_Data;
}
//-----------------------------------------------------------------------------
bool SEElectroCardioGramInterpolationWaveform::HasTimeStep() const
{
  return m_TimeStep == nullptr ? false : m_TimeStep->IsValid();
}
SEScalarTime& SEElectroCardioGramInterpolationWaveform::GetTimeStep()
{
  if (m_TimeStep == nullptr)
    m_TimeStep = new SEScalarTime();
  return *m_TimeStep;
}
double SEElectroCardioGramInterpolationWaveform::GetTimeStep(const TimeUnit& unit) const
{
  if (m_TimeStep == nullptr)
    return SEScalar::dNaN();
  return m_TimeStep->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SEElectroCardioGramInterpolationWaveform::operator==(SEElectroCardioGramInterpolationWaveform const& rhs) const
{
  if (this == &rhs)
    return true;

  return m_LeadNumber == rhs.m_LeadNumber
    && m_Rhythm == rhs.m_Rhythm
    && ((m_TimeStep && rhs.m_TimeStep) ? m_TimeStep->operator==(*rhs.m_TimeStep) : m_TimeStep == rhs.m_TimeStep)
    && ((m_Data && rhs.m_Data) ? m_Data->operator==(*rhs.m_Data) : m_Data == rhs.m_Data)
    && m_ActiveIndicies == rhs.m_ActiveIndicies;
}
bool SEElectroCardioGramInterpolationWaveform::operator!=(SEElectroCardioGramInterpolationWaveform const& rhs) const
{
  return !(*this == rhs);
}
//-----------------------------------------------------------------------------
}