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

import java.util.*;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.utilities.StringUtils;


public class SESubstance
{
	protected EnumSubstanceState               state;
	protected EnumSubstanceClass							 subClass;
	protected String                           name;
	protected SEScalarMassPerVolume            density;
  protected SEScalarMassPerAmount            molarMass;
  // Diffusion related-ish
  protected SEScalarMassPerAreaTime          maximumDiffusionFlux;
  protected SEScalar                         michaelisCoefficient;
  protected SEScalarElectricResistance			 membraneResistance;
  // Liquid related-ish
  protected SESubstanceAerosolization        aerosolization;
  protected SEScalarMassPerVolume            bloodConcentration;
  protected SEScalarMass                     massInBody;  
  protected SEScalarMass                     massInBlood;  
  protected SEScalarMass                     massInTissue;  
  protected SEScalarMassPerVolume            plasmaConcentration;
  protected SEScalarMassPerVolume						 effectSiteConcentration;
  protected SEScalarMass                     systemicMassCleared;
  protected SEScalarMassPerVolume            tissueConcentration;
  // Gas related-ish
	protected SEScalarVolumePerTime            alveolarTransfer;
	protected SEScalarVolumePerTimePressure    diffusingCapacity;
	protected SEScalarFraction                 endTidalFraction;
	protected SEScalarPressure                 endTidalPressure;
	protected SEScalar                         relativeDiffusionCoefficient;
	protected SEScalarInversePressure          solubilityCoefficient;
	
	protected SESubstanceClearance             clearance;
	protected SESubstancePharmacokinetics      pk;
	protected SESubstancePharmacodynamics      pd;
	
	public SESubstance()
	{
		
	}
	
	public void reset()
	{
		this.name=null;
		this.subClass=null;
		this.state=null;
		if(this.density!=null)
      this.density.invalidate();
    if(this.molarMass!=null)
			this.molarMass.invalidate();
    
    if(this.maximumDiffusionFlux!=null)
			this.maximumDiffusionFlux.invalidate();
    if(this.michaelisCoefficient!=null)
			this.michaelisCoefficient.invalidate();
    if(this.membraneResistance!=null)
    	this.membraneResistance.invalidate();
		
		if(this.aerosolization!=null)
      this.aerosolization.reset();
		if(this.bloodConcentration!=null)
       this.bloodConcentration.invalidate();
		if(this.massInBody!=null)
      this.massInBody.invalidate();
		if(this.massInBlood!=null)
      this.massInBlood.invalidate();
		if(this.massInTissue!=null)
      this.massInTissue.invalidate();
    if(this.plasmaConcentration!=null)
      this.plasmaConcentration.invalidate();
    if(this.effectSiteConcentration!=null)
      this.effectSiteConcentration.invalidate();
		if(this.systemicMassCleared!=null)
      this.systemicMassCleared.invalidate();
		if(this.tissueConcentration!=null)
      this.tissueConcentration.invalidate();
    
		if(this.alveolarTransfer!=null)
			this.alveolarTransfer.invalidate();
		if(this.diffusingCapacity!=null)
      this.diffusingCapacity.invalidate();
    if(this.endTidalFraction!=null)
			this.endTidalFraction.invalidate();
    if(this.endTidalPressure!=null)
			this.endTidalPressure.invalidate();
		if(this.relativeDiffusionCoefficient!=null)
      this.relativeDiffusionCoefficient.invalidate();
    if(this.solubilityCoefficient!=null)
			this.solubilityCoefficient.invalidate();
		
    if(this.clearance!=null)
      this.clearance.reset();
    if(this.pk!=null)
      this.pk.reset();
    if(this.pd!=null)
      this.pd.reset();    
	}
	
