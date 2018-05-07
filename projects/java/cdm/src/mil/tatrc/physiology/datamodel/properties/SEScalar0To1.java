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

import org.jfree.util.Log;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.Scalar0To1Data;

/**
 * @author abray
 * 0To1 Scalar Class, will enforce that value is between 0 and 1 (inclusive)
 */

public class SEScalar0To1 extends SEScalar
{
  public SEScalar0To1()
  {
    super();
  }
  
  /**
   * @param value
   * @param unit - specific any valid standard unit abbreviation
   * http://www.bipm.org/en/si/ for this quantity type
   */
  public SEScalar0To1(double value)
  {
  	this();
  	this.setValue(value, "");
  }
  

  public Scalar0To1Data unload()
  {
    if(!this.isValid())
      return null;

    Scalar0To1Data to = CDMSerializer.objFactory.createScalar0To1Data();
    unload(to);
    return to;
  }

  public boolean validUnit(String unit)
  {
    if(unit==null||unit.isEmpty()||unit.compareTo("unitless") == 0)
      return true;
    return false;
  }
  
  public void setValue(double d)
  {
  	if(d>1)
  	{
  		d=1;
  		Log.warn("Attempting to set 0-1 Scalar to greater than 1, bounding to 1");
  	}
  	else if(d<0)
  	{
  		d=0;
  		Log.warn("Attempting to set 0-1 Scalar to less than 0, bounding to 0");
  	}
  	this.value = d;
  }
}
