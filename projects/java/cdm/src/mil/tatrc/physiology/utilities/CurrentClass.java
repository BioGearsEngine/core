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

/**
 * 
 */
package mil.tatrc.physiology.utilities;

import java.util.Set;

/**
 * @author abray
 *
 */
public class CurrentClass extends SecurityManager 
{
  protected String getClassName() 
  {
    return getClassContext()[2].getName();
  }
  
  protected String getClassName(Set<String> ignoreMe) 
  {
    int i=-1;
    boolean skip;
    Class[] classes = getClassContext();
    for(Class clazz : classes)
    {
      i++;
      if(clazz.getName().indexOf("CurrentClass")>=0)
        continue;
      skip=false;
      for(String ignore : ignoreMe)
      {
        if(clazz.getName().indexOf(ignore)>=0)
        {
          skip=true;
          break;
        }
      }
      if(skip)continue;
      return clazz.getName()+"::"+Thread.currentThread().getStackTrace()[i+1].getLineNumber();
    }
    return "?CurrentClass?";
  }
  
  protected String getClassNameTrimmed(Set<String> ignoreMe)
  {
    int i=-1;
    boolean skip;
    Class[] classes = getClassContext();
    for(Class clazz : classes)
    {
      i++;
      if(clazz.getName().indexOf("CurrentClass")>=0)
        continue;
      skip=false;
      for(String ignore : ignoreMe)
      {
        if(clazz.getName().indexOf(ignore)>=0)
        {
          skip=true;
          break;
        }
      }
      if(skip)continue;
      String n = clazz.getName()+"::"+Thread.currentThread().getStackTrace()[i+1].getLineNumber();
      return n.substring(n.lastIndexOf(".")+1);
    }
    return "?CurrentClass?";
  }
  
  protected static CurrentClass cc=new CurrentClass();
  
  public static String getName()
  {
    return cc.getClassName();
  }
  
  public static String getName(Set<String>ignoreMe)
  {
    return cc.getClassName(ignoreMe);
  }
  
  public static String getTrimmedName(Set<String> ignoreMe)
  {
    return cc.getClassNameTrimmed(ignoreMe);
  }
}
