#ifndef BIOGEARS_FRAMEWORK_CONCURRENT_CONTAINER_H
#define BIOGEARS_FRAMEWORK_CONCURRENT_CONTAINER_H

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
//!  Abstract interface for all concurrent containers in this
//!  package

namespace biogears {
  //! \brief Common Interface for containers to be
  //!        passed to the Sources and Channels Templates
  template <typename T>
  class concurrent_container {
  public:
    virtual ~concurrent_container() = default;

    virtual bool insert(T  ) = 0;     //<! Insert an item by copy you can manually move in to this copy if you like.
    
    virtual void consume (  T&  ) = 0;  //<! Consumes an element of the container and assigns it to the passed value (Could be used to delete a specific element)
    virtual auto consume() ->  T = 0 ;  //<! Consumes an element of the container and returns it by value

    virtual size_t unsafe_size() const = 0; //!< Some metric of the containers current capacity

    virtual void abort() = 0;    //< Signals all source to abort any pending insets
    virtual void shutdown() = 0; //< Sets the queue to inoperable and calls abort
    virtual bool active() const = 0;   //< returns if the underlying channel is still active
  };
}

#endif  //BIOGEARS_FRAMEWORK_CONCURRENT_CONTAINER_H
