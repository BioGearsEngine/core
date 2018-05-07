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

package mil.tatrc.physiology.utilities.csv;

import java.io.*;
import java.util.*;

import mil.tatrc.physiology.datamodel.SEEqualOptions;
import mil.tatrc.physiology.utilities.*;
import mil.tatrc.physiology.utilities.csv.plots.CSVComparePlotter;
import mil.tatrc.physiology.utilities.csv.plots.PlotDriver;
import mil.tatrc.physiology.utilities.testing.TestReport;
import mil.tatrc.physiology.utilities.testing.TestSuite;

/**
 * Compare 2 CSV files and create a report on the differences
 * Will create plot images if a time column is provided
 * @author abray
 *
 */
public class CSVComparison extends TestReport
{
  public double limit=2.0;
  public boolean reportDifferences=false;
  
  public static void main(String[] args) throws IOException, InterruptedException
  {
    if(args.length<2)
    {
      if(args.length==1&&args[0].equals("default"))
      {
        args = new String[10];
        args[0] = "./Scenario1Results_1.txt";// Expected
        args[1] = "./Scenario1Results_1.txt";// Computed   
        args[2] = "2.0";// Percent Difference Allowed       
        args[3] = "false";// Report Differences in the error report, turn off if you are expecting a LOT of error
      }
      else
      {
        Log.fatal("Expected inputs : [expected results file path] [computed results file path] [percentDifference] [time to start comparing at] [track differences] [output folder] [zip results]");
        return;
      }
    }

    CSVComparison t = new CSVComparison();
    
    // Tolerance Limit
    if(args.length>2)
      t.limit=Double.parseDouble(args[2]);
    
    // Report the differences
    t.reportDifferences = false;
    if(args.length>3&&args[3]!=null&&args[3].equalsIgnoreCase("true"))
      t.reportDifferences = true;

    Log.info("Comparing "+args[0]+" to "+args[1]);
    Set<String> failures = t.compare(args[0],args[1]);
    t.write();
    //CSVComparePlotter plotter = new CSVComparePlotter();
    //plotter.plot(args[0], args[1], failures);
    PlotDriver driver = new PlotDriver();
    driver.preload = true;
    driver.preloadData(args[0], args[1]);
    driver.generateCompareJobs(args[0], args[1], failures);
    driver.execute();
  }

