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

package mil.tatrc.physiology.utilities.csv.plots;

import java.io.File;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import mil.tatrc.physiology.utilities.DoubleUtils;
import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.StringUtils;
import mil.tatrc.physiology.utilities.csv.CSVContents;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartUtilities;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.LogarithmicAxis;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYItemRenderer;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import java.awt.BasicStroke;
import java.awt.Font;
import java.awt.Color;
import java.awt.Paint;

/**
 * @author abray
 * This is a generic results plot tool.
 * It just plots every heading based on a generic template.
 */

public class CSVPlotTool
{
  //font size for tick labels and legend
  public static Font smallFont = new Font("SansSerif", Font.PLAIN, 15);
  //font size for X,Y axis title and plot title
  public static Font largeFont = new Font("SansSerif", Font.PLAIN, 22);  
  //width of lines
  public static float lineWidth = 2.0f;
  //color of expected data
  public static Paint expectedLineColor = Color.BLACK;
  //color of computed data
  public static Paint computedLineColor = Color.RED;

  public static void main(String[] args)
  {
    if(args.length<1)
    {
      Log.fatal("Expected inputs : [results file path]");
      return;
    }
    File f = new File(args[0]);
    if(!f.exists())
    {
      Log.fatal("Input file cannot be found");
      return;
    }
    String reportDir = "./graph_results/"+f.getName();
    reportDir=reportDir.substring(0,reportDir.lastIndexOf("."))+"/";  
    CSVPlotTool t = new CSVPlotTool();
    t.graphResults(args[0],reportDir);
  }

  public boolean graphResults(String csvFile, String graphLocation)
  {
    try
    {
      File f = new File(csvFile);
      if(!f.exists())
      {
        Log.fatal("Input file cannot be found");
        return false;
      }

      File dir = new File(graphLocation);
      if(dir.exists() && dir.isDirectory())
      {
        for(String g : dir.list())
          if(g.endsWith(".jpg"))
            FileUtils.delete(g);
      }      
      Thread.sleep(250);
      FileUtils.createDirectory(graphLocation);

      CSVContents result = new CSVContents(csvFile);
      Map<String, List<Double>> data = new HashMap<String, List<Double>>();
      if(!result.readAll(data))
        return false;
      if(!data.containsKey("Time(s)"))
      {
      	Log.error("CSV file, "+csvFile+", does not conteain a Time(s) header to plot against");
      	return false;
      }
      
      for(String header : data.keySet())
      {
        if(header.equals("Time(s)"))
          continue;
        createGraph(graphLocation, Color.white, header + "_vs_Time","Time",header,createXYSeries(header,data.get("Time(s)"),data.get(header)));
      }
    }
    catch(Exception ex)
    {
      Log.error("Unable to graph results file "+csvFile);
      return false;
    }
    return true;
  }

