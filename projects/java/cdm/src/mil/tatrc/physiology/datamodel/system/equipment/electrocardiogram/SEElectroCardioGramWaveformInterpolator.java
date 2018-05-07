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

import java.util.*;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.ElectroCardioGramInterpolationWaveformData;
import mil.tatrc.physiology.datamodel.bind.ElectroCardioGramWaveformInterpolatorData;
import mil.tatrc.physiology.datamodel.bind.EnumHeartRhythm;

public class SEElectroCardioGramWaveformInterpolator
{
  Map<Integer,Map<EnumHeartRhythm,SEElectroCardioGramInterpolationWaveform>> waveforms = new HashMap<Integer, Map<EnumHeartRhythm,SEElectroCardioGramInterpolationWaveform>>();
 
  public SEElectroCardioGramWaveformInterpolator()
  {
    reset();
  }
  
  public void reset()
  {
    waveforms.clear();
  }
  
  public boolean load(ElectroCardioGramWaveformInterpolatorData in)
  {
    reset();
    for(ElectroCardioGramInterpolationWaveformData wData : in.getWaveform())
    {
      SEElectroCardioGramInterpolationWaveform w = new SEElectroCardioGramInterpolationWaveform();
      w.load(wData);
      Map<EnumHeartRhythm, SEElectroCardioGramInterpolationWaveform> leads = this.waveforms.get(w.getLead());
      if(leads == null)
      {
      	leads = new HashMap<EnumHeartRhythm, SEElectroCardioGramInterpolationWaveform>();
      	this.waveforms.put(w.getLead(), leads);
      }
      leads.put(w.getRhythm(), w);
    }
    return true;
  }
  
  public ElectroCardioGramWaveformInterpolatorData unload()
  {
  	ElectroCardioGramWaveformInterpolatorData to =  CDMSerializer.objFactory.createElectroCardioGramWaveformInterpolatorData();
    unload(to);
    return to;
  }
  
  protected void unload(ElectroCardioGramWaveformInterpolatorData to)
  {
    for(Map<EnumHeartRhythm, SEElectroCardioGramInterpolationWaveform> leads : waveforms.values())
    {     
    	for(SEElectroCardioGramInterpolationWaveform w : leads.values())
    		to.getWaveform().add(w.unload());
    }
  }
  
  public SEElectroCardioGramInterpolationWaveform getWaveform(int lead, EnumHeartRhythm rhythm)
  {
  	Map<EnumHeartRhythm, SEElectroCardioGramInterpolationWaveform> leads = this.waveforms.get(lead);
  	if(leads == null)
  		return null;
    return leads.get(rhythm);
  }
}
