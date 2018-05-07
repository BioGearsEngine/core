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


package mil.tatrc.physiology.datamodel;

import java.io.PrintStream;
import java.lang.reflect.Method;
import java.util.*;

import mil.tatrc.physiology.datamodel.properties.SEFunction;
import mil.tatrc.physiology.utilities.DoubleUtils;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.StringUtils;

/**
 * @author boday
 *
 */
public class SEEqualOptions extends DoubleUtils
{
  /**
   * Use this as the name when a derived class
   * calls a parent class.equals method, so 
   * the parent class name does not get added 
   * to the path
   */
  public static final String PARENT_CLASS = "parentClass";
  
  public boolean compareID = true;
  public boolean compareName = true;
  public boolean returnOnFirstDiff = false;
  public boolean compareListsByOrder = true;
  
  protected boolean trackDifferences = false;
  protected boolean trackError = false;
  
  public class PathTrack
  {
    protected ArrayList<String> fromIDPath;
    protected ArrayList<String> toIDPath;
    protected ArrayList<String> CDMPath;
    
    public PathTrack()
    {
      this.fromIDPath = new ArrayList<String>();
      this.toIDPath = new ArrayList<String>();
      this.CDMPath = new ArrayList<String>();
    }
    
    public PathTrack(PathTrack pt)
    {
      this.fromIDPath = new ArrayList<String>(pt.fromIDPath);
      this.toIDPath = new ArrayList<String>(pt.toIDPath);
      this.CDMPath = new ArrayList<String>(pt.CDMPath);
      
      for(int i=0; i<this.CDMPath.size();)
      {
        if(SEEqualOptions.PARENT_CLASS.equals(this.CDMPath.get(i)))
        {
          this.CDMPath.remove(i);
          this.toIDPath.remove(i);
          this.fromIDPath.remove(i);
        }
        else
          i++;
      }    
    }
    
    public String getCDMPath(){return concatList(this.CDMPath);}
    public String getToIDPath(){return concatList(this.toIDPath);}
    public String getFromIDPath(){return concatList(this.fromIDPath);}
    
    protected String concatList(List<String> list)
    {
      StringBuilder str = new StringBuilder();
      for(String s : list)
      {
        if(s!=null&&s.equals("-1"))
          continue;
        str.append(s+"/");
      }
      return str.toString();
    }
    
    public int hashCode()
    {
      return this.CDMPath.hashCode() + 
             this.fromIDPath.hashCode() + 
             this.toIDPath.hashCode(); 
    }
    
    public boolean equals(Object o)
    {
      if(o==this) 
        return true;
      if(o==null || !(o instanceof PathTrack)) 
        return false;
      PathTrack cp= PathTrack.class.cast(o);
      if(!this.CDMPath.equals(cp.CDMPath))
        return false;
      if(!this.fromIDPath.equals(cp.fromIDPath))
        return false;
      if(!this.toIDPath.equals(cp.toIDPath))
        return false;
      return true;
    }
  }
  protected PathTrack tmpPathTrack;
  
  public class Track 
  {
    protected List<String> differences;
    protected List<Double> equalError;
    
    public Track()
    {
      this.differences = new ArrayList<String>();
      this.equalError = new ArrayList<Double>();
    }
    
    protected Track(Track t)
    {
      this.differences = new ArrayList<String>(t.differences);
      this.equalError = new ArrayList<Double>(t.equalError);
    }
    
    public List<String> getDifferences()
    {
      return Collections.unmodifiableList(differences);
    }
    
    public List<Double> getEqualError()
    {
      return Collections.unmodifiableList(equalError);
    }
    
    public void add(Track t)
    {
      this.differences.addAll(t.differences);
      this.equalError.addAll(t.equalError);
    }
  }
  protected Track tmpTrack;

  protected Map<PathTrack, Track> mapTrack = new HashMap<PathTrack, Track>();
  
  public SEEqualOptions()
  {
    super();
  }
  
  public SEEqualOptions(DoubleUtils.EqualMode mode, double equalLimit)
  {
    super(mode,equalLimit);
  }
  
  public void set(SEEqualOptions options)
  {
    super.set(options);
    this.compareID = options.compareID;
    this.compareName = options.compareName;
    this.returnOnFirstDiff = options.returnOnFirstDiff;
    this.compareListsByOrder = options.compareListsByOrder;
    this.trackDifferences = options.trackDifferences;
    this.trackError = options.trackError;
  }
  
  // set all flags to correspond to "contents only"
  public void setContentsOnly(boolean set)
  {
    this.compareID = !set;
    this.compareName = !set;
  }
  
  public boolean getContentsOnly()
  {
    return !this.compareID && !this.compareName;
  }
  
