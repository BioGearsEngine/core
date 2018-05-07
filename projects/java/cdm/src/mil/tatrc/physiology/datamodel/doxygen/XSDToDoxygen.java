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

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.jdom2.Comment;
import org.jdom2.Content;
import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.Parent;
import org.jdom2.input.SAXBuilder;

/**
 * 
 */
public class XSDToDoxygen
{
  private static final String XS = "http://www.w3.org/2001/XMLSchema";
  
  private File inputFile;
  private File outputFile;
  private Document xsdDocument;
  private Element schemaElement;
  private PrintWriter ixxWriter;
  private LinkedList<File> filesToProcess = new LinkedList<File>();
  private LinkedList<XSDTree> roots = new LinkedList<XSDTree>();
  private HashSet<File> filesProcessed = new HashSet<File>();
  
  private XSDTree xsdRoot;
  /** Stack of XSDTree nodes that are currently in scope starting from the xs:schema */
  private LinkedList<XSDTree> xsdStack = new LinkedList<XSDTree>();
  /** 
   * Map of the long fully qualified defgroup name to the xsd type name 
   * e.g CodeCitationData -> Codes_CodeCitationData
   */
  private HashMap<String, String> typeNameMap = new HashMap<String, String>();
  
  public XSDToDoxygen(File inputFile, File outputFile)
  {
    this.outputFile = outputFile;
    filesToProcess.add(inputFile);
  }
  
  public void run() throws Exception
  {
    while (!filesToProcess.isEmpty())
    {
      inputFile = filesToProcess.removeFirst();
      System.out.println ("Reading: "+inputFile.getAbsolutePath());
      
      parse();
      xsdRoot = elementToTreeNode(schemaElement);
      if (xsdRoot.comment.contains("@defgroup"))
      {
        DefGroup dg = xsdRoot.getDefGroup();
        xsdRoot.name = dg.bareName;
      }
      else
      {
        String fn = inputFile.getName();
        xsdRoot.name = fn.substring(0, fn.indexOf('.'));
      }
      
      xsdStack.addLast(xsdRoot);
      processSchemaElement(schemaElement);
      
      roots.add(xsdRoot);
      xsdStack.clear();
    }
    
    try 
    {
      System.out.println ("Writing: "+outputFile.getAbsolutePath());
      ixxWriter = new PrintWriter(new FileWriter(outputFile));
      for (XSDTree root : roots)
      {
        ixxWriter.println ("/**");
        writeTree(root);
        ixxWriter.println (" */");
        ixxWriter.println ();
      }
    }
    finally
    {
      ixxWriter.close();
    }
  }
  
  private void parse() throws Exception
  {
    SAXBuilder jdomBuilder = new SAXBuilder();
    xsdDocument = jdomBuilder.build(inputFile);
    schemaElement = xsdDocument.getRootElement();
    if (!XS.equals(schemaElement.getNamespace().getURI()))
    {
      throw new Exception ("Unexpected namespace: "+schemaElement.getNamespace());
    }
  }

