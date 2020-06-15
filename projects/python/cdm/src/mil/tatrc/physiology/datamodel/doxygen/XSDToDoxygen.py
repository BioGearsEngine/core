from xml.dom import minidom
from pathlib import Path
import os

class XSDToDoxygen:
    def __init__(self):
        self.name=[]
        self.files_processed=[]
        self.writer=""
    
    def process_sc(self,fout):
        for input_file in self.files_processed:
            sc=minidom.parse(input_file)
            for child in sc.childNodes:
                if child.nodeName == "xs:schema":
                    self.parse(child,fout)

    def parse(self,node,fout):
        for j,i in enumerate(node.childNodes):
            
            if i.nodeName=="xs:sequence":
                self.parse(i,fout)
            
            elif i.nodeName=="xs:attribute":
                fout.write(" * @defgroup CDM_"+self.name[-1]+"_"+i.getAttribute("name")+" "+i.getAttribute("name"))
                fout.write("\n")
                if len(self.writer)>0:
                    fout.write(self.writer)
                    self.writer=""

            elif i.nodeType == i.COMMENT_NODE:
                comment=i.data.strip("<<").lstrip(' ').replace("       "," * ")
                comment=comment.rstrip("* \n \n")
                if comment.startswith("@brief") or comment.startswith("@details") or comment.startswith("-"):
                    self.writer+=" * "+comment+"\n"

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
    import sys
    XSD=XSDToDoxygen()
    XSD.files_processed.append(sys.argv[1])
    fout=open(sys.argv[2],'w',encoding="utf-8")
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

