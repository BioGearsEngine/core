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

/**
 * 
 */
package mil.tatrc.physiology.utilities;

import java.io.Serializable;
import java.math.BigDecimal;
import java.util.Collections;
import java.util.List;

/**
 * @author abray
 *
 */
public class DoubleUtils implements Serializable
{
  public static void main(String[] args)
  {
    System.out.println(truncate(0.004999999999999999,2));
    
    double err=percentTolerance(0.0349066,0.035,1E-20);
    System.out.println(err);
    if(err<0.001)
    	System.out.println("equal");
    else
    	System.err.println("not equal");
    
    DoubleUtils opts = new DoubleUtils();
    //opts.truncateMantissasBy(9);
    opts.matchFromMantissa();
    if(DoubleUtils.equals(-0.000792888, -0.000792891,opts))
      System.out.println("equal");
    else
      System.err.println("not equal");
  }
  
  public enum EqualMode {PercentDifference,PercentTolerance,AbsoluteError}
  
  protected EqualMode mode;
  protected double equalLimit;
  protected double epsilon;
  
  public enum MantissaOption { None,MatchFrom, MatchTo, ManualMantissa }
  
  protected MantissaOption mantissaOption;
  protected int mantissaLength;

  /** Two approaches to checking for less than and less than or equal to.
   * 
   *  THE REASON FOR TWO MODES:  Depending on the equalLimit the user chooses, 
   *                             values can be reported to be less than (and/or 
   *                             equal to) when in reality they are numerically 
   *                             larger when using the "EqualMode" option. 
   *                             This can lead to unexpected behavior if the 
   *                             user is not aware.  
   *                             For this reason, the "Strict" option will 
   *                             provide numerically reliable results.  
   *   
   *  For checking for less than:
   *    "Equal Mode" Approach - Ensures the value is (1) truly less than (<) and 
   *                            (2) the values are not 'equal' as determined 
   *                            using the DoubleUtils equals method (hence 
   *                            using the equalLimit and Equalmode defined in 
   *                            the DoubleUtils object)
   *    "Strict" Approach - Ensures the value is truly less than (<).  Does not 
   *                        check to see if values are equal, only that they 
   *                        are less than.
   *
   *  For checking for less than or equal to:
   *    "Equal Mode" Approach - Ensures the value is (1) truly less than (<) or 
   *                            (2) the values are 'equal' as determined 
   *                            using the DoubleUtils equals method (hence 
   *                            using the equalLimit and Equalmode defined in 
   *                            the DoubleUtils object)
   *    "Strict" Approach - Checks if the value is greater than (>) only.  If 
   *                        value is greater than, then the return values 
   *                        indicates the value is not less than or equal to.
   */
  public enum LessThanOption { EqualMode, Strict }
  protected LessThanOption lessThanOption;

  
  protected double lastError;

  
  public DoubleUtils()
  {
    this.mode = EqualMode.PercentDifference;
    this.equalLimit = 0.5;
    this.epsilon = 1E-20;
    this.mantissaOption = MantissaOption.None;
    this.mantissaLength=-1;
    this.lessThanOption = LessThanOption.Strict;    
  }
  
  public DoubleUtils(DoubleUtils du)
  {
  	this.mode=du.mode;
  	this.equalLimit=du.equalLimit;
  	this.epsilon=du.epsilon;
  	this.mantissaOption = du.mantissaOption;
  	this.mantissaLength = du.mantissaLength;
    this.lessThanOption = du.lessThanOption;    
  }
  
  public DoubleUtils(EqualMode mode)
  {
    this();
    if(mode==null)
    {
      Log.warn("Mode not provided, using defaults");
      return;
    }
    this.mode=mode;
    if(EqualMode.PercentDifference.equals(mode))
      this.equalLimit=0.5;
    else if(EqualMode.PercentTolerance.equals(mode))
      this.equalLimit=0.5;
    else if(EqualMode.AbsoluteError.equals(mode))
    this.equalLimit=0.1;
    
    this.mantissaOption = MantissaOption.None;
    this.mantissaLength=-1;

    this.lessThanOption = LessThanOption.Strict;    
  }
  
  public DoubleUtils(EqualMode mode, double limit)
  {
	this();
    if(mode==null)
    {
      Log.warn("Mode not provided, using defaults");
      return;
    }
    this.mode=mode;
    this.equalLimit=limit;
    
    this.mantissaOption = MantissaOption.None;
    this.mantissaLength=-1;
    
    this.lessThanOption = LessThanOption.Strict;        
  }
  
  public void setPercentDifference(double limit)
  {
    this.mode=EqualMode.PercentDifference;
    this.equalLimit=limit;
  }
  
