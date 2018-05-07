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

package mil.tatrc.physiology.datamodel.compartment;


import java.util.ArrayList;
import java.util.List;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.LiquidCompartmentData;
import mil.tatrc.physiology.datamodel.properties.SEScalar;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.quantity.SELiquidSubstanceQuantity;

public class SELiquidCompartment extends SEFluidCompartment
{
  protected SEScalar pH;
	protected List<SELiquidSubstanceQuantity> substanceQuantities;
	
	public SELiquidCompartment()
	{
	  super();
		this.substanceQuantities=new ArrayList<SELiquidSubstanceQuantity>();
	}
	
	public void reset()
	{
		super.reset();
		this.pH=null;
		for (SELiquidSubstanceQuantity sq : this.substanceQuantities)
			sq.reset();
	}
	
	public boolean load(LiquidCompartmentData in)
	{
		super.load(in);
		if(in.getPH()!=null)
		  getPH().load(in.getPH());
		//SESubstance* substance;
		//SECompartmentSubstanceQuantity* subQ;
		//for(int i = 0; i < in.getSubstanceQuantity().size(); i++)
		//{
		// TODO Look up the substance somehow, pass in.get managers in.get load?
		//std::cout<<in.get.SubstanceQuantity()[i].Name()<<std::endl;
		//substance = new SESubstance();//SubstanceManagerSubstance(in.get.SubstanceQuantity[i].name());
		//subQ = new SECompartmentSubstanceQuantity(*substance);
		//m_SubstanceQuantities.push_back(subQ);
		//}
		return true;
	}
	
	public LiquidCompartmentData unload()
	{
		LiquidCompartmentData to = CDMSerializer.objFactory.createLiquidCompartmentData();
		unload(to);
		return to;		
	}
	
	protected void unload(LiquidCompartmentData data)
	{
	  super.unload(data);
		if(this.hasPH())
		  data.setPH(this.pH.unload());
		// TODO SUb Q
	}
	
	public SEScalar getPH() 
  {
    if (pH == null)
      pH = new SEScalar();
    return pH;
  }
  public boolean hasPH()
  {
    return pH == null ? false : pH.isValid();
  }
	
	public boolean hasSubstanceQuantities()
	{
		return substanceQuantities.size() > 0 ? true : false;
	}
	public boolean hasSubstanceQuantity(SESubstance substance)
	{
		for (SELiquidSubstanceQuantity csq : substanceQuantities)
		{
			if (csq.getSubstance() == substance)
				return true;
		}
		return false;
	}
	public SELiquidSubstanceQuantity getSubstanceQuantity(SESubstance substance)
	{
		for (SELiquidSubstanceQuantity csq : substanceQuantities)
		{
			if (substance == csq.getSubstance())
				return csq;
		}
		SELiquidSubstanceQuantity csq = new SELiquidSubstanceQuantity(substance);
		substanceQuantities.add(csq);
		return csq;
	}
	public List<SELiquidSubstanceQuantity> getSubstanceQuantities()
	{
		return substanceQuantities;
	}
	public void removeSubstanceQuantity(SESubstance substance)
	{
		for (SELiquidSubstanceQuantity csq : substanceQuantities)
		{
			if (csq.getSubstance() == substance)
			{
				substanceQuantities.remove(csq);
				return;
			}
		}
	}
	public void removeSubstanceQuantity(String substanceName)
	{
		for (SELiquidSubstanceQuantity csq : substanceQuantities)
		{
			if (csq.getSubstance().getName().equals(substanceName))
			{
				substanceQuantities.remove(csq);
				return;
			}
		}
	}
}
