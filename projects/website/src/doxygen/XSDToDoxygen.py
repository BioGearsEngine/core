#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Created on Wed Jun 10 10:20 2020

@author: Shashank
"""


from xml.dom import minidom
from pathlib import Path
import os
import sys
import argparse


"""
XSDToDoxygen class
_______________________

This class parses all of xsd files and extract comments and defgroups from those files and arrange them in heirarchial order

Follows pattern:-

@defgroup CDM_NameOfParent_NameOfChild NameOfChile
@brief Comment
{
 followed by heirarchial structure
}

"""

class XSDToDoxygen:
    def __init__(self):
        self.name=[]
        self.files_processed=[]
        self.tags=["xs:simpleType","xs:complexType","xs:extension","xs:restriction","xs:enumeration",
                   "xs:element","xs:sequence","xs:attribute","xs:include","xs:complexContent"]
        self.check=[]
        self.writer=""
    
    
    """
    Process_sc
    _____________

    This function stores all of the files within the xs:schema parent node 
    and then parses through all files and then append the file names into files_processed list

    """

    def process_sc(self,fout):
        for input_file in self.files_processed: 
            sc=minidom.parse(input_file)
            for child in sc.childNodes:
                if child.nodeName == "xs:schema":
                    self.parse(child,fout)

        """
    Parser
    ___________

    This is the main function which is called again and again till childnodes of the heirarchy is found
    It tries to align those heirarchial data structure according to comments and other nodes which play 
    certain crucial role in parsing
    Nodes are:-
    xs:simpletype
    xs:complextype
    xs:extension
    xs:restriction
    xs:enumeration
    xs:element
    xs:sequence
    xs:attribute
    xs:include
    and 
    node Type namely Comment_node
    """

    def parse(self,node,fout):
        """
        For parsing through all respective child Nodes in a XSD file
        """
        for j,i in enumerate(node.childNodes):
            
            """
            Case 1: Node Name = Simple Type or Complex Type
            Extract attribute name from the nodes
            Then append that into list of name
            And also write the brief related comments from the file
            """

            if i.nodeName == "xs:simpleType" or i.nodeName == "xs:complexType":
                if i.getAttribute("name"):
                    self.name.append(i.getAttribute("name"))
                    fout.write(" * @defgroup CDM_"+i.getAttribute("name")+" "+i.getAttribute("name"))
                    fout.write("\n")
                    if len(self.writer)>0:
                        fout.write(self.writer)
                        self.writer=""
                else:
                    self.name.append("null")
                    fout.write(" * @defgroup CDM_"+self.name[-3]+"_"+self.name[-2]+"_"+self.name[-1]+" "+self.name[-1])
                    fout.write("\n")
                    if len(self.writer)>0:
                        fout.write(self.writer)
                        self.writer=""
               
                """
                Reitrating through the process for all of the subsequent child nodes.
                For names if name is attribute loop will only function once 
                And only once the parse function will run
                """
                
                names_1=[s.nodeName for s in i.childNodes if s.nodeName in self.tags or s.nodeType==s.COMMENT_NODE]
                if self.tags[7] in list(set(names_1)):
                    for l in i.childNodes:
                        if l.nodeName in self.tags or l.nodeType==l.COMMENT_NODE:
                            fout.write(" * @{")
                            fout.write("\n")
                            self.parse(i,fout)
                            if len(self.writer)>0:
                                fout.write(self.writer)
                                self.writer=""
                            fout.write(" * @}")
                            fout.write("\n")
                            break 
                else:
                    for l in i.childNodes:
                        if l.nodeName in self.tags or l.nodeType==l.COMMENT_NODE:
                            fout.write(" * @{")
                            fout.write("\n")
                            self.parse(i,fout)
                            if len(self.writer)>0:
                                fout.write(self.writer)
                                self.writer=""
                            fout.write(" * @}")
                            fout.write("\n")
                fout.write(" *\n")
            
            elif i.nodeType == i.COMMENT_NODE:
                """
                Case2:
                    If nodeType == Comment_Node
                    it will simply replace all of the unwanted info from tags like replacing all of the empty spaces and stripping of characters
                """
                comment=i.data.strip("<<").lstrip(' ').replace("       "," * ")
                comment=comment.rstrip("* \n \n")
                if comment.startswith("@brief") or comment.startswith("@details") or comment.startswith("-"):
                    self.writer+=" * "+comment+"\n"

            elif i.nodeName == "xs:extension" or i.nodeName == "xs:restriction":
                """
                Case3:
                    If nodeName==extension or restriction
                    Just simply reiterate the pareser again as information will be in it's child
                """
                self.parse(i,fout)

            elif i.nodeName == "xs:enumeration":
                """
                Case4:
                    if nodeName==enumeration
                    Write the comments from the document
                    Then write a defgroup
                """
                if len(self.writer)>0:
                    fout.write(self.writer)
                    self.writer=""
                fout.write(" * @defgroup CDM_"+self.name[-1]+"_"+i.getAttribute("value")+" "+i.getAttribute("value"))
                fout.write("\n")
            
            elif i.nodeName == "xs:element":
                """
                Case5:
                    if nodeName==element
                    We need to check for
                    if parentNodename ==schema if it is true then simply write attribute name to defgroup
                    if parentNode.parentNode.parentNodeName==element then create a specific defgroup for that
                    else write a defgroup and parse through the next chilnodes again
                """
                if i.parentNode.nodeName=="xs:schema":
                    if len(self.writer)>0:
                        fout.write(self.writer)
                        self.writer=""
                    fout.write(" * @defgroup CDM_"+i.getAttribute("name")+" "+i.getAttribute("name"))
                    fout.write("\n")
                elif i.parentNode.parentNode.parentNode.nodeName=="xs:element":
                    if len(self.writer)>0:
                        fout.write(self.writer)
                        self.writer=""
                    fout.write(" * @defgroup CDM_"+self.name[-3]+"_"+self.name[-2]+"_"+self.name[-1]+"_"+i.getAttribute("name")+" "+i.getAttribute("name"))
                    fout.write("\n")
                    
                else:
                    if len(self.writer)>0:
                        fout.write(self.writer)
                        self.writer=""
                    fout.write(" * @defgroup CDM_"+self.name[-1]+"_"+i.getAttribute("name")+" "+i.getAttribute("name"))
                    fout.write("\n")
                    for m in i.childNodes:
                        if m.nodeName in self.tags or m.nodeType==m.COMMENT_NODE:
                            self.name.append(i.getAttribute("name"))
                            fout.write(" * @{")
                            fout.write("\n")
                            self.parse(i,fout)
                            if len(self.writer)>0:
                                fout.write(self.writer)
                                self.writer=""
                            fout.write(" * @}")
                            fout.write("\n")

            elif i.nodeName=="xs:sequence":
                """
                Case6:
                if nodename==sequence
                    then simply parse again for child nodes
                """

                self.parse(i,fout)
            
            elif i.nodeName=="xs:attribute":
                """
                Case7:
                if nodeName==attribute
                    Then write a specific defgroup and add comment
                """
                if len(self.writer)>0:
                    fout.write(self.writer)
                    self.writer=""
                fout.write(" * @defgroup CDM_"+self.name[-1]+"_"+i.getAttribute("name")+" "+i.getAttribute("name"))     
                fout.write("\n")
            
            elif i.nodeName == "xs:include":
                """
                Case8:
                    if nodeName==include
                    Stores all those file names into the files to be processed list
                """
                files=i.getAttribute("schemaLocation")
                if files.startswith("./"):
                    files=os.path.dirname(self.files_processed[0])+files[1:]
                elif files.startswith(".."):
                    files=os.path.dirname(self.files_processed[0])+files[2:]
                else:
                    if files=="CommonDataModel.xsd":
                        files=os.path.dirname(self.files_processed[0])+"/"+files
                    else:
                        files=os.path.dirname(self.files_processed[0])+"/cdm/"+files
                if files not in self.files_processed:
                    self.files_processed.append(files)

            else:
                self.parse(i,fout)
    
if __name__=="__main__":
    parser = argparse.ArgumentParser(description='Creation of xsd.ixx file after parsing all xsd files ')
    parser.add_argument('-p','--xsdpath',help='Path to BioGearsDataModel.xsd file',required=True)
    parser.add_argument('-f','--filepath',help='Name and path of .ixx file created',required=True)
    args=parser.parse_args()
    if args.xsdpath ==None and args.filepath ==None:
        parser.print_help()
        sys.exit(0)
    else:
        XSD=XSDToDoxygen()
        XSD.files_processed.append(args.xsdpath)
        destDir=args.filepath
        if not os.path.exists(destDir):
            os.mkdir(destDir)
        fout=open(os.path.join(destDir,"xsd.ixx"),'w',encoding="utf-8")
        fout.write("/** \n"
                   "* @defgroup CDM CDM \n"
                   "* @{ \n\n\n")
        XSD.process_sc(fout)
        fout.write("\n" 
                   "*} \n"
                   "*/")
        fout.close()

