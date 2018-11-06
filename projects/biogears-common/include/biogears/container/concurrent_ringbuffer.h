#ifndef BIOGEARS_COMMON_CONTAINER_CONCURRENTRINGBUFFER
#define BIOGEARS_COMMON_CONTAINER_CONCURRENTRINGBUFFER

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
//!  Concurrent Ringbuffer using std::queue std::mutex.
//!

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <biogears/framework/concurrent_container.h>

namespace biogears {

  template <typename T>
  class ConcurrentRingbuffer : public concurrent_container<T> {
  public:
    using value_type  = T;
    ConcurrentRingbuffer( const size_t capacity = 10 );
    virtual ~ConcurrentRingbuffer() = default;

    bool insert(T item) override;

    void consume(T& item) override;
    auto consume() -> T override;

    size_t capacity() const;
    size_t unsafe_size() const override;

    void abort() override;
    void shutdown() override;
    bool active() const override;

  protected:
    ConcurrentRingbuffer(const ConcurrentRingbuffer&) = delete;            // disable copying
    ConcurrentRingbuffer& operator=(const ConcurrentRingbuffer&) = delete; // disable assignment

    bool _active;
    size_t _capacity;
    std::queue<T> _queue;
    std::mutex _mutex;
    std::condition_variable _cond;
  };

}
#endif
