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

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.scenario.SEScenario;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEDataRequestManager;
import mil.tatrc.physiology.datamodel.utilities.SEEventHandler;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.LogListener;

/**
 * This is a class that will run a scenario through BioGears
 * and create it's results file. You could easily write your
 * own exec class that does what you want in between actions
 * but this may work for most of your needs.
 * @author abray
 */
public class BioGearsScenarioExec extends BioGears
{
  public void runScenario(String logFile, SEScenario scenario, String resultsFile, CDMUpdatedCallback callback)
  {  	
  	if(callback != null)
  		callback.drMgr = scenario.getDataRequests();  	
    runScenarioXML(logFile,CDMSerializer.serialize(scenario.unload()),resultsFile,callback);
  }
  
  public void runScenarioXML(String logFile, String scenarioXML, String resultsFile)
  {
  	runScenarioXML(logFile, scenarioXML, resultsFile, null);
  }
  
  protected void runScenarioXML(String logFile, String scenarioXML, String resultsFile, CDMUpdatedCallback callback)
  {    
    double callbackFreq_s = 0;
    if(callback!=null)
    { 
    	LogListener l = this.listener;
    	SEEventHandler eh = this.eventHandler;
      reset();// Only create our CDM objects if we have a callback to fill data out (also save off listner and event handler)
      this.listener = l;
      this.eventHandler = eh;
      this.cdmCallback = callback;
      callbackFreq_s = callback.callbackFrequency_s;
      this.requestData(callback.drMgr);
    }
    this.nativeObj = nativeAllocate(logFile);
    nativeExecuteScenario(this.nativeObj, scenarioXML, resultsFile, callbackFreq_s);
    nativeDelete(this.nativeObj);
    this.nativeObj=0;
    this.cdmCallback = null;
  }
  
  public void cancelScenario()
  {
    if(this.nativeObj!=0)
      nativeCancelScenario(this.nativeObj);
    Log.info("Cancelling execution");
  }
  
  protected native void nativeExecuteScenario(long nativeObj, String scenarioXML, String resultsFile, double updateFreq_s);
  protected native void nativeCancelScenario(long nativeObj);
  
}
