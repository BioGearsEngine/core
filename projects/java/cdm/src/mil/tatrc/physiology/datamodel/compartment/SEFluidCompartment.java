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

package mil.tatrc.physiology.datamodel.compartment;

import mil.tatrc.physiology.datamodel.bind.FluidCompartmentData;
import mil.tatrc.physiology.datamodel.properties.SEScalarPressure;
import mil.tatrc.physiology.datamodel.properties.SEScalarVolume;
import mil.tatrc.physiology.datamodel.properties.SEScalarVolumePerTime;

public abstract class SEFluidCompartment extends SECompartment
{
  protected SEScalarVolumePerTime inFlow;
  protected SEScalarVolumePerTime outFlow;
	protected SEScalarPressure pressure;	
	protected SEScalarVolume volume;
	
	public SEFluidCompartment()
	{
	  super();
	  
	}
	
	public void reset()
	{
	  super.reset();
		if (pressure != null)
			pressure.invalidate();
		if (inFlow != null)
		  inFlow.invalidate();
	  if (outFlow != null)
	    outFlow.invalidate();
		if (volume != null)
			volume.invalidate();
	}
	
	public boolean load(FluidCompartmentData in)
	{
		super.load(in);
		if (in.getPressure() != null) 
		   getPressure().load(in.getPressure()); 
		if (in.getInFlow() != null) 
		   getInFlow().load(in.getInFlow()); 
		if (in.getOutFlow() != null) 
      getOutFlow().load(in.getOutFlow()); 
		if (in.getVolume() != null) 
		   getVolume().load(in.getVolume()); 
		
		return true;
	}
	
	protected void unload(FluidCompartmentData data)
	{
	  super.unload(data);
		if (pressure != null)
			data.setPressure(pressure.unload());
		if (inFlow != null)
			data.setInFlow(inFlow.unload());
		if (outFlow != null)
      data.setOutFlow(outFlow.unload());
		if (volume != null)
			data.setVolume(volume.unload());
	}
	
	public SEScalarVolumePerTime getInFlow() 
	{
		if (inFlow == null)
		  inFlow = new SEScalarVolumePerTime();
		return inFlow;
	}
	public boolean hasInFlow()
	{
		return inFlow == null ? false : inFlow.isValid();
	}
	
	public SEScalarVolumePerTime getOutFlow() 
  {
    if (outFlow == null)
      outFlow = new SEScalarVolumePerTime();
    return outFlow;
  }
  public boolean hasOutFlow()
  {
    return outFlow == null ? false : outFlow.isValid();
  }
  
  public SEScalarPressure getPressure() 
  {
    if (pressure == null)
      pressure = new SEScalarPressure();
    return pressure;
  }
  public boolean hasPressure()
  {
    return pressure == null ? false : pressure.isValid();
  }
	
	public SEScalarVolume getVolume() 
	{
		if (volume == null)
			volume = new SEScalarVolume();
		return volume;
	}
	public boolean hasVolume()
	{
		return volume == null ? false : volume.isValid();
	}
}
