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

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.lang.reflect.Method;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBElement;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.xml.sax.SAXException;


/**
 * @author abray
 *
 */
public class JAXBSerializer
{
  protected JAXBContext    jbctxt         = null;
  protected Marshaller     marshaller     = null;
  protected Unmarshaller   unmarshaller   = null;
  protected final String   FILE_EXTENSION = "xml";
  protected SchemaFactory  sf             = null;
  protected String         bindPackage;
  protected Object         objFac;

  protected JAXBElement<?> out;
  protected Class<?>       spec[] = new Class[1];
  protected Object         args[] = new Object[1];
  
  public JAXBSerializer(String xsdDirectory, String xsdFile, String bindPackage, Object objectFactory) throws JAXBException, SAXException
  {
    File schemaFile = new File(xsdDirectory+"/"+xsdFile).getAbsoluteFile();
    if(!schemaFile.exists())
    {
      Log.error("Unable to find schema at "+xsdDirectory+"/"+xsdFile);
      return;
    }
    this.bindPackage = bindPackage;
    this.objFac   = objectFactory;
    
    this.jbctxt = JAXBContext.newInstance(this.bindPackage,this.objFac.getClass().getClassLoader());
    this.sf=SchemaFactory.newInstance(
      javax.xml.XMLConstants.W3C_XML_SCHEMA_NS_URI);
    this.marshaller = this.jbctxt.createMarshaller();
    this.marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
    this.marshaller.setProperty(Marshaller.JAXB_SCHEMA_LOCATION, "");
    this.unmarshaller = this.jbctxt.createUnmarshaller();
    // Set up validation
    Schema schema = sf.newSchema(schemaFile);
    this.unmarshaller.setSchema(schema);
    this.marshaller.setSchema(schema);
  }
  
  public synchronized String getBindPackage()
  {
    return this.bindPackage;
  }

  /**
   * Serializes a JAXB object into an XML string
   *
   * @param data JAXB object to serialize
   *
   * @return XML string representing JAXB object
   */
  public synchronized String serialize(Object data)
  {
    if(data==null)
      throw new RuntimeException("JAXBSerializer recieved null jaxb data object for serialization.");

    String str;
    ByteArrayOutputStream stream = new ByteArrayOutputStream();
    try
    {
      String rootName="";
      try
      {
        args[0] = data;
        spec[0] = data.getClass();
        rootName = data.getClass().getName();
        rootName = rootName.substring(rootName.lastIndexOf('.')+1,rootName.length()-4);
        Method method = objFac.getClass().getMethod("create"+rootName, spec);
        out = (JAXBElement<?>)method.invoke(objFac, args);
        marshaller.marshal(out, stream);
        str = stream.toString();
        return str;
      }
      catch(JAXBException e)
      {
        Log.error("Serialization error: ObjectFactory failed to find and/or call method create"+rootName+"Data to serialize to",e);
        return null;
      } 
    }
    catch (Exception e)
    {
      Log.error("Error Writing XML: ",e);
      if(stream.size()>0)
        Log.error("Error Writing XML: " + stream);
      e.printStackTrace();

      return null;
    }
  }

  /**
   * Deserializes an XML string into a JAXB object
   *
   * @param xml XML string to deserialize
   *
   * @return JAXB object representing string
   */
  public synchronized Object serialize(String xml)
  {
    try
    {
      ByteArrayInputStream stream = new ByteArrayInputStream(xml.getBytes());      
      JAXBElement<?> data = null;
      synchronized(unmarshaller)
      {
      	data = (JAXBElement<?>)unmarshaller.unmarshal(stream);
      }
      return data.getValue();
    }
    catch (Exception e)
    {
      Log.error("Error Reading XML ",e);
      Log.error("XML"+xml);
      e.printStackTrace();

      return null;
    }
  }

