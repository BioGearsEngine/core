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

import java.util.HashSet;

/**
 * XSD elements that can carry documentation
 */
public enum XSDType
{
  SCHEMA("schema"),
  ELEMENT("element"),
  COMPLEXTYPE("complexType"),
  SIMPLETYPE("simpleType"),
  ATTRIBUTE("attribute"),
  ENUMERATION("enumeration");
  
  private String xsdTypeName;
  private static HashSet<String> typeNames = new HashSet<String>();
  
  private XSDType(String typeName)
  {
    xsdTypeName = typeName;
  }
  
  public String getTypeName()
  {
    return xsdTypeName;
  }
  
  /**
   * Returns true if the XSD element type is a documentable type
   * @param typeName
   * @return
   */
  public static boolean isDocType(String typeName)
  {
    if (typeNames.isEmpty())
    {
      for (XSDType type : values())
      {
        typeNames.add(type.getTypeName());
      }
    }
    return typeNames.contains(typeName);
  }
}
