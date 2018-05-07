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

package mil.tatrc.physiology.datamodel.scenario.conditions;

import java.io.Serializable;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import mil.tatrc.physiology.datamodel.bind.ConditionData;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceCompound;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.utilities.Log;

public abstract class SECondition implements Serializable
{
	
	public SECondition() 
	{
		
	}
	
	public void copy(SECondition other) 
	{
	  reset();
	}
	
	public void reset() 
	{
		
	}
	
	public boolean load(ConditionData in) 
	{
		reset();
		return true;
	}
	
	public abstract ConditionData unload();
	
	protected void unload(ConditionData data)
	{
		
	}
	
	protected static List<String> ConditionPackages;
	public static SECondition bind2CDM(Object bind, SESubstanceManager substances)
	{
		String cClassName="";

		Class<?>  c=null;
		String bClassName = bind.getClass().getName();
		
		if(ConditionPackages==null)
		{
			ConditionPackages = new ArrayList<String>();
			ConditionPackages.add("patient.conditions");
			ConditionPackages.add("system.environment.conditions");
		}

		for(String pkg : ConditionPackages)
		{
			c=null;
			cClassName=bClassName.replace("bind", pkg);
			int s = cClassName.lastIndexOf(".");
			int e = cClassName.indexOf("Data", s);
			cClassName = cClassName.substring(0, s+1) + cClassName.substring(s+1,e);
			try{
				c = Class.forName(cClassName);
				break;
			}catch(Exception ex){};
		}
		try
		{
			if(c!=null)
			{
				Object o = c.newInstance();
				for(Method m : c.getMethods())
				{
					if(m.getName().equals("load"))
					{
						if (m.getParameterCount() == 1)
						{
							m.invoke(o, bind);
							return (SECondition)o;
						}
						else if (m.getParameterCount() == 2)
						{
							m.invoke(o, bind, substances);
							return (SECondition)o;
						}
					}
				}
			}
			Log.error("SE Class not found for "+bClassName);
		}
		catch(Exception ex)
		{
			Log.error("Error looking for CDM class for "+bClassName,ex);
		}
		return null;
	}

	public abstract String toString();
}
