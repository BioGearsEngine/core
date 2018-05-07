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

import java.io.FileNotFoundException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;

import java.util.ArrayList;
import java.util.List;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.ActionListData;
import mil.tatrc.physiology.datamodel.bind.BioGearsStateData;
import mil.tatrc.physiology.datamodel.bind.CompleteBloodCountData;
import mil.tatrc.physiology.datamodel.bind.ComprehensiveMetabolicPanelData;
import mil.tatrc.physiology.datamodel.bind.ConditionListData;
import mil.tatrc.physiology.datamodel.bind.EnumPatientAssessment;
import mil.tatrc.physiology.datamodel.bind.PulmonaryFunctionTestData;
import mil.tatrc.physiology.datamodel.bind.UrinalysisData;
import mil.tatrc.physiology.datamodel.patient.SEPatient;
import mil.tatrc.physiology.datamodel.patient.assessments.SECompleteBloodCount;
import mil.tatrc.physiology.datamodel.patient.assessments.SEComprehensiveMetabolicPanel;
import mil.tatrc.physiology.datamodel.patient.assessments.SEPatientAssessment;
import mil.tatrc.physiology.datamodel.patient.assessments.SEPulmonaryFunctionTest;
import mil.tatrc.physiology.datamodel.patient.assessments.SEUrinalysis;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.TimeUnit;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;
import mil.tatrc.physiology.datamodel.scenario.actions.SEAction;
import mil.tatrc.physiology.datamodel.scenario.conditions.SECondition;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEDataRequestManager;
import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Resources;
import mil.tatrc.physiology.utilities.Log;

public class BioGearsEngine extends BioGears
{
  protected boolean deadEngine = false;
  protected Resources m_resources = new Resources();
  public BioGearsEngine()
  {
    super();
  }
  
  public synchronized void reset()
  {
    super.reset();
    if(this.nativeObj!=0)
      this.nativeReset(this.nativeObj);
    this.deadEngine = false;
  }
  
  public synchronized boolean isDead()
  {
    return deadEngine;
  }

  // TODO Set a callback for patient events
  
  public synchronized boolean loadState(String logFile, String stateFile, SEDataRequestManager dataRequests)
  {  	
    return loadStateContents(logFile, stateFile, -1.0, dataRequests);
  }  
  public synchronized boolean loadState(String logFile, String stateFile, SEScalarTime simTime, SEDataRequestManager dataRequests)
  {  	  	
    return loadStateContents(logFile, stateFile, simTime.getValue(TimeUnit.s), dataRequests);
  }  
  protected synchronized boolean loadStateContents(String logFile, String stateFile, double simTime_s, SEDataRequestManager dataRequests)
  {
  	this.reset();
  	String dataRequestsXML = null;
    if(dataRequests !=null && !dataRequests.getRequestedData().isEmpty())
      dataRequestsXML = CDMSerializer.serialize(dataRequests.unload());
    if(dataRequestsXML == null)
    {
      Log.error("Invalid/No data requests provided");
      return false;
    }
    this.requestData(dataRequests);
  	this.nativeObj = nativeAllocate(logFile);
  	Path path = null;
    try {
      m_resources.findResource(stateFile);
    } catch ( FileNotFoundException e) {
      Log.error("State File : "+stateFile+" does not exist");
      return false;
    }

  	Object bind = null;
    try {
      CDMSerializer.readFile(m_resources.findResource(stateFile));
    } catch (FileNotFoundException e) {
      Log.error("Unable to find " + stateFile); 
      return false; 
    }

  	if(!(bind instanceof BioGearsStateData))
  	{
  		Log.error(stateFile +" is not an instance of BioGearsStateData"); 
  		return false;
  	}
  	return nativeLoadState(this.nativeObj, stateFile, simTime_s, dataRequestsXML);
  }
  
  public synchronized BioGearsStateData saveState(String stateFile)
  {
  	String xml = nativeSaveState(this.nativeObj, stateFile);
  	Object bind = CDMSerializer.serialize(xml);
  	if(bind instanceof BioGearsStateData)
  		return (BioGearsStateData)bind;
  	Log.error("State did not save");
  	return null;
  }

  public synchronized boolean initializeEngine(String logFile, SEPatient patient, List<SECondition> conditions, SEDataRequestManager dataRequests)
  {    
    this.reset();
    String patientXML = CDMSerializer.serialize(patient.unload());
    if(patientXML == null || patientXML.isEmpty())
    {
      Log.error("Invalid/No patient provided");
      return false;
    }
    String conditionsXML = null;
    if(conditions !=null && !conditions.isEmpty())
    {
      ConditionListData cData = CDMSerializer.objFactory.createConditionListData();
      for(SECondition c : conditions)
        cData.getCondition().add(c.unload());
      conditionsXML = CDMSerializer.serialize(cData);
    }
    String dataRequestsXML = null;
    if(dataRequests !=null && !dataRequests.getRequestedData().isEmpty())
      dataRequestsXML = CDMSerializer.serialize(dataRequests.unload());
    if(dataRequestsXML == null)
    {
      Log.error("Invalid/No data requests provided");
      return false;
    }
    this.requestData(dataRequests);
    this.nativeObj = nativeAllocate(logFile);
    this.deadEngine = !nativeInitializeEngine(this.nativeObj, patientXML, conditionsXML, dataRequestsXML);
    if(this.deadEngine)
    	Log.error("Unable to initialize engine");
    return !this.deadEngine;
  }
  
