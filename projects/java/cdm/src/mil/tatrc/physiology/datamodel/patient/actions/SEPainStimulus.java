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
import mil.tatrc.physiology.datamodel.bind.PainStimulusData;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;

public class SEPainStimulus extends SEPatientAction
{
	protected SEScalar0To1 severity;
	protected String location;

	public SEPainStimulus()
	{
		severity = null;
	}
	
	public void reset()
	{
		super.reset();
		if(severity!=null)
			this.severity.invalidate();
		this.location=null;
	}
	
	public void copy(SEPainStimulus other)
	{
		if(this==other)
			return;
		super.copy(other);
		location = other.location;
		severity = other.severity;
	}
	
	public boolean isValid()
	{
		return hasSeverity();
	}
	
	public boolean load(PainStimulusData in)
	{
		super.load(in);
    this.location=in.getLocation();
    this.getSeverity().load(in.getSeverity());
    
    return isValid();
	}
	
	public PainStimulusData unload()
	{
		PainStimulusData data = CDMSerializer.objFactory.createPainStimulusData();
		unload(data);
		return data;
	}
	
	protected void unload(PainStimulusData data)
	{
		super.unload(data);
		if (severity != null)
			data.setSeverity(severity.unload());
	}
	
	public String getLocation()
		{
		return location;
		}

	public boolean hasLocation()
	{
		return location==null?false:true;
	}
	public void setLocation(String location)
	{
		this.location = location;
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
		if (severity != null)
		{
			if(severity.getValue()==0.0)
			{
				return "Pain Stimulus Stopped"
				+"\n\tLocation: " + getLocation();
			}
		else
			{
				return "Pain Stimulus"
				+"\n\tLocation: " + getLocation()
				+"\n\tSeverity: " + Double.toString(severity.getValue());
								
					}
		}
		else
			return "Action not specified properly";
	}
}
