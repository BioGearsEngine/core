/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/container/Tree.h>
#include <biogears/exports.h>
#include <biogears/schema/cdm/EngineState.hxx>

namespace biogears {
class SEEventHandler;
class SEPatient;
class SEPatientAssessment;
class SECondition;
class SEConditionManager;
class SEAction;
class SEActionManager;
class TimeUnit;
class SESubstanceManager;
class SEBloodChemistrySystem;
class SECardiovascularSystem;
class SEDrugSystem;
class SEEndocrineSystem;
class SEEnergySystem;
class SEGastrointestinalSystem;
class SEHepaticSystem;
class SENervousSystem;
class SERenalSystem;
class SERespiratorySystem;
class SETissueSystem;
class SEEnvironment;
class SEAnesthesiaMachine;
class SEInhaler;
class SEElectroCardioGram;
class SECompartmentManager;
class PhysiologyEngineTrack;
class PhysiologyEngineConfiguration;

/**
 * @brief
 * Base exception class that all CDM classes throw when an error occurs
 */
struct PhysiologyEngineException : public CommonDataModelException {
  PhysiologyEngineException()
    : CommonDataModelException("Physiology Engine Error")
  {
  }
  PhysiologyEngineException(char const* _Message)
    : CommonDataModelException(_Message)
  {
  }
  PhysiologyEngineException(std::string const& _Message)
    : CommonDataModelException(_Message)
  {
  }
};

class BIOGEARS_API PhysiologyEngine : public Loggable {
public:
  PhysiologyEngine(Logger const* log);
  PhysiologyEngine();

  virtual ~PhysiologyEngine();

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Reset engine and set it to the state in the provided file.
  //! You may provided a Simulation Time to be used if desired.
  //! It will be reflected in the GetSimulationTime method.
  //! Return value indicates engine was able to load provided state file.
  //! Engine will be in a cleared state if this method fails.
  //!-------------------------------------------------------------------------------------------------
  virtual auto LoadState(char const* file, SEScalarTime const* simTime = nullptr) -> bool = 0;
  virtual auto LoadState(std::string const& file, SEScalarTime const* simTime = nullptr) -> bool = 0;
  virtual auto LoadState(char const* buffer, size_t size) -> bool = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Reset engine and set it to the state in the provided file.
  //! You may provided a Simulation Time to be used if desired.
  //! It will be reflected in the GetSimulationTime method.
  //! Return value indicates engine was able to load provided state file.
  //! Engine will be in a cleared state if this method fails.
  //!-------------------------------------------------------------------------------------------------
  virtual auto LoadState(CDM::PhysiologyEngineStateData const& state, SEScalarTime const* simTime = nullptr) -> bool = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! State object will be returned.
  //! Engine will be in a cleared state if this method fails.
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetStateData() const -> std::unique_ptr<CDM::PhysiologyEngineStateData> = 0;
  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Save the current state of the engine.
  //! State will be written to a file.
  //! Engine will be in a cleared state if this method fails.
  //!-------------------------------------------------------------------------------------------------
  virtual auto SaveStateToFile(char const* file) const -> void = 0;
  virtual auto SaveStateToFile(const std::string& file) const -> void = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! locates the xml patient file and reads in the values.
  //!
  //! This will create an engine that you can send instructions (patient,actions,conditions) to dynamically.
  //! The return value will indicate success failure of the creation of the engine.
  //! Some combinations of patients and conditions may prevent the engine from stabilizing
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto InitializeEngine(char const* patientFile) -> bool = 0;
  virtual auto InitializeEngine(char const* patientFile, const std::vector<SECondition const*>* conditions) -> bool = 0;
  virtual auto InitializeEngine(char const* patientFile, const std::vector<SECondition const*>* conditions, PhysiologyEngineConfiguration const* config) -> bool = 0;
  virtual auto InitializeEngine(const std::string& patientFile, const std::vector<SECondition const*>* conditions = nullptr, PhysiologyEngineConfiguration const* config = nullptr) -> bool = 0;

