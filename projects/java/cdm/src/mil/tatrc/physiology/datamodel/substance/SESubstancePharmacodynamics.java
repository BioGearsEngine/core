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
import mil.tatrc.physiology.datamodel.system.physiology.SEPupillaryResponse;

public class SESubstancePharmacodynamics
{
	protected SEScalarFraction      bronchodilation;
	protected SEScalarFraction      diastolicPressureModifier;
	protected SEScalarMassPerVolume EC50;
	protected SEScalar              EMaxShapeParameter;
	protected SEScalarFraction      heartRateModifier;
	protected SEScalarFraction      neuromuscularBlock;
	protected SEPupillaryResponse   pupillaryResponse;
	protected SEScalarFraction      respirationRateModifier;
	protected SEScalarFraction      sedation;
	protected SEScalarFraction      systolicPressureModifier;
	protected SEScalarFraction      tidalVolumeModifier;
	protected SEScalarFraction      tubularPermeabilityModifier;
	protected SEScalarFraction		  centralNervousModifier;
	protected SEScalarFrequency			effectSiteRateConstant;
	
	public SESubstancePharmacodynamics()
	{
		
	}
	
	public void reset()
	{
		if(this.bronchodilation!=null)
      this.bronchodilation.invalidate();
		if(this.diastolicPressureModifier!=null)
      this.diastolicPressureModifier.invalidate();
		if(this.EC50!=null)
      this.EC50.invalidate();
		if(this.EMaxShapeParameter!=null)
			this.EMaxShapeParameter.invalidate();
		if(this.heartRateModifier!=null)
      this.heartRateModifier.invalidate();
		if(this.neuromuscularBlock!=null)
      this.neuromuscularBlock.invalidate();
		if(this.pupillaryResponse!=null)
      this.pupillaryResponse.reset();
		if(this.respirationRateModifier!=null)
      this.respirationRateModifier.invalidate();
		if(this.sedation!=null)
      this.sedation.invalidate();
		if(this.systolicPressureModifier!=null)
      this.systolicPressureModifier.invalidate();
		if(this.tidalVolumeModifier!=null)
      this.tidalVolumeModifier.invalidate();
		if(this.tubularPermeabilityModifier!=null)
      this.tubularPermeabilityModifier.invalidate();
    if(this.centralNervousModifier!=null)
      this.centralNervousModifier.invalidate();
    if(this.effectSiteRateConstant!=null)
    	this.effectSiteRateConstant.invalidate();
	}
	
	public boolean isValid()
  {
    if(!hasBronchodilation() || 
       !hasDiastolicPressureModifier() ||
       !hasEC50() ||
       !hasEMaxShapeParameter() ||
       !hasHeartRateModifier() ||
       !hasNeuromuscularBlock() ||
       !hasPupillaryResponse() ||
       !hasRespirationRateModifier() ||
       !hasSedation() ||
       !hasSystolicPressureModifier() ||
       !hasTidalVolumeModifier() ||
       !hasTubularPermeabilityModifier() ||
       !hasCentralNervousModifier()||
       !hasEffectSiteRateConstant())
      return false;
    return true;
  }
	
