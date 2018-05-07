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
package mil.tatrc.physiology.datamodel.engine;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.ElectroCardioGramWaveformInterpolatorData;
import mil.tatrc.physiology.datamodel.bind.PhysiologyEngineConfigurationData;
import mil.tatrc.physiology.datamodel.bind.PhysiologyEngineDynamicStabilizationData;
import mil.tatrc.physiology.datamodel.bind.PhysiologyEngineStabilizationData;
import mil.tatrc.physiology.datamodel.bind.PhysiologyEngineTimedStabilizationData;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;
import mil.tatrc.physiology.datamodel.system.equipment.electrocardiogram.SEElectroCardioGramWaveformInterpolator;
import mil.tatrc.physiology.utilities.Log;

public class PhysiologyEngineConfiguration
{
  protected SEElectroCardioGramWaveformInterpolator  ecgInterpolator;
  protected PhysiologyEngineStabilization stabilizationCriteria;
  protected SEScalarTime                  timeStep;
  
  public PhysiologyEngineConfiguration()
  {
    clear();
  }
  
  public void reset()
  {
    if(ecgInterpolator!=null)
      this.ecgInterpolator.reset();
    if(stabilizationCriteria!=null)
      this.stabilizationCriteria.reset();
    if(timeStep!=null)
      this.timeStep.invalidate();
  }
  
  public void clear()
  {
    this.ecgInterpolator=null;
    this.stabilizationCriteria=null;   
    this.timeStep=null;
  }
  
  public boolean load(PhysiologyEngineConfigurationData in)
  {
    clear();
    
    if(in.getElectroCardioGramInterpolatorFile()!=null)
      getECGInterpolator().load((ElectroCardioGramWaveformInterpolatorData) CDMSerializer.readFile(in.getElectroCardioGramInterpolatorFile()));
    else if(in.getElectroCardioGramInterpolator()!=null)
    	getECGInterpolator().load(in.getElectroCardioGramInterpolator());

    PhysiologyEngineStabilizationData stabs=null;
    if(in.getStabilizationCriteria()!=null)
      stabs = in.getStabilizationCriteria();
    else if(in.getStabilizationCriteriaFile()!=null)
      stabs = (PhysiologyEngineStabilizationData)CDMSerializer.readFile(in.getStabilizationCriteriaFile());
    if(stabs != null)
    {
      if(stabs instanceof PhysiologyEngineTimedStabilizationData)
        stabilizationCriteria = new PhysiologyEngineTimedStabilization();
      else if(stabs instanceof PhysiologyEngineDynamicStabilizationData)
        stabilizationCriteria = new PhysiologyEngineDynamicStabilization();
      else
        Log.fatal("Unsupported stabilization critera : " + stabs.getClass().getName());
      stabilizationCriteria.load(stabs);
    }
    
    if(in.getTimeStep()!=null)
      getTimeStep().load(in.getTimeStep());
    return true;
  }
  
  public PhysiologyEngineConfigurationData unload()
  {
    PhysiologyEngineConfigurationData data = CDMSerializer.objFactory.createPhysiologyEngineConfigurationData();
    unload(data);
    return data;
  }
  
  protected void unload(PhysiologyEngineConfigurationData data)
  {
    if(hasECGInterpolator())
      data.setElectroCardioGramInterpolator(this.ecgInterpolator.unload());
    if(hasStabilizationCriteria())
      data.setStabilizationCriteria(this.stabilizationCriteria.unload());
    if(hasTimeStep())
      data.setTimeStep(this.timeStep.unload());
  }
  
  public boolean hasECGInterpolator()
  {
    return ecgInterpolator!=null;
  }
  public SEElectroCardioGramWaveformInterpolator getECGInterpolator()
  {
    if(ecgInterpolator==null)
      ecgInterpolator=new SEElectroCardioGramWaveformInterpolator();
    return ecgInterpolator;
  }
  
  public boolean hasStabilizationCriteria()
  {
    return stabilizationCriteria!=null;
  }
  public PhysiologyEngineStabilization getStabilizationCriteria()
  {
    return stabilizationCriteria;
  }

  public boolean hasTimeStep()
  {
    return timeStep!=null && timeStep.isValid();
  }
  public SEScalarTime getTimeStep()
  {
    if(timeStep==null)
      timeStep=new SEScalarTime();
    return timeStep;
  }
}