	public boolean load(SubstanceData data)
	{
		this.reset();
		if(data.getName()!=null)
			this.setName(data.getName());
		if(data.getState()!=null)
			this.setState(data.getState());
		if(data.getClassification()!=null)
			this.setSubClass(data.getClassification());
		if(data.getDensity()!=null)
      this.getDensity().load(data.getDensity());    
		if(data.getMolarMass()!=null)
			this.getMolarMass().load(data.getMolarMass());
		
		if(data.getMaximumDiffusionFlux()!=null)
			this.getMaximumDiffusionFlux().load(data.getMaximumDiffusionFlux());
		if(data.getMichaelisCoefficient()!=null)
			this.getMichaelisCoefficient().load(data.getMolarMass());
		if(data.getMembraneResistance()!=null)
			this.getMembraneResistance().load(data.getMembraneResistance());
				
		if(data.getAerosolization()!=null)
		  this.getAerosolization().load(data.getAerosolization());
		if(data.getBloodConcentration()!=null)
      this.getBloodConcentration().load(data.getBloodConcentration());
		if(data.getMassInBody()!=null)
      this.getMassInBody().load(data.getMassInBody());
		if(data.getMassInBlood()!=null)
      this.getMassInBlood().load(data.getMassInBlood());
		if(data.getMassInTissue()!=null)
      this.getMassInTissue().load(data.getMassInTissue());
    if(data.getPlasmaConcentration()!=null)
      this.getPlasmaConcentration().load(data.getPlasmaConcentration());
    if(data.getEffectSiteConcentration()!=null)
      this.getEffectSiteConcentration().load(data.getEffectSiteConcentration());
		if(data.getSystemicMassCleared()!=null)
      this.getSystemicMassCleared().load(data.getSystemicMassCleared());
		if(data.getTissueConcentration()!=null)
      this.getTissueConcentration().load(data.getTissueConcentration());
		
		if(data.getAlveolarTransfer()!=null)
		  this.getAlveolarTransfer().load(data.getAlveolarTransfer());
		if(data.getDiffusingCapacity()!=null)
		  this.getDiffusingCapacity().load(data.getDiffusingCapacity());
		if(data.getEndTidalFraction()!=null)
		  this.getEndTidalFraction().load(data.getEndTidalFraction());
		if(data.getEndTidalPressure()!=null)
		  this.getEndTidalPressure().load(data.getEndTidalPressure());
		if(data.getRelativeDiffusionCoefficient()!=null)
		  this.getRelativeDiffusionCoefficient().load(data.getRelativeDiffusionCoefficient());
		if(data.getSolubilityCoefficient()!=null)
		  this.getSolubilityCoefficient().load(data.getSolubilityCoefficient());

		if(data.getClearance()!=null)
      this.getClearance().load(data.getClearance());
    if(data.getPharmacokinetics()!=null)
      this.getPK().load(data.getPharmacokinetics());
		if(data.getPharmacodynamics()!=null)
      this.getPD().load(data.getPharmacodynamics());
    			
		return true;
	}
	
	public SubstanceData unload()
	{
		SubstanceData to = CDMSerializer.objFactory.createSubstanceData();
		unload(to);
		return to;
	}
	
	protected void unload(SubstanceData to)
	{
		if(hasName())
			to.setName(this.name);
		if(hasState())
			to.setState(this.state);
		if(hasSubClass())
			to.setClassification(this.subClass);
		if(hasDensity())
      to.setDensity(this.density.unload());    
		if(hasMolarMass())
			to.setMolarMass(this.molarMass.unload());
		
		if(hasMaximumDiffusionFlux())
			to.setMaximumDiffusionFlux(this.maximumDiffusionFlux.unload());
		if(hasMichaelisCoefficient())
			to.setMichaelisCoefficient(this.michaelisCoefficient.unload());
		if(hasMembraneResistance())
			to.setMembraneResistance(this.membraneResistance.unload());
				 
		if(hasAerosolization())
		  to.setAerosolization(this.aerosolization.unload());
		if(hasBloodConcentration())
      to.setBloodConcentration(this.bloodConcentration.unload());
		if(hasMassInBody())
      to.setMassInBody(this.massInBody.unload());
		if(hasMassInBlood())
      to.setMassInBlood(this.massInBlood.unload());
		if(hasMassInTissue())
      to.setMassInTissue(this.massInTissue.unload());
    if(hasPlasmaConcentration())
      to.setPlasmaConcentration(this.plasmaConcentration.unload());
    if(hasEffectSiteConcentration())
      to.setEffectSiteConcentration(this.effectSiteConcentration.unload());
    if(hasSystemicMassCleared())
      to.setSystemicMassCleared(this.systemicMassCleared.unload());
    if(hasTissueConcentration())
      to.setTissueConcentration(this.tissueConcentration.unload());
     
		if(hasAlveolarTransfer())
			to.setAlveolarTransfer(this.alveolarTransfer.unload());
		if(hasDiffusingCapacity())
      to.setDiffusingCapacity(this.diffusingCapacity.unload());
    if(hasEndTidalFraction())
			to.setEndTidalFraction(this.endTidalFraction.unload());
    if(hasEndTidalPressure())
			to.setEndTidalPressure(this.endTidalPressure.unload());
		if(hasRelativeDiffusionCoefficient())
      to.setRelativeDiffusionCoefficient(this.relativeDiffusionCoefficient.unload());
    if(hasSolubilityCoefficient())
			to.setSolubilityCoefficient(this.solubilityCoefficient.unload());
		
    if(hasClearance())
      to.setClearance(this.clearance.unload());
    if(hasPK())
      to.setPharmacokinetics(this.pk.unload());
    if(hasPD())
      to.setPharmacodynamics(this.pd.unload());    		
	}
	
