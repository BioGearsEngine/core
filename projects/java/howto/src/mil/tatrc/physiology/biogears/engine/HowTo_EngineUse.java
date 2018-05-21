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
package mil.tatrc.physiology.biogears.engine;

import java.util.*;

import mil.tatrc.physiology.datamodel.bind.EnumAnesthesiaMachineEvent;
import mil.tatrc.physiology.datamodel.bind.EnumPatientEvent;
import mil.tatrc.physiology.datamodel.bind.EnumSex;
import mil.tatrc.physiology.datamodel.patient.SEPatient;
import mil.tatrc.physiology.datamodel.patient.actions.SEHemorrhage;
import mil.tatrc.physiology.datamodel.patient.assessments.SECompleteBloodCount;
import mil.tatrc.physiology.datamodel.patient.conditions.ChronicAnemia;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.FrequencyUnit;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.LengthUnit;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.MassUnit;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.PressureUnit;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.TimeUnit;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.VolumePerTimeUnit;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.VolumeUnit;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;
import mil.tatrc.physiology.datamodel.scenario.conditions.SECondition;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEDataRequestManager;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEPhysiologyDataRequest;
import mil.tatrc.physiology.datamodel.utilities.SEEventHandler;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.LogListener;
import mil.tatrc.physiology.datamodel.bind.IntegerArray;

/**
 * How to use the Java interface to BioGears
 * For eclipse projects, please set the Debug/Run Configuration Working Directory for this file to
 * ${workspace_loc:BioGearsHowTo}.\bin
 */
public class HowTo_EngineUse
{
//Create a listener that will catch any messages logged in C++
 // This class will take the messages and add them to the log created in Java
 protected static class MyListener extends LogListener
 {
   protected MyListener()
   {
     super();
     // Since we are just pushing this data into the log
     // We set listen to false, when you create a LogListener
     // is will automatically register itself with the static Log class,
     // and when ever a Log even class is called, this will be called
     // and since this just calls the Log to log you will get into a recursive infinite loop.
     // This is just because I use the LogListener interface to listen to any log messages coming
     // from C++. Technically it is a LogListener as it is 'listening' to log events in C++.
     // At any rate the Java side will receive log events from C++, and you create your own 
     // LogListener so you can do as you see fit with those messages. BUT if you do want to 
     // push those messages to the Java Log, you will need this class to not listen so you
     // don't get into an infinite recursive loop
     listen(false);
   }    
   public void handleDebug(String msg) { Log.debug(msg); }
   public void handleInfo(String msg)  { Log.info(msg); }
   public void handleWarn(String msg)  { Log.warn(msg); }
   public void handleError(String msg) { Log.error(msg); }
   public void handleFatal(String msg) { Log.fatal(msg); }
 }
 
 protected static class MyEventHandler implements SEEventHandler
 {
  public void handlePatientEvent(EnumPatientEvent type, boolean active, SEScalarTime time)
  {
    if(type == EnumPatientEvent.START_OF_CARDIAC_CYCLE && active)
      Log.info("Patient started a new heart beat at time "+time);    
  }

  public void handleAnesthesiaMachineEvent(EnumAnesthesiaMachineEvent type, boolean active, SEScalarTime time)
  {
    
  } 
 }
 
 public enum InitializationType { PatientObject, PatientFile, StateFile };
 
