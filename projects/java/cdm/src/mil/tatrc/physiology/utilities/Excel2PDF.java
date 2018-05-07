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

import java.io.*;

import org.apache.poi.xssf.usermodel.XSSFRow;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;


import org.apache.pdfbox.pdmodel.PDDocument;
import org.apache.pdfbox.pdmodel.PDPage;
import org.apache.pdfbox.pdmodel.PDPageContentStream;
import org.apache.pdfbox.pdmodel.common.PDRectangle;
import org.apache.pdfbox.pdmodel.font.PDType1Font;
import org.apache.pdfbox.util.Matrix;

public class Excel2PDF
{
  public static void main(String[] args) throws Exception
  {
    Excel2PDF.convert("doc/Validation/Scenarios/AnesthesiaMachineValidation.xlsx", "AnesthesiaMachineValidation.pdf");
  }
  
  // https://svn.apache.org/viewvc/pdfbox/trunk/examples/src/main/java/org/apache/pdfbox/examples/pdmodel/
  // http://javamoody.blogspot.com/2014/04/creating-paginated-table-with-pdfbox.html
  
  public static void convert(String from, String to) throws IOException
  {
    FileInputStream xlFile = new FileInputStream(new File(from));
    // Read workbook into HSSFWorkbook
    XSSFWorkbook xlWBook = new XSSFWorkbook(xlFile); 
    //We will create output PDF document objects at this point
    PDDocument  pdf = new PDDocument ();
    
    //pdf.addTitle();
    for(int s=0; s<xlWBook.getNumberOfSheets(); s++)
    {
      XSSFSheet xlSheet = xlWBook.getSheetAt(s);
      Log.info("Processing Sheet : " + xlSheet.getSheetName());
      PDPage page = new PDPage(PDRectangle.A4);
      page.setRotation(90);
      pdf.addPage(page);
      PDRectangle pageSize = page.getMediaBox();
      PDPageContentStream contents = new PDPageContentStream(pdf, page);
      contents.transform(new Matrix(0, 1, -1, 0, pageSize.getWidth(), 0));// including a translation of pageWidth to use the lower left corner as 0,0 reference
      contents.setFont(PDType1Font.HELVETICA_BOLD, 16);
      contents.beginText();      
      contents.newLineAtOffset(50,pageSize.getWidth()-50);
      contents.showText(xlSheet.getSheetName());
      contents.endText();
      contents.close();
      
      
      int rows = xlSheet.getPhysicalNumberOfRows();     
      for (int r = 0; r < rows; r++) 
      {
        XSSFRow row = xlSheet.getRow(r);
        if (row == null ) 
          continue;
        int cells = row.getPhysicalNumberOfCells();        
        if(cells == 0)
          continue;// Add an empty Roe
                
      }        
    }

/*    
    //We will use the object below to dynamically add new data to the table
    PdfPCell table_cell;
    //Loop through rows.
    while(rowIterator.hasNext()) 
    {
      Row row = rowIterator.next(); 
      Iterator<Cell> cellIterator = row.cellIterator();
      while(cellIterator.hasNext()) 
      {
        Cell cell = cellIterator.next(); //Fetch CELL
        switch(cell.getCellType()) 
        { //Identify CELL type
          //you need to add more code here based on
          //your requirement / transformations
          case Cell.CELL_TYPE_STRING:
            //Push the data from Excel to PDF Cell
            table_cell=new PdfPCell(new Phrase(cell.getStringCellValue()));
            //feel free to move the code below to suit to your needs
            my_table.addCell(table_cell);
            break;
        }
        //next line
      }
    }
*/              
    pdf.save(new File(to));
    pdf.close();      
    xlWBook.close();
    xlFile.close(); //close xls
  }
  
  
  public class Column 
  {
    private String name;
    private float width;

    public Column(String name, float width) 
    {
        this.name = name;
        this.width = width;
    }
    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
    public float getWidth() { return width; }
    public void setWidth(float width) { this.width = width; }
  }
}
