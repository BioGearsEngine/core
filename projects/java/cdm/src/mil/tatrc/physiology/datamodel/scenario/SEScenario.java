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

package mil.tatrc.physiology.datamodel.scenario;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Objects;
import java.util.Set;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.TimeUnit;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;
import mil.tatrc.physiology.datamodel.scenario.actions.SEAction;
import mil.tatrc.physiology.datamodel.scenario.actions.SEAdvanceTime;
import mil.tatrc.physiology.datamodel.scenario.datarequests.*;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.SimpleEquals;

public class SEScenario 
{
	public static void main(String[] args)
	{
		SESubstanceManager mgr = new SESubstanceManager();
		mgr.loadSubstanceDirectory();
		boolean onlyCheckSchema=true;
		
		String searchDir;
		if(args.length==0)
			searchDir="../verification/Scenarios";
		else
			searchDir=args[0];
		List<String> files=FileUtils.findFiles(searchDir, ".xml", true);
		for(String file : files)
		{
			Log.info("Testing scenario file "+file);
			Object obj = CDMSerializer.readFile(file);
			if(obj==null)
				Log.error("Unable to read file "+file);
			if(!(obj instanceof ScenarioData))
				continue;
			ScenarioData sce1Data = (ScenarioData) obj;

			SEScenario sce1 = new SEScenario(mgr);
			sce1.load(sce1Data);

			// Let's make sure we don't have the same data request more than once
			Set<Integer> hashes = new HashSet<Integer>();
			for(SEDataRequest dr : sce1.getDataRequests().getRequestedData())
			{
				int hash = dr.hashCode();				
				if(!hashes.contains(hash))
					hashes.add(hash);
				else
					Log.error("Duplicate Data Request");
			}
			
			if(!onlyCheckSchema)
			{
				ScenarioData sce2Data = sce1.unload();
				SEScenario sce2 = new SEScenario(mgr);
				sce2.load(sce2Data);

				CDMSerializer.serialize(sce2.unload());

				int numActions;

				numActions = sce1Data.getAction().size();
				if(numActions != sce2Data.getAction().size())
					Log.error("We have a xsd actions problem with : "+file);			
				for(int i=0; i<numActions; i++)
				{
					ActionData action1 = sce1Data.getAction().get(i);
					ActionData action2 = sce2Data.getAction().get(i);

					//Log.info("Comparing "+action1.getClass().getSimpleName());
					if(!SimpleEquals.Equals(action1, action2))
					{
						Log.error("We have an actions problem with : "+file);
						Log.error("Scenario 1 action is : "+action1);
						Log.error("Scenario 2 action is : "+action2);					
					}
				}

				// Now let's compare SEActions
				numActions = sce1.actions.size();
				if(numActions != sce2.actions.size())
					Log.error("We have an actions problem with : "+file);

				for(int i=0; i<numActions; i++)
				{
					String action1 = sce1.actions.get(i).toString();
					String action2 = sce2.actions.get(i).toString();

					if(!action1.equals(action2))
					{
						Log.error("We have an actions problem with : "+file);
						Log.error("Scenario 1 action is : "+action1);
						Log.error("Scenario 2 action is : "+action2);					
					}
				}
			}
		}
	}
	
	protected String                        name;
	protected String                        description;
	protected SEScenarioInitialParameters   params;
	protected SEScenarioAutoSerialization   autoSerialize;
	protected String                        engineState;
	protected SEDataRequestManager          dataRequests;
	protected List<SEAction>                actions;
	
	protected SESubstanceManager substances;
	
	public SEScenario(SESubstanceManager substances)
	{
	  this.substances = substances; 
	  
		this.name = "";
		this.description = "";
		this.params = null;		
		this.autoSerialize = null;
		this.engineState = null;
		this.dataRequests = new SEDataRequestManager();
		this.actions = new ArrayList<SEAction>();
	}
	
	public void reset() 
	{
	  this.name = "";
    this.description = "";
    this.params = null;
    this.autoSerialize = null;
    this.engineState = null;
    this.actions.clear();		
		this.dataRequests.reset();
	}
	
