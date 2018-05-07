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

import java.io.Serializable;

/**
 * The <code>Pair</code> contains two objects, paired together
 */
public class Pair<L,R> implements Serializable
{
  private L l;
  private R r;
  
  public Pair(L l, R r)
  { 
    this.l = l;
    this.r = r;   
  }
  
  @Override
  public boolean equals(Object o)
  {
    if (o == null || o.getClass() != getClass())
    {
      return false;
    }
    else
    {
      @SuppressWarnings("unchecked")
      Pair<L,R> in = (Pair<L,R>)o;
      if (!this.getL().equals(in.getL()) || !this.getR().equals(in.getR()))
      {
        return false;
      }
    }
      
    return true;
  }
  
  @Override
  public int hashCode()
  {
    int hash = 5;
    hash = 11 * hash + this.l.hashCode();
    hash = 11 * hash + this.r.hashCode();
    return hash; 
  }
 
  public L getL()
  {
    return this.l;
  }
 
  public void setL(L l) 
  {
    this.l = l;
  }
  
  public R getR() 
  {
    return this.r;
  }
  
  public void setR(R r) 
  {
    this.r = r;
  } 
  
  public String toString()
  { 
    return "(" + 
      (l==null  ? "null" : l.toString()) + ", " + 
      (r==null ? "null" : r.toString()) + ")"; 
  }
  
  /**
   * Check if both parts of the pair are set (non-null)
   * 
   * @return
   */
  public boolean isPaired()
  {
    return l != null && r != null;
  }
}

