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

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.PrintWriter;
import java.lang.reflect.*;
import java.net.InetAddress;
import java.util.*;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.PatientAssessmentData;
import mil.tatrc.physiology.datamodel.bind.PatientData;
import mil.tatrc.physiology.datamodel.bind.ScalarData;
import mil.tatrc.physiology.datamodel.patient.SEPatient;
import mil.tatrc.physiology.datamodel.patient.assessments.SEPatientAssessment;
import mil.tatrc.physiology.datamodel.properties.SEScalar;
import mil.tatrc.physiology.utilities.WaveformUtils;
import mil.tatrc.physiology.utilities.csv.CSVContents;
import mil.tatrc.physiology.utilities.DoubleUtils;
import mil.tatrc.physiology.utilities.EmailUtil;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.StringUtils;
import mil.tatrc.physiology.utilities.UnitConverter;

import org.apache.poi.ss.usermodel.FormulaEvaluator;
import org.apache.poi.xssf.usermodel.*;

public abstract class ValdiationTool
{
  public static void main(String[] args)
  {
    SystemValidation.main(args);
    //PatientValidation.main(args);
  }
  protected String DEST_DIRECTORY;
  protected String DEFAULT_DIRECTORY;
  protected String DEFAULT_FILE;
  protected String TABLE_TYPE;
  protected String HEADER_PREPEND="";
  
  //Keywords success +/- 10% ,warning +/- 30%, danger+/-10%,fail
  protected static final String success = "<span class=\"success\">";
  protected static final String warning = "<span class=\"warning\">";
  protected static final String danger = "<span class=\"danger\">";
  protected static final String endSpan = "</span>";
  
  protected void CustomMarkdown(String sheetName, String destinationDirectory){};
  
  protected StringBuilder html = new StringBuilder();
  
  protected enum DataType { Mean, WaveformMin, WaveformMax, Enum, Patient2SystemMean, Patient2SystemMin, Patient2SystemMax, MeanPerWeight, WaveformMinPerWeight, WaveformMaxPerWeight }
  // Patient2System variables take the value from the patient file and compare it to a system property
  // PerWeight calculates the expected value based on the patient weight
  
  protected SEPatient                       patient;
  protected Map<String,List<Double>>        resultData = new HashMap<String,List<Double>>();
  protected Map<String,SEPatientAssessment> assessments;

  protected class ValidationRow
  {
    public DataType dType = null;
    // xlsx values
    public String   name;   
    public String   unit = null; 
    public String   weightUnit = null;
    public String   refValues  = "";
    public String   refCites   = "";
    public double   refValue    = Double.NaN;
    public double   intervalMin = Double.NaN;
    public double   intervalMax = Double.NaN;  
    public double   result = Double.NaN;
    public double   resultError;
    public String   resultEnum = "";
    
    //  table headings
    public String   header   = "";
    public String   expected = "";
    public String   engine   = "";
    public String   error    = "";
    public String   notes    = "";
    public String   table    = "";
    
    // Results Data
    public String       resultFile = "";
    public List<Double> results;
    public List<Double> weight;
    
    public String doubleFormat="3f";
    public double successTolerance = 10;
    public double warningTolerance = 30;
  }

