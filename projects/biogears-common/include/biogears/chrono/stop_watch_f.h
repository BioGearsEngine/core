#ifndef BIOGEARS_COMMON_CHRONO_StopWatch_f_H
#define BIOGEARS_COMMON_CHRONO_StopWatch_f_H

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
  class StopWatch_f {
  public:
    using rep =  double;
    using sec =  std::chrono::duration<double, std::ratio<1,1> >;
    using msec = std::chrono::duration<double, std::milli>;
    using nsec = std::chrono::duration<double, std::nano>;

    StopWatch_f();
    void reset();                                           //< Initialize the clock

    auto total() -> rep;                                    //< Set the end time and calculate the duration

    template<typename RequestedDuration>                    //< calling total multiple times will give you increasing durations
    auto total() ->  rep;

    auto total_s()  ->   sec::rep;
    auto total_ms() ->   msec::rep;
    auto total_ns() ->   nsec::rep;

    auto lap() ->  rep;                                      //< Like total, but sets initial to the final of the current duration
    template<typename RequestedDuration>
    auto lap() ->  rep;

    auto lap_s()  ->  sec::rep;
    auto lap_ms() ->  msec::rep;
    auto lap_ns() ->  nsec::rep;

  private:
    using time_point = std::chrono::time_point<std::chrono::steady_clock>;
    time_point initial;
    time_point lap_initial;
    time_point final;
  };
  
} //namespace biogears

#endif //BIOGEARS_COMMON_CHRONO_StopWatch_fFP_H

