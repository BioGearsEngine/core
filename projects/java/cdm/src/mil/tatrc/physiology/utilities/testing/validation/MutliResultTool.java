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
package mil.tatrc.physiology.utilities.testing.validation;

import java.awt.Color;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.util.*;

import org.jfree.data.xy.XYSeries;

import mil.tatrc.physiology.utilities.DoubleUtils;
import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Resources;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.OrderedProperties;
import mil.tatrc.physiology.utilities.WaveformUtils;
import mil.tatrc.physiology.utilities.csv.CSVContents;
import mil.tatrc.physiology.utilities.csv.plots.CSVPlotTool;

/**
 * I built this class to handle multiple results files
 * such as files full of circuit node pressures from the CV circuit
 * where each result file was run with a different frequency.
 * So this would take the value or average or max or some other statistical variation
 * of all the results files and create a plot for each property across all files
 * i.e. each point in a plot would be the pressure from the same node and the x axis is the frequency
 * @author abray
 *
 */
public class MutliResultTool
{
  protected static boolean wavformMeans=false;
  private   static Resources m_resources = new Resources();
  protected static class ResultSet
  {
    public String resultDir;
    public String baseline;
    public Map<String,String> divisors = new HashMap<String,String>();
    public List<String> sort = new ArrayList<String>();
  }
  protected static class ResultSummary
  {
    public String name;
    public double xValue;
    public double mean;
    public double meanError;
    public double max;
    public double maxError;
    public double min;
    public double minError;
    public double maxMean;
    public double maxMeanError;
    public double minMean;
    public double minMeanError;
  }
  public static Map<String,List<ResultSummary>> divideResultSummary(Map<String,List<ResultSummary>> numerator, Map<String,List<ResultSummary>> denominator)
  {
    if(numerator.size() != denominator.size())
    {
      Log.error("Result summaries do not contain the same number of results");
      return null;
    }
    
    Map<String,List<ResultSummary>> quotient = new HashMap<String,List<ResultSummary>>();
    
    for(String key : numerator.keySet())
    {
      //if(key.equals("LeftHeart1ToAorta2_Flow"))
      //  Log.info("Here");
      List<ResultSummary> nList = numerator.get(key);
      List<ResultSummary> dList = denominator.get(key);
      if(dList==null)
      {
        Log.error("Result summaries do not contain the same keyset");
        return null;
      }
      if(dList.size()!=1)
      {
        Log.error("Denominator List should only have 1 Result Summary");
        return null;
      }
      ResultSummary d = dList.get(0);
      
      List<ResultSummary> qList = new ArrayList<ResultSummary>();
      quotient.put(key, qList);
      for(int i=0; i<nList.size(); i++)
      {
        ResultSummary n = nList.get(i);

        ResultSummary q = new ResultSummary();
        qList.add(q);
        q.name = n.name;
        q.xValue = n.xValue;
        q.mean = n.mean/d.mean;
        q.meanError = Double.NaN;
        q.max = n.max/d.max;
        q.maxError = Double.NaN;
        q.min = n.min/d.min;
        q.minError = Double.NaN;
        q.maxMean = n.maxMean/d.maxMean;
        q.maxMeanError = Double.NaN;
        q.minMean = n.minMean/d.minMean;
        q.minMeanError = Double.NaN;        
      }
    }    
    return quotient;
  }
  public static Map<String,List<ResultSummary>> divideResultSummary(Map<String,List<ResultSummary>> numerator, List<ResultSummary> dList, String keyFilter)
  {
    Map<String,List<ResultSummary>> quotient = new HashMap<String,List<ResultSummary>>();
    for(String key : numerator.keySet())
    {
      if(key.indexOf(keyFilter)==-1)
        continue;
   
      if(key.indexOf("VenaCavaToRightHeart2")>-1)
        Log.info("Here");
      List<ResultSummary> nList = numerator.get(key);
      if(nList.size()!=dList.size())
      {
        Log.error("Number of results does not match");
        return null;
      }
      List<ResultSummary> qList = new ArrayList<ResultSummary>();
      quotient.put(key, qList);
      for(int i=0; i<nList.size(); i++)
      {
        ResultSummary n = nList.get(i);
        ResultSummary d = dList.get(i);
        if(n.xValue!=d.xValue || !n.name.equals(d.name))
        {
          Log.error("Result Summaries are out of sync");
          return null;
        }
        ResultSummary q = new ResultSummary();
        qList.add(q);
        q.name = n.name;
        q.xValue = n.xValue;
        q.mean = n.mean/d.mean;
        q.meanError = Double.NaN;
        q.max = n.max/d.max;
        q.maxError = Double.NaN;
        q.min = n.min/d.min;
        q.minError = Double.NaN;
        q.maxMean = n.maxMean/d.maxMean;
        q.maxMeanError = Double.NaN;
        q.minMean = n.minMean/d.minMean;
        q.minMeanError = Double.NaN;       
      }
    }
    return quotient;
  }
  