  public synchronized boolean initializeEngine(String logFile, String patientFile, List<SECondition> conditions, SEDataRequestManager dataRequests)
  {    
    this.reset();

    String patientXML = "";
    try {
      patientXML = FileUtils.readFile(m_resources.findResource(patientFile));
    } catch (FileNotFoundException e ) {
      Log.error("Invalid/No patient provided");
      return false;
    }


    String conditionsXML = null;
    if(conditions !=null && !conditions.isEmpty())
    {
      ConditionListData cData = CDMSerializer.objFactory.createConditionListData();
      for(SECondition c : conditions)
        cData.getCondition().add(c.unload());
      conditionsXML = CDMSerializer.serialize(cData);
    }
    String dataRequestsXML = null;
    if(dataRequests !=null && !dataRequests.getRequestedData().isEmpty())
      dataRequestsXML = CDMSerializer.serialize(dataRequests.unload());
    if(dataRequestsXML == null)
    {
      Log.error("Invalid/No data requests provided");
      return false;
    }
    this.requestData(dataRequests);
    this.nativeObj = nativeAllocate(logFile);
    this.deadEngine = !nativeInitializeEngine(this.nativeObj, patientXML, conditionsXML, dataRequestsXML);
    if(this.deadEngine)
    	Log.error("Unable to initialize engine");
    return !this.deadEngine;
  }
  
  public synchronized boolean advanceTime()
  {
    if(this.deadEngine)
    {
      Log.error("Engine has died");
      return false;
    }
    if(!nativeAdvanceTimeStep(this.nativeObj))
      deadEngine=true;
    return !deadEngine;
  }
  
  public synchronized boolean advanceTime(SEScalarTime time)
  {
    if(this.deadEngine)
    {
      Log.error("Engine has died");
      return false;
    }
    if(!nativeAdvanceTime(this.nativeObj, time.getValue(TimeUnit.s)))
      deadEngine=true;
    return !deadEngine;
  }
  
  public synchronized boolean processAction(SEAction action)
  {
    List <SEAction> actions = new ArrayList<SEAction>(); 
    actions.add(action);
    return processActions(actions);
  }
  
  public synchronized boolean processActions(List<SEAction> actions)
  {
    if(this.deadEngine)
    {
      Log.error("Engine has died");
      return false;
    }
    if(actions !=null && !actions.isEmpty())
    {
      ActionListData aData = CDMSerializer.objFactory.createActionListData();
      for(SEAction a : actions)
        aData.getAction().add(a.unload());
      String actionsXML = CDMSerializer.serialize(aData);
      if(!nativeProcessActions(this.nativeObj,actionsXML))
        deadEngine=true;
      return !deadEngine;
    }
    return true;
  }
  
  public synchronized boolean getPatientAssessment(SEPatientAssessment assessment)
  {
    if(this.deadEngine)
    {
      Log.error("Engine has died");
      return false;
    }
    if(assessment instanceof SEPulmonaryFunctionTest)
    {
      String xml = nativeGetAssessment(this.nativeObj, EnumPatientAssessment.PULMONARY_FUNCTION_TEST.ordinal());
      Object obj = CDMSerializer.serialize(xml);
      if(obj instanceof PulmonaryFunctionTestData)
      { 
        return ((SEPulmonaryFunctionTest)assessment).load((PulmonaryFunctionTestData)obj);
      }
      Log.error("XML could not be parsed by SEPulmonaryFunctionTest : "+xml);
      return false;
    }
    
    if(assessment instanceof SECompleteBloodCount)
    {
      String xml = nativeGetAssessment(this.nativeObj, EnumPatientAssessment.COMPLETE_BLOOD_COUNT.ordinal());
      Object obj = CDMSerializer.serialize(xml);
      if(obj instanceof CompleteBloodCountData)
      { 
        return ((SECompleteBloodCount)assessment).load((CompleteBloodCountData)obj);
      }
      Log.error("XML could not be parsed by SECompleteBloodCount : "+xml);
      return false;
    }
    
    if(assessment instanceof SEComprehensiveMetabolicPanel)
    {
      String xml = nativeGetAssessment(this.nativeObj, EnumPatientAssessment.COMPREHENSIVE_METABOLIC_PANEL.ordinal());
      Object obj = CDMSerializer.serialize(xml);
      if(obj instanceof ComprehensiveMetabolicPanelData)
      { 
        return ((SEComprehensiveMetabolicPanel)assessment).load((ComprehensiveMetabolicPanelData)obj);
      }
      Log.error("XML could not be parsed by SEComprehensiveMetabolicPanel : "+xml);
      return false;
    }
    
    if(assessment instanceof SEUrinalysis)
    {
      String xml = nativeGetAssessment(this.nativeObj, EnumPatientAssessment.URINALYSIS.ordinal());
      Object obj = CDMSerializer.serialize(xml);
      if(obj instanceof UrinalysisData)
      { 
        return ((SEUrinalysis)assessment).load((UrinalysisData)obj);        
      }
      Log.error("XML could not be parsed by SEUrinalysis : "+xml);
      return false;
    }
    
    return false;
  }
  
  /**
   * Used for C++ communication for calculation outside of the java language
   * @return - success flag from calculate
   */
  protected native void nativeReset(long nativeObj);
  
  protected native boolean nativeInitializeEngine(long nativeObj, String patientXML, String conditionsXML, String dataRequestsXML);
  protected native boolean nativeLoadState(long nativeObj, String stateFile, double simTime_s, String dataRequestsXML);// pass <0 as simTime to use the time in the file
  protected native String  nativeSaveState(long nativeObj, String stateFile);
  protected native boolean nativeAdvanceTimeStep(long nativeObj);
  protected native boolean nativeAdvanceTime(long nativeObj, double time_s);
  protected native boolean nativeProcessActions(long nativeObj, String actionsXML);
  protected native String nativeGetAssessment(long nativeObj, int type);
}
