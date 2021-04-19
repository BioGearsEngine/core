
// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarElectricInductance.h>
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
%import (module="SEScalarQuantity") <biogears/cdm/properties/SEScalarQuantity.h>

%nspace biogears::SEScalarElectricInductance;
%nspace biogears::ElectricInductanceUnit;

%ignore Unload;
%ignore Load; 

%rename(Equals)  operator==(const ElectricInductanceUnit&) const;
%rename(NotEquals)  operator!=(const ElectricInductanceUnit&) const;
%rename(Equals)  operator==(const SEScalarSelectricInductance&) const;
%rename(NotEquals)  operator!=(const SEScalarSelectricInductance&) const;

%include <biogears/cdm/properties/SEScalarElectricInductance.h>


