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

package mil.tatrc.physiology.datamodel.system.environment.conditions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.InitialEnvironmentData;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.datamodel.system.environment.SEEnvironmentalConditions;

public class InitialEnvironment extends SEEnvironmentCondition
{
  protected SEEnvironmentalConditions conditions;
  protected String                    conditionsFile;
  
  public InitialEnvironment()
  {
    this.conditions=new SEEnvironmentalConditions();
  }
  
  public InitialEnvironment(InitialEnvironment other)
  {
    this();
    copy(other);    
  }
  
  public void copy(InitialEnvironment other)
  {
    if(this==other)
      return;
    super.copy(other);
    this.conditions.copy(other.conditions);
    this.conditionsFile=other.conditionsFile;
  }
  
  public void reset()
  {
    super.reset();
    this.conditions.reset();
    this.conditionsFile="";
  }
  
  public boolean load(InitialEnvironmentData in, SESubstanceManager substances)
  {
    super.load(in);
    if(in.getConditions()!=null)
      this.conditions.load(in.getConditions(),substances);
    else if(in.getConditionsFile()!=null)
      this.conditionsFile=in.getConditionsFile();
    return true;
  }
  
  public InitialEnvironmentData unload()
  {
    InitialEnvironmentData data = CDMSerializer.objFactory.createInitialEnvironmentData();
    unload(data);
    return data;
  }
  
  protected void unload(InitialEnvironmentData data)
  {
    super.unload(data);
    if(this.hasConditions())
      data.setConditions(this.conditions.unload());
    else if(this.hasConditionsFile())
      data.setConditionsFile(this.conditionsFile);
  }
  
  public boolean hasConditions()
  {
    return this.conditions!=null;
  }
  public SEEnvironmentalConditions getConditions()
  {
    return this.conditions;
  }
  
  public boolean hasConditionsFile()
  {
    return this.conditionsFile!=null&&!this.conditionsFile.isEmpty();
  }
  public String getConditionsFile()
  {
    return this.conditionsFile;
  }
  public void setConditionsFile(String s)
  {
    this.conditionsFile = s;
  }
  
  public String toString()
  {
    if (conditions != null)
      return "Initial Environment : "+conditions.toString();
    else if(this.hasConditionsFile())
      return "Initial Environment:"
          + "\n\tConditionsFile: "+this.conditionsFile;
    else
      return "Condition not specified properly";
  }
}