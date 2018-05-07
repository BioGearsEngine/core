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


import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;

public class SEScenarioAutoSerialization 
{
  protected SEScalarTime period;
  protected EnumOnOff    periodTimeStamps;
  protected EnumOnOff    afterActions;
  protected EnumOnOff    reloadState;
  protected String       directory;
  protected String       filename;
  
  public SEScenarioAutoSerialization()
  {
    this.period = null;
    this.periodTimeStamps = null;
    this.afterActions = null;
    this.reloadState = null;
    this.directory = null;
    this.filename = null;
  }
  
  public void reset() 
  {
  	this.period = null;
    this.periodTimeStamps = null;
    this.afterActions = null;
    this.reloadState = null;
    this.directory = null;
    this.filename = null;
  }
  
  public boolean load(ScenarioAutoSerializationData in)
  {
    reset();
    
    getPeriod().load(in.getPeriod());
    setPeriodTimeStamps(in.getPeriodTimeStamps());
    setAfterActions(in.getAfterActions());
    setReloadState(in.getReloadState());
    setDirectory(in.getDirectory());
    setFileName(in.getFileName());
    
    return isValid();
  }
  
  public ScenarioAutoSerializationData unload()
  {
  	ScenarioAutoSerializationData data = CDMSerializer.objFactory.createScenarioAutoSerializationData();
    unload(data);
    return data;
  }  
  protected void unload(ScenarioAutoSerializationData data)
  {
  	if (hasPeriod())
      data.setPeriod(period.unload());
    if (hasPeriodTimeStamps())
      data.setPeriodTimeStamps(periodTimeStamps);
    if (hasAfterActions())
      data.setAfterActions(afterActions);
    if (hasReloadState())
      data.setReloadState(reloadState);
    if (hasDirectory())
      data.setDirectory(directory);
    if (hasFileName())
      data.setFileName(filename);
  }
  
  public boolean isValid()
  {
  	if (!hasPeriod())
  		return false;
  	if (period.getValue()<=0)
  		return false;
  	if (!hasPeriodTimeStamps())
  		return false;
  	if (!hasAfterActions())
  		return false;
  	if (!hasReloadState())
  		return false;
  	if (!hasDirectory())
  		return false;
  	if (!hasFileName())
  		return false;
  	return true;
  }

  public boolean hasPeriod()
  {
    return period == null ? false : period.isValid();
  }
  public SEScalarTime getPeriod()
  {
    if (period == null)
    	period = new SEScalarTime();
    return period;
  }
  
  public EnumOnOff getPeriodTimeStamps()
  {
    return periodTimeStamps;
  }
  public void setPeriodTimeStamps(EnumOnOff v)
  {
    this.periodTimeStamps = v;
  }
  public boolean hasPeriodTimeStamps()
  {
    return (periodTimeStamps==null) ? false : true;
  }
  public void invalidatePeriodTimeStamps()
  {
  	periodTimeStamps = null;
  }
  
  public EnumOnOff getAfterActions()
  {
    return afterActions;
  }
  public void setAfterActions(EnumOnOff v)
  {
    this.afterActions = v;
  }
  public boolean hasAfterActions()
  {
    return (afterActions==null) ? false : true;
  }
  public void invalidateAfterActions()
  {
  	afterActions = null;
  }
  
  public EnumOnOff getReloadState()
  {
    return reloadState;
  }
  public void setReloadState(EnumOnOff v)
  {
    this.reloadState = v;
  }
  public boolean hasReloadState()
  {
    return (reloadState==null) ? false : true;
  }
  public void invalidateReloadState()
  {
  	reloadState = null;
  }

  public String getDirectory()
  {
    return directory;
  }
  public void setDirectory(String dir)
  {
    this.directory = dir;
  }
  public boolean hasDirectory()
  {
    return (directory==null || directory.isEmpty()) ? false : true;
  }
  public void invalidateDirectory()
  {
    directory = "";
  }
 
  public String getFileName()
  {
    return filename;
  }
  public void setFileName(String name)
  {
    this.filename = name;
  }
  public boolean hasFileName()
  {
    return (filename==null || filename.isEmpty()) ? false : true;
  }
  public void invalidateFileName()
  {
  	filename = "";
  }
}