  public void loadData(String revision, String env, String arch, boolean sendEmail)
  {
    String directoryName = DEFAULT_DIRECTORY;
    String fileName = DEFAULT_FILE;
    String destinationDirectory = DEST_DIRECTORY;
    try
    {
      File dest = new File(DEST_DIRECTORY);
      dest.mkdir();
      // Delete current dir contents
      // FileUtils.delete(destinationDirectory);
      // Ok, let's make them again
      // FileUtils.createDirectory(destinationDirectory);
    }
    catch(Exception ex)
    {
      Log.error("Unable to clean directories");
      return;
    }
    try
    {   
      File xls = new File(directoryName+"/"+fileName);
      if(!xls.exists())
      {
        Log.error("Could not find xls file "+directoryName+"/"+fileName);
        return;
      }
      // Read in props file
      File file = new File("ValidationTables.config");
      FileInputStream fileInput = new FileInputStream(file);
      Properties config = new Properties();
      config.load(fileInput);
      fileInput.close();
      
      // Set up the Email object
      String hostname = "Unknown";
      try
      {
        InetAddress addr = InetAddress.getLocalHost();
        hostname = addr.getHostName();
      }
      catch(Exception ex)
      {
        System.out.println("Hostname can not be resolved");
      }    
      EmailUtil email = new EmailUtil();
      String subj = env+" "+arch+" "+TABLE_TYPE+" Validation from "+hostname + " Revision " + revision;
      email.setSubject(subj);
      email.setSender(config.getProperty("sender"));
      email.setSMTP(config.getProperty("smtp"));
      if(hostname.equals(config.get("buildhost")))
      {
        Log.info("Emailling all recipients " + subj);
        for(String recipient : config.getProperty("recipients").split(","))
          email.addRecipient(recipient.trim());
      }
      else
      {// Running on your own machine, just send it to yourself
        Log.info("Emailling local runner " + subj);
        email.addRecipient(System.getProperty("user.name")+"@ara.com");
      }
      html.append("<html>");
      html.append("<body>");
      
      // Get a list of all the results files we have to work with
      
      File vdir = new File("./Scenarios/Validation/");
      String[] vFiles = vdir.list();
      
      // Now read in the spreadsheet      
      FileInputStream xlFile = new FileInputStream(directoryName+"/"+fileName);   
      XSSFWorkbook xlWBook =  new XSSFWorkbook (xlFile);
      
      FormulaEvaluator evaluator = xlWBook.getCreationHelper().createFormulaEvaluator();
      List<ValidationRow> badSheets = new ArrayList<ValidationRow>();
      Map<String,List<ValidationRow>> tables = new HashMap<String,List<ValidationRow>>();
      Map<String,List<ValidationRow>> tableErrors = new HashMap<String,List<ValidationRow>>();
      List<ValidationRow> allRows = new ArrayList<ValidationRow>();
      for(int i=0; i<xlWBook.getNumberOfSheets(); i++)
      {
        XSSFSheet xlSheet = xlWBook.getSheetAt(i);
        Log.info("Processing Sheet : " + xlSheet.getSheetName());       
        String sheetName = xlSheet.getSheetName().trim().replaceAll(" ", "");
        
        List<String> sheetFiles = new ArrayList<String>();
        String rSheetName = sheetName+"ValidationResults.txt";
        File rFile = new File(rSheetName);
        if(!rFile.exists())
        {
          // Search for any file starting with the sheet name
          for(String f : vFiles)
            if(f.startsWith(sheetName) && f.endsWith(".txt"))
              sheetFiles.add(f);
        }
        else
          sheetFiles.add(rSheetName);
        
        for(String resultsName : sheetFiles)
        {
        	Log.info("Processing "+resultsName);
          try
          {          
            // Look for a results file
            CSVContents results = new CSVContents("./Scenarios/Validation/"+resultsName);
            results.readAll(resultData);              
            // Find any assessments
            assessments = new HashMap<String,SEPatientAssessment>();      
            for(String vFile : vFiles)
            {
              if(vFile.indexOf(sheetName)>-1 && vFile.indexOf('@')>-1)
              {
                Object aData = CDMSerializer.readFile("./Scenarios/Validation/"+vFile);
                if(aData instanceof PatientAssessmentData)
                {
                  String aClassName = "SE"+aData.getClass().getSimpleName();
                  aClassName = aClassName.substring(0, aClassName.indexOf("Data"));
                  try
                  {
                    Class<?> aClass = Class.forName("mil.tatrc.physiology.datamodel.patient.assessments."+aClassName);
                    SEPatientAssessment a = (SEPatientAssessment)aClass.newInstance();
                    aClass.getMethod("load", aData.getClass()).invoke(a, aData);
                    assessments.put(vFile, a);
                  }
                  catch(Exception ex)
                  {
                    Log.error("Unable to load assesment xml "+vFile,ex);
                  }
                }
                else
                  Log.error(vFile+" is named like a patient assessment, but its not?");
              }
            }
          }
          catch(Exception ex)
          {
            ValidationRow vRow = new ValidationRow();
            vRow.header = sheetName;
            vRow.error=danger+"No results found for sheet "+endSpan;
            badSheets.add(vRow);          
            continue;
          }
          // Is this patient validation?
          patient = null;
          if(TABLE_TYPE.equals("Patient"))
          {
            // Patient Name is encoded in the naming convention (or else it needs to be)
            String patientName = resultsName.substring(resultsName.lastIndexOf("-")+1,resultsName.indexOf("Results"));            
            patient = new SEPatient();
            patient.load((PatientData)CDMSerializer.readFile("./stable/"+patientName+".xml"));
          }

          allRows.clear();
          tables.clear();
          tableErrors.clear();                
          // Read the sheet and process all the validation data rows
          try
          {

            int rows = xlSheet.getPhysicalNumberOfRows();     
            for (int r = 0; r < rows; r++) 
            {
              XSSFRow row = xlSheet.getRow(r);
              if (row == null) 
                continue;
              int cells = 11;//row.getPhysicalNumberOfCells();
              XSSFCell cell = row.getCell(0);
              if(cell==null)
                continue;
              // Check to see if this row is a header
              String cellValue = cell.getStringCellValue();
              if(cellValue==null||cellValue.isEmpty())
                continue;// No property, skip it
              cellValue = row.getCell(1).getStringCellValue();
              if(cellValue!=null&&cellValue.equals("Units"))
                continue;// Header                      


              ValidationRow vRow = new ValidationRow();
              allRows.add(vRow);
              for (int c = 0; c <= cells; c++) 
              {
                cellValue = null;
                cell = row.getCell(c);
                if(cell==null)
                  continue;       
                switch(cell.getCellType())
                {
                  case XSSFCell.CELL_TYPE_NUMERIC:
                    cellValue = Double.toString(cell.getNumericCellValue());
                    break;
                  case XSSFCell.CELL_TYPE_STRING:
                    cellValue = cell.getStringCellValue();
                    break;
                  case XSSFCell.CELL_TYPE_FORMULA:
                    switch(evaluator.evaluateFormulaCell(cell))
                    {
                      case XSSFCell.CELL_TYPE_NUMERIC:
                        cellValue = String.format("%."+3+"g", cell.getNumericCellValue());
                        break;
                      case XSSFCell.CELL_TYPE_STRING:
                        cellValue = cell.getStringCellValue();
                        break;                      
                    }         
                    
                }

                switch(c)
                {
                  case 0://A                 
                    Log.info("Processing "+cellValue);                  
                    vRow.name = cellValue.trim().replaceAll(" ","");
                    String prop = vRow.name;
                    if(vRow.name.indexOf('*')!=-1)
                      prop = prop.substring(0, prop.length()-1);
                    vRow.header = vRow.name;
                    break;
                  case 1://B
                    if(cellValue!=null&&
                    !cellValue.equalsIgnoreCase("none")&&
                    !cellValue.equalsIgnoreCase("n\\a")&&
                    !cellValue.equalsIgnoreCase("n/a"))
                    {
                      vRow.unit = cellValue;
                    }                                                      
                    if(vRow.unit!=null&&!vRow.unit.isEmpty())
                      vRow.header+="("+vRow.unit+")";
                    break;
                  case 2://C
                  	if(cellValue!=null)
                    {
                      String unit=null;
                      int u = cellValue.indexOf("(");
                      if(u>-1)
                      {
                        unit = cellValue.substring(u+1, cellValue.indexOf(")"));
                        cellValue = cellValue.substring(0, u);
                      }
                      vRow.dType = DataType.valueOf(cellValue);
                      if(vRow.dType == DataType.MeanPerWeight || 
                         vRow.dType == DataType.WaveformMinPerWeight || 
                         vRow.dType == DataType.WaveformMaxPerWeight)
                      {
                        vRow.weightUnit = unit;
                      }
                    }
                    break;
                  case 3://D
                    // Replace any return characters with empty
                  	if(patient!=null && vRow.name.indexOf('*')==-1)
                  	{
                  		try
                  		{
                  			Method has = SEPatient.class.getMethod("has"+vRow.name);
                  			if((Boolean)has.invoke(patient))
                  			{
                  				Method get = SEPatient.class.getMethod("get"+vRow.name);
                  				SEScalar s = ((SEScalar)get.invoke(patient));
                  				vRow.refValue = s.getValue(vRow.unit);
                  				vRow.refValues = cellValue;
                  				break;
                  			}
                  			else
                  			{
                  				Log.error("Patient does not have a value for "+vRow.name);
                  			}
                  		}
                  		catch(Exception ex)
                  		{
                  			// Nothing to do, row is not a patient property
                  		}                  		
                  	}
                    if(cellValue==null)
                      vRow.refValues = null;
                    else
                      vRow.refValues = cellValue.replace("\n","");
                  	
                    break;
                  case 4://E
                    // Replace any return characters with empty
                    if(cellValue!=null)
                      cellValue = cellValue.replace("\n","");
                    vRow.refCites = cellValue;                  
                    break;
                  case 5://F Reference Page (Internal only)
                    break;
                  case 6://G Notes
                    if(cellValue!=null)
                      vRow.notes = cellValue;
                    break;// Skipping for now
                  case 7://H Internal Notes (Internal only)
                    break;
                  case 8://I Reading (Internal only)
                    break;
                  case 9://J Table (Internal only)
                    if(cellValue==null)
                      cellValue = "";
                    vRow.table = cellValue;
                    if(patient!=null)
                      vRow.table = patient.getName() +"Patient"+cellValue;
                    break;
                  case 10://K ResultFile (Internal only)
                    if(cellValue!=null)
                      vRow.resultFile = cellValue;
                    break;
                  case 11://L Mantissa Digits
                    if(cellValue!=null)
                      vRow.doubleFormat = cellValue;
                    if(patient != null && vRow.dType != DataType.Patient2SystemMean)
                    	vRow.refValues = String.format("%."+vRow.doubleFormat, vRow.refValue);
                    break;
                }
              }
            }  
          }
          catch(Exception ex)
          {
            Log.error("Error reading row",ex);
            ValidationRow vRow = new ValidationRow();
            vRow.header = sheetName;
            vRow.error=danger+"Sheet has errors"+endSpan;
            badSheets.add(vRow);   
            continue;
          }

          // Sort all of our rows, and validate them
          for(ValidationRow vRow : allRows)
          {
            if(vRow.table.isEmpty())
              vRow.table = sheetName;//Default table is the sheet name
            if(!tables.containsKey(vRow.table))
              tables.put(vRow.table, new ArrayList<ValidationRow>());   
            if(!tableErrors.containsKey(vRow.table))
              tableErrors.put(vRow.table, new ArrayList<ValidationRow>());   
            if(buildExpectedHeader(vRow))
            {
              Log.info("Validating "+vRow.header); 
              if(validate(vRow))
              {
                tables.get(vRow.table).add(vRow);
              }
              else
                tableErrors.get(vRow.table).add(vRow);
            }
            else
              tableErrors.get(vRow.table).add(vRow);
          }
        
          for(String name : tables.keySet())
          {
            if(name.contains("All"))
              continue;
            List<ValidationRow> t = tables.get(name);
            WriteHTML(t,name);
            WriteDoxyTable(t,name,destinationDirectory);
            if(name.equalsIgnoreCase(sheetName))
            {
              List<String> properties = new ArrayList<String>();
              for(ValidationRow vRow : t)  
                properties.add(vRow.name);
              for(ValidationRow vRow : tableErrors.get(name))  
                properties.add(vRow.name);
              CrossCheckValidationWithSchema(properties, tableErrors.get(name), name);
            }
            WriteHTML(tableErrors.get(name),name+"Errors");
            if(patient!=null)
            	CustomMarkdown(patient.getName(),destinationDirectory);  
          }             
        }
      }
      xlWBook.close();
      WriteHTML(badSheets,fileName+" Errors");
      html.append("</body>");
      html.append("</html>");
      if(sendEmail)
        email.sendHTML(html.toString());
    }
    catch(Exception ex)
    {
      Log.error("Error processing spreadsheet "+fileName,ex);
    }
    // Just for fun, I am going to create a single md file with ALL the tables in it
    
    try
    {
      String line;
      File vDir = new File(destinationDirectory);
      PrintWriter writer = new PrintWriter(destinationDirectory+"/AllValidationTables.md", "UTF-8");
      
      for (String fName : vDir.list())
      {
        if(fName.equals("AllValidationTables.md"))
          continue;
        if(new File(fName).isDirectory())
          continue;
        FileReader in = new FileReader(destinationDirectory+"/"+fName);
        BufferedReader inFile = new BufferedReader(in);
        writer.println(fName);
        while ((line = inFile.readLine()) != null)
          writer.println(line);
        inFile.close();
        writer.println("<br>");
      }
      writer.close();
    }
    catch (Exception ex)
    {
      Log.error("Unable to create single validation table file.",ex);
    }
  }
  
