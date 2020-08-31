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



// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// A class used to forward log messages for application specific handling logic
///
/// \details
/// The data model has a logging capability that the engine utilizes to log any info to a log file.
///
/// This logger has different levels at which it can log events:
///
///     Debug   - Extremely low level information, intended for the developer to assess any questions in functionality
///     Info    - A general level of information where the engine wants to inform a user of something
///     Warning - Something was not input properly or an assumption was made in the underlying methodology
///     Error   - A recoverable error has occured and the engine is still able to proceed, but results should be inspected for correctness
///     Fatal   - A unrcoverable error has occured and the engine will stop calculating
///
/// The user can create custom forwarders and add them to the engine's logger so that the forwarding methods are called
/// when events are logged. This allows a calling program to be notified of logging messages in order to handle them as needed.
//--------------------------------------------------------------------------------------------------
class MyLogger : public LoggerForward {
public:
  virtual void ForwardDebug(const std::string& msg, const std::string& origin) {}
  virtual void ForwardInfo(const std::string& msg, const std::string& origin) {}
  virtual void ForwardWarning(const std::string& msg, const std::string& origin) {}
  virtual void ForwardError(const std::string& msg, const std::string& origin) {}
  virtual void ForwardFatal(const std::string& msg, const std::string& origin) {}
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// A class used to forward event activity for application specific handling logic
///
/// \details
/// Various objects can enter into and out of a particular state, this will let you know when that happens.
///
/// Currently there are two objects that have event based data
///
///     Patient - please see the Patient.xsd or https://www.biogearsengine.com/documentation/group___patient__enum_patient_event.html
///     AnesthesiaMachine - please see Anesthesia.xsd or https://www.biogearsengine.com/documentation/group___anesthesia__enum_anesthesia_machine_event.html
///
/// The time provided is the current engine time.
/// The provided methods will be called at the end of the time step when the event was triggered
/// This allows a calling program to be notified of events in order to handle them as needed.
//--------------------------------------------------------------------------------------------------
class MyEventHandler : public SEEventHandler {
public:
  MyEventHandler(Logger* logger)
    : SEEventHandler()
  {
  }
  virtual void HandlePatientEvent(CDM::enumPatientEvent::value type, bool active, const SEScalarTime* time = nullptr) {}
  virtual void HandleAnesthesiaMachineEvent(CDM::enumAnesthesiaMachineEvent::value type, bool active, const SEScalarTime* time = nullptr) {}
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage of using a Physiology Engine
///
/// \details
/// Usage of the Logger class
/// Loading a SEPatient from disk
/// Advancing time
/// Retrieving data from a Physiology Engine
//--------------------------------------------------------------------------------------------------
void HowToEngineUse()
{
  // Create an engine object
  // BioGearsEngines will always output log messages to stdout and a log file
  // If you want this engine to write a log file, include the name
  // of the log file. If nullptr is given, the engine will only output to the console
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToEngineUse.log");
  bg->GetLogger()->Info("HowToEngineUse");

  // This BioGearsEngine logger is based on log4cpp (which is based on log4j)
  // BioGearsEngine logs to several distinct, ordered
  // category levels: DEBUG, INFO, WARN, ERROR, FATAL
  // These categories are orders, if your level is set to DEBUG you will recieve ALL messages.
  // If set to INFO, you will not recieve DEBUG, but everything else
  // If set to WARN, you will not recieve DEBUG and INFO, but everything else
  // You can specify which level you would like the engine to log
  // By Default the LogLevel is INFO
  // bg->GetLogger()->SetLogLevel(log4cpp::Priority::INFO);

  // You can tell the BioGearsEngine to also direct any output
  // to your own function. For example if you want to capture and process messages
  // and handle them in your own way, Give BioGearsEngine an instance of the LoggerForward class
  // and BioGearsEngine will log and also call the provided method with the message.
  // You can specify a set of functions to be called for any one of the log levels
  MyLogger myLogger;
  bg->GetLogger()->SetForward(&myLogger);

  // You can tell the BioGearsEngine to also notify you of any events as well
  // You can Poll objects for events, for example
  // bg->GetPatient().

  // NOTE, setting the LogLevel, Forwarder, EventHandler can be done after initialize and as many times as you want

  // The first order of business is to initialize the engine by loading a patient state.
  // Patient states provided in the SDK are the state of the engine at the time they stabilize
  // More details on creating a patient and stabilizing the engine can be found in HowTo-CreateAPatient.cpp
  SEScalarTime startTime;
  // You can optionally specify a specific simulation time for the engine to use as its initial simulation time
  // If no time is provided, the simulation time that is in the state file will be used
  // Note the provided state files are named to include what is simulation time is
  startTime.SetValue(0, TimeUnit::s);
  if (!bg->LoadState("./states/StandardMale@0s.xml", &startTime)) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }
  // See below on how to save a state

