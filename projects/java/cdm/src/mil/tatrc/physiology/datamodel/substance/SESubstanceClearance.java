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
import mil.tatrc.physiology.datamodel.bind.SubstanceClearanceData.RenalDynamics;
import mil.tatrc.physiology.datamodel.bind.SubstanceClearanceData.RenalDynamics.Regulation;
import mil.tatrc.physiology.datamodel.bind.SubstanceClearanceData.Systemic;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.utilities.Log;

public class SESubstanceClearance
{
	protected SEScalarFraction             fractionExcretedInFeces;
	protected SEScalarFraction             fractionExcretedInUrine;
	protected SEScalarFraction             fractionMetabolizedInGut;
	protected SEScalarFraction             fractionUnboundInPlasma;
	protected SEScalarVolumePerTimeMass    intrinsicClearance;
	protected SEScalarVolumePerTimeMass    renalClearance;
	protected EnumCharge                   chargeInBlood;
	protected SEScalar                     renalReabsorptionRatio;
	protected SEScalarMassPerTime          renalTransportMaximum;
	protected SEScalarMassPerTime          renalFiltrationRate;
	protected SEScalarMassPerTime          renalReabsorptionRate;
	protected SEScalarMassPerTime          renalExcretionRate;
	protected SEScalar                     glomerularFilterability;
	protected SEScalarVolumePerTimeMass    systemicClearance;
	
	public SESubstanceClearance()
	{
		
	}
	
	public void reset()
	{
		if(this.fractionExcretedInFeces!=null)
			this.fractionExcretedInFeces.invalidate();
		if(this.fractionExcretedInUrine!=null)
			this.fractionExcretedInUrine.invalidate();
		if(this.fractionMetabolizedInGut!=null)
			this.fractionMetabolizedInGut.invalidate();
		if(this.fractionUnboundInPlasma!=null)
			this.fractionUnboundInPlasma.invalidate();
		if(this.intrinsicClearance!=null)
			this.intrinsicClearance.invalidate();
		if(this.renalClearance!=null)
			this.renalClearance.invalidate();
		this.chargeInBlood = null;
		if(this.renalReabsorptionRatio!=null)
      this.renalReabsorptionRatio.invalidate();
		if(this.renalTransportMaximum!=null)
      this.renalTransportMaximum.invalidate();
		if(this.renalFiltrationRate!=null)
      this.renalFiltrationRate.invalidate();
		if(this.renalReabsorptionRate!=null)
      this.renalReabsorptionRate.invalidate();
		if(this.renalExcretionRate!=null)
      this.renalExcretionRate.invalidate();
		if(this.glomerularFilterability!=null)
      this.glomerularFilterability.invalidate();
		if(this.systemicClearance!=null)
			this.systemicClearance.invalidate();
	}
	
	public boolean isValid()
  {
   return (hasSystemic() || hasRenalClearance() || hasRenalRegulation());    
  }
	
	public boolean hasSystemic()
	{
	  if(  !hasFractionExcretedInFeces() ||
	       !hasFractionUnboundInPlasma() ||
	       !hasIntrinsicClearance() ||
	       !hasRenalClearance() ||
	       !hasSystemicClearance())
	      return false;
	  return true;
	}	
	public boolean hasRenalRegulation()
	{
	  if(  !hasChargeInBlood() ||
	       !hasFractionUnboundInPlasma() ||
	       !hasRenalReabsorptionRatio() ||
	       !hasRenalTransportMaximum())
	      return false;
	  return true;
	}
  
