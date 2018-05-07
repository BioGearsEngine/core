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
package mil.tatrc.physiology.biogears.testing;

import mil.tatrc.physiology.biogears.engine.*;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.TimeUnit;
import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Resources;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.testing.TestDriver;
import mil.tatrc.physiology.utilities.testing.TestDriver.TestJob;

import java.io.FileNotFoundException;

public class ScenarioTestDriver implements TestDriver.Executor
{
	private Resources m_resources = new Resources();
	// Yes I am manipulating the xml as a string when I have bind classes
	// I should really clean this up and use the bind classes (not the SE classes)
	
	public boolean ExecuteTest(TestJob job)
	{
		String outputFile = job.computedDirectory+job.name;
		String log;
		String results;
		String xml = "";
		try {
			xml = FileUtils.readFile(m_resources.findResource("verification/Scenarios/"+job.name));
		} catch (FileNotFoundException e) {
			Log.error("Could not find : verification/Scenarios/"+job.name);
			return false;
		}
		
		if(job.patientFile==null)
		{
			log = outputFile.replaceAll("xml", "log");
			results = outputFile.replaceAll(".xml", "Results.txt");
		}
		else
		{
			String patientName = job.patientFile.substring(0,job.patientFile.length()-4);
			log = outputFile.replaceAll(".xml", "-"+patientName+".log");
			results = outputFile.replaceAll(".xml", "-"+patientName+"Results.txt");
			
			int startTag = xml.indexOf("<PatientFile>");
			if(startTag == -1)
			{
				// I did not test this code!!
				startTag = xml.indexOf("<EngineStateFile>");
				String startXML = xml.substring(0,startTag);
				String restOfTheXML = xml.substring(xml.indexOf("</EngineStateFile>"));		
				xml = startXML+"<PatientFile>"+job.patientFile+"</PatientFile>"+restOfTheXML.substring(18);
				Log.warn("Replacing engine state with a patient file for xml file : "+job.name+". Expect initialization time");
			}
			else
			{
				String startXML = xml.substring(0,startTag);
				String restOfTheXML = xml.substring(xml.indexOf("</PatientFile>"));		
				xml = startXML+"<PatientFile>"+job.patientFile+restOfTheXML;
			}
		}
		if(job.useState)
		{
			
			int startTag = xml.indexOf("<InitialParameters>");
			if(startTag == -1)
			{
				Log.warn("Cannot find InitialParameters tag in xml...");
			}
			else
			{
				String startXML = xml.substring(0,startTag);
				String initialParameters = xml.substring(startTag,xml.indexOf("</InitialParameters>"));
				String restOfTheXML = xml.substring(xml.indexOf("</InitialParameters>"));
				
				if(initialParameters.indexOf("<Condition")>-1)
				{
					// Don't run any scenarios with conditions
					Log.warn("States cannont be applied to a scenario with conditions, will run with patient file for "+job.name);
				}
				else
				{				
				String patientStateFile;
				if(job.patientFile!=null && !job.patientFile.isEmpty())
					patientStateFile = job.patientFile;
				else// Dig it out of the xml
					patientStateFile = xml.substring(xml.indexOf("<PatientFile>")+13,xml.indexOf("</PatientFile>")).trim();					

				patientStateFile = patientStateFile.substring(0, patientStateFile.indexOf(".xml"));
				patientStateFile = "./states/"+patientStateFile+"@0s.xml";
				xml = startXML+"<EngineStateFile>"+patientStateFile+"</EngineStateFile>"+restOfTheXML.substring(20);
				}
			}
		}
		if(job.autoSerialization!=null)
		{
			int startTag = xml.indexOf("<AutoSerialization>");
			if(startTag != -1)
			{
				// strip this out
				String startXML = xml.substring(0,startTag);
				String restOfTheXML = xml.substring(xml.indexOf("</AutoSerialization>")+20);
				xml = startXML+restOfTheXML;
			}
			
			startTag = xml.indexOf("<DataRequests");
			if(startTag == -1)				
				startTag = xml.indexOf("<Action");
			
			String startXML = xml.substring(0,startTag);
			String restOfTheXML = xml.substring(startTag);
			String serializationTag = "<AutoSerialization>\n"+
			                          "  <Directory>"+job.autoSerialization.getDirectory() +"</Directory>\n"+
			                          "  <FileName>"+job.name+"</FileName>\n"+
			                          "  <AfterActions>"+job.autoSerialization.getAfterActions().value()+"</AfterActions>\n"+
			                          "  <Period value=\""+job.autoSerialization.getPeriod().getValue(TimeUnit.s)+"\" unit=\"s\"/>\n"+
			                          "  <PeriodTimeStamps>"+job.autoSerialization.getPeriodTimeStamps().value()+"</PeriodTimeStamps>\n"+
			                          "  <ReloadState>"+job.autoSerialization.getReloadState().value()+"</ReloadState>\n"+
			                          "</AutoSerialization>\n";
			xml = startXML+serializationTag+restOfTheXML;
			
		}
		BioGearsScenarioExec bge = new BioGearsScenarioExec();
		bge.setListener(job);			
		bge.runScenarioXML(log, xml, results);
		Log.info("Completed running "+job.name);
		bge=null;
		return true;
	}
	
	public static void main(String[] args)
	{
		TestDriver.main(args);
	}
}