  public void setPercentTolerance(double limit)
  {
    this.mode=EqualMode.PercentTolerance;
    this.equalLimit=limit;
    this.epsilon=1E-20;
  }

  /**
   * 
   * @param limit - 
   * @param epsilon
   */
  public void setPercentTolerance(double limit, double epsilon)
  {
    this.mode=EqualMode.PercentTolerance;
    this.equalLimit=limit;
    this.epsilon=epsilon;
    if(epsilon == 0.0)
    	Log.warn("Epsilon has been set to 0.0. Unexpected results may occur.");
    if(epsilon < 0.0)
      Log.warn("Epsilon has been set to a negative value (" + epsilon + "). Unexpected results may occur.");
    
  }
  
  public void setAbsoluteError(double limit)
  {
    this.mode=EqualMode.AbsoluteError;
    this.equalLimit=limit;
  }

  public void setLessThanOption(LessThanOption lessThanOption)
  {
    this.lessThanOption = lessThanOption;
  }
  
  
  public EqualMode getMode(){return this.mode;}
  public void setMode(EqualMode m){this.mode=m;}
  public double    getLimit(){return this.equalLimit;}
  public void setLimit(double limit){this.equalLimit=limit;}
  public LessThanOption getLessThanOption(){return this.lessThanOption;}
  
  
  /**
   * The to value mantissa will be truncated
   * to the same length as the from value mantissa
   */
  public void matchFromMantissa()
  {
    this.mantissaOption=MantissaOption.MatchFrom;
  }
  /**
   * The from value mantissa will be truncated
   * to the same length as the to value mantissa
   */
  public void matchToMantissa()
  {
    this.mantissaOption=MantissaOption.MatchTo;
  }
  /**
   * Both mantissas will be truncated to this length
   * before equal test
   */
  public void truncateMantissasTo(int mantissaLength)
  {
    this.mantissaOption=MantissaOption.ManualMantissa;
    if(mantissaLength < 0)
    {
      Log.warn("Mantissa Length is less than zero (" + mantissaLength + ").  Using absolute value of manitssa length instead");
      this.mantissaLength=Math.abs(mantissaLength);      
    }
    else
      this.mantissaLength=mantissaLength;
  }
  /** 
   * Mantissa will not be affected
   */
  public void clearMantissaOption()
  {
    this.mantissaOption=MantissaOption.None;
  }
  
  public void set(DoubleUtils to)
  {
    this.mode=to.mode;
    this.equalLimit=to.equalLimit;
    this.epsilon=to.epsilon;
    this.mantissaOption = to.mantissaOption;
    this.mantissaLength = to.mantissaLength;
    this.lessThanOption = to.lessThanOption;    
    
  }
  
  public double getLastError(){return lastError;}
  
  public static double truncate(double d, int mantissaLength)
  {
    if(Double.isNaN(d))
      return d;
   
    int mantissaLengthcopy = mantissaLength;
    if(mantissaLengthcopy < 0)
    {
      Log.warn("Mantissa Length is less than zero (" + mantissaLengthcopy + ").  Using absolute value of manitssa length instead");
      mantissaLengthcopy = Math.abs(mantissaLengthcopy);      
    }
    
    double new_d;
    BigDecimal bd = new BigDecimal(d);
    bd = bd.setScale(mantissaLengthcopy, BigDecimal.ROUND_HALF_UP);
    new_d = bd.doubleValue(); 
    //String s = StringUtils.toString(new_d,mantissaLength);
    //double e = Double.parseDouble(s);
    return new_d;
  }
  
  /**
   * Truncate via BigDecimal and you can specify the round mode
   * @param d
   * @param mantissaLength
   * @param bigDecimalRoundMode
   * @return
   */
  public static double truncate(double d, int mantissaLength, int bigDecimalRoundMode)
  {
    if(Double.isNaN(d))
      return d;

    int mantissaLengthcopy = mantissaLength;
    if(mantissaLengthcopy < 0)
    {
      Log.warn("Mantissa Length is less than zero (" + mantissaLengthcopy + ").  Using absolute value of manitssa length instead");
      mantissaLengthcopy = Math.abs(mantissaLengthcopy);      
    }
    
    BigDecimal bd = new BigDecimal(d);
    bd = bd.setScale(mantissaLengthcopy, bigDecimalRoundMode);
    return bd.doubleValue(); 
  }
  
