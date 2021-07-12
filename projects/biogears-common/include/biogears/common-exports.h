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
//! \date Sept 7 2017
//! \copyright Copyright 2017 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file 
//!
//! \brief Shared library visibility header
//!  Defines two macros that control dll visibility
//!         BIOGEARS_COMMON_PUBLIC_API
//!         BIOGEARS_COMMON_PRIVATE_API
//!


#if defined(BIOGEARS_COMMON_BUILD_STATIC)
  #define BIOGEARS_COMMON_PUBLIC_API
  #define BIOGEARS_COMMON_PRIVATE_API
#else
  #if defined _WIN32 || defined __CYGWIN__
    #ifdef biogears_common_EXPORTS
      #define BIOGEARS_COMMON_PUBLIC_API __declspec(dllexport)
      #define BIOGEARS_COMMON_PRIVATE_API
    #else
      #define BIOGEARS_COMMON_PUBLIC_API __declspec(dllimport)
      #define BIOGEARS_COMMON_PRIVATE_API
    #endif
  #else
    #if __GNUC__ >= 4
      #define BIOGEARS_COMMON_PUBLIC_API __attribute__ ((visibility ("default")))
      #define BIOGEARS_COMMON_PRIVATE_API  __attribute__ ((visibility ("hidden")))
    #else
      #define BIOGEARS_COMMON_PUBLIC_API
      #define BIOGEARS_COMMON_PRIVATE_API
    #endif
  #endif
#endif

