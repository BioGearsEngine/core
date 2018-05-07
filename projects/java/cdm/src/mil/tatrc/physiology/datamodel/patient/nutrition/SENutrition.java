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
package mil.tatrc.physiology.datamodel.patient.nutrition;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.NutritionData;
import mil.tatrc.physiology.datamodel.properties.SEScalarMass;
import mil.tatrc.physiology.datamodel.properties.SEScalarMassPerTime;
import mil.tatrc.physiology.datamodel.properties.SEScalarVolume;

public class SENutrition 
{
	protected SEScalarMass	         carbohydrate;
	protected SEScalarMassPerTime    carbohydrateDigestionRate;
	protected SEScalarMass	         fat;
	protected SEScalarMassPerTime    fatDigestionRate;
	protected SEScalarMass           protein;
	protected SEScalarMassPerTime    proteinDigestionRate;
	protected SEScalarMass	         calcium;
	protected SEScalarMass           sodium;
	protected SEScalarVolume         water;
	
	
	public SENutrition()
	{		
		this.carbohydrate=null;
		this.carbohydrateDigestionRate=null;
		this.fat=null;
		this.fatDigestionRate=null;
		this.protein=null;
		this.proteinDigestionRate=null;
		this.calcium=null;
		this.sodium=null;
		this.water=null;
	}
	
	public void reset()
	{
		if (carbohydrate != null)
			carbohydrate.invalidate();
		if (carbohydrateDigestionRate != null)
      carbohydrateDigestionRate.invalidate();
    if (fat != null)
			fat.invalidate();
    if (fatDigestionRate != null)
      fatDigestionRate.invalidate();
    if (protein != null)
			protein.invalidate();
    if (proteinDigestionRate != null)
      proteinDigestionRate.invalidate();
    if (calcium != null)
      calcium.invalidate();  
    if (sodium != null)
      sodium.invalidate();  
    if (water != null)
			water.invalidate();		
	}
	
	public void copy(SENutrition from)
	{
		this.reset();
		if(from.hasCarbohydrate())
			this.getCarbohydrate().set(from.getCarbohydrate());
		if(from.hasCarbohydrateDigestionRate())
      this.getCarbohydrateDigestionRate().set(from.getCarbohydrateDigestionRate());
    if(from.hasFat())
			this.getFat().set(from.getFat());
    if(from.hasFatDigestionRate())
      this.getFatDigestionRate().set(from.getFatDigestionRate());
    if(from.hasProtein())
			this.getProtein().set(from.getProtein());
    if(from.hasProteinDigestionRate())
      this.getProteinDigestionRate().set(from.getProteinDigestionRate());
    if(from.hasSodium())
			this.getSodium().set(from.getSodium());   
    if(from.hasCalcium())
      this.getCalcium().set(from.getCalcium()); 
    if(from.hasWater())
			this.getWater().set(from.getWater());
	}
	
	public boolean load(NutritionData in)
	{
		if (in.getCarbohydrate() != null)
			getCarbohydrate().load(in.getCarbohydrate());
		if (in.getCarbohydrateDigestionRate() != null)
      getCarbohydrateDigestionRate().load(in.getCarbohydrateDigestionRate());
    if (in.getFat() != null)
			getFat().load(in.getFat());
    if (in.getFatDigestionRate() != null)
      getFatDigestionRate().load(in.getFatDigestionRate());
    if (in.getProtein() != null)
			getProtein().load(in.getProtein());
    if (in.getProteinDigestionRate() != null)
      getProteinDigestionRate().load(in.getProteinDigestionRate());
    if (in.getCalcium() != null)
      getCalcium().load(in.getCalcium());  
    if (in.getSodium() != null)
      getSodium().load(in.getSodium());  
    if (in.getWater() != null)
      getWater().load(in.getWater());
		return true;
	}
	
	public NutritionData unload()
	{
		NutritionData data = CDMSerializer.objFactory.createNutritionData();
		unload(data);
		return data;
	}
	
	protected void unload(NutritionData data)
	{
		if (carbohydrate != null)
			data.setCarbohydrate(carbohydrate.unload());
		if (carbohydrateDigestionRate != null)
      data.setCarbohydrateDigestionRate(carbohydrateDigestionRate.unload());
    if (fat != null)
			data.setFat(fat.unload());
    if (fatDigestionRate != null)
      data.setFatDigestionRate(fatDigestionRate.unload());
    if (protein != null)
			data.setProtein(protein.unload());
    if (proteinDigestionRate != null)
      data.setProteinDigestionRate(proteinDigestionRate.unload());
    if (sodium != null)
			data.setSodium(sodium.unload());	
    if (calcium != null)
      data.setCalcium(calcium.unload());  
    if (water != null)
			data.setWater(water.unload());
	}		
	
	public SEScalarMass getCarbohydrate()
	{
		if (carbohydrate == null)
			carbohydrate = new SEScalarMass();
		return carbohydrate;
	}
	public boolean hasCarbohydrate()
	{
		return carbohydrate == null ? false : carbohydrate.isValid();
	}
	
	public SEScalarMassPerTime getCarbohydrateDigestionRate()
  {
    if (carbohydrateDigestionRate == null)
      carbohydrateDigestionRate = new SEScalarMassPerTime();
    return carbohydrateDigestionRate;
  }
  public boolean hasCarbohydrateDigestionRate()
  {
    return carbohydrateDigestionRate == null ? false : carbohydrateDigestionRate.isValid();
  }
	
	public SEScalarMass getFat()
	{
		if (fat == null)
			fat = new SEScalarMass();
		return fat;
	}
	public boolean hasFat()
	{
		return fat == null ? false : fat.isValid();
	}
	
	public SEScalarMassPerTime getFatDigestionRate()
  {
    if (fatDigestionRate == null)
      fatDigestionRate = new SEScalarMassPerTime();
    return fatDigestionRate;
  }
  public boolean hasFatDigestionRate()
  {
    return fatDigestionRate == null ? false : fatDigestionRate.isValid();
  }
	
	public SEScalarMass getProtein()
	{
		if (protein == null)
			protein = new SEScalarMass();
		return protein;
	}
	public boolean hasProtein()
	{
		return protein == null ? false : protein.isValid();
	}
	
	public SEScalarMassPerTime getProteinDigestionRate()
  {
    if (proteinDigestionRate == null)
      proteinDigestionRate = new SEScalarMassPerTime();
    return proteinDigestionRate;
  }
  public boolean hasProteinDigestionRate()
  {
    return protein == null ? false : protein.isValid();
  }

	public SEScalarMass getSodium()
	{
		if (sodium == null)
			sodium = new SEScalarMass();
		return sodium;
	}
	public boolean hasSodium()
	{
		return sodium == null ? false : sodium.isValid();
	}
	
	public SEScalarMass getCalcium()
  {
    if (calcium == null)
      calcium = new SEScalarMass();
    return calcium;
  }
  public boolean hasCalcium()
  {
    return calcium == null ? false : calcium.isValid();
  }
	
	public SEScalarVolume getWater()
	{
	    if (water == null)
	    	water = new SEScalarVolume();
	    return water;
	}
	public boolean hasWater()
	{
	    return water == null ? false : water.isValid();
	}
}
