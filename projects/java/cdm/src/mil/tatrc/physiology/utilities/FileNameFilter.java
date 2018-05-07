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

import java.io.File;
import java.io.FilenameFilter;
import java.util.*;

/**
 * Filters out names that do not equal given "filename" and are not files(directories)
 * Filters out names that have .zip if ExcludeZip is true 
 * @author khaith
 *
 */
public class FileNameFilter implements FilenameFilter
{
  String filename;
  Set<String> ignoredExtensions=new HashSet<String>();
  
  public FileNameFilter(String filename)
  {
    this.filename = filename;
  }
  
  public FileNameFilter(String filename, String[] excludeExtensions)
  {
  	int dot=-1;
    this.filename = filename;
    for(String ext : excludeExtensions)
    {
    	dot=ext.indexOf(".");
    	if(dot==-1)
    		this.ignoredExtensions.add(ext);
    	else
    		this.ignoredExtensions.add(ext.substring(dot+1));
    }
  }
  
  public boolean accept(File dir, String name)
  {  
    int indexOfDelim;
    String prefix,suffix;
    
    indexOfDelim = name.lastIndexOf(".");
    if(indexOfDelim>-1)
    	prefix=name.substring(0, indexOfDelim);
    else
    	prefix=name;
		if(!prefix.equals(this.filename))
			return false;
		
    if(!this.ignoredExtensions.isEmpty())
    {
    	suffix=name.substring(indexOfDelim+1);
    	for(String ext : this.ignoredExtensions)
    	{
    		if(suffix.equals(ext))
    		{
    			return false;
    		}
    	}
    }
   return true;
  }
}

