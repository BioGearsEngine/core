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

package mil.tatrc.physiology.datamodel.substance.quantity;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.LiquidSubstanceQuantityData;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.AmountPerVolumeUnit;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.MassPerAmountUnit;
import mil.tatrc.physiology.datamodel.properties.SEScalarAmountPerVolume;
import mil.tatrc.physiology.datamodel.properties.SEScalarFraction;
import mil.tatrc.physiology.datamodel.properties.SEScalarMass;
import mil.tatrc.physiology.datamodel.properties.SEScalarMassPerVolume;
import mil.tatrc.physiology.datamodel.properties.SEScalarPressure;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.MassPerVolumeUnit;
import mil.tatrc.physiology.datamodel.substance.SESubstance;

public class SELiquidSubstanceQuantity extends SESubstanceQuantity
{
  protected SEScalarMassPerVolume   concentration;
  protected SEScalarMass            mass;
	protected SEScalarMass            massCleared;
	protected SEScalarMass            massDeposited;
	protected SEScalarMass            massExcreted;
  protected SEScalarAmountPerVolume molarity;
  protected SEScalarPressure        partialPressure;
  protected SEScalarFraction        saturation;

  public SELiquidSubstanceQuantity(SESubstance s)
  {
  	super(s);
    concentration = null;
    mass = null;
    massCleared = null;
    massDeposited = null;
    massExcreted = null;
    molarity = null;
    partialPressure = null;
    saturation = null;
  }

  public void reset()
  {
    super.reset();
    if (concentration != null)
      concentration.invalidate();
    if (mass != null)
      mass.invalidate();
    if(massCleared!=null)
			massCleared.invalidate();
    if(massDeposited!=null)
    	massDeposited.invalidate();		
    if(massExcreted!=null)
			massExcreted.invalidate();		
    if (molarity != null)
      molarity.invalidate();
    if (partialPressure != null)
      partialPressure.invalidate();
    if (saturation != null)
      saturation.invalidate();
  }

  public boolean load(LiquidSubstanceQuantityData in)
  {
    super.load(in);
    if (in.getConcentration() != null) 
      getConcentration().load(in.getConcentration()); 
    if (in.getMass() != null) 
      getMass().load(in.getMass()); 
    if(in.getMassCleared()!=null)
			getMassCleared().load(in.getMassCleared());
    if(in.getMassDeposited()!=null)
    	getMassDeposited().load(in.getMassDeposited());
		if(in.getMassExcreted()!=null)
			getMassExcreted().load(in.getMassExcreted());				
    if (in.getPartialPressure() != null) 
      getPartialPressure().load(in.getPartialPressure()); 
    if (in.getSaturation() != null) 
      getSaturation().load(in.getSaturation()); 
  
    return true;
  }

  public LiquidSubstanceQuantityData unload()
  {
    LiquidSubstanceQuantityData data = CDMSerializer.objFactory.createLiquidSubstanceQuantityData();
    unload(data);
    return data;
  }

  protected void unload(LiquidSubstanceQuantityData data)
  {
    super.unload(data);
    if (hasConcentration())
      data.setConcentration(concentration.unload());
    if (hasMass())
      data.setMass(mass.unload());
    if(hasMassCleared())
    	data.setMassCleared(this.massCleared.unload());
    if(hasMassDeposited())
    	data.setMassDeposited(this.massDeposited.unload());
		if(hasMassExcreted())
			data.setMassExcreted(this.massExcreted.unload());		
    if (hasPartialPressure())
      data.setPartialPressure(partialPressure.unload());
    if (hasSaturation())
      data.setSaturation(saturation.unload());	 
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

  public boolean hasMass()
  {
    return mass == null ? false : mass.isValid();
  }
  public SEScalarMass getMass()
  {
    if (mass == null)
      mass = new SEScalarMass();
    return mass;
  }
  
  public SEScalarMass getMassCleared() 
	{ 
		if(this.massCleared==null)
			this.massCleared=new SEScalarMass();
		return this.massCleared;
	}
	public boolean      hasMassCleared() {return this.massCleared==null?false:this.massCleared.isValid();}
	
	public SEScalarMass getMassDeposited() 
	{ 
		if(this.massDeposited==null)
			this.massDeposited=new SEScalarMass();
		return this.massDeposited;
	}
	public boolean      hasMassDeposited() {return this.massDeposited==null?false:this.massDeposited.isValid();}
	
	public SEScalarMass getMassExcreted() 
	{ 
		if(this.massExcreted==null)
			this.massExcreted=new SEScalarMass();
		return this.massExcreted;
	}
	public boolean      hasMassExcreted() {return this.massExcreted==null?false:this.massExcreted.isValid();}
	 
  public boolean hasMolarity()
  {
    return molarity == null ? false : molarity.isValid();
  }
  public SEScalarAmountPerVolume getMolarity()
  {
    if (molarity == null)
      molarity = new SEScalarAmountPerVolume();
    molarity.setValue(getConcentration().getValue(MassPerVolumeUnit.ug_Per_mL)/substance.getMolarMass().getValue(MassPerAmountUnit.ug_Per_mol),AmountPerVolumeUnit.mol_Per_mL);
    return molarity;
  }

  public boolean hasPartialPressure()
  {
    return partialPressure == null ? false : partialPressure.isValid();
  }
  public SEScalarPressure getPartialPressure()
  {
    if (partialPressure == null)
      partialPressure = new SEScalarPressure();
    return partialPressure;
  }

  public boolean hasSaturation()
  {
    return saturation == null ? false : saturation.isValid();
  }
  public SEScalarFraction getSaturation()
  {
    if (saturation == null)
      saturation = new SEScalarFraction();
    return saturation;
  }
}
