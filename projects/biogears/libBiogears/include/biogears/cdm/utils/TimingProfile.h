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

#pragma once

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/utils/Logger.h>

#include <chrono>
#include <map>
#include <string>

namespace biogears {


enum class State {
  Ready, // Timer has no data and is not running
  Running, // Timer is currently running
  Ran // Timer has been stopped and has data
};

struct Timer {
  std::chrono::high_resolution_clock::time_point start;
  std::chrono::high_resolution_clock::time_point end;
  State state = State::Ready;
};

BG_EXT template class BIOGEARS_API std::map<std::string, biogears::Timer>;

class BIOGEARS_API TimingProfile : public Loggable {
public:
  TimingProfile();
  TimingProfile(Logger* logger);
  virtual ~TimingProfile();

  void Clear();

  /**
     * Resets the timer, this can have different effects based on the state of the timer:
     *    1) Ready   - No effect
     *    2) Running - Resets the elapsed time to zero and continues running
     *    3) Ran     - Resets the elapsed time to zero and places the timer back into the Ready state
     */
  void Reset(const std::string& label);

  /** 
     * Starts the timer.  Calling Start while the timer is running will reset its elapsed time.  Calling Start
     * after the timer has been started and stopped will reset its elapsed time, it does not pause and unpause
     * the running elapsed time.
     */
  void Start(const std::string& label);

  /** 
     * Stops the timer, elapsed time will remain constant until calling Reset or Start
     */
  void Stop(const std::string& label);

  /**
     * Returns the total elapsed time in seconds to three decimal places.  If the timer is running, this function
     * returns the elapsed time up until now.  If the timer has been stopped, this function returns the elapsed time
     * between the start and stop.  If the timer hasn't been run yet (Ready state), this function returns 0.
     */
  double GetElapsedTime_s(const std::string& label);

  /**
     * Prints the time from a call to Profile to console
     * If nullptr is passed, all labels will be computed and printed
     */
  void Print(const std::string& label);

  /**
     * Returns the elapsed time for a particular timer
     */
  template <typename Duration>
  typename Duration::rep GetElapsedTime(const std::string& label)
  {
    biogears::State state = m_timers[label].state;

    if (state == biogears::State::Running) {
      return std::chrono::duration_cast<Duration>(std::chrono::high_resolution_clock::now() - m_timers[label].start).count();
    } else if (state == biogears::State::Ran) {
      return std::chrono::duration_cast<Duration>(m_timers[label].end - m_timers[label].start).count();
    } else {
      return typename Duration::rep(0);
    }
  }

private:
  std::map<std::string, biogears::Timer> m_timers;

  std::stringstream m_ss;
};
}