  // There are specific events that can occur while the engine runs and you submit various actions
  // You can either poll/query the patient object to see if it is in a specific state
  bg->GetPatient().IsEventActive(CDM::enumPatientEvent::CardiacArrest);
  // You can also derive a callback class that will be called whenever an Event is entered or exited by the patient
  MyEventHandler myEventHandler(bg->GetLogger());
  bg->SetEventHandler(&myEventHandler);

  // We are interested in 2 substances and their amounts in compartments
  // Let's grab the substance via name from the substance manager, string look ups are slow
  // So it's a good idea to cache this pointer so you can reuse it when asking at various time steps
  // You can find all substances defined in xml files in the substances directory of your working directory
  // Names are in those xml files. ALL substance xml files are loaded into a substance managers when the engine is created
  SESubstance* O2 = bg->GetSubstanceManager().GetSubstance("Oxygen");
  SESubstance* CO2 = bg->GetSubstanceManager().GetSubstance("CarbonDioxide");

  // The tracker is responsible for advancing the engine time AND outputting the data requests below at each time step
  // If you do not wish to write data to a file, you do not need to make any data requests
  

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TidalVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalLungVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("OxygenSaturation");
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set(BGE::VascularCompartment::Aorta, *O2, "PartialPressure");
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set(BGE::VascularCompartment::Aorta, *CO2, "PartialPressure");
  bg->GetEngineTrack()->GetDataRequestManager().CreateGasCompartmentDataRequest().Set(BGE::PulmonaryCompartment::Lungs, "Volume");
  bg->GetEngineTrack()->GetDataRequestManager().CreateGasCompartmentDataRequest().Set(BGE::PulmonaryCompartment::Trachea, "InFlow");

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToEngineUse.csv");

  // We are ready to execute the engine
  // simply tell the engine how long you would like it to execute
  bg->AdvanceModelTime(5, TimeUnit::s); // Note this tracker class takes in seconds
  // If not using the tracker call
  // bg->AdvanceModelTime(5,TimeUnit::s);
  // If you do not provide a time, the engine will execute 1 time step, the BioGears timestep can be retrieved with:
  //bg->GetTimeStep(TimeUnit::s);

  // You will notice that the timing calls mentioned above take in a specific unit
  // The Common Data Model has a conversion library internal to it that will convert data to whatever you specify.
  // There are predefined units defined in each unit quantity scalar class header.
  // Note the SE simpley stands for Synthetic Enviroment.
  // Each unit quantity has it's own class representation along with commonly used unit definitions
  // You may use strings to define your unit if you do not find a unit predefined (but strings have a runtime cost to them)
  // Units are defined in the standard abbreviation, for example:
  // http://www.bipm.org/en/si/
  // Along with si units, english units and medical units are supported
  // You can check to see if a unit is valid by calling the IsValidUnit method on a particular quantity scalar class.

  // Engine Data is available in 3 general forms
  // Assessments - Data formed at a level of a clinicians report.
  //               This is high level data, such as a mean or generalized data
  //               value intended to give a general overview of the state of the system.
  // Systems - System specific data properties that provide more specific information for a specific system
  //           A system can be a physiological system (cardiovascular, respiratory, etc.).
  //           It could also be a piece of equipment (anesthesia machine, ECG, etc.)
  // Compartments - Compartments are a more detailed view into the components that make up a system (Anatomy or Equipment)
  //                compartments reflect the low level data such as Volume, Pressure, Flow, and Substance Quantities (Mass/Concentration Volume/VolumeFraction)
  //                Compartments are contained in a hierarchical object model that  encapsulates the body and its anatomy down to organs and organ components
  //                For example the lung compartment is made up of the left lung and right lung

