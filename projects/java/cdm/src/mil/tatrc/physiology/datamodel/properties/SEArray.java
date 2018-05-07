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

import java.util.*;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.SEEqualOptions;
import mil.tatrc.physiology.datamodel.bind.ArrayData;
import mil.tatrc.physiology.datamodel.bind.EnumErrorType;
import mil.tatrc.physiology.datamodel.exceptions.InvalidUnitException;
import mil.tatrc.physiology.utilities.DoubleUtils;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.StringUtils;
import mil.tatrc.physiology.utilities.UnitConverter;

/**
 * Convert ArrayData into actual arrays
 * @author abray
 */
public class SEArray extends SEProperty
{
  protected String   unit;
  protected double[] dData;
  protected List<Double> dList;
  protected int precision;
  
  public SEArray()
  {
    super();
    invalidate();
  }
  
  public SEArray(double[] data, String units)
  {
    super();
    invalidate();
    setDoubleData(data, units);
  }
  
  public boolean set(SEArray ary)
  {
    if (ary == null || !ary.isValid())
      return false;
    this.invalidate();

    this.precision = ary.precision;
    this.unit = ary.unit;
    if (ary.dData != null)
      this.dData = Arrays.copyOf(ary.dData, ary.dData.length);
    else if (ary.dList != null && !ary.dList.isEmpty())
    {
      if (this.dList == null)
        this.dList = new ArrayList<Double>();
      this.dList.addAll(ary.dList);
    }
    return true;
  }

  public boolean loadData(Object data) throws ClassCastException
  {
    if (data instanceof ArrayData)
      return load((ArrayData) data);
    else if (data instanceof SEArray)
      return set((SEArray)data);
    return false;
  }

  public boolean load(ArrayData data)
  {
    if (data == null)
      return false;
    return this.setDoubleData(data.getValue().getDoubleList(),data.getUnit());
  }

  public Object unloadData()
  {
    return unload();
  }

  public ArrayData unload()
  {
    if (!this.isValid())
      return null;
    
    ArrayData to = CDMSerializer.objFactory.createArrayData();
    unload(to);
    
    return to;
  }

  protected void unload(ArrayData to)
  {
    if(this.unit != null && !this.unit.equals(""))
      to.setUnit(this.unit);
    to.setValue(CDMSerializer.objFactory.createDoubleArray());
    if(this.dData!=null)
      toList(to.getValue().getDoubleList(),this.dData,this.precision);
    else
      to.getValue().getDoubleList().addAll(this.dList);
  }
  
  public  int hashCode()
  {
  	if(this.dData!=null)
      return this.dData.hashCode()+this.unit.hashCode();
  	else if(this.dList!=null)
  	  return this.dList.hashCode()+this.unit.hashCode();
  	return 0;
  }

  public boolean equals(Object to)
  {
    if ((to == null) || !(to instanceof SEArray))
    {
      return false;
    }

    return SEArray.equals(this, (SEArray) to, null);
  }

  public static boolean equals(SEArray from, SEArray to)
  {
    return SEArray.equals(from, to, null);
  }
  
  public static boolean equals(SEArray from, SEArray to, SEEqualOptions options)
  {
    return equals(from, to, options, null);
  }

