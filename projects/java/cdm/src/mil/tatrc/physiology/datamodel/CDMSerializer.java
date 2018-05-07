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

package mil.tatrc.physiology.datamodel;

import java.io.File;
import java.nio.file.Path;
import java.util.*;

import mil.tatrc.physiology.datamodel.bind.ObjectFactory;
import mil.tatrc.physiology.utilities.JAXBSerializer;
import mil.tatrc.physiology.utilities.Log;
/**
 * Serialization helper class.    
 * Provides services for  
 * - serializing an object to a string 
 * - deserializing a string into an object 
 * - writing an xml string to an xml file 
 * - reading an xml file into an xml string
 *
 * I think it's better to have a singleton wrapper
 * around the jaxb bind factory
 * @author abray
 */
public class CDMSerializer 
{ 
  private CDMSerializer()
  {
    
  }
  private static JAXBSerializer jaxb;
  private static List<String> ATOMIC_PKGS;
  private static List<String> ELEMENT_PKGS;
  private static String       workingDir = ".";
  
  public static final ObjectFactory objFactory = new ObjectFactory();
  
  public static void main(String[] args)
  {
    if(args.length==0)
    {
      Log.error("Must provide at least one input xml file to read");
      System.exit(1);
    }
    for(String file : args)
    {
      Object obj = CDMSerializer.readFile(file);
      if(obj != null)
      {
        Log.info(args[0]+" is a valid xml file for "+obj.getClass().getName());
      }
    }
  }
  private static JAXBSerializer get()
  {
    if(jaxb==null)
    {
      try
      {
        jaxb=new JAXBSerializer
          (
            workingDir,
            "xsd/BioGearsDataModel.xsd",
            "mil.tatrc.physiology.datamodel.bind",
            objFactory
          );
      }
      catch(Exception ex)
      {
        Log.error("Could not create CDMSerializer",ex);
      }
    }
    return jaxb;
  }
  
  public static synchronized String getBindPackage()
  {
    return get().getBindPackage();
  }
  
  public static List<String> getAtomicPackages()
  {
    if(ATOMIC_PKGS==null)
    {
      ATOMIC_PKGS= new ArrayList<String>();
    }
    return Collections.unmodifiableList(ATOMIC_PKGS);
  }
  
  public static List<String> getElementPackages()
  {
    if(ELEMENT_PKGS==null)
    {
      ELEMENT_PKGS= new ArrayList<String>();
    }
    return Collections.unmodifiableList(ELEMENT_PKGS);
  }
  
  public static synchronized String serialize(Object data)
  {
    return get().serialize(data);
  }
  
  public static synchronized Object serialize(String xml)
  {
    return get().serialize(xml);
  
  }
  public static synchronized void writeFile(String fileName, Object data)
  {
    get().writeFile(fileName,data);
  }
  
  public static synchronized Object readFile(String fileName)
  {
    return get().readFile(fileName);
  }
  
    public static synchronized Object readFile(Path fileName)
  {
    return get().readFile(fileName.toString());
  }

  public static synchronized Object readFile(File file)
  {
    return get().readFile(file);
  }
  
  public static synchronized Object binaryDecode(String data)
  {
    return get().binaryDecode(data);
  }
  
  public static synchronized Object binaryDecode(byte[] bytes)
  {
    return get().binaryDecode(bytes);
  }
  
  public static synchronized Object binaryDecodeDecompress(byte[] bytes)
  {
    return get().binaryDecodeDecompress(bytes);
  }
  
  public static synchronized String binaryEncodeToString(Object o)
  {
    return get().binaryEncodeToString(o);
  }
  
  public static synchronized byte[] binaryEncodeToBytes(Object o)
  {
    return get().binaryEncodeToBytes(o);
  }
  
  public static synchronized byte[] binaryEncodeCompressToBytes(Object o)
  {
    return get().binaryEncodeCompressToBytes(o);
  }
  
  public static synchronized boolean isCDMFile(String fileName)
  {
    return get().hasVaildFileExtension(fileName);
  }
}