  public void trackError(boolean b)
  {
    trackError = b;
  }
  
  public boolean isTrackingError()
  {
    return trackError;
  }
  
  public void trackDifferences(boolean b)
  {
    trackDifferences = b;
  }

  public boolean isTrackingDifferences()
  {
    return trackDifferences;
  }
  
  public void trackDifference(String diff)
  {
    if (trackDifferences)
    {
      addDifference(diff);
    }
  }
  
  public void trackNull()
  {
    if (trackDifferences)
    {
      addDifference("null");
    }
  }
  
  public void trackInvalid(String name, SEFunction from, SEFunction to)
  {
    if (trackDifferences)
    {
      String string1 = (from!=null&&from.isValid()) ? from.toString() : "invalid";
      String string2 = (to!=null&&to.isValid()) ? to.toString() : "invalid";
      
      addDifference(name + ": " + string1 + " != " + string2);
    }
  }
  
  public void trackInvalid(String name)
  {
    if (trackDifferences)
    {
      addDifference(name + ": Invalid");
    }
  }
  
  public void track(String name, String value1, String value2)
  {
    if (trackDifferences)
    {    
      addDifference(name + ": " + value1 + " != " + value2);
    }
  }
   
  public void track(String name, boolean value1, boolean value2)
  {
    if (trackDifferences)
    {    
      addDifference(name + ": " + Boolean.toString(value1) + " != " + Boolean.toString(value2));
    }
  }
  
  public void track(String name, double value1, double value2)
  {
    if (trackDifferences)
    {    
      // Assumes you are logging right after the equals call, for proper error
      String post = EqualMode.AbsoluteError.equals(this.mode) ? "" : "%";
      addDifference(name + ": " + value1 + " != " + value2 +"["+StringUtils.toString(this.lastError,3)+post+"]");
    }
    if (trackError)
    {
      addEqualError(this.lastError);
    }
  }
  
  public void track(String name, double value1, String unit1, double value2, String unit2)
  {
    if (trackDifferences)
    {    
      // Assumes you are logging right after the equals call
      String post = EqualMode.AbsoluteError.equals(this.mode) ? "" : "%";
      addDifference(name + ": " + value1 + " " + unit1 + " != " + value2 + " " + unit2 + "[" + StringUtils.toString(this.lastError, 3) + post + "]");
    }
    if (trackError)
    {
      addEqualError(this.lastError);
    }
  }
  
  public void track(String name, int value1, int value2)
  {
    if (trackDifferences)
    {    
      addDifference(name + ": " + value1 + " != " + value2);
    }
  }
  
  public void track(String name, Object value1, Object value2)
  {
    if (trackDifferences)
    {
      String string1 = (value1 != null) ? value1.toString() : "null";
      String string2 = (value2 != null) ? value2.toString() : "null";
      
      addDifference(name + ": " + string1 + " != " + string2);
    }
  }
  
  public void track(double value1, String unit1, double value2, String unit2)
  {
    if (trackDifferences)
    {    
      // Assumes you are logging right after the equals call
      String post = EqualMode.AbsoluteError.equals(this.mode) ? "" : "%";
      addDifference(value1 + " " + unit1 + " != " + value2 + " " + unit2 + "[" + StringUtils.toString(this.lastError, 3) + post + "]");
    }
    if (trackError)
    {
      addEqualError(this.lastError);
    }
  }
  
  public void track(String value1, String value2)
  {
    if (trackDifferences)
    {     
      addDifference(value1 + " != " + value2);
    }
  }
  
  public void pushTrack(String CDMPath, String fromID, String toID)
  {
    if (trackDifferences||trackError)
    {
      if (this.tmpPathTrack == null)
        this.tmpPathTrack = new PathTrack();
      
      String[] CDMPathList = CDMPath.split("/");
      for (String str : CDMPathList)
        this.tmpPathTrack.CDMPath.add(str);
      
      if (fromID != null)
      {
        String[] fromIDPathList = fromID.split("/");
        for (String str : fromIDPathList)
          this.tmpPathTrack.fromIDPath.add(str);
      }
      else
        this.tmpPathTrack.fromIDPath.add("-1");
      
      if (toID != null)
      {
        String[] toIDPathList = toID.split("/");
        for (String str : toIDPathList)
          this.tmpPathTrack.toIDPath.add(str);
      }
      else
        this.tmpPathTrack.toIDPath.add("-1");
    }
  }
  
