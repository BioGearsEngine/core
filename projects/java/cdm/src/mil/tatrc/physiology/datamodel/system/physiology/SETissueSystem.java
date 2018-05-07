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

package mil.tatrc.physiology.datamodel.system.physiology;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.TissueSystemData;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SETissueSystem extends SEPhysiologySystem implements SESystem
{
  protected SEScalarVolumePerTime 			carbonDioxideProductionRate;
  protected SEScalarFraction			 			dehydrationFraction;
  protected SEScalarVolume        			extracellularFluidVolume;
  protected SEScalarVolume        			extravascularFluidVolume;
  protected SEScalarVolume        			intracellularFluidVolume;
  protected SEScalarVolume        			TotalBodyFluidVolume;
  protected SEScalar              			intracellularFluidPH;  
  protected SEScalarVolumePerTime 			oxygenConsumptionRate;
  protected SEScalar				      			respiratoryExchangeRatio;
  protected SEScalarAmountPerVolume			liverInsulinSetPoint;
  protected SEScalarMassPerVolume				liverGlucagonSetPoint;
  protected SEScalarAmountPerVolume			muscleInsulinSetPoint;
  protected SEScalarMassPerVolume				muscleGlucagonSetPoint;
  protected SEScalarAmountPerVolume			fatInsulinSetPoint;
  protected SEScalarMassPerVolume				fatGlucagonSetPoint;
  protected SEScalarMass								liverGlycogen;
  protected SEScalarMass								muscleGlycogen;
  protected SEScalarMass								storedProtein;
  protected SEScalarMass								storedFat;

  public SETissueSystem()
  {
    carbonDioxideProductionRate = null;
    dehydrationFraction = null;
    extracellularFluidVolume = null;
    extravascularFluidVolume = null;
    intracellularFluidVolume = null;
    TotalBodyFluidVolume = null;
    intracellularFluidPH = null;
    oxygenConsumptionRate = null;
    respiratoryExchangeRatio = null;
    liverInsulinSetPoint = null;
    liverGlucagonSetPoint = null;
    muscleInsulinSetPoint = null;
    muscleGlucagonSetPoint = null;
    fatInsulinSetPoint = null;
    fatGlucagonSetPoint = null;
    liverGlycogen = null;
    muscleGlycogen = null;
    storedProtein = null;
    storedFat = null;
  }

  public void reset()
  {		
    if (dehydrationFraction != null)
    	dehydrationFraction.invalidate();
    if (carbonDioxideProductionRate != null)
      carbonDioxideProductionRate.invalidate();
    if (extracellularFluidVolume != null)
      extracellularFluidVolume.invalidate();
    if (extravascularFluidVolume != null)
      extravascularFluidVolume.invalidate();
    if (intracellularFluidVolume != null)
      intracellularFluidVolume.invalidate(); 
    if (TotalBodyFluidVolume != null)
        TotalBodyFluidVolume.invalidate(); 
    if (intracellularFluidPH != null)
      	intracellularFluidPH.invalidate();
    if (oxygenConsumptionRate != null)
      oxygenConsumptionRate.invalidate();
    if (respiratoryExchangeRatio != null)
      respiratoryExchangeRatio.invalidate();
    if (liverInsulinSetPoint != null)
    	liverInsulinSetPoint.invalidate();
    if (liverGlucagonSetPoint != null)
    	liverGlucagonSetPoint.invalidate();
    if (muscleInsulinSetPoint != null)
    	muscleInsulinSetPoint.invalidate();
    if (muscleGlucagonSetPoint != null)
    	muscleGlucagonSetPoint.invalidate();
    if (fatInsulinSetPoint != null)
    	fatInsulinSetPoint.invalidate();
    if (fatGlucagonSetPoint != null)
    	fatGlucagonSetPoint.invalidate();
    if (liverGlycogen != null)
    	liverGlycogen.invalidate();
    if (muscleGlycogen != null)
    	muscleGlycogen.invalidate();
    if (storedProtein != null)
    	storedProtein.invalidate();
    if (storedFat != null)
    	storedFat.invalidate();
  }

  public boolean load(TissueSystemData in)
  {		
    if (in.getCarbonDioxideProductionRate() != null)
      getCarbonDioxideProductionRate().load(in.getCarbonDioxideProductionRate());   
    if (in.getDehydrationFraction() != null)
      getDehydrationFraction().load(in.getDehydrationFraction()); 
    if (in.getExtracellularFluidVolume() != null)
      getExtracellularFluidVolume().load(in.getExtracellularFluidVolume());
    if (in.getExtravascularFluidVolume() != null)
      getExtravascularFluidVolume().load(in.getExtravascularFluidVolume());
    if (in.getIntracellularFluidVolume() != null)
      getIntracellularFluidVolume().load(in.getIntracellularFluidVolume());
    if (in.getTotalBodyFluidVolume() != null)
        getTotalBodyFluidVolume().load(in.getTotalBodyFluidVolume());
    if (in.getIntracellularFluidPH() != null)
      getIntracellularFluidPH().load(in.getIntracellularFluidPH());
    if (in.getOxygenConsumptionRate() != null)
      getOxygenConsumptionRate().load(in.getOxygenConsumptionRate());
    if (in.getRespiratoryExchangeRatio() != null)
      getRespiratoryExchangeRatio().load(in.getRespiratoryExchangeRatio());
    if (in.getLiverInsulinSetPoint() != null)
      getLiverInsulinSetPoint().load(in.getLiverInsulinSetPoint());
    if (in.getLiverGlucagonSetPoint() != null)
      getLiverGlucagonSetPoint().load(in.getLiverGlucagonSetPoint());
    if (in.getMuscleInsulinSetPoint() != null)
      getMuscleInsulinSetPoint().load(in.getMuscleInsulinSetPoint());
    if (in.getMuscleGlucagonSetPoint() != null)
      getMuscleGlucagonSetPoint().load(in.getMuscleGlucagonSetPoint());
    if (in.getFatInsulinSetPoint() != null)
      getFatInsulinSetPoint().load(in.getFatInsulinSetPoint());
    if (in.getFatGlucagonSetPoint() != null)
      getFatGlucagonSetPoint().load(in.getFatGlucagonSetPoint());
    if (in.getLiverGlycogen() != null)
    	getLiverGlycogen().load(in.getLiverGlycogen());
    if (in.getMuscleGlycogen() != null)
    	getMuscleGlycogen().load(in.getMuscleGlycogen());
    if (in.getStoredProtein() != null)
    	getStoredProtein().load(in.getStoredProtein());
    if (in.getStoredFat() != null)
    	getStoredFat().load(in.getStoredFat());
   
    return true;
  }

  public TissueSystemData unload()
  {
    TissueSystemData data = CDMSerializer.objFactory.createTissueSystemData();
    unload(data);
    return data;
  }

  protected void unload(TissueSystemData data)
  {		
    if (hasCarbonDioxideProductionRate())
      data.setCarbonDioxideProductionRate(carbonDioxideProductionRate.unload());   
    if (hasDehydrationFraction())
      data.setDehydrationFraction(dehydrationFraction.unload()); 
    if (extracellularFluidVolume != null)
      data.setExtracellularFluidVolume(extracellularFluidVolume.unload());
    if (extravascularFluidVolume != null)
      data.setExtravascularFluidVolume(extravascularFluidVolume.unload());
    if (intracellularFluidVolume != null)
      data.setIntracellularFluidVolume(intracellularFluidVolume.unload());
    if (TotalBodyFluidVolume != null)
        data.setTotalBodyFluidVolume(TotalBodyFluidVolume.unload());
    if (intracellularFluidPH != null)
      data.setIntracellularFluidPH(intracellularFluidPH.unload());
    if (hasOxygenConsumptionRate())
      data.setOxygenConsumptionRate(oxygenConsumptionRate.unload());
    if (hasRespiratoryExchangeRatio())
      data.setRespiratoryExchangeRatio(respiratoryExchangeRatio.unload());    
    if (hasLiverInsulinSetPoint())
      data.setLiverInsulinSetPoint(liverInsulinSetPoint.unload());  
    if (hasLiverGlucagonSetPoint())
      data.setLiverGlucagonSetPoint(liverGlucagonSetPoint.unload());  
    if (hasMuscleInsulinSetPoint())
      data.setMuscleInsulinSetPoint(muscleInsulinSetPoint.unload());  
    if (hasMuscleGlucagonSetPoint())
      data.setMuscleGlucagonSetPoint(muscleGlucagonSetPoint.unload()); 
    if (hasFatInsulinSetPoint())
      data.setFatInsulinSetPoint(fatInsulinSetPoint.unload());  
    if (hasFatGlucagonSetPoint())
      data.setFatGlucagonSetPoint(fatGlucagonSetPoint.unload()); 
    if (hasLiverGlycogen())
      data.setLiverGlycogen(liverGlycogen.unload()); 
    if (hasMuscleGlycogen())
      data.setMuscleGlycogen(muscleGlycogen.unload()); 
    if (hasStoredProtein())
      data.setStoredProtein(storedProtein.unload()); 
    if (hasStoredFat())
      data.setStoredFat(storedFat.unload()); 
  }

  public boolean hasCarbonDioxideProductionRate()
  {
    return carbonDioxideProductionRate == null ? false : carbonDioxideProductionRate.isValid();
  }
  public SEScalarVolumePerTime getCarbonDioxideProductionRate()
  {
    if (carbonDioxideProductionRate == null)
      carbonDioxideProductionRate = new SEScalarVolumePerTime();
    return carbonDioxideProductionRate;
  }
  
  public boolean hasDehydrationFraction()
  {
    return dehydrationFraction == null ? false : dehydrationFraction.isValid();
  }
  public SEScalarFraction getDehydrationFraction()
  {
    if (dehydrationFraction == null)
    	dehydrationFraction = new SEScalarFraction();
    return dehydrationFraction;
  }
  
  public boolean hasExtracellularFluidVolume()
  {
    return extracellularFluidVolume == null ? false : extracellularFluidVolume.isValid();
  }
  public SEScalarVolume getExtracellularFluidVolume()
  {
    if (extracellularFluidVolume == null)
      extracellularFluidVolume = new SEScalarVolume();
    return extracellularFluidVolume;
  }

  public boolean hasExtravascularFluidVolume()
  {
    return extravascularFluidVolume == null ? false : extravascularFluidVolume.isValid();
  }
  public SEScalarVolume getExtravascularFluidVolume()
  {
    if (extravascularFluidVolume == null)
      extravascularFluidVolume = new SEScalarVolume();
    return extravascularFluidVolume;
  }

  public boolean hasIntracellularFluidVolume()
  {
    return intracellularFluidVolume == null ? false : intracellularFluidVolume.isValid();
  }
  public SEScalarVolume getIntracellularFluidVolume()
  {
    if (intracellularFluidVolume == null)
      intracellularFluidVolume = new SEScalarVolume();
    return intracellularFluidVolume;
  }

  public boolean hasTotalBodyFluidVolume()
  {
    return TotalBodyFluidVolume == null ? false : TotalBodyFluidVolume.isValid();
  }
  public SEScalarVolume getTotalBodyFluidVolume()
  {
    if (TotalBodyFluidVolume == null)
      TotalBodyFluidVolume = new SEScalarVolume();
    return TotalBodyFluidVolume;
  }
  
  public boolean hasIntracellularFluidPH()
  {
    return intracellularFluidPH == null ? false : intracellularFluidPH.isValid();
  }
  public SEScalar getIntracellularFluidPH()
  {
    if (intracellularFluidPH == null)
      intracellularFluidPH = new SEScalar();
    return intracellularFluidPH;
  }

  public boolean hasOxygenConsumptionRate()
  {
    return oxygenConsumptionRate == null ? false : oxygenConsumptionRate.isValid();
  }
  public SEScalarVolumePerTime getOxygenConsumptionRate()
  {
    if (oxygenConsumptionRate == null)
      oxygenConsumptionRate = new SEScalarVolumePerTime();
    return oxygenConsumptionRate;
  }
  
  public boolean hasRespiratoryExchangeRatio()
  {
    return respiratoryExchangeRatio == null ? false : respiratoryExchangeRatio.isValid();
  }
  public SEScalar getRespiratoryExchangeRatio()
  {
    if (respiratoryExchangeRatio == null)
      respiratoryExchangeRatio = new SEScalar();
    return respiratoryExchangeRatio;
  }
  
  public boolean hasLiverInsulinSetPoint()
  {
    return liverInsulinSetPoint == null ? false : liverInsulinSetPoint.isValid();
  }
  public SEScalarAmountPerVolume getLiverInsulinSetPoint()
  {
    if (liverInsulinSetPoint == null)
    	liverInsulinSetPoint = new SEScalarAmountPerVolume();
    return liverInsulinSetPoint;
  }
  
  public boolean hasLiverGlucagonSetPoint()
  {
    return liverGlucagonSetPoint == null ? false : liverGlucagonSetPoint.isValid();
  }
  public SEScalarMassPerVolume getLiverGlucagonSetPoint()
  {
    if (liverGlucagonSetPoint == null)
    	liverGlucagonSetPoint = new SEScalarMassPerVolume();
    return liverGlucagonSetPoint;
  }
  
  public boolean hasMuscleInsulinSetPoint()
  {
    return muscleInsulinSetPoint == null ? false : muscleInsulinSetPoint.isValid();
  }
  public SEScalarAmountPerVolume getMuscleInsulinSetPoint()
  {
    if (muscleInsulinSetPoint == null)
    	muscleInsulinSetPoint = new SEScalarAmountPerVolume();
    return muscleInsulinSetPoint;
  }
  
  public boolean hasMuscleGlucagonSetPoint()
  {
    return muscleGlucagonSetPoint == null ? false : muscleGlucagonSetPoint.isValid();
  }
  public SEScalarMassPerVolume getMuscleGlucagonSetPoint()
  {
    if (muscleGlucagonSetPoint == null)
    	muscleGlucagonSetPoint = new SEScalarMassPerVolume();
    return muscleGlucagonSetPoint;
  }

  public boolean hasFatInsulinSetPoint()
  {
    return fatInsulinSetPoint == null ? false : fatInsulinSetPoint.isValid();
  }
  public SEScalarAmountPerVolume getFatInsulinSetPoint()
  {
    if (fatInsulinSetPoint == null)
    	fatInsulinSetPoint = new SEScalarAmountPerVolume();
    return fatInsulinSetPoint;
  }
  
  public boolean hasFatGlucagonSetPoint()
  {
    return fatGlucagonSetPoint == null ? false : fatGlucagonSetPoint.isValid();
  }
  public SEScalarMassPerVolume getFatGlucagonSetPoint()
  {
    if (fatGlucagonSetPoint == null)
    	fatGlucagonSetPoint = new SEScalarMassPerVolume();
    return fatGlucagonSetPoint;
  }
  
  public boolean hasLiverGlycogen()
  {
    return liverGlycogen == null ? false : liverGlycogen.isValid();
  }
  public SEScalarMass getLiverGlycogen()
  {
    if (liverGlycogen == null)
    	liverGlycogen = new SEScalarMass();
    return liverGlycogen;
  }
  
  public boolean hasMuscleGlycogen()
  {
    return muscleGlycogen == null ? false : muscleGlycogen.isValid();
  }
  public SEScalarMass getMuscleGlycogen()
  {
    if (muscleGlycogen == null)
    	muscleGlycogen = new SEScalarMass();
    return muscleGlycogen;
  }
  
  public boolean hasStoredProtein()
  {
    return storedProtein == null ? false : storedProtein.isValid();
  }
  public SEScalarMass getStoredProtein()
  {
    if (storedProtein == null)
    	storedProtein = new SEScalarMass();
    return storedProtein;
  }
  
  public boolean hasStoredFat()
  {
    return storedFat == null ? false : storedFat.isValid();
  }
  public SEScalarMass getStoredFat()
  {
    if (storedFat == null)
    	storedFat = new SEScalarMass();
    return storedFat;
  }
}
