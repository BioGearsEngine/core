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

package mil.tatrc.physiology.datamodel.system.environment;

import java.util.*;

import org.jfree.util.Log;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.EnumSubstanceState;
import mil.tatrc.physiology.datamodel.bind.EnumSurroundingType;
import mil.tatrc.physiology.datamodel.bind.EnvironmentalConditionsData;
import mil.tatrc.physiology.datamodel.bind.SubstanceConcentrationData;
import mil.tatrc.physiology.datamodel.bind.SubstanceFractionData;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceConcentration;
import mil.tatrc.physiology.datamodel.substance.SESubstanceFraction;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;

public class SEEnvironmentalConditions
{
	protected EnumSurroundingType             surroundingType;
	protected SEScalarMassPerVolume	          airDensity;
	protected SEScalarLengthPerTime	          airVelocity;
	protected SEScalarTemperature             ambientTemperature;
	protected SEScalarPressure	              atmosphericPressure;
	protected SEScalarHeatResistanceArea	    clothingResistance;
	protected SEScalarFraction	              emissivity;
	protected SEScalarTemperature	            meanRadiantTemperature;
	protected SEScalarFraction                relativeHumidity;
	protected SEScalarTemperature	            respirationAmbientTemperature;

	protected List<SESubstanceFraction>       ambientGases;
	protected List<SESubstanceConcentration>  ambientAerosols;
	
	
	public SEEnvironmentalConditions()
	{
		this.surroundingType=null;

		this.airDensity=null;
		this.airVelocity=null;
		this.ambientTemperature=null;
		this.atmosphericPressure=null;
		this.clothingResistance=null;
		this.emissivity=null;
		this.meanRadiantTemperature=null;
		this.relativeHumidity=null;
		this.respirationAmbientTemperature=null;

		this.ambientGases=new ArrayList<SESubstanceFraction>();
		this.ambientAerosols=new ArrayList<SESubstanceConcentration>();

	}
	
	public void reset()
	{
		surroundingType = null;
		if (airDensity != null)
			airDensity.invalidate();
		if (airVelocity != null)
			airVelocity.invalidate();
		if (ambientTemperature != null)
			ambientTemperature.invalidate();
		if (atmosphericPressure != null)
			atmosphericPressure.invalidate();
		if (clothingResistance != null)
			clothingResistance.invalidate();
		if (emissivity != null)
			emissivity.invalidate();
		if (meanRadiantTemperature != null)
			meanRadiantTemperature.invalidate();
		if (relativeHumidity != null)
			relativeHumidity.invalidate();
		if (respirationAmbientTemperature != null)
			respirationAmbientTemperature.invalidate();
		
		this.ambientGases.clear();
		this.ambientAerosols.clear();
	}
	
	public void copy(SEEnvironmentalConditions from)
  {
    this.reset();
    if(from.hasSurroundingType())
      this.setSurroundingType(from.surroundingType);
    if(from.hasAirDensity())
      this.getAirDensity().set(from.getAirDensity());
    if(from.hasAirVelocity())
      this.getAirVelocity().set(from.getAirVelocity());
    if(from.hasAmbientTemperature())
      this.getAmbientTemperature().set(from.getAmbientTemperature());
    if(from.hasAtmosphericPressure())
      this.getAtmosphericPressure().set(from.getAtmosphericPressure());
    if(from.hasClothingResistance())
      this.getClothingResistance().set(from.getClothingResistance());
    if(from.hasEmissivity())
      this.getEmissivity().set(from.getEmissivity());
    if(from.hasMeanRadiantTemperature())
      this.getMeanRadiantTemperature().set(from.getMeanRadiantTemperature());
    if(from.hasRelativeHumidity())
      this.getRelativeHumidity().set(from.getRelativeHumidity());
    if(from.hasRespirationAmbientTemperature())
      this.getRespirationAmbientTemperature().set(from.getRespirationAmbientTemperature());
    
    if(from.ambientGases!=null)
    {
      SESubstanceFraction mine;
      for(SESubstanceFraction sf : from.ambientGases)
      {
        mine=this.createAmbientGas(sf.getSubstance());
        if(sf.hasFractionAmount())
          mine.getFractionAmount().set(sf.getFractionAmount());
      }
    }    
    
    if(from.ambientAerosols!=null)
    {
      SESubstanceConcentration mine;
      for(SESubstanceConcentration sc : from.ambientAerosols)
      {
        mine=this.createAmbientAerosol(sc.getSubstance());
        if(sc.hasConcentration())
          mine.getConcentration().set(sc.getConcentration());
      }
    }    
  }
	
