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
        self.list_names=[]
        self.base1=[]
        
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
        c_type_name=c_type["@name"]
        self.name.append(c_type_name)
        self.iterdict(c_type)
        if len(self.base1)==0 and len(self.type_el)==0:
            self.name.remove(c_type_name)
        elif len(self.base1)!=0 and len(self.type_el)!=0:
            item=list(zip(self.name_el[1:],self.type_el))
            self.tags[self.name[0]]=item
            if len(self.base1)==1:
                self.tags[self.name[0]].append(self.base1[0])
            else:
                for l in self.base1:
                    self.tags[self.name[0]].append(l)
        elif len(self.base1)!=0 and len(self.type_el)==0:
            self.tags[self.name[0]]=[]
            if len(self.base1)==1:
                self.tags[self.name[0]].append(self.base1[0])
            else:
                for l in self.base1:
                    self.tags[self.name[0]].append(l)
        else:
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
                                #print(d)
                                self.list_types.append(type1)
                    else:
                        self.iterdict(v)
                elif k=="xs:list":
                    type1=v["@itemType"]
                    self.type_el.append("list "+type1)
                elif k=="xs:extension":
                    base1=v["@base"]
                    self.base1.append(base1)

                self.iterdict(v)

            elif isinstance(v,list):
                for i in v:
                    self.iterdict(i)
            elif k=="@name":
                self.name_el.append(v)
            elif k=="@type":
                self.type_el.append(v)

    def parse(self,node,fout):
        for i in node.childNodes:
            self.GenComplexType(i,fout)

if __name__=="__main__":
    import sys
    CDM=CDM2MD()
    CDM.files_processed.append("./xsd/cdm/Properties.xsd")
    destDir = "./doc/doxygen/processed_md/"
    #destDir="/home/shashank/Desktop/biogears_tips/"
    if not os.path.exists(destDir):
        os.mkdir(destDir)
    fout=open(os.path.join(destDir,"CDMTables.md"),'w',encoding="utf-8")
    CDM.process_sc(fout)
    list_type=list(set(CDM.list_types))
    list_name=list(set(CDM.list_names))
    CDM.list_types=list_type
    CDM.list_names=list_name
 