  public void pushTrack(String CDMPath)
  {
    if (trackDifferences||trackError)
    {
      if (this.tmpPathTrack == null)
        this.tmpPathTrack = new PathTrack();
      
      String[] CDMPathList = CDMPath.split("/");
      for (String str : CDMPathList)
        this.tmpPathTrack.CDMPath.add(str);
      this.tmpPathTrack.fromIDPath.add("-1");
      this.tmpPathTrack.toIDPath.add("-1");
    }
  }
  
  public void addDifference(String str)
  {
    Track track = this.mapTrack.get(tmpPathTrack);
    if (track == null)
    {
      track = new Track();
      this.mapTrack.put(new PathTrack(tmpPathTrack), track);
    }
    
    track.differences.add(str);
  }
  
  public void addEqualError(double error)
  {
    Track track = this.mapTrack.get(tmpPathTrack);
    if (track == null)
    {
      track = new Track();
      this.mapTrack.put(new PathTrack(tmpPathTrack), track);
    }
    
    track.equalError.add(error);
  }
  
  public Map<PathTrack, Track> getMapTrack()
  {
    return Collections.unmodifiableMap(this.mapTrack);
  }
  
  public boolean hasPath(String CDMPath)
  {   
    return hasPath(CDMPath, "-1", "-1");
  }
  
  public boolean hasPath(String CDMPath, String fromID, String toID)
  {
    if (this.tmpPathTrack == null)
      return false;
    
    boolean hasPath = true;
    int index = tmpPathTrack.CDMPath.size() - 1;
    if (index >= 0 && tmpPathTrack.CDMPath.get(index).equals(CDMPath))
      hasPath &= true;
    index = tmpPathTrack.fromIDPath.size() - 1;
    if (index >= 0 && tmpPathTrack.fromIDPath.get(index).equals(CDMPath))
      hasPath &= true;
    index = tmpPathTrack.toIDPath.size() - 1;
    if (index >= 0 && tmpPathTrack.toIDPath.get(index).equals(CDMPath))
      hasPath &= true;
    
    return hasPath;
  }
  
  public void popTrack()
  {
    if ((trackDifferences||trackError) && tmpPathTrack != null)
    {
      int index = tmpPathTrack.CDMPath.size() - 1;
      if (index >= 0)
        tmpPathTrack.CDMPath.remove(index);
      index = tmpPathTrack.fromIDPath.size() - 1;
      if (index >= 0)
      tmpPathTrack.fromIDPath.remove(index);
      index = tmpPathTrack.toIDPath.size() - 1;
      if (index >= 0)
      tmpPathTrack.toIDPath.remove(index);
    }
  }
  
  public void popAll()
  {
    if ((trackDifferences||trackError) && tmpPathTrack != null)
    {
      tmpPathTrack.CDMPath.clear();
      tmpPathTrack.fromIDPath.clear();
      tmpPathTrack.toIDPath.clear();
    }
  }
  
  public void trackToMap()
  {
    popTrack();
  }
  
  public void appendTrackMap(Map<PathTrack,Track> map)
  {
    Track fromTrack;
    Track myTrack;
    for(PathTrack fromKey : map.keySet())
    {
      fromTrack = map.get(fromKey);
      myTrack = mapTrack.get(fromKey);
      if(myTrack==null)
      {
        myTrack = new Track();
        mapTrack.put(fromKey, myTrack);
      }     
      myTrack.equalError.addAll(fromTrack.equalError);
      if(myTrack.differences.isEmpty()&&!fromTrack.equalError.isEmpty()&&!fromTrack.differences.isEmpty())
        myTrack.differences.add(fromTrack.differences.get(0));
    }
  }
  
  public void clearTracks()
  {
    this.mapTrack.clear();
  }
    
  public void outputLog(PrintStream stream)
  {
    stream.println("compareID: " + compareID);
    stream.println("compareName: " + compareName);
    stream.println("returnOnFirstDiff: " + returnOnFirstDiff);
    //stream.println("compareListsByOrder: " + Boolean.toString(compareListsByOrder));
    
    if (!trackDifferences && !trackError)
    {
      stream.println("trackError: false");
      stream.println("trackDifferences: false");
    }
    else
    {
      stream.println("Equal Mode options: ");
      if(DoubleUtils.EqualMode.PercentDifference.equals(mode))
        stream.println("percentDifference: " + equalLimit);
      else if(DoubleUtils.EqualMode.PercentTolerance.equals(mode))
        stream.println("percentTolerance: " + equalLimit);
      else
        stream.println("absoluteError: " + equalLimit);
      
      if (mapTrack.isEmpty())
      {
        stream.println("No Tracked differences/error");
      }
      
      stream.println("trackError:" + trackError);
      stream.println("trackDifferences:" + trackDifferences);
      
      Track trak;
      for(PathTrack key : mapTrack.keySet())
      {     
        stream.println("");
        stream.println("CDMPath : "+key.getCDMPath());
        stream.println("To IDPath : "+key.getToIDPath());
        stream.println("From IDPath : "+key.getFromIDPath());
        
        trak = mapTrack.get(key);
        if(trackError)
        {
          stream.println("Equal Error :");
          stream.println(trak.equalError.toString());
        }
        if(trackDifferences)
        {
          stream.println("Differences :");
          for(String s : trak.differences)
            stream.println(s);
        }
      }
      
    }
    stream.println("\n");
  }
  
