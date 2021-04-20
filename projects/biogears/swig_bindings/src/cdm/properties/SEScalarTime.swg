
// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarTime.h>
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
%import (module="SEProperty") <biogears/cdm/properties/SEProperty.h>
%import (module="SEScalar") <biogears/cdm/properties/SEScalar.h>
%import (module="SEUnitScalar") <biogears/cdm/properties/SEUnitScalar.h>
%import (module="CComoundUnit") <biogears/cdm/utils/unitconversion/CompoundUnit.h>
%import (module="SEScalarQuantity") <biogears/cdm/properties/SEScalarQuantity.h>

%nspace biogears::SEScalarTime;
%nspace biogears::TimeUnit;

%ignore Unload;
%ignore Load; 

%ignore operator<;
%ignore operator<=;
%ignore operator>;
%ignore operator>=;

%ignore operator-;
%ignore operator-=;
%ignore operator+;
%ignore operator+=;
%ignore operator*;
%ignore operator*=;
%ignore operator/;
%ignore operator/=;

%ignore operator<<;

%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<TimeUnit>::ToString;

%rename(Equals)  operator==(const TimeUnit&) const;
%rename(NotEquals)  operator!=(const TimeUnit&) const;
%rename(Equals)  operator==(const SEScalarTime&) const;
%rename(NotEquals)  operator!=(const SEScalarTime&) const; 
 
%nspace biogears::SEScalarQuantity<TimeUnit>;


namespace biogears {
   class TimeUnit;
}
  %template(QuantityTime) biogears::SEScalarQuantity<TimeUnit>;

%include <biogears/cdm/properties/SEScalarTime.h>




