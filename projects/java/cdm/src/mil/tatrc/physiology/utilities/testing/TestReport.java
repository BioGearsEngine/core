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

/**
 * 
 */
package mil.tatrc.physiology.utilities.testing;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.TestReportData;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.TimeUnit;
import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Log;

/**
 * @author abray, boday
 *
 */
public class TestReport
{  
	protected String         name;
	protected String         fileName;
	protected String         reportDir;
	protected List<TestSuite>testSuites;
	protected boolean removeNamespace=false;
	protected String  dbDirectory="./";
	protected List<String> knownFailingSuites;

	private class Data
	{
		public String name;
		public int runs;
		public int errors;
		public double time;
		public List<String> failures = new ArrayList<String>();
		public List<String> requirements = new ArrayList<String>();
		public StringBuilder html=new StringBuilder();
	}

	private class DataComparator implements Comparator<Data>
	{
		public int compare(Data o1, Data o2) 
		{
			return o1.name.compareTo(o2.name);
		}
	}

	public TestReport()
	{
		setFileName("TestReport.xml");
		this.reportDir="./";
		testSuites = new ArrayList<TestSuite>();
		knownFailingSuites = new ArrayList<String>();
	}

	/**
	 * Set the directory to look for any external
	 * database/dataset files
	 * @param dbDir
	 */
	public void setDatabaseDirectory(String dbDir)
	{
		this.dbDirectory=dbDir;
	}

	public String getDatabaseDirectory()
	{
		return dbDirectory;
	}

	public void setFullReportPath(String path)
	{
		path = path.replaceAll("[\\\\]", "/");
		String dir = path.substring(0,path.lastIndexOf("/")+1);
		setReportDirectory(dir);
		String file = path.substring(path.lastIndexOf("/")+1);
		setFileName(file);
	}

	public void setReportDirectory(String dir)
	{
		try
		{
			FileUtils.createDirectory(dir);
			this.reportDir=dir+"/";
		}
		catch(Exception ex)
		{
			Log.error(ex);
			this.reportDir="./";
		}
	}

	public void setFileName(String fileName)
	{
		if(fileName.indexOf(".")>=0)
		{
			this.fileName=fileName;
			this.name = fileName.substring(0,fileName.lastIndexOf("."));
		}
		else
		{
			this.name = fileName;
			this.fileName=fileName+".xml";// make it an xml file
		}

	}
	public void setFileName(String name, String extension)
	{
		this.name = name;
		this.fileName=name+extension;
	}
	public String getFileName()
	{
		return this.fileName;
	}

	public String getName()
	{
		return this.name;
	}
	public void setName(String name)
	{
		this.name = name;
	}

	public void load(TestReportData in)
	{

	}

	public TestReportData unload()
	{
		TestReportData testReport = CDMSerializer.objFactory.createTestReportData();
		for(TestSuite s : this.testSuites)
		{
			testReport.getTestSuite().add(s.unload());
		}
		return testReport;
	}

	public int getErrors()
	{
		int errors=0;
		for(TestSuite suite : this.testSuites)
			errors+=suite.getErrors();
		return errors;
	}

	public TestSuite createTestSuite()
	{
		TestSuite suite = new TestSuite();
		addSuite(suite);
		return suite;
	}

	public void addSuite(TestSuite suite)
	{
		this.testSuites.add(suite);
	}

	public void createErrorSuite(String error)
	{
		TestSuite testSuite = new TestSuite();
		testSuite.setTime(0,TimeUnit.s);
		testSuite.setTests(1);
		testSuite.setErrors(1);
		testSuite.setName(error);
		testSuite.setPerformed(false);
		this.addSuite(testSuite);
	}

	public void addKnownSuiteFailure(String suiteName)
	{
		knownFailingSuites.add(suiteName);
	}

	public void addSummary(TestReport rpt)
	{
		for(TestSuite s : rpt.testSuites)
		{
			addSuite(s);
		}
	}

	public void write()
	{
		write(this.reportDir);
	}

	public void write(String toDirectory)
	{
		if(toDirectory==null||toDirectory.isEmpty())
			toDirectory="./";

		if(removeNamespace)
		{
			try
			{     
				String str = CDMSerializer.serialize(unload());
				str = str.replaceAll("xmlns=\"uri:/mil/dtra/iwmdt/nucs\"", "");
				String reportName = toDirectory + this.fileName;
				BufferedWriter out = new BufferedWriter(new FileWriter(reportName));
				out.write(str);
				out.close();
			}
			catch (IOException e)
			{
				Log.error(e);
			}
		}
		else
			CDMSerializer.writeFile(toDirectory + this.fileName, unload());
	}

	public String toHTML()
	{
		return toHTML(null);
	}

