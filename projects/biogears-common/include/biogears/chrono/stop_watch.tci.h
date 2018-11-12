#ifndef BIOGEARS_CHRONO_STOPWATCH_TCI_H
#define BIOGEARS_CHRONO_STOPWATCH_TCI_H

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

#include <biogears/chrono/stop_watch.h>

namespace biogears {
//!
//!  \brief Sets initial to the time of construction
//!         call reset if this isn't the intended interval to me measured
//!
template<typename ConstructedDuration>
StopWatch<ConstructedDuration>::StopWatch()
	:initial(std::chrono::steady_clock::now())
	, lap_initial(initial)
	, final(initial)
{}
//-----------------------------------------------------------------------------

//!
//!  \brief Resets the initial time_point used for calculating intervals
//!         to the current time
//!
template<typename ConstructedDuration>
void StopWatch<ConstructedDuration>::reset()
{
	initial = std::chrono::steady_clock::now();
	lap_initial = initial;
	final = initial;
}
//-----------------------------------------------------------------------------
//!
//!  \brief sets the final time_point used for calculating intervals
//!  \return duration.count() : (final - initial)
//!
template<typename ConstructedDuration>
auto StopWatch<ConstructedDuration>::total() -> typename ConstructedDuration::rep
{
	final = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<ConstructedDuration>(final - initial).count();
}
//-----------------------------------------------------------------------------
//!
//!  \brief  Templated version of total for overriding the output duration type
//!          sets the final time_point used for calculating intervals
//!  \return duration.count() : (final - initial)
//!
template<typename ConstructedDuration>
template<typename RequestedDuration>
auto StopWatch<ConstructedDuration>::total() -> typename RequestedDuration::rep
{
	final = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<RequestedDuration>(final - initial).count();
}

//-----------------------------------------------------------------------------
template<typename ConstructedDuration>
auto StopWatch<ConstructedDuration>::total_s() -> typename sec::rep
{
	return total<std::chrono::seconds>();
}

//-----------------------------------------------------------------------------
template<typename ConstructedDuration>
auto StopWatch<ConstructedDuration>::total_ms() -> typename msec::rep
{
	return total<std::chrono::milliseconds>();
}

//-----------------------------------------------------------------------------
template<typename ConstructedDuration>
auto StopWatch<ConstructedDuration>::total_ns() -> typename nsec::rep
{
	return total<std::chrono::nanoseconds>();
}

//-----------------------------------------------------------------------------
//!
//!  \brief  Sets the final time_point used for calculating intervals Then sets
//!          initial to steady_clock::now() for the next interval calculation
//!  \return duration.count() : (final - initial)
//!
template<typename ConstructedDuration>
auto StopWatch<ConstructedDuration>::lap() -> typename ConstructedDuration::rep
{
	auto final = std::chrono::steady_clock::now();
	auto durration = std::chrono::duration_cast<ConstructedDuration>(final - lap_initial).count();
	lap_initial = final;
	return durration;
}
//-----------------------------------------------------------------------------
//!
//!  \brief  Templated version of total for overriding the output duration type
//!          sets the final time_point calculates final-initial. Then sets
//!          initial to steady_clock::now() for the next interval calculation
//!  \return duration.count() : (final - initial)
//!
template<typename ConstructedDuration>
template<typename RequestedDuration>
auto StopWatch<ConstructedDuration>::lap() -> typename RequestedDuration::rep
{
	auto final = std::chrono::steady_clock::now();
	auto durration = std::chrono::duration_cast<RequestedDuration>(final - lap_initial).count();
	lap_initial = final;
	return durration;
}

//-----------------------------------------------------------------------------
template<typename ConstructedDuration>
auto StopWatch<ConstructedDuration>::lap_s()  -> typename sec::rep
{
	return lap<sec>();
}

//-----------------------------------------------------------------------------
template<typename ConstructedDuration>
auto StopWatch<ConstructedDuration>::lap_ms() -> typename msec::rep
{
	return lap<msec>();
}

//-----------------------------------------------------------------------------
template<typename ConstructedDuration>
auto StopWatch<ConstructedDuration>::lap_ns() -> typename nsec::rep
{
	return lap<std::chrono::nanoseconds>();
}

}

#endif //BIOGEARS_CHRONO_STOPWATCH_TCI_H
