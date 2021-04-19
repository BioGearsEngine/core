
// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
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
%import (module="SEFunction") <biogears/cdm/properties/SEFunction.h>
%import (module="SEFunction") <biogears/cdm/properties/SEFunction.h>
%import (module="SEScalarTime") <biogears/cdm/properties/SEScalarTime.h>
%import (module="SEScalarVolume") <biogears/cdm/properties/SEScalarVolume.h>

%ignore Unload;
%ignore Load; 

%nspace biogears::SEFunctionVolumeVsTime;
%rename(Equals)  operator==(const SEFunctionVolumeVsTime&) const;
%rename(NotEquals)  operator!=(const SEFunctionVolumeVsTime&) const;

%include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>


