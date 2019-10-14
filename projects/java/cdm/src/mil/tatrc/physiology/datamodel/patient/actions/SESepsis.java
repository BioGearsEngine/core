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


import java.util.*;
import mil.tatrc.physiology.datamodel.CDMSerializer;
//import mil.tatrc.physiology.datamodel.bind.SepsisData;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;


public class SESepsis extends SEPatientAction
{
	//Required to specify a Sepsis action
	protected String compartment;
	protected SEScalar0To1 severity;
	
	public SESepsis()
	{
		compartment = null;
		severity = null;
	}
	
	public void copy(SESepsis other)
	{
		if(this==other)
			return;
		super.copy(other);
		severity = other.severity;
		compartment = other.compartment;
	}
	
	public void reset()
	{
		super.reset();
		if(severity!=null)
			this.severity.invalidate();
		this.compartment=null;
	}
	
	public boolean isValid()
	{
		return hasCompartment() && hasSeverity();
	}
	
//	public boolean load(SepsisData in)
//	{
//		super.load(in);
//    this.getSeverity().load(in.getSeverity());
//    this.compartment=in.getCompartment();
//		
//    return isValid();
//	}
	
//	public SepsisData unload()
//	{
//		SepsisData data = CDMSerializer.objFactory.createSepsisData();
//		unload(data);
//		return data;
//	}
	
//	protected void unload(SepsisData data)
//	{
//		super.unload(data);
//		if(this.severity!=null)
//			data.setSeverity(severity.unload());
//		if(this.compartment!=null)
//			data.setCompartment(compartment);
//	}
	
// Standard Get/Has and ToString methods needed for all BioGears Patient Actions
	public String getCompartment()
	{
		return compartment;
	}
	public boolean hasCompartment()
	{
		return compartment==null?false:true;
	}
	public void setCompartment(String compartment)
	{
		this.compartment = compartment;
	}
	
	public SEScalar0To1 getSeverity()
	{
		if (severity==null)
			{
				severity = new SEScalar0To1();
			}
		return severity;
	}
	public boolean hasSeverity()
	{
		return severity==null?false:true;
	}

	
	public String toString()
	{
		if (this.isValid())
			{
					return "Patient Condition:  Sepsis"
							+"\n\tSite of Infection: " + getCompartment()
							+"\n\tSeverity: " + Double.toString(severity.getValue());					
			}
		else
			return "Action not specified properly";
	}
}