  protected boolean buildExpectedHeader(ValidationRow vRow)
  {
    if(vRow.name.startsWith("EndTidalCarbonDioxideFraction"))
     Log.info("Here");
    if(vRow.name.contains("*"))
    {
      vRow.error = warning+"Not Validating"+endSpan;
      return false;
    }
    if(vRow.refValues==null||vRow.refCites==null)
    {
      vRow.error = danger+"No reference provided"+endSpan;
      return true;
    }
    String[] refs  = vRow.refCites.trim().split("\\s*,\\s*");
    String[] rVals = vRow.refValues.trim().split("\\s*,\\s*");
    // Check to see if we have 1 value
    if(rVals.length==1)
    {
      if(refs.length!=1)
      {
        vRow.expected = "1 reference value should have 1 reference";
        return false;
      }
      String cite = "@cite ";
      if(vRow.refCites.contains("["))
        cite = "";// This is an equation, not a cite
      if(vRow.dType==DataType.Mean        ||  vRow.dType == DataType.MeanPerWeight ||
         vRow.dType==DataType.WaveformMin ||  vRow.dType == DataType.WaveformMinPerWeight ||
         vRow.dType==DataType.WaveformMax ||  vRow.dType == DataType.WaveformMaxPerWeight)
      {
      	try{vRow.refValue = Double.parseDouble(vRow.refValues);}
      	catch(Exception ex)
      	{
      		Log.error(vRow.name +" could not parse reference value "+vRow.refValues);
      	}
        if(vRow.refCites.equals("N/A"))
          vRow.expected = String.format("%."+vRow.doubleFormat, vRow.refValue);
        else
          vRow.expected = String.format("%."+vRow.doubleFormat, vRow.refValue)+" "+cite+vRow.refCites;
        
        if(vRow.dType==DataType.MeanPerWeight || vRow.dType==DataType.WaveformMinPerWeight || vRow.dType==DataType.WaveformMaxPerWeight)
        {
          if(vRow.header.endsWith(")"))
            vRow.header = vRow.header.substring(0, vRow.header.length()-1) + "/" +vRow.weightUnit + ")";
          else
            vRow.header = vRow.header + "/" +vRow.weightUnit;
        }               
      }
      else if(vRow.dType==DataType.Patient2SystemMean || vRow.dType==DataType.Patient2SystemMax || vRow.dType==DataType.Patient2SystemMin)
      {        
        vRow.header = vRow.name;   
        vRow.name = vRow.refValues;
        if(vRow.dType == DataType.Patient2SystemMax)
          vRow.header += " Maximum";
        if(vRow.dType == DataType.Patient2SystemMin)
          vRow.header += " Minimum";
        if(vRow.unit!=null && !vRow.unit.isEmpty())
          vRow.header += "("+vRow.unit+")";       
        vRow.expected = String.format("%."+vRow.doubleFormat, vRow.refValue);
      }
      else if(!vRow.refValues.isEmpty())
      {
        if(vRow.refCites.equals("N/A"))
          vRow.expected = vRow.refValues;
        else
          vRow.expected = vRow.refValues+" "+cite+vRow.refCites;
      }
           
      return true;
    }
    // Check to see if vRow.refValues is an interval, which is always form of [x,y]
    
    char start = vRow.refValues.charAt(0);
    if( rVals.length==2 && start=='[')
    {// We are an interval range   
      if(refs.length!=1)
      {
        vRow.expected = "Need a single reference for an interval";
        return false;
      }
      int sep = vRow.refValues.indexOf(",");
      if(sep==-1)
      {
        vRow.expected = "Malformed interval string : "+vRow.refValues;
        return false;
      }
      if(vRow.refValues.indexOf("]")<sep)
      {
        vRow.expected = "Malformed interval string : "+vRow.refValues;
        return false;
      }
      vRow.intervalMin = Double.parseDouble(vRow.refValues.substring(1, sep));
      vRow.intervalMax = Double.parseDouble(vRow.refValues.substring(sep+1,vRow.refValues.length()-1));   
      String cite = "@cite ";
      if(vRow.refCites.contains("["))
        cite = "";// This is an equation, not a cite
      vRow.expected = vRow.refValues+" "+cite+vRow.refCites;
      return true;
    }
    else
    {// It's more than one value,intervals, this is where things get tricky..      
      List<String> cites = new ArrayList<String>();
      List<Double> values = new ArrayList<Double>();

      int cIdx=0;
      for(int i=0; i<rVals.length; i++)
      {
        if(rVals[i].startsWith("["))
        {
          cites.add(refs[cIdx]);
          try
          {values.add(Double.parseDouble(rVals[i].substring(1)));}
          catch(Exception ex)
          {
          	Log.error(vRow.header + " has string formatting error : " + vRow.refValues);
          	return false;
          }
          i++;
          if(!rVals[i].endsWith("]"))
          {
            vRow.expected = "Malformed interval string : "+vRow.refValues;
            return false;
          }
          cites.add(refs[cIdx]);
          values.add(Double.parseDouble(rVals[i].substring(0, rVals[i].length()-1)));
          cIdx++;
          continue;
        }
        cites.add(refs[cIdx]);
        values.add(Double.parseDouble(rVals[i]));
        cIdx++;
      }

      int minIdx=0, maxIdx=0;
      vRow.intervalMin = Double.MAX_VALUE;
      vRow.intervalMax = -Double.MAX_VALUE;
      for(int i=0; i<values.size(); i++)
      {
        double d = values.get(i);
        if(d < vRow.intervalMin)
        {
          minIdx = i;
          vRow.intervalMin = d;
        }
        if(d > vRow.intervalMax)
        {
          maxIdx = i;
          vRow.intervalMax = d;
        }
      }
      String cite = "@cite ";
      if(cites.get(minIdx).contains("["))
        cite = "";// This is an equation, not a cite
      if(cites.get(minIdx).equals(cites.get(maxIdx)))
        vRow.expected = "["+vRow.intervalMin+", "+vRow.intervalMax+"] "+cite+cites.get(minIdx);
      else// We should never have two different calculations for our interval... but if we do, you got more coding to do!
        vRow.expected = "["+vRow.intervalMin+" @cite "+cites.get(minIdx)+", "+vRow.intervalMax+" @cite "+cites.get(maxIdx)+"]";

      return true;
    }
  }

