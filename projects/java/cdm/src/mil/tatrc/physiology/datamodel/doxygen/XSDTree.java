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

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Documentation tree of XSD elements
 */
public class XSDTree
{
  private static Pattern defGroup = Pattern.compile("@defgroup\\s+([^@]\\S*)(\\s+[^@]\\S*)?");
  
  /** Name of the element (e.g. "CodeCitationData") */
  public String name;
  /** Comment(s) attached to the element or empty string */
  public String comment;
  /** Element type (e.g. "StringData") */
  public String elementType;
  /** XSD type (e.g. "complexType") */
  public XSDType xsdType; 
  
  /** minOccurs from XSD schema */
  public String minOccurs;
  /** maxOccurs from XSD schema */
  public String maxOccurs;
  
  public List<XSDTree> children = new ArrayList<XSDTree>();
  public XSDTree parent;
  public String extension;
  
  public DefGroup getDefGroup()
  {
    DefGroup rtn = new DefGroup();
    StringBuffer sb = new StringBuffer();
    XSDTree t = this;
    while (t != null)
    {
      if (sb.length() > 0)
      {
        sb.insert(0, '_');
      }
      sb.insert(0, t.name);
      t = t.parent;
    }
    rtn.fullName = sb.toString();

    Matcher matcher = defGroup.matcher(comment);
    if (matcher.find())
    {
      if (matcher.groupCount() == 1)
      {
        rtn.printableName = matcher.group(1);
        rtn.bareName = name;
      }
      else
      {
        rtn.fullName = matcher.group(1);
        rtn.bareName = rtn.fullName;
        rtn.printableName = matcher.group(2).trim();
      }
    }
    else
    {
      rtn.bareName = name;
      rtn.printableName = name;
    }
    
    return rtn;
  }
  
  /**
   * Create a Description object for rendering the documentation
   * @param typeMap
   * @return
   */
  public Description getDescription(Map<String, String> typeMap)
  {
    Description rtn = new Description ();
    
    String commentNoRef = comment;
    

    if (elementType != null && !elementType.contains(":"))
    {
      String longElementType = typeMap.get(elementType);
      if (longElementType == null)
      {
        longElementType = elementType;
      }
      rtn.ref = longElementType;
    }
    if (extension != null && extension.length() > 0 && !extension.contains(":"))
    {
      String longElementType = typeMap.get(extension);
      if (longElementType == null)
      {
        longElementType = extension;
      }
      rtn.base = longElementType;
    }
    if (minOccurs != null || maxOccurs != null)
    {
      StringBuffer minMaxOccurs = new StringBuffer();
      if (minOccurs != null)
      {
        minMaxOccurs.append("minOccurs:"+minOccurs);
      }
      if (maxOccurs != null)
      {
        if (minMaxOccurs.length() > 0) minMaxOccurs.append(" ");
        minMaxOccurs.append("maxOccurs:"+maxOccurs);
      }
      rtn.minMaxOccurs = minMaxOccurs.toString();
    }
    
    rtn.desc = commentNoRef;
    
    return rtn;
  }
  
  /**
   * Collapse all of the enumeration fields into the parent node
   */
  public void collapseEnumeration()
  {
    StringBuffer enumTypes = new StringBuffer();
    if (children.isEmpty()) return;
    
    for (XSDTree child : children)
    {
      if (child.xsdType != XSDType.ENUMERATION)
      {
        return;
      }
      if (!child.children.isEmpty())
      {
        return;
      }
      enumTypes.append("\n - "+child.name);
      if (child.comment.length() > 0)
      {
        enumTypes.append(": "+child.comment);
      }
    }
    
    comment += enumTypes.toString();
    children.clear();
  }
}