  /**
   * Walk throug the Schema DOM tree building the XSDTree 
   * @param element
   * @throws IOException 
   */
  private void processSchemaElement(Element element) throws IOException
  {
    List<Element> children = element.getChildren();
    for (Element child : children)
    {
      String childName = child.getName();
      if (XSDType.isDocType(childName))
      {
        XSDTree childTree = elementToTreeNode(child);
        childTree.parent = xsdStack.getLast();
        
        // This can't be done until the parent pointer is linked up.
        if (
          childTree.xsdType == XSDType.COMPLEXTYPE ||
          childTree.xsdType == XSDType.SIMPLETYPE)
        {
          DefGroup group = childTree.getDefGroup();
          typeNameMap.put(childTree.name, group.fullName);
        }
        
        if (childTree.xsdType == XSDType.ENUMERATION)
        {
          extractCommentFromParent(childTree);
        }
        
        if (childTree.xsdType == XSDType.ELEMENT)
        {
          childTree.minOccurs = child.getAttributeValue("minOccurs");
          childTree.maxOccurs = child.getAttributeValue("maxOccurs");
        }
        
        xsdStack.getLast().children.add(childTree);
        xsdStack.addLast(childTree);
        processSchemaElement(child);
        xsdStack.removeLast();
      }
      else if(childName.equals("extension") || childName.equals("restriction"))
      {
        String base = child.getAttributeValue("base");
        if (base != null)
        {
          xsdStack.getLast().extension = base;
        }
        processSchemaElement(child);
      }
      else if(childName.equals("include"))
      {
        String location = child.getAttributeValue("schemaLocation");
        File locationFile = 
          new File(inputFile.getParentFile(), location).getCanonicalFile();
        if (!filesProcessed.contains(locationFile))
        {
          filesToProcess.add(locationFile);
          filesProcessed.add(locationFile);
        }
      }
      else
      {
        processSchemaElement(child);
      }
    }
  }

  /**
   * Searches for the enumeration value documentation in the parent comment.
   * If the text is there, remove it from the parent and move it down into 
   * the enumeration value. (See the documentationfor enumHEMPTAPSE3TerrainLayeredConductivityPreference)
   * @param enumeration Enumeration value to check
   */
  private void extractCommentFromParent(XSDTree enumeration)
  {
    XSDTree parentType = enumeration.parent;
    if (parentType.comment != null && parentType.comment.contains(enumeration.name))
    {
      int enumValueIndex = parentType.comment.indexOf(enumeration.name);
      int enumStartIndex = parentType.comment.lastIndexOf('\n', enumValueIndex);
      int enumEndIndex = parentType.comment.indexOf('\n', enumValueIndex);
      if (enumEndIndex < 0) enumEndIndex = parentType.comment.length();
      
      // Extract the comment for this enum value, remove the name
      String enumComment = parentType.comment.substring(enumStartIndex+1, enumEndIndex).trim();
      
      // Peform a sanity check, the comment should contain a colon to indicate
      // that the comment is defining this value (vs. possibly mentioning it
      // in passing)
      if (enumComment.contains(":"))
      {
        Pattern p = Pattern.compile("^\\([0-9]+\\)");
        Matcher m = p.matcher(enumComment);
        enumComment = m.replaceAll("");
        // Delete the name because this is kept in enumeration.name
        enumComment = enumComment.replace(enumeration.name, "");
        // Delete the "": surrounding where the name was
        enumComment = enumComment.replace("\"\":", "");
        
        // Store the result in the comment
        enumeration.comment = enumComment.trim();
        
        // Delete the redundant line from the parent element comment
        StringBuffer parentComment = new StringBuffer(parentType.comment);
        parentComment.delete(enumStartIndex, enumEndIndex);
        parentType.comment = parentComment.toString();
      }
    }
  }
  
  private void writeTree(XSDTree tree)
  {
    //tree.collapseEnumeration();
    
    DefGroup defgroup = tree.getDefGroup();
    println (defgroup.toString());
    Description description = tree.getDescription(typeNameMap);
    String descriptionString = description.toString();
    if (descriptionString.length() > 0)
    {
      println (descriptionString);
    }
    
    /*
    if (!tree.comment.contains("@defgroup"))
    {
      println ("@defgroup "+tree.name);
    }
    if (tree.xsdType == XSDType.ELEMENT)
    {
      if (!tree.elementType.contains(":"))
      {
        println ("@brief @ref "+tree.elementType);
      }
    }
    println (tree.comment);
    */
    
    if (!tree.children.isEmpty())
    {
      println ("@{");
      for (XSDTree child : tree.children)
      {
        writeTree(child);
      }
      println ("@}");
      println ();
    }
  }
  
