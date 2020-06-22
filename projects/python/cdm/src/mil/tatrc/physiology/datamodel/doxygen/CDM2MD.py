import os
from xml.dom import minidom
import xmltodict
import json

class CDM2MD:
    def __init__(self):
        self.name=[]
        self.name_el=[]
        self.type_el=[]
        self.files_processed=[]
        self.tags={}
        self.list_types=[]
        
    def process_sc(self,fout):
        for input_file in self.files_processed:
            sc=minidom.parse(input_file)
            for child in sc.childNodes:
                if child.nodeName == "xs:schema":
                    self.parse(child,fout)

    def GenComplexType(self,node,fout):
        xml_json=json.dumps(xmltodict.parse(node.toxml()))
        json_dict=json.loads(xml_json)
        c_type=json_dict["xs:complexType"]
        #print(c_type)
        c_type_name=c_type["@name"]
        self.name.append(c_type_name)
        self.iterdict(c_type)
        item=list(zip(self.name_el[1:],self.type_el))
        self.tags[self.name[0]]=item
        self.name_el=[]
        self.type_el=[]
        self.base1=[]
        self.name=[]

    def iterdict(self,d):
        for k,v in d.items():
            if isinstance(v, dict):
                if k=="xs:element":
                    name1=v["@name"]
                    self.name_el.append(name1)
                    if "@type" in v:
                        type1=v["@type"]
                        self.type_el.append(type1)
                        if "@maxOccurs" in v:
                            if v["@maxOccurs"]=="unbounded":
                                self.list_types.append(type1)
                    else:
                        self.iterdict(v)
                elif k=="xs:list":
                    type1=v["@itemType"]
                    self.type_el.append("list "+type1)

    def parse(self,node,fout):
        for i in node.childNodes:
            self.GenComplexType(i,fout)

if __name__=="__main__":
    import sys
    CDM=CDM2MD()
    CDM.files_processed.append("./xsd/BioGearsDataModel.xsd")
    destDir = "./doc/doxygen/processed_md/"
    #destDir="/home/shashank/Desktop/biogears_tips/"
    if not os.path.exists(destDir):
        os.mkdir(destDir)
    fout=open(os.path.join(destDir,"CDMTables.md"),'w',encoding="utf-8")
    CDM.process_sc(fout)

