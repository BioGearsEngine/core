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
package mil.tatrc.physiology.utilities;

import java.lang.reflect.Field;
import java.util.*;


public class SimpleEquals
{
	public static boolean Equals(Object A, Object B)
	{
		if(A==null && B==null)
			return true;
		if(A.getClass() != B.getClass())
		{
			Log.error("Classes are not the same type");
			return false;
		}
		
		List<Field> fieldList = new ArrayList<Field>();
    Class<?> tmpClass = A.getClass();
    while (tmpClass != null) 
    {
        fieldList.addAll(Arrays.asList(tmpClass.getDeclaredFields()));
        tmpClass = tmpClass .getSuperclass();
    }
	
		for(Field f : fieldList)
		{			
			try
			{
				f.setAccessible(true);
				Object a = f.get(A);
				Object b = f.get(B);
				
				Log.info("Comparing "+f.getName());
				
				if(a==null && b==null)
					continue;
				if( (a==null && b!=null) || (a!=null && b==null))
				{
					Log.error(f.getName()+" is only present in " + (a==null?"B":"A"));
					return false;
				}
				
				
				if(f.getType().isPrimitive() || f.getType() == String.class || f.isEnumConstant())
				{
					if(!a.equals(b))
					{
						Log.error("A "+f.get(A) +" != B "+f.get(B));
						return false;
					}
				}
				else if(List.class.isAssignableFrom(f.getType()))
				{
					List<?> aList = (List<?>) a;
					List<?> bList = (List<?>) b;
					if(aList.size() != bList.size())
					{
						Log.error(f.getName()+" list sizes are not the same ");
						return false;
					}
					for(int i=0; i<aList.size(); i++)
					{
						if(!Equals(aList.get(i),bList.get(i)))
							return false;
					}
				}
				else
				{
					Log.info("Comparing "+a.getClass().getSimpleName());
					if(!Equals(a,b))
						return false;
				}								
			} 
			catch (IllegalArgumentException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			} 
			catch (IllegalAccessException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}			
		}		
		return true;
	}
}
