%{
   #include <stdexcept>
   #include <string>
   #include <vector>
%}

%import "windows.i"
#if defined(SWIGCSHARP)
  %import "csharp/std_string.i"
  %import "csharp/std_vector.i"
  %import "csharp/std_map.i"
  %import "csharp/std_except.i"

namespace std 
{
  %ignore runtime_error;
  struct runtime_error {};
}

#elif defind(SWIGPYTHON)
  %import "python/std_string.i"
  %import "python/std_vector.i"
  %import "python/std_map.i"
  %import "python/std_except.i"
#else
  %import "std_string.i"
  %import "std_vector.i"
  %import "std_map.i"
  %import "std_except.i"
#endif 
%import"exception.i"

%import <biogears/common-exports.h>
%import <biogears/cdm-exports.h>
%import <biogears/exports.h>

%nspace std::vector<std::string>;
%template (vector_string) std::vector<std::string>;