	public boolean load(SubstancePharmacodynamicsData data)
	{
		this.reset();
		
		if(data.getBronchodilation()!=null)
      this.getBronchodilation().load(data.getBronchodilation());
		if(data.getDiastolicPressureModifier()!=null)
      this.getDiastolicPressureModifier().load(data.getDiastolicPressureModifier());
		if(data.getEC50()!=null)
      this.getEC50().load(data.getEC50());
		if(data.getEMaxShapeParameter()!=null)
      this.getEMaxShapeParameter().load(data.getEMaxShapeParameter());
		if(data.getHeartRateModifier()!=null)
      this.getHeartRateModifier().load(data.getHeartRateModifier());
		if(data.getNeuromuscularBlock()!=null)
      this.getNeuromuscularBlock().load(data.getNeuromuscularBlock());
		if(data.getPupillaryResponse()!=null)
      this.getPupillaryResponse().load(data.getPupillaryResponse());
		if(data.getRespirationRateModifier()!=null)
      this.getRespirationRateModifier().load(data.getRespirationRateModifier());
		if(data.getSedation()!=null)
      this.getSedation().load(data.getSedation());
		if(data.getSystolicPressureModifier()!=null)
      this.getSystolicPressureModifier().load(data.getSystolicPressureModifier());
		if(data.getTidalVolumeModifier()!=null)
      this.getTidalVolumeModifier().load(data.getTidalVolumeModifier());
		if(data.getTubularPermeabilityModifier()!=null)
      this.getTubularPermeabilityModifier().load(data.getTubularPermeabilityModifier());
    if(data.getCentralNervousModifier()!=null)
      this.getCentralNervousModifier().load(data.getCentralNervousModifier());
    if(data.getEffectSiteRateConstant()!=null)
    	this.getEffectSiteRateConstant().load(data.getEffectSiteRateConstant());
    
		return true;
	}
	
	public SubstancePharmacodynamicsData unload()
	{
	  if(!isValid())
      return null;
    SubstancePharmacodynamicsData to = CDMSerializer.objFactory.createSubstancePharmacodynamicsData();
		unload(to);
		return to;
	}
	
	protected void unload(SubstancePharmacodynamicsData to)
	{
		if(hasBronchodilation())
      to.setBronchodilation(this.bronchodilation.unload());
		if(hasDiastolicPressureModifier())
      to.setDiastolicPressureModifier(this.diastolicPressureModifier.unload());
		if(hasEC50())
      to.setEC50(this.EC50.unload());
		if(hasEMaxShapeParameter())
      to.setEMaxShapeParameter(this.EMaxShapeParameter.unload());
		if(hasHeartRateModifier())
      to.setHeartRateModifier(this.heartRateModifier.unload());
		if(hasNeuromuscularBlock())
      to.setNeuromuscularBlock(this.neuromuscularBlock.unload());
		if(hasPupillaryResponse())
      to.setPupillaryResponse(this.pupillaryResponse.unload());
		if(hasRespirationRateModifier())
      to.setRespirationRateModifier(this.respirationRateModifier.unload());
		if(hasSedation())
      to.setSedation(this.sedation.unload());
		if(hasSystolicPressureModifier())
      to.setSystolicPressureModifier(this.systolicPressureModifier.unload());
		if(hasTidalVolumeModifier())
      to.setTidalVolumeModifier(this.tidalVolumeModifier.unload());
		if(hasTubularPermeabilityModifier())
      to.setTubularPermeabilityModifier(this.tubularPermeabilityModifier.unload());
    if(hasCentralNervousModifier())
      to.setCentralNervousModifier(this.centralNervousModifier.unload());
    if(hasEffectSiteRateConstant())
    	to.setEffectSiteRateConstant(this.effectSiteRateConstant.unload());
	}
	
	public SEScalarFraction getBronchodilation() 
  { 
    if(this.bronchodilation==null)
      this.bronchodilation=new SEScalarFraction();
    return this.bronchodilation;
  }
  public boolean      hasBronchodilation() {return this.bronchodilation==null?false:this.bronchodilation.isValid();}
  
  public SEScalarFraction getDiastolicPressureModifier() 
  { 
    if(this.diastolicPressureModifier==null)
      this.diastolicPressureModifier=new SEScalarFraction();
    return this.diastolicPressureModifier;
  }
  public boolean      hasDiastolicPressureModifier() {return this.diastolicPressureModifier==null?false:this.diastolicPressureModifier.isValid();}
  
  public SEScalarMassPerVolume getEC50() 
  { 
    if(this.EC50==null)
      this.EC50=new SEScalarMassPerVolume();
    return this.EC50;
  }
  public boolean      hasEC50() {return this.EC50==null?false:this.EC50.isValid();}
  
