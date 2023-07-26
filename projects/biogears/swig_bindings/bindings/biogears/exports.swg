%{
   #include <stdexcept>
   #include <string>
   #include <vector>
   #include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) exports
#endif

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
#endif

#if defined(SWIGPYTHON)
  %include std_container.i 
  %include std_map.i
  %include std_vector.i
  %import "python/std_string.i"
  %import "python/std_vector.i"
  %import "python/std_map.i"
  %import "python/std_except.i"
#endif 

%import"exception.i"

%import <biogears/common-exports.h>
%import <biogears/cdm-exports.h>
%import <biogears/exports.h>

%nspace std::vector<std::string>;
%template (vector_string) std::vector<std::string>;
