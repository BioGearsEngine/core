#CDM2MD file for prop.xsd
import os
from xml.dom import minidom


def parse(node):
        for i in node.childNodes:
            if i.nodeName == "xs:complexType":
                GenComplexType(i)

def GenComplexType(node):
    global name,name_el,type_el,tags
    xml_json=json.dumps(xmltodict.parse(node.toxml()))
    json_dict=json.loads(xml_json)
    c_type=json_dict["xs:complexType"]
    c_type_name=c_type["@name"]
    name.append(c_type_name)
    iterdict(c_type)
    item=list(zip(name_el[1:],type_el))
    tags[name[0]]=item
    name_el=[]
    type_el=[]
    name=[]

def iterdict(d):
    for k,v in d.items():        
        if isinstance(v, dict):
            if k=="xs:element":
                name1=v["@name"]
                name_el.append(name1)
                if "@type" in v:
                    type1=v["@type"]
                    type_el.append(type1)
                else:
                    iterdict(v)
            elif k=="xs:list":
                type1=v["@itemType"]
                type_el.append("list "+type1)
                    
            iterdict(v)
                
if __name__=="__main__":
    input_file="/opt/biogears/core/share/xsd/cdm/Properties.xsd"
    sc=minidom.parse(input_file)
    name=[]
    name_el=[]
    type_el=[]
    tags={}
    for child in sc.childNodes:
        if child.nodeName == "xs:schema":
            parse(child)

