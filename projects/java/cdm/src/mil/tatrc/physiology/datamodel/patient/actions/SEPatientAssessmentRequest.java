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
import mil.tatrc.physiology.datamodel.bind.EnumPatientAssessment;
import mil.tatrc.physiology.datamodel.bind.PatientAssessmentRequestData;

public class SEPatientAssessmentRequest extends SEPatientAction
{
	protected EnumPatientAssessment type;
	
	public SEPatientAssessmentRequest()
	{
		type = null;
	}
	
	public void copy(SEPatientAssessmentRequest other)
	{
		if(this==other)
			return;
		super.copy(other);
		type = other.type;
	}
	
	public void reset()
	{
		super.reset();
		type = null;
	}
	
	public boolean isValid()
	{
		return hasType();
	}
	
	public boolean load(PatientAssessmentRequestData in)
	{
		super.load(in);
		type = in.getType();
		return isValid();
	}
	
	public PatientAssessmentRequestData unload()
	{
		PatientAssessmentRequestData data = CDMSerializer.objFactory.createPatientAssessmentRequestData();
		unload(data);
		return data;
	}
	
	protected void unload(PatientAssessmentRequestData data)
	{
		super.unload(data);
		if (hasType())
			data.setType(type);
	}
	
	public EnumPatientAssessment getType()
	{
		return type;
	}
	public void setType(EnumPatientAssessment type)
	{
		this.type = type;
	}
	public boolean hasType()
	{
		return type == null ? false : true;
	}
	
	public String toString() 
	{
		if (type != null)
			return "Patient Type"
					+ "\n\tType: " + getType().value();
		else
			return "Type not specified properly";
	}
}
