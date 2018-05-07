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

package mil.tatrc.physiology.datamodel.system.equipment.electrocardiogram;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SEElectroCardioGram implements SESystem
{
	protected SEScalarElectricPotential lead3ElectricPotential;
	
	public SEElectroCardioGram()
	{
		lead3ElectricPotential = null;
	}
	
	public void reset()
	{
		if (hasLead3ElectricPotential())
			lead3ElectricPotential.invalidate();
	}
	
	public boolean load(ElectroCardioGramData in)
	{
		if (in.getLead3ElectricPotential() != null)
			getLead3ElectricPotential().load(in.getLead3ElectricPotential());
		return true;
	}
	
	public ElectroCardioGramData unload()
	{
		ElectroCardioGramData data = CDMSerializer.objFactory.createElectroCardioGramData();
		unload(data);
		return data;
	}
	
	protected void unload(ElectroCardioGramData data)
	{
		if (hasLead3ElectricPotential())
			data.setLead3ElectricPotential(lead3ElectricPotential.unload());
	}
	

	public SEScalarElectricPotential getLead3ElectricPotential()
	{
		if (lead3ElectricPotential == null)
			lead3ElectricPotential = new SEScalarElectricPotential();
		return lead3ElectricPotential;
	}
	public boolean hasLead3ElectricPotential()
	{
		return lead3ElectricPotential == null ? false : lead3ElectricPotential.isValid();
	}
	
}
