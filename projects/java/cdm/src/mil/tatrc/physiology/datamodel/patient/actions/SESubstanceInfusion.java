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
import mil.tatrc.physiology.datamodel.bind.SubstanceInfusionData;
import mil.tatrc.physiology.datamodel.properties.SEScalarMassPerVolume;
import mil.tatrc.physiology.datamodel.properties.SEScalarVolumePerTime;
import mil.tatrc.physiology.datamodel.substance.SESubstance;

public class SESubstanceInfusion extends SESubstanceAdministration
{
	protected SEScalarMassPerVolume concentration;
	protected SEScalarVolumePerTime rate;
	protected SESubstance substance;
	
	public SESubstanceInfusion(SESubstance substance)
	{
		this.rate = null;
		this.concentration = null;
		this.substance = substance;
	}
	
	public void copy(SESubstanceInfusion other)
	{
		if(this==other)
			return;
		super.copy(other);
		substance = other.substance;
		
		if (other.rate != null)
			getRate().set(other.rate);
		else if (rate != null)
			rate.invalidate();
		
		if (other.concentration != null)
			getConcentration().set(other.concentration);
		else if (concentration != null)
			concentration.invalidate();
	}
	
	public void reset()
	{
		super.reset();
		if (rate != null)
			rate.invalidate();
		if (concentration != null)
			concentration.invalidate();
	}
	
	public boolean isValid()
	{
		return hasRate() && hasConcentration() && hasSubstance();
	}
	
	public boolean load(SubstanceInfusionData in)
	{
		super.load(in);
		getRate().load(in.getRate());
		getConcentration().load(in.getConcentration());
		return isValid();
	}
	
	public SubstanceInfusionData unload()
	{
		SubstanceInfusionData data = CDMSerializer.objFactory.createSubstanceInfusionData();
		unload(data);
		return data;
	}
	
	protected void unload(SubstanceInfusionData data)
	{
		super.unload(data);
		if (rate != null)
			data.setRate(rate.unload());
		if (concentration != null)
			data.setConcentration(concentration.unload());
    data.setSubstance(substance.getName());
	}
	
	public boolean hasConcentration()
	{
		return concentration == null ? false : concentration.isValid();
	}
	public SEScalarMassPerVolume getConcentration()
	{
		if (concentration == null)
			concentration = new SEScalarMassPerVolume();
		return concentration;
	}
	
	public boolean hasRate()
	{
		return rate ==  null ? false : rate.isValid();
	}
	public SEScalarVolumePerTime getRate()
	{
		if (rate == null)
			rate = new SEScalarVolumePerTime();
		return rate;
	}
	
	public boolean hasSubstance() { return substance != null; }
	public SESubstance getSubstance()
	{
		return substance;
	}
	
	public String toString()
	{
		if (rate != null || concentration != null)	
			return "Substance Infusion" 
					+ "\n\tRate: " + getRate() 
					+ "\n\tConcentration: " + getConcentration() 
					+ "\n\tSubstance: " + getSubstance().getName() ;
		else
			return "Action not specified properly";
	}
}