  protected enum CellContent { Value, Error, Both };
  
  protected class ResultSummaryComparator implements Comparator<ResultSummary>
  {
    public int compare(ResultSummary r1, ResultSummary r2) 
    {
      if (r1.xValue < r2.xValue) 
        return -1;
      if (r1.xValue > r2.xValue) 
        return 1;
      return 0;
    }
  }
  
  public static void main(String[] args)
  {
    MutliResultTool me = new MutliResultTool();    
    try
    {      
      ResultSet rs = new ResultSet();
      String str = FileUtils.readFile(m_resources.findResource("Characterize.config"));
      InputStream is = new ByteArrayInputStream(str.getBytes());
      OrderedProperties props = new OrderedProperties();
      props.load(is);
      Set<String> keySet = props.keySet();
      for(String key : keySet)
      {        
        String[] p = props.get(key).split(",");
        if(p.length>0)
          rs.resultDir = p[0].trim();
        else
        {
          Log.error("Properties file needs at least a directory of a result to analyze.");
          return;
        }
        if(p.length>1)
          rs.baseline = p[1].substring(p[1].indexOf("=")+1).trim();
        for(int i=2; i<p.length; i++)
        {
          if(p[i].indexOf("Divisor")>-1)
          {
            String[] divisor = p[i].split("=");
            rs.divisors.put(divisor[0].substring(0, divisor[0].indexOf("Divisor")).trim(),divisor[1].trim());
          }
          if(p[i].indexOf("Sort")>-1)
          {
            String[] sortby = p[i].split("=");
            rs.sort.add(sortby[1].trim());
          }
        }            
        me.summarize(rs);
      }
    }
    catch(Exception ex)
    {
      Log.warn("Could not find a Caracterize.config file, plotting everything I can find");
      File vdir = new File("./UnitTests/CardiovascularCharacterization/");
      File[] cDirs = vdir.listFiles();    
      ResultSet rs = new ResultSet();
      for(File cDir : cDirs)
      {
        if(cDir.isDirectory())
        {          
          rs.resultDir = cDir.getAbsolutePath()+"/";
          me.summarize(rs);
        }
      }
    }    
  }
    
  public void summarize(ResultSet rs)
  {
    new File(rs.resultDir+"Summary").mkdir();
    boolean tableOnly = true;// Just create a markdown table, not plots or summary txt files (Quick running)
    try
    {
      Map<String,List<ResultSummary>> baselineSummaries = null;
      if(rs.baseline!=null)
      {
        baselineSummaries = new HashMap<String,List<ResultSummary>>();
        processResultsFile(rs.resultDir,rs.baseline+"Results.txt",baselineSummaries,null,null);
      }
      
      CSVPlotTool plotter = new CSVPlotTool();
      Map<String,List<ResultSummary>> resultSummaries = new HashMap<String,List<ResultSummary>>();
            
      String resultUnit  = null;
      File vdir = new File(rs.resultDir);
      String[] cFiles = vdir.list();         
      for(String cFile : cFiles)
      {
        if(!cFile.endsWith("Results.txt"))
          continue;
        String xUnit = processResultsFile(rs.resultDir, cFile, resultSummaries, baselineSummaries, tableOnly?null:plotter);
        if(resultUnit == null)
          resultUnit = xUnit;
        if(!xUnit.equals(resultUnit))
          Log.error("The results file "+rs.resultDir+cFile+" has units of "+xUnit+", but I expected units of "+resultUnit);
      }
      List<String> groundPaths = new ArrayList<String>();
      for(String key : resultSummaries.keySet())
      {
        List<ResultSummary> l = resultSummaries.get(key);
        Collections.sort(l,new ResultSummaryComparator());
        if(key.indexOf("Ground")>-1)
          groundPaths.add(key);
      }
      // Remove any Ground Paths from the summary
      for(String groundPath : groundPaths)
      {
        System.out.println("Removing "+groundPath+" from Summary");
        resultSummaries.remove(groundPath);
        baselineSummaries.remove(groundPath);
      }
      if(!tableOnly)      
        plotSummaries(rs.resultDir+"Summary/",resultSummaries, resultUnit);
      WriteCSV(rs.resultDir+"Summary/",resultUnit+"-Values", resultUnit, rs.sort, resultSummaries,CellContent.Value);
      WriteCSV(rs.resultDir+"Summary/",resultUnit+"-Errors", resultUnit, rs.sort, resultSummaries,CellContent.Error);
      WriteCSV(rs.resultDir+"Summary/",resultUnit+"-Values(Error)", resultUnit, rs.sort, resultSummaries,CellContent.Both);
      
      if(rs.baseline!=null)
      {
        Map<String,List<ResultSummary>> normalized = divideResultSummary(resultSummaries,baselineSummaries);
        WriteCSV(rs.resultDir+"Summary/",resultUnit+"-Normalized", resultUnit, rs.sort, normalized,CellContent.Value);
      }
      List<String> sortBy = new ArrayList<String>();
      for(String divisor : rs.divisors.keySet())
      {
        sortBy.clear();
        sortBy.add(divisor);
        String prop = rs.divisors.get(divisor);
        List<ResultSummary> dList=null;
        for(String key : resultSummaries.keySet())
        {
          if(key.indexOf(prop)>-1)
          {
            dList = resultSummaries.get(key);
            break;
          }
        }
        if(dList==null)
        {
          Log.error("Unable to find "+divisor+"Divisor="+prop);
        }
        else
        {
          Map<String,List<ResultSummary>> rsMap = divideResultSummary(resultSummaries,dList,divisor);
          WriteCSV(rs.resultDir+"Summary/",resultUnit+"-"+prop+"Divisor", resultUnit, sortBy, rsMap, CellContent.Value);
        }
      }
      
    }
    catch(Exception ex)
    {
      Log.error(ex);
    }
  }
  
