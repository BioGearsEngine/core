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

//namespace biogears {
//%nodefaultctor BioGearsEngine;
//class BioGearsEngine : public PhysiologyEngine{
//public:
//  
//  BioGearsEngine(Logger* logger);
//  BioGearsEngine(const char* logFileName);
//  bool InitializeEngine(const char* patientFile);
//  virtual void AdvanceModelTime(bool appendDataTrack = false) override;
//  virtual void AdvanceModelTime(double time, const TimeUnit& unit  = TimeUnit::s, bool appendDataTrack = false) override;
//  virtual bool GetPatientAssessment(SEPatientAssessment& assessment) override;
//  virtual bool ProcessAction(const SEAction& action) override;
//};
//}


%ignore biogears::BioGearsEngine::LoadState(const std::string& file, const SEScalarTime* simTime = nullptr);
%ignore biogears::BioGearsEngine::LoadState(const CDM::PhysiologyEngineStateData& state, const SEScalarTime* simTime = nullptr);
%ignore biogears::BioGearsEngine::SaveState;
%ignore biogears::BioGearsEngine::InitializeEngine(const char* patientFile, const std::vector<const SECondition*>* conditions);
%ignore biogears::BioGearsEngine::InitializeEngine(const char* patientFile, const std::vector<const SECondition*>* conditions , const PhysiologyEngineConfiguration* config );
%ignore biogears::BioGearsEngine::InitializeEngine(const std::string& patientFile, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr);
%ignore biogears::BioGearsEngine::InitializeEngine(const SEPatient& patient, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr);
%ignore biogears::BioGearsEngine::GetEngineTrack();
%ignore biogears::BioGearsEngine::GetConfiguration();
%ignore biogears::BioGearsEngine::GetAnesthesiaMachine();
%ignore biogears::BioGearsEngine::GetElectroCardioGram();
%ignore biogears::BioGearsEngine::GetInhaler();
%ignore biogears::BioGearsEngine::GetCompartments();

%ignore biogears::BioGearsEngine::BioGearsEngine(const std::string& logFileName);
%ignore biogears::BioGearsEngine::BioGearsEngine(Logger* logger, const std::string& working_dir);
%ignore biogears::BioGearsEngine::BioGearsEngine(const std::string&, const std::string& working_dir);

%rename (AdvanceModelTimeBy) AdvanceModelTime(double time, const TimeUnit& unit  = TimeUnit::s, bool appendDataTrack = false);
%ignore SetEventHandler(SEEventHandler* handler);

%warnfilter(401) biogears::BioGearsEngine;
%include <biogears/engine/controller/BioGearsEngine.h>
