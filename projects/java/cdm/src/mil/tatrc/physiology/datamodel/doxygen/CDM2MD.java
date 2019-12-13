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
package mil.tatrc.physiology.datamodel.doxygen;

import java.io.IOException;
import java.io.PrintWriter;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.*;


import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.FindObjects;
import mil.tatrc.physiology.utilities.FindObjects.BagMethod;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.StringUtils;


/*
*  Last Edited: lmarin, June 2019
*/


import mil.tatrc.physiology.datamodel.bind.*;

/**
 * This class will generate markdown tables
 * for some important cdm data structures
 * for use in our documentation
 */
public class CDM2MD
{

    public static void main(String[] args)
    {
        try
        {
      String destDir = "./doc/doxygen/processed_md/"; // This might be a bug, I don't know if he meant to put it in doc/markdown
            FileUtils.createDirectory(destDir);

      PrintWriter writer=new PrintWriter(destDir+"CDMTables.md", "UTF-8");
            
            writer.append("CDM Tables {#CDMTables}\n");
            writer.append("=======================\n");
            
            List<String> skipProperties = new ArrayList<String>();
      //skipProperties.add("Comment");
      //skipProperties.add("ScenarioTime");
            
      Set<Class<? extends Object>> sce = FindObjects.findAllClasses("mil.tatrc.physiology.datamodel.bind");
      for(Class<?> c : sce) {
        if( c.getSimpleName().startsWith("Scalar")
            || c.getSimpleName() == "PropertyData"

         ){ continue;}
        WriteDoxyTable(c, "CDM", writer, skipProperties);
      }
            writer.close();

        } 
        catch (Exception e)
        {
            Log.error("Could not create directory .markdown",e);
            return;
        }
    }
    