  protected String processResultsFile(String resultsDir, String resultsFile, Map<String,List<ResultSummary>> resultSummaries, Map<String,List<ResultSummary>> baselineSummaries, CSVPlotTool plotter) throws IOException
  {
    String xValue = null;
    String xUnit  = null;
    
    if(resultsFile.endsWith("Results.txt"))
    {            
      Log.info("Reading file "+resultsFile);
      CSVContents sceResults = new CSVContents(resultsDir+resultsFile);
      // I assume the x value is encoded at the start of the file name
      String xSub = resultsFile.substring(0, resultsFile.indexOf("Results"));
      // Now pull out the double, the rest will be the 'unit'
      xValue = null;
      for(int i=0; i<xSub.length(); i++)
      {
        char c = xSub.charAt(i);
        if(Character.isAlphabetic(c))
        {
          xValue = xSub.substring(0, i);
          if(xUnit==null)
            xUnit  = xSub.substring(i);
          else
            if(!xUnit.equals(xSub.substring(i)))
            {
              Log.error("There is a mix of results files in this directory, I am not programmed to handle that currently." + xSub.substring(i) + " vs. " + xUnit);
              xUnit = null;                  
            }
          break;
        }
      }
      if(xValue == null || xUnit == null)
      {
        Log.error("Ignoring file does not follow expected naming convention (No xValue) " + resultsFile);
        return xUnit;
      }
      Map<String,List<Double>> resultData= new HashMap<String,List<Double>>();
      sceResults.readAll(resultData);     
      if(plotter!=null)
      {
        // Let's Plot everything
        for(String header : resultData.keySet())
        {
          if(header.equals("Time(s)"))
            continue;
          plotter.createGraph(resultsDir+xValue+xUnit+"/", Color.white, header + "_vs_Time","Time",header,plotter.createXYSeries(header,resultData.get("Time(s)"),resultData.get(header)));
        }
      }
      // Now let's add our data to the summary
      for(String key : resultData.keySet())
      {
        //if(key.indexOf("CardiacOutput")>-1)
        //  System.out.println("here");
        List<Double> result = resultData.get(key);
        List<ResultSummary> rsList = resultSummaries.get(key);
        if(rsList==null)
        {
          rsList = new ArrayList<ResultSummary>();
          resultSummaries.put(key, rsList);
        }
        
        ResultSummary rs = new ResultSummary();
        rs.name    = resultsFile;
        rs.xValue  = Double.parseDouble(xValue);
        rs.mean    = DoubleUtils.getAverage(result);
        rs.max     = DoubleUtils.getMax(result);
        rs.min     = DoubleUtils.getMin(result);
        rs.maxMean = WaveformUtils.getMax(result);
        rs.minMean = WaveformUtils.getMin(result);        
        if(baselineSummaries!=null)
        {
          ResultSummary rsBaseline = baselineSummaries.get(key).get(0);
          rs.meanError    = DoubleUtils.percentTolerance(rsBaseline.mean, rs.mean, 1e-10);
          rs.maxError     = DoubleUtils.percentTolerance(rsBaseline.max, rs.max, 1e-10);
          rs.minError     = DoubleUtils.percentTolerance(rsBaseline.min, rs.min, 1e-10);
          rs.maxMeanError = DoubleUtils.percentTolerance(rsBaseline.maxMean, rs.maxMean, 1e-10);
          rs.minMeanError = DoubleUtils.percentTolerance(rsBaseline.minMean, rs.minMean, 1e-10);
        }
        rsList.add(rs);
      }
    }
    return xUnit;
  }
  