  protected boolean validate(ValidationRow vRow)
  {
    if(vRow.name.startsWith("EndTidalCarbonDioxideFraction"))
      Log.info("Break");
    if(!getResult(vRow))
    {
      return false;
    }
    switch(vRow.dType)
    {
      case Mean:
      case Patient2SystemMean:
      {
        vRow.result = DoubleUtils.getAverage(vRow.results);
        vRow.engine = "Mean of " + String.format("%."+vRow.doubleFormat, vRow.result); //StringUtils.toString(vRow.mean,3);
        break;
      }      
      case WaveformMin:
      {
        vRow.result = WaveformUtils.getMin(vRow.results);
        vRow.engine = "Minimum of " + String.format("%."+vRow.doubleFormat, vRow.result); //StringUtils.toString(vRow.mean,3);
        break;
      }
      case Patient2SystemMin:
      {
        vRow.result = DoubleUtils.getMin(vRow.results);
        vRow.engine = "Minimum of " + String.format("%."+vRow.doubleFormat, vRow.result); //StringUtils.toString(vRow.mean,3);
        break;
      }      
      case WaveformMax:
      {
        vRow.result = WaveformUtils.getMax(vRow.results);
        vRow.engine = "Maximum of " + String.format("%."+vRow.doubleFormat, vRow.result); //StringUtils.toString(vRow.mean,3);
        break;
      }
      case Patient2SystemMax:
      {
        vRow.result = DoubleUtils.getMax(vRow.results);
        vRow.engine = "Maximum of " + String.format("%."+vRow.doubleFormat, vRow.result); //StringUtils.toString(vRow.mean,3);
        break;
      }      
      case Enum:
      {
        vRow.engine = vRow.resultEnum;
        break;
      }
      case MeanPerWeight:
      {
        List<Double> resultPerWeight = new ArrayList<Double>();
        for(int i=0; i<vRow.results.size(); i++)
        {
          resultPerWeight.add(vRow.results.get(i) / vRow.weight.get(i));
        }
        vRow.result = DoubleUtils.getAverage(resultPerWeight);
        vRow.engine = "Mean of " + String.format("%."+3+"g", vRow.result); //StringUtils.toString(vRow.mean,3);
        break;
      }
      case WaveformMinPerWeight:
      {
        List<Double> xMin = new ArrayList<Double>();
        List<Double> yMin = new ArrayList<Double>();
        List<Double> xMax = new ArrayList<Double>();
        List<Double> yMax = new ArrayList<Double>();    
        WaveformUtils.getPeriodBounds(vRow.weight, vRow.results, xMin, yMin, xMax, yMax);
        List<Double> resultPerWeight = new ArrayList<Double>();
        for(int i=0; i<yMin.size(); i++)
        {
          resultPerWeight.add(yMin.get(i) / xMin.get(i));
        }
        vRow.result = DoubleUtils.getMin(resultPerWeight);
        vRow.engine = "Minimum of " + String.format("%."+3+"g", vRow.result); //StringUtils.toString(vRow.mean,3);
        break;
      }
      case WaveformMaxPerWeight:
      {
        List<Double> xMin = new ArrayList<Double>();
        List<Double> yMin = new ArrayList<Double>();
        List<Double> xMax = new ArrayList<Double>();
        List<Double> yMax = new ArrayList<Double>();    
        WaveformUtils.getPeriodBounds(vRow.weight, vRow.results, xMin, yMin, xMax, yMax);
        List<Double> resultPerWeight = new ArrayList<Double>();
        for(int i=0; i<yMax.size(); i++)
        {
          resultPerWeight.add(yMax.get(i) / xMax.get(i));
        }
        vRow.result = DoubleUtils.getMax(resultPerWeight);
        vRow.engine = "Maximum of " + String.format("%."+3+"g", vRow.result); //StringUtils.toString(vRow.mean,3);
        break;
      }
      default:
      {
        vRow.error =  danger+"Unsupported data type "+vRow.dType+endSpan;
        return false;
      }
    }
    if(Double.isNaN(vRow.result) && vRow.resultEnum.isEmpty())
    {
      vRow.error = danger+"Not found in result"+endSpan;
      return false;
    }
    
    if(!Double.isNaN(vRow.intervalMin)&&!Double.isNaN(vRow.intervalMax))
    {     
      if(vRow.result < vRow.intervalMin)
      {
        vRow.resultError = -DoubleUtils.percentTolerance(vRow.intervalMin, vRow.result, 1e-10);
      }
      else if(vRow.result > vRow.intervalMax)
      {
        vRow.resultError = DoubleUtils.percentTolerance(vRow.intervalMax, vRow.result, 1e-10);
      }
      else
      {
        vRow.error = success+"Within bounds"+endSpan;
        return true;
      }
    }
    else if(!Double.isNaN(vRow.refValue))
    {// What is the % difference between the value and the test value      
      if(vRow.dType == DataType.Patient2SystemMax && vRow.result <= vRow.refValue)
      {
        vRow.resultError = 0;
        vRow.error  = success+"Under Max"+endSpan;
        return true;
      }
      if(vRow.dType == DataType.Patient2SystemMin && vRow.result >= vRow.refValue)
      {
        vRow.resultError = 0;
        vRow.error  = success+"Above Min"+endSpan;
        return true;
      }      
      vRow.resultError = DoubleUtils.percentTolerance(vRow.refValue, vRow.result, 1e-10);
      if(vRow.result < vRow.refValue)
      	vRow.resultError = -vRow.resultError;
    }
    else if(vRow.dType == DataType.Enum)
    {
      if(vRow.resultEnum.equalsIgnoreCase(vRow.engine))
      {
        vRow.error  = success+"Match"+endSpan;
      }
      else
      {
        vRow.error  = danger+"No Match"+endSpan;
      }
      return true;
    }
    else
    {
      if(vRow.error.isEmpty())
        vRow.error =  danger+"Unsupported reference value"+endSpan;
      return false;
    }
    
    if(Double.isNaN(vRow.resultError))
    {
      vRow.error  = warning+"N/A : divide by 0"+endSpan;
    }
    else
    {    
      String span;
      double abs = Math.abs(vRow.resultError);
      if(abs<vRow.successTolerance)
        span = success;
      else if(abs<vRow.warningTolerance)
        span = warning; 
      else
        span = danger;
      vRow.error  = span+StringUtils.toString(vRow.resultError,1)+"%"+endSpan;
    }
    
    return true;
  }
  
