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

package mil.tatrc.physiology.datamodel.scenario.actions;

import java.io.Serializable;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import mil.tatrc.physiology.datamodel.bind.ActionData;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceCompound;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.utilities.Log;

public abstract class SEAction implements Serializable
{
	protected String comment;
	protected SEScalarTime scenarioTime;
	
	
	public SEAction() 
	{
		comment = null;
		scenarioTime = null;
	}
	
	public void copy(SEAction other) 
	{
		reset();
		comment = other.comment;
	}
	
	public void reset() 
	{
		comment = null;
	}
	
	public boolean load(ActionData in) 
	{
		reset();		
		this.comment = in.getComment();
		return true;
	}
	
	public abstract ActionData unload();
	
	protected void unload(ActionData data)
	{
		if(hasComment())
			data.setComment(this.comment);
	}
	
	public boolean hasComment()
	{
		return this.comment != null && !this.comment.isEmpty();
	}	
	public String getComment()
	{
		return this.comment;
	}
	public void setComment(String comment)
	{
		this.comment = comment;
	}
	public void invalidateComment()
	{
		this.comment = null;
	}
	
	
	public boolean hasScenarioTime()
	{
		return this.scenarioTime == null ? false : this.scenarioTime.isValid();
	}	
	public SEScalarTime getScenarioTime()
	{
		if(this.scenarioTime == null)
			this.scenarioTime = new SEScalarTime();
		return this.scenarioTime;
	}
	
	
	protected static List<String> actionPackages;
	public synchronized static SEAction bind2CDM(Object bind, SESubstanceManager subs)
	{
		String cClassName="";

		Class<?>  c=null;
		String bClassName = bind.getClass().getName();
		
		if(actionPackages==null)
		{
			actionPackages = new ArrayList<String>();
			actionPackages.add("scenario.actions");
			actionPackages.add("patient.actions");
			actionPackages.add("system.equipment.anesthesia.actions");
			actionPackages.add("system.equipment.inhaler.actions");
			actionPackages.add("system.environment.actions");
		}

		for(String pkg : actionPackages)
		{
			c=null;
			cClassName=bClassName.replace("bind", pkg);
			int s = cClassName.lastIndexOf(".");
			int e = cClassName.indexOf("Data", s);
			cClassName = cClassName.substring(0, s+1) + "SE" + cClassName.substring(s+1,e);
			try{
				c = Class.forName(cClassName);
				break;
			}catch(Exception ex){};
		}
		try
		{
			if(c!=null)
			{
				Object o=null;
				for(Constructor<?> ctor : c.getConstructors())
				{
					Class<?>[] params = ctor.getParameterTypes();
					if(params.length>0)
					{
						if(params[0].isAssignableFrom(SESubstance.class))
						{
							Method m=bind.getClass().getMethod("getSubstance");
							String subName = (String)m.invoke(bind);
							o=ctor.newInstance(subs.getSubstance(subName));
						}
						else if(params[0].isAssignableFrom(SESubstanceCompound.class))
						{
							Method m=bind.getClass().getMethod("getSubstanceCompound");
							String subName = (String)m.invoke(bind);
							o=ctor.newInstance(subs.getCompound(subName));
						}
					}
				}
				if(o==null)
					o = c.newInstance();
				for(Method m : c.getMethods())
				{
					if(m.getName().equals("load"))
					{
						if(m.getParameterTypes().length==1)						
							m.invoke(o, bind);
						else// I assume you need the substance manager, which is a good assumption now...
							m.invoke(o, bind, subs);
						return (SEAction)o;
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
	
	public abstract boolean isValid();
	public abstract String toString();
}
