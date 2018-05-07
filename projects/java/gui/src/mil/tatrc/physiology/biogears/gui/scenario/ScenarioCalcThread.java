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

package mil.tatrc.physiology.biogears.gui.scenario;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import mil.tatrc.physiology.biogears.engine.BioGearsScenarioExec;
import mil.tatrc.physiology.biogears.gui.controls.ConsoleListener;
import mil.tatrc.physiology.datamodel.scenario.SEScenario;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.csv.plots.CSVPlotTool;

public class ScenarioCalcThread extends Thread
{
	protected SEScenario scenario;
  protected BioGearsScenarioExec exec;
	protected ScenarioCalcCallback callback;
  protected ConsoleListener listener;
  
	protected boolean engineRunning;
	protected boolean createGraphs=true;
	protected ActionListener pnt;

	public ScenarioCalcThread(BioGearsScenarioExec executor, SEScenario scenario, ConsoleListener listener, ScenarioCalcCallback callback, ActionListener pnt)
	{	  
		this.listener=listener;
		this.scenario = scenario;
		this.callback = callback;
		this.pnt = pnt;
		this.exec = executor;
	}

	public void run() 
	{
		String resultsFile = scenario.getDataRequests().getFilename().replaceAll("[\\\\]", "/");
		
    File resultsDir = new File(resultsFile.substring(0,resultsFile.lastIndexOf("/"))+"/"+scenario.getName()+"Graphs");
    resultsDir.mkdirs();

    CSVPlotTool plotter=new CSVPlotTool();
		Log.addAppender(this.listener);
		this.exec.setListener(this.listener);
		// Put the results file in the same place as the sceFile
		String logFile = resultsFile.replaceAll(".txt", ".log");
		this.engineRunning=true;
		this.exec.runScenario(logFile, scenario, resultsFile, this.callback);
		this.engineRunning=false;
		this.pnt.actionPerformed(new ActionEvent(this, 1, "Graphing"));
		if(this.createGraphs)
		  plotter.graphResults(resultsFile,resultsDir.getAbsolutePath());	
		Log.info("Calculation Complete");
		Log.removeAppender(this.listener);
		this.pnt.actionPerformed(new ActionEvent(this, 1, "CalcComplete"));
	}
	
	public void cancel(boolean createGraphs)
	{
		this.createGraphs=createGraphs;
		if(this.engineRunning)
		{
			this.exec.cancelScenario();
		}
	}
}