  protected boolean getResult(ValidationRow vRow)
  {
    if(vRow.name.startsWith("Carina-CarbonDioxide"))
      Log.info("Here");
    
    if(vRow.dType == DataType.MeanPerWeight || vRow.dType == DataType.WaveformMinPerWeight || vRow.dType == DataType.WaveformMaxPerWeight)
    {// Take off the weight unit from the header and unit we got from the spreadsheet
      String weightHeader = "PatientWeight("+vRow.weightUnit+")";
      vRow.weight = resultData.get(weightHeader);
      if(vRow.weight==null)
      {// Maybe the result is in a different unit than our reference value
        String newUnit=null;
        String[] split;
        for(String result : resultData.keySet())
        {
          split = result.split("[(]");
          if(split.length==2&&split[0].equals("PatientWeight"))
          {
            newUnit = split[1].substring(0, split[1].indexOf(')'));
            vRow.weight = resultData.get(result);
            for(int i=0; i<vRow.weight.size(); i++)
            {
              double newValue = UnitConverter.convert(vRow.weight.get(i), newUnit, vRow.weightUnit);
              vRow.weight.set(i, newValue);
            }
            // Since we did a conversion, let's update the key
            resultData.remove(result);
            result = split[0]+"("+vRow.weightUnit+")";
            resultData.put(result,vRow.weight);
            break;
          }
        }
        if(newUnit==null)// Nope, I don't think the result is there
        {
          vRow.error = danger+"PatientWeight, Not In Results!"+endSpan;
          return false;
        }
      } 
    }
    
    if(vRow.resultFile.indexOf('@')>-1)
    {
      // It's an assessment row
      for(String s : assessments.keySet())
      {
        if(s.indexOf(vRow.resultFile)>-1)
        {
          try
          {
            SEPatientAssessment a = assessments.get(s);
            Method m = a.getClass().getMethod("get"+vRow.name);
            Object r = m.invoke(a);
            if(r instanceof SEScalar)
            {
              SEScalar scalar = (SEScalar)m.invoke(a);
              if(!scalar.isValid())
              {
                vRow.error = danger+vRow.name+", Not Valid In Assessment!"+endSpan;
                return false;
              }
              vRow.results = new ArrayList<Double>();
              vRow.results.add(scalar.getValue(vRow.unit));
            }
            else if(r.getClass().isEnum())
            {// TODO will need to get the enum value at some point
              vRow.resultEnum = r.toString();
            }
            return true;
          }
          catch(Exception ex)
          {
            vRow.error = danger+vRow.name+", Not In Assessment!"+endSpan;
            return false;
          }
        }
      }
      vRow.error = danger+vRow.name+", was not found in Assessment!"+endSpan;
      return false;
    }
    
    vRow.results = getResults(vRow.name,vRow.unit);
    if(vRow.results==null && patient !=null)
      vRow.results = getResults("Patient"+vRow.name,vRow.unit);// See if this a patient output
    if(vRow.results==null)// Nope, I don't think the result is there
    {
      vRow.error = danger+"Not In Results!"+endSpan;
      return false;
    }
    return true;
  }
  
