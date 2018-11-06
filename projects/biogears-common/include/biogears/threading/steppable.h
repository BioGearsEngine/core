#ifndef BIOGEARS_THREADING_STEPPABLE_H
#define BIOGEARS_THREADING_STEPPABLE_H

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
//! \date   Sept 22 2017
//! \copyright Copyright 2017 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file 
//!
//! \brief Abstract Interface for Steppable object
//!        which has a primary work function that can be controlled
//!        by a thread pool or some other execution engine
//!
namespace biogears {

	template<typename Signiture>
  class Steppable {
  public:
    virtual ~Steppable() {};

    virtual void step()  = 0;  //< Executes a single pass of the logic loop;
    virtual std::function<Signiture> step_as_func() = 0; //< Returns a std::bind of the instance to be called multiple times ;
  };
}

#endif //BIOGEARS_THREADING_STEPPABLE_H