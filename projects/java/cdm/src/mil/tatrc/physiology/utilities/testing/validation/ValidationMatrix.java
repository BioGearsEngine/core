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

import java.io.*;
import java.util.*;

import mil.tatrc.physiology.utilities.Log;

import org.apache.poi.xssf.usermodel.XSSFCell;
import org.apache.poi.xssf.usermodel.XSSFColor;
import org.apache.poi.xssf.usermodel.XSSFRow;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

public class ValidationMatrix
{
  protected static final String success = "<span class=\"success\">";
  protected static final String warning = "<span class=\"warning\">";
  protected static final String danger = "<span class=\"danger\">";
  protected static final String endSpan = "</span>";
  
  public static void main(String[] args) throws Exception
  {
    String from = "doc/Validation/Scenarios/";
    String to = "./validation/";
    
    if(args.length==2)
    {
      from = args[0];
      to = args[1];
    }
    
    File dir = new File(from);
    if(!dir.exists()||!dir.isDirectory())
    {
      Log.error("Source directory does not exists or is not a directory");
      return;
    }
    
    for(File f : dir.listFiles())
      ValidationMatrix.convert(f.getAbsolutePath(), to);
  }
  
  enum Agreement {NA,Good,Ok,Bad};
  
  public static class SheetSummary
  {
    public String name;
    public String description;
    public String validationType;
    public short  goodAgreement=0;
    public short  okAgreement=0;
    public short  badAgreement=0;
  }
  public static class Sheet 
  {    
    public SheetSummary summary;
    public List<List<Cell>> table = new ArrayList<List<Cell>>();
  }
  public static class Cell
  {
    public Cell(String text, Agreement agreement, Map<String,String> refs)
    {      
      this.agreement = agreement; 
      this.text = text;
      if(this.text == null || this.text.isEmpty())
        return;
      this.text = this.text.replaceAll("\n", " ");
      // <br> in tables don't do anything
      //this.text = this.text.replaceAll(";", "<br>");
      //this.text = this.text.replaceAll(".", "<br>");
      switch(this.agreement)
      {
        case Good:
          this.text = success + this.text + endSpan;
          break;
        case Ok:
          this.text = warning + this.text + endSpan;
          break;
        case Bad:
          this.text = danger + this.text + endSpan;
          break;
      }
      int i=0;
      for(String key : refs.keySet())
        this.text = this.text.replaceAll(key, refs.get(key));
    }
    public String text;
    public Agreement agreement;
  }
  
