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

import mil.tatrc.physiology.datamodel.properties.CommonUnits.FrequencyUnit;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.TimeUnit;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.VolumeUnit;
import mil.tatrc.physiology.datamodel.scenario.SEScenario;
import mil.tatrc.physiology.datamodel.scenario.actions.SEAdvanceTime;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEPhysiologyDataRequest;
import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Resources;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.LogListener;

import java.io.FileNotFoundException;

public class HowTo_RunScenario
{

  // Create a callback object that BioGears will call at the specified update frequency.
  // Note the time provided refers to simulation time, not real time
  // For example, if you request a callback every second, it will be called
  // every time 1 second is simulated
  protected static class MyCallback extends CDMUpdatedCallback
  {
    protected BioGears bg;
    protected MyCallback(BioGears bg, double updateFrequency_s)
    {
      super(updateFrequency_s);
      this.bg = bg;
    }
    
    public void update(double time_s)
    {
      // Note only the data requested (As a DataRequest) in the scenario file will be updated in the BioGears CDM objects
      // All data is pulled from the engine at callback time, so when we get here, the CDM objects have the latest values      
      Log.info("Heart Rate " + bg.cardiovascular.getHeartRate());
      Log.info("Respiration Rate " + bg.respiratory.getRespirationRate());
      Log.info("Total Lung Volume " + bg.respiratory.getTotalLungVolume());      
    }    
  }
  
  // Create a listener that will catch any messages logged in C++
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
  
  public static void main(String[] args)
  {
    // Create a log file for this example (by default, a BioGears.log will be made)
    // NOTE the engine will have its own log, so there is a Java log and an Engine log!!
    // You don't have to have a Java log, but if you want to, this is how you can do it.
    Log.setFileName("HowTo_StaticEngineDriver.log");
    // Note there is a single singleton logger in Java, but a log per engine in C++
    // i.e. in Java, multiple engines will write to the same log, where as in C++, each engine will write to its own log
    // The listener and callback objects are unique to this engine
    
    // Create a BioGears Engine
    BioGearsScenarioExec bge = new BioGearsScenarioExec();
        
    // I am going to create a listener that will get any log messages (Info, Warnings, Errors, Fatal Errors)
    // that come from the engine. The default listener will just put them into the log file
    // If you want to do custom logic that occurs when the engine throws an error (or any other message type), just create a class just like this one
    bge.setListener(new MyListener());
    
    // Static Execution refers to fact that the engine is only going to execute and record outputs as defined in a predefined scenario file
    // You cannot request different data from the engine, and you cannot give any new actions to the engine.
    // You provide the name of the log file to associate with running this scenario,
    // the scenario file, 
    // the name and location of a results file
    // an optional callback that will be called so you can get the latest data values and do some custom logic
    Resources resources = new Resources();
    String xml = "";
    try {
      FileUtils.readFile( resources.findResource("Scenarios/Basic/Basic1.xml") );
    } catch ( FileNotFoundException e ) {
      Log.error("Unable to find Scenarios/Basic/Basic1.xml"); 
      return;
    }

    bge.runScenarioXML("./Scenarios/Basic/Basic1.log", xml,"./Scenarios/Basic/Basic1Results.csv", null);// No Callback, just write out the file
    
    // You could create and provide an SEScenario object as well
    SEScenario sce = new SEScenario(bge.substanceManager);
    sce.setName("HowTo_StaticEngine");
    sce.setDescription("Simple Scenario to demonstraight building a scenario by the CDM API");
    sce.getInitialParameters().setPatientFile("Standard.xml");
    // When filling out a data request, units are optional
    // The units will be set to whatever units the engine uses.
    SEPhysiologyDataRequest hr = new SEPhysiologyDataRequest();
    hr.setName("HeartRate");
    hr.setUnit(FrequencyUnit.Per_min.toString());
    sce.getDataRequests().getRequestedData().add(hr);
    SEPhysiologyDataRequest rr = new SEPhysiologyDataRequest();
    rr.setName("RespirationRate");
    rr.setUnit(FrequencyUnit.Per_min.toString());
    sce.getDataRequests().getRequestedData().add(rr);
    SEPhysiologyDataRequest tlv = new SEPhysiologyDataRequest();    
    tlv.setName("TotalLungVolume");
    tlv.setUnit(VolumeUnit.mL.toString());
    sce.getDataRequests().getRequestedData().add(tlv);
    // Let's just run for 2 minutes
    SEAdvanceTime adv = new SEAdvanceTime();
    adv.getTime().setValue(2,TimeUnit.min);
    sce.getActions().add(adv);
    bge.runScenario("./Scenarios/HowToStaticEngine.log",sce,"./Scenarios/HowToStaticEngineResults.xml", new MyCallback(bge,1.0));// Callback with new data every simulated second
    
    // Note if your engine is running in another thread, you can call bge.cancelScenario to halt the engine
    
    // Be nice to your memory and deallocate the native memory associated with this engine if you are done with it
    bge.cleanUp();
    // Note you can now run a static (scenario) or another dynamic engine with the bge object, it will allocate and manage a new C++ engine 
  }
}
