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
package mil.tatrc.physiology.datamodel.doxygen;

/**
 * Captures the description elements for a particular data type
 */
public class Description
{
  /** Fully qualified reference to the base class name (if this is an extension type) */
  public String base = "";
  /** Description from doxygen text */
  public String desc = "";
  /** Fully qualified reference type */
  public String ref = "";
  /** minOccurs and maxOccurs */
  public String minMaxOccurs = "";
  
  @Override
  public String toString()
  {
    if (base.length() > 0 || ref.length() > 0 || minMaxOccurs.length() > 0)
    {
      String extendsText = "";
      String refText = "";
      String minMaxOccursText = "";
      
      if (base.length() > 0)
      {
        extendsText = "<br/>(Extends @ref "+base+")";
      }
      if (ref.length() > 0)
      {
        refText = "<br/>(Type @ref "+ref+")";
      }
      if (minMaxOccurs.length() > 0)
      {
        minMaxOccursText = "<br/>("+minMaxOccurs+")";
      }
      
      return desc;//+refText+extendsText+minMaxOccursText;
    }
    return desc;
  }
}
