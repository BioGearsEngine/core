from xml.dom import minidom
from pathlib import Path
import os

class XSDToDoxygen:
    def __init__(self):
        self.name=[]
        self.files_processed=[]

    def process_sc(self):
        for input_file in self.files_processed:
            sc=minidom.parse(input_file)
            for child in sc.childNodes:
                if child.nodeName == "xs:schema":
                    self.fun(child)

    def fun(self,node):
        for j,i in enumerate(node.childNodes):
            if i.nodeName == "xs:include":
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

if __name__=="__main__":
    import sys
    XSD=XSDToDoxygen()
    XSD.files_processed.append("/opt/biogears/core/build/runtime/xsd/BioGearsDataModel.xsd")
    XSD.process_sc()
    print(XSD.files_processed)


