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
package mil.tatrc.physiology.datamodel.doxygen;

import java.io.*;

import mil.tatrc.physiology.utilities.Log;

/**
 * 
 */
public class DoxygenPreprocessor
{
  public static void main(String[] args)
  {
    Log.setFileName("DoxygenPreprocessor.log");
    try
    {
      if(args.length!=2)
      {
        Log.info("Command arguments are : <Directory of files to process> <Directory to place processed files>");
        return;
      }
      String line;
      File srcDir = new File(args[0]);
      if(!srcDir.isDirectory())
      {
        Log.info("Cannot find Source Directory : "+args[0]);
        return;
      }
      File dDir = new File(args[1]);
      dDir.mkdir();
      
      for (String fName : srcDir.list())
      {        
        if(new File(fName).isDirectory())
          continue;// Not making this recursive at this point
        Log.info("Processing file : "+args[0]+"/"+fName);
        FileReader in = new FileReader(args[0]+"/"+fName);
        BufferedReader inFile = new BufferedReader(in);
        PrintWriter writer = new PrintWriter(dDir.getAbsolutePath()+"/"+fName, "UTF-8");
        while ((line = inFile.readLine()) != null)
        {
          if(line.indexOf("@insert")>-1)
          {
            String iName = line.substring(line.indexOf(" ")).trim();
            File f = new File(args[0]+"/"+iName); 
            if(f.exists())
            {
              // Read this file
              FileReader iFile = new FileReader(args[0]+"/"+iName);            
              BufferedReader insertFile = new BufferedReader(iFile);
              while ((line = insertFile.readLine()) != null)
              {
                writer.println(line);
              }
              insertFile.close();
            }
            else
            {
              Log.error("Could not find "+f.getName()+" to insert into this page");
              writer.println("<img src=\"./images/MissingTable.jpg\"><center><i>Could not find "+f.getName()+"</i></center><br>");
            }
          }
          else
            writer.println(line);
        }
        inFile.close();
        writer.close();
      }      
    }
    catch (Exception ex)
    {
      Log.error("Unable to create single validation table file.",ex);
    }
  }

}