    protected static void WriteDoxyTable(Class<?> c, String xsdFile, PrintWriter writer, List<String> skipProperties)
  {     
        String tableName = c.getSimpleName();
    if(tableName.startsWith("Enum")){
            tableName = tableName.substring(4);
    }
    String columnHeaders[] = new String[3];
    int maxColumnLength[] = new int[columnHeaders.length];
    columnHeaders[0] = "Property Name";
    columnHeaders[1] = "Type";
    columnHeaders[2] = "Definition";
    for(int i=0; i<maxColumnLength.length; i++)
      maxColumnLength[i] = columnHeaders[i].length();
    
    List<BagMethod> bagMethods = FindObjects.getBagMethods(c,skipProperties);
    if(c.isEnum())
    {
        for(Object o : c.getEnumConstants())
        {
            Method m;
            Enum<?> e = (Enum<?>)o;
            try
                {
                BagMethod bag = new BagMethod();
                     m = o.getClass().getMethod("value");
                     bag.propertyName = (String) m.invoke(o);
                    bag.returnType = c;
                    bagMethods.add(bag);
                } 
            catch (Exception ex)
                {
                    Log.info("Enum is not happy",ex);
                }
        }
    }
    
    // Compute all of our header values and sizes
    for(BagMethod bag : bagMethods)
    {
      // Header (was built when we had to pull data out of the results file)
      if(bag.propertyName.length()>maxColumnLength[0])
        maxColumnLength[0] = bag.propertyName.length();
      if(bag.returnType.getSimpleName().length()>maxColumnLength[1])
        maxColumnLength[1] = bag.returnType.getSimpleName().length();           
    }
    String descPrepend;
    if(c.isEnum()){ 
      descPrepend = "@copybrief mil.tatric.physiology::datamodel::enum"+tableName; }
    else{ 
      descPrepend = "@copybrief biogears::SE"+tableName.substring(0, tableName.length()-4); 
    }
    maxColumnLength[2] = descPrepend.length()+maxColumnLength[0];
    
    try
    {
      // Create file and start the table        
        writer.println("");
        writer.println("@anchor "+StringUtils.removeSpaces(tableName)+"Table");
      writer.println("## "+ tableName);
        writer.println(descPrepend+"");
        
        if(!bagMethods.isEmpty())
        {
            for(int i=0; i<columnHeaders.length; i++)
                writer.print("|"+pad(columnHeaders[i],maxColumnLength[i]));
            writer.println("|");
            for(int i=0; i<columnHeaders.length; i++)
                writer.print("|"+pad("---",maxColumnLength[i]));
            writer.println("|");
            // Now loop the vData and write out table rows
            for(BagMethod bag : bagMethods)
            {
                writer.print("|"+pad(bag.propertyName,maxColumnLength[0]));
                if(bag.returnType.getSimpleName().equals("List"))
                {
                    // If it is a list, I am doing this special code, not generic smart yet
                    if(bag.propertyName.equals("AmbientSubstance"))
                    {
                        writer.print("|"+"List of SubstanceFractions");
                        writer.print("|"+"@ref SubstanceFractionTable");
                    }
                    else if(bag.propertyName.equals("Commands"))
                    {
                        writer.print("|"+"List of Commands");
                        String tables = "@ref BreathHoldTable <br> @ref ForcedExhaleTable <br> @ref ForcedInhaleTable <br> @ref UseInhalerTable";
                        writer.print("|"+tables);
                    }
                    else if(bag.propertyName.equals("SubstanceQuantities") && tableName.startsWith("Gas"))
                    {
                        writer.print("|"+"List of SEGasSubstanceQuantity");
                        writer.print("|"+"@ref GasSubstanceQuantityTable");
                    }
                    else if(bag.propertyName.equals("SubstanceQuantities") && tableName.startsWith("Liquid"))
                    {
                        writer.print("|"+"List of SELiquidSubstanceQuantity");
                        writer.print("|"+"@ref LiquidSubstanceQuantityTable");
                    }
                    else if(bag.propertyName.equals("Components"))
                    {
                        writer.print("|"+"List of SESubstanceCompoundComponent");
                        writer.print("|"+"@ref SubstanceCompoundComponentTable");
                    }
                    else if(bag.propertyName.equals("GasFraction")) 
                    { 
                        writer.print("|"+"List of GasFraction");
                        writer.print("|"+"@ref MechanicalVentilationTable");
                    }
                    else if(bag.propertyName.equals("AmbientGas"))
                    {
                        writer.print("|"+"List of AmbientGas");
                        writer.print("|"+"@ref EnvironmentalConditionsTable");
                    }                   
                    else if(bag.propertyName.equals("AmbientAerosol"))
                    {
                        writer.print("|"+"List of AmbientAerosol");
                        writer.print("|"+"@ref EnvironmentalConditionsTable");
                    }
                    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    else
              writer.print("|"+pad("List of SE" + bag.propertyName + "s",maxColumnLength[1]));
              writer.print("|"+pad("@ref " + bag.propertyName + "Table",maxColumnLength[2]));
              //Log.error("Unsupported List type for :"+bag.propertyName+" on table "+tableName);
                    
                }
                else
                {
                    writer.print("|"+pad(bag.returnType.getSimpleName(),maxColumnLength[1]));
            if(
                Enum.class.isAssignableFrom(bag.returnType) 
                || String.class.isAssignableFrom(bag.returnType) 
                || bag.returnType.getSimpleName().startsWith("Scalar") 
                || bag.returnType.getSimpleName().startsWith("Function") 
                )
            {
                  writer.print("|"+pad(descPrepend+"::Get"+ bag.propertyName.replace("_","::"),maxColumnLength[2]));
            }
                    else
                    {
                        String refTable = bag.returnType.getSimpleName();
                        writer.print("|"+pad("@ref "+refTable+"Table",maxColumnLength[2]));
                    }
                }
                writer.println("|");
            }   
        }
      writer.println("");
    }
    catch(Exception ex)
    {
      Log.error("Error writing cdm table for "+tableName,ex);      
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
      //Log.error("Could not pad "+s+" with a max of "+max,ex);
      return s;
    }
  }

}
