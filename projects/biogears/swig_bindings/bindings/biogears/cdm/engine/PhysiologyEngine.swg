
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

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>

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
