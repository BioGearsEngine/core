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
import mil.tatrc.physiology.datamodel.bind.ActiveHeatingData;
import mil.tatrc.physiology.datamodel.properties.SEScalarArea;
import mil.tatrc.physiology.datamodel.properties.SEScalarFraction;
import mil.tatrc.physiology.datamodel.properties.SEScalarPower;

public class SEActiveHeating
{
	protected SEScalarPower       power;
	protected SEScalarArea        surfaceArea;
	protected SEScalarFraction    surfaceAreaFraction;

	public SEActiveHeating() 
	{
		power=null;
		surfaceArea=null;
		surfaceAreaFraction=null;
	}

	public void copy(SEActiveHeating other)
	{
		if(this==other)
			return;
		if (other.power != null)
			getPower().set(other.getPower());
		else if (power != null)
			power.invalidate();
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
		if (power != null)
			power.invalidate();
		if (surfaceArea != null)
			surfaceArea.invalidate();
		if (surfaceAreaFraction != null)
			surfaceAreaFraction.invalidate();		
	}


	public boolean load(ActiveHeatingData in)
	{		
		reset();
		getPower().load(in.getPower());
		if(in.getSurfaceArea()!=null)
			getSurfaceArea().load(in.getSurfaceArea());
		if(in.getSurfaceAreaFraction()!=null)
			getSurfaceAreaFraction().load(in.getSurfaceAreaFraction());		
		return true;
	}

	public ActiveHeatingData unload()
	{
		ActiveHeatingData data = CDMSerializer.objFactory.createActiveHeatingData();
		unload(data);
		return data;
	}

	protected void unload(ActiveHeatingData data)
	{
		data.setPower(this.power.unload());
		if(hasSurfaceArea())
			data.setSurfaceArea(this.surfaceArea.unload());
		if(hasSurfaceAreaFraction())
			data.setSurfaceAreaFraction(this.surfaceAreaFraction.unload());
	}

	public boolean hasPower()
	{
		return power == null ? false : power.isValid();
	}
	public SEScalarPower getPower()
	{
		if (power == null)
			power = new SEScalarPower();
		return power;
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
				+ "\n\tPower: " + (this.hasPower()?this.getPower():"None")
				+ "\n\tSurfaceArea: " + (this.hasSurfaceArea()?this.getSurfaceArea():"None")
				+ "\n\tSurfaceAreaFraction: " + (this.hasSurfaceAreaFraction()?this.getSurfaceAreaFraction():"None")
				;
	}
}
