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

package mil.tatrc.physiology.datamodel.substance;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.properties.*;

public class SESubstanceFraction
{
	protected SEScalarFraction  fractionAmount;
	protected SESubstance       substance;
	
	public SESubstanceFraction(SESubstance s)
	{
		if(s==null)
			throw new RuntimeException("Must provide a valid substance");
		this.substance=s;
	}
	
	public void reset()
	{
		if(this.fractionAmount!=null)
			this.fractionAmount.invalidate();
	}
	
	public void copy(SESubstanceFraction from)
	{
	  this.substance=from.substance;
	  if(from.hasFractionAmount())
	    this.getFractionAmount().set(from.getFractionAmount());
	}
	
	public boolean load(SubstanceFractionData data)
	{
		this.reset();
		if(data.getFractionAmount()!=null)
			this.getFractionAmount().load(data.getFractionAmount());
		return true;
	}
	
	public SubstanceFractionData unload()
	{
		SubstanceFractionData to = CDMSerializer.objFactory.createSubstanceFractionData();
		unload(to);
		return to;
	}
	
	protected void unload(SubstanceFractionData to)
	{
		if(hasFractionAmount())
			to.setFractionAmount(this.fractionAmount.unload());		
		to.setName(this.substance.getName());
	}
	
	public SEScalarFraction getFractionAmount() 
	{ 
		if(this.fractionAmount==null)
			this.fractionAmount=new SEScalarFraction();
		return this.fractionAmount;
	}
	public boolean hasFractionAmount() {return this.fractionAmount==null?false:this.fractionAmount.isValid();}
	
	public boolean hasSubstance() { return substance != null; }
	public SESubstance getSubstance() 
	{ 
		return this.substance;
	}
	
	public String toString()
	{
		if(!hasSubstance())
			return "";
		String str = "Fraction of "+this.substance.getName()+": "+(hasFractionAmount()?getFractionAmount():"None");			
		return str;
	}
}
