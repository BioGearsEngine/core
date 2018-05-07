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
import mil.tatrc.physiology.datamodel.bind.ScalarFractionData;

/**
 * @author abray
 * Fraction Scalar Class, will enforce that units are proper to Fractions
 */

public class SEScalarFraction extends SEScalar
{
  public SEScalarFraction()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalarFraction(double value)
  {
  	this();
  	this.setValue(value, "");
  }
  

  public ScalarFractionData unload()
  {
    if(!this.isValid())
      return null;

    ScalarFractionData to = CDMSerializer.objFactory.createScalarFractionData();
    unload(to);
    return to;
  }

  public boolean validUnit(String unit)
  {
    if(unit==null||unit.isEmpty()||unit.compareTo("unitless") == 0)
      return true;
    return false;
  }
}