  public boolean load(SubstanceClearanceData data)
	{
		this.reset();
		
		double rc = Double.NaN;
		double fuip = Double.NaN;
		
		if(data.getSystemic()!=null)
		{
		  Systemic sys = data.getSystemic();
		  if(sys.getFractionExcretedInFeces()!=null)
		    this.getFractionExcretedInFeces().load(sys.getFractionExcretedInFeces());
		  if(sys.getFractionExcretedInUrine()!=null)
		    this.getFractionExcretedInUrine().load(sys.getFractionExcretedInUrine());
		  if(sys.getFractionMetabolizedInGut()!=null)
		    this.getFractionMetabolizedInGut().load(sys.getFractionMetabolizedInGut());
		  if(sys.getFractionUnboundInPlasma()!=null)
		  {
		    fuip = sys.getFractionUnboundInPlasma().getValue();
		    this.getFractionUnboundInPlasma().load(sys.getFractionUnboundInPlasma());
		  }
		  if(sys.getIntrinsicClearance()!=null)
		    this.getIntrinsicClearance().load(sys.getIntrinsicClearance());
		  if(sys.getRenalClearance()!=null)
		  {
		    rc = sys.getRenalClearance().getValue();
        this.getRenalClearance().load(sys.getRenalClearance());
		  }
		  if(sys.getSystemicClearance()!=null)
		    this.getSystemicClearance().load(sys.getSystemicClearance());
		}
		if(data.getRenalDynamics()!=null)
		{
		  SubstanceClearanceData.RenalDynamics rd = data.getRenalDynamics();
		  if(rd.getClearance()!=null)
		  {
		    if(!Double.isNaN(rc) && rd.getClearance().getValue()!=rc)
		      Log.error("Different RenalClearance values between Systemic and RenalDynamic, using RenalDynamic");
		    this.getRenalClearance().load(rd.getClearance());
		  }
		  if(rd.getRegulation()!=null)
		  {
		    SubstanceClearanceData.RenalDynamics.Regulation rdr = rd.getRegulation();
		    this.chargeInBlood = rdr.getChargeInBlood();
		    if(rdr.getReabsorptionRatio()!=null)
	        this.getRenalReabsorptionRatio().load(rdr.getReabsorptionRatio());
		    if(rdr.getTransportMaximum()!=null)
	        this.getRenalTransportMaximum().load(rdr.getTransportMaximum());
		    if(rdr.getFractionUnboundInPlasma()!=null)
		    {
		      if(!Double.isNaN(fuip) && rdr.getFractionUnboundInPlasma().getValue()!=fuip)
	          Log.error("Different FractionUnboundInPlasma values between Systemic and RenalDynamic, using RenalDynamic");
	        
          this.getFractionUnboundInPlasma().load(rdr.getFractionUnboundInPlasma());
		    }
		  }
		  if(rd.getFiltrationRate()!=null)
        this.getRenalFiltrationRate().load(rd.getFiltrationRate());
		  if(rd.getReabsorptionRate()!=null)
        this.getRenalReabsorptionRate().load(rd.getReabsorptionRate());
		  if(rd.getExcretionRate()!=null)
        this.getRenalExcretionRate().load(rd.getExcretionRate());
		  if(rd.getGlomerularFilterability()!=null)
        this.getGlomerularFilterability().load(rd.getGlomerularFilterability());     		  
		}
		

		return true;
	}
	
	public SubstanceClearanceData unload()
	{
	  if(!isValid())
      return null;
    SubstanceClearanceData to = CDMSerializer.objFactory.createSubstanceClearanceData();
		unload(to);
		return to;
	}
	
	protected void unload(SubstanceClearanceData to)
	{		
	  if(hasSystemic())
	  {
	    Systemic sys = CDMSerializer.objFactory.createSubstanceClearanceDataSystemic();
	    to.setSystemic(sys);
	    if(hasFractionExcretedInFeces())
	      sys.setFractionExcretedInFeces(this.fractionExcretedInFeces.unload());
	    if(hasFractionExcretedInUrine())
	      sys.setFractionExcretedInUrine(this.fractionExcretedInUrine.unload());
	    if(hasFractionMetabolizedInGut())
	      sys.setFractionMetabolizedInGut(this.fractionMetabolizedInGut.unload());
	    if(hasFractionUnboundInPlasma())
	      sys.setFractionUnboundInPlasma(this.fractionUnboundInPlasma.unload());
	    if(hasIntrinsicClearance())
	      sys.setIntrinsicClearance(this.intrinsicClearance.unload());
	    if(hasRenalClearance())
	      sys.setRenalClearance(this.renalClearance.unload());
	    if(hasSystemicClearance())
	      sys.setSystemicClearance(this.systemicClearance.unload());
	  }
	  
	  if(hasRenalClearance() || hasRenalRegulation())
	  {	
	    // Renal Dynamics
	    RenalDynamics rd = CDMSerializer.objFactory.createSubstanceClearanceDataRenalDynamics();
	    to.setRenalDynamics(rd);
	    if(hasRenalRegulation())
	    {
	      Regulation rdr = CDMSerializer.objFactory.createSubstanceClearanceDataRenalDynamicsRegulation();
        rdr.setChargeInBlood(this.chargeInBlood);
        if(hasRenalReabsorptionRatio())
          rdr.setReabsorptionRatio(this.renalReabsorptionRatio.unload());
        if(hasRenalTransportMaximum())
          rdr.setTransportMaximum(this.renalTransportMaximum.unload());
        if(hasFractionUnboundInPlasma())
          rdr.setFractionUnboundInPlasma(this.fractionUnboundInPlasma.unload());        
        rd.setRegulation(rdr);
	    }	      
	    else
	    {
	      rd.setClearance(this.renalClearance.unload());	     
	    }		
	    if(hasRenalFiltrationRate())
	      rd.setFiltrationRate(this.renalFiltrationRate.unload());
	    if(hasRenalReabsorptionRate())
	      rd.setReabsorptionRate(this.renalReabsorptionRate.unload());
	    if(hasRenalExcretionRate())
	      rd.setExcretionRate(this.renalExcretionRate.unload());
	    if(hasGlomerularFilterability())
	      rd.setGlomerularFilterability(this.glomerularFilterability.unload());
	  }
				
	}
	
