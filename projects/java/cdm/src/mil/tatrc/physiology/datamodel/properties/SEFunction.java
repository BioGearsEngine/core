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

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.SEEqualOptions;
import mil.tatrc.physiology.datamodel.bind.FunctionData;
import mil.tatrc.physiology.datamodel.exceptions.InvalidUnitException;
import mil.tatrc.physiology.utilities.DoubleUtils;
import mil.tatrc.physiology.utilities.StringUtils;
import mil.tatrc.physiology.utilities.UnitConverter;

/**
 * @author abray
 */
public class SEFunction
{
  protected double[] dependent;
  protected double[] independent;
  protected String   dependentUnit;
  protected String   independentUnit;

  public SEFunction()
  {
    super();
    invalidate();
  }

  public boolean set(SEFunction ary)
  {
  	if(!ary.isValid())
  		return false;
    this.dependentUnit = ary.dependentUnit;
    this.dependent = Arrays.copyOf(ary.dependent,ary.dependent.length);
    this.independentUnit = ary.independentUnit;
    this.independent = Arrays.copyOf(ary.independent,ary.independent.length);
    return true;
  }
  
  public static synchronized boolean equals(double[] from, double[] to)
  {
    return equals(from, "", to, "", null);
  }
  public static synchronized boolean equals(double[] from, double[] to, SEEqualOptions options)
  {
    return equals(from, "", to, "", options);
  }
  public static synchronized boolean equals(double[] from, double[] to, SEEqualOptions options, String name)
  {
    return equals(from, "", to, "", options, name);
  }
  public static synchronized boolean equals(double[] from, String fromUnits, double[] to, String toUnits, SEEqualOptions options)
  {
    return equals(from, fromUnits, to, toUnits, options, null);
  }
  public static synchronized boolean equals(double[] from, String fromUnits, double[] to, String toUnits, SEEqualOptions options, String name)
  {
    if (from == to)
      return true;
    if (from == null || to == null)
      return false;

    if (name == null)
      name = "SEArray";

    if (from == null || to == null)
    {
      if (options != null)
        options.track(name, from, to);
      return false;
    }

    boolean equals = true;
    boolean forceReturn = true;
    if (options != null)
    {
      options.pushTrack(name);
      forceReturn = options.returnOnFirstDiff;
    }

    if (from.length != to.length)
    {
      if (options != null)
      {
        options.track("length", from.length, to.length);
        options.trackToMap();
      }
      return false;
    }

    if (!UnitConverter.isCompatibleWithUnit(fromUnits, toUnits))
    {
      if (options != null)
      {
        options.track("units incompatible", fromUnits, toUnits);
        options.trackToMap();
      }
      return false;
    }

    for (int i = 0; i < to.length; i++)
    {
      if (!DoubleUtils.equals(from[i], fromUnits, to[i], toUnits, options))
      {
        if (options != null)
          options.track("Array[" + i + "]", from[i], fromUnits, to[i], toUnits);
        if (forceReturn)
          return false;
        equals = false;
      }
    }

    if (options != null)
      options.trackToMap();
    return equals;
  }
  
  public int hashCode()
  {
  	int code=1;
    if(!this.isValid())
    	return 1;
    code += this.dependent.hashCode();
    code += this.independent.hashCode();
    code += this.dependentUnit.hashCode();
    code += this.independentUnit.hashCode();
    return code;
  }

  public boolean loadData(Object data) throws ClassCastException
  {
    if (data instanceof FunctionData)
      return load((FunctionData) data);
    else if (data instanceof SEFunction)
      return set((SEFunction) data);
    return false;
  }

  public boolean load(FunctionData in)
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
  
  public FunctionData unload()
  {
    if(!this.isValid())
      return null;
    FunctionData to = CDMSerializer.objFactory.createFunctionData();
    unload(to);
    return to;
  }

