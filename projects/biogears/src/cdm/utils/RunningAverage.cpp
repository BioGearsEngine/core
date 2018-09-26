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
#include <biogears/cdm/utils/RunningAverage.h>

//Include Standard Includes

//Project Includes
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
RunningAverage::RunningAverage()
{
  Reset();
}

RunningAverage::~RunningAverage()
{
}

bool RunningAverage::Load(const CDM::RunningAverageData& in)
{
  m_Sum = in.sum();
  m_NumSamples = in.numSamples();
  return true;
}
CDM::RunningAverageData* RunningAverage::Unload() const
{
  CDM::RunningAverageData* data = new CDM::RunningAverageData();
  Unload(*data);
  return data;
}
void RunningAverage::Unload(CDM::RunningAverageData& data) const
{
  data.sum(m_Sum);
  data.numSamples(m_NumSamples);
}

void RunningAverage::Reset()
{
  m_Sum = 0.0;
  m_NumSamples = 0;
}

double RunningAverage::Sample(double d)
{
  m_Sum += d;
  m_NumSamples++;
  return Value();
}

double RunningAverage::Value()
{
  return m_Sum / double(m_NumSamples);
}
}