  // When retrieving one of these types of data structures, the structure will contain various properties of a SEScalar types, std::string and enums.
  // (ex. HeartRate, Volume, Flow, ResperatoryRate, etc.)
  // The general naming convention used in creating these structures and their properties is a 'Get', 'Has' style
  // For Scalar objects, these structures only instantiate scalar objects for data they have, in order to maintain a small memory footprint.
  // Calling the Get method will create A new scalar (managed by the structure) and the value will be set to NaN.
  // ex. bg->GetCardiovascularSystem()->GetBloodVolume()->GetValue(VolumeUnit::L);
  // You can check to see if an engine has output a particular property by calling a complimentary 'Has' method
  // ex. bg->GetCardiovascularSystem()->HasBloodVolume(), if the value is not NaN, true will be returned
  //
  // When you set a value to a scalar, you MUST provide a unit!
  // When you which to get the value of a scalar, you MUST provide a unit!
  // If the scalar is of a unitless quantity (i.e. fractions) you can use SEScalar::unitless or SEScalar::nullUnit
  // Scalars hold the data in the unit provided, conversions are only performed if the requested unit is not the same as the Scalars set unit
  // Conversions are fast, but it is recommended to understand the units used in the engine and try to use those for optimal use of the engine
  //
  // SEScalar has overloaded the << operator and will out put in the form of 'value(unit)' (ex. '50(mL)')
  // If the scalar is unitless no parens will be provided.

  // Here we demostrate pulling data from various physiology systems

  bg->GetLogger()->Info(std::stringstream() << "HeartRate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "BloodVolume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "ArterialSystolicPressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "ArterialDiastolicPressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "MeanArterialPressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);

  // Here we demonstrate pulling data from a compartment
  // NOTE THAT WHEN YOU PULL A COMPARTMENT FROM THE COMPARTMENT MANAGER, THAT IS A STRING LOOK UP, TRY TO CACHE COMPARTMENTS OF INTEREST INTO YOUR OWN LIST/MAP
  // This allows a more direct access to the underlying data calculated by the methodology
  // For example, getting an Invasive Blood Pressure, any arterial compartment pressure can be pulled, i.e. femoral artery (right/left leg)
  // Since this is an arterial compartment, blood is flowing through this compartment, where as the carina compartment is air flow
  bg->GetLogger()->Info(std::stringstream() << "Invasive Blood Pressure : " << bg->GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Aorta)->GetPressure(PressureUnit::mmHg) << PressureUnit::mmHg);

  bg->GetLogger()->Info(std::stringstream() << "RespirationRate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
  bg->GetLogger()->Info(std::stringstream() << "Total Lung Volume : " << bg->GetRespiratorySystem()->GetTotalLungVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "End Tidal CarbonDioxide Fraction : " << bg->GetRespiratorySystem()->GetEndTidalCarbonDioxideFraction());

  // Using the compartment we can get more detailed respiratory information
  // Here we will test the flow of air in the carina compartment. A positive flow is an inhale and a negative flow is an exhale.
  // We can get the amount of CO2 exhaled and O2 inhaled by looking at the volume fraction of the carina of a particular substance

  const SEGasCompartment* carina = bg->GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::Trachea);
  if (carina->GetInFlow(VolumePerTimeUnit::L_Per_s) > 0) { // We are inhaling, so let's grab the amount of O2 coming into the body
    bg->GetLogger()->Info(std::stringstream() << "O2 Inhaled " << carina->GetSubstanceQuantity(*O2)->GetVolume(VolumeUnit::mL) << VolumeUnit::mL);
  } else { // We are exhaling, so let's grab the amount of CO2 that is leaving the body
    bg->GetLogger()->Info(std::stringstream() << "CO2 Exhaled " << carina->GetSubstanceQuantity(*CO2)->GetVolume(VolumeUnit::mL) << VolumeUnit::mL);
  }

  bg->GetLogger()->Info(std::stringstream() << "OxygenSaturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());
  bg->GetLogger()->Info(std::stringstream() << "ArterialBlood pH : " << bg->GetBloodChemistrySystem()->GetArterialBloodPH());
  //  You should save off the SESubstanceQuantity* if you will need it more than once
  bg->GetLogger()->Info(std::stringstream() << "Lactate Concentration : " << bg->GetSubstanceManager().GetSubstance("Lactate")->GetBloodConcentration(MassPerVolumeUnit::mg_Per_dL) << MassPerVolumeUnit::mg_Per_dL);
  bg->GetLogger()->Info(std::stringstream() << "Core Body Temperature : " << bg->GetEnergySystem()->GetCoreTemperature(TemperatureUnit::C) << TemperatureUnit::C);

  // Save the state of the engine
  bg->SaveState("./states/FinalEngineUseState.xml");

  bg->GetLogger()->Info("Finished");
}
