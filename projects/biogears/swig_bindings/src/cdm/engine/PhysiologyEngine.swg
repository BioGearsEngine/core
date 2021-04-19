
// Add necessary symbols to generated header
%{
#include <memory> 

#include <biogears/container/Tree.tci.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/cdm/CommonDataModel.h>
using namespace biogears;
using namespace CDM;
%}

%include "windows.i"
#if defined(SWIGCSHARP)
  %include "csharp/std_string.i"
  %include "csharp/std_vector.i"
  %include "csharp/std_except.i"
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



%import(module="CommonDataModelException") <biogears/cdm/CommonDataModel.h>
%import(module="Logger") <biogears/cdm/utils/Logger.h>

%nspace "biogears::PhysiologyEngine";

namespace biogears {

struct PhysiologyEngineException : public CommonDataModelException {
  PhysiologyEngineException(const char* _Message)
    : CommonDataModelException(_Message)
  {
    
  }
};

class PhysiologyEngine {
  public:
  virtual ~PhysiologyEngine() {}; 
  virtual bool InitializeEngine(const char* patientFile) = 0;
  virtual void AdvanceModelTime(bool appendDataTrack = false) = 0;

};
} // namespace biogears
