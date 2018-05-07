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
import mil.tatrc.physiology.datamodel.bind.DehydrationData;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;

public class Dehydration extends SEPatientCondition
{
	protected SEScalar0To1 DehydrationFraction;
	
	public Dehydration()
	{
		DehydrationFraction = null;
	}
	
	public void reset()
	{
		super.reset();
		if (DehydrationFraction != null)
			DehydrationFraction.invalidate();
	}
	
	public void copy(Dehydration other)
	{
		if(this==other)
			return;
		super.copy(other);
		if (other.DehydrationFraction != null)
			getDehydrationFraction().set(other.getDehydrationFraction());
		else if (DehydrationFraction != null)
			DehydrationFraction.invalidate();
	}
	
	public boolean load(DehydrationData in) 
	{
		super.load(in);
		getDehydrationFraction().load(in.getDehydrationFraction());
		return true;
	}
	
	public DehydrationData unload() 
	{
		DehydrationData to = CDMSerializer.objFactory.createDehydrationData();
		unload(to);
		return to;
	}
	
	protected void unload(DehydrationData data)
	{
		super.unload(data);
		if (DehydrationFraction != null)
			data.setDehydrationFraction(DehydrationFraction.unload());
	}
	
	public boolean hasDehydrationFraction()
	{
		return DehydrationFraction == null ? false : DehydrationFraction.isValid();
	}
	public SEScalar0To1 getDehydrationFraction()
	{
		if (DehydrationFraction == null)
			DehydrationFraction = new SEScalar0To1();
		return DehydrationFraction;
	}
	
	public String toString()
	{
		return "Dehydration" 
				+ "\n\tFractional amount of body weight change due to fluid loss: " + getDehydrationFraction();
	}

	
	
}
