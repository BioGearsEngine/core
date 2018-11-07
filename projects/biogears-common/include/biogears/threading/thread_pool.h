#ifndef BIOGEARS_THREADING_THREADPOOL_H
#define BIOGEARS_THREADING_THREADPOOL_H

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
//! \date Sept 22 2017
//! \copyright Copyright 2017 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file 
//!
//! \brief Thread pool Implementation using Steppable
//!

#include <biogears/threading/thread_work_channel.h>
#include <biogears/threading/steppable.h>

#include <biogears/common-exports.h>
namespace biogears {

  class BIOGEARS_COMMON_PUBLIC_API ThreadPool {
  public:
    using SourcePtr = ThreadWorkChannel<void(void)>::SourcePtr;
    using SinkPtr = ThreadWorkChannel<void(void)>::SinkPtr;
    ThreadPool( size_t );
    ThreadPool(const ThreadPool& ) = delete;
    ThreadPool(ThreadPool&& );
    ~ThreadPool();

    void start() ;   //< Starts the Thread Pool;
    void stop()  ;   //< Stops  the thread pool
    bool stop_if_empty();   //< Stops  the thread pool
    void join()  ;   //< Waits till all threads have joined

    bool suspend();  //< Suspends individual work threads even when work is available
    bool resume();//< Resumes  individual work threads to continue working the work queue

    void queue_work(std::function<void(void)>);
    void queue_work(Steppable<void(void)>&);
     
    SourcePtr const get_source();
    SinkPtr   const get_sink();
  private:
    struct Implementation;
    std::unique_ptr<Implementation> _impl;
  };
}

#endif //BIOGEARS_THREADING_POOL_H
