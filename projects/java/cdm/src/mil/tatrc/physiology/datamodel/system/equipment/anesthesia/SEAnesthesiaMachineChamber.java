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

package mil.tatrc.physiology.datamodel.system.equipment.anesthesia;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.AnesthesiaMachineChamberData;
import mil.tatrc.physiology.datamodel.bind.EnumOnOff;
import mil.tatrc.physiology.datamodel.properties.SEScalarFraction;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceFraction;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;

public class SEAnesthesiaMachineChamber 
{
	protected EnumOnOff state;
	protected SESubstance substance;
	protected SEScalarFraction substanceFraction;

	public SEAnesthesiaMachineChamber()
	{
		this.state = null;
		this.substance = null;
		this.substanceFraction = null;
	}

	public void reset()
	{
		state = null;
		if (substanceFraction != null)
			substanceFraction.invalidate();
	}

	public void copy(SEAnesthesiaMachineChamber from)
	{
		reset();
		this.state=from.state;
		this.substance=from.substance;
		if(from.hasSubstanceFraction())
			this.getSubstanceFraction().set(from.substanceFraction);	    
	}

	public boolean load(AnesthesiaMachineChamberData in, SESubstanceManager subMgr)
	{
		if (in.getState() != null)
			setState(in.getState());
		if (in.getSubstance()!=null)
			setSubstance(subMgr.getSubstance(in.getSubstance()));
		if (in.getSubstanceFraction() != null)
			getSubstanceFraction().load(in.getSubstanceFraction());
		return true;
	}

	public AnesthesiaMachineChamberData unload()
	{
		AnesthesiaMachineChamberData data = CDMSerializer.objFactory.createAnesthesiaMachineChamberData();
		unload(data);
		return data;
	}

	protected void unload(AnesthesiaMachineChamberData data)
	{
		if(hasSubstance())
			data.setSubstance(this.substance.getName());
		if (hasState())
			data.setState(state);
		if (hasSubstanceFraction())
			data.setSubstanceFraction(substanceFraction.unload());
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

	public boolean hasSubstanceFraction()
	{
		return substanceFraction == null ? false : substanceFraction.isValid();
	}
	public SEScalarFraction getSubstanceFraction()
	{
		if (substanceFraction == null)
			substanceFraction = new SEScalarFraction();
		return substanceFraction;
	}

	public boolean hasSubstance()
	{
		return this.substance!=null;
	}
	public SESubstance getSubstance()
	{
		return substance;
	}
	public void setSubstance(SESubstance substance)
	{
		this.substance = substance;
	}

	public String toString()
	{
			return "Anesthesia Machine Chamber"
			+ "\n\tState: " + (hasState()?getState():"NotProvided")
			+ "\n\tSubstance Fraction: " + getSubstanceFraction()
			+ "\n\tSubstance: " + (hasSubstance()?getSubstance().getName():"NotProvided");
	}
}