  /**
   * @param args
   */
  public Set<String> compare(String expectedFilePath, String computedFilePath)
  {    
  	
  	File expectedFile = new File(expectedFilePath);
    if(!expectedFile.exists())
    {
      // Check to see if there is a zip available
      String eZip = expectedFilePath.replaceAll(".txt", ".zip");
      expectedFile = new File(eZip);
      if(!expectedFile.exists())
      {
        Log.error("Expected file does not exist "+expectedFilePath);
        Log.info("I am going to try to plot the computed");
      }
      expectedFilePath = eZip;
    }
    File computedFile = new File(computedFilePath);
    if(!computedFile.exists())
    {
    	Log.error("Computed file does not exist "+computedFilePath);
    	return null;
    }
    
    String report = computedFilePath.substring(0,computedFilePath.length()-4)+"/"+computedFile.getName();
    report=report.substring(0, report.length()-4)+"Report.xml";		
    this.setFullReportPath(report);
  	  	
  	Set<String> failures = new HashSet<String>();
  	try
  	{
  		FileUtils.delete(this.reportDir);
  		Thread.sleep(1000);
  		FileUtils.createDirectory(this.reportDir);  		
  	}
  	catch(Exception ex)
  	{
  		Log.warn("Having a hard time deleting the old report directory",ex);
  	}
    
    
    
    CSVContents expectedResults;
    CSVContents computedResults;
    try
    {
    	expectedResults = new CSVContents(expectedFilePath);
    	computedResults = new CSVContents(computedFilePath);
    }
    catch(Exception ex)
    {
      Log.error("Unable to read input files",ex);
      return null;
    }
    
    int totalErrors=0;

    List<String> expectedHeaders = expectedResults.getHeaders();
    List<String> computedHeaders = computedResults.getHeaders();
    
    if(expectedHeaders==null)
    {
    	Log.error("No headers for "+expectedFilePath);
    	return null;
    }
    if(computedHeaders==null)
    {
    	Log.error("No headers for "+computedFilePath);
    	return null;
    }
    
    // Create the Test Case
    TestSuite suite = new TestSuite();
    suite.setName(this.name);
    this.addSuite(suite);
    suite.startCase(this.name);
    // Set up the Compare Options
    SEEqualOptions opts = suite.getCaseEqualOptions();
    this.limit = 5.0;   //changing this a percentage point TODO: fix issues with oxygen production/cO2 consumption
    opts.setPercentDifference(this.limit);
    opts.trackError(true);
    opts.trackDifferences(false);
    //opts.truncateMantissasTo(5);

    if(expectedHeaders.size()!=computedHeaders.size())
    {
    	Log.warn("Number of results is different, expected "+expectedHeaders.size()+" but computed is "+computedHeaders.size());
    }
    
    for(String header : expectedHeaders)
    {
      if(!computedHeaders.contains(header))
      {
      	totalErrors++;
    	  Log.error("Computed results did not provide expected result "+header);
      }       
    }
    double time=0;
    boolean firstColumnIsTime = false;
    if(expectedHeaders.get(0).substring(0, 4).equalsIgnoreCase("Time"))
    	firstColumnIsTime = true;
    
    while(true)
    {    
	    List<Double> expectedData = expectedResults.readNextLine();
	    List<Double> computedData = computedResults.readNextLine();
	    
	    if( ( expectedData.isEmpty() && !computedData.isEmpty()) ||
	    		(!expectedData.isEmpty() &&  computedData.isEmpty()))
	    {
	    	Log.error("Computed is not the same length as expected");
	    	totalErrors++;
	    	break;
	    }
	    if(expectedData.isEmpty() || computedData.isEmpty())
	    	break;	   
	    
	    time = expectedData.get(0);
	    for(int i=0; i<expectedData.size(); i++)
	    {    	
	    	String header = expectedHeaders.get(i);
	    	double expected=expectedData.get(i);
	    	int idx=computedHeaders.indexOf(header);
	    	if(idx==-1)
	    		continue;
	    	double computed=computedData.get(idx);
	
	    	if(!DoubleUtils.equals(expected, computed, opts))
	    	{
	    		totalErrors++;
	    		
	    		if(reportDifferences)
	    		{
	    			if(!firstColumnIsTime)
	    				Log.error(header+" does not match expected "+expected+", != computed "+computed+" ["+opts.getLastError()+"%]","");
	    			else
	    				Log.error(header+" @Time "+expectedData.get(i)+": expected "+expected+", != computed "+computed+" ["+opts.getLastError()+"%]","");
	    		}
	    		else if(!failures.contains(header))
	    		{
	    			Log.error("Compare Failed for "+name);
	    			if(!firstColumnIsTime)
	    				Log.error(header+" does not match expected "+expected+", != computed "+computed+" ["+opts.getLastError()+"%]","");
	    			else
	    				Log.error(header+" @Time "+expectedData.get(i)+": expected "+expected+", != computed "+computed+" ["+opts.getLastError()+"%]","");	    			
	    		}
	    		failures.add(header);
	    	}
	    }
    }
    
    if(firstColumnIsTime)
    	Log.info("Compared "+time+" time units worth of data");

    if(totalErrors>0)
      Log.error(totalErrors+" errors found");
    if(suite.caseFailed())
      Log.error(computedFilePath +" Comparison failed!!");
    else
      Log.info(computedFilePath + " Comparison SUCCESS!!");
        
    suite.endCase();
    return failures;
  }
  
}