	public SEScalarFraction getFractionExcretedInFeces() 
	{ 
		if(this.fractionExcretedInFeces==null)
			this.fractionExcretedInFeces=new SEScalarFraction();
		return this.fractionExcretedInFeces;
	}
	public boolean hasFractionExcretedInFeces() {return this.fractionExcretedInFeces==null?false:this.fractionExcretedInFeces.isValid();}
	
	public SEScalarFraction getFractionExcretedInUrine() 
	{ 
		if(this.fractionExcretedInUrine==null)
			this.fractionExcretedInUrine=new SEScalarFraction();
		return this.fractionExcretedInUrine;
	}
	public boolean hasFractionExcretedInUrine() {return this.fractionExcretedInUrine==null?false:this.fractionExcretedInUrine.isValid();}
	
	public SEScalarFraction getFractionMetabolizedInGut() 
	{ 
		if(this.fractionMetabolizedInGut==null)
			this.fractionMetabolizedInGut=new SEScalarFraction();
		return this.fractionMetabolizedInGut;
	}
	public boolean hasFractionMetabolizedInGut() {return this.fractionMetabolizedInGut==null?false:this.fractionMetabolizedInGut.isValid();}
	
	public SEScalarFraction getFractionUnboundInPlasma() 
	{ 
		if(this.fractionUnboundInPlasma==null)
			this.fractionUnboundInPlasma=new SEScalarFraction();
		return this.fractionUnboundInPlasma;
	}
	public boolean hasFractionUnboundInPlasma() {return this.fractionUnboundInPlasma==null?false:this.fractionUnboundInPlasma.isValid();}
	
	public SEScalarVolumePerTimeMass getIntrinsicClearance() 
	{ 
		if(this.intrinsicClearance==null)
			this.intrinsicClearance=new SEScalarVolumePerTimeMass();
		return this.intrinsicClearance;
	}
	public boolean hasIntrinsicClearance() {return this.intrinsicClearance==null?false:this.intrinsicClearance.isValid();}
	
	public SEScalarVolumePerTimeMass getRenalClearance() 
	{ 
		if(this.renalClearance==null)
			this.renalClearance=new SEScalarVolumePerTimeMass();
		return this.renalClearance;
	}
	public boolean hasRenalClearance() {return this.renalClearance==null?false:this.renalClearance.isValid();}
	
	public EnumCharge getChargeInBlood() { return this.chargeInBlood;}
  public void       setChargeInBlood(EnumCharge charge){this.chargeInBlood=charge;}
  public boolean    hasChargeInBlood(){return this.chargeInBlood==null?false:true;}

  public SEScalar getRenalReabsorptionRatio() 
  { 
    if(this.renalReabsorptionRatio==null)
      this.renalReabsorptionRatio=new SEScalar();
    return this.renalReabsorptionRatio;
  }
  public boolean hasRenalReabsorptionRatio() {return this.renalReabsorptionRatio==null?false:this.renalReabsorptionRatio.isValid();}
  
  public SEScalarMassPerTime getRenalTransportMaximum() 
  { 
    if(this.renalTransportMaximum==null)
      this.renalTransportMaximum=new SEScalarMassPerTime();
    return this.renalTransportMaximum;
  }
  public boolean hasRenalTransportMaximum() {return this.renalTransportMaximum==null?false:this.renalTransportMaximum.isValid();}
  
  public SEScalarMassPerTime getRenalFiltrationRate() 
  { 
    if(this.renalFiltrationRate==null)
      this.renalFiltrationRate=new SEScalarMassPerTime();
    return this.renalFiltrationRate;
  }
  public boolean hasRenalFiltrationRate() {return this.renalFiltrationRate==null?false:this.renalFiltrationRate.isValid();}
  
  public SEScalarMassPerTime getRenalReabsorptionRate() 
  { 
    if(this.renalReabsorptionRate==null)
      this.renalReabsorptionRate=new SEScalarMassPerTime();
    return this.renalReabsorptionRate;
  }
  public boolean hasRenalReabsorptionRate() {return this.renalReabsorptionRate==null?false:this.renalReabsorptionRate.isValid();}
  
  public SEScalarMassPerTime getRenalExcretionRate() 
  { 
    if(this.renalExcretionRate==null)
      this.renalExcretionRate=new SEScalarMassPerTime();
    return this.renalExcretionRate;
  }
  public boolean hasRenalExcretionRate() {return this.renalExcretionRate==null?false:this.renalExcretionRate.isValid();}
  
  public SEScalar getGlomerularFilterability() 
  { 
    if(this.glomerularFilterability==null)
      this.glomerularFilterability=new SEScalar();
    return this.glomerularFilterability;
  }
  public boolean hasGlomerularFilterability() {return this.glomerularFilterability==null?false:this.glomerularFilterability.isValid();}
 
	public SEScalarVolumePerTimeMass getSystemicClearance() 
	{ 
		if(this.systemicClearance==null)
			this.systemicClearance=new SEScalarVolumePerTimeMass();
		return this.systemicClearance;
	}
	public boolean hasSystemicClearance() {return this.systemicClearance==null?false:this.systemicClearance.isValid();}
	
}