	public String  getName() { return this.name;}
	public void    setName(String name){this.name=name;}
	public boolean hasName(){return StringUtils.exists(this.name);}
	
	public EnumSubstanceState  getState() { return this.state;}
	public void                setState(EnumSubstanceState state){this.state=state;}
	public boolean             hasState(){return this.state==null?false:true;}
	
	public EnumSubstanceClass getSubClass() { return this.subClass;}
	public void							  setSubClass(EnumSubstanceClass subClass){this.subClass=subClass;}
	public boolean						hasSubClass() {return this.subClass==null?false:true;}
	
	public SEScalarMassPerVolume getDensity() 
  { 
    if(this.density==null)
      this.density=new SEScalarMassPerVolume();
    return this.density;
  }
  public boolean hasDensity() {return this.density==null?false:this.density.isValid();}
  
	public SEScalarMassPerAmount getMolarMass() 
	{ 
		if(this.molarMass==null)
			this.molarMass=new SEScalarMassPerAmount();
		return this.molarMass;
	}
	public boolean hasMolarMass() {return this.molarMass==null?false:this.molarMass.isValid();}
	
  ////////////////
  // Diffusion-ish //
  ////////////////
	
	public SEScalarMassPerAreaTime getMaximumDiffusionFlux() 
	{ 
		if(this.maximumDiffusionFlux==null)
			this.maximumDiffusionFlux=new SEScalarMassPerAreaTime();
		return this.maximumDiffusionFlux;
	}
	public boolean hasMaximumDiffusionFlux() {return this.maximumDiffusionFlux==null?false:this.maximumDiffusionFlux.isValid();}
	
	
	public SEScalar getMichaelisCoefficient() 
	{ 
		if(this.michaelisCoefficient==null)
			this.michaelisCoefficient=new SEScalar();
		return this.michaelisCoefficient;
	}
	public boolean hasMichaelisCoefficient() {return this.michaelisCoefficient==null?false:this.michaelisCoefficient.isValid();}
	
	public SEScalarElectricResistance getMembraneResistance()
	{
		if(this.membraneResistance==null)
			this.membraneResistance=new SEScalarElectricResistance();
		return this.membraneResistance;
	}
	public boolean hasMembraneResistance() {return this.membraneResistance==null?false:this.membraneResistance.isValid();}

	////////////////
	// Liquid-ish //
	////////////////
	
	public SESubstanceAerosolization getAerosolization()
	{
	  if(this.aerosolization==null)
	    this.aerosolization = new SESubstanceAerosolization();
	  return this.aerosolization;
	}
	public boolean hasAerosolization() { return this.aerosolization!=null; }
	public void removeAerosolization() { this.aerosolization = null; }
		
	public SEScalarMassPerVolume getBloodConcentration() 
  { 
    if(this.bloodConcentration==null)
      this.bloodConcentration=new SEScalarMassPerVolume();
    return this.bloodConcentration;
  }
  public boolean hasBloodConcentration() {return this.bloodConcentration==null?false:this.bloodConcentration.isValid();}
  
	public SEScalarMass getMassInBody() 
  { 
    if(this.massInBody==null)
      this.massInBody=new SEScalarMass();
    return this.massInBody;
  }
  public boolean hasMassInBody() {return this.massInBody==null?false:this.massInBody.isValid();}
  
  public SEScalarMass getMassInBlood() 
  { 
    if(this.massInBlood==null)
      this.massInBlood=new SEScalarMass();
    return this.massInBlood;
  }
  public boolean hasMassInBlood() {return this.massInBlood==null?false:this.massInBlood.isValid();}
  
  public SEScalarMass getMassInTissue() 
  { 
    if(this.massInTissue==null)
      this.massInTissue=new SEScalarMass();
    return this.massInTissue;
  }
  public boolean hasMassInTissue() {return this.massInTissue==null?false:this.massInTissue.isValid();}
  
