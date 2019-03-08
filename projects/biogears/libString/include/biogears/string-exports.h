/*****************************************  *********************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/


#pragma once

#if defined(__clang__)
  #define BIOGEARS_STRING_API
  #define BG_EXT
#elif defined(__gnu_linux__)
  #define BIOGEARS_STRING_API __attribute__ ((visibility ("default")))
  #define BG_EXT extern
#elif defined(_WIN32)
  #ifdef biogears_string_EXPORTS
    #define BIOGEARS_STRING_API __declspec(dllexport)
  #else
    #define BIOGEARS_STRING_API __declspec(dllimport)
  #endif
  #include<string>
  #if _MSC_VER >= 1600 &&  _MSC_VER < 1910 
    #ifndef MSVC_2015_STRING_HACK
      //#define MSVC_2015_STRING_HACK
      //const std::basic_string<char>::size_type std::basic_string<char>::npos = static_cast<std::basic_string<char>::size_type>(-1);
    #endif
  #endif //_MSC_VER
  //#pragma warning(push,0)
    //template class BIOGEARS_STRING_API std::allocator<char>;
    //template class BIOGEARS_STRING_API std::_String_alloc<std::_String_base_types<char, std::allocator<char>>>;
    //template class BIOGEARS_STRING_API std::basic_string<char>;
    //#pragma warning(pop)  
#else 
  #define BIOGEARS_STRING_API
  #define BG_EXT
#endif
