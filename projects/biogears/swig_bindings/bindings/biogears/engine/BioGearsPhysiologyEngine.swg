%{
#include <biogears/cdm/utils/Logger.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
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

%include <biogears/config.h>

OPEN_BIOGEARS_NAMESPACE
BIOGEARS_NAMESPACE CreateBioGearsEngine(const std::string working_dir, BIOGEARS_NAMESPACE Logger* logger);
CLOSE_BIOGEARS_NAMESPACE