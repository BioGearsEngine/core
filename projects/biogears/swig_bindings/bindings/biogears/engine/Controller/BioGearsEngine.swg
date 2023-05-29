%{
#include <biogears/container/Tree.tci.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/schema/biogears/BioGears.hxx>

#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/config.h>

%ignore   BIOGEARS_NAMESPACE PhysiologyEngine::GetStateData();
%ignore   BIOGEARS_NAMESPACE PhysiologyEngine::SaveStateToFile(const std::string& file = "");
%ignore   BIOGEARS_NAMESPACE PhysiologyEngine::LoadState(const std::string& file, const SEScalarTime* simTime = nullptr);

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/patient/SEPatient.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>
%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>
%import <biogears/cdm/system/environment/SEEnvironment.swg>
%import <biogears/cdm/system/physiology/SEBloodChemistrySystem.swg>
%import <biogears/cdm/system/physiology/SECardiovascularSystem.swg>
%import <biogears/cdm/system/physiology/SEDrugSystem.swg>
%import <biogears/cdm/system/physiology/SEEndocrineSystem.swg>
%import <biogears/cdm/system/physiology/SEEnergySystem.swg>
%import <biogears/cdm/system/physiology/SEGastrointestinalSystem.swg>
%import <biogears/cdm/system/physiology/SEHepaticSystem.swg>
%import <biogears/cdm/system/physiology/SENervousSystem.swg>
%import <biogears/cdm/system/physiology/SERenalSystem.swg>
%import <biogears/cdm/system/physiology/SERespiratorySystem.swg>
%import <biogears/cdm/system/physiology/SETissueSystem.swg>
%import <biogears/cdm/engine/PhysiologyEngineTrack.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace "BIOGEARS_NAMESPACE BioGearsEngine"
%nspace "BIOGEARS_NAMESPACE Logger"

%ignore BIOGEARS_NAMESPACE BioGearsEngine::LoadState(const std::string& file, const SEScalarTime* simTime = nullptr);
%ignore BIOGEARS_NAMESPACE BioGearsEngine::LoadState(const CDM::PhysiologyEngineStateData& state, const SEScalarTime* simTime = nullptr);
%ignore BIOGEARS_NAMESPACE BioGearsEngine::GetStateData();
%ignore BIOGEARS_NAMESPACE BioGearsEngine::SaveStateToFile(const std::string& file);
%ignore BIOGEARS_NAMESPACE BioGearsEngine::InitializeEngine(const char* patientFile, const std::vector<const SECondition*>* conditions);
%ignore BIOGEARS_NAMESPACE BioGearsEngine::InitializeEngine(const char* patientFile, const std::vector<const SECondition*>* conditions , const PhysiologyEngineConfiguration* config );
%ignore BIOGEARS_NAMESPACE BioGearsEngine::InitializeEngine(const std::string& patientFile, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr);
%ignore BIOGEARS_NAMESPACE BioGearsEngine::InitializeEngine(const SEPatient& patient, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr);

%ignore BIOGEARS_NAMESPACE BioGearsEngine::GetConfiguration();
%ignore BIOGEARS_NAMESPACE BioGearsEngine::GetAnesthesiaMachine();
%ignore BIOGEARS_NAMESPACE BioGearsEngine::GetElectroCardioGram();
%ignore BIOGEARS_NAMESPACE BioGearsEngine::GetInhaler();
%ignore BIOGEARS_NAMESPACE BioGearsEngine::GetCompartments();

%ignore BIOGEARS_NAMESPACE BioGearsEngine::BioGearsEngine(const std::string& logFileName);
%ignore BIOGEARS_NAMESPACE BioGearsEngine::BioGearsEngine(Logger* logger, const std::string& working_dir);
%ignore BIOGEARS_NAMESPACE BioGearsEngine::BioGearsEngine(const std::string&, const std::string& working_dir);

%rename (AdvanceModelTimeBy) AdvanceModelTime(double time, const TimeUnit& unit  = TimeUnit::s, bool appendDataTrack = false);

%warnfilter(401) BIOGEARS_NAMESPACE BioGearsEngine;

#if defined(SWIGCSHARP)
  %ignore create_logger(const char* logfile);
  %ignore destroy_logger(Logger** engine);
  %ignore create_biogears_engine(BIOGEARS_NAMESPACE Logger* logger, const char* working_dir);
  %ignore create_biogears_engine(const char* logger, const char* working_dir);
  %ignore destroy_biogears_engine(BioGearsEngine** engine);
#endif 
 
%include <biogears/engine/controller/BioGearsEngine.h>