  public static boolean equals(double fValue, double tValue)
  {
    return equals(fValue, tValue, null);
  }
  // Tolerance is a percentage, not a decimal value
  public static boolean equals(double fValue, double tValue, DoubleUtils opts)
  {
    return equals(fValue,"",tValue,"", opts);
  }
  public static boolean equals(double fValue, String fUnit, double tValue, String tUnit)
  {
    return equals(fValue,fUnit,tValue,tUnit,null);
  }
  public static boolean equals(double fValue, String fUnit, double tValue, String tUnit, DoubleUtils opts)
  {
    if(!validDoubles(fValue,tValue))
      return false;
    
    if(fValue==0&&tValue==0)
      return true;
    
    if(Double.isNaN(fValue)&&Double.isNaN(tValue))
      return true;
    
    double tmp=tValue;
    if(!tUnit.equals(fUnit))
      tmp = UnitConverter.convert(tValue, tUnit, fUnit);
    
    if(opts!=null)
    {
      if(MantissaOption.MatchFrom.equals(opts.mantissaOption))
      {
        tmp=truncate(tmp,mantissaLength(fValue));
      }
      else if(MantissaOption.MatchTo.equals(opts.mantissaOption))
      {
        tmp=truncate(tmp,mantissaLength(tValue));
        fValue=truncate(fValue,mantissaLength(tValue));
      }
      else if(MantissaOption.ManualMantissa.equals(opts.mantissaOption))
      {
        fValue=truncate(fValue, opts.mantissaLength);
        tmp=truncate(tmp, opts.mantissaLength);
      }
    }
    boolean equals;
    if(opts==null || EqualMode.PercentDifference.equals(opts.mode))
    {
      if(opts!=null)
      {
      	opts.setLastError(percentDifference(fValue,tmp, opts.epsilon));
      	// Note: percentDifference returns the absolute value of the difference, 
      	// but it can also return negative infinity in some case.  Hence, we 
      	// need the absolute value on the check for equality below.
      	equals = Math.abs(opts.lastError) <= opts.equalLimit;
      }
      else
      {
        equals= Math.abs(percentDifference(fValue,tmp, 1E-20)) <= 0.5;// Default
      }
    }
    else if(EqualMode.PercentTolerance.equals(opts.mode))
    {
    	opts.setLastError(percentTolerance(fValue, tmp, opts.epsilon));
    	// Percent tolerance can (correctly) be negative.  However, when
    	// comparing to a limit, we should use the absolute value
    	equals = Math.abs(opts.lastError) <= opts.equalLimit;
    }
    else
    {
    	opts.setLastError(Math.abs(fValue - tmp));
    	equals = opts.lastError <= opts.equalLimit;
    }
    return equals;
  }
  protected void setLastError(double err){lastError=err;}
  
  public static double percentTolerance(double expected, double calculated, double epsilon)
  {
    // Check for 'invalid' numbers first
    if (Double.isNaN(expected) || Double.isNaN(calculated) || Double.isInfinite(expected) || Double.isInfinite(calculated))
      Log.warn("While finding percent tolerance from values 'expected' = " + expected + " and 'calculated' =" + calculated +
        ", invalid values (NaN or Infinity) were found.  Unexpected results may occur.");
    // Handle special cases
    if (expected == 0.0 && calculated == 0.0)
      return 0.0;
    else if (expected == 0.0 || calculated == 0.0)
    {
      if (Math.abs(expected + calculated) < epsilon)
        return 0.0;
      else
      {
        if (expected == 0.0)
          return Double.POSITIVE_INFINITY;
        else if (expected < 0.0)
          return -100.0;
        else
          return 100.0;
      }
    }
    else
      return Math.abs(calculated - expected) / expected * 100.0;
  }

  public static double percentDifference(double expected, double calculated, double epsilon)
  {
    // Check for 'invalid' numbers first
    if (Double.isNaN(expected) || Double.isNaN(calculated) || Double.isInfinite(expected) || Double.isInfinite(calculated))
      Log.warn("While finding percent difference from values 'expected' = " + expected + " and 'calculated' =" + calculated +
        ", invalid values (NaN or Infinity) were found.  Unexpected results may occur.");
    // Handle special cases
    if (expected == 0.0 && calculated == 0.0)
      return 0.0;
    else if (expected == 0.0 || calculated == 0.0)
    {
      if (Math.abs(expected + calculated) < epsilon)
        return 0.0;
      else
        return 200.0;
    }
    else
    {
      double difference = (calculated - expected);
      double average = (calculated + expected) / 2.0;

      if (average == 0.0)
        return Double.POSITIVE_INFINITY;

      return Math.abs(difference / average) * 100.0;
    }
  }

  
  public static boolean lessThan(double value, double limit)
  {
    return lessThan(value,"",limit,"",null);
  }
  public static boolean lessThan(double value, String vUnit, double limit, String lUnit)
  {
    return lessThan(value,vUnit,limit,lUnit,null);
  }
  public static boolean lessThan(double value, double limit, DoubleUtils opts)
  {
    return lessThan(value,"",limit,"",opts);
  }
  public static boolean lessThan(double value, String vUnit, double limit, String lUnit, DoubleUtils opts)
  {
    if(!validDoubles(value,limit))
      return false;

    double tmp=value;
    if(!vUnit.equals(lUnit))
      tmp = UnitConverter.convert(value, vUnit, lUnit);

    if(LessThanOption.Strict.equals(opts.lessThanOption))
    {
      if(tmp<limit)
        return true;            
    }
    else
    {
      if(tmp<limit && !equals(tmp,limit,opts))
        return true;      
    }
    
    return false;
  }
  
