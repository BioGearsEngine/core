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

package mil.tatrc.physiology.datamodel.system.environment;


import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.EnvironmentData;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SEEnvironment implements SESystem
{

	protected SEScalarPower	                  convectiveHeatLoss;
	protected SEScalarHeatConductancePerArea  convectiveHeatTranferCoefficient;
	protected SEScalarPower	                  evaporativeHeatLoss;
	protected SEScalarHeatConductancePerArea  evaporativeHeatTranferCoefficient;
	protected SEScalarPower	                  radiativeHeatLoss;
	protected SEScalarHeatConductancePerArea  radiativeHeatTranferCoefficient;
	protected SEScalarPower	                  respirationHeatLoss;
	protected SEScalarPower	                  skinHeatLoss;
	
	protected SEEnvironmentalConditions       conditions;
	protected SEActiveHeating                 activeHeating;
  protected SEActiveCooling                 activeCooling;
  protected SEAppliedTemperature            appliedTemperature;
	
	public SEEnvironment()
	{		
		this.convectiveHeatLoss=null;
		this.convectiveHeatTranferCoefficient=null;
		this.evaporativeHeatLoss=null;
		this.evaporativeHeatTranferCoefficient=null;
		this.radiativeHeatLoss=null;
		this.radiativeHeatTranferCoefficient=null;
		this.respirationHeatLoss=null;
		this.skinHeatLoss=null;
		
		this.conditions=null;
		activeHeating=null;  
    activeCooling=null;
    appliedTemperature=null;
	}
	
	public void reset()
	{		
		if (convectiveHeatLoss != null)
			convectiveHeatLoss.invalidate();
		if (convectiveHeatTranferCoefficient != null)
			convectiveHeatTranferCoefficient.invalidate();
		if (evaporativeHeatLoss != null)
			evaporativeHeatLoss.invalidate();
		if (evaporativeHeatTranferCoefficient != null)
			evaporativeHeatTranferCoefficient.invalidate();
		if (radiativeHeatLoss != null)
			radiativeHeatLoss.invalidate();
		if (radiativeHeatTranferCoefficient != null)
			radiativeHeatTranferCoefficient.invalidate();
		if (respirationHeatLoss != null)
			respirationHeatLoss.invalidate();
		if (skinHeatLoss != null)
			skinHeatLoss.invalidate();
		if (conditions != null)
		  conditions.reset();
		if (activeHeating != null)
      activeHeating.reset();
    if (activeCooling != null)
      activeCooling.reset();
    if (appliedTemperature != null)
      appliedTemperature.reset();
	}
	
	public boolean load(EnvironmentData in, SESubstanceManager substances)
	{		
		reset();
		if (in.getConvectiveHeatLoss() != null)
			getConvectiveHeatLoss().load(in.getConvectiveHeatLoss());
		if (in.getConvectiveHeatTranferCoefficient() != null)
			getConvectiveHeatTranferCoefficient().load(in.getConvectiveHeatTranferCoefficient());
		if (in.getEvaporativeHeatLoss() != null)
			getEvaporativeHeatLoss().load(in.getEvaporativeHeatLoss());
		if (in.getEvaporativeHeatTranferCoefficient() != null)
			getEvaporativeHeatTranferCoefficient().load(in.getEvaporativeHeatTranferCoefficient());
		if (in.getRadiativeHeatLoss() != null)
			getRadiativeHeatLoss().load(in.getRadiativeHeatLoss());
		if (in.getRadiativeHeatTranferCoefficient() != null)
			getRadiativeHeatTranferCoefficient().load(in.getRadiativeHeatTranferCoefficient());
		if (in.getRespirationHeatLoss() != null)
			getRespirationHeatLoss().load(in.getRespirationHeatLoss());
		if (in.getSkinHeatLoss() != null)
			getSkinHeatLoss().load(in.getSkinHeatLoss());	
		
		if (in.getConditions() != null)
		  getConditions().load(in.getConditions(),substances); 
		if(in.getActiveHeating()!=null)
      getActiveHeating().load(in.getActiveHeating());
    if(in.getActiveCooling()!=null)
      getActiveCooling().load(in.getActiveCooling());
    if(in.getAppliedTemperature()!=null)
      getAppliedTemperature().load(in.getAppliedTemperature());
		return true;
	}
	
	public EnvironmentData unload()
	{
		EnvironmentData data = CDMSerializer.objFactory.createEnvironmentData();
		unload(data);
		return data;
	}
	
	protected void unload(EnvironmentData data)
	{		
		if (convectiveHeatLoss != null)
		    data.setConvectiveHeatLoss(convectiveHeatLoss.unload());
		if (convectiveHeatTranferCoefficient != null)
		    data.setConvectiveHeatTranferCoefficient(convectiveHeatTranferCoefficient.unload());
		if (evaporativeHeatLoss != null)
		    data.setEvaporativeHeatLoss(evaporativeHeatLoss.unload());
		if (evaporativeHeatTranferCoefficient != null)
			data.setEvaporativeHeatTranferCoefficient(evaporativeHeatTranferCoefficient.unload());
		if (radiativeHeatLoss != null)
		    data.setRadiativeHeatLoss(radiativeHeatLoss.unload());
		if (radiativeHeatTranferCoefficient != null)
		    data.setRadiativeHeatTranferCoefficient(radiativeHeatTranferCoefficient.unload());
		if (respirationHeatLoss != null)
		    data.setRespirationHeatLoss(respirationHeatLoss.unload());
		if (skinHeatLoss != null)
		    data.setSkinHeatLoss(skinHeatLoss.unload());
		
		if (conditions != null)
      data.setConditions(conditions.unload());
		if (hasActiveHeating())
      data.setActiveHeating(getActiveHeating().unload());
    if (hasActiveCooling())
      data.setActiveCooling(getActiveCooling().unload());
    if (hasAppliedTemperature())
      data.setAppliedTemperature(getAppliedTemperature().unload());
	}

	public SEScalarPower getConvectiveHeatLoss()
	{
		if (convectiveHeatLoss == null)
			convectiveHeatLoss = new SEScalarPower();
		return convectiveHeatLoss;
	}
	public boolean hasConvectiveHeatLoss()
	{
		return convectiveHeatLoss == null ? false : convectiveHeatLoss.isValid();
	}
	
	public SEScalarHeatConductancePerArea getConvectiveHeatTranferCoefficient()
	{
		if (convectiveHeatTranferCoefficient == null)
			convectiveHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
		return convectiveHeatTranferCoefficient;
	}
	public boolean hasConvectiveHeatTranferCoefficient()
	{
		return convectiveHeatTranferCoefficient == null ? false : convectiveHeatTranferCoefficient.isValid();
	}
	
	public SEScalarPower getEvaporativeHeatLoss()
	{
		if (evaporativeHeatLoss == null)
			evaporativeHeatLoss = new SEScalarPower();
		return evaporativeHeatLoss;
	}
	public boolean hasEvaporativeHeatLoss()
	{
		return evaporativeHeatLoss == null ? false : evaporativeHeatLoss.isValid();
	}
	
	public SEScalarHeatConductancePerArea getEvaporativeHeatTranferCoefficient()
	{
		if (evaporativeHeatTranferCoefficient == null)
			evaporativeHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
		return evaporativeHeatTranferCoefficient;
	}
	public boolean hasEvaporativeHeatTranferCoefficient()
	{
		return evaporativeHeatTranferCoefficient == null ? false : evaporativeHeatTranferCoefficient.isValid();
	}
	
	public SEScalarPower getRadiativeHeatLoss()
	{
		if (radiativeHeatLoss == null)
			radiativeHeatLoss = new SEScalarPower();
		return radiativeHeatLoss;
	}
	public boolean hasRadiativeHeatLoss()
	{
		return radiativeHeatLoss == null ? false : radiativeHeatLoss.isValid();
	}
	
	public SEScalarHeatConductancePerArea getRadiativeHeatTranferCoefficient()
	{
		if (radiativeHeatTranferCoefficient == null)
			radiativeHeatTranferCoefficient = new SEScalarHeatConductancePerArea();
		return radiativeHeatTranferCoefficient;
	}
	public boolean hasRadiativeHeatTranferCoefficient()
	{
		return radiativeHeatTranferCoefficient == null ? false : radiativeHeatTranferCoefficient.isValid();
	}
	
	public SEScalarPower getRespirationHeatLoss()
	{
		if (respirationHeatLoss == null)
			respirationHeatLoss = new SEScalarPower();
		return respirationHeatLoss;
	}
	public boolean hasRespirationHeatLoss()
	{
		return respirationHeatLoss == null ? false : respirationHeatLoss.isValid();
	}
	
	public SEScalarPower getSkinHeatLoss()
	{
		if (skinHeatLoss == null)
			skinHeatLoss = new SEScalarPower();
		return skinHeatLoss;
	}
	public boolean hasSkinHeatLoss()
	{
		return skinHeatLoss == null ? false : skinHeatLoss.isValid();
	}
	
	public SEEnvironmentalConditions getConditions()
  {
    if (conditions == null)
      conditions = new SEEnvironmentalConditions();
    return conditions;
  }
  public boolean hasConditions()
  {
    return conditions != null;
  }
  
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
}