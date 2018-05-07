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

package mil.tatrc.physiology.datamodel.patient.conditions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.StarvationData;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;

public class Starvation extends SEPatientCondition
{
	protected SEScalarTime timeSinceMeal;
	
	public Starvation()
	{
		timeSinceMeal = null;
	}
	
	public void reset()
	{
		super.reset();
		if (timeSinceMeal != null)
			timeSinceMeal.invalidate();
	}
	
	public void copy(Starvation other)
	{
		if(this==other)
			return;
		super.copy(other);
		if (other.timeSinceMeal != null)
			getTimeSinceMeal().set(other.getTimeSinceMeal());
		else if (timeSinceMeal != null)
			timeSinceMeal.invalidate();
	}
	
	public boolean load(StarvationData in) 
	{
		super.load(in);
		getTimeSinceMeal().load(in.getTimeSinceMeal());
		return true;
	}
	
	public StarvationData unload() 
	{
		StarvationData to = CDMSerializer.objFactory.createStarvationData();
		unload(to);
		return to;
	}
	
	protected void unload(StarvationData data)
	{
		super.unload(data);
		if (timeSinceMeal != null)
			data.setTimeSinceMeal(timeSinceMeal.unload());
	}
	
	public boolean hasTimeSinceMeal()
	{
		return timeSinceMeal == null ? false : timeSinceMeal.isValid();
	}
	public SEScalarTime getTimeSinceMeal()
	{
		if (timeSinceMeal == null)
			timeSinceMeal = new SEScalarTime();
		return timeSinceMeal;
	}
	
	public String toString()
	{
		return "Starvation" 
				+ "\n\tTime since last meal: " + getTimeSinceMeal();
	}
	
}
