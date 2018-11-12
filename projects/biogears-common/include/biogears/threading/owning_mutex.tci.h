#ifndef BIOGEARS_THREADING_OWNING_MUTEX_TCI_H
#define BIOGEARS_THREADING_OWNING_MUTEX_TCI_H

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
//! \author Alex Pronschinske
//! \date   2017 May 03
//!

#include <biogears/threading/owning_mutex.h>

namespace biogears {

  //============================================================================
  // OwningMutex definitions
  //----------------------------------------------------------------------------
  template<typename T>
  OwningMutex<T>::OwningMutex(T&& item) : _item(std::move(item)),_on_loan(false) {}

  //----------------------------------------------------------------------------
  template<typename T>
  OwningMutex<T>::OwningMutex(const T& item) : _item(item),_on_loan(false) {}

  //----------------------------------------------------------------------------
  template<typename T>
  template <typename ...Args>
  OwningMutex<T>::OwningMutex(InPlace, Args&& ...args)
    : _mutex()
    , _on_loan(false)
    , _item(std::forward<Args>(args)...)
  {}

  //----------------------------------------------------------------------------
  template<typename T>
  template <typename ...Args>
  OwningMutex<T> OwningMutex<T>::make(Args&& ...args) {
    return OwningMutex<T>{InPlace::Unit, std::forward<Args>(args)...};
  }
  //----------------------------------------------------------------------------
  template<typename T>
  OwningMutex<T>::OwningMutex(OwningMutex<T>&& obj)
  :_item ( std::move(obj._item) )
  , _mutex()
  , _on_loan(obj._on_loan)
  {
    assert(!obj._on_loan);
  }

  //----------------------------------------------------------------------------
  template<typename T>
  OwningMutex<T>&  OwningMutex<T>::operator=(OwningMutex<T>&& rhs)
  { 
    assert ( !rhs._on_loan );
    _item = std::move(rhs._item);
  }
  //----------------------------------------------------------------------------
  template<typename T>
  Guard<T> OwningMutex<T>::acquire() const {
    _mutex.lock();
    _on_loan = true;
    return Guard<T>(this);
  }

  //----------------------------------------------------------------------------
  template<typename T>
  Option<Guard<T>> OwningMutex<T>::try_acquire() const {
    return (_mutex.try_lock()) ? _on_loan = true,Option<Guard<T>>(Guard<T>(this))
                               : Option<Guard<T>>();
  }

  //============================================================================
  // Guard definitions
  //----------------------------------------------------------------------------
  template<typename T>
  Guard<T>::Guard(): _pmutex(nullptr) 
  {}

  //----------------------------------------------------------------------------
  template<typename T>
  Guard<T>::Guard(Guard<T>&& other) 
  { 
    *this = std::move(other); 
  }

  //----------------------------------------------------------------------------
  template<typename T>
  Guard<T>& Guard<T>::operator=(Guard<T>&& other) {
    if (this != &other) {
      _pmutex = other._pmutex;
      other._pmutex = nullptr;
    }
    return *this;
  }

  //----------------------------------------------------------------------------
  template<typename T>
  Guard<T>::Guard(const OwningMutex<T>* pmutex) : _pmutex(pmutex) {}

  //----------------------------------------------------------------------------
  template<typename T>
  Guard<T>::~Guard() {
    if (_pmutex) { _pmutex->_on_loan = false;_pmutex->_mutex.unlock(); }
    _pmutex = nullptr;
  }

} // end namespace biogears

#endif //BIOGEARS_THREADING_OWNING_MUTEX_TCI_H
