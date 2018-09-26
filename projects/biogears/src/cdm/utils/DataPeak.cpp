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

#include <biogears/cdm/utils/DataPeak.h>

namespace biogears {
DataPeak::DataPeak(unsigned int samples, double epsilon)
{
  m_Samples = samples;
  m_Epsilon = epsilon;
  m_FindMax = true;
  m_FindMin = false;
  Reset();
}

void DataPeak::Reset()
{
  m_PeakCount = 0;
  m_SamplePeak = 0;
  m_LastTopPeak = 0;
  m_LastBottomPeak = 0;
  m_PositiveTrend = true;
}

double DataPeak::LastTopPeak()
{
  return m_LastTopPeak;
}

double DataPeak::LastBottomPeak()
{
  return m_LastBottomPeak;
}

bool DataPeak::Peaked(double value)
{
  if (m_PositiveTrend) {
    double p = m_SamplePeak - m_Epsilon;
    if (value < p) {
      // We may want to move the window to extend
      // over the down trending point-epsilon to enforce
      // the data is actually trending down.
      // Make sure the data is moving below a moving window
      // Currently the window stays at the peak
      // But you could get a plateau below the window
      // then another peak
      //                  .--.
      //        peak     /    \
      //       .--      .
      // ----./---\..../-------window
      //   ./
      m_PeakCount++;
      if (m_PeakCount == m_Samples) {
        m_PeakCount = 0;
        m_LastTopPeak = m_SamplePeak;
        m_SamplePeak = value;
        m_PositiveTrend = false;
        return m_FindMax ? true : false;
      }
    } else if (value >= p && value <= m_SamplePeak) {
      // Do Nothing, in limbo right now...
    } else {
      m_PeakCount = 0;
      m_SamplePeak = value;
    }
  } else {
    double p = m_SamplePeak + m_Epsilon;
    if (value > p) {
      m_PeakCount++;
      if (m_PeakCount == m_Samples) {
        m_PeakCount = 0;
        m_LastBottomPeak = m_SamplePeak;
        m_SamplePeak = value;
        m_PositiveTrend = true;
        return m_FindMin ? true : false;
      }
    } else if (value <= p && value >= m_SamplePeak) {
      // Do Nothing, in limbo right now...
    } else {
      m_PeakCount = 0;
      m_SamplePeak = value;
    }
  }
  return false;
}
}