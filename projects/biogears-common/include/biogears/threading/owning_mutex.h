#ifndef BIOGEARS_THREADING_OWNINGMUTEX_H
#define BIOGEARS_THREADING_OWNINGMUTEX_H

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
//! @author Alex Pronschinske
//! @date   2017 May 03
//!

#include <biogears/framework/option.tci.h>
#include <biogears/framework/shared_propagate_const.h>

#include <mutex>

namespace biogears {

  // Forward declarations
  template<typename T> class OwningMutex;
  template<typename T> class Guard;

  //----------------------------------------------------------------------------
  //!
  //! Mutex class that owns the data it locks
  //!
  //! @note THIS CLASS IS MOVE ONLY.  Much like unique_ptr, this class has no copy
  //!       constructor or copy assignment operator.  Furthermore, this class enforces
  //!       RAII by deleting the "default" constructor.
  //! @note DO NOT MOVE AN INSTANCE OF OWNINGMUTEX WHILE IT IS LOCKED.  The new mutex
  //!       will have stolen the owned item out from under the old mutex, this will
  //!       invalidate the active Guard.
  //!
  //! @brief To access the data call the acquire method, and then dereference the
  //!        Guard object that is return using the "->" operator.
  //!
  //!       Keep in mind the OwningMutex must outlive the Guards or they will reference
  //!       deleted memory. We could avoid that by IMPL shared_ptr to IMPL, but only
  //!       allowing a single copy at a time
  template<typename T>
  class OwningMutex {
  public:
    OwningMutex() = delete;
    OwningMutex(const OwningMutex<T>&) = delete;
    OwningMutex<T>& operator=(const OwningMutex<T>&) = delete;


    OwningMutex(T&& item);
    OwningMutex(const T& item);

    enum InPlace { Unit };

    template <typename ...Args>
    explicit OwningMutex(InPlace, Args&& ...args);

    template <typename ...Args>
    static OwningMutex<T> make(Args&& ...args);

    Guard<T> acquire() const;
    Option<Guard<T>> try_acquire() const;

    // Allowed only for make_shared
    //---------------------
    OwningMutex(OwningMutex<T>&&);
  protected:
    // Data members
    //-------------
    mutable std::mutex _mutex;
    mutable T _item;
    mutable bool _on_loan;
    // friends
    //--------
    friend Guard<T>;
  private:
    OwningMutex<T>& operator=(OwningMutex<T>&&);
  };

  //----------------------------------------------------------------------------
  template<typename T>
  class Guard {
  public:
    Guard();

    Guard(Guard<T>&& other);
    Guard<T>& operator=(Guard<T>&& other);

    ~Guard();

    T& get() const { return _pmutex->_item; }
    T& operator*() const { return _pmutex->_item; }
    T* operator->() const { return &_pmutex->_item; }

  protected:
    // private constructors
    //---------------------
    Guard(const OwningMutex<T>* pmutex);

    // Data members
    //-------------
    const OwningMutex<T>* _pmutex = nullptr;

    // friends
    //--------
    friend OwningMutex<T>;
  };

  template<typename T, typename ...Args>
  shared_propagate_const<OwningMutex<T>> make_shared_mutex(Args&& ...args) {
    return shared_propagate_const<OwningMutex<T>>( OwningMutex<T>::InPlace::Unit
                                          , std::forward<Args>(args)...
                                          );
  }

} // end namespace biogears

#endif // BIOGEARS_THREADING_OWNINGMUTEX_H
