#ifndef BIOGEARS_THREADING_RUNNABLE_H
#define BIOGEARS_THREADING_RUNNABLE_H

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
//! \brief Abstract Interface for Runnable object
//!        which manages its own thread but can be controlled externally
//!        to be driven by an execution engine which listens to external control signals
//!
namespace biogears {

  class Runnable {
  public:
    virtual ~Runnable() {};

    virtual void run()   = 0; //< Starts Executing Runnable until Runnable Completes
    virtual void stop()  = 0; //< Sets Stop condition so running process will finish at the next possible moment;
    virtual void join()  = 0; //< Blocks until running process is completed;
  };
}

#endif //BIOGEARS_THREADING_RUNNABLE_H