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

//Standard Include
#include <map>
//Project Includes
#include <iostream>

#include <biogears/cdm/utils/TimingProfile.h>

namespace biogears {

TimingProfile::TimingProfile(){

}

TimingProfile::TimingProfile(Logger* logger)
: Loggable(logger)
{

}

TimingProfile::~TimingProfile(){

}

void TimingProfile::Clear()
{
  m_timers.clear();
}

void TimingProfile::Reset(const std::string& label)
{
  if (label.empty()) {
    return;
  }
  if (m_timers[label].state == State::Running) {
    m_timers[label].start = std::chrono::high_resolution_clock::now();
  } else if (m_timers[label].state == State::Ran) {
    m_timers[label].state = State::Ready;
  }

  m_ss << "TimingProfile:Reset:" << label << std::ends;
  Debug(m_ss);
}

void TimingProfile::Start(const std::string& label)
{
  if (label.empty()) {
    return;
  }

  m_timers[label].start = std::chrono::high_resolution_clock::now();
  m_timers[label].state = State::Running;

  m_ss << "TimingProfile:Start:" << label << std::ends;
  Debug(m_ss);
}

void TimingProfile::Stop(const std::string& label)
{
  if (label.empty()) {
    return;
  }

  if (m_timers[label].state == State::Running) {
    m_timers[label].end = std::chrono::high_resolution_clock::now();
    m_timers[label].state = State::Ran;
  }

  m_ss << "TimingProfile:Stop:" << label << std::ends;
  Debug(m_ss);
}

double TimingProfile::GetElapsedTime_s(const std::string& label)
{
  if (label.empty()) {
    return 0;
  }

  std::chrono::milliseconds::rep milliseconds = GetElapsedTime<std::chrono::milliseconds>(label);
  double seconds = milliseconds / 1000.0;

  m_ss << "TimingProfile:Profile:" << label << ":" << seconds << "s" << std::ends;
  Debug(m_ss);

  return seconds;
}

void TimingProfile::Print(const std::string& label)
{
  if (label.empty()) {
    for (const std::pair<std::string, Timer>& timerPair : m_timers) {
      if (timerPair.second.state != State::Ready) {
        std::cout << "TimingProfile:Elapsed:" << timerPair.first << ":" << GetElapsedTime_s(timerPair.first) << std::endl;
      }
    }
  } else {
    std::cout << "TimingProfile:Elapsed:" << label << ":" << GetElapsedTime_s(label) << "s" << std::endl;
  }
}
}