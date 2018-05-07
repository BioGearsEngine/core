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

import java.util.*;

/**
 * The <code>TimingProfile</code>
 * 
 * @author byoung
 *
 */
public class TimingProfile
{
  private static TimingProfile me = new TimingProfile();
  private class Timing
  {
    double start=0;
    double elapsed=0;
  }
  private static Map<String,Timing> fProfiles = new HashMap<String,Timing>();   
  
  /**
   * Clears all labels and times
   */
  public static void clear()
  {
    fProfiles.clear();
  }
  
  private static boolean log=false;
  public static void logProfiles(boolean b)
  {
    log=b;
  }
  
  /**
   * Returns run time in seconds
   * @param label
   * @return
   */
  public static double profile(String label)
  {
    Timing timing=fProfiles.get(label);
    if(timing==null)
    {
      timing = me.new Timing(); 
      fProfiles.put(label, timing);
    }
    if(timing.start==0)
      timing.start=System.currentTimeMillis()*0.001;
    double elapsed = getElapsed(fProfiles.get(label));
    if(log)
      Log.info("TimingProfile:Profile:"+label+elapsed); 
    return elapsed;
  }
  
  public static double profile(String label, String desc)
  {
    Timing timing=fProfiles.get(label);
    if(timing==null)
    {
      timing = me.new Timing(); 
      fProfiles.put(label, timing);
    }
    if(timing.start==0)
      timing.start=System.currentTimeMillis()*0.001;
    double elapsed = getElapsed(fProfiles.get(label));
    if(log)
      Log.info("TimingProfile:Profile:"+label+" "+desc+" "+elapsed);
    return elapsed;
  }

  public static void start(String label)
  {
    Timing timing=fProfiles.get(label);
    if(timing==null)
    {
      timing = me.new Timing(); 
      fProfiles.put(label, timing);
    }
    timing.start=System.currentTimeMillis()*0.001;
    
    if(log)
      Log.info("TimingProfile:Start:"+label);
  }
  
  public static double stop(String label)
  {
  	double elapsed=profile(label,"Stop");
    fProfiles.remove(label);
    return elapsed;
  }
  
  public static double pause(String label)
  {
    Timing timing=fProfiles.get(label);
    if(timing==null)
    {
      timing = me.new Timing(); 
      fProfiles.put(label, timing);
    }
    if(timing.start==0)
      timing.start=System.currentTimeMillis()*0.001;
    
    timing.elapsed += getElapsed(fProfiles.get(label));
    if(log)
      Log.info("TimingProfile:Pause:"+label+" "+timing.elapsed);
    timing.start=0L;
    return timing.elapsed;
  }
  
  private static double getElapsed(Timing timing)
  {
    return (System.currentTimeMillis()*0.001)-timing.start;    
  }
  
  public static void print(String label)
  {
    if(!fProfiles.containsKey(label))
      System.out.println("Unregistered label "+label);
    else
      System.out.println("TimingProfile:Elapsed:"+label+":"+profile(label));
  }
  
  public static void print()
  {
    for(String label : fProfiles.keySet())
      System.out.println("TimingProfile:Elapsed:"+label+":"+profile(label));
  }
}
