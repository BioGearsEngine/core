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
import mil.tatrc.physiology.datamodel.bind.ScalarHeatCapacitancePerMassData;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.HeatCapacitancePerMass;

/**
 * @author abray
 * HeatConductancePerArea Scalar Class, will enforce that units are proper to HeatConductancePerAreas
 */

public class SEScalarHeatCapacitancePerMass extends SEScalar
{
  public SEScalarHeatCapacitancePerMass()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarHeatCapacitancePerMass(double value, String unit)
  {
  	this();
  	this.setValue(value, unit);
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public SEScalarHeatCapacitancePerMass(double value, HeatCapacitancePerMass unit)
  {
    this();
    this.setValue(value,unit);
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public void setValue(double value, HeatCapacitancePerMass unit)
  {
    this.setValue(value,unit.toString());
  }
  
  /**
   * @param value
   * @param unit - enumeration of commonly used units for this type
   * Units are not limited to this set.
   * You can use the other method convention:(double,string)
   * and specify any valid standard unit abbreviation sting for this type
   * Unit Abbreviation Standards: http://www.bipm.org/en/si/
   */
  public double getValue(HeatCapacitancePerMass unit)
  {
    return this.getValue(unit.toString());
  }
  
  

  public ScalarHeatCapacitancePerMassData unload()
  {
    if(!this.isValid())
      return null;

    ScalarHeatCapacitancePerMassData to = CDMSerializer.objFactory.createScalarHeatCapacitancePerMassData();
    unload(to);
    return to;
  }

  public boolean validUnit(String unit)
  {
    if(HeatCapacitancePerMass.validUnit(unit))
      return true;
    return false;
  }
}
