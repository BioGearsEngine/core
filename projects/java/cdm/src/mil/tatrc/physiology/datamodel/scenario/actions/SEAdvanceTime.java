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

package mil.tatrc.physiology.datamodel.scenario.actions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.AdvanceTimeData;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;

public class SEAdvanceTime extends SEAction 
{
	protected SEScalarTime time;
	
	public SEAdvanceTime() 
	{
		time = null;
	}
	
	public void copy(SEAdvanceTime other)
	{
		if(this==other)
			return;
		super.copy(other);
	}
	
	public void reset() 
	{
		super.reset();
		if (hasTime())
			time.invalidate();
	}
	
	public boolean isValid()
	{
		return hasTime();
	}

	public boolean load(AdvanceTimeData in) 
	{
		super.load(in);
		getTime().load(in.getTime());
		return isValid();
	}
	
	public AdvanceTimeData unload()
	{
		AdvanceTimeData data = CDMSerializer.objFactory.createAdvanceTimeData();
		unload(data);
		return data;
	}
	
	protected void unload(AdvanceTimeData data)
	{
		super.unload(data);
		if (hasTime())
			data.setTime(this.time.unload());
	}
	
	public boolean hasTime()
	{
		return time == null ? false : time.isValid();
	}
	public SEScalarTime getTime() 
	{
		if (time == null)
			time = new SEScalarTime();
		return time;
	}
	
	public String toString() 
	{
		return "Advance Time" 
				+ "\n\tTime: " + getTime();
	}
}