  public void createGraph(String toDir, Paint color, String title, String XAxisLabel, String YAxisLabel, XYSeries...xyData)
  {
    new File(toDir).mkdir();
    
    Log.info("Creating Graph "+toDir+"/"+title);    
    double resMin0	  = 1.e6;
    double resMax0   = -1.e6;
    double resMin1	  = 1.e6;
    double resMax1   = -1.e6;

    XYSeriesCollection dataSet = new XYSeriesCollection();
    for(XYSeries data : xyData)
    {
    	if(data != null && !data.isEmpty())
    		dataSet.addSeries(data);
    }

    JFreeChart chart = ChartFactory.createXYLineChart(
        title ,					 // chart title
        XAxisLabel,				 // x axis label
        YAxisLabel,				 // y axis label
        dataSet,                   // data
        PlotOrientation.VERTICAL,  // orientation
        true,                      // include legend
        true,                      // tooltips
        false                      // urls
        );

    XYPlot plot = (XYPlot) chart.getPlot();
/* I have residual and error plots turned off, there are some plots that contain these names in their title, and I don't want this code running, need a better way to see if we are doing a special plot rather than title contents
    if(title.contains("Residual"))
    {
      // Make plot symmetric about x axis
      resMax0 = xyData[0].getMaxY();
      resMin0 = xyData[0].getMinY();
      if (Math.abs(xyData[0].getMinY()) > Math.abs(xyData[0].getMaxY()))  resMax0 = Math.abs(resMin0);
      if (Math.abs(xyData[0].getMaxY()) > Math.abs(xyData[0].getMinY()))  resMin0 = -1.0*Math.abs(resMax0);
      if((resMin0==0.0) && (resMax0==0.0))
      {
        resMin0 = -0.00001;
        resMax0 =  0.00001;
      }    	
      ValueAxis yAxis = plot.getRangeAxis();
      yAxis.setRange(resMin0 + 0.05*resMin0, resMax0 + 0.05*resMax0);//5% buffer so we can see top and bottom clearly
    }
    else if(title.contains("Error"))
    {
      // Make plot symmetric about x axis
      resMax0 = xyData[0].getMaxY();
      resMin0 = xyData[0].getMinY();
      if((resMin0==0.0) && (resMax0==0.0))
      {
        resMin0 = -0.00001;
        resMax0 =  0.00001;
      }
      if(resMin0>=0.0) resMin0 = -0.01;
      ValueAxis yAxis = plot.getRangeAxis();
      yAxis.setRange(resMin0 + 0.05*resMin0, resMax0 + 0.05*resMax0);//5% buffer so we can see top and bottom clearly

      /*
    	yAxis.setTickLabelPaint(new Color(1,0,0));
    	yAxis.setTickMarkPaint(new Color(1,0,0));
    	yAxis.setAxisLinePaint(new Color(1,0,0));
    	yAxis.setLabelPaint(new Color(1,0,0));

    	ValueAxis xAxis = plot.getDomainAxis();
    	xAxis.setTickLabelPaint(new Color(1,0,0));
    	xAxis.setTickMarkPaint(new Color(1,0,0));
    	yAxis.setAxisLinePaint(new Color(1,0,0));
    	yAxis.setLabelPaint(new Color(1,0,0));
       *
    }
    else
*/      
    {    	
      if(title.indexOf("Hemoglobin-GlomerularFilterability")>-1)
        System.out.println("stop");
      if(xyData.length > 1)
      {        
        // Make plot symmetric about x axis
        resMax0 = xyData[0].getMaxY();
        resMin0 = xyData[0].getMinY();
        resMax1 = xyData[1].getMaxY();
        resMin1 = xyData[1].getMinY();
        if(resMin1<resMin0) resMin0=resMin1;
        if(resMax1>resMax0) resMax0=resMax1;
        if(DoubleUtils.isZero(resMin0))
          resMin0 = -0.001;
        if(DoubleUtils.isZero(resMax0))
          resMax0 =  0.001;
        if(resMin0>=0.0) resMin0 = -0.01;
        if(YAxisLabel.indexOf("PlasmaConcentration")>-1)
          plot.setRangeAxis(new LogarithmicAxis("Log("+YAxisLabel+")")); 
        else
        {
          ValueAxis yAxis = plot.getRangeAxis();
          yAxis.setRange(resMin0 + 0.05*resMin0, resMax0 + 0.15*Math.abs(resMax0));//5% buffer so we can see top and bottom clearly
        }
        String NaNCheck = "";
        if(Double.isNaN(resMax0)||Double.isNaN(resMin0))
          NaNCheck += "Expected is NaN ";
        if(Double.isNaN(resMax1)||Double.isNaN(resMin1))
          NaNCheck += "Computed is NaN ";
        if(!NaNCheck.isEmpty())
          plot.getDomainAxis().setLabel(NaNCheck);
      }
      else
      {
        // Make plot symmetric about x axis
        resMax0 = xyData[0].getMaxY();
        resMin0 = xyData[0].getMinY();
        if(Double.isNaN(resMax0)||Double.isNaN(resMin0))
        	plot.getDomainAxis().setLabel("Computed is NaN");
        if(DoubleUtils.isZero(resMin0))
          resMin0 = -0.001;
        if(DoubleUtils.isZero(resMax0))
          resMax0 =  0.001;
        if(resMin0>=0.0) resMin0 = -0.01;
        if(YAxisLabel.indexOf("PlasmaConcentration")>-1)
          plot.setRangeAxis(new LogarithmicAxis("Log("+YAxisLabel+")"));
        else
        {
          ValueAxis yAxis = plot.getRangeAxis();
          yAxis.setRange(resMin0 + 0.05*resMin0, resMax0 + 0.15*Math.abs(resMax0));//5% buffer so we can see top and bottom clearly
        }        	
      }
    }

    formatXYPlot(chart,color);
    //Changing line widths and colors
    XYItemRenderer r = plot.getRenderer(); 
    BasicStroke wideLine = new BasicStroke( lineWidth ); 
    r.setSeriesStroke(0, wideLine); 
    r.setSeriesStroke(1, wideLine); 
    XYLineAndShapeRenderer renderer = (XYLineAndShapeRenderer) plot.getRenderer();
    if(xyData.length>1)
    {
      renderer.setSeriesStroke(//makes a dashed line
          0, //argument below float[]{I,K} -> alternates between solid and opaque (solid for I, opaque for K)
          new BasicStroke(lineWidth, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND, 1.0f, new float[]{15.0f, 30.0f}, 0.0f)
          );
      renderer.setDrawSeriesLineAsPath(true);
      renderer.setUseFillPaint(true);
    }  
    renderer.setBaseShapesVisible(false);
    renderer.setSeriesFillPaint(0, expectedLineColor); 
    renderer.setSeriesFillPaint(1, computedLineColor ); 
    renderer.setSeriesPaint(0, expectedLineColor); 
    renderer.setSeriesPaint(1, computedLineColor); 

    try
    {
      if(toDir==null||toDir.isEmpty())
        toDir=".";
      File JPGFile = new File(toDir+"/"+MakeFileName(title)+".jpg");
      ChartUtilities.saveChartAsJPEG(JPGFile, chart, 1600, 800);
    }
    catch (IOException e)
    {
      Log.error(e.getMessage());
    }
  }
  
