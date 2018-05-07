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

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.SubstanceDataRequestData;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;

public class SESubstanceDataRequest extends SEDataRequest
{
	protected String      compartment;
	protected SESubstance substance;
	
	public SESubstanceDataRequest(SESubstance s)
	{
		this.substance = s;
	}
	
	public void reset()
	{
		super.reset();
		compartment = null;		
	}
	
	public boolean load(SubstanceDataRequestData in, SESubstanceManager substances)
	{
		super.load(in);
		if(in.getCompartment()!=null)
			setCompartment(in.getCompartment());		
		return true;
	}
	
	public SubstanceDataRequestData unload()
	{
		SubstanceDataRequestData data = CDMSerializer.objFactory.createSubstanceDataRequestData();
		unload(data);
		return data;
	}
	
	protected void unload(SubstanceDataRequestData data)
	{
		super.unload(data);
		if (hasCompartment())
			data.setCompartment(compartment);
		data.setSubstance(substance.getName());
	}
	
	public int hashCode()
	{
		int c = super.hashCode();
		if(compartment!=null)
			c = 31 * c + compartment.hashCode();
		if(substance!=null)
			c = 31 * c + substance.getName().hashCode();
		return c;
	}
	
	/*
	 * Compartment
	 */
	public String getCompartment()
	{
		return compartment;
	}
	public void setCompartment(String compartment)
	{
		this.compartment = compartment;
	}
	public boolean hasCompartment()
	{
		return compartment == null ? false : !compartment.isEmpty();
	}
	public void invalidateCompartment()
	{
		compartment = null;
	}
	
	/*
	 * Substance
	 */
	public SESubstance getSubstance()
	{
		return substance;
	}
}
