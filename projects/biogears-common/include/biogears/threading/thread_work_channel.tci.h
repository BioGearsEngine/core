#ifndef BIOGEARS_COMMON_THREADWORKCHANNEL_IMPL
#define BIOGEARS_COMMON_THREADWORKCHANNEL_IMPL

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

//! \author Steven A White
//! \date   Sept 7 2017
//! \copyright Copyright 2017 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file 
//!
//!  Concurrent Queue using std::queue std::mutex.
//!

#include <biogears/threading/thread_work_channel.h>

#include <biogears/container/concurrent_queue.tci.h>

namespace biogears {
template<typename Signiture>
ThreadWorkChannel<Signiture>::ThreadWorkChannel()
:_queue()
{

}
//---------------------------------------------------------------------------
template<typename Signiture>
std::shared_ptr<ThreadWorkChannel<Signiture>> ThreadWorkChannel<Signiture>::make_shared()
{
	return  std::shared_ptr<ThreadWorkChannel<Signiture>>(new ThreadWorkChannel<Signiture>());
}
//---------------------------------------------------------------------------
template<typename Signiture>
bool ThreadWorkChannel<Signiture>::insert(std::function<Signiture>& func)
{
	return _queue.insert(func);
}
//---------------------------------------------------------------------------
template<typename Signiture>
bool ThreadWorkChannel<Signiture>::insert(std::function<Signiture>&& func)
{
	return _queue.insert(std::move(func));
}
//---------------------------------------------------------------------------
template<typename Signiture>
void ThreadWorkChannel<Signiture>::consume(std::function<Signiture>& func)
{
	_queue.consume(func);
}
//---------------------------------------------------------------------------
template<typename Signiture>
std::function<Signiture> ThreadWorkChannel<Signiture>::consume()
{
	std::function<Signiture> func;
	_queue.consume(func);
	return func;
}
//---------------------------------------------------------------------------
template<typename Signiture>
size_t ThreadWorkChannel<Signiture>::unsafe_size() const
{
	return _queue.unsafe_size();
}
//---------------------------------------------------------------------------
template<typename Signiture>
void ThreadWorkChannel<Signiture>::abort()
{
	_queue.abort();
}
//---------------------------------------------------------------------------
template<typename Signiture>
void ThreadWorkChannel<Signiture>::shutdown()
{
	_queue.shutdown();
}
//---------------------------------------------------------------------------
template<typename Signiture>
auto ThreadWorkChannel<Signiture>::as_source() -> SourcePtr
{
	return this->shared_from_this();
}
//---------------------------------------------------------------------------
template<typename Signiture>
auto ThreadWorkChannel<Signiture>::as_sink() -> SinkPtr
{
	return this->shared_from_this();
}
//---------------------------------------------------------------------------
}

#endif //BIOGEARS_COMMON_THREADWORKCHANNEL_IMPL
