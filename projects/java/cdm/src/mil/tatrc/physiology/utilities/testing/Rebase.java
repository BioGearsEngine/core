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
package mil.tatrc.physiology.utilities.testing;

import java.io.File;
import java.io.IOException;
import java.util.*;

import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.OrderedProperties;

public class Rebase
{

  public static void main(String[] args)
  {
    boolean scenarios=true;
    Log.setFileName("Rebase.log");    
    String fromDir = "\\\\sed-biogears\\JenkinsBuild\\History\\MSVC-x86\\225 - a4c8402903f7c89e4799f9f2ad2c55339195e582\\Scenarios\\";
    String toDir = "..\\verification\\Scenarios\\";
    String configFile = "ALL";//"DebugRun.config";
    switch(args.length)
    {
      case 1:
      {
        if(args[0].equals("UnitTests"))
        {
          scenarios = false;
          fromDir = ".\\UnitTests\\";
          toDir = "..\\verification\\UnitTests\\";
          // TODO Not very well tested
        }
        break;
      }
      case 2:
      {
        fromDir = args[0];
        toDir = args[1];
        break;
      }
      case 3:
      {
        fromDir = args[0];
        toDir = args[1];
        configFile = args[2];
      }
      case 4:
      {
        fromDir = args[0];
        toDir = args[1];
        configFile = args[2];
        scenarios = args[3].equalsIgnoreCase("true") ? true:false;
      }
    }
    // TODO Make sure from/to dir ends with a slash
    
    Log.info("Rebasing from "+fromDir);
    Log.info("Rebasing to "+toDir);
    Log.info("Rebasing config "+configFile);
         
    File fDir = new File(fromDir);
    // TODO replace forward slashes with back slashes
    if(!fromDir.endsWith("\\"))
      fromDir = fromDir + "\\";

    if(!fDir.exists())
    {
      Log.error("From dir does not exist : "+fromDir);
      return;
    }
    if(!fDir.isDirectory())
    {
      Log.error("From dir is not a directory : "+fromDir);
      return;
    }
    try
    {
      FileUtils.createDirectory(toDir);
    }
    catch(Exception ex)
    {
      Log.error("Could not create to Directory : "+toDir,ex);
      return;
    }
    List<String> resultsFiles;
    if(configFile.isEmpty() || configFile.equalsIgnoreCase("ALL"))
      resultsFiles = FileUtils.findFiles(fromDir, ".txt", true);
    else
    {
      // Read the config file and find the files
      resultsFiles = new ArrayList<String>();
      OrderedProperties config = new OrderedProperties();
      try
      {
        config.load(configFile);
      }
      catch(Exception ex)
      {
        Log.error("Unable to find config file : "+configFile, ex);
        return;
      }
      for(String f : config.getKeys())
      {
        if(f.endsWith(".xml"))
        {
          String resultFile = fromDir+f;
          resultsFiles.add(resultFile.replaceAll(".xml","Results.txt"));
        }
      }
    }
    for(String rFile : resultsFiles)
    {
      String srcLog = "";
      if(scenarios)
        srcLog = rFile.replaceAll("Results.txt", ".log");
      String prepend = rFile.substring(rFile.indexOf(fromDir)+fromDir.length(),rFile.indexOf(".txt"));
      if(scenarios)
      {
        String[] dirs = prepend.split("[/\\\\]");
        if(dirs.length==1)
        	continue;//We ignore anything in the Scenario\root folder, those are not verifiable runs, (like runs to make initial state files)
        prepend = dirs[0] +"\\\\Current Baseline\\\\" + dirs[1];
      }
      String newFile = toDir + prepend + ".zip";
      Log.info("Creating new file "+newFile);
      String rPath = "";
      for(String rDir : newFile.split("\\\\"))
      {
        if(rDir.endsWith(".zip"))
        {
          break;
        }
        rPath += rDir + "\\";
      }
      try
      {
        FileUtils.createDirectory(rPath);
      } 
      catch (IOException ex)
      {
        Log.error("Could not create directory : "+rPath, ex);
      }
      File oldZip = new File(newFile);
      if(oldZip.exists())
        oldZip.delete();
      FileUtils.zipFiles(new String[]{rFile}, newFile);
      try
      {
        if(!srcLog.isEmpty())
          FileUtils.copy(new File(srcLog), new File(toDir + prepend + ".log"));
      }
      catch(Exception ex)
      {
        Log.error("Could not find a log : " + srcLog);
      }
    }
  }

}
