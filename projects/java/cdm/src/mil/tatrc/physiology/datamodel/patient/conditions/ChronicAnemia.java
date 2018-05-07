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
import mil.tatrc.physiology.datamodel.bind.ChronicAnemiaData;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;

public class ChronicAnemia extends SEPatientCondition
{
	protected SEScalar0To1 reductionFactor;
	
	public ChronicAnemia()
	{
		reductionFactor = null;
	}
	
	public void reset()
	{
		super.reset();
		if (reductionFactor != null)
			reductionFactor.invalidate();
	}
	
	public void copy(ChronicAnemia other)
	{
		if(this==other)
			return;
		super.copy(other);
		if (other.reductionFactor != null)
			getReductionFactor().set(other.getReductionFactor());
		else if (reductionFactor != null)
			reductionFactor.invalidate();
	}
	
	public boolean load(ChronicAnemiaData in) 
	{
		super.load(in);
		getReductionFactor().load(in.getReductionFactor());
		return true;
	}
	
	public ChronicAnemiaData unload() 
	{
		ChronicAnemiaData to = CDMSerializer.objFactory.createChronicAnemiaData();
		unload(to);
		return to;
	}
	
	protected void unload(ChronicAnemiaData data)
	{
		super.unload(data);
		if (reductionFactor != null)
			data.setReductionFactor(reductionFactor.unload());
	}
	
	public boolean hasReductionFactor()
	{
		return reductionFactor == null ? false : reductionFactor.isValid();
	}
	public SEScalar0To1 getReductionFactor()
	{
		if (reductionFactor == null)
			reductionFactor = new SEScalar0To1();
		return reductionFactor;
	}
	
	public String toString()
	{
		return "Chronic Anemia" 
				+ "\n\tReduction Factor: " + getReductionFactor();
	}

	
}
