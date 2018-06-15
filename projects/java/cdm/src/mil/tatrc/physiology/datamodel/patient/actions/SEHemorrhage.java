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
import mil.tatrc.physiology.datamodel.properties.SEScalarVolumePerTime;
import mil.tatrc.physiology.datamodel.properties.SEScalarFlowResistance;

public class SEHemorrhage extends SEPatientAction
{
	//Required to specify a hemorrhage action
	protected String compartment;
	protected List<Integer> mcis;
	protected SEScalarVolumePerTime initialRate;
	protected SEScalarFlowResistance bleedResistance;
	Map<String,List<Integer>> organMap = new HashMap<String,List<Integer>>();

	
	public SEHemorrhage()
	{
		compartment = null;
		initialRate = null;
		bleedResistance = null;
		mcis = null;
		makeOrganMap(organMap);
	}
	
	public void copy(SEHemorrhage other)
	{
		if(this==other)
			return;
		super.copy(other);
		initialRate = other.initialRate;
		bleedResistance = other.bleedResistance;
		mcis = other.mcis;
		compartment = other.compartment;
	}
	
	public void reset()
	{
		super.reset();
		if(initialRate!=null)
			this.initialRate.invalidate();
		if(bleedResistance!=null)
			this.bleedResistance.invalidate();
		this.compartment=null;

		this.mcis = null;
	}
	
	public boolean isValid()
	{
		return hasCompartment() && hasInitialRate();
	}
	
	public boolean load(HemorrhageData in)
	{
		super.load(in);
    this.getInitialRate().load(in.getInitialRate());
    this.compartment=in.getCompartment();
    this.getBleedResistance();
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
		if(this.initialRate!=null)
			data.setInitialRate(initialRate.unload());
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
	
	public SEScalarVolumePerTime getInitialRate()
	{
		if (initialRate==null)
			{
				initialRate = new SEScalarVolumePerTime();
			}
		return initialRate;
	}
	public boolean hasInitialRate()
	{
		return initialRate==null?false:true;
	}

	public SEScalarFlowResistance getBleedResistance()
	{
		if (bleedResistance==null)
			{
				bleedResistance = new SEScalarFlowResistance();
			}
		return bleedResistance;
	}
	public boolean hasBleedResistance()
	{
		return bleedResistance==null?false:true;
	}

	private void setMCIS()
	{
		int sev = 0;
		//We're scaling flow rate by a value that will cause irreversible shock in 10 minutes, assuming blood volume = 5L
		double flowScale_mL_Per_min = 250.0;
		if(this.initialRate.getValue("mL/min") >= flowScale_mL_Per_min)
		{
			sev = 5;
		}
		else
		{
			sev = (int)Math.ceil(5.0*this.initialRate.getValue("mL/min") / flowScale_mL_Per_min);
		}
	
		this.mcis = new ArrayList<Integer>();
		this.mcis.add(0,sev);
		
		switch(compartment){
			case "LeftArm":
			case "RightArm":
				this.mcis.addAll(new ArrayList<Integer>(Arrays.asList(3,0,0,0)));
				break;
			case "LeftLeg":
			case "RightLeg":
				this.mcis.addAll(new ArrayList<Integer>(Arrays.asList(4,0,0,0)));
				break;
			case "Aorta":
				this.mcis.addAll(new ArrayList<Integer>(Arrays.asList(2,6,4,0)));
				break;
			case "Brain":
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
		organs.put("VenaCava", new ArrayList<Integer>(Arrays.asList(6,6)));
		organs.put("LeftLung", new ArrayList<Integer>(Arrays.asList(7,1)));
		organs.put("RightLung", new ArrayList<Integer>(Arrays.asList(7,1)));
		organs.put("Myocardium", new ArrayList<Integer>(Arrays.asList(7,2)));
		organs.put("Liver", new ArrayList<Integer>(Arrays.asList(8,1)));
		organs.put("Spleen", new ArrayList<Integer>(Arrays.asList(8,2)));
		organs.put("Splanchnic", new ArrayList<Integer>(Arrays.asList(8,3)));
		organs.put("LeftKidney", new ArrayList<Integer>(Arrays.asList(8,4)));
		organs.put("RightKidney", new ArrayList<Integer>(Arrays.asList(8,4)));
		organs.put("SmallIntestine", new ArrayList<Integer>(Arrays.asList(8,5)));
		organs.put("LargeIntestine", new ArrayList<Integer>(Arrays.asList(8,6)));
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
					if(initialRate.getValue("mL/min")==0.0)
					{
							return "Stop Hemorrhage"
									+"\n\tCompartment: " + getCompartment();
					}
					else
					{
						return "Hemorrhage"
								+"\n\tCompartment: " + getCompartment()
								+"\n\tInitial Bleeding Rate: " + Double.toString(initialRate.getValue())
								+"\n\tInjury Code: " + mcisToString();
								
					}
			}
		else
			return "Action not specified properly";
	}
}



