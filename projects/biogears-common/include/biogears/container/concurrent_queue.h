#ifndef BIOGEARS_COMMON_CONTAINER_CONCURRENTQUEUE
#define BIOGEARS_COMMON_CONTAINER_CONCURRENTQUEUE

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

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <biogears/framework/concurrent_container.h>

namespace biogears {

  template <typename T>
  class ConcurrentQueue : public concurrent_container<T> {
  public:
    using value_type  = T;
    ConcurrentQueue();
    virtual ~ConcurrentQueue() = default;

    bool insert ( T  ) override;

    void consume( T&  ) override;
    auto consume( ) ->  T override;

    size_t unsafe_size() const override;

    void abort()    override;
    void shutdown() override;
    bool active() const override;

  protected:
    ConcurrentQueue(const ConcurrentQueue&) = delete;            // disable copying
    ConcurrentQueue& operator=(const ConcurrentQueue&) = delete; // disable assignment

    bool _active;
    std::queue<T> _queue;
    std::mutex    _mutex;
    std::condition_variable _cond;
  };

}
#endif
