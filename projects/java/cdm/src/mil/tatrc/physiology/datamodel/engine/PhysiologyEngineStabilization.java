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

import mil.tatrc.physiology.datamodel.bind.EnumOnOff;
import mil.tatrc.physiology.datamodel.bind.PhysiologyEngineStabilizationData;

public abstract class PhysiologyEngineStabilization 
{
  protected EnumOnOff trackingStabilization;
  
  public PhysiologyEngineStabilization()
  {
    reset();
  }
  
  public void reset()
  {
    this.trackingStabilization=EnumOnOff.OFF;
  }
  
  public boolean load(PhysiologyEngineStabilizationData in) 
  {
    reset();
    this.trackingStabilization=in.getTrackingStabilization();
    return true;
  }
  
  public abstract PhysiologyEngineStabilizationData unload();
  
  protected void unload(PhysiologyEngineStabilizationData data)
  {
    data.setTrackingStabilization(this.trackingStabilization);
  }
  
  public EnumOnOff isTrackingStabilization()
  {
    return this.trackingStabilization;
  }
  public void TrackStabilization(EnumOnOff b)
  {
    this.trackingStabilization=b;
  }
  
}
