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
import mil.tatrc.physiology.datamodel.bind.BloodChemistrySystemData;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SEBloodChemistrySystem extends SEPhysiologySystem implements SESystem
{
  protected SEScalarMassPerVolume          bloodDensity;
  protected SEScalar                       bloodPH;
  protected SEScalarHeatCapacitancePerMass bloodSpecificHeat;
  protected SEScalarMassPerVolume          bloodUreaNitrogenConcentration;
  protected SEScalarFraction               carbonDioxideSaturation;
  protected SEScalarFraction               carbonMonoxideSaturation;
  protected SEScalarFraction               hematocrit;
  protected SEScalarMass                   hemoglobinContent;
  protected SEScalarFraction               oxygenSaturation;
  protected SEScalarVolume                 plasmaVolume;
  protected SEScalarAmountPerVolume        phosphate;
  protected SEScalarFraction               pulseOximetry;
  protected SEScalarAmountPerVolume        redBloodCellCount;
  protected SEScalarFraction               shuntFraction;
  protected SEScalarAmountPerVolume        strongIonDifference;
  protected SEScalarMassPerVolume          totalProteinConcentration;
  protected SEScalarFraction               volumeFractionNeutralLipidInPlasma;
  protected SEScalarFraction               volumeFractionNeutralPhospholipidInPlasma;
  protected SEScalarAmountPerVolume        whiteBloodCellCount;
  protected SEScalarAmountPerVolume				redBloodCellAcetylcholinesterase;

  protected SEScalarPressure               arterialCarbonDioxidePressure;
  protected SEScalarPressure               arterialOxygenPressure;
  protected SEScalarPressure               pulmonaryArterialOxygenPressure;
  protected SEScalarPressure               pulmonaryArterialCarbonDioxidePressure;
  protected SEScalarPressure               pulmonaryVenousOxygenPressure;
  protected SEScalarPressure               pulmonaryVenousCarbonDioxidePressure;
  protected SEScalarPressure               venousOxygenPressure;
  protected SEScalarPressure               venousCarbonDioxidePressure;

  public SEBloodChemistrySystem()
  {
    bloodDensity = null;
    bloodPH = null;
    bloodSpecificHeat = null;
    bloodUreaNitrogenConcentration = null;
    carbonDioxideSaturation = null;
    carbonMonoxideSaturation = null;
    hematocrit = null;
    hemoglobinContent = null;
    oxygenSaturation = null;
    phosphate = null;
    plasmaVolume = null;
    pulseOximetry = null;
    redBloodCellAcetylcholinesterase = null;
    redBloodCellCount = null;
    shuntFraction = null;
    strongIonDifference = null;
    totalProteinConcentration = null;
    volumeFractionNeutralLipidInPlasma = null;
    volumeFractionNeutralPhospholipidInPlasma = null;
    whiteBloodCellCount = null;
   

    arterialCarbonDioxidePressure = null;
    arterialOxygenPressure = null;
    pulmonaryArterialOxygenPressure = null;
    pulmonaryArterialCarbonDioxidePressure = null;
    pulmonaryVenousOxygenPressure = null;
    pulmonaryVenousCarbonDioxidePressure = null;
    venousOxygenPressure = null;
    venousCarbonDioxidePressure = null;
  }

  public void reset()
  {
    if (bloodDensity != null)
      bloodDensity.invalidate();
    if (bloodPH != null)
      bloodPH.invalidate();
    if (bloodSpecificHeat != null)
      bloodSpecificHeat.invalidate();
    if (bloodUreaNitrogenConcentration != null)
      bloodUreaNitrogenConcentration.invalidate();
    if (carbonDioxideSaturation != null)
      carbonDioxideSaturation.invalidate();
    if (carbonMonoxideSaturation != null)
    	carbonMonoxideSaturation.invalidate();
    if (hematocrit != null)
      hematocrit.invalidate();
    if (hemoglobinContent != null)
      hemoglobinContent.invalidate();
    if (oxygenSaturation != null)
      oxygenSaturation.invalidate();
    if (phosphate != null)
    	phosphate.invalidate();
    if (plasmaVolume != null)
      plasmaVolume.invalidate();
    if (pulseOximetry != null)
    	pulseOximetry.invalidate();
    if (redBloodCellAcetylcholinesterase != null)
      redBloodCellAcetylcholinesterase.invalidate();
    if (redBloodCellCount != null)
      redBloodCellCount.invalidate();
    if (shuntFraction != null)
      shuntFraction.invalidate();
    if (strongIonDifference != null)
    	strongIonDifference.invalidate();
    if (totalProteinConcentration != null)
      totalProteinConcentration.invalidate();
    if (volumeFractionNeutralLipidInPlasma != null)
      volumeFractionNeutralLipidInPlasma.invalidate();
    if (volumeFractionNeutralPhospholipidInPlasma != null)
      volumeFractionNeutralPhospholipidInPlasma.invalidate();
    if (whiteBloodCellCount != null)
      whiteBloodCellCount.invalidate();


    if (arterialCarbonDioxidePressure != null)
      arterialCarbonDioxidePressure.invalidate();
    if (arterialOxygenPressure != null)
      arterialOxygenPressure.invalidate();
    if (pulmonaryArterialOxygenPressure != null)
      pulmonaryArterialOxygenPressure.invalidate();
    if (pulmonaryArterialCarbonDioxidePressure != null)
      pulmonaryArterialCarbonDioxidePressure.invalidate();
    if (pulmonaryVenousOxygenPressure != null)
      pulmonaryVenousOxygenPressure.invalidate();
    if (pulmonaryVenousCarbonDioxidePressure != null)
      pulmonaryVenousCarbonDioxidePressure.invalidate();
    if (venousOxygenPressure != null)
      venousOxygenPressure.invalidate();
    if (venousCarbonDioxidePressure != null)
      venousCarbonDioxidePressure.invalidate();
  }

  public boolean load(BloodChemistrySystemData in)
  {
    if (in.getBloodDensity() != null)
      getBloodDensity().load(in.getBloodDensity());
    if (in.getBloodPH() != null)
      getBloodPH().load(in.getBloodPH());
    if (in.getBloodSpecificHeat() != null)
      getBloodSpecificHeat().load(in.getBloodSpecificHeat());
    if (in.getBloodUreaNitrogenConcentration() != null)
      getBloodUreaNitrogenConcentration().load(in.getBloodUreaNitrogenConcentration());
    if (in.getCarbonDioxideSaturation() != null)
      getCarbonDioxideSaturation().load(in.getCarbonDioxideSaturation());
    if (in.getCarbonMonoxideSaturation() != null)
    	getCarbonMonoxideSaturation().load(in.getCarbonMonoxideSaturation());
    if (in.getHematocrit() != null)
      getHematocrit().load(in.getHematocrit());
    if (in.getHemoglobinContent() != null)
      getHemoglobinContent().load(in.getHemoglobinContent());
    if (in.getOxygenSaturation() != null)
      getOxygenSaturation().load(in.getOxygenSaturation());
    if (in.getPhosphate() != null)
    	getPhosphate().load(in.getPhosphate());  
    if (in.getPlasmaVolume() != null)
      getPlasmaVolume().load(in.getPlasmaVolume());  
    if (in.getPulseOximetry() != null)
    	getPulseOximetry().load(in.getPulseOximetry());  
    if (in.getRedBloodCellAcetylcholinesterase() != null)
      getRedBloodCellAcetylcholinesterase().load(in.getRedBloodCellAcetylcholinesterase());
    if (in.getRedBloodCellCount() != null)
      getRedBloodCellCount().load(in.getRedBloodCellCount());
    if (in.getShuntFraction() != null)
      getShuntFraction().load(in.getShuntFraction());
    if (in.getStrongIonDifference() != null)
    	getStrongIonDifference().load(in.getStrongIonDifference());
    if (in.getTotalProteinConcentration() != null)
      getTotalProteinConcentration().load(in.getTotalProteinConcentration());
    if (in.getVolumeFractionNeutralLipidInPlasma() != null)
      getVolumeFractionNeutralLipidInPlasma().load(in.getVolumeFractionNeutralLipidInPlasma());
    if (in.getVolumeFractionNeutralPhospholipidInPlasma() != null)
      getVolumeFractionNeutralPhospholipidInPlasma().load(in.getVolumeFractionNeutralPhospholipidInPlasma());
    if (in.getWhiteBloodCellCount() != null)
      getWhiteBloodCellCount().load(in.getWhiteBloodCellCount());


    if (in.getArterialCarbonDioxidePressure() != null)
      getArterialCarbonDioxidePressure().load(in.getArterialCarbonDioxidePressure());
    if (in.getArterialOxygenPressure() != null)
      getArterialOxygenPressure().load(in.getArterialOxygenPressure());
    if (in.getPulmonaryArterialCarbonDioxidePressure() != null)
      getPulmonaryArterialCarbonDioxidePressure().load(in.getPulmonaryArterialCarbonDioxidePressure());
    if (in.getPulmonaryArterialOxygenPressure() != null)
      getPulmonaryArterialOxygenPressure().load(in.getPulmonaryArterialOxygenPressure());   
    if (in.getPulmonaryVenousCarbonDioxidePressure() != null)
      getPulmonaryVenousCarbonDioxidePressure().load(in.getPulmonaryVenousCarbonDioxidePressure());  
    if (in.getPulmonaryVenousOxygenPressure() != null)
      getPulmonaryVenousOxygenPressure().load(in.getPulmonaryVenousOxygenPressure());
    if (in.getVenousOxygenPressure() != null)
      getVenousOxygenPressure().load(in.getVenousOxygenPressure());
    if (in.getVenousCarbonDioxidePressure() != null)
      getVenousCarbonDioxidePressure().load(in.getVenousCarbonDioxidePressure());

    return true;
  }

  public BloodChemistrySystemData unload()
  {
    BloodChemistrySystemData data = CDMSerializer.objFactory.createBloodChemistrySystemData();
    unload(data);
    return data;
  }

  protected void unload(BloodChemistrySystemData data)
  {
    if (bloodDensity != null)
      data.setBloodDensity(bloodDensity.unload());
    if (bloodPH != null)
      data.setBloodPH(bloodPH.unload());
    if (bloodSpecificHeat != null)
      data.setBloodSpecificHeat(bloodSpecificHeat.unload());
    if (bloodUreaNitrogenConcentration != null)
      data.setBloodUreaNitrogenConcentration(bloodUreaNitrogenConcentration.unload());
    if (carbonDioxideSaturation != null)
      data.setCarbonDioxideSaturation(carbonDioxideSaturation.unload());
    if (carbonMonoxideSaturation != null)
      data.setCarbonMonoxideSaturation(carbonMonoxideSaturation.unload());
    if (hematocrit != null)
      data.setHematocrit(hematocrit.unload());
    if (hemoglobinContent != null)
      data.setHemoglobinContent(hemoglobinContent.unload());
    if (oxygenSaturation != null)
      data.setOxygenSaturation(oxygenSaturation.unload());
    if (phosphate != null)
      data.setPhosphate(phosphate.unload());
    if (plasmaVolume != null)
      data.setPlasmaVolume(plasmaVolume.unload());
    if (pulseOximetry != null)
      data.setPulseOximetry(pulseOximetry.unload());
    if (redBloodCellAcetylcholinesterase != null)
      data.setRedBloodCellAcetylcholinesterase(redBloodCellAcetylcholinesterase.unload());
    if (redBloodCellCount != null)
      data.setRedBloodCellCount(redBloodCellCount.unload());
    if (shuntFraction != null)
      data.setShuntFraction(shuntFraction.unload());
    if (strongIonDifference != null)
      data.setStrongIonDifference(strongIonDifference.unload());
    if (totalProteinConcentration != null)
      data.setTotalProteinConcentration(totalProteinConcentration.unload());
    if (volumeFractionNeutralLipidInPlasma != null)
      data.setVolumeFractionNeutralLipidInPlasma(volumeFractionNeutralLipidInPlasma.unload());
    if (volumeFractionNeutralPhospholipidInPlasma != null)
      data.setVolumeFractionNeutralPhospholipidInPlasma(volumeFractionNeutralPhospholipidInPlasma.unload());
    if (whiteBloodCellCount != null)
      data.setWhiteBloodCellCount(whiteBloodCellCount.unload());

    if (arterialCarbonDioxidePressure != null)
      data.setArterialCarbonDioxidePressure(arterialCarbonDioxidePressure.unload());
    if (arterialOxygenPressure != null)
      data.setArterialOxygenPressure(arterialOxygenPressure.unload());
    if (pulmonaryArterialOxygenPressure != null)
      data.setPulmonaryArterialOxygenPressure(pulmonaryArterialOxygenPressure.unload());
    if (pulmonaryArterialCarbonDioxidePressure != null)
      data.setPulmonaryArterialCarbonDioxidePressure(pulmonaryArterialCarbonDioxidePressure.unload());
    if (pulmonaryVenousOxygenPressure != null)
      data.setPulmonaryVenousOxygenPressure(pulmonaryVenousOxygenPressure.unload());
    if (pulmonaryVenousCarbonDioxidePressure != null)
      data.setPulmonaryVenousCarbonDioxidePressure(pulmonaryVenousCarbonDioxidePressure.unload());
    if (venousOxygenPressure != null)
      data.setVenousOxygenPressure(venousOxygenPressure.unload());
    if (venousCarbonDioxidePressure != null)
      data.setVenousCarbonDioxidePressure(venousCarbonDioxidePressure.unload());
  }

  public boolean hasBloodDensity()
  {
    return bloodDensity == null ? false : bloodDensity.isValid();
  }
  public SEScalarMassPerVolume getBloodDensity()
  {
    if (bloodDensity == null)
      bloodDensity = new SEScalarMassPerVolume();
    return bloodDensity;
  }

  public boolean hasBloodPH()
  {
    return bloodPH == null ? false : bloodPH.isValid();
  }
  public SEScalar getBloodPH()
  {
    if (bloodPH == null)
      bloodPH = new SEScalar();
    return bloodPH;  }

  public boolean hasBloodSpecificHeat()
  {
    return bloodSpecificHeat == null ? false : bloodSpecificHeat.isValid();
  }
  public SEScalarHeatCapacitancePerMass getBloodSpecificHeat()
  {
    if (bloodSpecificHeat == null)
      bloodSpecificHeat = new SEScalarHeatCapacitancePerMass();
    return bloodSpecificHeat;
  }
  
  public boolean hasBloodUreaNitrogenConcentration()
  {
    return bloodUreaNitrogenConcentration == null ? false : bloodUreaNitrogenConcentration.isValid();
  }
  public SEScalarMassPerVolume getBloodUreaNitrogenConcentration()
  {
    if (bloodUreaNitrogenConcentration == null)
      bloodUreaNitrogenConcentration = new SEScalarMassPerVolume();
    return bloodUreaNitrogenConcentration;
  }

  public boolean hasCarbonDioxideSaturation()
  {
    return carbonDioxideSaturation == null ? false : carbonDioxideSaturation.isValid();
  }
  public SEScalarFraction getCarbonDioxideSaturation()
  {
    if (carbonDioxideSaturation == null)
      carbonDioxideSaturation = new SEScalarFraction();
    return carbonDioxideSaturation;
  }
  
  public boolean hasCarbonMonoxideSaturation()
  {
    return carbonMonoxideSaturation == null ? false : carbonMonoxideSaturation.isValid();
  }
  public SEScalarFraction getCarbonMonoxideSaturation()
  {
    if (carbonMonoxideSaturation == null)
    	carbonMonoxideSaturation = new SEScalarFraction();
    return carbonMonoxideSaturation;
  }

  public boolean hasHematocrit()
  {
    return hematocrit == null ? false : hematocrit.isValid();
  }
  public SEScalarFraction getHematocrit()
  {
    if (hematocrit == null)
      hematocrit = new SEScalarFraction();
    return hematocrit;  }

  public boolean hasHemoglobinContent()
  {
    return hemoglobinContent == null ? false : hemoglobinContent.isValid();
  }
  public SEScalarMass getHemoglobinContent()
  {
    if (hemoglobinContent == null)
      hemoglobinContent = new SEScalarMass();
    return hemoglobinContent;  }

  public boolean hasOxygenSaturation()
  {
    return oxygenSaturation == null ? false : oxygenSaturation.isValid();
  }
  public SEScalarFraction getOxygenSaturation()
  {
    if (oxygenSaturation == null)
      oxygenSaturation = new SEScalarFraction();
    return oxygenSaturation;  }

  public boolean hasPhosphate()
  {
    return phosphate == null ? false : phosphate.isValid();
  }
  public SEScalarAmountPerVolume getPhosphate()
  {
    if (phosphate == null)
    	phosphate = new SEScalarAmountPerVolume();
    return phosphate;
  }
  
  public boolean hasPlasmaVolume()
  {
    return plasmaVolume == null ? false : plasmaVolume.isValid();
  }
  public SEScalarVolume getPlasmaVolume()
  {
    if (plasmaVolume == null)
      plasmaVolume = new SEScalarVolume();
    return plasmaVolume;
  }
  
  public boolean hasPulseOximetry()
  {
    return pulseOximetry == null ? false : pulseOximetry.isValid();
  }
  public SEScalarFraction getPulseOximetry()
  {
    if (pulseOximetry == null)
    	pulseOximetry = new SEScalarFraction();
    return pulseOximetry;
  }

  public boolean hasRedBloodCellAcetylcholinesterase()
  {
    return redBloodCellAcetylcholinesterase == null ? false : redBloodCellAcetylcholinesterase.isValid();
  }
  public SEScalarAmountPerVolume getRedBloodCellAcetylcholinesterase()
  {
    if (redBloodCellAcetylcholinesterase == null)
      redBloodCellAcetylcholinesterase = new SEScalarAmountPerVolume();
    return redBloodCellAcetylcholinesterase;
  }
  
  public boolean hasRedBloodCellCount()
  {
    return redBloodCellCount == null ? false : redBloodCellCount.isValid();
  }
  public SEScalarAmountPerVolume getRedBloodCellCount()
  {
    if (redBloodCellCount == null)
      redBloodCellCount = new SEScalarAmountPerVolume();
    return redBloodCellCount;  }

  public boolean hasShuntFraction()
  {
    return shuntFraction == null ? false : shuntFraction.isValid();
  }
  public SEScalarFraction getShuntFraction()
  {
    if (shuntFraction == null)
      shuntFraction = new SEScalarFraction();
    return shuntFraction;  }
  
  public boolean hasStrongIonDifference()
  {
    return strongIonDifference == null ? false : strongIonDifference.isValid();
  }
  public SEScalarAmountPerVolume getStrongIonDifference()
  {
    if (strongIonDifference == null)
    	strongIonDifference = new SEScalarAmountPerVolume();
    return strongIonDifference;
  }
  
  public boolean hasTotalProteinConcentration()
  {
    return totalProteinConcentration == null ? false : totalProteinConcentration.isValid();
  }
  public SEScalarMassPerVolume getTotalProteinConcentration()
  {
    if (totalProteinConcentration == null)
      totalProteinConcentration = new SEScalarMassPerVolume();
    return totalProteinConcentration;
  }

  public boolean hasVolumeFractionNeutralLipidInPlasma()
  {
    return volumeFractionNeutralLipidInPlasma == null ? false : volumeFractionNeutralLipidInPlasma.isValid();
  }
  public SEScalarFraction getVolumeFractionNeutralLipidInPlasma()
  {
    if (volumeFractionNeutralLipidInPlasma == null)
      volumeFractionNeutralLipidInPlasma = new SEScalarFraction();
    return volumeFractionNeutralLipidInPlasma;
  }

  public boolean hasVolumeFractionNeutralPhospholipidInPlasma()
  {
    return volumeFractionNeutralPhospholipidInPlasma == null ? false : volumeFractionNeutralPhospholipidInPlasma.isValid();
  }
  public SEScalarFraction getVolumeFractionNeutralPhospholipidInPlasma()
  {
    if (volumeFractionNeutralPhospholipidInPlasma == null)
      volumeFractionNeutralPhospholipidInPlasma = new SEScalarFraction();
    return volumeFractionNeutralPhospholipidInPlasma;
  }

  public boolean hasWhiteBloodCellCount()
  {
    return whiteBloodCellCount == null ? false : whiteBloodCellCount.isValid();
  }
  public SEScalarAmountPerVolume getWhiteBloodCellCount()
  {
    if (whiteBloodCellCount == null)
      whiteBloodCellCount = new SEScalarAmountPerVolume();
    return whiteBloodCellCount;
  }
  

  public boolean hasArterialCarbonDioxidePressure()
  {
    return arterialCarbonDioxidePressure == null ? false : arterialCarbonDioxidePressure.isValid();
  }
  public SEScalarPressure getArterialCarbonDioxidePressure()
  {
    if (arterialCarbonDioxidePressure == null)
      arterialCarbonDioxidePressure = new SEScalarPressure();
    return arterialCarbonDioxidePressure;
  }

  public boolean hasArterialOxygenPressure()
  {
    return arterialOxygenPressure == null ? false : arterialOxygenPressure.isValid();
  }
  public SEScalarPressure getArterialOxygenPressure()
  {
    if (arterialOxygenPressure == null)
      arterialOxygenPressure = new SEScalarPressure();
    return arterialOxygenPressure;
  }

  public boolean hasPulmonaryArterialCarbonDioxidePressure()
  {
    return pulmonaryArterialCarbonDioxidePressure == null ? false : pulmonaryArterialCarbonDioxidePressure.isValid();
  }
  public SEScalarPressure getPulmonaryArterialCarbonDioxidePressure()
  {
    if (pulmonaryArterialCarbonDioxidePressure == null)
      pulmonaryArterialCarbonDioxidePressure = new SEScalarPressure();
    return pulmonaryArterialCarbonDioxidePressure;
  }

  public boolean hasPulmonaryArterialOxygenPressure()
  {
    return pulmonaryArterialOxygenPressure == null ? false : pulmonaryArterialOxygenPressure.isValid();
  }
  public SEScalarPressure getPulmonaryArterialOxygenPressure()
  {
    if (pulmonaryArterialOxygenPressure == null)
      pulmonaryArterialOxygenPressure = new SEScalarPressure();
    return pulmonaryArterialOxygenPressure;
  }

  public boolean hasPulmonaryVenousCarbonDioxidePressure()
  {
    return pulmonaryVenousCarbonDioxidePressure == null ? false : pulmonaryVenousCarbonDioxidePressure.isValid();
  }
  public SEScalarPressure getPulmonaryVenousCarbonDioxidePressure()
  {
    if (pulmonaryVenousCarbonDioxidePressure == null)
      pulmonaryVenousCarbonDioxidePressure = new SEScalarPressure();
    return pulmonaryVenousCarbonDioxidePressure;
  }

  public boolean hasPulmonaryVenousOxygenPressure()
  {
    return pulmonaryVenousOxygenPressure == null ? false : pulmonaryVenousOxygenPressure.isValid();
  }
  public SEScalarPressure getPulmonaryVenousOxygenPressure()
  {
    if (pulmonaryVenousOxygenPressure == null)
      pulmonaryVenousOxygenPressure = new SEScalarPressure();
    return pulmonaryVenousOxygenPressure;  }

  public boolean hasVenousCarbonDioxidePressure()
  {
    return venousCarbonDioxidePressure == null ? false : venousCarbonDioxidePressure.isValid();
  }
  public SEScalarPressure getVenousCarbonDioxidePressure()
  {
    if (venousCarbonDioxidePressure == null)
      venousCarbonDioxidePressure = new SEScalarPressure();
    return venousCarbonDioxidePressure;
  }

  public boolean hasVenousOxygenPressure()
  {
    return venousOxygenPressure == null ? false : venousOxygenPressure.isValid();
  }
  public SEScalarPressure getVenousOxygenPressure()
  {
    if (venousOxygenPressure == null)
      venousOxygenPressure = new SEScalarPressure();
    return venousOxygenPressure;  }
}
