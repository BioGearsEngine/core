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
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolatorWaveform.h>
#include <biogears/schema/cdm/Physiology.hxx>
#include <biogears/cdm/properties/SEScalarTime.h>

namespace biogears {
SEElectroCardioGramInterpolatorWaveform::SEElectroCardioGramInterpolatorWaveform(Logger* logger)
  : Loggable(logger)
{
  m_TimeStep = nullptr;
  m_Data = nullptr;
  m_Rhythm = (CDM::enumHeartRhythm::value)-1;
  m_LeadNumber = 0;
}

SEElectroCardioGramInterpolatorWaveform::~SEElectroCardioGramInterpolatorWaveform()
{
  Clear();
}

void SEElectroCardioGramInterpolatorWaveform::Clear()
{
  m_LeadNumber = 0;
  m_Rhythm = (CDM::enumHeartRhythm::value)-1;
  SAFE_DELETE(m_TimeStep);
  SAFE_DELETE(m_Data);
  m_ActiveIndicies.clear();
}

bool SEElectroCardioGramInterpolatorWaveform::Load(const CDM::ElectroCardioGramInterpolationWaveformData& in)
{
  Clear();
  m_Rhythm = in.Rhythm();
  m_LeadNumber = in.Lead();
  GetData().Load(in.Data());
  if (in.TimeStep().present())
    GetTimeStep().Load(in.TimeStep().get());
  if (in.ActiveIndicies().present()) {
    for (size_t i = 0; i < in.ActiveIndicies().get().IntegerList().size(); i++)
      m_ActiveIndicies.push_back(in.ActiveIndicies().get().IntegerList()[i]);
  }
  return true;
}

CDM::ElectroCardioGramInterpolationWaveformData* SEElectroCardioGramInterpolatorWaveform::Unload() const
{
  CDM::ElectroCardioGramInterpolationWaveformData* data(new CDM::ElectroCardioGramInterpolationWaveformData());
  Unload(*data);
  return data;
}

void SEElectroCardioGramInterpolatorWaveform::Unload(CDM::ElectroCardioGramInterpolationWaveformData& data) const
{
  if (HasRhythm())
    data.Rhythm(m_Rhythm);
  if (HasLeadNumber())
    data.Lead(m_LeadNumber);
  if (HasData()) {
    data.Data(std::unique_ptr<CDM::FunctionElectricPotentialVsTimeData>(m_Data->Unload()));
    data.ActiveIndicies(std::unique_ptr<CDM::IntegerArray>(new CDM::IntegerArray()));
    data.ActiveIndicies().get().IntegerList(std::unique_ptr<CDM::IntegerList>(new CDM::IntegerList()));
    for (int i : m_ActiveIndicies)
      data.ActiveIndicies().get().IntegerList().push_back(i);
  }
  if (HasTimeStep())
    data.TimeStep(std::unique_ptr<CDM::ScalarTimeData>(m_TimeStep->Unload()));
}

CDM::ElectroCardioGramWaveformLeadNumber SEElectroCardioGramInterpolatorWaveform::GetLeadNumber() const
{
  return m_LeadNumber;
}
void SEElectroCardioGramInterpolatorWaveform::SetLeadNumber(CDM::ElectroCardioGramWaveformLeadNumber n)
{
  m_LeadNumber = n;
}
bool SEElectroCardioGramInterpolatorWaveform::HasLeadNumber() const
{
  return m_LeadNumber == 0 ? false : true;
}
void SEElectroCardioGramInterpolatorWaveform::InvalidateLeadNumber()
{
  m_LeadNumber = 0;
}

CDM::enumHeartRhythm::value SEElectroCardioGramInterpolatorWaveform::GetRhythm() const
{
  return m_Rhythm;
}
void SEElectroCardioGramInterpolatorWaveform::SetRhythm(CDM::enumHeartRhythm::value rhythm)
{
  m_Rhythm = rhythm;
}
bool SEElectroCardioGramInterpolatorWaveform::HasRhythm() const
{
  return m_Rhythm == ((CDM::enumHeartRhythm::value)-1) ? false : true;
}
void SEElectroCardioGramInterpolatorWaveform::InvalidateRhythm()
{
  m_Rhythm = (CDM::enumHeartRhythm::value)-1;
}

bool SEElectroCardioGramInterpolatorWaveform::HasData() const
{
  return m_Data == nullptr ? false : m_Data->IsValid();
}
SEFunctionElectricPotentialVsTime& SEElectroCardioGramInterpolatorWaveform::GetData()
{
  if (m_Data == nullptr)
    m_Data = new SEFunctionElectricPotentialVsTime();
  return *m_Data;
}
const SEFunctionElectricPotentialVsTime* SEElectroCardioGramInterpolatorWaveform::GetData() const
{
  return m_Data;
}

bool SEElectroCardioGramInterpolatorWaveform::HasTimeStep() const
{
  return m_TimeStep == nullptr ? false : m_TimeStep->IsValid();
}
SEScalarTime& SEElectroCardioGramInterpolatorWaveform::GetTimeStep()
{
  if (m_TimeStep == nullptr)
    m_TimeStep = new SEScalarTime();
  return *m_TimeStep;
}
double SEElectroCardioGramInterpolatorWaveform::GetTimeStep(const TimeUnit& unit) const
{
  if (m_TimeStep == nullptr)
    return SEScalar::dNaN();
  return m_TimeStep->GetValue(unit);
}
}