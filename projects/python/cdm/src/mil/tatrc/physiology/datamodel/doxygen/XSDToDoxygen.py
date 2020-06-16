from xml.dom import minidom
from pathlib import Path
import os
import sys
import argparse


class XSDToDoxygen:
    def __init__(self):
        self.name=[]
        self.files_processed=[]
        self.tags=["xs:simpleType","xs:complexType","xs:extension","xs:restriction","xs:enumeration",
                   "xs:element","xs:sequence","xs:attribute","xs:include","xs:complexContent"]
        self.check=[]
        self.writer=""

    def process_sc(self,fout):
        for input_file in self.files_processed: 
            sc=minidom.parse(input_file)
            for child in sc.childNodes:
                if child.nodeName == "xs:schema":
                    self.parse(child,fout)

    def parse(self,node,fout):
        for j,i in enumerate(node.childNodes):
            
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
                
                names_1=[s.nodeName for s in i.childNodes if s.nodeName in self.tags or s.nodeType==s.COMMENT_NODE]
                
                if self.tags[7] in list(set(names_1)):
                    
                    for l in i.childNodes:
                        
                        if l.nodeName in self.tags or l.nodeType==l.COMMENT_NODE:
                            fout.write(" * @{")
                            fout.write("\n")
                            self.parse(i,fout)
                            #fout.write(i.childNodes)
                            fout.write(" * @}")
                            fout.write("\n")
                            break
                else:
                    
                    for l in i.childNodes:
                        
                        if l.nodeName in self.tags or l.nodeType==l.COMMENT_NODE:
                            fout.write(" * @{")
                            fout.write("\n")
                            self.parse(i,fout)
                            #fout.write(i.childNodes)
                            fout.write(" * @}")
                            fout.write("\n")
                fout.write(" *\n")

            elif i.nodeType == i.COMMENT_NODE:
                
                comment=i.data.strip("<<").lstrip(' ').replace("       "," * ")
                comment=comment.rstrip("* \n \n")
                if comment.startswith("@brief") or comment.startswith("@details") or comment.startswith("-"):
                    self.writer+=" * "+comment+"\n"
            
            elif i.nodeName == "xs:extension" or i.nodeName == "xs:restriction":
                self.parse(i,fout)

            elif i.nodeName == "xs:enumeration":
                if len(self.writer)>0:
                    fout.write(self.writer)
                    self.writer=""
                fout.write(" * @defgroup CDM_"+self.name[-1]+"_"+i.getAttribute("value")+" "+i.getAttribute("value"))
                fout.write("\n")
            
            elif i.nodeName == "xs:element":
                
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
                            fout.write(" * @}")
                            fout.write("\n")

            elif i.nodeName=="xs:sequence":
                self.parse(i,fout)
            
            elif i.nodeName=="xs:attribute":
                
                if len(self.writer)>0:
                    fout.write(self.writer)
                    self.writer=""
                fout.write(" * @defgroup CDM_"+self.name[-1]+"_"+i.getAttribute("name")+" "+i.getAttribute("name"))     
                fout.write("\n")
                            
            elif i.nodeName == "xs:include":
                
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
        fout=open(args.filepath,'w',encoding="utf-8")
        fout.write("/**")
        fout.write("\n")
        fout.write("* @defgroup CDM CDM")
        fout.write("\n")
        fout.write("* @{")
        fout.write("\n\n\n")
        XSD.process_sc(fout)
        fout.write("\n")
        fout.write("*}")
        fout.write("\n")
        fout.write("*/")
        fout.close()