  protected List<Double> getResults(String name, String unit)
  {
    //if(name.equals("BloodDensity"))
    //  Log.info("Here");
    String header = name;
    if(unit!=null && !unit.isEmpty())
      header = name+"("+unit+")";
    List<Double> results = resultData.get(header);
    if(results==null)
    {// Maybe the result is in a different unit than our reference value
      String outputUnit=null;
      String[] split;
      for(String result : resultData.keySet())
      {
        split = result.split("[(]");
        if(split.length==2&&split[0].equals(name))
        {
        	outputUnit = split[1].substring(0, split[1].indexOf(')'));
          results = resultData.get(result);
          for(int i=0; i<results.size(); i++)
          {
          	if(!UnitConverter.isCompatibleWithUnit(outputUnit, unit))
          	{
          		Log.error(name + " has incompatible units between what was generated " + outputUnit + " and the validation unit " + unit);
          		return null;
          	}
            double newValue = UnitConverter.convert(results.get(i), outputUnit, unit);
            results.set(i, newValue);
          }
          // Since we did a conversion, let's update the key
          resultData.remove(result);
          result = split[0]+"("+unit+")";
          resultData.put(result,results);
          break;
        }
      }            
    } 
    return results;
  }

  protected void WriteDoxyTable(List<ValidationRow> vData, String sheetName, String destinationDirectory)
  {
    if(vData.isEmpty())
      return;
    String columnHeaders[] = new String[5];
    int maxColumnLength[] = new int[columnHeaders.length];
    columnHeaders[0] = "Property Name";
    columnHeaders[1] = "Expected Value";
    columnHeaders[2] = "Engine Value";
    columnHeaders[3] = "Percent Error";
    columnHeaders[4] = "Notes";
    for(int i=0; i<maxColumnLength.length; i++)
      maxColumnLength[i] = columnHeaders[i].length();
    
    // Compute all of our header values and sizes
    for(ValidationRow vRow : vData)
    {
      // Header (was built when we had to pull data out of the results file)
      if(vRow.header.length()>maxColumnLength[0])
        maxColumnLength[0] = vRow.header.length();
      if(vRow.expected.length()>maxColumnLength[1])
        maxColumnLength[1] = vRow.expected.length();
      if(vRow.engine.length()>maxColumnLength[2])
        maxColumnLength[2] = vRow.engine.length();
      if(vRow.error.length()>maxColumnLength[3])
        maxColumnLength[3] = vRow.error.length();
      if(vRow.notes.length()>maxColumnLength[4])
        maxColumnLength[4] = vRow.notes.length();
    }

    PrintWriter writer=null;
    try
    {
      // Create file and start the table
      writer = new PrintWriter(destinationDirectory+"/"+sheetName+"ValidationTable.md", "UTF-8");
      for(int i=0; i<columnHeaders.length; i++)
        writer.print("|"+pad(columnHeaders[i],maxColumnLength[i]));
      writer.println("|");
      for(int i=0; i<columnHeaders.length; i++)
        writer.print("|"+pad("---",maxColumnLength[i]));
      writer.println("|");
      // Now loop the vData and write out table rows
      for(ValidationRow vRow : vData)
      {
        writer.print("|"+pad(vRow.header,maxColumnLength[0]));
        writer.print("|"+pad(vRow.expected,maxColumnLength[1]));
        writer.print("|"+pad(vRow.engine,maxColumnLength[2]));
        writer.print("|"+pad(vRow.error,maxColumnLength[3]));
        writer.print("|"+pad(vRow.notes,maxColumnLength[4]));
        writer.println("|");
      }

      writer.close();
    }
    catch(Exception ex)
    {
      Log.error("Error writing validation table for "+sheetName,ex);
      writer.close();
    }
  }