  /**
   * Serialized a jaxb object to a file
   * @param fileName
   * @param data
   * @return
   */
  public synchronized void writeFile(String fileName, Object data)
  {
    if(data==null)
      throw new RuntimeException("JAXBSerializer recieved null jaxb data object for serialization.");

    if(data instanceof String)
    {
      FileUtils.writeFile(fileName, (String)data);
      return;
    }

    try
    {
      String rootName="";
      try
      {
        args[0] = data;
        spec[0] = data.getClass();
        rootName = data.getClass().getName();
        rootName = rootName.substring(rootName.lastIndexOf('.')+1,rootName.indexOf("Data"));
        Method method = objFac.getClass().getMethod("create"+rootName, spec);
        out = (JAXBElement<?>)method.invoke(objFac, args);
        FileOutputStream stream = new FileOutputStream(fileName);
        marshaller.marshal(out, stream);
        stream.close();
      }
      catch(JAXBException e)
      {
        Log.error("Serialization error: ObjectFactory failed to find and/or call method create"+rootName+"Data to serialize to",e);
      } 
    }
    catch (Exception e)
    {
      Log.error("Error Writing JAXB File: ",e);
    }
  }

  /**
   * Read a file name into a JAXB class object
   * @param fileName
   * @return
   */
  public synchronized Object readFile(String fileName)
  {
    File f = new File(fileName);
    if(f.exists())
    	return readFile(f);    
    Log.error(fileName+" does not exist");
    return null;
  }

  /**
   * Read a file name into a JAXB class object
   * @param fileName
   * @return
   */
  public synchronized Object readFile(File file)
  {
    try
    {
      FileInputStream fin = new FileInputStream(file);
      JAXBElement<?> data = null;
      synchronized(unmarshaller)
      {
      	data = (JAXBElement<?>)unmarshaller.unmarshal(fin);
      }
      fin.close();
      return data.getValue();
    }
    catch (Exception e)
    {
      Log.error("Error Reading XML File "+file,e);
      return null;
    }
  }

  public synchronized Object binaryDecode(String data)
  {
    try
    {
      if(data==null||data.isEmpty())
        return null;
      byte[] bytes = Base64.decode(data);
      return binaryDecode(bytes);
    }
    catch(Exception ex)
    {
      Log.error("Unable to read binary encoding ",ex);
      return null;
    }
  }

  public synchronized Object binaryDecode(byte[] bytes)
  {
    try
    {
      if(bytes==null)
        return null;
      ObjectInputStream ois = new ObjectInputStream(new ByteArrayInputStream(bytes));
      Object o = ois.readObject();
      ois.close();
      return o;
    }
    catch(Exception ex)
    {
      Log.error("Unable to read binary encoding ",ex);
      return null;
    }
  }

  public synchronized Object binaryDecodeDecompress(byte[] bytes)
  {
    try
    {
      if(bytes==null)
        return null;
      GZIPInputStream gzipin = new GZIPInputStream(new ByteArrayInputStream(bytes));
      ObjectInputStream ois = new ObjectInputStream(gzipin);
      Object o = ois.readObject();
      ois.close();
      return o;
    }
    catch(Exception ex)
    {
      Log.error("Unable to read binary encoding ",ex);
      return null;
    }
  }

  public synchronized String binaryEncodeToString(Object o)
  {
    try
    {
      return Base64.encode(binaryEncodeToBytes(o));
    }
    catch(Exception ex)
    {
      Log.error("Unable to binary encode object: "+o.getClass().getName(),ex);
      return null;
    }
  }

  public synchronized byte[] binaryEncodeToBytes(Object o)
  {
    try
    {
      ByteArrayOutputStream baos = new ByteArrayOutputStream();
      ObjectOutputStream oos = new ObjectOutputStream(baos);
      oos.writeObject(o);
      oos.close();
      return baos.toByteArray();
    }
    catch(Exception ex)
    {
      Log.error("Unable to binary encode object: "+o.getClass().getName(),ex);
      return null;
    }
  }

  public synchronized byte[] binaryEncodeCompressToBytes(Object o)
  {
    try
    {
      ByteArrayOutputStream baos = new ByteArrayOutputStream();
      GZIPOutputStream gzipout = new GZIPOutputStream(baos);
      ObjectOutputStream oos = new ObjectOutputStream(gzipout);
      oos.writeObject(o);
      oos.close();
      gzipout.finish();
      return baos.toByteArray();
    }
    catch(Exception ex)
    {
      Log.error("Unable to binary encode object: "+o.getClass().getName(),ex);
      return null;
    }
  }

  /**
   * Verifies that filename contains right extension
   *
   * @param fileName File name to check
   *
   * @return if it does or not
   */
  public synchronized boolean hasVaildFileExtension(String fileName)
  {
    if (!fileName.toLowerCase().endsWith(FILE_EXTENSION.toLowerCase()))
    {
      return false;
    }
    return true;
  }
}
