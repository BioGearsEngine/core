#ifndef BIOGEARS_FRAMEWORK_BILLBOARD_TCI_H
#define BIOGEARS_FRAMEWORK_BILLBOARD_TCI_H

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
//! \file   
//! \author Alex Pronschinske
//! \date   2017
//! \modified May 11th 2016
//! \brief  Thread Safe Shared Memory for a Pub/Sub File
//!

#include <biogears/framework/billboard.h>

namespace biogears {

//==============================================================================
// Billboard declaration
//------------------------------------------------------------------------------
///
/// The billboard value shared between the Publisher and all Viewers
///
/// **This class for internal use only.**
///
  template<class T>
  class Billboard
  {
  public:
    Billboard() = delete;
    
    template<typename Fundamental = T>
    Billboard(const Fundamental in_left, 
              const Fundamental in_right,
              typename  std::enable_if<std::is_fundamental<Fundamental>::value>::type * = nullptr);
    
    template<typename Compound = T>
    Billboard(Compound&& in_left, 
		          Compound&& in_right, 
		          typename  std::enable_if<std::is_compound<Compound>::value>::type* = nullptr);

    ~Billboard() = default;

    void flip();

    // Data members
		typename std::remove_reference<T>::type left;
		typename std::remove_reference<T>::type right;
    std::atomic<bool> left_active;
  };

//==============================================================================
// Beacon definitions
//------------------------------------------------------------------------------
  template<class T>
	template<typename Fundamental>
	Billboard<T>::Billboard(const Fundamental in_left,
		const Fundamental in_right,
		typename  std::enable_if<std::is_fundamental<Fundamental>::value>::type * h) 
		: left(in_left), right(in_right), left_active(true) {
  }

//------------------------------------------------------------------------------
  template<class T>
	template<typename Compound>
	Billboard<T>::Billboard(Compound&& in_left,
		Compound&& in_right,
		typename  std::enable_if<std::is_compound<Compound>::value>::type* )
	: left(std::move(in_left)), right(std::move(in_right))
    , left_active(true) {
  }

//------------------------------------------------------------------------------
  template<class T>
  void Billboard<T>::flip() {
    left_active.store(!left_active.load());
  }

//==============================================================================
// Viewer definitions
//------------------------------------------------------------------------------
  template<class T>
  Viewer<T>::Viewer(std::shared_ptr<Billboard<T> > billboard)
    : _billboard(billboard)
  {}

//------------------------------------------------------------------------------
  template<class T>
  Viewer<T>::Viewer(const Viewer<T>& other) {
    *this = other;
  }

//------------------------------------------------------------------------------
  template<class T>
  Viewer<T>& Viewer<T>::operator =(const Viewer<T>& other) {
    if (this != &other) {
      _billboard = other._billboard;
    }
    return *this;
  }

//------------------------------------------------------------------------------
  template<class T>
  Viewer<T>::Viewer(Viewer<T>&& other) {
    *this = std::move(other);
  }

//------------------------------------------------------------------------------
  template<class T>
  Viewer<T>& Viewer<T>::operator =(Viewer<T>&& other) {
    if (this != &other) {
      _billboard = std::move(other._billboard);
    }
    return *this;
  }

//------------------------------------------------------------------------------
  template<class T>
  Viewer<T> Viewer<T>::clone() const {
    return Viewer<T>(_billboard);
  }

//------------------------------------------------------------------------------
  template<class T>
  T Viewer<T>::get() const {
    assert(_billboard.get() != nullptr);
    return _billboard->left_active.load() ? _billboard->left : _billboard->right;
  }

//==============================================================================
// Publisher definitions
//------------------------------------------------------------------------------
  template<class T>
  Publisher<T>::Publisher(std::shared_ptr<Billboard<T>> billboard)
    : _billboard(billboard)
  {}

//------------------------------------------------------------------------------
  template<class T>
  Publisher<T>::Publisher(Publisher<T>&& other) {
    *this = std::move(other);
  }

//------------------------------------------------------------------------------
  template<class T>
  Publisher<T>& Publisher<T>::operator =(Publisher<T>&& other) {
    if (this != &other) {
      _billboard = std::move(other._billboard);
    }
    return *this;
  }

//------------------------------------------------------------------------------
  template<class T>
  template<typename... Dummy, typename Compound>
  typename std::enable_if< !std::is_trivial<Compound>::value >::type
    Publisher<T>::put(Compound&& new_val) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments!");
    assert(_billboard.get() != nullptr);
    auto curr_is_left_active = _billboard->left_active.load();
    if (curr_is_left_active) {
      _billboard->right = std::move(new_val);
    } else   {
      _billboard->left = std::move(new_val);
    }
    _billboard->left_active.store( !curr_is_left_active );
  }
  //------------------------------------------------------------------------------
  template<class T>
  template<typename... Dummy, typename Fundamental>
  typename std::enable_if<std::is_trivial<Fundamental>::value>::type
    Publisher<T>::put(const Fundamental& new_val) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments!");
    assert(_billboard.get() != nullptr);
    auto curr_is_left_active = _billboard->left_active.load();
    if (curr_is_left_active) {
      _billboard->right = Fundamental(new_val);
    }
    else {
      _billboard->left = Fundamental(new_val);
    }
    _billboard->left_active.store(!curr_is_left_active);
  }
//------------------------------------------------------------------------------
  template<class T>
  T Publisher<T>::get() const {
    assert(_billboard.get() != nullptr);
    return _billboard->left_active.load() ? _billboard->left : _billboard->right;
  }

//------------------------------------------------------------------------------
  template<class T>
  Viewer<T> Publisher<T>::make_viewer() const {
    return Viewer<T>(_billboard);
  }

//==============================================================================
// make_billboard definition
//------------------------------------------------------------------------------
  template <class T>
  BillboardPair<T> make_billboard(T&& init_val) {
    using std::move;
    using std::make_shared;
    using std::make_pair;

    T copy_init_val(init_val);
    auto p_billboard = make_shared<Billboard<T>>(move(init_val), move(copy_init_val));
    auto new_publisher = Publisher<T>(p_billboard);
    auto new_viewer = Viewer<T>(p_billboard);

    return { move(new_publisher), move(new_viewer) };
  }

	template <class T>
	BillboardPair<T> make_billboard(const T& init_val) {
    return make_billboard<T>( std::move(T(init_val)) );
  }

} // end namespace biogears

#endif