  public void plotSummaries(String toDir, Map<String,List<ResultSummary>> resultSummaries, String xAxisLabel)
  {
    new File(toDir).mkdir();
    
    CSVPlotTool plotter = new CSVPlotTool();
    for(String key : resultSummaries.keySet())
    {
      XYSeries mean         = new XYSeries(key+"Mean");
      XYSeries meanError    = new XYSeries(key+"MeanError");
      XYSeries max          = new XYSeries(key+"Max");
      XYSeries maxError     = new XYSeries(key+"MaxError");
      XYSeries min          = new XYSeries(key+"Min");
      XYSeries minError     = new XYSeries(key+"MinError");

      XYSeries maxMean      = new XYSeries(key+"MaxMean");
      XYSeries maxMeanError = new XYSeries(key+"MaxMeanError");
      XYSeries minMean      = new XYSeries(key+"MinMean");
      XYSeries minMeanError = new XYSeries(key+"MinMeanError");

      try
      {
        //if(key.indexOf("CardiacOutput")>-1)
        //  System.out.println("here");
        String key_no_units = key;
        if(key.indexOf("(")>-1)
          key_no_units = key.substring(0, key_no_units.indexOf("("));
        PrintWriter writer = new PrintWriter(toDir+"/"+key_no_units+"vs"+xAxisLabel+"Summary.txt", "UTF-8");
        writer.println(xAxisLabel+"\t"+key+"_Mean\t"+key+"_MeanError\t"+key+"_Max\t"+key+"_MaxError\t"+key+"_Min\t"+key+"_MinError\t"+key+"_MaxMean\t"+key+"_MaxMeanError\t"+key+"_MinMean"+key+"_MinMeanError\t");
        for(ResultSummary rs : resultSummaries.get(key))
        {
          mean.add(rs.xValue,rs.mean);
          meanError.add(rs.xValue,rs.meanError);
          max.add(rs.xValue,rs.max);
          maxError.add(rs.xValue,rs.maxError);
          min.add(rs.xValue,rs.min);
          minError.add(rs.xValue,rs.minError);
          if(wavformMeans)
          {
            maxMean.add(rs.xValue,rs.maxMean);
            maxMeanError.add(rs.xValue,rs.maxMeanError);
            minMean.add(rs.xValue,rs.minMean);
            minMeanError.add(rs.xValue,rs.minMeanError);
          }

          writer.println(rs.xValue+"\t"+rs.mean+"\t"+rs.meanError+"\t"+rs.max+"\t"+rs.maxError+"\t"+rs.min+"\t"+rs.minError+"\t"+rs.maxMean+"\t"+rs.maxMeanError+"\t"+rs.minMean+"\t"+rs.minMeanError);
        }
        writer.close();
      }
      catch(Exception ex)
      {
        Log.error("Problem writing summary txt file",ex);
      }
        plotter.createGraph(toDir+"plots", Color.white, key+"Mean", xAxisLabel, key, mean);
        plotter.createGraph(toDir+"plots", Color.white, key+"MeanError", xAxisLabel, key, meanError);
        plotter.createGraph(toDir+"plots", Color.white, key+"Max", xAxisLabel, key, max);
        plotter.createGraph(toDir+"plots", Color.white, key+"MaxError", xAxisLabel, key, maxError);
        plotter.createGraph(toDir+"plots", Color.white, key+"Min", xAxisLabel, key, min);
        plotter.createGraph(toDir+"plots", Color.white, key+"MinError", xAxisLabel, key, minError);
        if(wavformMeans)
        {
          plotter.createGraph(toDir+"plots", Color.white, key+"MaxMean", xAxisLabel, key, maxMean);
          plotter.createGraph(toDir+"plots", Color.white, key+"MaxMeanError", xAxisLabel, key, maxMeanError);
          plotter.createGraph(toDir+"plots", Color.white, key+"MinMean", xAxisLabel, key, minMean);      
          plotter.createGraph(toDir+"plots", Color.white, key+"MinMeanError", xAxisLabel, key, minMeanError);
        }
    }    
  }
  
