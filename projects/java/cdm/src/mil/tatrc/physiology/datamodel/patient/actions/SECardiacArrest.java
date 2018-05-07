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
import mil.tatrc.physiology.datamodel.bind.CardiacArrestData;

public class SECardiacArrest extends SEPatientAction
{
	protected EnumOnOff state;
	
	public SECardiacArrest()
	{
		state = null;
	}
	
	public void copy(SECardiacArrest other)
	{
		if(this==other)
			return;
		super.copy(other);
		state = other.state;
	}
	
	public void reset()
	{
		super.reset();
		state = null;
	}
	
	public boolean isValid()
	{
		return hasState();
	}
	
	public boolean load(CardiacArrestData in)
	{
		super.load(in);
		this.state = in.getState();
		return isValid();
	}
	
	public CardiacArrestData unload()
	{
		CardiacArrestData data = CDMSerializer.objFactory.createCardiacArrestData();
		unload(data);
		return data;
	}
	
	protected void unload(CardiacArrestData data)
	{
		super.unload(data);
		if (hasState())
			data.setState(state);
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
		return "Cardiac Arrest"
				+ "\n\tState: " + getState();
	}
}
