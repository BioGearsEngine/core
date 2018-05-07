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
import mil.tatrc.physiology.datamodel.bind.SubstanceCompoundInfusionData;
import mil.tatrc.physiology.datamodel.properties.SEScalarVolume;
import mil.tatrc.physiology.datamodel.properties.SEScalarVolumePerTime;
import mil.tatrc.physiology.datamodel.substance.SESubstanceCompound;

public class SESubstanceCompoundInfusion extends SESubstanceAdministration
{
	protected SEScalarVolume bagVolume;
	protected SEScalarVolumePerTime rate;
	protected SESubstanceCompound compound;
	
	public SESubstanceCompoundInfusion(SESubstanceCompound compound)
	{
		this.rate = null;
		this.bagVolume = null;
		this.compound = compound;
	}

	public void copy(SESubstanceCompoundInfusion other)
	{
		if(this==other)
			return;
		super.copy(other);
		compound = other.compound;
		
		if (other.rate != null)
			getRate().set(other.rate);
		else if (rate != null)
			rate.invalidate();
		
		if (other.bagVolume != null)
			getBagVolume().set(other.bagVolume);
		else if (bagVolume != null)
			bagVolume.invalidate();
	}
	
	public void reset()
	{
		super.reset();
		if (rate != null)
			rate.invalidate();
		if (bagVolume != null)
			bagVolume.invalidate();
	}
	
	public boolean isValid()
	{
		return hasRate() && hasBagVolume() && hasSubstanceCompound();
	}
	
	public boolean load(SubstanceCompoundInfusionData in)
	{
		super.load(in);
		getRate().load(in.getRate());
		getBagVolume().load(in.getBagVolume());
		return isValid();
	}
	
	public SubstanceCompoundInfusionData unload()
	{
		SubstanceCompoundInfusionData data = CDMSerializer.objFactory.createSubstanceCompoundInfusionData();
		unload(data);
		return data;
	}
	
	protected void unload(SubstanceCompoundInfusionData data)
	{
		super.unload(data);
		if (rate != null)
			data.setRate(rate.unload());
		if (bagVolume != null)
			data.setBagVolume(bagVolume.unload());
		data.setSubstanceCompound(getSubstanceCompound().getName());
	}
	
	public boolean hasBagVolume()
	{
		return bagVolume == null ? false : bagVolume.isValid();
	}
	public SEScalarVolume getBagVolume()
	{
		if (bagVolume == null)
			bagVolume = new SEScalarVolume();
		return bagVolume;
	}
	
	public boolean hasRate()
	{
		return rate == null ? false : rate.isValid();
	}
	public SEScalarVolumePerTime getRate()
	{
		if (rate == null)
			rate = new SEScalarVolumePerTime();
		return rate;
	}
	
	public boolean hasSubstanceCompound(){return true;}
	public SESubstanceCompound getSubstanceCompound()
	{
		return compound;
	}
	
	public String toString()
	{
		if (rate != null || compound != null)
			return "Compound Infusion "
					+ "\n\tRate: " + getRate() 
					+ "\n\tBag Volume: " + getBagVolume() 
					+ "\n\tSubstance Compound: " + getSubstanceCompound().getName();
		else
			return "Action not specified properly";
	}
}
