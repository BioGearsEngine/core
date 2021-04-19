

// Add necessary symbols to generated header
%{
#include <biogears/container/Tree.tci.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
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


%import(module="Logger") <biogears/cdm/utils/Logger.h>
%import(module="PhysiologyEngine") <biogears/cdm/engine/PhysiologyEngine.h>

%nspace "biogears::BioGearsEngine"

namespace biogears {
%nodefaultctor BioGearsEngine;
class BioGearsEngine {
public:
  
  BioGearsEngine(Logger* logger);
  BioGearsEngine(const char* logFileName);
  bool InitializeEngine(const char* patientFile);
  virtual void AdvanceModelTime(bool appendDataTrack = false) override;

};
}
