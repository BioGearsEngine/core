

%{
#include <memory> 

#include <biogears/container/Tree.tci.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/cdm/CommonDataModel.h>
USING_BIOGEARS_NAMESPACE;
using namespace CDM;
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/container/Tree.swg>

%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEGenericScalar.swg>

%import <biogears/cdm/scenario/SEAction.swg>

%import <biogears/cdm/patient/assessments/SEPatientAssessment.swg>

%import <biogears/cdm/patient/SEPatient.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>
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
%import <biogears/cdm/utils/SEEventHandler.swg>
%import <biogears/cdm/engine/PhysiologyEngineTrack.h>

%nspace "BIOGEARS_NAMESPACE PhysiologyEngine";
%nspace "BIOGEARS_NAMESPACE PhysiologyEngineTrack";

%ignore BIOGEARS_NAMESPACE PhysiologyEngineException::PhysiologyEngineException(const std::string& _Message);

%ignore BIOGEARS_NAMESPACE PhysiologyEngine::LoadState(const std::string& file, const SEScalarTime* simTime = nullptr);
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::LoadState(const CDM::PhysiologyEngineStateData& state, const SEScalarTime* simTime = nullptr);
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::GetStateData();
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::SaveStateToFile(const std::string& file);
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::InitializeEngine(const char* patientFile, const std::vector<const SECondition*>* conditions);
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::InitializeEngine(const char* patientFile, const std::vector<const SECondition*>* conditions , const PhysiologyEngineConfiguration* config );
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::InitializeEngine(const std::string& patientFile, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr);
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::InitializeEngine(const SEPatient& patient, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr);
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::GetConfiguration();
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::GetAnesthesiaMachine();
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::GetElectroCardioGram();
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::GetInhaler();
%ignore BIOGEARS_NAMESPACE PhysiologyEngine::GetCompartments();

%rename (AdvanceModelTimeBy) AdvanceModelTime(double time, const TimeUnit& unit  = TimeUnit::s, bool appendDataTrack = false);

%include <biogears/cdm/engine/PhysiologyEngine.h>
