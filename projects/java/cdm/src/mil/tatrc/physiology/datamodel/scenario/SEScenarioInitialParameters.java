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

import java.util.ArrayList;
import java.util.List;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.engine.PhysiologyEngineConfiguration;
import mil.tatrc.physiology.datamodel.patient.SEPatient;
import mil.tatrc.physiology.datamodel.scenario.conditions.SECondition;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.utilities.Log;

public class SEScenarioInitialParameters 
{
  protected PhysiologyEngineConfiguration config;
  protected SEPatient                     patient;
  protected String                        patientFile;
  protected List<SECondition>             conditions;
  
  public SEScenarioInitialParameters()
  {
    this.config = null;
    this.patient = null;
    this.patientFile = "";
    this.conditions = new ArrayList<SECondition>();
  }
  
  public void reset() 
  {
    config=null;
    patient=null;
    patientFile = "";
    conditions.clear();
  }
  
  public boolean load(ScenarioInitialParametersData in, SESubstanceManager substances)
  {
    reset();
    
    if(in.getConfiguration()!=null)
      getConfiguration().load(in.getConfiguration());
    if(in.getPatient()!=null)
      this.getPatient().load(in.getPatient());
    if(in.getPatientFile()!=null)
      this.patientFile = in.getPatientFile();    
    
    // Conditions
    try
    {
      for (ConditionData conditionData : in.getCondition())
      {   
        SECondition c = SECondition.bind2CDM(conditionData, substances);
        if(c==null)
        {
          Log.error("Unable to load action "+conditionData.getClass().getName());
          return false;
        }
        else
          this.conditions.add(c);
      }
    }
    catch (Exception e)
    {
      Log.fatal("Unsupported Action type found in scenario file", e);
    }
    
    return isValid();
  }
  
  public ScenarioInitialParametersData unload()
  {
    ScenarioInitialParametersData data = CDMSerializer.objFactory.createScenarioInitialParametersData();
    unload(data);
    return data;
  }
  
  protected void unload(ScenarioInitialParametersData data)
  {
    if(!isValid())
    {
      Log.error("Scenario Invalid, cannot unload to a valid jaxb object");
      return;
    }
    if(hasConfiguration())
      data.setConfiguration(config.unload());
    if(hasPatient())
      data.setPatient(this.patient.unload());
    if(hasPatientFile())
      data.setPatientFile(this.patientFile);
    for(SECondition c : this.conditions)
      data.getCondition().add(c.unload());    
  }
  
  public boolean isValid()
  {
    if (!hasPatientFile()&&!hasPatient())
      return false;
   return true;
  }

  public boolean hasConfiguration()
  {
    return config!=null;
  }
  public PhysiologyEngineConfiguration getConfiguration()
  {
    if(this.config==null)
      this.config=new PhysiologyEngineConfiguration();
    return this.config;
  }
  
  public SEPatient getPatient()
  {
    return patient;
  }
  public void setPatient(SEPatient patient)
  {
    this.patient = patient;
    this.patientFile = "";
  }
  public boolean hasPatient()
  {
    return patient==null ? false : true;
  }
  public void invalidatePatient()
  {
    patient = null;
  }

  public String getPatientFile()
  {
    return patientFile;
  }
  public void setPatientFile(String patientFile)
  {
    this.patient = null;
    this.patientFile = patientFile;
  }
  public boolean hasPatientFile()
  {
    return patientFile.isEmpty() ? false : true;
  }
  public void invalidatePatientFile()
  {
    patientFile = "";
  }
  
  public List<SECondition> getConditions() 
  {
    return conditions;
  }
}
