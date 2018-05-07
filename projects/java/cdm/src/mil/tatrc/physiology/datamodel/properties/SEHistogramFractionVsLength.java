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
package mil.tatrc.physiology.datamodel.properties;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.HistogramFractionVsLengthData;
import mil.tatrc.physiology.datamodel.exceptions.InvalidUnitException;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.LengthUnit;


/**
 * @author abray
 */
public class SEHistogramFractionVsLength extends SEHistogram
{
  public SEHistogramFractionVsLength()
  {
    super();
  }
    
  public HistogramFractionVsLengthData unload()
  {
    if(!isValid())
      return null;
    HistogramFractionVsLengthData to = CDMSerializer.objFactory.createHistogramFractionVsLengthData();
    unload(to);
    return to;
  }

  public boolean isValidDependentUnit(String unit)
  {
    if(unit == null || unit.isEmpty())
    	return true;
    return false;
  }
  
  public boolean isValidIndependentUnit(String unit)
  {
    return LengthUnit.validUnit(unit);
  }
  
  public String getLengthUnit()
  {
    return this.independentUnit;
  }
 
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setFraction(double[] data)
  {
    setDependent(data);
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setFraction(String data)
  {
    setDependent(data);
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public double[] getFraction()
  {
    return getDependent();
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public String getFractionString()
  {
    return getDependentString();
  }
  
  // Independent //
  
  public double[] getLength(String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Length");
    return getIndependent(unit);
  }
  
  public String getLengthString(String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Length");
    return getIndependentString(unit);
  }
  
  public void setLength(double[] data, String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Length");
    setIndependent(data, unit);
  }
  
  public void setLength(String data, String unit)
  {
    if(!isValidIndependentUnit(unit))
      throw new InvalidUnitException(unit, "Length");
    setIndependent(data, unit);
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public double[] getLength(LengthUnit unit)
  {
    return getIndependent(unit.toString());
  }
  
  /**
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public String getLengthString(LengthUnit unit)
  {
    return getIndependentString(unit.toString());
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setLength(double[] data, LengthUnit unit)
  {
    setIndependent(data, unit.toString());
  }
  
  /**
   * @param data
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention to set units as a String
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setLength(String data, LengthUnit unit)
  {
    setIndependent(data, unit.toString());
  }
}
