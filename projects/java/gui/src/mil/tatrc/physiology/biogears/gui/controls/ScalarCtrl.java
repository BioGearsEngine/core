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
package mil.tatrc.physiology.biogears.gui.controls;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import mil.tatrc.physiology.datamodel.properties.SEScalar;
import mil.tatrc.physiology.utilities.StringUtils;

public class ScalarCtrl 
{
	
	public static class ScalarProperty
	{
		public String       name;
		public List<String> units;	
		public Class<?>     scalarClass;
	}
	public static class ScalarPropertyComparator implements Comparator<ScalarProperty> 
	{
	    public int compare(ScalarProperty from, ScalarProperty to) 
	    {
	        return from.name.compareTo(to.name);
	    }
	}
	public static List<ScalarProperty> getScalarProperties(Class<?> c, boolean recursive)
	{
		List<ScalarProperty> properties = new ArrayList<ScalarProperty>();
		for (Method m : c.getMethods())
		{
			if (m.getName().startsWith("get"))
			{
				if (SEScalar.class.isAssignableFrom(m.getReturnType()))
				{
					ScalarProperty ctrl = new ScalarProperty();
					ctrl.name = StringUtils.spaceCamelCase(m.getName().replaceAll("get", ""));
					ctrl.scalarClass = m.getReturnType();
					ctrl.units=getUnitsList((Class<? extends SEScalar>)ctrl.scalarClass);
					properties.add(ctrl);
				}
				else if (recursive && m.getReturnType().getSimpleName().startsWith("SE"))
				{
					List<ScalarProperty> childProperties = getScalarProperties(m.getReturnType(), true);
					if(m.getReturnType().getSimpleName().equals("SESubstance"))
						continue;
					if(m.getReturnType().getSimpleName().equals("SESubstanceFraction"))
						continue;
					if(m.getReturnType().getSimpleName().equals("SESubstanceCompartmentEffect"))
						continue;
					if(m.getReturnType().getSimpleName().equals("SENutrition"))
					{
					  for(ScalarProperty p : childProperties)
					    p.name = "Stomach "+p.name;
					}
					properties.addAll(childProperties);
				}
			}
		}
		Collections.sort(properties, new ScalarPropertyComparator());
		return properties;
	}
	
	public static List<String> getUnitsList(Class<? extends SEScalar> c)
	{
	  List<String> unitList = new ArrayList<String>();	 
		for(Method m : c.getMethods())
		{
			if(m.getName().equals("getValue")&&m.getParameterTypes().length==1)
			{
				if(m.getParameterTypes()[0].isEnum())
				{
				    Enum<?>[] enums = (Enum<?>[]) m.getParameterTypes()[0].getEnumConstants();
				    for(Enum<?> e : enums)
				        unitList.add(e.toString());
					return unitList;
				}
			}
		}
		return null;
	}	

}
