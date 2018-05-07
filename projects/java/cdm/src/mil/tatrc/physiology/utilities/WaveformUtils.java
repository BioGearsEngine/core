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

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Paint;
import java.awt.Shape;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;



import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartUtilities;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
import org.jfree.util.ShapeUtilities;

import mil.tatrc.physiology.utilities.DoubleUtils;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.StringUtils;
import mil.tatrc.physiology.utilities.csv.CSVContents;
import mil.tatrc.physiology.utilities.csv.plots.CSVPlotTool;

public class WaveformUtils
{
  public static void main(String[] args)
  {
    String resultsFile;
    String resultsName;

    if(args.length==0)
    {
      resultsFile = "./Scenarios/Basic/Basic1Results.txt";
      resultsName = "TidalVolume(mL)";
      resultsName = "TotalLungVolume(mL)";
    }
    else if(args.length!=2)
    {
      Log.info("Inputs are : <ResultsFile> <ResultName to do waveform analysis>");
      return; 
    }
    else
    {
      resultsFile = args[0];
      resultsName = args[1];
    }

    try
    {
      CSVContents results = new CSVContents(resultsFile);
      Map<String,List<Double>> resultData=new HashMap<String,List<Double>>();
      results.readAll(resultData);
      if(resultsName==null)// Grab whatever from the results
        resultsName = resultData.keySet().iterator().next();        
      WaveformUtils.createGraph(".", Color.white, resultsName, "Time(s", resultsName, resultData.get("Time(s)"), resultData.get(resultsName));
    }
    catch(Exception ex)
    {
      Log.error("Unable to read Results file "+resultsName);
    }
  }
  
  public static double getMin(List<Double> data)
  {    
    return DoubleUtils.getAverage(getMinimums(data));
  }
  public static List<Double> getMinimums(List<Double> data)
  {
    List<Double> xMin = new ArrayList<Double>();
    List<Double> yMin = new ArrayList<Double>();
    List<Double> xMax = new ArrayList<Double>();
    List<Double> yMax = new ArrayList<Double>();    
    getPeriodBounds(data,data,xMin,yMin,xMax,yMax);
    return xMin;
  }
  
  public static double getMax(List<Double> data)
  {    
    return DoubleUtils.getAverage(getMaximums(data));
  }
  public static List<Double> getMaximums(List<Double> data)
  {
    List<Double> xMin = new ArrayList<Double>();
    List<Double> yMin = new ArrayList<Double>();
    List<Double> xMax = new ArrayList<Double>();
    List<Double> yMax = new ArrayList<Double>();    
    getPeriodBounds(data,data,xMin,yMin,xMax,yMax);
    return xMax;
  }
  
  public static void getPeriodBounds(List<Double> xData, List<Double> yData, List<Double> xMin, List<Double> yMin, List<Double> xMax, List<Double> yMax)
  {
    if(xData.size()!=yData.size())
      throw new RuntimeException("Waveform x and y data not the same size");
    
    xMin.clear();
    yMin.clear();
    xMax.clear();
    yMax.clear();
      
    // Ride the y value, and save off the min/max when we hit one, and the x value at those points
    double p1,p2;
    Boolean looking4max=null;
    for(int i=1; i<yData.size(); i++)
    {
      p1=yData.get(i-1);
      p2=yData.get(i);
      if(p1==p2)
        continue;
      if(p2>p1)
      {
        looking4max=true;
        break;
      }
      if(p1>p2)
      {
        looking4max=false;
        break;
      }
    }
    if(looking4max!=null)
    {
      for(int i=1; i<yData.size(); i++)
      {
        p1=yData.get(i-1);
        p2=yData.get(i);
        if(p1==p2)
          continue;
        if(looking4max && p2<p1)
        {
          looking4max=false;
          xMax.add(xData.get(i-1));
          yMax.add(p1);
        }
        if(!looking4max && p2>p1)
        {
          looking4max=true;
          xMin.add(xData.get(i-1));
          yMin.add(p1);
        }
      }
    }
    else// All the values are the same!
    {
      yMin.add(yData.get(0));
      yMax.add(yData.get(0));
      xMin.add(xData.get(0));
      xMax.add(xData.get(0));
    }
  }


  public static void createGraph(String toDir, Paint bgColor, String title, String XAxisLabel, String YAxisLabel, List<Double> xData ,List<Double> yData)
  {
    JFreeChart chart;
    CSVPlotTool plotter = new CSVPlotTool();
    try
    {
      XYSeriesCollection dataSet = new XYSeriesCollection();    
      chart = ChartFactory.createXYLineChart(
          title ,                     // chart title
          XAxisLabel,                 // x axis label
          YAxisLabel,                 // y axis label
          dataSet,                   // data
          PlotOrientation.VERTICAL,  // orientation
          true,                      // include legend
          true,                      // tooltips
          false                      // urls
          );
      XYPlot plot = (XYPlot) chart.getPlot();
      XYLineAndShapeRenderer r = new XYLineAndShapeRenderer();
      plot.setRenderer(r);
      plotter.formatXYPlot(chart, Color.white);
           
      // Set up the line to plot
      BasicStroke wideLine = new BasicStroke( CSVPlotTool.lineWidth ); 
      r.setSeriesStroke(0, wideLine);       
      r.setSeriesLinesVisible(0, true);
      r.setSeriesShapesVisible(0, false);
      dataSet.addSeries(plotter.createXYSeries(title,xData,yData));
      
      // Pull out our max and mins
      List<Double> xMin = new ArrayList<Double>();
      List<Double> yMin = new ArrayList<Double>();
      List<Double> xMax = new ArrayList<Double>();
      List<Double> yMax = new ArrayList<Double>();
      
      WaveformUtils.getPeriodBounds(xData, yData, xMin, yMin, xMax, yMax);
      
      // Let's put some shapes over the max's and min's
      XYSeries seriesMaxes = new XYSeries("Max");
      r.setSeriesLinesVisible(1, false);
      r.setSeriesShapesVisible(1, true);  
      r.setSeriesPaint(1, Color.green);
      Shape maxShape = ShapeUtilities.createUpTriangle(10);
      r.setSeriesShape(1, maxShape);
      for(int i=0; i<xMax.size(); i++)
        seriesMaxes.add(xMax.get(i),yMax.get(i));
      dataSet.addSeries(seriesMaxes);
      
      XYSeries seriesMins = new XYSeries("Min");
      r.setSeriesLinesVisible(2, false);
      r.setSeriesShapesVisible(2, true);  
      r.setSeriesPaint(2, Color.red);
      Shape minShape = ShapeUtilities.createDownTriangle(10);
      r.setSeriesShape(2, minShape);
      for(int i=0; i<xMin.size(); i++)
        seriesMins.add(xMin.get(i),yMin.get(i));
      dataSet.addSeries(seriesMins);
    }
    catch(Exception ex)
    {
      Log.error("Unable to make base chart");
      return;
    }

    // Now let's mark all the max and min values

    try
    {
      if(toDir==null||toDir.isEmpty())
        toDir=".";
      File JPGFile = new File(toDir+"/"+StringUtils.removeParens(title)+".jpg");
      ChartUtilities.saveChartAsJPEG(JPGFile, chart, 3200, 1600);
    }
    catch (IOException e)
    {
      Log.error(e.getMessage());
    }
  }


}
