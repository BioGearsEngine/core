Physiology Engine Interface {#physeng}
===========================

The following infomation is a method by method walk through of the %PhysiologyEngine.h class that everything revolves around.
Reading this in parrallel with the HowTo-EngineUse.cpp provided in the SDK will give you a firm understanding of using %BioGears.

When you create an instance of a %BioGears engine, you will be returned a pointer to a PhysiologyEngine object.
This generic interface is the controlling class for a physiology engine modeling a single patient.

@code 
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("MyBioGearsEngine.log");
@endcode

@anchor LoggingInterface
### Logging

You can provide an optional string into this creation method that will create a log file on disk where the engine will output all errors, warnings and other logged information.
It is highly recommended to create a log, and check it often. If you do not wish to have this engine create a log, you can pass in nullptr.
You can also provide a callback class to the logger that will forward all logging events to so that your application can easily monitor the %BioGears engine.
For more details, please consult the HowTo-EngineUse.cpp file in the SDK.
You can access and utilize this logger with in your own application as such:
@code
  //--------------------------------------------------------------------------------------------------
	/// \brief
	/// Retrieve the Logger associated with this engine
	//--------------------------------------------------------------------------------------------------
	virtual Logger* GetLogger() = 0;
@endcode

@anchor EngineInitialization
### Initializing the Engine

There are two ways to initialize the engine once created.

@anchor EngineState
#### Engine State

An engine state file is the exact state of the engine written into an xml file. 
Once loaded, the engine is instatly ready to process instructions.
You can specify an xml file on disk, or a state object held in memory.
You can optionally specify a specific simulation time for the engine to use as its initial simulation time.
If no time is provided, the simulation time that is in the state file will be used.
The SDK provides a state file for various patients at simulation time 0 located in the bin/states directory.

@code
  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Reset engine and set it to the state in the provided file.
  /// You may provided a Simulation Time to be used if desired.
  /// It will be reflected in the GetSimulationTime method.
  /// Return value indicates engine was able to load provided state file.
  /// Engine will be in a cleared state if this method fails.
  //--------------------------------------------------------------------------------------------------
  virtual bool LoadState(const std::string& file, const SEScalarTime* simTime=nullptr) = 0;

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Reset engine and set it to the state in the provided file.
  /// You may provided a Simulation Time to be used if desired.
  /// It will be reflected in the GetSimulationTime method.
  /// Return value indicates engine was able to load provided state file.
  /// Engine will be in a cleared state if this method fails.
  //--------------------------------------------------------------------------------------------------
  virtual bool LoadState(const CDM::PhysiologyEngineStateData& state, const SEScalarTime* simTime = nullptr) = 0;
@endcode

At any point during the life of an engine, you create your own state object for later use.
@code
  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Save the current state of the engine.
  /// State will be written to a file if provided.
  /// State object will be returned.
  /// Engine will be in a cleared state if this method fails.
  //--------------------------------------------------------------------------------------------------
  virtual std::unique_ptr<CDM::PhysiologyEngineStateData> SaveState(const std::string& file = "") = 0;
@endcode

@anchor PatientCreation
#### Patient Creation

If you would like to create your own patient or apply a condition to a specific patient, 
you will need to initialize the engine with a patient definition.

@code
  //--------------------------------------------------------------------------------------------------
	/// \brief
	/// locates the xml patient file and reads in the values. 
	///
	/// This will create an engine that you can send instructions (patient,actions,conditions) to dynamically.
	/// The return value will indicate success failure of the creation of the engine.  
	/// Some combinations of patients and conditions may prevent the engine from stabilizing
	///
	//--------------------------------------------------------------------------------------------------
	virtual bool InitializeEngine(const std::string& patientFile, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr) = 0;

	//--------------------------------------------------------------------------------------------------
	/// \brief
	///
	/// This will create an engine that you can send instructions (patient,actions,conditions) to dynamically.
	/// The return value will indicate success failure of the creation of the engine.  
	/// Some combinations of patients and conditions may prevent the engine from stabilizing
	///
	//--------------------------------------------------------------------------------------------------
  virtual bool InitializeEngine(const SEPatient& patient, const std::vector<const SECondition*>* conditions = nullptr, const PhysiologyEngineConfiguration* config = nullptr) = 0;
@endcode

@anchor ConditionsInterface
When a patient definition is provided, %BioGears will go through an initialization algorithm that will take several minutes
as to tune the engine to model the specific state requested of the patient. 
This initalization method is also the only way to specify any conditions (chronic/disease states).

#### Patient Conditions
@secreflist
  @refitem ChronicAnemiaTable "Chronic Anemia"
  @refitem ChronicObstructivePulmonaryDiseaseTable "COPD"
  @refitem ChronicPericardialEffusionTable "Chronic Pericardial Effusion"
  @refitem ChronicRenalStenosisTable "Chronic Renal Stenosis"
  @refitem ChronicVentricularSystolicDysfunctionTable "Chronic Ventricular Systolic Dysfunction"
  @refitem ConsumeMealTable "Consume Meal"
  @refitem ImpairedAlveolarExchangeTable "Impaired Alveolar Exchange"
  @refitem LobarPneumoniaTable "Lobar Pneumonia"
@endsecreflist

#### %Environment Conditions
@secreflist
  @refitem InitialEnvironmentTable "Initial Environment"
@endsecreflist

<br>
Initializing the patient simulation with conditions will extend the initialization time by a few more minutes.
Once the InitalizeEngine method returns, the engine is stabilize, and it is recommended to save the engine state for future use.
(Assuming the patient vitals are acceptable, it may take some adjusting of condition severity to get a desired patient state with conditions applied.)
The SDK provides multiple tested patient files for use in the bin/patients directory. For more information on this look at the @ref PatientMethodology.

@anchor EngineConfiguration
##### Engine Configuration

You can provide an engine configuration to the initialize methods. 
This configuration is used to tweek various variables associated in the %BioGears methodology.
Configuration modification requires a very indepth knowledge of the engine.
It is not recommended to provide another configuration unless you know what effects it will have.
There are some useful configuration options that may be of interest, such as writing data to a csv file while the initialization algorithm executes.
Come visit us on the forums if this is something you want to know more about.

You can retrieve and view the configuration with this method (note you will need to cast it to the BioGearsConfiguration class to see all configuration data)

@code
  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// returns the engine configuration. 	
  //--------------------------------------------------------------------------------------------------
  virtual const PhysiologyEngineConfiguration* GetConfiguration() = 0;
@endcode

@anchor DataTracking
### Data Tracking

%BioGears has the ability to write specifically requested data to a comma seperated text file as the engine advances time.
These csv file are very helpful in debugging and ensuring %BioGears is modeling correctly.
How to utilize this functionality is demonstrated in all the HowTo files provided in the SDK.

@code
  //--------------------------------------------------------------------------------------------------
	/// \brief
	/// Retrieve the PhysiologyEngineTrack associated with tracking data from this engine to a file
	//--------------------------------------------------------------------------------------------------
	virtual PhysiologyEngineTrack* GetEngineTrack() = 0;
@endcode

@anchor AdvanceTime
### Advancing The Simulation In Time

Once the engine is initialized, it is ready to accept direction.
The engine does not advance time on its own, you must explictly tell the engine to simulate a specific amount of time.
There is a HowTo example that shows how to encapsulate the engine in a thread class that automatically advances time and processes actions.
How you decide to drive the simulation is up to you, but you must explicitly advance time inorder for the models to simulate physiology.

@code
  //--------------------------------------------------------------------------------------------------
	/// \brief
	/// executes one pass through the time loop of the engine at the fixed timestep
	///
	/// Events, errors, and warning as are logged to file not errors are returned
	/// through the API at this time. 
	///
	//--------------------------------------------------------------------------------------------------
	virtual void AdvanceModelTime() = 0;

	//--------------------------------------------------------------------------------------------------
	/// \brief
	/// executes one pass through the time loop of the engine at the fixed timestep
	///
	/// Events, errors, and warning as are logged to file not errors are returned
	/// through the API at this time. 
	///
	//--------------------------------------------------------------------------------------------------
	virtual void AdvanceModelTime(double time, const TimeUnit& unit) = 0;
@endcode

If no time is provided, the engine will simulate for the smallest amount of time it can.
The minimum amount of time the engine can simulate is the engines' time step, you can retrieve this duration with this method:

@code
  //--------------------------------------------------------------------------------------------------
	/// \brief
	/// returns the engine time step that is used when advancing time. 
	///
	//--------------------------------------------------------------------------------------------------
	virtual double GetTimeStep(const TimeUnit& unit) = 0;
@endcode

You can retrieve the total amount of time the engine has simulated with the following call:

@code
  //--------------------------------------------------------------------------------------------------
	/// \brief
	/// returns the current time of the simulation. 	
	//--------------------------------------------------------------------------------------------------
	virtual double GetSimulationTime(const TimeUnit& unit) = 0;
@endcode
Note that the simulation time is 0 when the InitializeEngine method returns.
Remember that you can set the simulation time to what ever you want when loading a state.
It will then be incremented from that point on whenever you call AdvanceModelTime.

@anchor ProcessActions
### Process Action

Actions are the means by which instructions are provided to a physiology engine.
You will need to create an instance of an action class, fill it out with the necessary data and pass it into this method.
@code
	//--------------------------------------------------------------------------------------------------
	/// \brief
	/// Execute the provided action.
	/// true will be returned if the engine supports the action
	/// false will be returned if the engine does not support the action
	///
	//--------------------------------------------------------------------------------------------------
	virtual bool ProcessAction(const SEAction& action) = 0;
@endcode

#### Patient Actions
@secreflist
  @refitem AcuteStressTable "Acute Stress"
  @refitem AirwayObstructionTable "Airway Obstruction"
  @refitem ApneaTable "Apnea"
  @refitem AsthmaAttackTable "Asthma Attack"
  @refitem BrainInjuryTable "Brain Injury"
  @refitem BronchoconstrictionTable "Bronchoconstriction"
  @refitem CardiacArrestTable "Cardiac Arrest"
  @refitem ChestCompressionForceTable "Chest Compression Force"
  @refitem ChestCompressionForceScaleTable "Chest Compression Force Scale"
  @refitem ChestOcclusiveDressingTable "Chest Occlusive Dressing"
  @refitem ConsciousRespirationTable "Conscious Respiration"
  @refitem ConsumeNutrientsTable "Consume Nutrients"
  @refitem ExerciseTable "Exercise"
  @refitem HemorrhageTable "Hemorrhage"
  @refitem IntubationTable "Intubation"
  @refitem MechanicalVentilationTable "Mechanical Ventilation"
  @refitem NeedleDecompressionTable "Needle Decompression"
  @refitem PericardialEffusionTable "Pericardial Effusion"
  @refitem SubstanceBolusTable "Substance Bolus"
  @refitem SubstanceCompoundInfusionTable "Substance Compound Infusion"
  @refitem SubstanceInfusionTable "Substance Infusion"
  @refitem TensionPneumothoraxTable "Tension Pneumothorax"
  @refitem UrinateTable "Urinate"
@endsecreflist

#### %Environment Actions
@secreflist
  @refitem EnvironmentChangeTable "Environment Change"
  @refitem ThermalApplicationTable "Thermal Application"
@endsecreflist

#### Anesthesia Machine Actions
@secreflist
  @refitem AnesthesiaMachineConfigurationTable "Anesthesia Machine Configuration"
  @refitem ExpiratoryValveLeakTable "Expiratory Valve Leak"
  @refitem ExpiratoryValveObstructionTable "Expiratory Valve Obstruction"
  @refitem InspiratoryValveLeakTable "Inspiratory Valve Leak"
  @refitem InspiratoryValveObstructionTable "Inspiratory Valve Obstruction"
  @refitem MaskLeakTable "Mask Leak"
  @refitem OxygenTankPressureLossTable "Oxygen Tank Pressure Loss"
  @refitem OxygenWallPortPressureLossTable "Oxygen Wall Port Pressure Loss"
  @refitem SodaLimeFailureTable "SodaLime Failure"
  @refitem TubeCuffLeakTable "Tube Cuff Leak"
  @refitem VaporizerFailureTable "Vaporizer Failure"
  @refitem VentilatorPressureLossTable "Ventilator Pressure Loss"
  @refitem YPieceDisconnectTable "Y-Piece Disconnect"
@endsecreflist

#### %Inhaler Actions
@secreflist
  @refitem InhalerConfigurationTable "Inhaler Configuration"
@endsecreflist

@anchor PatientState
### Patient State

Once you start advancing time and processing actions, the patient state can start to change.
Depending on what actions you process, it can change slightly, such as a slight increase in mean arterial pressure,
or it can change quite dramatically and start affecting multiple systems through various physiological feedback mechanisms. 
These dramatic changes in patient state are tracked by means of @ref PatientEventTable "Patient Events". 

You can track patient event states in one of two ways.

- Poll the patient object via Patient object method:
@code
  //--------------------------------------------------------------------------------------------------
	/// \brief
	/// Returns the patient object used by the engine 
	///
	//--------------------------------------------------------------------------------------------------
	virtual const SEPatient& GetPatient() = 0;
@endcode
Note you can get more @ref PatientTable "Patient" data from this class, not just event status.

- Set up a callback method that will automatically call your code when patient event status changes.
Set the callback method with this method
@code
  //--------------------------------------------------------------------------------------------------
	/// \brief
	/// Add a callback object that will be called whenever a pateint or anesthesia machine event changes state
	//--------------------------------------------------------------------------------------------------
	virtual void SetEventHandler(SEEventHandler* handler) = 0;
@endcode

Event State is primarily associated with the patient, but there are events associated with @ref AnesthesiaMachineEventTable "Anesthesia Machine".

Look at the SDK HowTo-UseEngine.cpp for a full example in using both of these methods.

@anchor AssessmentsInterface
### Patient Assessments

Patient assessments are intended to give general patient overviews, formed at the level of a clinician's report.
The following assessments are availiable from %BioGears:

@secreflist
  @refitem CompleteBloodCountTable "Complete Blood Count"
  @refitem ComprehensiveMetabolicPanelTable "Comprehensive Metabolic Panel"
  @refitem UrinalysisTable "Urinalysis"
  @refitem PulmonaryFunctionTestTable "Pulmonary Function Test"
@endsecreflist 

You must create and provide an assessment object to the physiology engine via this method:<br>
*Note that assessments could add extra comupation time to gather and format data for the assessment.*

@code
  //--------------------------------------------------------------------------------------------------
	/// \brief
	/// Determines the assessment type and fills the data object with current data. 
	///
	/// Assessments can be queried at any point in the calculation and as many times are desired. 
	///
	//--------------------------------------------------------------------------------------------------
	virtual bool GetPatientAssessment(SEPatientAssessment& assessment) = 0; 
@endcode

@anchor SystemsInterface
### Systems Data

The bodies physiology, equipment, and the environment are all systems and each system has a method to retrieve its associated class in order to access this system data.<br>

%BioGears supports the following systems:

|Code Method                                                                             | CDM Table                                           |
|---                                                                                     |---                                                  |
|@code virtual const SEEnvironment* GetEnvironment() = 0; @endcode                       | @ref EnvironmentTable "Environment"                 |
|@code virtual const SEBloodChemistrySystem* GetBloodChemistrySystem() = 0; @endcode     | @ref BloodChemistrySystemTable "BloodChemistry"     |
|@code virtual const SECardiovascularSystem* GetCardiovascularSystem() = 0; @endcode     | @ref CardiovascularSystemTable "Cardiovascular"     |
|@code virtual const SEEndocrineSystem* GetEndocrineSystem() = 0; @endcode               | @ref EndocrineSystemTable "Endocrine"               |
|@code virtual const SEEnergySystem* GetEnergySystem() = 0; @endcode                     | @ref EnergySystemTable "Energy"                     |
|@code virtual const SERenalSystem* GetRenalSystem() = 0; @endcode                       | @ref RenalSystemTable "Renal"                       |
|@code virtual const SEGastrointestinalSystem* GetGastrointestinalSystem() = 0; @endcode | @ref GastrointestinalSystemTable "Gastrointestinal" |
|@code virtual const SENervousSystem* GetNervousSystem() = 0; @endcode                   | @ref NervousSystemTable "Nervous"                   |
|@code virtual const SERespiratorySystem* GetRespiratorySystem() = 0; @endcode           | @ref RespiratorySystemTable "Respiratory"           |
|@code virtual const SEDrugSystem* GetDrugSystem() = 0; @endcode                         | @ref DrugSystemTable "Drug"                         |
|@code virtual const SETissueSystem* GetTissueSystem() = 0; @endcode                     | @ref TissueSystemTable "Tissue"                     |
|@code virtual const SEAnesthesiaMachine* GetAnesthesiaMachine() = 0; @endcode           | @ref AnesthesiaMachineTable "Anesthesia Machine"    |
|@code virtual const SEElectroCardioGram* GetElectroCardioGram() = 0; @endcode           | @ref ElectroCardioGramTable "ElectroCardioGram"     |
|@code virtual const SEInhaler* GetInhaler() = 0; @endcode                               | @ref InhalerTable "Inhaler"                         |

@anchor CompartmentsInterface
### Compartments

A compartment represents the fluid dynamics of an anatomical organ or equipment component. 
Comparments can represent various fidelities of data for these compoenents, such as:
	- An anatomical space, such as the body's skin, muscles
	- An organ, such as the liver
	- An organ substructure, such as the Left Heart
	- Extravascular tissue of an organ
	- A component of a piece of equipment, such as an anesthesia machine ventillator
  
The following compartment types are used to represent various anatomical structures for both physiology and equipment:

@secreflist
 @refitem GasCompartmentTable "Gas Compartment"
 @refitem LiquidCompartmentTable "Liquid Compartment"
 @refitem ThermalCompartmentTable "Thermal Compartment"
 @refitem TissueCompartmentTable "Tissue Compartment"
@endsecreflist 

Various comparments can be used to represent a parent/child hierarcical structure, and comparments can be a parent for one or more child compartments.
Parent compartments agregate data from their children, such as the parent compartment volume is simply the sum of each of its childrens' volume.<br>
For eample, the heart has the following hierarchy :

  - Heart
    - Myocardium
    - Right Heart
    - Left Heart
    - Pericardium

Compartments also contain a substance quatity for each substance in the compartment as it moves through the body/equipment. <br>
The following types are used to hold compartment substance information

@secreflist
 @refitem GasSubstanceQuantityTable "Gas Substance Quantity"
 @refitem LiquidSubstanceQuantityTable "Liquid Substance Quantity"
@endsecreflist

The enumerations for compartments available in %BioGears is found in the BioGearsPhysiologyEngine.h file.
As these are programatic enumerations, you can use the auto-complete feature of your favorite programming IDE to view these enumerations as you code.
%BioGears discritizes it's compartments into enumerations based on fluid type and equipment. Here is a list of the various enumerated compartment names:

@secreflist
 @refitem LiquidCompartmentTable  "Chyme"
 @refitem GasCompartmentTable     "Pulmonary"
 @refitem LiquidCompartmentTable  "Pulmonary(Aerosols)"
 @refitem TissueCompartmentTable  "Tissue"
 @refitem LiquidCompartmentTable  "Extracellular"
 @refitem LiquidCompartmentTable  "Intracellular"
 @refitem LiquidCompartmentTable  "Vascular"
 @refitem LiquidCompartmentTable  "Urine"
 @refitem LiquidCompartmentTable  "Lymph"
 @refitem ThermalCompartmentTable "Temperature"
 @refitem GasCompartmentTable     "Environment"
 @refitem GasCompartmentTable     "Anesthesia Machine"
 @refitem GasCompartmentTable     "Inhaler"
@endsecreflist

As we make changes to models, some of the naming/hierarchy of compartments used by %BioGears could change, it's best to refer to the code file itself 
for the latest list of available compartments and their hierarchy.

Note that there are both liquid and gas compartments available for pulmonary spaces. 
The gas compartments represent the air flow and gaseous substances through the pulmonary tract.
The liquid compartments represent the air flow and aerosolized liquid/solid substances through the pulmonary tract.

All compartments are accessed via the SECompartmentManager retrieved from this method:

@code
  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Retrieves the engine compartments, providing such data as:
  /// flows, pressure, volume as well as substance volumes and volume fractions.
  ///
  //--------------------------------------------------------------------------------------------------
  virtual const SECompartmentManager& GetCompartments() = 0;
@endcode

@anchor SubstancesInterface
#### Substances

Various Substances are available in the body and there is a SESubstanceManager associated with the engine. <br>
You can retrieve the SESubstanceManager with this method:

@code
  //--------------------------------------------------------------------------------------------------
	/// \brief
	/// Retrieves the associated substance manager.
	///
	//--------------------------------------------------------------------------------------------------
	virtual SESubstanceManager& GetSubstanceManager() = 0;
@endcode

In practice, only a substance reference are used with compartments to get the Substance Quantity object associated with a particular compartment.
The data contained in the @ref SubstanceTable "substance definition" is primarily used in engine methodology, 
but there may be substance data you maybe interested in, such as the @ref Substance_SubstanceTissuePharmacokineticsData "Partition Coefficient" of a substance.


