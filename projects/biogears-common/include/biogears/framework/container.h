#ifndef BIOGEARS_FRAMEWORK_CONTAINER_H
#define BIOGEARS_FRAMEWORK_CONTAINER_H

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
//! \date   Sept 15 2017
//! \copyright Copyright 2017 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file 
//!
//!  Abstract interface for all non-concurrent containers in this
//!  package
namespace biogears {

  //! \brief Common Interface for containers to be
  //!        passed to the Source and Channel Templates
  template <typename T>
  class container {
  public:
    virtual ~container() = default;

    virtual insert( typename T::value_type&  ) = 0;     //<! Insert an item by copy. Value is non const could be used to return an overflow value
    virtual bool insert( typename T::value_type&& )= 0; //<! Insert an item by copy. Value is non const could be used to return an overflow value
    virtual void consume ( typename T::value_type&  ) = 0;  //<! Insert consumes an element of the container and assigns it to the passed value (Could be used to delete a specific element)
    virtual auto consume() -> typename T::value_type = 0 ;  //<! Consumes an element of the container and returns it by value

    virtual void peek ( const typename T::value_type&  ) const = 0;  //<! Sets the value of the next item to be consumed to the address passed
    virtual auto peek() -> const typename T::value_type&  const = 0; //<! Returns teh value of the next item to be consumed by value

    virtual size_t size() const = 0; //!< Some metric of the containers current capacity

    virtual void abort() = 0;    //< Signals all source to abort any pending insets
  };
}

#endif //BIOGEARS_FRAMEWORK_CONTAINER_H