  // NEW REFACTOR //
  virtual auto GetActions() const -> SEActionManager const& = 0;
  virtual auto GetConditions() const -> SEConditionManager const& = 0;


  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //!
  //! This will create an engine that you can send instructions (patient,actions,conditions) to dynamically.
  //! The return value will indicate success failure of the creation of the engine.
  //! Some combinations of patients and conditions may prevent the engine from stabilizing
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto InitializeEngine(const SEPatient& patient, const std::vector<SECondition const*>* conditions = nullptr, PhysiologyEngineConfiguration const* config = nullptr) -> bool = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Retrieve the Logger associated with this engine
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetLogger() const -> Logger const * = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Retrieve the PhysiologyEngineTrack associated with tracking data from this engine to a file
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetEngineTrack() const -> PhysiologyEngineTrack const & = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! returns the engine configuration.
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetConfiguration() const -> PhysiologyEngineConfiguration const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! returns the engine time step that is used when advancing time.
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetTimeStep(const TimeUnit& unit) const -> double = 0;
  virtual auto GetTimeStep() const -> SEScalarTime const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! returns the current time of the simulation.
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetSimulationTime(TimeUnit const& unit) const -> double = 0;
  virtual auto GetSimulationTime() const -> SEScalarTime const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! executes one pass through the time loop of the engine at the fixed timestep
  //!
  //! Events, errors, and warning as are logged to file not errors are returned
  //! through the API at this time.
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto AdvanceModelTime(bool appendDataTrack = false) -> bool = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! executes time loop of the engine beginning at the current time
  //!  and running for the duration specified in the call at the fixed timestep
  //!
  //! Events, errors, and warning as are logged to file not errors are returned
  //! through the API at this time.
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto AdvanceModelTime(double time, TimeUnit const& unit = TimeUnit::s, bool appendDataTrack = false) -> bool = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Execute the provided action.
  //! true will be returned if the engine supports the action
  //! false will be returned if the engine does not support the action
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto ProcessAction(SEAction const& action) -> bool = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Retrieves the associated substance manager.
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetSubstanceManager() const -> SESubstanceManager const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Add a callback object that will be called whenever a pateint or anesthesia machine event changes state
  //!-------------------------------------------------------------------------------------------------
  virtual auto SetEventHandler(SEEventHandler* handler) -> void = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the patient object used by the engine
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetPatient() const -> SEPatient const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Determines the assessment type and fills the data object with current data.
  //!
  //! Assessments can be queried at any point in the calculation and as many times are desired.
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetPatientAssessment(SEPatientAssessment& assessment) const -> bool = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the environment object used by the engine
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetEnvironment() const -> SEEnvironment const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Blood Chemistry System
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetBloodChemistrySystem() const -> SEBloodChemistrySystem const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Cardiovascular System
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetCardiovascularSystem() const -> SECardiovascularSystem const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the drug system
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetDrugsSystem() const -> SEDrugSystem const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Endocrine System
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetEndocrineSystem() const -> SEEndocrineSystem const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Energy System
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetEnergySystem() const -> SEEnergySystem const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Gastrointestinal System
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetGastrointestinalSystem() const -> SEGastrointestinalSystem const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Hepatic System
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetHepaticSystem() const -> SEHepaticSystem const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Nervous System
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetNervousSystem() const -> SENervousSystem const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Renal System
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetRenalSystem() const -> SERenalSystem const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Respiratory System
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetRespiratorySystem() const -> SERespiratorySystem const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the tissue system
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetTissueSystem() const -> SETissueSystem const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Anesthesia machine
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetAnesthesiaMachine() const -> SEAnesthesiaMachine const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Electrocardiogram machine
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetElectroCardioGram() const -> SEElectroCardioGram const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Inhaler
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetInhaler() const -> SEInhaler const& = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Retrieves the engine compartments, providing such data as:
  //! flows, pressure, volume as well as substance volumes and volume fractions.
  //!
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetCompartments() const -> SECompartmentManager const& = 0;
  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Return a Graph of DataRequest that the current PhysiologyEngine Supports.
  //!-------------------------------------------------------------------------------------------------
  virtual auto GetDataRequestGraph() const -> Tree<char const*> = 0;

  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the AutoTracking Property. True implies for each time advance
  //! All DataRequest will be tracked if the current simulation time is a multiple of
  //! GetDataRequestManager().GetSamplesPerSecond();
  //!-------------------------------------------------------------------------------------------------
  virtual auto IsAutoTracking() const -> bool = 0;
  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Return Allows the toggling of the SetAutoTrackFlag to On/Off
  //!-------------------------------------------------------------------------------------------------
  virtual auto SetAutoTrackFlag(bool flag) -> void = 0;
  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Returns the current state of the Track Stabilization Property.
  //! When true the user expects the Results file to include engine values during stabilization routines
  //! When false the user expects the 0 time index to be the first engine readout post stabilization.
  //!-------------------------------------------------------------------------------------------------
  virtual auto IsTrackingStabilization() const -> bool = 0;
  //!-------------------------------------------------------------------------------------------------
  //! \brief
  //! Return Allows the toggling of the SetAutoTrackFlag to On/Off
  //!-------------------------------------------------------------------------------------------------
  virtual auto SetTrackStabilizationFlag(bool flag) -> void = 0;
};
}