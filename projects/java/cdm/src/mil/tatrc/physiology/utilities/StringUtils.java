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

import java.text.DecimalFormat;
import java.util.*;

/**
 * The <code>StringUtils</code> provides utils for creating string overviews
 *  of datamodel objects
 * 
 * @author byoung,abray
 *
 */
public class StringUtils
{
	public static void main(String[] args)
  {
		double d = 9.1234567890;
		toString(d,6);
  }
	
	public static boolean exists(String str)
	{
		if(str==null)
			return false;
		if(str.isEmpty()||str.equals(""))
			return false;
		return true;
	}
	
  private static Map<Integer,DecimalFormat> formatters=new HashMap<Integer,DecimalFormat>();
 
  public static DecimalFormat getFormatter(int precision)
  {
  	DecimalFormat df = formatters.get(precision);
		if(df==null)
		{
			StringBuffer format = new StringBuffer("#0.");
			for(int i=0; i<precision; i++)
				format.append("#");
			df=new DecimalFormat(format.toString());
			formatters.put(precision,df);
		}
		return df;
  }
  
  private static Map<Integer,DecimalFormat> scientificFormatters=new HashMap<Integer,DecimalFormat>();
  
  public static DecimalFormat getScientificFormatter(int precision)
  {
    DecimalFormat df = scientificFormatters.get(precision);
    if(df==null)
    {
      StringBuffer format = new StringBuffer("0.");
      for(int i=0; i<precision; i++)
        format.append("#");
      format.append("E0");      
      df=new DecimalFormat(format.toString());
      scientificFormatters.put(precision,df);
    }
    return df;
  }
  
  public static String toString(double d, int precision)
  {
  	// TODO DecimalFormat is a bit slow, check out Java Performance Tuning from O'Reilly
  	StringBuffer sb = new StringBuffer();
  	if(precision>0)
  	{
  		DecimalFormat df = getFormatter(precision);
  	  sb.append(df.format(d));
  	}
  	else
  	{
  		sb.append(d);
  	}
  	return sb.toString();
  }
  
  public static String toString(double[] data)
  {
  	return toString(data,-1);
  }
  
  public static String toString(double[] data, int precision)
  {
  	// TODO DecimalFormat is a bit slow, check out Java Performance Tuning from O'Reilly
  	StringBuffer sb = new StringBuffer();
  	if(precision>0)
  	{
  		DecimalFormat df = getFormatter(precision);

  		for(int i=0; i<data.length; i++)
  		{
  			sb.append(df.format(data[i]));
  			if(i<data.length-1)
  				sb.append(",");
  		}
  	}
  	else
  	{
  		for(int i=0; i<data.length; i++)
  		{
  			sb.append(data[i]);
  			if(i<data.length-1)
  				sb.append(",");
  		}
  	}
  	return sb.toString();
  }
  
  public static String toString(List<Double> data)
  {
  	return toString(data,-1);
  }
  
  public static String toString(List<Double> data, int precision)
  {
  	int idx=0;
  	// TODO DecimalFormat is a bit slow, check out Java Performance Tuning from O'Reilly
  	StringBuffer sb = new StringBuffer();
  	if(precision>0)
  	{
  		DecimalFormat df = getFormatter(precision);

  		for(Double d : data)
  		{
  			sb.append(df.format(d));
  			if(idx++!=data.size())
  				sb.append(",");
  		}
  	}
  	else
  	{
  		for(Double d : data)
  		{
  			sb.append(d);
  			if(idx++!=data.size())
  				sb.append(",");
  		}
  	}
  	return sb.toString();
  }


  public static double[] toArray(String dataString)
  {
  	return toArray(dataString,0);
  }
  
  public static double[] toArray(String dataString, int precision)
  {
  	List<String> ary = new ArrayList<String>(); 

  	String val;
  	int start = 0, end = 0;    
  	while (end < dataString.length())
  	{
  		end = dataString.indexOf(',', start);

  		if (end >= 0)
  		{
  			val = dataString.substring(start, end);
  		}
  		else
  		{
  			val = dataString.substring(start);
  			end = dataString.length();
  		}
  		val = val.trim();
  		if (val.equals(""))
  		{
  			val = "0";
  		}
  		ary.add(val);
  		start = end + 1;
  	}
  	double[] d = new double[ary.size()];
  	for(int i=0; i<d.length; i++)
  	{
  		d[i] = Double.valueOf(ary.get(i));
  	}
  	return d;
  }
  
  /**
   * List will be cleared internally
   * @param dataString - Comma delimeted string - TODO space delimited support
   * @param list
   */
  public static void toList(String dataString, List<Double> list)
  {
  	list.clear();
  	List<String> ary = new ArrayList<String>(); 

  	String val;
  	int start = 0, end = 0;    
  	while (end < dataString.length())
  	{
  		end = dataString.indexOf(',', start);// TODO support space delimited too

  		if (end >= 0)
  		{
  			val = dataString.substring(start, end);
  		}
  		else
  		{
  			val = dataString.substring(start);
  			end = dataString.length();
  		}
  		val = val.trim();
  		if (val.equals(""))
  		{
  			val = "0";
  		}
  		ary.add(val);
  		start = end + 1;
  	}
  	for(int i=0; i<ary.size(); i++)
  	{
  		list.add(Double.valueOf(ary.get(i)));
  	}
  }
  
  public static String removeParens(String str)
  {
  	int openIdx = str.indexOf("(");
  	if(openIdx==-1)
  		return str;
  	int closeIdx = str.indexOf(")");
  	return str.substring(0,openIdx) + str.substring(closeIdx+1);
  }
  
  public static String spaceCamelCase(String s)
  {
  	String out="";
  	String[] words = s.split("(?<!(^|[A-Z]))(?=[A-Z])|(?<!^)(?=[A-Z][a-z])"); 
    for (int i=0; i<words.length; i++) 
    {
        out=out+words[i];
        if(i<words.length-1)
        	out=out+" ";
    }
    return out;
  }
  
  public static String removeSpaces(String s)
  {
    return s.replaceAll(" ", "");
  }
  
  public static String[] GetEnumNames(Enum<?>[] enumValues)
  {
    String[] enumNames = new String[enumValues.length];

    int index = 0;
    for (Enum<?> value : enumValues)
    {
      enumNames[index++] = String.valueOf(value);
    }

    return enumNames;
  }
}
