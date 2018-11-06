#ifndef BIOGEARS_COMMON_CHRONO_STOPWATCH
#define BIOGEARS_COMMON_CHRONO_STOPWATCH

//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.apache.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------

//!
//! \author Steven A White
//! \date   Aug 24th 2017
//!
//!  Portable timming code using c++11 chrono
//!

#include <chrono>

namespace biogears {

  template<typename ConstructedDuration> //<!std::chrono::duration
  class StopWatch {
  public:
    using rep = typename ConstructedDuration::rep;
    using sec = std::chrono::seconds;
    using msec = std::chrono::milliseconds;
    using nsec = std::chrono::nanoseconds;

    StopWatch();
    void reset();                                           //< Initialize the clock

    auto total() -> rep;                                    //< Set the end time and calculate the duration

    template<typename RequestedDuration>                    //< calling total multiple times will give you increasing durations
    auto total() -> typename RequestedDuration::rep;

    auto total_s()  -> typename  sec::rep;
    auto total_ms() -> typename  msec::rep;
    auto total_ns() -> typename  nsec::rep;

    auto lap() -> rep;                                      //< Like total, but sets initial to the final of the current duration
    template<typename RequestedDuration>
    auto lap() -> typename RequestedDuration::rep;

    auto lap_s()  -> typename sec::rep;
    auto lap_ms() -> typename msec::rep;
    auto lap_ns() -> typename nsec::rep;

  private:
    using time_point = std::chrono::time_point<std::chrono::steady_clock>;
    time_point initial;
    time_point lap_initial;
    time_point final;
  };
  
} //namespace biogears

#endif //BIOGEARS_COMMON_CHRONO_STOPWATCH

