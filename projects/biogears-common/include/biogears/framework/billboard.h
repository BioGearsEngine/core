#ifndef BIOGEARS_FRAMEWORK_BILLBOARD_H
#define BIOGEARS_FRAMEWORK_BILLBOARD_H

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
//! Container for a Pub/Sub Model which allows MultiConsume
//! Single Publisher Data Transfer
//!

#include <atomic>
#include <memory>
#include <type_traits>

namespace biogears {

  // Forward declaration
  template<class T>
  class Billboard;

//------------------------------------------------------------------------------
///
/// Viewer half of the billboard design
///
/// This class supports both copy and move, but it is preferable to use copy
/// semantics because this class is a thin wrapper around a `shared_ptr`.
///
  template<class T>
  class Viewer
  {
  public:
    // Complete explicit constructor
    Viewer(std::shared_ptr<Billboard<T>> billboard);

    // Copy construction and assignment
    Viewer(const Viewer<T>& other);
    Viewer<T>& operator =(const Viewer<T>& other);

    // Move construction and assignment
    Viewer(Viewer<T>&& other);
    Viewer<T>& operator =(Viewer<T>&& other);

    ///
    /// provided for some algorithms in BIOGEARS_core
    ///
    Viewer<T> clone() const;

    ~Viewer() = default;

    ///
    /// Get a copy of the billboard's current value.  
    /// Asserts in debug if uninitialized undefined in release
    /// 
    T get() const;

  private:
    // Default construction is essentially a null pointer, DO NOT USE.
    Viewer() = default;

    std::shared_ptr<Billboard<T>> _billboard = nullptr;
  };

//------------------------------------------------------------------------------
///
/// Publisher half of the billboard design
///
/// **This class is move-only.**  The billboard value can be updated via the `put`
/// method.
///
  template<class T>
  class Publisher
  {
  public:
    // Complete explicit constructor
    Publisher(std::shared_ptr<Billboard<T>> billboard);

    ~Publisher() = default;

    // Copy constructor and copy assignment operator are **deleted**
    Publisher(const Publisher<T>&) = delete;
    Publisher<T>& operator =(const Publisher<T>&) = delete;

    // Move construction and assignment
    Publisher(Publisher<T>&& other);
    Publisher<T>& operator =(Publisher<T>&& other);

    ///
    /// Change billboard value
    ///
    template<typename... Dummy, typename Compound = T>
    typename std::enable_if< !std::is_trivial<Compound>::value >::type
    put(Compound&& new_val);

    template<typename... Dummy, typename Fundamental = T>
    typename std::enable_if<std::is_trivial<Fundamental>::value>::type
    put(const Fundamental& new_val);
    ///
    /// Get a copy of the billboard's current value.  **This method will cause an
    /// error if it is called on an uninitialized (or "null") instance.**
    ///
    T get() const;

    //!
    //! Make a new Viewer
    //!
    Viewer<T> make_viewer() const;

  private:
    // Default construction is essentially a null pointer, DO NOT USE.
    Publisher() = default;

    std::shared_ptr<Billboard<T>> _billboard = nullptr;
  };

//------------------------------------------------------------------------------
///
/// Make a Publisher-Viewer pair
///
/// This is the intended way to create a billboard (owned by the Publisher
/// and all Viewers).  The billboard is a thread-safe, non-blocking, continuous
/// value (of type `T`).  The Publisher is a move-only object with unique
/// write privileges to the billboard value.  All Viewers are clone-preferred
/// objects with read-only access to the billboard value.  Because all Viewers
/// have read-access to the billboard value *and* their access must be atomic and
/// non-blocking, read access is provided by *copying* the current billboard value.
/// It is therefore advised that the user put large billboard types behind shared
/// pointers.
///
  template <typename T>
  struct BillboardPair {
    Publisher<T> publisher;
    Viewer<T> viewer;
  };

  template <class T>
  BillboardPair<T> make_billboard(T&& init_val);

  template <class T>
  BillboardPair<T> make_billboard(const T& init_val);

} // end namespace biogears

#endif