  public SEScalar getEMaxShapeParameter() 
  { 
    if(this.EMaxShapeParameter==null)
      this.EMaxShapeParameter=new SEScalar();
    return this.EMaxShapeParameter;
  }
  public boolean      hasEMaxShapeParameter() {return this.EMaxShapeParameter==null?false:this.EMaxShapeParameter.isValid();}
  
  public SEScalarFraction getHeartRateModifier() 
  { 
    if(this.heartRateModifier==null)
      this.heartRateModifier=new SEScalarFraction();
    return this.heartRateModifier;
  }
  public boolean      hasHeartRateModifier() {return this.heartRateModifier==null?false:this.heartRateModifier.isValid();}
  
  public SEScalarFraction getNeuromuscularBlock() 
  { 
    if(this.neuromuscularBlock==null)
      this.neuromuscularBlock=new SEScalarFraction();
    return this.neuromuscularBlock;
  }
  public boolean      hasNeuromuscularBlock() {return this.neuromuscularBlock==null?false:this.neuromuscularBlock.isValid();}
  
  public SEPupillaryResponse getPupillaryResponse() 
  { 
    if(this.pupillaryResponse==null)
      this.pupillaryResponse=new SEPupillaryResponse();
    return this.pupillaryResponse;
  }
  public boolean      hasPupillaryResponse() {return this.pupillaryResponse!=null;}
  
  public SEScalarFraction getRespirationRateModifier() 
  { 
    if(this.respirationRateModifier==null)
      this.respirationRateModifier=new SEScalarFraction();
    return this.respirationRateModifier;
  }
  public boolean      hasRespirationRateModifier() {return this.respirationRateModifier==null?false:this.respirationRateModifier.isValid();}
  
  public SEScalarFraction getSedation() 
  { 
    if(this.sedation==null)
      this.sedation=new SEScalarFraction();
    return this.sedation;
  }
  public boolean      hasSedation() {return this.sedation==null?false:this.sedation.isValid();}
  
  public SEScalarFraction getSystolicPressureModifier() 
  { 
    if(this.systolicPressureModifier==null)
      this.systolicPressureModifier=new SEScalarFraction();
    return this.systolicPressureModifier;
  }
  public boolean      hasSystolicPressureModifier() {return this.systolicPressureModifier==null?false:this.systolicPressureModifier.isValid();}
  
  public SEScalarFraction getTidalVolumeModifier() 
  { 
    if(this.tidalVolumeModifier==null)
      this.tidalVolumeModifier=new SEScalarFraction();
    return this.tidalVolumeModifier;
  }
  public boolean      hasTidalVolumeModifier() {return this.tidalVolumeModifier==null?false:this.tidalVolumeModifier.isValid();}
  
  public SEScalarFraction getTubularPermeabilityModifier() 
  { 
    if(this.tubularPermeabilityModifier==null)
      this.tubularPermeabilityModifier=new SEScalarFraction();
    return this.tubularPermeabilityModifier;
  }
  public boolean      hasTubularPermeabilityModifier() {return this.tubularPermeabilityModifier==null?false:this.tubularPermeabilityModifier.isValid();}
  
  public SEScalarFraction getCentralNervousModifier()
  {
  	if(this.centralNervousModifier==null)
  		this.centralNervousModifier=new SEScalarFraction();
  	return this.centralNervousModifier;
  }
  public boolean	  hasCentralNervousModifier() {return this.centralNervousModifier==null?false:this.centralNervousModifier.isValid();}
 
  public SEScalarFrequency getEffectSiteRateConstant()
  {
  	if(this.effectSiteRateConstant==null)
  		this.effectSiteRateConstant=new SEScalarFrequency();
  	return this.effectSiteRateConstant;
  }
  public boolean 		hasEffectSiteRateConstant() {return this.effectSiteRateConstant==null?false:this.effectSiteRateConstant.isValid();}
  
}