  protected String pad(String s, int max)
  {
    if(s==null)
      return new String(new char[(max+5)]).replace('\0',' ');
    // Using a pad of 5 between columns
    try
    {
      String ret =  s + new String(new char[(max-s.length())+5]).replace('\0',' ');
      return ret;
    }
    catch(Exception ex)
    {
      Log.error("Could not pad "+s+" with a max of "+max,ex);
      return "";
    }
  }
  
  
  // TODO this should also cross check that we have everything accounted for with assessments too
  protected void CrossCheckValidationWithSchema(List<String> properties, List<ValidationRow> eData, String sheetName)
  {
    // Remove any * in the properties list
    for(int i=0; i<properties.size(); i++)
    {
      String p = properties.get(i);
      int a = p.indexOf('*');
      if(a>-1)
        properties.set(i, p.substring(0, a));
      }
    
    Class<?> c = null;
    try
    {
      if(TABLE_TYPE.equals("System"))
        c = Class.forName("mil.tatrc.physiology.datamodel.bind."+sheetName+"SystemData");
      else if(TABLE_TYPE.equalsIgnoreCase("Patient"))
        c = Class.forName("mil.tatrc.physiology.datamodel.bind.PatientData");
      else
        throw new RuntimeException("Unknown table type "+TABLE_TYPE);
    }
    catch(Exception ex)
    {
      Log.error("Cannot map sheet "+sheetName+" to a CDM class.",ex);
      return;
    }
    Set<String> cdm = new HashSet<String>();
    // Grab all scalar data property names from the bind class
    for(Method m : c.getMethods())
    {
      if(m.getName().startsWith("get")&&ScalarData.class.isAssignableFrom(m.getReturnType()))
      {
        boolean found=false;
        String prop = m.getName().substring(3);
        cdm.add(prop);
        for(String p : properties)
        {
          if(p.equals(prop))
          {
            found=true;
            break;
          }
        }
        if(!found)
        {// Missing a row for this property
          ValidationRow row = new ValidationRow();
          row.name = row.header = prop;
          row.error = danger+"Missing entry for property"+endSpan;
          eData.add(row);
        }
      }
    }
    
    for(String property : properties)
    {      
      if(!property.contains("-")&&!cdm.contains(property))
      {
        try
        {// Maybe this is a non Scalar method...
          c.getMethod("get"+property);
        }
        catch(Exception ex)
        {
          try
          {// Or a boolean method...
            c.getMethod("is"+property);            
          }
          catch(Exception e2)
          {
            ValidationRow row = new ValidationRow();
            row.name = row.header = property;
            row.error = danger+"Is not in the CDM"+endSpan;
            eData.add(row);
          }
        }
        
      }
    }
  }
  
  protected void WriteHTML(List<ValidationRow> vData, String tableName)
  {
    html.append("<br><br>");
    
    html.append("<table border=\"1\">");
    html.append("<tr>");
    html.append("<th> "+tableName+" </th>");      
    html.append("<th> Expected Value </th>");
    html.append("<th> Engine Value </th>");
    html.append("<th> Percent Error </th>");
    html.append("<th> Notes </th>");
    html.append("</tr>");
    
    for(ValidationRow vRow : vData)
    {
      if(vRow.error.contains(success))
        html.append("<tr bgcolor=\"#00FF00\">");
      else if(vRow.error.contains(warning))
        html.append("<tr bgcolor=\"#FFFF00\">");
      else if(vRow.error.contains(danger))
        html.append("<tr bgcolor=\"#FF0000\">");
      else
          html.append("<tr bgcolor=\"#FFFFFF\">");
      html.append("<td>"+vRow.header+"</td>");
      html.append("<td>"+vRow.expected+"</td>");
      html.append("<td>"+vRow.engine+"</td>");
      html.append("<td>"+vRow.error+"</td>");
      html.append("<td>"+vRow.notes+"</td>");    
    }
    html.append("</table>");
  }
}
