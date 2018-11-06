#ifndef BIOGEARS_THREADING_THREAD_H
#define BIOGEARS_THREADING_THREAD_H

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
//! \author Steven A White
//! \date Sept 22 2017
//! \copyright Copyright 2017 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file 
//!
//! \brief Abstract Interface for Thread Classes to Implement
//!        This is a work in progress. I think this should be a real class
//!        that takes a Steppable or a void(void) and runs it with these features
//!        Simply an attempt to unify thread usage across projects
//!


namespace biogears {
  class Thread {
  public:
    virtual ~Thread() {};

    virtual void main()  = 0; //< Serial Execution of main logic;
    virtual void start() = 0; //< Launch main() in a self container thread;
    virtual void stop()  = 0; //< Signal main() to stop;
    virtual void suspend() = 0; //< signal main() to sleep until the next start call;
    virtual void join()  = 0; //< Block until main() has terminated execution;
  };
}

#endif //BIOGEARS_THREADING_THREAD_H