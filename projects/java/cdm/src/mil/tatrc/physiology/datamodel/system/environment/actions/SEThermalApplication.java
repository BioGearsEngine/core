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

package mil.tatrc.physiology.datamodel.system.environment.actions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.ThermalApplicationData;
import mil.tatrc.physiology.datamodel.system.environment.*;

public class SEThermalApplication extends SEEnvironmentAction
{
	protected SEActiveHeating      activeHeating;
	protected SEActiveCooling      activeCooling;
	protected SEAppliedTemperature appliedTemperature;
	protected Boolean              appendToPrevious;
	
	public SEThermalApplication() 
	{
		appendToPrevious = null;
	  activeHeating=null;	 
	  activeCooling=null;
	  appliedTemperature=null;
	}
	
	public void copy(SEThermalApplication other)
	{
		if(this==other)
			return;
		super.copy(other);
		appendToPrevious = other.appendToPrevious;
		if (other.activeHeating != null)
			getActiveHeating().copy(other.getActiveHeating());
		else if (activeHeating != null)
		  activeHeating.reset();
		if (other.activeCooling != null)
      getActiveCooling().copy(other.getActiveCooling());
    else if (activeCooling != null)
      activeCooling.reset();
		if (other.appliedTemperature != null)
      getAppliedTemperature().copy(other.getAppliedTemperature());
    else if (appliedTemperature != null)
      appliedTemperature.reset();
	}
	
	public void reset()
	{
		super.reset();
		appendToPrevious = null;
		if (activeHeating != null)
		  activeHeating.reset();
		if (activeCooling != null)
		  activeCooling.reset();
		if (appliedTemperature != null)
		  appliedTemperature.reset();
	}
	
	public boolean isValid()
	{
		return hasActiveCooling() || hasActiveHeating() || hasAppliedTemperature();
	}

	
	public boolean load(ThermalApplicationData in)
	{
		super.load(in);
		this.appendToPrevious = in.isAppendToPrevious();
		if(in.getActiveHeating()!=null)
		  getActiveHeating().load(in.getActiveHeating());
		if(in.getActiveCooling()!=null)
      getActiveCooling().load(in.getActiveCooling());
		if(in.getAppliedTemperature()!=null)
		  getAppliedTemperature().load(in.getAppliedTemperature());
		return isValid();
	}
	
	public ThermalApplicationData unload()
	{
	  ThermalApplicationData data = CDMSerializer.objFactory.createThermalApplicationData();
		unload(data);
		return data;
	}
	
	protected void unload(ThermalApplicationData data)
	{
		super.unload(data);
		if(appendToPrevious!=null)
			data.setAppendToPrevious(appendToPrevious);
		if (hasActiveHeating())
		  data.setActiveHeating(getActiveHeating().unload());
		if (hasActiveCooling())
      data.setActiveCooling(getActiveCooling().unload());
		if (hasAppliedTemperature())
      data.setAppliedTemperature(getAppliedTemperature().unload());
	}
	
	public boolean hasAppendToPrevious() { return this.appendToPrevious!=null;}
	public boolean appendToPrevious() { return appendToPrevious; }
	
	public boolean hasActiveHeating()
	{
		return activeHeating != null;
	}
	public SEActiveHeating getActiveHeating()
	{
		if (activeHeating == null)
		  activeHeating = new SEActiveHeating();
		return activeHeating;
	}
	
  public boolean hasActiveCooling()
  {
    return activeCooling != null;
  }
  public SEActiveCooling getActiveCooling()
  {
    if (activeCooling == null)
      activeCooling = new SEActiveCooling();
    return activeCooling;
  }
  
  public boolean hasAppliedTemperature()
  {
    return appliedTemperature != null;
  }
  public SEAppliedTemperature getAppliedTemperature()
  {
    if (appliedTemperature == null)
      appliedTemperature = new SEAppliedTemperature();
    return appliedTemperature;
  }
	
	public String toString() 
	{
      return "Thermal Application:" 
          + (this.hasActiveHeating()?this.getActiveHeating().toString():"No Heating")
          + (this.hasActiveCooling()?this.getActiveCooling().toString():"No Cooling")
          + (this.hasAppliedTemperature()?this.getAppliedTemperature().toString():"No Applied Temperature");
	}
}
