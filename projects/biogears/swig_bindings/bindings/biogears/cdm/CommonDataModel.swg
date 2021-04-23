%import "windows.i"
#if defined(SWIGCSHARP)
  %import "csharp/std_string.i"
  %import "csharp/std_vector.i"
  %import "csharp/std_except.i"

namespace std 
{
  %ignore runtime_error;
  struct runtime_error {};
}

#elif defind(SWIGPYTHON)
  %import "python/std_string.i"
  %import "python/std_vector.i"
  %import "python/std_except.i"
#else
  %import "std_string.i"
  %import "std_vector.i"
  %import "std_except.i"
#endif 
%import"exception.i"

// Add necessary symbols to generated header
%{
#include <exception>
#include<biogears/cdm/CommonDataModel.h>
%}

%include <biogears/exports.h>

%nspace "biogears::CommonDataModelException"

%ignore biogears::CommonDataModelException::CommonDataModelException(std::string const &);

%ignore Unload;
%ignore Load; 
%ignore ToString( std::ostream& ); 
%ignore ToString(std::ostream& str) const;

%ignore  operator+;
%ignore  operator+=;
%ignore  operator-;
%ignore  operator-=;
%ignore  operator/;
%ignore  operator/=;
%ignore  operator*;
%ignore  operator*=;

%ignore operator<;
%ignore operator<=;
%ignore operator>;
%ignore operator>=;
%ignore  operator==;
%ignore  operator!=;

%ignore  operator<<;
%ignore  operator>>;

%nspace std::vector<double>;
%template(vector_d) std::vector<double>;

%include <biogears/cdm/CommonDataModel.h>
