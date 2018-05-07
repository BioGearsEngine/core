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

import java.util.Arrays;
import java.util.List;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.SEEqualOptions;
import mil.tatrc.physiology.datamodel.bind.HistogramData;
import mil.tatrc.physiology.datamodel.exceptions.InvalidUnitException;
import mil.tatrc.physiology.utilities.DoubleUtils;
import mil.tatrc.physiology.utilities.StringUtils;
import mil.tatrc.physiology.utilities.UnitConverter;

/**
 * @author abray
 */
public class SEHistogram extends SEFunction
{
  

  public SEHistogram()
  {
    super();
    invalidate();
  }
  
  public boolean loadData(Object data) throws ClassCastException
  {
    if (data instanceof HistogramData)
      return load((HistogramData) data);
    else if (data instanceof SEHistogram)
      return set((SEHistogram) data);
    return false;
  }

  public boolean load(HistogramData in)
  {
  	if(!isValidDependentUnit(in.getDependentUnit()))
  		return false;
  	if(!isValidIndependentUnit(in.getIndependentUnit()))
  		return false;
    this.dependentUnit=in.getDependentUnit();
    this.dependent=SEArray.toArray(in.getDependent().getDoubleList());
    
    this.independentUnit=in.getIndependentUnit();
    this.independent=SEArray.toArray(in.getIndependent().getDoubleList());
    return this.isValid();
  }

  public Object unloadData()
  {
    return unload();
  }
  
  public HistogramData unload()
  {
    if(!this.isValid())
      return null;
    HistogramData to = CDMSerializer.objFactory.createHistogramData();
    unload(to);
    return to;
  }

  protected void unload(HistogramData to)
  {
    to.setDependentUnit(this.dependentUnit);
    to.setDependent(CDMSerializer.objFactory.createDoubleArray());
    SEArray.toList(to.getDependent().getDoubleList(),this.dependent);
    
    to.setIndependentUnit(this.independentUnit);
    to.setIndependent(CDMSerializer.objFactory.createDoubleArray());
    SEArray.toList(to.getIndependent().getDoubleList(),this.independent);
  }
  
  public boolean isValid()
  {
    if(this.dependent.length==0||this.independent.length==0)
      return false;
    if(this.dependent.length+1 != this.independent.length)
      return false;
    if(!isValidDependentUnit(this.dependentUnit))
    	return false;
    if(!isValidIndependentUnit(this.independentUnit))
    	return false;
    return true;
  }
}
