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

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.BrainInjuryData;
import mil.tatrc.physiology.datamodel.bind.EnumBrainInjuryType;
import mil.tatrc.physiology.datamodel.bind.EnumIntubationType;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;

public class SEBrainInjury extends SEPatientAction
{
	protected SEScalar0To1        severity;
	protected EnumBrainInjuryType type;
	
	public SEBrainInjury()
	{
		severity = null;
		type     = null;
	}
	
	public void reset()
	{
		super.reset();
		if (severity != null)
			severity.invalidate();
		this.type = null;
	}
	
	public void copy(SEBrainInjury other)
	{
		if(this==other)
			return;
		super.copy(other);
		if (other.severity != null)
			getSeverity().set(other.getSeverity());
		else if (severity != null)
			severity.invalidate();
		this.type = other.type;
	}
	
	public boolean isValid()
	{
		return hasSeverity() && hasType();
	}
	
	public boolean load(BrainInjuryData in) 
	{
		super.load(in);
		getSeverity().load(in.getSeverity());
		setType(in.getType());
		return isValid();
	}
	
	public BrainInjuryData unload()
	{
		BrainInjuryData data = CDMSerializer.objFactory.createBrainInjuryData();
		unload(data);
		return data;
	}
	
	protected void unload(BrainInjuryData data)
	{
		super.unload(data);
		if (severity != null)
			data.setSeverity(severity.unload());
		if (type != null)
			data.setType(this.type);
	}
	
	public EnumBrainInjuryType getType()
	{
		return type;
	}
	public void setType(EnumBrainInjuryType t)
	{
		type = t;
	}
	public boolean hasType()
	{
		return type == null ? false : true;
	}
	
	public boolean hasSeverity()
	{
		return severity == null ? false : severity.isValid();
	}
	public SEScalar0To1 getSeverity()
	{
		if (severity == null)
			severity = new SEScalar0To1();
		return severity;
	}	

	public String toString()
	{
		if (severity != null)
			return "Brain Injury" 
					+ "\n\tType: " + getType()
					+ "\n\tSeverity: " + getSeverity();
		else
			return "Action not specified properly";
	}
}
