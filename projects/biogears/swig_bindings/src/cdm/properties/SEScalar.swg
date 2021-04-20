
// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalar.h>
using namespace biogears;
using namespace CDM;
%}

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

%include <biogears/exports.h>

%import (module="Loggable") <biogears/cdm/utils/Logger.h>
%import (module="CommonDataModel") <biogears/cdm/CommonDataModel.h>
%import (module="Property") <biogears/cdm/properties/SEProperty.h>

%nspace biogears::SEScalar;
%nspace biogears::NoUnit;

%rename (LessThan)  operator<;
%rename (LessThanEqual)  operator<=;
%rename (GreaterThan)  operator>;
%rename (GreaterThanEqual)  operator>=;
 
%ignore  biogears::SEScalar::operator==;
%ignore  biogears::SEScalar::operator!=;

%ignore  biogears::SEScalar::operator+;
%ignore  biogears::SEScalar::operator+=;
%ignore  biogears::SEScalar::operator-;
%ignore  biogears::SEScalar::operator-=;
%ignore  biogears::SEScalar::operator/;
%ignore  biogears::SEScalar::operator/=;
%ignore  biogears::SEScalar::operator*;
%ignore  biogears::SEScalar::operator*=;

%ignore  biogears::operator==;
%ignore  biogears::operator!=;

%ignore  biogears::operator+;
%ignore  biogears::operator+=;
%ignore  biogears::operator-;
%ignore  biogears::operator-=;
%ignore  biogears::operator/;
%ignore  biogears::operator/=;
%ignore  biogears::operator*;
%ignore  biogears::operator*=;

%ignore  biogears::operator<<;

%ignore  biogears::SEScalar::ToString;
%ignore  biogears::SEScalar::operator>>;
%ignore  biogears::SEScalar::operator<<;

%ignore biogears::SEScalar::Load;
%ignore biogears::SEScalar::Unload;


%ignore unitless;

%include <biogears/cdm/properties/SEScalar.h>