  /**
   * Create an XSDTree element from a schema XML element
   * @param element Schema XML element
   * @return
   */
  private XSDTree elementToTreeNode(Element element)
  {
    XSDTree treeNode = new XSDTree();
    for (XSDType type : XSDType.values())
    {
      if (type.getTypeName().equals(element.getName()))
      {
        treeNode.xsdType = type;
        break;
      }
    }
    
    // Not a recognized type
    if (treeNode.xsdType == null)
    {
      return null;
    }
    
    if (treeNode.xsdType == XSDType.ENUMERATION)
    {
      treeNode.name = element.getAttributeValue("value");
    }
    else
    {
      treeNode.name = element.getAttributeValue("name");
    }
    treeNode.elementType = element.getAttributeValue("type");
    treeNode.comment = getComment(element);
    
    return treeNode;
  }
  
  /**
   * Emits a blank line to the output stream
   */
  private void println ()
  {
    ixxWriter.println(" *");
  }
  
  /**
   * Prints a line to the output stream.  This inserts comment prefixes
   * in front of each line.
   * @param line
   */
  private void println (String line)
  {
    String[] lines = line.split("\n");
    for (String oneline : lines)
    { 
      ixxWriter.println(" * "+oneline.trim());
    }
  }
  
  /**
   * Obtains any XML comments sibling content that immediately preceed this
   * element
   * @param element
   * @return
   */
  private String getComment(Element element)
  {
    Parent parent = element.getParent();
    int i = parent.indexOf(element);
    
    StringBuffer comments = new StringBuffer();
    
    // Search backward from this position for preceding comments
    for (int x=i-1; x>=0; x--)
    {
      Content c = parent.getContent(x);
      if (c.getCType() == Content.CType.Comment)
      {
        Comment comment = (Comment) c;
        String text = comment.getText().trim();
        if (isValidComment(text))
        {
          // This comment pertains to the preceding element
          if (text.startsWith("<<"))
          {
            break;
          }
          if (comments.length() > 0)
          {
            comments.insert(0, '\n');
          }
          comments.insert(0, text);
        }
      }
      else if (c.getCType() == Content.CType.Element)
      {
        // Stop looking when we hit another element 
        break;
      }
    }
    
    // Search forward from this position for inline commentx
    for (int x=i+1; x<parent.getContentSize(); x++)
    {
      Content c = parent.getContent(x);
      if (c.getCType() == Content.CType.Comment)
      {
        Comment comment = (Comment) c;
        String text = comment.getText().trim();
        if (isValidComment(text))
        {
          // This comment pertains to the preceding element
          if (text.startsWith("<<"))
          {
            // Strip the "<<"
            text = text.substring(2);
            if (comments.length() > 0)
            {
              comments.insert(0, '\n');
            }
            comments.insert(0, text);
          }
          else
          {
            break;
          }
        }
      }
      else if (c.getCType() == Content.CType.Element)
      {
        // Stop looking when we hit another element 
        break;
      }
    }
    return comments.toString();
  }
  
  /**
   * Verifies rule that the comment must contain brief and details tags
   * @param text Text to verify
   * @return
   */
  private boolean isValidComment(String text)
  {
    return text != null &&
      text.contains("@copydoc") || text.contains("@brief");
  }
  
  /**
   * @param args
   */
  public static void main(String[] args) throws Exception
  {
    if (args.length < 2)
    {
      System.err.println ("XSDToDoxygen <input.xsd> <output.xii>");
      System.exit(1);
    }
    
    File inputFile = new File(args[0]);
    if (!inputFile.exists())
    {
      System.err.println ("Input file "+inputFile.getAbsolutePath()+" does not exist");
      System.exit(1);
    }
    
    File outputFile = new File(args[1]);
    if (!outputFile.getParentFile().isDirectory())
    {
      System.err.println ("Output file directory does not exist "+outputFile.getParentFile().getAbsolutePath());
      System.exit(1);
    }

    XSDToDoxygen xsdToDoxygen = new XSDToDoxygen(inputFile, outputFile);

    xsdToDoxygen.run();
  }

}
