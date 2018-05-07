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
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.*;

/**
 * @author khaith
 *
 */
public class OrderedProperties extends Properties 
{
  private Map<String,String> properties=new LinkedHashMap<String,String>();
  
  public void load(String fileName) throws FileNotFoundException, IOException
  {
  	File file = new File(fileName);
		InputStream input;
		this.load(input = new FileInputStream(file));
  }

  public Enumeration keys() { return Collections.enumeration(properties.keySet()); }
  public Set<String> getKeys() { return properties.keySet();  }

  public Enumeration elements() { return Collections.enumeration(properties.values()); }
  public Collection<String> getElements() { return properties.values(); }

  public boolean contains(Object value) { return properties.containsValue(value);    }

  public void putAll(Map<? extends Object, ? extends Object> map) {
      properties.putAll((Map<? extends String, ? extends String>)map);
  }

  public int size() { return properties.size();   }

  public boolean isEmpty() { return properties.isEmpty();    }

  public boolean containsKey(Object key) { return properties.containsKey(key);    }

  public boolean containsValue(Object value) {  return properties.containsValue(value);    }

  public String get(Object key) { return properties.get(key);    }

  public String put(Object key, Object value) { return properties.put((String)key, (String) value);    }

  public Object remove(Object key) { return properties.remove(key);    }

  public void clear() { properties.clear();    }

  public Set keySet() { return properties.keySet();    }
  public Set<String> getKeySet() { return properties.keySet(); }
 
  public Collection values() { return properties.values();    }
  public Collection<String> getValues() { return properties.values(); }

  public Set entrySet()  { return properties.entrySet();  }
  public Set<Map.Entry<String, String>> getEntrySet() { return properties.entrySet(); }

  public boolean equals(Object o)  { return properties.equals(o);  }

  public int hashCode()  { return properties.hashCode();  }

  public String getProperty(String key)
  {
    Object oval = get(key);
    String sval = (oval instanceof String) ? (String)oval : null;
    return ((sval == null) && (defaults != null)) ? defaults.getProperty(key) : sval;
  }
  
  public void list(java.io.PrintStream out)
  {
    for(String key : properties.keySet())
     out.println(key + "=" + properties.get(key));
  }

  public void list(java.io.PrintWriter out)
  {
    for(String key : properties.keySet())
      out.println(key + "=" + properties.get(key));
  }
}


