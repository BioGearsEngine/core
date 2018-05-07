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

package mil.tatrc.physiology.datamodel.substance;

import java.util.ArrayList;
import java.util.List;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.SubstanceConcentrationData;
import mil.tatrc.physiology.datamodel.bind.SubstanceCompoundData;
import mil.tatrc.physiology.utilities.StringUtils;

public class SESubstanceCompound
{
	protected String                 name;
	protected List<SESubstanceConcentration> components;
	
	public SESubstanceCompound()
	{
		
	}
	
	public void reset()
	{
		this.name=null;
		
		if(this.components!=null)
			this.components.clear();
	}
	
	public boolean load(SubstanceCompoundData data, SESubstanceManager mgr)
	{
		this.reset();
		if(data.getName()!=null)
			this.setName(data.getName());

		if(data.getComponent()!=null)
		{			
			for(SubstanceConcentrationData cData : data.getComponent())
			{
				this.createComponent(mgr.getSubstance(cData.getName())).load(cData);
			}
		}
		
		return true;
	}
	
	public SubstanceCompoundData unload()
	{
		SubstanceCompoundData to = CDMSerializer.objFactory.createSubstanceCompoundData();
		unload(to);
		return to;
	}
	
	protected void unload(SubstanceCompoundData to)
	{
		if(hasName())
			to.setName(this.name);
		
		if(hasComponents())
		{
			for(SESubstanceConcentration c : this.getComponents())
				to.getComponent().add(c.unload());
		}
	}
	
	public String  getName() { return this.name;}
	public void    setName(String name){this.name=name;}
	public boolean hasName(){return StringUtils.exists(this.name);}
	
	public SESubstanceConcentration createComponent(SESubstance s) 
	{ 
		return getComponent(s);
	}	
	public List<SESubstanceConcentration> getComponents() 
	{ 
		if(this.components==null)
			this.components=new ArrayList<SESubstanceConcentration>();
		return this.components;
	}	
	public boolean hasComponents() {return this.components==null?false:this.components.size()>0;}
	public boolean hasComponent(SESubstance s) 
	{
		if(this.components==null)
			return false;
		for(SESubstanceConcentration c : this.components)
		{
			if(c.getSubstance()==s)
				return true;
		}
		return false;
	}
	public SESubstanceConcentration getComponent(SESubstance s) 
	{		
		for(SESubstanceConcentration c : getComponents())
		{
			if(c.getSubstance()==s)
				return c;
		}
		SESubstanceConcentration c=new SESubstanceConcentration(s);
		this.getComponents().add(c);
		return c;
	}
}