	public String toHTML(Map<String,List<String>> groups)
	{
		boolean sortResults = true;
		StringBuilder buffer = new StringBuilder();
		buffer.append("<html>");
		buffer.append("<body>");

		// Any Test Suites Not Run?
		buffer.append("<table border=\"1\">");
		buffer.append("<tr><th>Missing Reports</th></tr>");
		for(TestSuite suite : this.testSuites)
		{
			if(!suite.isPerformed())
				buffer.append("<tr bgcolor=\"#FF0000\"><td>"+suite.getName()+"</td></tr>");
		}
		buffer.append("</table>");
		// Make a little room
		buffer.append("<br>");
		
		// Any suites known to fail?
		buffer.append("<table border=\"1\">");
		buffer.append("<tr><th>Known Failing Reports</th></tr>");
		for(String suite : this.knownFailingSuites)
		{
			buffer.append("<tr bgcolor=\"#FFFF00\"><td>"+suite+"</td></tr>");
		}
		buffer.append("</table>");

		buffer.append("</body>");
		buffer.append("</html>");

		if(groups == null)
		{
			groups = new HashMap<String,List<String>>();
			List<String> all = new ArrayList<String>();
			for(TestSuite suite : this.testSuites)
				all.add(suite.getName());
			groups.put(this.name, all);
		}

		for(String group : groups.keySet())
		{
			List<String> groupTests = groups.get(group);
			// Make a little room
			buffer.append("<br>");

			// Report Suite Summary Results
			buffer.append("<table border=\"1\">");
			buffer.append("<caption><b>"+group+"</b></caption>");
			buffer.append("<tr>");
			buffer.append("<th> Test Case </th>");      
			buffer.append("<th> # of Failures </th>");
			buffer.append("<th> # of Runs </th>");
			buffer.append("<th> Time </th>");
			buffer.append("<th> Failures </th>");
			buffer.append("<th> Requirements Tested </th>");
			buffer.append("</tr>");


			int runs=0;
			int totalRuns=0;
			int totalErrors=0;
			double totalTime=0;
			List<TestReport.Data> errorData = new ArrayList<TestReport.Data>();
			List<TestReport.Data> passedData = new ArrayList<TestReport.Data>();

			TestReport.Data data;
			for(TestSuite suite : this.testSuites)
			{ 
				if(!groupTests.contains(suite.getName()))
						continue;
				
				totalRuns+=suite.getTests();
				totalErrors+=suite.getErrors();
				totalTime+=suite.getTime();


				if(suite.isPerformed())
				{
					runs++;
					data=new Data();
					data.name=suite.getName();
					data.runs=suite.getTests();
					data.errors=suite.getErrors();        
					data.time=suite.getTime();
					if(suite.getCase()!=null)
					{
						for(String failure : suite.getCase().getFailure())
						{// Only keeping failures with this string pattern: (Could make this an input and more easy to configure)
							int idx = failure.indexOf("ScenarioResultsCompareTool : Compare Failed for ");
							if(idx>-1)
								data.failures.add(failure.substring(48));
						}
					}
					data.requirements.addAll(suite.getRequirements());
					if(suite.getErrors()>0)
					{
						errorData.add(data);
						data.html.append("<tr bgcolor=\"#FF0000\">");
					}
					else
					{
						passedData.add(data);
						data.html.append("<tr bgcolor=\"#00FF00\">");
					}
					data.html.append("<td align=\"left\">"+data.name+"</td>");   
					data.html.append("<td>"+data.errors+"</td>");
					data.html.append("<td>"+data.runs+"</td>");  
					data.html.append("<td>"+data.time+"</td>");
					data.html.append("<td>");
					if(data.failures.size() > 0)
					{
						int i=0;
						while (i < data.failures.size()-1)
						{
							data.html.append(data.failures.get(i) + "<br>");
							i++;
							if(i>5)//Only write out the first few errors, could be a LOT of errors
								break;
						}
						data.html.append(data.failures.get(i));
					}
					data.html.append("</td>");
					data.html.append("<td>");
					if (data.requirements.size() > 0)
					{
						int i=0;
						while (i < data.requirements.size()-1)
						{
							data.html.append(data.requirements.get(i) + " , ");
							i++;
						}
						data.html.append(data.requirements.get(i));
					}
					data.html.append("</td>");
					data.html.append("</tr>");
				}
			}

			buffer.append("<tr>");
			buffer.append("<td align=\"left\">"+"Totals for "+runs+" test suites"+"</td>");   
			buffer.append("<td>"+totalErrors+"</td>");
			buffer.append("<td>"+totalRuns+"</td>");  
			buffer.append("<td>"+totalTime+"</td>"); 
			buffer.append("</tr>");

			if(sortResults)
			{
				DataComparator comparator=new DataComparator();
				Collections.sort(errorData,comparator);
				Collections.sort(passedData,comparator);
			}
			for(Data d : errorData)
				buffer.append(d.html);
			for(Data d : passedData)
				buffer.append(d.html);

			buffer.append("</table>");

			// Make a little room
			buffer.append("<br>");
		}

		return buffer.toString();
	}
}