	public boolean load(EnvironmentalConditionsData in, SESubstanceManager substances)
	{
		this.reset();
		if (in.getSurroundingType() != null)
			setSurroundingType(in.getSurroundingType());
		if (in.getAirDensity() != null)
			getAirDensity().load(in.getAirDensity());
		if (in.getAirVelocity() != null)
			getAirVelocity().load(in.getAirVelocity());
		if (in.getAmbientTemperature() != null)
			getAmbientTemperature().load(in.getAmbientTemperature());
		if (in.getAtmosphericPressure() != null)
			getAtmosphericPressure().load(in.getAtmosphericPressure());
		if (in.getClothingResistance() != null)
			getClothingResistance().load(in.getClothingResistance());
		if (in.getEmissivity() != null)
			getEmissivity().load(in.getEmissivity());
		if (in.getMeanRadiantTemperature() != null)
			getMeanRadiantTemperature().load(in.getMeanRadiantTemperature());
		if (in.getRelativeHumidity() != null)
			getRelativeHumidity().load(in.getRelativeHumidity());
		if (in.getRespirationAmbientTemperature() != null)
			getRespirationAmbientTemperature().load(in.getRespirationAmbientTemperature());
		
		SESubstance sub;
		if(in.getAmbientGas()!=null)
		{
			for(SubstanceFractionData subData : in.getAmbientGas())
			{
				sub = substances.getSubstance(subData.getName());
				if(sub == null)
				{
					Log.error("Substance does not exist : "+subData.getName());
					return false;
				}
				if(sub.getState() != EnumSubstanceState.GAS)
				{
					Log.error("Environment Ambient Gas must be a gas, "+subData.getName()+" is not a gas...");
					return false;
				}
				this.createAmbientGas(sub).getFractionAmount().load(subData.getFractionAmount());
			}
		}
		
		if(in.getAmbientAerosol()!=null)
		{
			for(SubstanceConcentrationData subData : in.getAmbientAerosol())
			{
				sub = substances.getSubstance(subData.getName());
				if(sub == null)
				{
					Log.error("Substance does not exist : "+subData.getName());
					return false;
				}
				if(sub.getState() != EnumSubstanceState.SOLID && sub.getState() != EnumSubstanceState.LIQUID)
				{
					Log.error("Environment Ambient Aerosol must be a liquid or a gas, "+subData.getName()+" is neither...");
					return false;
				}
				this.createAmbientAerosol(sub).getConcentration().load(subData.getConcentration());
			}
		}
		
		return true;
	}
	
	public EnvironmentalConditionsData unload()
	{
	  EnvironmentalConditionsData data = CDMSerializer.objFactory.createEnvironmentalConditionsData();
		unload(data);
		return data;
	}
	
	protected void unload(EnvironmentalConditionsData data)
	{
		if (hasSurroundingType())
			data.setSurroundingType(surroundingType);
		if (airDensity != null)
			data.setAirDensity(airDensity.unload());
		if (airVelocity != null)
			data.setAirVelocity(airVelocity.unload());
		if (ambientTemperature != null)
			data.setAmbientTemperature(ambientTemperature.unload());
		if (atmosphericPressure != null)
			data.setAtmosphericPressure(atmosphericPressure.unload());		
		if (clothingResistance != null)
			data.setClothingResistance(clothingResistance.unload());
		if (emissivity != null)
		    data.setEmissivity(emissivity.unload());	
		if (meanRadiantTemperature != null)
		    data.setMeanRadiantTemperature(meanRadiantTemperature.unload());
		if (relativeHumidity != null)
		    data.setRelativeHumidity(relativeHumidity.unload());
		if (respirationAmbientTemperature != null)
		    data.setRespirationAmbientTemperature(respirationAmbientTemperature.unload());		
		
		for(SESubstanceFraction ambSub : this.ambientGases)
			data.getAmbientGas().add(ambSub.unload());
		
		for(SESubstanceConcentration ambSub : this.ambientAerosols)
			data.getAmbientAerosol().add(ambSub.unload());
	}
	
