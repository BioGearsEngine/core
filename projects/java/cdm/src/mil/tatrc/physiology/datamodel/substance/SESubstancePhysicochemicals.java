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


import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.properties.*;

public class SESubstancePhysicochemicals
{
	protected SEScalar                     acidDissociationConstant;
	protected EnumSubstanceBindingProtein  bindingProtien;
	protected SEScalar                     bloodPlasmaRatio;
	protected SEScalarFraction             fractionUnboundInPlasma;
	protected EnumSubstanceIonicState      ionicState;
	protected SEScalar                     logP;
	protected SEScalar                     oralAbsorptionRateConstant;	
		
	public SESubstancePhysicochemicals()
	{
		
	}
	
	public void reset()
	{
		if(this.acidDissociationConstant!=null)
			this.acidDissociationConstant.invalidate();
		this.bindingProtien = null;
		if(this.bloodPlasmaRatio!=null)
			this.bloodPlasmaRatio.invalidate();
		if(this.fractionUnboundInPlasma!=null)
			this.fractionUnboundInPlasma.invalidate();
		this.ionicState=null;
		if(this.logP!=null)
			this.logP.invalidate();	
		if(this.oralAbsorptionRateConstant!=null)
			this.oralAbsorptionRateConstant.invalidate();		
	}
	
	public boolean isValid()
  {		
    if(!hasAcidDissociationConstant() || 
       !hasBindingProtein() ||
       !hasBloodPlasmaRatio() ||
       !hasFractionUnboundInPlasma() ||
       !hasIonicState() ||
       !hasLogP())       
      return false;
    return true;
  }
  
  public boolean load(SubstancePhysicochemicalData data)
	{
		this.reset();
	
		if(data.getAcidDissociationConstant()!=null)
			this.getAcidDissociationConstant().load(data.getAcidDissociationConstant());
		if(data.getBindingProtein()!=null)
			this.setBindingProtein(data.getBindingProtein());
		if(data.getBloodPlasmaRatio()!=null)
			this.getBloodPlasmaRatio().load(data.getBloodPlasmaRatio());
	  if(data.getFractionUnboundInPlasma()!=null)
			this.getFractionUnboundInPlasma().load(data.getFractionUnboundInPlasma());
		if(data.getIonicState()!=null)
			this.setIonicState(data.getIonicState());
		if(data.getLogP()!=null)
			this.getLogP().load(data.getLogP());
		if(data.getOralAbsorptionRateConstant()!=null)
			this.getOralAbsorptionRateConstant().load(data.getOralAbsorptionRateConstant());
			
		return true;
	}
	
	public SubstancePhysicochemicalData unload()
	{
	  if(!isValid())
      return null;
	  SubstancePhysicochemicalData to = CDMSerializer.objFactory.createSubstancePhysicochemicalData();
		unload(to);
		return to;
	}
	
	protected void unload(SubstancePhysicochemicalData to)
	{
		if(hasAcidDissociationConstant())
			to.setAcidDissociationConstant(this.acidDissociationConstant.unload());
		if(hasBindingProtein())
			to.setBindingProtein(this.bindingProtien);
		if(hasBloodPlasmaRatio())
			to.setBloodPlasmaRatio(this.bloodPlasmaRatio.unload());
		if(hasFractionUnboundInPlasma())
			to.setFractionUnboundInPlasma(this.fractionUnboundInPlasma.unload());
		if(hasIonicState())
			to.setIonicState(this.ionicState);
		if(hasLogP())
			to.setLogP(this.logP.unload());
		if(hasOralAbsorptionRateConstant())
			to.setOralAbsorptionRateConstant(this.oralAbsorptionRateConstant.unload());		
	}
	
	public SEScalar getAcidDissociationConstant() 
	{ 
		if(this.acidDissociationConstant==null)
			this.acidDissociationConstant=new SEScalar();
		return this.acidDissociationConstant;
	}
	public boolean hasAcidDissociationConstant() {return this.acidDissociationConstant==null?false:this.acidDissociationConstant.isValid();}
	
	public EnumSubstanceBindingProtein  getBindingProtein() { return this.bindingProtien;}
	public void                setBindingProtein(EnumSubstanceBindingProtein protein){this.bindingProtien=protein;}
	public boolean             hasBindingProtein(){return this.bindingProtien==null?false:true;}
	
	public SEScalar getBloodPlasmaRatio() 
	{ 
		if(this.bloodPlasmaRatio==null)
			this.bloodPlasmaRatio=new SEScalar();
		return this.bloodPlasmaRatio;
	}
	public boolean hasBloodPlasmaRatio() {return this.bloodPlasmaRatio==null?false:this.bloodPlasmaRatio.isValid();}
	
	public SEScalarFraction getFractionUnboundInPlasma() 
	{ 
		if(this.fractionUnboundInPlasma==null)
			this.fractionUnboundInPlasma=new SEScalarFraction();
		return this.fractionUnboundInPlasma;
	}
	public boolean hasFractionUnboundInPlasma() {return this.fractionUnboundInPlasma==null?false:this.fractionUnboundInPlasma.isValid();}
	
	public EnumSubstanceIonicState  getIonicState() { return this.ionicState;}
	public void                setIonicState(EnumSubstanceIonicState state){this.ionicState=state;}
	public boolean             hasIonicState(){return this.ionicState==null?false:true;}
	
	public SEScalar getLogP() 
	{ 
		if(this.logP==null)
			this.logP=new SEScalar();
		return this.logP;
	}
	public boolean hasLogP() {return this.logP==null?false:this.logP.isValid();}
	
	
	public SEScalar getOralAbsorptionRateConstant() 
	{ 
		if(this.oralAbsorptionRateConstant==null)
			this.oralAbsorptionRateConstant=new SEScalar();
		return this.oralAbsorptionRateConstant;
	}
	public boolean hasOralAbsorptionRateConstant() {return this.oralAbsorptionRateConstant==null?false:this.oralAbsorptionRateConstant.isValid();}
}
