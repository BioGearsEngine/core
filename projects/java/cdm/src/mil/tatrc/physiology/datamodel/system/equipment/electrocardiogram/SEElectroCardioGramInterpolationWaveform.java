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
package mil.tatrc.physiology.datamodel.system.equipment.electrocardiogram;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.ElectroCardioGramInterpolationWaveformData;
import mil.tatrc.physiology.datamodel.bind.EnumHeartRhythm;
import mil.tatrc.physiology.datamodel.properties.SEFunctionElectricPotentialVsTime;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;
import mil.tatrc.physiology.utilities.Log;

public class SEElectroCardioGramInterpolationWaveform
{
	protected int lead;
  protected EnumHeartRhythm                   rhythm;
  protected SEFunctionElectricPotentialVsTime data;
  protected SEScalarTime                      timeStep;
  
  public SEElectroCardioGramInterpolationWaveform()
  {
    reset();
  }
  
  public void reset()
  {
  	lead = 0;
    rhythm = null;
    data = null;
    timeStep = null;
  }
  
  public boolean load(ElectroCardioGramInterpolationWaveformData in)
  {
    reset();
    setRhythm(in.getRhythm());
    setLead(in.getLead());
    getData().load(in.getData());
    if(in.getTimeStep()!=null)
      getTimeStep().load(in.getTimeStep()); 
    return true;
  }
  
  public ElectroCardioGramInterpolationWaveformData unload()
  {
  	ElectroCardioGramInterpolationWaveformData to =  CDMSerializer.objFactory.createElectroCardioGramInterpolationWaveformData();
    unload(to);
    return to;
  }
  
  protected void unload(ElectroCardioGramInterpolationWaveformData to)
  {
  	to.setLead(lead);
    to.setRhythm(rhythm);
    to.setData(data.unload());
    if(hasTimeStep())
      to.setTimeStep(timeStep.unload());
  }
  
  public int getLead()
  {
    return lead;
  }
  public void setLead(int l)
  {
  	if(l>0 && l<=12)
  		this.lead = l;
  	Log.error("Lead must be between 1 and 12");
  }
  public boolean hasLead()
  {
    return lead != 0;
  }
  public void removeLead()
  {
  	this.lead = 0;
  }
  
  public EnumHeartRhythm getRhythm()
  {
    return rhythm;
  }
  public void setRhythm(EnumHeartRhythm r)
  {
    this.rhythm = r;
  }
  public boolean hasRhythm()
  {
    return rhythm == null ? false : true;
  }
  
  public boolean hasData()
  {
    return data == null ? false : data.isValid();
  }
  public SEFunctionElectricPotentialVsTime getData()
  {
    if (data == null)
      data = new SEFunctionElectricPotentialVsTime();
    return data;
  }

  public boolean hasTimeStep()
  {
    return timeStep == null ? false : timeStep.isValid();
  }
  public SEScalarTime getTimeStep()
  {
    if (timeStep == null)
      timeStep = new SEScalarTime();
    return timeStep;
  }
}
