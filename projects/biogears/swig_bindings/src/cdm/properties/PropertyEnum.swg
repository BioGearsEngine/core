
// Add necessary symbols to generated header
%{
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/properties/PropertyEnum.h>
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

%import (module="CommonDataModel") <biogears/cdm/CommonDataModel.h>

%nspace biogears::SESide;
%nspace biogears::SEOpenClosed;
%nspace biogears::SEOnOff;
%nspace biogears::SEErrorType;

%rename(SESideToString) operator<<(std::ostream& os, const SESide& e);
%rename(SEOpenClosedToString) operator<<(std::ostream& os, const SEOpenClosed& e);
%rename(SEOnOffToString) operator<<(std::ostream& os, const SEOnOff& e);
%rename(SEErrorTypeToString) operator<<(std::ostream& os, const SEErrorType& e);

%include <biogears/cdm/properties/PropertyEnum.h>

