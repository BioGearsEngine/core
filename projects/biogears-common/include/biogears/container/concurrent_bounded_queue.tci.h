#ifndef BIOGEARS_CONTAINER_CONCURRENTBOUNDEDQUEUE_TCI_H
#define BIOGEARS_CONTAINER_CONCURRENTBOUNDEDQUEUE_TCI_H

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
//! Bounded Concurrent Queue using std::queue std::mutex.
//!
#include <biogears/container/concurrent_bounded_queue.h>

namespace biogears {
  //!
  //! Constructs  a concurrent Bounded Queue
  //!
  template <typename T>
  ConcurrentBoundedQueue<T>::ConcurrentBoundedQueue( const size_t cap)
    :_active(true)
    , _capacity(cap)
    , _queue()
    , _mutex()
    , _cond()
  {  }

	//-----------------------------------------------------------------------------
  //!
  //! \param  T&&[INOUT] : New Element to be moved in the queue
  //!                      if size exceeds capacity block until the next pop.
  //! \return bool : true if queue is active.
  //!
  //! Pushes item on the queue. Blocks when the queue exceeds its bounds.
  //!
  template <typename T>
  bool ConcurrentBoundedQueue<T>::insert(T item)
  {
    std::unique_lock<std::mutex> mlock(_mutex);
    while (_active && _queue.size() == _capacity) {
      _cond.wait(mlock);
    }
    if(_active) {  _queue.push( std::move(item) );  }

    mlock.unlock();
    _cond.notify_one();

    return _active;
  }

  //-----------------------------------------------------------------------------
  //!
  //!  \param  T& [OUT] : Result of the dequeue
  //!  \brief Dequeues an item if it exist else blocks until one is available
  //!         or the queue is shutdown.
  template <typename T>
  void ConcurrentBoundedQueue<T>::consume(T& item)
  {
    std::unique_lock<std::mutex> mlock(_mutex);
    while (_active && _queue.empty()) {
      _cond.wait(mlock);
    }
    if(! _queue.empty() ) {
      item = std::move (_queue.front() );
      _queue.pop();
    }
  }
  //-----------------------------------------------------------------------------
  //!
  //! \return T : the front of the queue at the time of the pop() call
  //!
  //! Thread safe block using a std::mutex.
  //! Call is also blocking on empty() so consumers can block indefenitly
  //! if no element is ever produced.
  template <typename T>
  auto ConcurrentBoundedQueue<T>::consume() -> T
  {
    std::unique_lock<std::mutex> mlock(_mutex);
    while (_active && _queue.empty()) {
      _cond.wait(mlock);
    }
    T val{};
    if(! _queue.empty() ) {
      val = std::move( _queue.front() );
      _queue.pop();
    }
    return val;
  }
  //-----------------------------------------------------------------------------
  //!
  //! \return size_t : The result the maximum capacity of the BoundedQueue
  //!
  template <typename T>
  size_t ConcurrentBoundedQueue<T>::capacity() const
  {
    return _capacity;
  }
  //-----------------------------------------------------------------------------
  //!
  //! \return size_t : The result of _queue.size().
  //!
  //! A non thread_safe size check. If MPSC then the size will never be less then
  //! the value of the current result.
  template <typename T>
  size_t ConcurrentBoundedQueue<T>::unsafe_size() const
  {
    return _queue.size();
  }
  //-----------------------------------------------------------------------------
  //!
  //!  \brief Notifies all threads to wake up and abort the current operation once
  //!
  template <typename T>
  void ConcurrentBoundedQueue<T>::shutdown()
  {
    std::unique_lock<std::mutex> mlock(_mutex);
    _active = false;
    _cond.notify_all();
  }
  //-----------------------------------------------------------------------------
  //!
  //!  \brief Shuts down the queue so no other operations can take place
  //!         This call can not be done and is used to allow all pending threads to
  //!         shutdown.
  template <typename T>
  void ConcurrentBoundedQueue<T>::abort()
  {
    _cond.notify_all();
  }
  //-----------------------------------------------------------------------------
  //!
  //! \return bool : The current state of the queue. False if shutdown has been called
  //!
  template <typename T>
  bool ConcurrentBoundedQueue<T>::active() const
  {
    return _active;
  }
}
#endif //BIOGEARS_CONTAINER_CONCURRENTBOUNDEDQUEUE_TCI_H
