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
import mil.tatrc.physiology.datamodel.bind.AppliedTemperatureData;
import mil.tatrc.physiology.datamodel.bind.EnumOnOff;
import mil.tatrc.physiology.datamodel.properties.SEScalarArea;
import mil.tatrc.physiology.datamodel.properties.SEScalarFraction;
import mil.tatrc.physiology.datamodel.properties.SEScalarTemperature;

public class SEAppliedTemperature
{
	protected SEScalarTemperature temperature;
	protected SEScalarArea        surfaceArea;
	protected SEScalarFraction    surfaceAreaFraction;
	protected EnumOnOff           state;

	public SEAppliedTemperature() 
	{
		temperature=null;
		surfaceArea=null;
		surfaceAreaFraction=null;
		state = null;
	}

	public void copy(SEAppliedTemperature other)
	{
		if(this==other)
			return;
		state = other.state;
		if (other.temperature != null)
			getTemperature().set(other.getTemperature());
		else if (temperature != null)
			temperature.invalidate();
		if (other.surfaceArea != null)
			getSurfaceArea().set(other.getSurfaceArea());
		else if (surfaceArea != null)
			surfaceArea.invalidate();
		if (other.surfaceAreaFraction != null)
			getSurfaceAreaFraction().set(other.getSurfaceAreaFraction());
		else if (surfaceAreaFraction != null)
			surfaceAreaFraction.invalidate();
	}

	public void reset()
	{
		state = null;
		if (temperature != null)
			temperature.invalidate();
		if (surfaceArea != null)
			surfaceArea.invalidate();
		if (surfaceAreaFraction != null)
			surfaceAreaFraction.invalidate();
	}


	public boolean load(AppliedTemperatureData in)
	{
		reset();
		if(in.getState()!=null)
			this.state = in.getState();
		getTemperature().load(in.getTemperature());
		if(in.getSurfaceArea()!=null)
			getSurfaceArea().load(in.getSurfaceArea());
		if(in.getSurfaceAreaFraction()!=null)
			getSurfaceAreaFraction().load(in.getSurfaceAreaFraction());

		return true;
	}

	public AppliedTemperatureData unload()
	{
		AppliedTemperatureData data = CDMSerializer.objFactory.createAppliedTemperatureData();
		unload(data);
		return data;
	}

	protected void unload(AppliedTemperatureData data)
	{
		if (hasState())
			data.setState(state);
		data.setTemperature(this.temperature.unload());
		if(hasSurfaceArea())
			data.setSurfaceArea(this.surfaceArea.unload());
		if(hasSurfaceAreaFraction())
			data.setSurfaceAreaFraction(this.surfaceAreaFraction.unload());
	}
	

	public EnumOnOff getState()
	{
		return state;
	}
	public void setState(EnumOnOff onOrOff)
	{
		state = onOrOff;
	}
	public boolean hasState()
	{
		return state == null ? false : true;
	}


	public boolean hasTemperature()
	{
		return temperature == null ? false : temperature.isValid();
	}
	public SEScalarTemperature getTemperature()
	{
		if (temperature == null)
			temperature = new SEScalarTemperature();
		return temperature;
	}

	public boolean hasSurfaceArea()
	{
		return surfaceArea == null ? false : surfaceArea.isValid();
	}
	public SEScalarArea getSurfaceArea()
	{
		if (surfaceArea == null)
			surfaceArea = new SEScalarArea();
		return surfaceArea;
	}

	public boolean hasSurfaceAreaFraction()
	{
		return surfaceAreaFraction == null ? false : surfaceAreaFraction.isValid();
	}
	public SEScalarFraction getSurfaceAreaFraction()
	{
		if (surfaceAreaFraction == null)
			surfaceAreaFraction = new SEScalarFraction();
		return surfaceAreaFraction;
	}


	public String toString() 
	{
		return "Thermal Application:" 
				+ "\n\tState: " + getState()
				+ "\n\tAppliedTemperature: " + (this.hasTemperature()?this.getTemperature():"None")
				+ "\n\tAppliedSurfaceArea: " + (this.hasSurfaceArea()?this.getSurfaceArea():"None")
				+ "\n\tAppliedSurfaceAreaFraction: " + (this.hasSurfaceAreaFraction()?this.getSurfaceAreaFraction():"None");
	}
}