  public void LogErrors()
  {
    if (mapTrack.isEmpty())
      return;
    
    Track trak;
    for(PathTrack key : mapTrack.keySet())
    {          
      trak = mapTrack.get(key);
      if(trackError)
      {
        if (!trak.equalError.isEmpty())
        {
          Log.error("Equal Error :" + trak.equalError.toString());
        }
      }
      if(trackDifferences)
      {
        for(String s : trak.differences)
          Log.error(s);
      }
    }
  }
   
  public boolean compareLists(List<?> from, List<?> to)
  {
    if (this.compareListsByOrder)
      return compareListsByOrder(from, to);
    
    return compareListsByContainment(from, to);
  }
  
  private boolean compareListsByOrder(List<?> from, List<?> to)
  {
    boolean equals = true;
    Class<?> listClass = from.getClass();
    
    if (from.size() == to.size())
    {
      Class<?>[] paramTypes = new Class<?>[3];
      paramTypes[0] = listClass;
      paramTypes[1] = listClass;
      paramTypes[2] = SEEqualOptions.class;
      Method equalsMethod = null;
      try
      {
        equalsMethod = listClass.getMethod("equals", paramTypes);
      }
      catch (Exception ex)
      {
        Log.error(ex);
        return false;
      }
      
      Object[] args = new Object[3];
      args[2] = this;
      for (int i=0; i< from.size(); i++)
      {   
        args[0] = from.get(i);
        args[1] = to.get(i);
        
        try
        {
          if (((Boolean) equalsMethod.invoke(listClass, args)).booleanValue() == false)
          {
            equals = false;
            trackDifference(listClass.getName() + " lists at index " + Integer.toString(i) + "are not equal");
            if (this.returnOnFirstDiff)
              return false;
          }
        }
        catch (Exception ex)
        {
          Log.error(ex);
          return false;
        }
      }
    }
    else
    {
      equals = false;
      trackDifference(listClass.getName() + " list sizes are not equal");
      return false;
    }
  
    return equals;
  }
  
  private boolean compareListsByContainment(List<?> from, List<?> to)
  {
    boolean equals = true;
    Class<?> listClass = from.getClass();
    
    if (from.size() == to.size())
    {
      Class<?>[] paramTypes = new Class<?>[3];
      paramTypes[0] = listClass;
      paramTypes[1] = listClass;
      paramTypes[2] = SEEqualOptions.class;
      Method equalsMethod = null;
      Method idMethod = null;
      try
      {
        equalsMethod = listClass.getMethod("equals", paramTypes);
        idMethod = listClass.getMethod("ID");
      }
      catch (Exception ex)
      {
        Log.error(ex);
        return false;
      }
      
      Object[] args = new Object[3];
      args[2] = this;
      
      String id1, id2;
      for (int i = 0; i < from.size(); i++)
      {   
        args[0] = from.get(i);
        try 
        {
          id1 = (String)idMethod.invoke(args[0]);
        }
        catch (Exception ex)
        {
          Log.error(ex);
          return false;
        }
        
        boolean bFoundID = false;
        for (int j = 0; j < to.size(); j++)
        {
          args[1] = to.get(j);
          try
          {
            id2 = (String)idMethod.invoke(args[1]);
          }
          catch (Exception ex)
          {
            Log.error(ex);
            return false;
          }
          
          if (id1.equals(id2))
          {
            bFoundID = true;
            try
            {
              if (((Boolean) equalsMethod.invoke(listClass, args)).booleanValue() == false)
              {
                equals = false;
                trackDifference(listClass.getName() + " lists at index " + Integer.toString(i) + "are not equal");
                if (this.returnOnFirstDiff)
                  return false;
              }
            }
            catch (Exception ex)
            {
              Log.error(ex);
              return false;
            }
          }
        }
        if (!bFoundID)
        {
          equals = false;
          trackDifference(listClass.getName() + " list does not contain ID: " + id1);
          if (this.returnOnFirstDiff)
            return false;
        }
      }
    }
    else
    {
      equals = false;
      trackDifference(listClass.getName() + " list sizes are not equal");
      return false;
    }
  
    return equals;
  }
}