  public static boolean lessThanEqualTo(double value, double limit)
  {
    return lessThanEqualTo(value,"",limit,"",null);
  }
  public static boolean lessThanEqualTo(double value, String vUnit, double limit, String lUnit)
  {
    return lessThanEqualTo(value,vUnit,limit,lUnit,null);
  }
  public static boolean lessThanEqualTo(double value, double limit, DoubleUtils opts)
  {
    return lessThanEqualTo(value,"",limit,"",opts);
  }
  public static boolean lessThanEqualTo(double value, String vUnit, double limit, String lUnit, DoubleUtils opts)
  {
    if(!validDoubles(value,limit))
      return false;

    double tmp=value;
    if(!vUnit.equals(lUnit))
      tmp = UnitConverter.convert(value, vUnit, lUnit);

    if(null == opts)
    {
      // In this case, always assume you want a "strict" check
      if(tmp>limit)
        return false;
      else
        return true;      
    }
    else
    {
      if(LessThanOption.Strict.equals(opts.lessThanOption))
      {
        if(tmp>limit)
          return false;
        else
          return true;
      }
      else
      {
        if(tmp<limit || equals(tmp,limit,opts))
          return true;      
      }      
    }
    
    return false;
  }
  
  public static int mantissaLength(double d)
  {
    return Math.abs(BigDecimal.valueOf(d).scale());
  }


  /**
   * lowerLimit <= value <= upperLimit
   * @param lowerLimit
   * @param value
   * @param upperLimit
   * @return
   */
  public static boolean inRange(double lowerLimit, double value, double upperLimit)
  {
    if(upperLimit < lowerLimit)
      Log.warn("While determining if value is in range, upper bound is less than lower bound. Unexpected results may occur.");
    else if(upperLimit == lowerLimit)
      Log.warn("While determining if value is in range, upper bound is equal to lower bound. Unexpected results may occur.");

    return (lessThanEqualTo(lowerLimit, value) && lessThanEqualTo(value, upperLimit));
  }
  
  public static double getMax(List<Double> list)
  {
    double max=-Double.MAX_VALUE;
    for(double d : list)
    {
      if(d>max)
        max=d;
    }
    return max;
  }
  public static double getMax(double[] ary)
  {
    double max=-Double.MAX_VALUE;
    for(double d : ary)
    {
      if(d>max)
        max=d;
    }
    return max;
  }
  
  public static double getMin(List<Double> list)
  {
    double min=Double.MAX_VALUE;
    for(double d : list)
    {
      if(d<min)
        min=d;
    }
    return min;
  }
  public static double getMin(double[] ary)
  {
    double min=Double.MAX_VALUE;
    for(double d : ary)
    {
      if(d<min)
        min=d;
    }
    return min;
  }
  
  public static double getAverage(List<Double> values)
  {
    double sum=0;
    for(double d : values)
      sum += d;
    return sum/values.size();
  }
  public static double getAverage(double[] ary)
  {
    double sum=0;
    for(double d : ary)
      sum += d;
    return sum/ary.length;
  }
  
  public static double getStandardDeviation(List<Double> values)
  {
    double sum=0;
    double avg=getAverage(values);
    for(double d : values)
      sum+=(d-avg)*(d-avg);
    return Math.sqrt(sum/values.size());
  }
  
  private static boolean validDoubles(double a, double b)
  {
    if(Double.isNaN(a)&&!Double.isNaN(b)||
      Double.isNaN(b)&&!Double.isNaN(a)||
      Double.isInfinite(a)&&!Double.isInfinite(b)||
      Double.isInfinite(b)&&!Double.isInfinite(a))
   {
     return false;
   }
    return true;
  }

  public static boolean isZero(double d)
  {
    return d >= -1E-8 && d <= 1E-8;
  }

}
