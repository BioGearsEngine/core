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
import mil.tatrc.physiology.datamodel.bind.HemorrhageData;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;

public class SEHemorrhage extends SEPatientAction
{
	//Required to specify a hemorrhage action
	protected String compartment;
	protected SEScalar0To1 severity;
	protected List<Integer> mcis;
	Map<String,List<Integer>> organMap = new HashMap<String,List<Integer>>();

	
	public SEHemorrhage()
	{
		compartment = null;
		mcis = null;
		severity = null;
		makeOrganMap(organMap);
	}
	
	public void copy(SEHemorrhage other)
	{
		if(this==other)
			return;
		super.copy(other);
		mcis = other.mcis;
		compartment = other.compartment;
		severity = other.severity;
	}
	
	public void reset()
	{
		super.reset();
		if(severity!=null)
			this.severity.invalidate();
		this.compartment=null;
		this.mcis = null;
	}
	
	public boolean isValid()
	{
		return hasCompartment() && hasSeverity();
	}
	
	public boolean load(HemorrhageData in)
	{
		super.load(in);
    this.compartment=in.getCompartment();
    this.getSeverity().load(in.getSeverity());
    
    this.setMCIS();
		
    return isValid();
	}
	
	public HemorrhageData unload()
	{
		HemorrhageData data = CDMSerializer.objFactory.createHemorrhageData();
		unload(data);
		return data;
	}
	
	protected void unload(HemorrhageData data)
	{
		super.unload(data);
		if(this.severity!=null)
			data.setSeverity(severity.unload());
		if(this.compartment!=null)
			data.setCompartment(compartment);
	}
	
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
		this.setMCIS();
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

	

	private void setMCIS()
	{
		int sev = (int)Math.ceil(5.0*this.severity.getValue());
	
		this.mcis = new ArrayList<Integer>();
		this.mcis.add(0,sev);
		
		switch(compartment){
			case "Arm":
				this.mcis.addAll(new ArrayList<Integer>(Arrays.asList(3,0,0,0)));
				break;
			case "Leg":
				this.mcis.addAll(new ArrayList<Integer>(Arrays.asList(4,0,0,0)));
				break;
			case "Major Artery":
				this.mcis.addAll(new ArrayList<Integer>(Arrays.asList(2,6,4,0)));
				break;
			case "Head":
				this.mcis.addAll(new ArrayList<Integer>(Arrays.asList(2,6,1,0)));
				break;
			default:
				//If we get in here, then we it's one of the comparments in the torso and it's easier to just map the rest
						this.mcis.add(2);
						this.mcis.addAll(organMap.get(compartment));
						this.mcis.add(0);
						break;		
		}
		if(mcis.size()<5)
		{
			//If we can't find it, assign it the code for aorta
			this.mcis.addAll(new ArrayList<Integer>(Arrays.asList(2,6,4,0)));
		}
	}
	
	private void makeOrganMap(Map<String,List<Integer>> organs)
	{
		organs.put("Vena Cava", new ArrayList<Integer>(Arrays.asList(6,6)));
		organs.put("Lung", new ArrayList<Integer>(Arrays.asList(7,1)));
		organs.put("Heart", new ArrayList<Integer>(Arrays.asList(7,2)));
		organs.put("Liver", new ArrayList<Integer>(Arrays.asList(8,1)));
		organs.put("Spleen", new ArrayList<Integer>(Arrays.asList(8,2)));
		organs.put("Splanchnic", new ArrayList<Integer>(Arrays.asList(8,3)));
		organs.put("Kidney", new ArrayList<Integer>(Arrays.asList(8,4)));
		organs.put("Small Intestine", new ArrayList<Integer>(Arrays.asList(8,5)));
		organs.put("Large Intestine", new ArrayList<Integer>(Arrays.asList(8,6)));
	}
	
	private String mcisToString()
	{
		String str = "";
		for (int i=0; i<mcis.size(); i++)
		{
			 str+=mcis.get(i).toString();
		}
		return str;
	}
	
	public String toString()
	{
		if (mcis != null)
			{
					if(severity.getValue()==0.0)
					{
							return "Stop Hemorrhage"
									+"\n\tCompartment: " + getCompartment();
					}
					else
					{
						return "Hemorrhage"
								+"\n\tCompartment: " + getCompartment()
								+"\n\tSeverity: " + Double.toString(severity.getValue())
								+"\n\tInjury Code: " + mcisToString();
								
					}
			}
		else
			return "Action not specified properly";
	}
}