	public EnumSurroundingType getSurroundingType()
	{
		return surroundingType;
	}
	public void setSurroundingType(EnumSurroundingType surroundingType)
	{
		this.surroundingType = surroundingType;
	}
	public boolean hasSurroundingType()
	{
		return surroundingType == null ? false : true;
	}
	
	public SEScalarMassPerVolume getAirDensity()
	{
		if (airDensity == null)
			airDensity = new SEScalarMassPerVolume();
		return airDensity;
	}
	public boolean hasAirDensity()
	{
		return airDensity == null ? false : airDensity.isValid();
	}
	
	public SEScalarLengthPerTime getAirVelocity()
	{
		if (airVelocity == null)
			airVelocity = new SEScalarLengthPerTime();
		return airVelocity;
	}
	public boolean hasAirVelocity()
	{
		return airVelocity == null ? false : airVelocity.isValid();
	}
	
	public SEScalarTemperature getAmbientTemperature()
	{
		if (ambientTemperature == null)
			ambientTemperature = new SEScalarTemperature();
		return ambientTemperature;
	}
	public boolean hasAmbientTemperature()
	{
		return ambientTemperature == null ? false : ambientTemperature.isValid();
	}

	public SEScalarPressure getAtmosphericPressure()
	{
		if (atmosphericPressure == null)
			atmosphericPressure = new SEScalarPressure();
		return atmosphericPressure;
	}
	public boolean hasAtmosphericPressure()
	{
		return atmosphericPressure == null ? false : atmosphericPressure.isValid();
	}
	
	public SEScalarHeatResistanceArea getClothingResistance()
	{
		if (clothingResistance == null)
			clothingResistance = new SEScalarHeatResistanceArea();
		return clothingResistance;
	}
	public boolean hasClothingResistance()
	{
		return clothingResistance == null ? false : clothingResistance.isValid();
	}
	
	public SEScalarFraction getEmissivity()
	{
	    if (emissivity == null)
	    	emissivity = new SEScalarFraction();
	    return emissivity;
	}
	public boolean hasEmissivity()
	{
	    return emissivity == null ? false : emissivity.isValid();
	}
	
	public SEScalarTemperature getMeanRadiantTemperature()
	{
		if (meanRadiantTemperature == null)
			meanRadiantTemperature = new SEScalarTemperature();
		return meanRadiantTemperature;
	}
	public boolean hasMeanRadiantTemperature()
	{
		return meanRadiantTemperature == null ? false : meanRadiantTemperature.isValid();
	}
	
	public SEScalarFraction getRelativeHumidity()
	{
		if (relativeHumidity == null)
			relativeHumidity = new SEScalarFraction();
		return relativeHumidity;
	}
	public boolean hasRelativeHumidity()
	{
		return relativeHumidity == null ? false : relativeHumidity.isValid();
	}
	
	public SEScalarTemperature getRespirationAmbientTemperature()
	{
		if (respirationAmbientTemperature == null)
			respirationAmbientTemperature = new SEScalarTemperature();
		return respirationAmbientTemperature;
	}
	public boolean hasRespirationAmbientTemperature()
	{
		return respirationAmbientTemperature == null ? false : respirationAmbientTemperature.isValid();
	}
	