  public static void convert(String from, String to) throws IOException
  {
    FileInputStream xlFile = new FileInputStream(new File(from));
    // Read workbook into HSSFWorkbook
    XSSFWorkbook xlWBook = new XSSFWorkbook(xlFile);
    List<SheetSummary> sheetSummaries = new ArrayList<SheetSummary>();// has to be an ordered list as sheet names can only be so long
    Map<String,String> refs = new HashMap<String,String>();
    
    List<Sheet> Sheets = new ArrayList<Sheet>();
    
    for(int s=0; s<xlWBook.getNumberOfSheets(); s++)
    {
      XSSFSheet xlSheet = xlWBook.getSheetAt(s);
      Log.info("Processing Sheet : " + xlSheet.getSheetName());
      if(xlSheet.getSheetName().equals("Summary"))
      {
        int rows = xlSheet.getPhysicalNumberOfRows(); 
        for (int r = 1; r < rows; r++) 
        {
          XSSFRow row = xlSheet.getRow(r);
          if (row == null ) 
            continue;
          SheetSummary ss = new SheetSummary();
          sheetSummaries.add(ss);
          ss.name = row.getCell(0).getStringCellValue();
          ss.description = row.getCell(1).getStringCellValue();
          ss.validationType = row.getCell(2).getStringCellValue();
        }
      }
      else if(xlSheet.getSheetName().equals("References"))
      {
        int rows = xlSheet.getPhysicalNumberOfRows(); 
        for (int r = 1; r < rows; r++) 
        {
          XSSFRow row = xlSheet.getRow(r);
          if (row == null ) 
            continue;
          refs.put("\\["+r+"\\]", "@cite "+row.getCell(1).getStringCellValue());
        }
      }
      else
      {
        int rows = xlSheet.getPhysicalNumberOfRows(); 
        Sheet sheet = new Sheet();
        sheet.summary = sheetSummaries.get(s-2);
        Sheets.add(sheet);

        int cells = xlSheet.getRow(0).getPhysicalNumberOfCells();  

        for (int r = 0; r < rows; r++) 
        {
          XSSFRow row = xlSheet.getRow(r);
          if (row == null ) 
            continue;

          String cellValue=null;

          for (int c = 0; c < cells; c++) 
          {
            List<Cell> column;
            if(r==0)
            {
              column = new ArrayList<Cell>();
              sheet.table.add(column);
            }
            else
            {
              column = sheet.table.get(c);
            }

            XSSFCell cell = row.getCell(c);
            if(cell==null)
            {
              column.add(new Cell("",Agreement.NA,refs));
              continue;
            }
            cellValue=null;
            switch(cell.getCellType())
            {          
              case XSSFCell.CELL_TYPE_NUMERIC:
                cellValue = Double.toString(cell.getNumericCellValue());
                break;
              case XSSFCell.CELL_TYPE_STRING:
                cellValue = cell.getStringCellValue();
                break;
            }
            if(cellValue==null||cellValue.isEmpty())
              column.add(new Cell("",Agreement.NA,refs));
            else
            {
              Agreement a = Agreement.NA;
              XSSFColor color = cell.getCellStyle().getFillForegroundColorColor();
              if(color!=null)
              {
                byte[] rgb = color.getRGB();
                if(rgb[0]<-25 && rgb[1]>-25 && rgb[2]<-25)
                {
                  a = Agreement.Good;
                  sheet.summary.goodAgreement++;
                }
                else if(rgb[0]>-25 && rgb[1]>-25 && rgb[2]<-25)
                {
                  a = Agreement.Ok;
                  sheet.summary.okAgreement++;
                }
                else if(rgb[0]>-25 && rgb[1]<-25 && rgb[2]<-25)
                {
                  a = Agreement.Bad;
                  sheet.summary.badAgreement++;
                }
              }              
              column.add(new Cell(cellValue,a,refs));             
            }
          }
        }
      }        
    }
    xlWBook.close();
    xlFile.close(); //close xls
    
    
    // Generate our Tables for each Sheet
    PrintWriter writer=null;
    try
    {
      String name = from.substring(from.lastIndexOf('/')+1,from.lastIndexOf('.'))+"Scenarios";
      
      writer = new PrintWriter(to+name+"Summary.md", "UTF-8");      
      writer.println("|Scenario|Description|Validation Type|Good agreement|General agreement with deviations|Some major disagreements|");
      writer.println("|--- |--- |:---: |:---: |:---: |:---: |");
      for(Sheet sheet : Sheets)
      {
        writer.println("|"+sheet.summary.name+"|"+sheet.summary.description+"|"+sheet.summary.validationType+"|"+success+sheet.summary.goodAgreement+endSpan+"|"+warning+sheet.summary.okAgreement+endSpan+"|"+danger+sheet.summary.badAgreement+endSpan+"|");
      }
      writer.close();
      
      // Create file and start the table
      writer = new PrintWriter(to+name+".md", "UTF-8");
      writer.println(name+" {#"+name+"}");
      writer.println("=======");
      writer.println();
      
     
      writer.println();
      
      for(Sheet sheet : Sheets)
      {       
        Log.info("Writing table : "+sheet.summary.name);
        writer.println("## "+ sheet.summary.name);

        writer.println(sheet.summary.description);
        writer.println("We used a "+sheet.summary.validationType+" validation method(s).");
        writer.println("");

        for(int row=0; row<sheet.table.get(0).size(); row++)
        {
          for(int col=0; col<sheet.table.size(); col++)
          {
            writer.print("|"+sheet.table.get(col).get(row).text);
          }
          writer.println("|");
          if(row==0)
          {
            for(int col=0; col<sheet.table.size(); col++)
            {
              writer.print("|---   ");
            }
            writer.println("|");
          }          
        }
        writer.println();
        writer.println();
      }
      writer.close();
    }
    catch(Exception ex)
    {
      Log.error("Error writing tables for "+from,ex);
      writer.close();
    }
  }
  
  protected static String pad(String s, int max)
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
  
}
