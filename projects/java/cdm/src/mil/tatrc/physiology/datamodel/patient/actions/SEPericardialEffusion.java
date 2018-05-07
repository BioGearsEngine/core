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

package mil.tatrc.physiology.datamodel.patient.actions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.PericardialEffusionData;
import mil.tatrc.physiology.datamodel.properties.SEScalarVolumePerTime;

public class SEPericardialEffusion extends SEPatientAction
{
	protected SEScalarVolumePerTime EffusionRate;
	
	public SEPericardialEffusion()
	{
		EffusionRate=null;
	}
	
	public void copy(SEPericardialEffusion other)
	{
		if(this==other)
			return;
		super.copy(other);
		if (other.EffusionRate != null)
			getEffusionRate().set(other.getEffusionRate());
		else if (EffusionRate != null)
			EffusionRate.invalidate();
	}
	
	public void reset()
	{
		super.reset();
		if (EffusionRate != null)
			EffusionRate.invalidate();
	}
	
	public boolean isValid()
	{
		return hasEffusionRate();
	}
	
	public boolean load(PericardialEffusionData in)
	{
		super.load(in);
		getEffusionRate().load(in.getEffusionRate());
		return isValid();
	}
	
	public PericardialEffusionData unload() 
	{
		PericardialEffusionData data = CDMSerializer.objFactory.createPericardialEffusionData();
		unload(data);
		return data;
	}
	
	protected void unload(PericardialEffusionData data)
	{
		super.unload(data);
		if (EffusionRate != null)
			data.setEffusionRate(EffusionRate.unload());
	}
	
	public boolean hasEffusionRate()
	{
		return EffusionRate == null ? false : EffusionRate.isValid();
	}
	public SEScalarVolumePerTime getEffusionRate()
	{
		if (EffusionRate == null)
			EffusionRate = new SEScalarVolumePerTime();
		return EffusionRate;
	}
	
	public String toString() 
	{
		if (EffusionRate != null)
			return "Pericardial Effusion" 
					+ "\n\tEffusion Rate: " + getEffusionRate(); 
		else
			return "Action not properly specified";
	}
}