  protected void unload(FunctionData to)
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
    if(this.dependent.length != this.independent.length)
      return false;
    if(!isValidDependentUnit(this.dependentUnit))
    	return false;
    if(!isValidIndependentUnit(this.independentUnit))
    	return false;
    return true;
  }
  
  public boolean isValidDependentUnit(String unit)
  {
  	return true;
  }
  
  public boolean isValidIndependentUnit(String unit)
  {
  	return true;
  }
  
  public void invalidate()
  {
    this.dependent = new double[0];
    this.independent = new double[0];
    this.dependentUnit="";
    this.independentUnit="";
  }

  public boolean equals(Object to)
  {
    if ((to == null) || !(to instanceof SEFunction))
    {
      return false;
    }

    return SEFunction.equals(this, (SEFunction) to, null);
  }

  public boolean equals(SEFunction to, SEEqualOptions options)
  {
    return SEFunction.equals(this, to, options);
  }

  public static boolean equals(SEFunction from, SEFunction to)
  {
    return SEFunction.equals(from, to, null);
  }
  
  public static boolean equals(SEFunction from, SEFunction to, SEEqualOptions options)
  {
    return equals(from, to, options, null);
  }

  public static boolean equals(SEFunction from, SEFunction to, SEEqualOptions options, String name)
  {
    if (from == to)
      return true;
    if (to == null && from != null && !from.isValid())
      return true;
    if (from == null && to != null && !to.isValid())
      return true;

    boolean forceReturn = true;
    if (options != null)
    {
      if (name == null)
        name = "SEFunction";
      options.pushTrack(name);
      forceReturn = options.returnOnFirstDiff;
    }

    if (from == null || to == null)
    {
      if (options != null)
      {
        options.track(name, from, to);
        options.trackToMap();
      }
      return false;
    }
    if (!from.isValid() && !to.isValid())
    {
    	if(options != null)
        options.trackToMap();
      return true;
    }
    if((from.isValid()&&!to.isValid())||(!from.isValid()&&to.isValid()))
    {
      if(options!=null)
      {
        options.trackInvalid(name.toString(),from,to);
        options.trackToMap();
      }
     return false; 
    }

    boolean equals = true;
    if (!SEFunction.equals(from.dependent, from.dependentUnit, to.dependent, to.dependentUnit, options, "Dependent"))
    {
      if (forceReturn)
      {
        if(options!=null)
          options.trackToMap();
        return false;
      }
      equals = false;
    }

    if (!SEFunction.equals(from.independent, from.independentUnit, to.independent, to.independentUnit, options, "Independent"))
    {
      if (forceReturn)
      {
        if(options!=null)
          options.trackToMap();
        return false;
      }
      equals = false;
    }

    if (options != null)
      options.trackToMap();
    return equals;
  }
  
  public int length()
  {
  	return this.independent.length;
  }
  
  public String getDependentUnit()
  {
  	return this.dependentUnit;
  }
  
  public String getIndependentUnit()
  {
  	return this.independentUnit;
  }

  public double[] getDependent()
  {
    return this.dependent;
  }
  
  public double[] getDependent(String unit)
  {
  	if(!isValidDependentUnit(unit))
  		throw new InvalidUnitException(unit, "Dependent");
  	if(this.dependentUnit.equals(unit))
  		return this.dependent;
  	return UnitConverter.convert(dependent, dependentUnit, unit);
  }
  
  public String getDependentString()
  {
    return StringUtils.toString(this.dependent);
  }
  
  public String getDependentString(String unit)
  {
  	if(!isValidDependentUnit(unit))
  		throw new InvalidUnitException(unit, "Dependent");
  	if(this.dependentUnit.equals(unit))
  		return StringUtils.toString(this.dependent);
  	double[]d=UnitConverter.convert(dependent, dependentUnit, unit);
  	return StringUtils.toString(d);
  }
  
  public void setDependent(double[] data)
  {
  	if(!isValidDependentUnit(""))
  		throw new InvalidUnitException("no unit", "Dependent");
  	this.dependent=data;
  	this.dependentUnit="";
  }
  
  public void setDependent(double[] data, String unit)
  {
  	if(!isValidDependentUnit(unit))
  		throw new InvalidUnitException(unit, "Dependent");
  	this.dependent=data;
  	this.dependentUnit=unit;
  }

  public void setDependent(String data)
  {
  	if(!isValidDependentUnit(""))
  		throw new InvalidUnitException("no unit", "Dependent");
  	this.dependent=StringUtils.toArray(data);
  	this.dependentUnit="";
  }
  
  public void setDependent(String data, String unit)
  {
  	if(!isValidDependentUnit(unit))
  		throw new InvalidUnitException(unit, "Dependent");
  	this.dependent=StringUtils.toArray(data);
  	this.dependentUnit=unit;
  }
  
  public double[] getIndependent()
  {
    return this.independent;
  }
  
  public double[] getIndependent(String unit)
  {
  	if(!isValidIndependentUnit(unit))
  		throw new InvalidUnitException(unit, "Independent");
  	if(this.independentUnit.equals(unit))
  		return this.independent;
  	return UnitConverter.convert(independent, independentUnit, unit);
  }
  
  public String getIndependentString()
  {
    return StringUtils.toString(this.independent);
  }
  
  public String getIndependentString(String unit)
  {
  	if(!isValidIndependentUnit(unit))
  		throw new InvalidUnitException(unit, "Independent");
  	if(this.independentUnit.equals(unit))
  		return StringUtils.toString(this.independent);
  	double[]d=UnitConverter.convert(independent, independentUnit, unit);
  	return StringUtils.toString(d);
  }
  
  public void setIndependent(double[] data)
  {
  	if(!isValidIndependentUnit(""))
  		throw new InvalidUnitException("no unit", "Ordiate");
  	this.independent=data;
  	this.independentUnit="";
  }
  
  public void setIndependent(double[] data, String unit)
  {
  	if(!isValidIndependentUnit(unit))
  		throw new InvalidUnitException(unit, "Ordiate");
  	this.independent=data;
  	this.independentUnit=unit;
  }
  
  public void setIndependent(String data)
  {
  	if(!isValidIndependentUnit(""))
  		throw new InvalidUnitException("no unit", "Independent");
  	this.independent=StringUtils.toArray(data);
  	this.independentUnit="";
  }
  
  public void setIndependent(String data, String unit)
  {
  	if(!isValidIndependentUnit(unit))
  		throw new InvalidUnitException(unit, "Independent");
  	this.independent=StringUtils.toArray(data);
  	this.independentUnit=unit;
  }
}
