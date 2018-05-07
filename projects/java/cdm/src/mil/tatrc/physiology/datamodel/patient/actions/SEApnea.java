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
import mil.tatrc.physiology.datamodel.bind.ApneaData;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;

public class SEApnea extends SEPatientAction
{
	protected SEScalar0To1 severity;
	
	public SEApnea()
	{
		severity = null;
	}
	
	public void reset()
	{
		super.reset();
		if (severity != null)
			severity.invalidate();
	}
	
	public void copy(SEApnea other)
	{
		if(this==other)
			return;
		super.copy(other);
		if (other.severity != null)
			getSeverity().set(other.getSeverity());
		else if (severity != null)
			severity.invalidate();
	}
	
	public boolean isValid()
	{
		return hasSeverity();
	}
	
	public boolean load(ApneaData in) 
	{
		super.load(in);
		getSeverity().load(in.getSeverity());
		return isValid();
	}
	
	public ApneaData unload()
	{
		ApneaData data = CDMSerializer.objFactory.createApneaData();
		unload(data);
		return data;
	}
	
	protected void unload(ApneaData data)
	{
		super.unload(data);
		if (severity != null)
			data.setSeverity(severity.unload());
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
			return "Apnea" 
					+ "\n\tSeverity: " + getSeverity();
		else
			return "Action not specified properly";
	}
}
