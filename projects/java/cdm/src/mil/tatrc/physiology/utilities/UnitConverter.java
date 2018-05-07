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

import java.io.File;
import java.io.IOException;
import java.nio.file.*;
import static java.nio.file.StandardCopyOption.*;

import java.util.ArrayList;
import java.util.List;
import mil.tatrc.physiology.datamodel.exceptions.InvalidUnitException;

/**
 * UnitConverter class that implements Unit Conversion using
 * UnitConversionJavaSupport DLL support
 * 
 * @author abray
 */
public class UnitConverter
{
  /** Loads Unit Conversion DLL */
  static
  {
    String UCEDefsPath = System.getProperty("user.dir") + "/UCEDefs.txt";
    String UCEDefsHomePath = System.getProperty("user.home") + "/UCEDefs.txt";
    
    try{
            Files.copy(
              new File(UCEDefsPath).toPath()
              , new File(UCEDefsHomePath).toPath()
              , REPLACE_EXISTING);
    }
    catch (IOException e)
    {
      System.err.println("Unable to locate UCEDefs.txt");
    }

    Resources resources = new Resources();
    System.out.println("Attempting to load release libraries");
    if (!FileUtils.loadLibraries(resources.getLibraries()))
    {
      System.err.println("Unable to find the following JNI Libraries." + resources.getLibraries().toString());
      System.out.println("Attempting to load debug libraries");
      if (!FileUtils.loadLibraries(resources.getDebugLibraries()))
      { 
        System.err.println("Unable to find the following Debug JNI Libraries." + resources.getDebugLibraries().toString());

      }
    }
  }
  
  /**
   * Optional initialize method
   * @param workingDirectory
   * @return
   */
  public static boolean initialize(String workingDirectory)
  {
    File workingDirectoryFile = new File(workingDirectory).getAbsoluteFile();
    File f = new File(workingDirectoryFile.getAbsolutePath() + "/UCEDefs.txt").getAbsoluteFile();
    
    if(!f.exists())
    {
      //fLog.error("Unable to find "+workingDirectory+"/UCEDefs.txt");
      return false;
    }

    nativeInitialize(workingDirectory);

    return true;
  }
  private static native void nativeInitialize(String workingDirectory);
  
  /**
   * Converts values from one unit to the other unit
   * 
   * @param value - original value
   * @param fromUnit - original unit
   * @param toUnit - new unit
   * @return - new value
   */
  public static double convert(double value, String fromUnit, String toUnit)// throws InvalidUnitsException
  {
    if(Double.isNaN(value))
    {
      //fLog.error("Conversion Input is NaN");
      return value;
    }
    if(fromUnit.equals(toUnit))
      return value;
    if(fromUnit.isEmpty()||toUnit.isEmpty())
    {
      //fLog.warn("No unit supplied to convertions, returning input");
      return value;
    }
    double d = nativeConvert(value, fromUnit, toUnit);
    //if(Double.isNaN(d))
      //fLog.error("Conversion equals NaN");
    return d;
  }
  /** nativeConvert method */
  private static native double nativeConvert(double value, String fromUnit, String toUnit);
  
  public static synchronized double[] convert(double[] data, String fromUnits, String toUnits)
  {
    if(fromUnits.equals(toUnits))
      return data;// TODO return a copy?

    if(!isCompatibleWithUnit(fromUnits, toUnits))
      throw new InvalidUnitException(fromUnits,toUnits);
    
    double[] tmp=new double[data.length];
    for(int i = 0; i < data.length; i++)
    {
      tmp[i]=convert(data[i], fromUnits, toUnits);
    }
    return tmp;
  }
  
  
  /**
   * Determines if units are compatible with each other 
   * 
   * @param unitTemplates - unit that we want to be like
   * @param unit - the unit in question
   * @return - boolean
   */
  public static boolean isCompatibleWithUnit(String unitTemplate, String unit)
  {
    if(unitTemplate==null)
      return false;
    if(unit.equals(unitTemplate))
      return true;
    return nativeIsCompatibleWithUnit(unitTemplate, unit);
  }
  
  /**
   * Determines if unit are compatible with an array of units 
   * 
   * @param unitTemplates - array of standard units
   * @param unit - unit comparing to
   * @return - boolean
   */
  public static boolean isCompatibleWithUnitList(String[] unitTemplates, String unit)
  {
    if(unitTemplates==null)
      return false;
    for(int i=0;i<unitTemplates.length;i++)
    {
      if(unit.equals(unitTemplates[i]))
        return true;
    }
    return nativeIsCompatibleWithUnitList(unitTemplates, unit);
  }
  /** nativeIsCompatibleWithUnitInList method */
  private static native boolean nativeIsCompatibleWithUnit(String unitTemplate, String unit);
  private static native boolean nativeIsCompatibleWithUnitList(String[] unitTemplates, String unit);
  
  /**
   * Determines if the provided unit is part of the 
   * specified unit quantity type
   * i.e. is MeV an Energy Unit
   * 
   * @param unitQuantity - unit quantity that we want to be like
   * @param unit - the unit in question
   * @return - boolean
   */
  public static boolean isOfQuantityType(String templateUnit, String unit)
  {
    if(templateUnit==null)
      return false;
    if(templateUnit.equals(unit))
      return true;
    return nativeIsOfQuantityType(templateUnit, unit);
  }
  
  /**
   * Determines if the provided unit is part of the 
   * specified unit quantity types
   * i.e. is MeV an Energy Unit
   * 
   * @param unitTemplates - array of standard units
   * @param unit - unit comparing to
   * @return - boolean
   */
  public static boolean isOfQuantityTypes(String[] templateUnits, String unit)
  {
    if(templateUnits==null||templateUnits.length==0)
      return false;
    if(unit==null)
      return false;
    for(String u : templateUnits)
    {
      if(unit.equals(u))
        return true;
    }
    return nativeIsOfQuantityTypes(templateUnits, unit);
  }
  /** nativeIsCompatibleWithUnitInList method */
  private static native boolean nativeIsOfQuantityType(String unitQuantity, String unit);
  private static native boolean nativeIsOfQuantityTypes(String[] unitQuantities, String unit);
  
}
