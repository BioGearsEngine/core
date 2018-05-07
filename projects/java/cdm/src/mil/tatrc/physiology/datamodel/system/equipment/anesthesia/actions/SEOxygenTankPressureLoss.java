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

package mil.tatrc.physiology.datamodel.system.equipment.anesthesia.actions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.EnumOnOff;
import mil.tatrc.physiology.datamodel.bind.OxygenTankPressureLossData;

public class SEOxygenTankPressureLoss extends SEAnesthesiaMachineAction
{
	protected EnumOnOff state;
	
	public SEOxygenTankPressureLoss()
	{
		state = EnumOnOff.ON;
	}
	
	public void reset()
	{
		super.reset();
		state = EnumOnOff.ON;
	}
	
	public boolean isValid()
	{
		return hasState();
	}
	
	public boolean load(OxygenTankPressureLossData in)
	{
		super.load(in);
		if (in.getState() != null)
			setState(in.getState());
		return isValid();
	}
	
	public OxygenTankPressureLossData unload()
	{
		OxygenTankPressureLossData data = CDMSerializer.objFactory.createOxygenTankPressureLossData();
		unload(data);
		return data;
	}
	
	protected void unload(OxygenTankPressureLossData data)
	{
		super.unload(data);
		if (hasState())
			data.setState(state);
	}
	
	/*
	 * State
	 */
	public EnumOnOff getState()
	{
		return state;
	}
	public void setState(EnumOnOff state)
	{
		this.state = state;
	}
	public boolean hasState()
	{
		return state == null ? false : true;
	}
	public void invalidateState()
	{
		state = null;
	}
	
	public String toString()
	{
		return "Oxygen Tank Pressure Loss"
				+ "\n\tState: " + getState();
	}
}
