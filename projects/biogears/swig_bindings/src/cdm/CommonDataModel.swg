
%include "windows.i"
#if defined(SWIGCSHARP)
  %include "csharp/std_string.i"
  %include "csharp/std_vector.i"
  %include "csharp/std_except.i"

  namespace std 
{
  %ignore runtime_error;
  struct runtime_error {};
}

#elif defind(SWIGPYTHON)
  %include "python/std_string.i"
  %include "python/std_vector.i"
  %include "python/std_except.i"
#else
  %include "std_string.i"
  %include "std_vector.i"
  %include "std_except.i"
#endif 
%include "exception.i"

// Add necessary symbols to generated header
%{
#include <exception>
#include<biogears/cdm/CommonDataModel.h>
%}

%include <biogears/exports.h>

%nspace "biogears::CommonDataModelException"

%include <biogears/cdm/CommonDataModel.h>
