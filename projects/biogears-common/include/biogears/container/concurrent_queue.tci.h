#ifndef BIOGEARS_COMMON_CONTAINER_CONCURRENTQUEUE_IMPL
#define BIOGEARS_COMMON_CONTAINER_CONCURRENTQUEUE_IMPL

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

#include "concurrent_queue.h"

namespace biogears {
//!
//!  Constructs a ConcurrentQueue
//!
  template <typename T>
  ConcurrentQueue<T>::ConcurrentQueue()
    :_active(true)
    , _queue()
    , _mutex()
    , _cond()
  {}


  //-----------------------------------------------------------------------------
  //!
  //!  \param T [IN] : Item to be queued
  //!  Moves an item T into the Queue
  template <typename T>
  bool ConcurrentQueue<T>::insert(T item)
  {
    std::unique_lock<std::mutex> mlock(_mutex);
    if(_active) {
      _queue.push( std::move(item) );
    }
    mlock.unlock();
    _cond.notify_one();

    return true;
  }

  //-----------------------------------------------------------------------------
  //!
  //!  \param  T& [OUT] : Result of the dequeue
  //!  \brief Dequeues an item if it exist else blocks until one is available
  //!         or the queue is shutdown
  template <typename T>
  void ConcurrentQueue<T>::consume(T& item)
  {
    std::unique_lock<std::mutex> mlock(_mutex);
    while (_active && _queue.empty()) {
      _cond.wait(mlock);
    }
    if (!_queue.empty()) {
      item = std::move( _queue.front() );
      _queue.pop();
    }
  }
  //-----------------------------------------------------------------------------
  //!
  //!  \return  T&  : Item to be dequeued
  //!  \brief Dequeues an item if it exist else blocks until one is available
  //!         returns the result by copy
  template <typename T>
  auto ConcurrentQueue<T>::consume() -> T
  {
    std::unique_lock<std::mutex> mlock(_mutex);
    while (_active && _queue.empty()) {
      _cond.wait(mlock);
    }
    T val{};
    if (!_queue.empty()) {
      val = std::move( _queue.front() );
      _queue.pop();
    }
    return val;
  }
  //-----------------------------------------------------------------------------
  //!
  //!  \return  size_t  : Item to be dequeued
  //!  \brief Returns the size of the queue with out obtaining a mutex lock
  //!         Will not be accurate if concurrent queue/dequeues occur during call.
  template <typename T>
  size_t ConcurrentQueue<T>::unsafe_size() const
  {
    return _queue.size();
  }
  //-----------------------------------------------------------------------------
  //!
  //!  \brief Notifies all threads to wake up and abort the current operation once
  //!
  template <typename T>
  void  ConcurrentQueue<T>::abort()
  {
    _cond.notify_all();
  }
  //-----------------------------------------------------------------------------
  //!
  //!  \brief Shuts down the queue so no other operations can take place
  //!         This call can not be done and is used to allow all pending threads to
  //!         shutdown.
  template <typename T>
  void  ConcurrentQueue<T>::shutdown()
  {
    std::unique_lock<std::mutex> mlock(_mutex);
    _active = false;
    _cond.notify_all();
  }
  //-----------------------------------------------------------------------------
  //!
  //! \return bool : The current state of the queue. False if shutdown has been called
  //!
  template <typename T>
  bool  ConcurrentQueue<T>::active() const
  {
    return _active;
  }
  //-----------------------------------------------------------------------------
}
#endif
