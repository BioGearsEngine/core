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

package mil.tatrc.physiology.datamodel.system.equipment.inhaler.actions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.InhalerConfigurationData;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.datamodel.system.equipment.inhaler.SEInhaler;

public class SEInhalerConfiguration extends SEInhalerAction
{
	protected SEInhaler configuration;
	protected String    configurationFile;
  
	public SEInhalerConfiguration()
	{
	}
	
	public SEInhalerConfiguration(SEInhalerConfiguration other)
	{
		this();
		copy(other);		
	}
	
	public void copy(SEInhalerConfiguration other)
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
	
	public boolean load(InhalerConfigurationData in, SESubstanceManager subMgr)
	{
		super.load(in);
		if(in.getConfiguration()!=null)
			getConfiguration().load(in.getConfiguration(),subMgr);
		else if(in.getConfigurationFile()!=null)
		  this.configurationFile=in.getConfigurationFile();
		return isValid();
	}
	
	public InhalerConfigurationData unload()
	{
		InhalerConfigurationData data = CDMSerializer.objFactory.createInhalerConfigurationData();
		unload(data);
		return data;
	}
	
	protected void unload(InhalerConfigurationData data)
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
	public SEInhaler getConfiguration()
	{
	  if(this.configuration==null)
	    this.configuration=new SEInhaler();
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
	  String str = "Inhaler Configuration";
	  if(hasConfiguration())
	  {
	    str += "\n\tState: "; str += this.configuration.hasState()?this.configuration.getState():"Not Supplied";
	    str += "\n\tMetered Dose: "; str += this.configuration.hasMeteredDose()?this.configuration.getMeteredDose():"Not Supplied";
	    str += "\n\tNozzle Loss: "; str += this.configuration.hasNozzleLoss()?this.configuration.getNozzleLoss():"Not Supplied";
	    str += "\n\tSpacer Volume: "; str += this.configuration.hasSpacerVolume()?this.configuration.getSpacerVolume():"Not Supplied";    
	    str += "\n\tSubstance: "; str += this.configuration.hasSubstance()?this.configuration.getSubstance().getName():"Not Supplied";
	  }
	  
	  if(this.hasConfigurationFile())
		  str +="\n\tFile: "+this.configurationFile;
	  
	  return str;
	}
}