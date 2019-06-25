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

import java.io.BufferedWriter;
import java.io.BufferedReader;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;

import java.io.IOException;
import mil.tatrc.physiology.utilities.Log;

/**
 * 
 */
public class DoxygenPreprocessor
{
  public static void main(String[] args)
  {
    Log.setFileName("DoxygenPreprocessor.log");
    if(args.length!=2)
    { // Check # of input args
      Log.info("Command arguments are : <Directory of files to process> <Directory to place processed files>");
      return;
    }
    Path sourceDir = Paths.get(args[0]);
    if(!Files.isDirectory(sourceDir) ) { // locate source directory
      Log.error(String.format("Cannot find Source Directory : %s",args[0]));
      return;
    }
    Path destDir = Paths.get(args[1]);
    if(!Files.exists(destDir)) { // check for then create destination directory
      try {
        Files.createDirectories(destDir);
      } catch (IOException e) {
        Log.error (String.format("Unable to destination directory %s. I might not have file permissions", destDir)); 
      }
    }
    try {
      DirectoryStream<Path> directoryStream = Files.newDirectoryStream(sourceDir);
      for (Path file: directoryStream)  {        
        if( Files.isDirectory(file) || ! file.toString().toLowerCase().endsWith(".md") )
        {  continue; } // If the file is a directory or not an 'md' file, ignore it
        
        Log.info("Processing file : "+ file.toString() );
        BufferedReader inputSource = Files.newBufferedReader(file, StandardCharsets.ISO_8859_1);
        Path processedFile = destDir.resolve(file.getFileName());
        BufferedWriter processedResult      = Files.newBufferedWriter(processedFile,  StandardOpenOption.CREATE, StandardOpenOption.TRUNCATE_EXISTING);
        String line = "";
        while ((line = inputSource.readLine()) != null) {
          if(line.indexOf("@insert")>-1)   {
            Path stubFile = Paths.get(line.substring(line.indexOf(" ")).trim());
            if( Files.exists(stubFile) ) {
              BufferedReader stubReader = Files.newBufferedReader(stubFile);            
              while ((line = stubReader.readLine()) != null) {  
                processedResult.write(line);   
                processedResult.newLine();
              }
              stubReader.close();
            } else if( Files.exists(Paths.get(args[0],stubFile.toString())) ) {
              // Read this file
              BufferedReader stubReader = Files.newBufferedReader(Paths.get(args[0]).resolve(stubFile));
              while ((line = stubReader.readLine()) != null)
              {  
                processedResult.write(line);  
                processedResult.newLine();
              }
              stubReader.close();
            } else {
              Log.error( String.format("Could not find %s to insert into this page searched [., %s] ", stubFile.toString(), args[0]));
              processedResult.newLine();
              processedResult.write("<img src=\"./images/MissingTable.jpg\"><center><i>Could not find "+stubFile.toString() +"</i></center><br>");
            }
          } else {
            processedResult.write(line);
            processedResult.newLine();
          }
        }
        inputSource.close();
        processedResult.close();
      }      
      directoryStream.close();
    } catch (IOException ex) {
        Log.error (ex);
        Log.error (String.format("An Exception has occured with the filesystem. I might not have file permissions"));

    }
  }
}