	public SESubstanceFraction createAmbientGas(SESubstance substance)
	{
		return getAmbientGas(substance);
	}
	public SESubstanceFraction getAmbientGas(SESubstance substance)
	{
		for(SESubstanceFraction sf : this.ambientGases)
		{
			if(sf.getSubstance().getName().equals(substance.getName()))
			{				
				return sf;
			}
		}		
		SESubstanceFraction sf = new SESubstanceFraction(substance);		
		this.ambientGases.add(sf);
		return sf;
	}
	public boolean hasAmbientGas()
	{
		return !this.ambientGases.isEmpty();
	}
	public boolean hasAmbientGas(SESubstance substance)
	{
		for(SESubstanceFraction sf : this.ambientGases)
		{
			if(sf.getSubstance()==substance)
			{				
				return true;
			}
		}
		return false;
	}
	public List<SESubstanceFraction> getAmbientGas()
	{
		return this.ambientGases;
	}
	public void removeAmbientGas(SESubstance substance)
	{
		for(SESubstanceFraction sf : this.ambientGases)
		{
			if(sf.getSubstance()==substance)
			{
				this.ambientGases.remove(sf);
				return;
			}
		}	
	}

  
  public SESubstanceConcentration createAmbientAerosol(SESubstance substance)
	{
		return getAmbientAerosol(substance);
	}
	public SESubstanceConcentration getAmbientAerosol(SESubstance substance)
	{
		for(SESubstanceConcentration sc : this.ambientAerosols)
		{
			if(sc.getSubstance().getName().equals(substance.getName()))
			{				
				return sc;
			}
		}		
		SESubstanceConcentration sc = new SESubstanceConcentration(substance);		
		this.ambientAerosols.add(sc);
		return sc;
	}
	public boolean hasAmbientAerosol()
	{
		return !this.ambientAerosols.isEmpty();
	}
	public boolean hasAmbientAerosol(SESubstance substance)
	{
		for(SESubstanceConcentration sc : this.ambientAerosols)
		{
			if(sc.getSubstance()==substance)
			{				
				return true;
			}
		}
		return false;
	}
	public List<SESubstanceConcentration> getAmbientAerosol()
	{
		return Collections.unmodifiableList(this.ambientAerosols);
	}
	public void removeAmbientAerosol(SESubstance substance)
	{
		for(SESubstanceConcentration sc : this.ambientAerosols)
		{
			if(sc.getSubstance()==substance)
			{
				this.ambientAerosols.remove(sc);
				return;
			}
		}	
	}
	
	
  public void trim()
  {
  	SESubstanceConcentration sc;
    Iterator<SESubstanceConcentration> icon = this.ambientAerosols.iterator();
    while(icon.hasNext())
    {
      sc=icon.next();
      if(sc.getConcentration().getValue()==0)
      	icon.remove();
    }    
    
    SESubstanceFraction sf;
    Iterator<SESubstanceFraction> ifrac = this.ambientGases.iterator();
    while(ifrac.hasNext())
    {
      sf=ifrac.next();
      if(sf.getFractionAmount().getValue()==0)
      	ifrac.remove();
    }    
  }
  
	public String toString()
	{
			String str = "Envriomental Conditions:" 
					+ "\n\tSurroundingType: " + (hasSurroundingType()?getSurroundingType():"None")
					+ "\n\tAirDensity: " + (hasAirDensity()?getAirDensity():"None")
					+ "\n\tAirVelocity: " + (hasAirVelocity()?getAirVelocity():"None")
					+ "\n\tAmbientTemperature: " + (hasAmbientTemperature()?getAmbientTemperature():"None")
					+ "\n\tAtmosphericPressure: " + (hasAtmosphericPressure()?getAtmosphericPressure():"None")
      		+ "\n\tClothingResistance: " + (hasClothingResistance()?getClothingResistance():"None")
      		+ "\n\tEmissivity: " + (hasEmissivity()?getEmissivity():"None")
      		+ "\n\tMeanRadiantTemperature: " + (hasMeanRadiantTemperature()?getMeanRadiantTemperature():"None")
      		+ "\n\tRelativeHumidity: " + (hasRelativeHumidity()?getRelativeHumidity():"None")
      		+ "\n\tRespirationAmbientTemperature: " + (hasRespirationAmbientTemperature()?getRespirationAmbientTemperature():"None");
			for(SESubstanceFraction sf : this.ambientGases)
				str += "\n\t"+sf.toString();
			for(SESubstanceConcentration sc : this.ambientAerosols)
			str += "\n\t"+sc.toString();
			
			return str;
	}
	
	
}