  public SEScalarMassPerVolume getPlasmaConcentration() 
  { 
    if(this.plasmaConcentration==null)
      this.plasmaConcentration=new SEScalarMassPerVolume();
    return this.plasmaConcentration;
  }
  public boolean hasPlasmaConcentration() {return this.plasmaConcentration==null?false:this.plasmaConcentration.isValid();}
  
  public SEScalarMassPerVolume getEffectSiteConcentration() 
  { 
    if(this.effectSiteConcentration==null)
      this.effectSiteConcentration=new SEScalarMassPerVolume();
    return this.effectSiteConcentration;
  }
  public boolean hasEffectSiteConcentration() {return this.effectSiteConcentration==null?false:this.effectSiteConcentration.isValid();}
  
  public SEScalarMass getSystemicMassCleared() 
  { 
    if(this.systemicMassCleared==null)
      this.systemicMassCleared=new SEScalarMass();
    return this.systemicMassCleared;
  }
  public boolean hasSystemicMassCleared() {return this.systemicMassCleared==null?false:this.systemicMassCleared.isValid();}

  public SEScalarMassPerVolume getTissueConcentration() 
  { 
    if(this.tissueConcentration==null)
      this.tissueConcentration=new SEScalarMassPerVolume();
    return this.tissueConcentration;
  }
  public boolean hasTissueConcentration() {return this.tissueConcentration==null?false:this.tissueConcentration.isValid();}
  
  /////////////
	// Gas-ish //
  /////////////
  
	public SEScalarVolumePerTime getAlveolarTransfer() 
	{ 
		if(this.alveolarTransfer==null)
			this.alveolarTransfer=new SEScalarVolumePerTime();
		return this.alveolarTransfer;
	}
	public boolean hasAlveolarTransfer() {return this.alveolarTransfer==null?false:this.alveolarTransfer.isValid();}
	
	public SEScalarVolumePerTimePressure getDiffusingCapacity() 
  { 
    if(this.diffusingCapacity==null)
      this.diffusingCapacity=new SEScalarVolumePerTimePressure();
    return this.diffusingCapacity;
  }
  public boolean hasDiffusingCapacity() {return this.diffusingCapacity==null?false:this.diffusingCapacity.isValid();}
  
	public SEScalarFraction getEndTidalFraction() 
	{ 
		if(this.endTidalFraction==null)
			this.endTidalFraction=new SEScalarFraction();
		return this.endTidalFraction;
	}
	public boolean hasEndTidalFraction() {return this.endTidalFraction==null?false:this.endTidalFraction.isValid();}
	
	public SEScalarPressure getEndTidalPressure() 
	{ 
		if(this.endTidalPressure==null)
			this.endTidalPressure=new SEScalarPressure();
		return this.endTidalPressure;
	}
	public boolean hasEndTidalPressure() {return this.endTidalPressure==null?false:this.endTidalPressure.isValid();}
	
	public SEScalarInversePressure getSolubilityCoefficient() 
	{ 
		if(this.solubilityCoefficient==null)
			this.solubilityCoefficient=new SEScalarInversePressure();
		return this.solubilityCoefficient;
	}
	public boolean hasSolubilityCoefficient() {return this.solubilityCoefficient==null?false:this.solubilityCoefficient.isValid();}
	
	public SEScalar getRelativeDiffusionCoefficient() 
	{ 
		if(this.relativeDiffusionCoefficient==null)
			this.relativeDiffusionCoefficient=new SEScalar();
		return this.relativeDiffusionCoefficient;
	}
	public boolean hasRelativeDiffusionCoefficient() {return this.relativeDiffusionCoefficient==null?false:this.relativeDiffusionCoefficient.isValid();}
	
	///////////////
	// Clearance //
	///////////////

	public SESubstanceClearance getClearance()
	{
	  if(this.clearance==null)
	    this.clearance = new SESubstanceClearance();
	  return this.clearance;
	}
	public boolean hasClearance() { return this.clearance!=null; }
	public void removeClearance() { this.clearance = null; }

	///////////
	// PK/PD //
	///////////
	
	public SESubstancePharmacokinetics getPK()
  {
    if(this.pk==null)
      this.pk = new SESubstancePharmacokinetics();
    return this.pk;
  }
  public boolean hasPK() { return this.pk!=null; }
  public void removePK() { this.pk = null; }
  
  public SESubstancePharmacodynamics getPD()
  {
    if(this.pd==null)
      this.pd = new SESubstancePharmacodynamics();
    return this.pd;
  }
  public boolean hasPD() { return this.pd!=null; }
  public void removePD() { this.pd = null; }
	
	
	
}
