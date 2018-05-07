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

import java.util.ArrayList;
import java.util.List;

import org.jfree.util.Log;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.EnumOnOff;
import mil.tatrc.physiology.datamodel.bind.EnumSubstanceState;
import mil.tatrc.physiology.datamodel.bind.MechanicalVentilationData;
import mil.tatrc.physiology.datamodel.bind.SubstanceFractionData;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceFraction;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.datamodel.system.SESystem;
import mil.tatrc.physiology.utilities.DoubleUtils;

public class SEMechanicalVentilation extends SEPatientAction
{
  protected SEScalarVolumePerTime flow;
  protected SEScalarPressure      pressure;
  protected EnumOnOff             state;

	protected List<SESubstanceFraction>  gasFractions;

  public SEMechanicalVentilation()
  {
    clear();
  }
  
  public void clear()
  {
  	flow = null;
  	pressure = null;
  	state = null;
		this.gasFractions=new ArrayList<SESubstanceFraction>();
  }

  public void reset()
  {
    state = null;
    if (flow != null)
    	flow.invalidate();
    if (pressure != null)
    	pressure.invalidate();
    this.gasFractions.clear();
  }

  public void copy(SEMechanicalVentilation from)
  {
    reset();

    setState(from.getState());
    if(from.hasFlow())
    	getFlow().set(from.getFlow());
    if(from.hasPressure())
    	getPressure().set(from.getPressure());    
    
    if(from.gasFractions!=null)
    {
      SESubstanceFraction mine;
      for(SESubstanceFraction sf : from.gasFractions)
      {
        mine=this.createGasFraction(sf.getSubstance());
        if(sf.hasFractionAmount())
          mine.getFractionAmount().set(sf.getFractionAmount());
      }
    }    
  }
  
  @Override
	public boolean isValid()
	{
  	if (!hasState())
    {
      Log.error("Mechanical Ventilation must have state.");
      return false;
    }
    if (getState() == EnumOnOff.OFF)
      return true;
    if (!hasGasFraction())
    {
    	Log.error("Mechanical Ventilation must have gas fractions.");
      return false;
    }
    else
    {
      double total = 0;
      for (SESubstanceFraction sf : gasFractions)
      {
        total += sf.getFractionAmount().getValue();
      }
      if (!DoubleUtils.equals(1.0, total))
      {
      	Log.error("Mechanical Ventilation Gas fractions do not sum to 1");
        return false;
      }
    }
    if (!hasPressure() && !hasFlow())
    {
    	Log.error("Mechanical Ventilation must have pressure and/or flow");
      return false;
    }
    return true;
	}

  public boolean load(MechanicalVentilationData in, SESubstanceManager substances)
  {
  	this.reset();
    if (in.getState() != null)
      setState(in.getState());
    if (in.getFlow() != null)
    	getFlow().load(in.getFlow());
    if (in.getPressure() != null)
    	getPressure().load(in.getPressure());
    
    SESubstance sub;
		if(in.getGasFraction()!=null)
		{
			for(SubstanceFractionData subData : in.getGasFraction())
			{
				sub = substances.getSubstance(subData.getName());
				if(sub == null)
				{
					Log.error("Substance does not exist : "+subData.getName());
					return false;
				}
				if(sub.getState() != EnumSubstanceState.GAS)
				{
					Log.error("Gas Fraction substance must be a gas, "+subData.getName()+" is not a gas...");
					return false;
				}
				this.createGasFraction(sub).getFractionAmount().load(subData.getFractionAmount());
			}
		}
    return isValid();
  }

  public MechanicalVentilationData unload()
  {
  	MechanicalVentilationData data = CDMSerializer.objFactory.createMechanicalVentilationData();
    unload(data);
    return data;
  }

  protected void unload(MechanicalVentilationData data)
  {
    if (hasState())
      data.setState(state);
    if (hasFlow())
      data.setFlow(flow.unload());
    if (hasPressure())
      data.setPressure(pressure.unload());
    
    for(SESubstanceFraction gf : this.gasFractions)
			data.getGasFraction().add(gf.unload());
  }

  public EnumOnOff getState()
  {
    return state;
  }
  public void setState(EnumOnOff state)
  {
    this.state = state;
  }
  public boolean hasState()
  {
    return state == null ? false : true;
  }

  public boolean hasFlow()
  {
    return flow == null ? false : flow.isValid();
  }
  public SEScalarVolumePerTime getFlow()
  {
    if (flow == null)
    	flow = new SEScalarVolumePerTime();
    return flow;
  }

  public boolean hasPressure()
  {
    return pressure == null ? false : pressure.isValid();
  }
  public SEScalarPressure getPressure()
  {
    if (pressure == null)
    	pressure = new SEScalarPressure();
    return pressure;
  }
  
  public SESubstanceFraction createGasFraction(SESubstance substance)
	{
		return getGasFraction(substance);
	}
	public SESubstanceFraction getGasFraction(SESubstance substance)
	{
		for(SESubstanceFraction sf : this.gasFractions)
		{
			if(sf.getSubstance().getName().equals(substance.getName()))
			{				
				return sf;
			}
		}		
		SESubstanceFraction sf = new SESubstanceFraction(substance);		
		this.gasFractions.add(sf);
		return sf;
	}
	public boolean hasGasFraction()
	{
		return !this.gasFractions.isEmpty();
	}
	public boolean hasGasFraction(SESubstance substance)
	{
		for(SESubstanceFraction sf : this.gasFractions)
		{
			if(sf.getSubstance()==substance)
			{				
				return true;
			}
		}
		return false;
	}
	public List<SESubstanceFraction> getGasFraction()
	{
		return this.gasFractions;
	}
	public void removeGasFraction(SESubstance substance)
	{
		for(SESubstanceFraction sf : this.gasFractions)
		{
			if(sf.getSubstance()==substance)
			{
				this.gasFractions.remove(sf);
				return;
			}
		}	
	}

	public String toString()
	{
		String cnts = "Mechanical Ventilation"
				+ "\n\tState: " + getState()
		    + "\n\tFlow: " + (hasFlow() ? getFlow() : "Not Provided")
		    + "\n\tPressure: " + (hasPressure() ? getPressure() : "Not Provided");
		for(SESubstanceFraction sf : this.gasFractions)
			cnts += "\n\tSubstanceFraction: " + sf.getSubstance().getName() + " : " + sf.getFractionAmount();
		
		return cnts;
	}
}