 public static void main(String[] args)
 {
	 SEScalarTime time = new SEScalarTime(0, TimeUnit.s);
	 
   // Create a log file for this example (by default, a BioGears.log will be made)
   // NOTE the engine will have its own log, so there is a Java log and an Engine log!!
   // You don't have to have a Java log, but if you want to, this is how you can do it.
   Log.setFileName("HowTo_DynamicEngineDriver.log");
   // Note there is a single singleton logger in Java, but a log per engine in C++
   // i.e. in Java, multiple engines will write to the same log, where as in C++, each engine will write to its own log
   // The listener and callback objects are unique to this engine
   
   // Create a BioGears Engine
   BioGearsEngine bge = new BioGearsEngine();// The engine log will be named based the provided string + ".log" 
   
   // I am going to create a listener that will get any log messages (Info, Warnings, Errors, Fatal Errors)
   // that come from the engine. The default listener will just put them into the log file
   // If you want to do custom logic that occurs when the engine throws an error (or any other message type), just create a class just like this one
   bge.setListener(new MyListener());
   
   // I want to know when ever the patient and anesthesia machine(if used) enters and exits a particular state
   bge.setEventHandler(new MyEventHandler());
   
   // Here are the data I want back from the engine
   // The CDM objects on the bge object will be updated 
   // at the end of AdvanceTime calls (which are blocking)
   // No other data values will have data in Java classes
   SEDataRequestManager dataRequests = new SEDataRequestManager();
   SEPhysiologyDataRequest hr = new SEPhysiologyDataRequest();
   hr.setName("HeartRate");
   hr.setUnit(FrequencyUnit.Per_min.toString());
   dataRequests.getRequestedData().add(hr);
   SEPhysiologyDataRequest rr = new SEPhysiologyDataRequest();
   rr.setName("RespirationRate");
   rr.setUnit(FrequencyUnit.Per_min.toString());
   dataRequests.getRequestedData().add(rr);
   SEPhysiologyDataRequest tlv = new SEPhysiologyDataRequest();    
   tlv.setName("TotalLungVolume");
   tlv.setUnit(VolumeUnit.mL.toString());
   dataRequests.getRequestedData().add(tlv);
   SEPhysiologyDataRequest bv = new SEPhysiologyDataRequest();    
   bv.setName("BloodVolume");
   bv.setUnit(VolumeUnit.mL.toString());
   dataRequests.getRequestedData().add(bv);
   
   
   InitializationType initType = InitializationType.StateFile;
   // INITIALIZE THE ENGINE WITH A PATIENT
   switch(initType)
   {
   case PatientObject:
   	{
      // Set up your patient
      // This is the minimum and recommended data to set for a patient
      // But you can get more complicated, consult our Patient Variability Documentation for more details
      SEPatient patient = new SEPatient();
      patient.setName("Standard");
      patient.setSex(EnumSex.MALE);
      patient.getAge().setValue(44,TimeUnit.yr);
      patient.getWeight().setValue(170,MassUnit.lb);
      patient.getHeight().setValue(71,LengthUnit.in);
      patient.getBodyFatFraction().setValue(0.21);
      patient.getHeartRateBaseline().setValue(72,FrequencyUnit.Per_min);
      patient.getDiastolicArterialPressureBaseline().setValue(72.,PressureUnit.mmHg);
      patient.getSystolicArterialPressureBaseline().setValue(114.,PressureUnit.mmHg);
      patient.getRespirationRateBaseline().setValue(16,FrequencyUnit.Per_min);
      
      // If I wanted to make set a condition on the patient
      // Note that while you can have multiple conditions on a patient
      // It is more than likely not tested and the engine may or may not converge
      List<SECondition> conditions = new ArrayList<SECondition>();
      ChronicAnemia anemia = new ChronicAnemia();// One of these days I will prefix these with SE...
      anemia.getReductionFactor().setValue(0.3);
      conditions.add(anemia);
      
      // Allocate an engine
      bge.initializeEngine("./Scenarios/HowToDynamicEngine.log", patient, null/*optionally, pass in our conditions list*/, dataRequests);
	     // This method will block while the engine stabilizes to meet the defined patient parameters
   		break;
   	}
   case PatientFile:
	   {
	     // Allocate an engine
	     bge.initializeEngine("./Scenarios/HowToDynamicEngine.log", "./patient/StandardMale.xml", null/*optionally, pass in a conditions list*/, dataRequests);	     
	     // This method will block while the engine stabilizes to meet the defined patient parameters
	  	 break;
	   }
   case StateFile:
	   {
	  	 bge.loadState("./Scenarios/HowToDynamicEngine.log", "./states/StandardMale@0s.xml", dataRequests);
	  	 // This method method sets the engine to the provided state instantaneously and you are ready to process actions/advance time
	  	 // You can alternatively specify the starting simTime of the engine	  	 
	  	 //bge.loadState("./Scenarios/HowToDynamicEngine.log", "./states/StandardMale@0s.xml", time, dataRequests);
	  	 break;
	   }
   }
      
   // Now we can start telling the engine what to do
   // All the same concepts apply from the C++ HowTo files, so look there if you want to see more examples
   
   if(!bge.advanceTime()) // Advance one engine time step
   {
     Log.info("Something bad happened");
     return;
   }
   // The CDM objects contained in bge will be automatically updated after this method returns
   // NOTE ONLY THE CDM PROPERTIES ASSOCIATED WITH THE DATA REQUESTS PROVIDED IN initializeEngine WILL BE UPDATED
   // it would be pretty slow to push EVERYTHING from the engine into Java if you are not using it all.
   // This is just a limitation of the Java interface, let me know if you think we can push everything or have some other idea for pushing..
   // SO this does mean that you have to know up front everything you are going to use from the engine, there may be ways to make it more
   // dynamic it getting data back from the engine, but at this time, I am not going to support that, not that it cannot be done, again let's talk if you want it...
   
   Log.info("Heart Rate " + bge.cardiovascular.getHeartRate());
   Log.info("Respiration Rate " + bge.respiratory.getRespirationRate());
   Log.info("Total Lung Volume " + bge.respiratory.getTotalLungVolume());
   Log.info("Blood Volume " + bge.cardiovascular.getBloodVolume());    
   
   // Let's get an assessment from the engine
   // Assessments can involve extra calculation to generate the data necessary for the specified assessment
   SECompleteBloodCount cbc = new SECompleteBloodCount();
   bge.getPatientAssessment(cbc);
   Log.info("Red Blood Count "+cbc.getRedBloodCellCount());
   Log.info("White Blood Count "+cbc.getWhiteBloodCellCount());
   
   // You can check if the patient is in a specific state/event
   if(bge.patient.isEventActive(EnumPatientEvent.CARDIAC_ARREST))
     Log.info("CODE BLUE!");
   
   time.setValue(1, TimeUnit.s);
   if(!bge.advanceTime(time)) // Simulate one second
   {
     Log.info("Something bad happened");
     return;
   }
   // Again, the CDM is updated after this call
   
   Log.info("Heart Rate " + bge.cardiovascular.getHeartRate());
   Log.info("Respiration Rate " + bge.respiratory.getRespirationRate());
   Log.info("Total Lung Volume " + bge.respiratory.getTotalLungVolume());   
   Log.info("Blood Volume " + bge.cardiovascular.getBloodVolume());      
   
   // Let's do something to the patient, you can either send actions over one at a time, or pass in a List<SEAction>
   SEHemorrhage h = new SEHemorrhage();
   //The injury code is stored in a 5 digit Integer Array (which contains an IntegerList)
   //Access IntegerList and add code for severe bleeding from abdominal artery--42650)
   bge.processAction(h);
   // Note CDM is not updated after this call, you have to advance some time

   time.setValue(5.0,TimeUnit.min);
   if(!bge.advanceTime(time)) // Simulate one second
   {
     Log.error("Engine was unable to stay within modeling parameters with requested actions");
     return;
   }
   // Again, the CDM is updated after this call
   
   Log.info("Heart Rate " + bge.cardiovascular.getHeartRate());
   Log.info("Respiration Rate " + bge.respiratory.getRespirationRate());
   Log.info("Total Lung Volume " + bge.respiratory.getTotalLungVolume());     
   Log.info("Blood Volume " + bge.cardiovascular.getBloodVolume());    
   
   // Be nice to your memory and deallocate the native memory associated with this engine if you are done with it
   bge.cleanUp();
   // Note you can now run a static (scenario) or another dynamic engine with the bge object, it will allocate and manage a new C++ engine 
 }
}

