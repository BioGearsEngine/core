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
import mil.tatrc.physiology.datamodel.bind.EnumOnOff;
import mil.tatrc.physiology.datamodel.bind.EnumSide;
import mil.tatrc.physiology.datamodel.bind.NeedleDecompressionData;

public class SENeedleDecompression extends SEPatientAction
{
	protected EnumOnOff state;
	protected EnumSide side;
	
	public SENeedleDecompression()
	{
		state = null;
		side = null;
	}
	
	public void copy(SENeedleDecompression other)
	{
		if(this==other)
			return;
		super.copy(other);
		state = other.state;
		side = other.side;
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
	
	public boolean load(NeedleDecompressionData in)
	{
		super.load(in);
		this.state = in.getState();
		this.side = in.getSide();
		return true;
	}
	
	public NeedleDecompressionData unload()
	{
		NeedleDecompressionData data = CDMSerializer.objFactory.createNeedleDecompressionData();
		unload(data);
		return data;
	}
	
	protected void unload(NeedleDecompressionData data)
	{
		super.unload(data);
		if (hasState())
			data.setState(state);
		if (hasSide())
			data.setSide(side);
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
	
	public EnumSide getSide()
	{
		return side;
	}
	
	public void setSide(EnumSide rightOrLeft)
	{
		side = rightOrLeft;
	}
	
	public boolean hasSide()
	{
		return side == null ? false : true;
	}
	
	public String toString()
	{
		return "Needle Decompression"
				+ "\n\tState: " + getState()
		    + "\n\tSide: " + getSide();
	}
}