  public static boolean equals(SEArray from, SEArray to, SEEqualOptions options, String name)
  {
    if (options != null)
    {
      if (name == null)
        name = "SEArray";        
       options.pushTrack(name);
    }
    boolean equals=true;
    
    if (from.isValid() && to.isValid())
    {
      if(from.dData!=null&&to.dData!=null)
      {
        if(options != null)
          options.popTrack();//The SEArray.equals will track the name again
        equals &= SEArray.equals(from.dData, from.unit, to.dData, to.unit, options, name);
      }
      else if(from.dList!=null&&(to.dList!=null&&!to.dList.isEmpty()))
      {
        if(options != null)
          options.popTrack();//The SEArray.equals will track the name again
        equals &= SEArray.equals(from.dList, from.unit, to.dList, to.unit, options, name);
      }
      else
      {
        if(from.dData!=null)
        {        	
          double[] todData = to.getDoubleData();
          if(options != null)
            options.popTrack();//The SEArray.equals will track the name again
          equals &= SEArray.equals(from.dData, from.unit, todData, to.unit, options, name);
        }
        else if(from.dList!=null)
        {
          List<Double> todList=new ArrayList<Double>();
          to.getDoubleData(todList);
          if(options != null)
            options.popTrack();//The SEArray.equals will track the name again
          equals &= SEArray.equals(from.dList, from.unit, todList, to.unit, options, name);
        }
        else
        {
          Log.error("Incompatible equals");
          equals = false;
        }
      }
    }
    return equals;
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

    if (options != null)
    {
      if (name == null)
        name = "SEArray";
       options.pushTrack(name);      
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

    boolean equals = true;
    boolean forceReturn = true;
    if (options != null)
    {
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
  
  public static synchronized boolean equals(List<Double> from, List<Double> to)
  {
    return equals(from, "", to, "", null);
  }
  public static synchronized boolean equals(List<Double> from, List<Double> to, SEEqualOptions options)
  {
    return equals(from, "", to, "", options);
  }
  public static synchronized boolean equals(List<Double> from, List<Double> to, SEEqualOptions options, String name)
  {
    return equals(from, "", to, "", options, name);
  }
  public static synchronized boolean equals(List<Double> from, String fromUnits, List<Double> to, String toUnits, SEEqualOptions options)
  {
    return equals(from, fromUnits, to, toUnits, options, null);
  }
  public static synchronized boolean equals(List<Double> from, String fromUnits, List<Double> to, String toUnits, SEEqualOptions options, String name)
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

    if (from.size() != to.size())
    {
      if (options != null)
      {
        options.track("length", from.size(), to.size());
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

    for (int i = 0; i < to.size(); i++)
    {
      if (!DoubleUtils.equals(from.get(i), fromUnits, to.get(i), toUnits, options))
      {
        if (options != null)
          options.track("Array[" + i + "]", from.get(i), fromUnits, to.get(i), toUnits);
        if (forceReturn)
          return false;
        equals = false;
      }
    }

    if (options != null)
      options.trackToMap();
    return equals;
  }
  
  
  public int size()
  {
  	if(this.dData!=null)
      return this.dData.length;
  	if(this.dList!=null)
  	  return this.dList.size();
  	return 0;
  }
  
  public boolean isValid()
  {
    if(!this.validUnit(this.unit))
      return false;
    if(this.dData==null)
    {
      if(this.dList==null)
        return false;
      if(this.dList.size()==0)
        return false;
    }
    else if(this.dData.length==0)
      return false;
    return true;
  }
  
  public boolean validUnit(String unit)
  {
    return true;
  }
  
  public void invalidate()
  {
    this.unit="";
    this.dData=null;
    this.dList=null;
    this.unit="";
    this.precision=-1;
  }
  
  /**
   * Set the amount of precision when converting to a string (i.e. xml)
   * @param p
   * @return
   */
  public void setPrecision(int p)
  {
    this.precision=p;
  }
  
  public int getPrecision()
  {
    return this.precision;
  }
  
  public void truncate(int mantissaLength)
  {
    if(this.dData!=null)
    {
      for(int i=0;i<this.dData.length;i++)
      {
        this.dData[i]=DoubleUtils.truncate(this.dData[i], mantissaLength);
      }
    }
    else if(this.dList!=null&&!this.dList.isEmpty())
    {
      for(int i=0;i<this.dList.size();i++)
      {
        this.dList.set(i, DoubleUtils.truncate(this.dList.get(i), mantissaLength));
      }
    }
  }
  
  public String getUnit()
  {
    return this.unit;
  }
  
  public double getMaxValue()
  {
  	return getMaxValue(this.unit);
  }
  public double getMaxValue(String unit)
  {
  	if(!this.validUnit(unit))
  		throw new InvalidUnitException(unit, this.getClass().getName());
  	
    double max = -Double.MAX_VALUE;    
    if(this.dData!=null)
    {
    	max=DoubleUtils.getMax(this.dData);
    }
    else if(this.dList!=null)
    {
      max=DoubleUtils.getMax(this.dList);
    }
    return UnitConverter.convert(max, this.unit, unit);
  }
  
  public double getMinValue()
  {
  	return getMinValue(this.unit);
  }
  public double getMinValue(String unit)
  {
  	if(!this.validUnit(unit))
    	throw new InvalidUnitException(unit,this.getClass().getName());
    
    double min = Double.MAX_VALUE;
    if(this.dData!=null)
    {
    	min = DoubleUtils.getMin(this.dData);
    }
    else if(this.dList!=null)
    {
    	min = DoubleUtils.getMin(this.dList);
    }
    return UnitConverter.convert(min,this.unit,unit);
  }
  
  public void getIndex(int idx, SEScalar to)
  {
    if(dData!=null)
    {
      if(idx<0||idx>=this.dData.length)
        throw new ArrayIndexOutOfBoundsException(idx);
      to.setValue(this.dData[idx],this.unit);     
    }
    else if(this.dList!=null)
    {
      if(idx<0||idx>=this.dList.size())
        throw new ArrayIndexOutOfBoundsException(idx);
      to.setValue(this.dList.get(idx),this.unit);     
    }
    else
      throw new ArrayIndexOutOfBoundsException(idx);
  }
  
  public double getIndex(int idx)
  {
  	if(dData!=null)
  	{
  		if(idx<0||idx>=this.dData.length)
  			throw new ArrayIndexOutOfBoundsException(idx);
  		return this.dData[idx];
  	}
  	else if(this.dList!=null)
  	{
  		if(idx<0||idx>=this.dList.size())
  			throw new ArrayIndexOutOfBoundsException(idx);
  		return this.dList.get(idx);
  	}
  	else
  	  throw new ArrayIndexOutOfBoundsException(idx);
  }
  
  public double getIndex(int idx, String unit)
  {
  	if(!this.validUnit(unit))
  		throw new InvalidUnitException(unit, this.getClass().getName());
  	
    return UnitConverter.convert(this.getIndex(idx),this.unit, unit);
  }
  
  public double[] getSubsequence(int startIdx, int endIdx)
  {
     return getSubsequence(startIdx,endIdx,this.unit);
  }
  
  public double[] getSubsequence(int startIdx, int endIdx, String unit)
  {
    int n=0;
    double[] subsequence=null;
    if(dData!=null)
    {
      if(startIdx<0||startIdx>this.dData.length)
        throw new ArrayIndexOutOfBoundsException(startIdx);
      if(endIdx<0||endIdx>this.dData.length)
        throw new ArrayIndexOutOfBoundsException(endIdx);
      subsequence = new double[endIdx-startIdx];
      for(int i=startIdx; i<endIdx; i++)
        subsequence[n++]=this.dData[i];
    }
    else if(this.dList!=null)
    {
      if(startIdx<0||startIdx>this.dList.size())
        throw new ArrayIndexOutOfBoundsException(startIdx);
      if(endIdx<0||endIdx>this.dList.size())
        throw new ArrayIndexOutOfBoundsException(endIdx);
      subsequence = new double[endIdx-startIdx];
      for(int i=startIdx; i<endIdx; i++)
        subsequence[n++]=this.dList.get(i);
    }
    return subsequence;
  }
  
  public void setIndex(int idx, double value)
  {
  	if(!this.isValid())
  		this.append(value);
  	else
    	this.setIndex(idx, value, "");
  }
  
  public void setIndex(int idx, double value, String unit)
  {

  	if(!this.isValid())
  		this.append(value,unit);
  	else
  	{
  		if(!this.validUnit(unit))
    		throw new InvalidUnitException(unit,this.getClass().getName());
    	
  		double d=value;
    	if(this.unit.isEmpty()&&!unit.isEmpty())
    	  this.unit = unit;
    	if(!this.unit.isEmpty()&&!this.unit.equals(unit))
      	d=UnitConverter.convert(d, unit, this.unit);
    	if(idx>=this.size())
    		throw new ArrayIndexOutOfBoundsException(idx);
    	if(this.dData!=null)    		
    	  this.dData[idx]=d;
    	else
    		this.dList.set(idx,d);
  	}
  }

  public boolean append(SEScalar s)
  {
    if (s != null && s.isValid())
    {
      if(this.append(s.getValue(),s.getUnit()))
        return true;
    }
    return false;
  }
  
  public boolean append(SEArray a)
  {
    if (a != null && a.isValid())
    {
      if(this.append(a.getDoubleData(),a.getUnit()))
        return true;
    }
    return false;
  }
  
  public boolean append(double d)
  {
    return this.append(d,"");
  }
  
  public boolean append()
  {
    return this.append(Double.NaN,this.unit);
  }
  
  public boolean append(double d, String unit)
  {
  	if(!this.validUnit(unit))
  		throw new InvalidUnitException(unit,this.getClass().getName());
  	
  	if(this.unit.isEmpty()&&!unit.isEmpty())
  	  this.unit = unit;
  	
  	if(this.dData!=null)
  	{
  	  for(double data : this.dData)
  	    this.dList.add(data);
  	  this.dData=null;
  	}
    if(!this.unit.isEmpty()&&!this.unit.equals(unit))
    {
    	d=UnitConverter.convert(d, unit, this.unit);
    }
    if (this.dList == null)
      this.dList = new ArrayList<Double>();
    this.dList.add(d);
    return true;
  }
  
  public boolean append(double[] array, String unit)
  {
    if(!this.validUnit(unit))
      throw new InvalidUnitException(unit,this.getClass().getName());
    
    if(this.unit.isEmpty()&&!unit.isEmpty())
      this.unit = unit;
    
    if(this.dData!=null)
    {
      for(double data : this.dData)
        this.dList.add(data);
      this.dData=null;
    }
    if(!this.unit.isEmpty()&&!this.unit.equals(unit))
    {
      array=UnitConverter.convert(array, unit, this.unit);
    }
    if (this.dList == null)
      this.dList = new ArrayList<Double>();
    addArrayToList(this.dList, array);
    return true;
  }
  
  public int[] getIntData()
  {
    return getIntData(this.unit);
  }

  public int[] getIntData(String unit)
  {
    if(!this.validUnit(unit))
    	throw new InvalidUnitException(unit,this.getClass().getName());
    
    if(this.dData!=null)
    {
      int[] ints = new int[this.dData.length];
      for(int i=0;i<ints.length;i++)
        ints[i] = (int)this.dData[i];
      return ints;
    }
    else if(this.dList!=null)
    {
      int[] ints = new int[this.dList.size()];
      for(int i=0;i<ints.length;i++)
        ints[i] = this.dList.get(i).intValue();
      return ints;
    }
    return null;
  }
  
  public double[] getDoubleData()
  {
    return getDoubleData(this.unit);
  }

  /**
   * A new double[] will be allocated and returned
   * You may want to use the list version of getDoubleData
   * @param unit
   * @return
   */
  public double[] getDoubleData(String unit)
  {
    if(!this.validUnit(unit))
    	throw new InvalidUnitException(unit,this.getClass().getName());
    
    // TODO maybe we want a map of double[] where the map key is a unit
    // So we can return those and not allocate on each get?
    
    if(this.dData!=null)
    {
      double dAry[] = new double[this.dData.length];
      for(int i=0; i<this.dData.length; i++)
      {
        if(this.unit.equals(unit))
          dAry[i]=this.dData[i];
        else
          dAry[i] = UnitConverter.convert(this.dData[i], this.unit, unit);
      }
      return dAry;
    }
    else if(this.dList!=null)
    {
      double dAry[] = new double[this.dList.size()];
      for(int i=0; i<this.dList.size(); i++)
      {
        if(this.unit.equals(unit))
          dAry[i]=this.dList.get(i);
        else
          dAry[i] = UnitConverter.convert(this.dList.get(i), this.unit, unit);
      }
      return dAry;
    }
    return null;
  }
  
  /**
   * List will be cleared internally
   * @param list
   * @return
   */
  public boolean getDoubleData(List<Double> list)
  {
  	return this.getDoubleData(list, this.unit);
  }
  
  /**
   * List will be cleared internally
   * @param list
   * @param unit
   * @return
   */
  public boolean getDoubleData(List<Double> list, String unit)
  {
  	if(list==null)
  		return false;
  	if(!this.isValid())
  	  return false;
  	
    if(!this.validUnit(unit))
      throw new InvalidUnitException(unit,this.getClass().getName());
    
    list.clear();
    if(this.unit.equals(unit))
    {
    	if(this.dData!=null)
    	{
    		for(double d : this.dData)
    			list.add(d);
    	}
    	else
    		list.addAll(this.dList);
    }
    else
    {
    	if(this.dData!=null)
    	{
    		for(double d : this.dData)
    			list.add(UnitConverter.convert(d, this.unit, unit));
    	}
    	else if(this.dList!=null)
    	{
    		for(double d : this.dList)
    			list.add(UnitConverter.convert(d, this.unit, unit));
    	}
    	else
    	  return false;
    }
    return true;
  }

  public boolean setIntData(int[] in)
  {
  	this.dData = new double[in.length];
  	for(int i=0;i<in.length;i++)
  		this.dData[i] = in[i];
  	return setDoubleData(this.dData);
  }
  
 
  public boolean setIntData(int[] in, String units)
  {
    if (!this.validUnit(units))
    	throw new InvalidUnitException(unit,this.getClass().getName());
    
    this.dData = new double[in.length];
    for(int i=0;i<in.length;i++)
      this.dData[i] = in[i];
    return setDoubleData(this.dData, units);  
  }
  
  /**
   * Member array is set to this array.
   * A copy is not made.
   * @param in
   * @return
   */
  public boolean setDoubleData(double[] in)
  {
    return setDoubleData(in,"");
  }
  
  public boolean setDoubleData(List<Double> in)
  {
  	return setDoubleData(in,"");
  }
  
 
  /**
   * Member array is set to this array.
   * A copy is not made. 
   * @param in
   * @param units
   * @return
   */
  public boolean setDoubleData(double[] in, String units)
  {
    if (in==null)
      return false;
    if (!this.validUnit(units))
      throw new InvalidUnitException(units, this.getClass().getName());

    this.unit = units;
    if(this.unit==null)
      this.unit="";   
    this.dData=in;
    this.dList=null;
    return true;
  }
  
  public boolean setDoubleData(List<Double> in, String units)
  {
  	if (in==null)
      return false;
    if (!this.validUnit(units))
    	throw new InvalidUnitException(unit,this.getClass().getName());

    this.dData=null;
    this.unit = units;
    if(this.unit==null)
      this.unit=""; 
    this.dList=in;
    return true;
  }
  
  public boolean setStringData(String dataString)
  {
    return this.setStringData(dataString, "", this.precision);
  }
  
  public boolean setStringData(String dataString, String units)
  {
    return this.setStringData(dataString, units, this.precision);
  }
  
  public boolean setStringData(String dataString, String units, int precision)
  {
    if (dataString==null)
      return false;
    if (!this.validUnit(units))
      throw new InvalidUnitException(units,this.getClass().getName());
    
    if(units==null)
      this.unit="";
    else
      this.unit = units;
    
    this.dData=StringUtils.toArray(dataString,precision);
    this.dList=null;
    return true;
  }
  
  public static void invalidate(double[] d)
  {
    if(d==null)
      return;
    for(int i=0; i<d.length; i++)
      d[i]=Double.NaN;
  }
  
  /**
   * List will be cleared internally
   * @param to
   * @param dAry
   */
  public static void toList(List<Double>to, double[] dAry)
  {
  	to.clear();
  	if(dAry!=null)
  	{
  	  for(double d : dAry)
  	    to.add(d);
  	}
  }
  
  /**
   * List will be cleared internally
   * @param to
   * @param dAry
   * @param precision
   */
  public static void toList(List<Double>to, double[] dAry, int precision)
  {
  	to.clear();
  	if(to instanceof ArrayList)
  	  ((ArrayList<Double>)to).ensureCapacity(dAry.length);
    for(double d : dAry)
    {
      if(precision>0)
        d = DoubleUtils.truncate(d,precision);
      to.add(d);
    }
  }
  
  public static void toList(List<Double>to, List<Double> from, int precision)
  {
    to.clear();
    if(precision>0)
    {
      if(to instanceof ArrayList)
        ((ArrayList<Double>)to).ensureCapacity(from.size());
      for(double d : from)
      {
        d = DoubleUtils.truncate(d,precision);
        to.add(d);
      }
    }
    else
      to.addAll(from);
  }
  
  public static double[] subset(double[] data, int startIdx, int length)
  {
    if(startIdx>=data.length||length<0)
      throw new IndexOutOfBoundsException();
    double[] sub = new double[length];
    for(int i=0;i<sub.length;i++)
      sub[i]=data[startIdx+i];
    return sub;
  }
  
  public String toString()
  {
    return Arrays.toString(getDoubleData()) + unit;
  }
  
  public static String[] copyOfRange(String[] original, int from, int to)
  {
    if (original == null)
      return null;
    if (from > to)
      throw new IndexOutOfBoundsException("from is greater than to");
    if (from < 0 || from >= original.length)
      throw new IndexOutOfBoundsException("from is out of bounds");
    if (to < 0 || to >= original.length)
        throw new IndexOutOfBoundsException("to is out of bounds");
      
    int size = to - from + 1;
    String[] copy = new String[size];
    int i = 0;
    for (int j = to; j <= from; j++)
    {
      copy[i++] = original[j];
    }
    
    return copy;
  }
  
  /**
   * Helper method to convert an array of double primitives into an array of integer primitives.
   * While autoboxing is useful, it does not provide ability to cast Integer[] to int[]
   * so the normal (int[])collection.toArray() cannot work.
   * @param c Collection to convert
   * @return Array of integer primitives
   */
  public static int[] toIntArray(double [] array) {
    int [] a = new int[array.length];
    for (int i = 0; i < array.length; i++)
    {
      a[i] = (int)array[i];
    }
    return a;
  }
  
  /**
   * Helper method to convert a set of Integer objects into an array of integer primitives.
   * While autoboxing is useful, it does not provide ability to cast Integer[] to int[]
   * so the normal (int[])collection.toArray() cannot work.
   * @param c Collection to convert
   * @return Array of integer primitives
   */
  public static int[] toIntArray(Set<Integer> list)
  {
    if (list == null)
      return null;
    
    int[] a = new int[list.size()];
    Iterator<Integer> it = list.iterator();
    int index = 0;
    while (it.hasNext())
    {
      a[index++] = it.next();
    }
    return a;
  }
  
   /**
   * Helper method to convert an array of Double objects into an array of double primitives.
   * While autoboxing is useful, it does not provide ability to cast Double[] to double[]
   * so the normal (double[])collection.toArray() cannot work.
   * @param c Collection to convert
   * @return Array of double primitives
   */
  public static double[] toArray(Double [] array)
  {
    double [] a = new double[array.length];
    for (int i = 0; i < array.length; i++)
    {
      a[i] = array[i];
    }
    return a;
  }
  
  /**
   * Helper method to convert an array of Double objects into an array
   * of double primitives. 
   * @param array Array to convert
   * @return Array of Double objects
   */
  public static Double[] toArray(double[] array)
  {
    Double [] a = new Double[array.length];
    for (int i = 0; i < array.length; i++)
    {
      a[i] = array[i];
    }
    return a;
  }

  public static void addArrayToList(List<Double> list, double[] array)
  {
    if (list == null || array == null)
      return;
    
    for (double d : array)
    {
      list.add(d);
    }
  }
  
  public static void addArrayToList(List<Integer> list, int[] array)
  {
    if (list == null || array == null)
      return;
    
    for (int i : array)
    {
      list.add(i);
    }
  }
  
  public static void addArrayToSet(Set<Integer> set, int[] array)
  {
    if (set == null || array == null)
      return;
    
    for (int i : array)
    {
      set.add(i);
    }
  }
  
  public static void addArrayToSet(Set<Integer> set, double[] array)
  {
    if (set == null || array == null)
      return;
    
    for (double d : array)
    {
      set.add((int)d);
    }
  }
  
  /**
   * Helper method to convert an list of Double objects into an array
   * of double primitives. 
   * @param list List to convert
   * @return Array of double objects
   */
  public static double[] toArray(List<Double> list)
  {
    double [] a = new double[list.size()];
    for (int i = 0; i < list.size(); i++)
    {
      a[i] = list.get(i);
    }
    return a;
  }
  
  /**
   * Helper method to convert a set of Double objects into an array of double primitives.
   * While autoboxing is useful, it does not provide ability to cast Double[] to double[]
   * so the normal (double[])collection.toArray() cannot work.
   * @param c Collection to convert
   * @return Array of double primitives
   */
  public static double[] toArray(Set<Double> set)
  {
    if (set == null)
      return null;
    
    double[] a = new double[set.size()];
    Iterator<Double> it = set.iterator();
    int index = 0;
    while (it.hasNext())
    {
      a[index++] = it.next();
    }
    return a;
  }
  
  /**
  * Helper method to convert a list of Double arrays into an array of double primitives.
  * @param c List to convert
  * @return Array of double primitives
  */
 public static double[] ListToArray(List<Double[]> list)
 {
   int size = 0;
   for (Double[] dArray : list)
   {
     size += dArray.length;
   }
   
   double [] a = new double[size];
   int index = 0;
   for (Double[] dArray : list)
   {
     for (Double d : dArray)
     {
       a[index++] = d;
     }
   }
   return a;
 }
 
  /**
   * Helper method to convert an array of Boolean objects into an array of boolean
   * primitives. While autoboxing is useful, it does not provide ability to cast
   * Boolean[] to boolean[] so the normal (boolean[])collection.toArray() cannot
   * work.
   * 
   * @param c Collection to convert
   * @return Array of boolean primitives
   */
  public static boolean[] toBooleanArray(Boolean[] array)
  {
    boolean[] a = new boolean[array.length];
    for (int i = 0; i < array.length; i++)
    {
      a[i] = array[i];
    }
    return a;
  }
  
  /**
   * Helper method to convert an list of Boolean objects into an array
   * of boolean primitives. 
   * @param list List to convert
   * @return Array of double objects
   */
  public static boolean[] toBooleanArray(List<Boolean> list)
  {
    boolean [] a = new boolean[list.size()];
    for (int i = 0; i < list.size(); i++)
    {
      a[i] = list.get(i);
    }
    return a;
  }
}
