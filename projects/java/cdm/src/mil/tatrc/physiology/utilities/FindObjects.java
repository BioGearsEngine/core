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

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.*;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import org.reflections.Reflections;
import org.reflections.scanners.*;
import org.reflections.util.*;

/**
 * @author abray
 *
 */
public class FindObjects
{
  /**
   * Examine the system and find all classes of a specific type
   * Instantiate all modules and put them in a list.
   */
  public static <T> Set<Class<? extends T>> findClassSubTypes(String packageName, Class<T> target)
  {
    Reflections reflections = new Reflections(packageName);
    Set<Class<? extends T>> found = reflections.getSubTypesOf(target);
    return found;
  }
  public static Set<Class<? extends Object>> findAllClasses(String packageName)
  {
  	List<ClassLoader> classLoadersList = new LinkedList<ClassLoader>();
  	classLoadersList.add(ClasspathHelper.contextClassLoader());
  	classLoadersList.add(ClasspathHelper.staticClassLoader());

  	Reflections reflections = new Reflections(new ConfigurationBuilder()
  	    .setScanners(new SubTypesScanner(false /* don't exclude Object.class */), new ResourcesScanner())
  	    .setUrls(ClasspathHelper.forClassLoader(classLoadersList.toArray(new ClassLoader[0])))
  	    .filterInputsBy(new FilterBuilder().include(FilterBuilder.prefix(packageName))));
  	
  	Set<Class<? extends Object>> allClasses = reflections.getSubTypesOf(Object.class);
  	return allClasses;
  }
  
  /** This is a VERY brute force way of looking for class relationships */
  public static Set<Class<?>> findAssignableClassInArchive(String arPath, String packageName, Class<?> target) throws IOException
  {
    Class<?> found;
    Set<Class<?>> classes = new HashSet<Class<?>>();
    
    String className;
    String path = packageName.replace(".", "/");

    ZipEntry ae;
    String itemName;
    ZipFile zip = new ZipFile(arPath);
    Enumeration<? extends ZipEntry> e = zip.entries();
    
    while (e.hasMoreElements())
    {
      ae = (ZipEntry) e.nextElement();
      itemName = ae.getName();
      try
      {
        if(itemName.indexOf(path)>-1 && itemName.endsWith(".class"))
        {
          className = itemName.replace("/", ".").substring(0, itemName.indexOf(".class"));
          found = Class.forName(className,false,Thread.currentThread().getContextClassLoader());
          if(target.equals(found))
            continue;// Don't want the same class
          if (target.isAssignableFrom(found))
          {
            classes.add(found);
          }
        }
      }
      catch(Exception ex)
      {
        Log.error(ex);
      }
    }
    zip.close();
    return classes;
  }
  
  public static Set<File> findFile(String fileName, File directory)
  {
    
    Set<File> found = new HashSet<File>();
    if(directory.isFile())
    {
      Log.warn("findFile needs a directory to search, not a file");
      return found;
    }
    findFile(found,fileName,directory);    
    return found;
  }
  private static void findFile(Set<File> found, String fileName, File directory)
  {
    for(File f : directory.listFiles())
    {
      if(f.isDirectory())
      {
        findFile(found,fileName,f);
      }
      else if(f.isFile())
      {
        if(f.getName().equals(fileName))
          found.add(f);
      }
    }
  }
  
  public static class BagMethod
  {
    public String propertyName;
    /**Methods of: has"propertyName", get"propertyName"*/
    public Method has=null,get=null;
    public Class<?> returnType=null;
    public String   myName;
    public Object   me;
  } 
  /** Analyze the provided class for atomics */
  public static List<BagMethod> getBagMethods(Class<?> clazz, List<String> skipProperties)
  {
  	//if(clazz.getSimpleName().equals("ConsumeMeal"))
  	//	Log.info("break");
    // Get all methods we care about (get and has methods)
    String methodName;
    Map<String,Method> methodMap = new HashMap<String,Method>();
    Method[] methods=clazz.getMethods();
    // Let's get all method names of methods we care about
    // Or at least with out ones we don't care about...
    for(Method method : methods)
    {
      methodName=method.getName();
      if(methodName.length()<3)
        continue;     
      if(!methodName.startsWith("has")&&!methodName.startsWith("get"))
        continue;
        
      if(methodName.startsWith("get"))
      {
        if(method.getGenericParameterTypes().length!=0)
          continue;
      }
      else if(methodName.startsWith("has")&&method.getGenericParameterTypes().length!=0)
        continue;       
      methodMap.put(methodName,method);
    }
    
    // Now bin them by type name:
    String property;    
    List<BagMethod> bagMethods = new ArrayList<BagMethod>();
    for (String name : methodMap.keySet())
    {
      if (name.startsWith("has"))
      {
      	property = name.substring(3);
      	if(skipProperties!=null)
      	{
      		boolean found=false;
      		for(String skip : skipProperties)
      		{
      			if(property.equals(skip))
      			{
      				found=true;
      				break;
      			}
      		}
      		if(found)
      			continue;
      	}
        BagMethod found = new BagMethod();
        found.me = clazz;
        found.propertyName = property;
        found.has = methodMap.get("has" + property);
        found.get = methodMap.get("get" + property);
        if(found.get != null)
        {
        	bagMethods.add(found);
          found.returnType = found.get.getReturnType();
        }
      }
    }
    return bagMethods;
  }
}
