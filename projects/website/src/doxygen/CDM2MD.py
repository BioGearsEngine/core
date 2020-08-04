#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Created on Fri Jun 19 15:34:31 2020

@author: Shashank

"""

import os
from xml.dom import minidom
import xmltodict
import json
import argparse

"""
CDM2MD class
______________________________________________________________________

Converts every ComplexType root level tags to the table following
structure:-

@anchor <ComplexTypeName>+Table
##<ComplexTypeName>
@copybrief CDM_<ComplexTypeName>
|Property Name   |Type    |Definition|
|---             |---        |---            |
|<Element Name> |Element type  |@ref/@copybrief <ElementName >Table |
"""

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
        """
        Process_sc
        ___________________
        
        Creates a minidom parser
        """
        for input_file in self.files_processed:
            sc=minidom.parse(input_file)
            for child in sc.childNodes:
                if child.nodeName == "xs:schema":
                    self.parse(child,fout)

    def GenComplexType(self,node,fout):
        """
        GenComplexType
        ____________________________________________________
        
        It takes in the complexType node as an argument then
        converts that node into dictionary level structure and
        by reiterating through that it extracts element name and
        type from that dictionary depending on the structure
        
        """
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
        """
        Iterates over the dictionary and extracts name
        and type from it and append to the names and type
        lists
        """
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
            elif k=="@maxOccurs":
                if v=="unbounded":
                    if "@name" in d.keys():
                        self.list_names.append(d["@name"])
                    if "@type" in d.keys():
                        self.list_types.append(d["@type"])


    def ComplexException(self,node,fout):
        xml_json=json.dumps(xmltodict.parse(node.toxml()),indent=5)
        json_dict=json.loads(xml_json)
        c_type=json_dict["xs:complexType"]
        c_type_name=c_type["@name"]
        self.tags[c_type_name]=[]
        c_type_el=c_type["xs:complexContent"]["xs:extension"]["xs:sequence"]["xs:element"]
        c_type_base=c_type["xs:complexContent"]["xs:extension"]["@base"]
        self.tags[c_type_name].append(c_type_base)
        for i in c_type_el:
            self.tags[c_type_name].append((i["@name"],i["@name"]))
        for i in c_type_el:
            self.iterexec(i)
            item=list(zip(self.name_el[1:],self.type_el))
            self.tags[i["@name"]]=item
            self.name_el=[]
            self.type_el=[]

    def iterexec(self,d):
        for k,v in d.items():
            if isinstance(v,list) and k=="xs:element":
                for i in v:
                    self.iterexec(i)
            elif k=="@name":
                if v=="Regulation":
                    self.name_el.append(v)
                    self.type_el.append(v)
                else:
                    self.name_el.append(v)

            elif k=="@type":
                self.type_el.append(v)
            elif isinstance(v,dict):
                self.iterexec(v)

    def parse(self,node,fout):
        """
        Parse
        ____________________________________________________

        It checks for the node of childnodes to be either complexType
        or include type for parsing through the function to create table
        """
        for i in node.childNodes:
            if i.nodeName == "xs:complexType":
                if i.getAttribute("name")=="SubstanceClearanceData":
                    self.ComplexException(i,fout)
                else:
                    self.GenComplexType(i,fout)

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

    def convert(self):
        """
        Convert
        _______________________________________________________

        It converts dictionary of lists into dictionary of dictionaries
        and replaces all of the string which are extensions with the specific 
        dictionary as well
        """

        for i,j in self.tags.items():
            for k in j:
                if isinstance(k,str):
                    if k in self.tags:
                        for l in self.tags[k]:
                            self.tags[i].append(l)
                        self.tags[i].remove(k)
                    else:
                        self.tags[i].remove(k)
        for i in self.tags:
            self.tags[i]=list(set(self.tags[i]))
        for i in self.tags:
            self.tags[i]=dict(self.tags[i])
            self.tags[i]=dict(sorted(self.tags[i].items(),key=lambda x:x[0]))
        self.tags=dict(sorted(self.tags.items(),key=lambda x:x[0]))
        #print(self.tags)

    def table_create(self,fout):
        """
        Table_create
        ____________________________________________________________________________________________

        It is used for creation of CDMTables from the dictionary of dictionaries
        names tags variables and then extracts it specific Property Name represented 
        with dictionary key and Types are replaced with dictionary values and definition are
        replaced with @ref and @copybrief tags accordingly with list or another types
        """
        fout.write("CDM Tables {#CDMTables}\n")
        fout.write("=======================\n")
        for i in self.tags:
            fout.write("@anchor "+i+"Table\n")
            fout.write("## "+i+"\n")
            fout.write("@copybrief "+"CDM_"+i+"\n")
            fout.write("|Property Name   |"+"Type     |"+"Definiton     |\n")
            fout.write("|---          "+"|---         "+"|---             |\n")
            values=self.tags[i]
            for propname,type1 in values.items():
                fout.write("|"+propname)
                if propname in self.list_names:
                    fout.write("|List of SE"+propname+"s")
                    fout.write("|@ref "+propname+"Table"+" "*10+"|\n")
                else:
                    if type1.startswith("xs:"):
                        fout.write("|"+type1[3:]+" "*5)
                        if type1=="xs:string":
                            txt="|@copybrief CDM_"+i+"_"+propname+" "*20+"|\n"
                            fout.write(txt)
                        else:
                            fout.write("|@ref "+type1[3:]+"Table"+"|\n")

                    elif type1.startswith("enum"):
                        fout.write("|E"+type1[1:])
                        txt="|@copybrief CDM_"+i+"_"+propname+" "*20+"|\n"
                        fout.write(txt)
                    elif type1.startswith("Scalar"):
                        fout.write("|"+type1)
                        txt="|@copybrief CDM_"+i+"_"+propname+" "*20+"|\n"
                        fout.write(txt)
                    elif type1 in self.tags.keys():
                        if type1=="DoubleArray" or type1=="IntegerArray":
                            fout.write("|"+type1)
                            fout.write("|@ref "+type1+"Table"+"|\n")
                        else:
                            if type1 in self.list_types:
                                fout.write("|List of SE"+propname+"s")
                                fout.write("|@ref "+propname+"Table"+" "*10+"|\n")
                            else:
                                fout.write("|"+type1)
                                fout.write("|@ref "+type1+"Table"+" "*10+"|\n")
                    elif type1.startswith("list"):
                        fout.write("|List of SE"+propname+"s")
                        fout.write("|@ref "+propname+"Table"+" "*10+"|\n")
                    elif type1 not in self.tags.keys():
                        fout.write("|"+type1)
                        fout.write("|@ref "+type1+"Table"+" "*10+"|\n")

            fout.write("\n\n")

if __name__=="__main__":
    parser = argparse.ArgumentParser(description='Creation of CDMTables.md file after parsing all xsd files ')
    parser.add_argument('-p','--xsdpath',help='Path to BioGearsDataModel.xsd file',required=True)
    parser.add_argument('-f','--filepath',help='Name and path of CDMTables.md file created',required=True)
    args=parser.parse_args()
    if args.xsdpath ==None and args.filepath ==None:
        parser.print_help()
        sys.exit(0)
    else:
        CDM=CDM2MD()
        CDM.files_processed.append(args.xsdpath)
        destDir=args.filepath
        if not os.path.exists(destDir):
            os.mkdir(destDir)
        fout=open(os.path.join(destDir,"CDMTables.md"),'w',encoding="utf-8")
        CDM.process_sc(fout)
        CDM.convert()
        list_type=list(set(CDM.list_types))
        list_name=list(set(CDM.list_names))
        CDM.list_types=list_type
        CDM.list_names=list_name
        CDM.table_create(fout)  