	public boolean load(ScenarioData in)
	{
		reset();
		
		this.name = in.getName();
		this.description = in.getDescription();
		if(in.getInitialParameters()!=null)
		  getInitialParameters().load(in.getInitialParameters(), this.substances);
		if(in.getAutoSerialization()!=null)
			getAutoSerialization().load(in.getAutoSerialization());
		if(in.getDataRequests()!=null)
			this.dataRequests.load(in.getDataRequests(), this.substances);
		else if(in.getEngineStateFile() != null)
			this.engineState = in.getEngineStateFile();
			
		try
		{
			double scenarioTime_s = 0;
			SEAdvanceTime advTime = new SEAdvanceTime();
			
			for (ActionData actionData : in.getAction())
			{		
				SEAction a = SEAction.bind2CDM(actionData,this.substances);
				if(a==null)
				{
					Log.error("Unable to load action "+actionData.getClass().getName());
					return false;
				}
				else
				{
					this.actions.add(a);
					a.getScenarioTime().setValue(scenarioTime_s,TimeUnit.s);
					
					if(actionData instanceof AdvanceTimeData)
					{
						advTime.load((AdvanceTimeData)actionData);						
						scenarioTime_s += advTime.getTime().getValue(TimeUnit.s);
					}					
				}
			}
		}
		catch (Exception e)
		{
			Log.fatal("Unsupported Action type found in scenario file", e);
		}
		return isValid();
	}
	
	public ScenarioData unload()
	{
		ScenarioData data = CDMSerializer.objFactory.createScenarioData();
		unload(data);
		return data;
	}
	
	protected void unload(ScenarioData data)
	{
		if(!isValid())
		{
			Log.error("Scenario Invalid, cannot unload to a valid jaxb object");
			return;
		}
		data.setName(this.name);
		data.setDescription(this.description);
		if(hasInitialParameters())
		  data.setInitialParameters(params.unload());
		if(hasAutoSerialization())
			data.setAutoSerialization(autoSerialize.unload());
		else if(hasEngineState())
			data.setEngineStateFile(this.engineState);
		if(!this.dataRequests.getRequestedData().isEmpty())
			data.setDataRequests(this.dataRequests.unload());
		for(SEAction a : this.actions)
			data.getAction().add(a.unload());
	}
	
	public boolean isValid()
	{
		if (actions.size() == 0)
			return false;
		if(!hasInitialParameters() && !hasEngineState())
		  return false;
		return true;
	}

	public String getName()
	{
		return name;
	}
	public void setName(String name)
	{
		this.name = name;
	}
	public boolean hasName()
	{
		return name.isEmpty() ? false : true;
	}
	public void invalidateName()
	{
		name = "";
	}
	
	public String getDescription()
	{
		return description;
	}
	public void setDescription(String description)
	{
		this.description = description;
	}
	public boolean hasDescription()
	{
		return description.isEmpty() ? false : true;
	}
	public void invalidateDescription()
	{
		description = "";
	}
	
	public boolean hasEngineState()
	{
		if(hasInitialParameters())
			return false;
		return this.engineState != null && !this.engineState.isEmpty();
	}
	public void setEngineState(String stateFile)
	{
		invalidateInitialParameters();
		this.engineState = stateFile;
	}
	public String getEngineState(){ return this.engineState; }
	public void invalidateEngineState()
	{
		this.engineState = null;
	}
	
	public boolean hasInitialParameters()
  {
    return params!=null && params.isValid();
  }
  public SEScenarioInitialParameters getInitialParameters()
  {
    if(this.params==null)
      this.params=new SEScenarioInitialParameters();
    return this.params;
  }
  public void invalidateInitialParameters()
  {
  	this.params = null;
  }
  
  public boolean hasAutoSerialization()
  {
    return autoSerialize!=null && autoSerialize.isValid();
  }
  public SEScenarioAutoSerialization getAutoSerialization()
  {
    if(this.autoSerialize==null)
      this.autoSerialize=new SEScenarioAutoSerialization();
    return this.autoSerialize;
  }
  public void invalidateAutoSerialization()
  {
  	this.autoSerialize = null;
  }

	public List<SEAction> getActions() 
	{
		return actions;
	}

	public SEDataRequestManager getDataRequests() 
	{
		return this.dataRequests;
	}
	
	public SESubstanceManager getSubstanceManager()
	{
		return this.substances;
	}
}
