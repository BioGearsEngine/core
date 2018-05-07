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
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.properties.SEScalarFraction;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;

public class SEBreathHold extends SEConsciousRespirationCommand
{
	protected SEScalarTime period;
	
	public SEBreathHold()
	{
		period = null;
	}
	
	public void copy(SEBreathHold other)
	{
		if(this==other)
			return;

		period = other.period;
	}
	
	public SEConsciousRespirationCommand getCopy()
	{
		SEConsciousRespirationCommand command = new SEBreathHold();
		command.copy(this);
		
		return command;
	}
	
	public void reset()
	{
		super.reset();

		if (period != null)
			period.invalidate();
	}
	
	public boolean isValid()
	{
		return hasPeriod();
	}
	
	public boolean load(BreathHoldData in)
	{
		super.load(in);

		getPeriod().load(in.getPeriod());
		
		return isValid();
	}
	
	public BreathHoldData unload()
	{
		BreathHoldData data = CDMSerializer.objFactory.createBreathHoldData();
		unload(data);
		
		return data;
	}
	
	protected void unload(BreathHoldData data)
	{
		super.unload(data);

		if (hasPeriod())
			data.setPeriod(period.unload());
	}
	
	public boolean hasPeriod()
	{
		return period != null;
	}
	public SEScalarTime getPeriod()
	{
		if (period == null)
			period = new SEScalarTime();
		return period;
	}
	
	public String toString()
	{
		if (period != null)
		{
			return "Breath Hold"
					+ "\n\tPeriod: " + getPeriod();
		}
		else
			return "Action not specified properly";
	}
}