  public String MakeFileName(String title)
  {
  	String name = StringUtils.removeParens(title);
  	name=name.replaceAll(" ", "_");
  	name=name.replaceAll("[/\\\\]", "_Per_");
  	return name;
  }

  public XYSeries createXYSeries(String name, List<Double> x, List<Double>y)
  {
  	if(y==null || x==null)
  	{
  		Log.error("null data provided");
  		return null;
  	}
    if(y.size()!=x.size())
    {
      Log.error("axis data is not the same size");
    }    
    XYSeries series = new XYSeries(name);
    for(int i=0; i<x.size();i++) 
    {
    	if(i>=y.size())
    		series.add(x.get(i),(Double)Double.NaN);
    	else
    		series.add(x.get(i),y.get(i));
    }
    return series;
  }

  public void formatXYPlot(JFreeChart chart, Paint bgColor)
  {
    XYPlot plot = (XYPlot) chart.getPlot();
    
    //For Scientific notation
    NumberFormat formatter = new DecimalFormat("0.######E0");
    
    for(int i = 0; i < plot.getDomainAxisCount(); i++)
    {
    	plot.getDomainAxis(i).setLabelFont(largeFont);
    	plot.getDomainAxis(i).setTickLabelFont(smallFont);
    	plot.getDomainAxis(i).setLabelPaint(bgColor==Color.red?Color.white:Color.black);
    	plot.getDomainAxis(i).setTickLabelPaint(bgColor==Color.red?Color.white:Color.black);
    }
    for(int i = 0; i < plot.getRangeAxisCount(); i++)
    {
    	plot.getRangeAxis(i).setLabelFont(largeFont);
    	plot.getRangeAxis(i).setTickLabelFont(smallFont);
    	plot.getRangeAxis(i).setLabelPaint(bgColor==Color.red?Color.white:Color.black);
    	plot.getRangeAxis(i).setTickLabelPaint(bgColor==Color.red?Color.white:Color.black);
    	NumberAxis rangeAxis = (NumberAxis)plot.getRangeAxis(i);
      rangeAxis.setNumberFormatOverride(formatter);
    }

    //White background outside of plottable area
    chart.setBackgroundPaint(bgColor);

    plot.setBackgroundPaint(Color.white);
    plot.setDomainGridlinePaint(Color.black);
    plot.setRangeGridlinePaint(Color.black);

    plot.setDomainCrosshairVisible(true);
    plot.setRangeCrosshairVisible(true);

    chart.getLegend().setItemFont(smallFont);
    chart.getTitle().setFont(largeFont);
    chart.getTitle().setPaint(bgColor==Color.red?Color.white:Color.black);
  }

}
