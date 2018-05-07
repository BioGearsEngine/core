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

package mil.tatrc.physiology.datamodel.scenario.datarequests;

import mil.tatrc.physiology.datamodel.bind.CompartmentSubstanceDataRequestData;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;

public abstract class SECompartmentSubstanceDataRequest extends SECompartmentDataRequest
{
	protected SESubstance         substance;
	
	public SECompartmentSubstanceDataRequest()
	{
		
	}
	
	public void reset()
	{
		super.reset();
		
		substance = null;
	}
	
	public boolean load(CompartmentSubstanceDataRequestData in, SESubstanceManager substances)
	{
		super.load(in);
			
		if (in.getSubstance() != null)
			setSubstance(substances.getSubstance(in.getSubstance()));
		return true;
	}
	
	protected void unload(CompartmentSubstanceDataRequestData data)
	{
		super.unload(data);
		
		if (hasSubstance())
			data.setSubstance(substance.getName());
	}
	
	public int hashCode()
	{
		int c = super.hashCode();
		if(substance!=null)
			c = 31 * c + substance.getName().hashCode();
		return c;
	}
	

	/*
	 * Substance
	 */
	public SESubstance getSubstance()
	{
		return substance;
	}
	public void setSubstance(SESubstance substance)
	{
		this.substance = substance;
	}
	public boolean hasSubstance()
	{
		return substance == null ? false : true;
	}
	public void invalidateSubstance()
	{
		substance = null;
	}
}
