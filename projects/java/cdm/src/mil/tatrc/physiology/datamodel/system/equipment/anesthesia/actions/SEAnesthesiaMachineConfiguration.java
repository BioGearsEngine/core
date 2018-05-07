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

package mil.tatrc.physiology.datamodel.system.equipment.anesthesia.actions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.AnesthesiaMachineConfigurationData;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.datamodel.system.equipment.anesthesia.SEAnesthesiaMachine;

public class SEAnesthesiaMachineConfiguration extends SEAnesthesiaMachineAction
{
	protected SEAnesthesiaMachine configuration;
	protected String              configurationFile;
  
	public SEAnesthesiaMachineConfiguration()
	{
		
	}
	
	public SEAnesthesiaMachineConfiguration(SEAnesthesiaMachineConfiguration other)
	{
		this();
		copy(other);		
	}
	
	public void copy(SEAnesthesiaMachineConfiguration other)
	{
	  if(this==other)
      return;
    super.copy(other);
		this.configuration.copy(other.configuration);
		this.configurationFile=other.configurationFile;
	}
	
	public void reset()
	{
		super.reset();
		
		if (this.configuration != null)
			this.configuration.reset();
		if (this.configurationFile != null)
			this.configurationFile="";
	}
	
	public boolean isValid()
	{
		return hasConfiguration() || hasConfigurationFile();
	}
	
	public boolean load(AnesthesiaMachineConfigurationData in, SESubstanceManager subMgr)
	{
		super.load(in);
		if(in.getConfiguration()!=null)
			getConfiguration().load(in.getConfiguration(),subMgr);
		else if(in.getConfigurationFile()!=null)
		  this.configurationFile=in.getConfigurationFile();
		return isValid();
	}
	
	public AnesthesiaMachineConfigurationData unload()
	{
	  AnesthesiaMachineConfigurationData data = CDMSerializer.objFactory.createAnesthesiaMachineConfigurationData();
		unload(data);
		return data;
	}
	
	protected void unload(AnesthesiaMachineConfigurationData data)
	{
		super.unload(data);
		if(this.hasConfiguration())
		  data.setConfiguration(this.configuration.unload());
		else if(this.hasConfigurationFile())
		  data.setConfigurationFile(this.configurationFile);
	}
	
	public boolean hasConfiguration()
	{
	  return this.configuration!=null;
	}
	public SEAnesthesiaMachine getConfiguration()
	{
	  if(this.configuration==null)
	    this.configuration=new SEAnesthesiaMachine();
		return this.configuration;
	}
	
	public boolean hasConfigurationFile()
	{
	  return this.configurationFile!=null&&!this.configurationFile.isEmpty();
	}
	public String getConfigurationFile()
	{
	  return this.configurationFile;
	}
	public void setConfigurationFile(String s)
	{
	  this.configurationFile = s;
	}
	
	public String toString()
	{
	  String str = "Anesthesia Machine Configuration";
	  if(hasConfiguration())
	  {
	    str += "\n\tConnection: "; str += this.configuration.hasConnection()?this.configuration.getConnection():"Not Supplied";
	    str += "\n\tInlet Flow: "; str += this.configuration.hasInletFlow()?this.configuration.getInletFlow():"Not Supplied";
	    str += "\n\tInspiratory/Expiratory Ratio: "; str += this.configuration.hasInspiratoryExpiratoryRatio()?this.configuration.getInspiratoryExpiratoryRatio():"Not Supplied";
	    str += "\n\tOxygen Fraction: "; str += this.configuration.hasOxygenFraction()?this.configuration.getOxygenFraction():"Not Supplied";    
	    str += "\n\tOxygen Source: "; str += this.configuration.hasOxygenSource()?this.configuration.getOxygenSource():"Not Supplied";
	    str += "\n\tPositive End Expired Pressure: "; str += this.configuration.hasPositiveEndExpiredPressure()?this.configuration.getPositiveEndExpiredPressure():"Not Supplied";
	    str += "\n\tPrimary Gas: "; str += this.configuration.hasPrimaryGas()?this.configuration.getPrimaryGas():"Not Supplied";
	    str += "\n\tReliefValvePressure: "; str += this.configuration.hasReliefValvePressure()?this.configuration.getReliefValvePressure():"Not Supplied";
	    str += "\n\tRespiratory Rate: "; str += this.configuration.hasRespiratoryRate()?this.configuration.getRespiratoryRate():"Not Supplied";
	    str += "\n\tVentilator Pressure: "; str += this.configuration.hasVentilatorPressure()?this.configuration.getVentilatorPressure():"Not Supplied";
	    if(this.configuration.hasLeftChamber())
	    {
	      str += "\n\tLeftChamberState: "; str += this.configuration.getLeftChamber().hasState()?this.configuration.getLeftChamber().getState():"Not Supplied";
	      str += "\n\tLeftChamberSubstance: "; str += this.configuration.getLeftChamber().hasSubstance()?this.configuration.getLeftChamber().getSubstance().getName():"Not Supplied";    
	      str += "\n\tLeftChamberSubstanceAmount: "; str += this.configuration.getLeftChamber().hasSubstanceFraction()?this.configuration.getLeftChamber().getSubstanceFraction():"Not Supplied";	         
	    }
	    if(this.configuration.hasRightChamber())
      {
        str += "\n\tRightChamberState: "; str += this.configuration.getRightChamber().hasState()?this.configuration.getRightChamber().getState():"Not Supplied";
        str += "\n\tRightChamberSubstance: "; str += this.configuration.getRightChamber().hasSubstance()?this.configuration.getRightChamber().getSubstance().getName():"Not Supplied";    
        str += "\n\tRightChamberSubstanceAmount: "; str += this.configuration.getRightChamber().hasSubstanceFraction()?this.configuration.getRightChamber().getSubstanceFraction():"Not Supplied";   
      }
	    if(this.configuration.hasOxygenBottleOne())
	    {
	      str += "\n\tOxygenBottleOneVolume: "; str += this.configuration.getOxygenBottleOne().hasVolume()?this.configuration.getOxygenBottleOne().getVolume():"Not Supplied";
	    }
	    if(this.configuration.hasOxygenBottleTwo())
      {
        str += "\n\tOxygenBottleTwoVolume: "; str += this.configuration.getOxygenBottleTwo().hasVolume()?this.configuration.getOxygenBottleTwo().getVolume():"Not Supplied";
      }
	  }
		if(this.hasConfigurationFile())
		  str +="\n\tFile: "+this.configurationFile;
		return str;
	}
}