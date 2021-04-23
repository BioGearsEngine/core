%{
#include <biogears/container/Tree.tci.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
%}

%ignore   biogears::PhysiologyEngine::SaveState(const std::string& file = "");
%ignore   biogears::PhysiologyEngine::LoadState(const std::string& file, const SEScalarTime* simTime = nullptr);

%import <biogears/cdm/engine/PhysiologyEngine.swg>

%nspace "biogears::BioGearsEngine"

namespace biogears {
%nodefaultctor BioGearsEngine;
class BioGearsEngine : public PhysiologyEngine{
public:
  
  BioGearsEngine(Logger* logger);
  BioGearsEngine(const char* logFileName);
  bool InitializeEngine(const char* patientFile);
  virtual void AdvanceModelTime(bool appendDataTrack = false) override;
  virtual void AdvanceModelTime(double time, const TimeUnit& unit  = TimeUnit::s, bool appendDataTrack = false) override;
  virtual bool GetPatientAssessment(SEPatientAssessment& assessment) override;
  virtual bool ProcessAction(const SEAction& action) override;
};
}
