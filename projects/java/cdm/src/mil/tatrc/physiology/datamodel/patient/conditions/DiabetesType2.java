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

package mil.tatrc.physiology.datamodel.patient.conditions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.ChronicAnemiaData;
import mil.tatrc.physiology.datamodel.bind.DiabetesType2Data;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;

public class DiabetesType2 extends SEPatientCondition
{
	protected SEScalar0To1 insulinProductionSeverity;
	protected SEScalar0To1 insulinResistanceSeverity;
	
	public DiabetesType2()
	{
		insulinProductionSeverity = null;
		insulinResistanceSeverity = null;
	}
	
	public void reset()
	{
		super.reset();
		if (insulinProductionSeverity != null)
			insulinProductionSeverity.invalidate();
		if (insulinResistanceSeverity != null)
			insulinResistanceSeverity.invalidate();
	}
	
	public void copy(DiabetesType2 other)
	{
		if(this==other)
			return;
		super.copy(other);
		if (other.insulinProductionSeverity != null)
			getInsulinProductionSeverity().set(other.getInsulinProductionSeverity());
		else if (insulinProductionSeverity != null)
			insulinProductionSeverity.invalidate();
		
		if (other.insulinResistanceSeverity != null)
			getInsulinResistanceSeverity().set(other.getInsulinResistanceSeverity());
		else if (insulinResistanceSeverity != null)
			insulinResistanceSeverity.invalidate();
	}
	
	public boolean load(DiabetesType2Data in) 
	{
		super.load(in);
		getInsulinProductionSeverity().load(in.getInsulinProductionSeverity());
		getInsulinResistanceSeverity().load(in.getInsulinResistanceSeverity());
		return true;
	}
	
	public DiabetesType2Data unload() 
	{
		DiabetesType2Data to = CDMSerializer.objFactory.createDiabetesType2Data();
		unload(to);
		return to;
	}
	
	protected void unload(DiabetesType2Data data)
	{
		super.unload(data);
		if (insulinProductionSeverity != null)
			data.setInsulinProductionSeverity(insulinProductionSeverity.unload());
		if (insulinResistanceSeverity != null)
			data.setInsulinResistanceSeverity(insulinResistanceSeverity.unload());
	}
	
	public boolean hasInsulinProductionSeverity()
	{
		return insulinProductionSeverity == null ? false : insulinProductionSeverity.isValid();
	}
	public SEScalar0To1 getInsulinProductionSeverity()
	{
		if (insulinProductionSeverity == null)
			insulinProductionSeverity = new SEScalar0To1();
		return insulinProductionSeverity;
	}
	
	public boolean hasInsulinResistanceSeverity()
	{
		return insulinResistanceSeverity == null ? false : insulinResistanceSeverity.isValid();
	}
	public SEScalar0To1 getInsulinResistanceSeverity()
	{
		if (insulinResistanceSeverity == null)
			insulinResistanceSeverity = new SEScalar0To1();
		return insulinResistanceSeverity;
	}
	
	public String toString()
	{
		return "Diabetes Type 2" 
				+ "\n\tInsulin Production Severity: " + getInsulinProductionSeverity()
				+ "\n\tInsulin Resistance Severity: " + getInsulinResistanceSeverity();
	}

	
}