  protected void WriteCSV(String resultsDir, String resultsName, String xAxisLabel, List<String> sortBy, Map<String,List<ResultSummary>> resultSummaries, CellContent c)
  {
    if(resultSummaries.isEmpty())
      return;
    
    PrintWriter writer=null;
    // Grab a result summary
    List<ResultSummary> rsList = resultSummaries.values().iterator().next();
    
    try
    {
      rsList = resultSummaries.values().iterator().next();
      if(sortBy==null)
        sortBy = new ArrayList<String>();
      if(sortBy.isEmpty())
        sortBy.add("All");
      for(String sort : sortBy)
      {
        Map<String,List<ResultSummary>> sorted;
        if(sort.equals("All"))
          sorted = resultSummaries;
        else
        {
          sorted= new HashMap<String,List<ResultSummary>>();

          for(String key : resultSummaries.keySet())
          {          
            if(key.indexOf(sort)==-1)
              continue;
            sorted.put(key,resultSummaries.get(key));
          }
        }
        Map<String,PrintWriter> writers = new HashMap<String,PrintWriter>();
        writers.put("Mean",new PrintWriter(resultsDir+"/"+resultsName+"-"+sort+"-Mean.csv", "UTF-8"));
        writers.put("Max",new PrintWriter(resultsDir+"/"+resultsName+"-"+sort+"-Max.csv", "UTF-8"));
        writers.put("Min",new PrintWriter(resultsDir+"/"+resultsName+"-"+sort+"-Min.csv", "UTF-8"));
        if(wavformMeans)
        {
          writers.put("MaxMean",new PrintWriter(resultsDir+"/"+resultsName+"-"+sort+"-MaxMean.csv", "UTF-8"));
          writers.put("MinMean",new PrintWriter(resultsDir+"/"+resultsName+"-"+sort+"-MinMean.csv", "UTF-8"));
        }

        double v=Double.NaN,err=Double.NaN;
        for(String type : writers.keySet())
        {
          int i=-1;          
          writer = writers.get(type);
          writer.print(xAxisLabel+",");
          for(String key : sorted.keySet())
          {
            i++;
            writer.print(key + (i<sorted.size()-1?",":"\n"));
          }

          for(int r=0; r<rsList.size(); r++)
          {
            i=0;
            writer.print(rsList.get(r).xValue+",");
            for(String key : sorted.keySet())
            {
              ResultSummary rs = sorted.get(key).get(r);

              if(type.equals("Mean"))
              {
                v = rs.mean;
                err = rs.meanError;
              }
              else if(type.equals("Min"))
              {
                v = rs.min;
                err = rs.minError;
              }
              else if(type.equals("Max"))
              {
                v = rs.max;
                err = rs.maxError;
              }
              else if(type.equals("MinMean"))
              {
                v = rs.minMean;
                err = rs.minMeanError;
              }
              else if(type.equals("MaxMean"))
              {
                v = rs.maxMean;
                err = rs.maxMeanError;
              }
              else
              {
                v=Double.NaN;
                err=Double.NaN;
                Log.warn("Writer not supported by ResultSummary");
              }
              writer.print(getCellString(v,err,c) + (i++<sorted.size()-1?",":"\n"));
            }
          }
          writer.close();
        }
      }
    }
    catch(Exception ex)
    {
      Log.error("Error writing validation table for "+resultsName,ex);
      writer.close();
    }
  }
  
  protected String truncate(double d)
  {
    return String.format("%."+3+"g", d);
  }
  
  protected String getCellString(double v, double err, CellContent c)
  {
    switch(c)
    {
      case Value:
        return truncate(v);
      case Error:
        return truncate(err)+"%";
      case Both:
        return truncate(v)+" ("+truncate(err)+"%)";
    }
    return "";
  }

  protected String pad(String s, int max)
  {
    if(s==null)
      return new String(new char[(max+1)]).replace('\0',' ');
    // Using a pad of 1 between columns
    try
    {
      String ret =  s + new String(new char[(max-s.length())+1]).replace('\0',' ');
      return ret;
    }
    catch(Exception ex)
    {
      Log.error("Could not pad "+s+" with a max of "+max,ex);
      return "";
    }
  }
  
}
