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
import mil.tatrc.physiology.datamodel.bind.ChestOcclusiveDressingData;
import mil.tatrc.physiology.datamodel.bind.EnumOnOff;
import mil.tatrc.physiology.datamodel.bind.EnumSide;

public class SEChestOcclusiveDressing extends SEPatientAction
{
	protected EnumSide side;
	protected EnumOnOff state;
	
	public SEChestOcclusiveDressing()
	{
		side = null;
		state = null;
	}
	
	public void copy(SEChestOcclusiveDressing other)
	{
		if(this==other)
			return;
		super.copy(other);
		this.state = other.state;
		this.side = other.side;
	}
	
	public void reset()
	{
		super.reset();
		state = null;
		side = null;
	}
	
	public boolean isValid()
	{
		return hasSide() && hasState();
	}
	
	public boolean load(ChestOcclusiveDressingData in)
	{
		super.load(in);
		state = in.getState();
		side = in.getSide();
		return isValid();
	}
	
	public ChestOcclusiveDressingData unload()
	{
		ChestOcclusiveDressingData data = CDMSerializer.objFactory.createChestOcclusiveDressingData();
		unload(data);
		return data;
	}
	
	protected void unload(ChestOcclusiveDressingData data)
	{
		super.unload(data);
		if (hasState())
			data.setState(state);
		if (hasSide())
			data.setSide(side);
	}
	
	public EnumSide getSide()
	{
		return side;
	}
	public void setSide(EnumSide leftOrRight)
	{
		side = leftOrRight;
	}
	public boolean hasSide()
	{
		return side == null ? false : true;
	}
	
	public EnumOnOff getState()
	{
		return state;
	}
	public void setState(EnumOnOff onOrOff)
	{
		state = onOrOff;
	}
	public boolean hasState()
	{
		return state == null ? false : true;
	}
	
	public String toString()
	{
		if (side != null)
			return "Chest Occlusive Dressing" 
					+ "\n\tState: " + getState()
					+ "\n\tSide: " + getSide();
		else
			return "Action not properly specified";
	}
}
