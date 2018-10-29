#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/cdm/utils/DataTrack.h>

#include "StateGenerator.h"

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
namespace biogears
{
  
std::string patientFiles[] = {"Bradycardic.xml" //length 26
                             ,"Carol.xml"
                             ,"Cynthia.xml"
                             ,"DefaultFemale.xml"
                             ,"DefaultMale.xml"
                             ,"DefaultTemplateFemale.xml"
                             ,"DefaultTemplateMale.xml"
                             ,"ExtremeFemale.xml"
                             ,"ExtremeMale.xml"
                             ,"Gus.xml"
                             ,"Hassan.xml"
                             ,"Jane.xml"
                             ,"Jeff.xml"
                             ,"Joel.xml"
                             ,"Nathan.xml"
                             ,"Overweight.xml"
                             ,"Ricky.xml"
                             ,"Roy.xml"
                             ,"Soldier.xml"
                             ,"StandardFemale.xml"
                             ,"StandardMale.xml"
                             ,"Tachycardic.xml"
                             ,"ToughGirl.xml"
                             ,"ToughGuy.xml"
                             ,"Tristan.xml"
                             ,"Underweight.xml"};

std::string findAndReplace(std::string& S, const std::string& toReplace, const std::string& replaceWith)
{
  size_t start = 0;
  while(true){
    size_t pos = S.find(toReplace,start);
    if(pos == std::string::npos){
      break;
    }
    S.replace(pos, toReplace.length(), replaceWith);
    start = pos + replaceWith.length();
  }
  return S;
}
//-------------------------------------------------------------------------------
StateGenerator::StateGenerator()
{  
}
//-------------------------------------------------------------------------------
StateGenerator::~StateGenerator()
{ 
}
//-------------------------------------------------------------------------------
//!
//! \param patientNum -- WHAT AM I
//! \param XMLFileName -- WHAT AM I
//! \param XMLString   -- WHAT AM I
//! \return -- 
int StateGenerator::runScenario(int patientNum, std::string& XMLFileName, std::string& XMLString)
{
  std::string patientXML(patientFiles[patientNum]);

  std::string patientLog = "-" + patientXML;
  patientLog = findAndReplace(patientLog, ".xml", ".log");
  
  std::string patientResults = "-" + patientXML;
  patientResults = findAndReplace(patientResults, ".xml", "Results.csv");

  std::string logFile(XMLFileName);
  std::string outputFile(XMLFileName);
  logFile = findAndReplace(logFile, ".xml", patientLog);
  outputFile = findAndReplace(outputFile, ".xml", patientResults);

  std::unique_ptr<PhysiologyEngine> eng = CreateBioGearsEngine(logFile);
  //eng->GetLogger()->SetForward(this);  //'this' assumes that we're running out of an object that can be passed into the SetForward function of a logger object
  //eng->GetLogger()->LogToConsole(false);
  DataTrack* trk = &eng->GetEngineTrack()->GetDataTrack();
  BioGearsScenario sce(eng->GetSubstanceManager());

  XMLString = findAndReplace(XMLString, ">all<", ">" + patientXML + "<");
  char* sceXML = strdup(XMLString.c_str());//this line converts std::string to char*
  std::istringstream istr(sceXML);
  xml_schema::properties properties;
  properties.schema_location("uri:/mil/tatrc/physiology/datamodel", "./xsd/BioGearsDataModel.xsd");
  sce.Load(*CDM::Scenario(istr, 0, properties));
  BioGearsScenarioExec* exec = new BioGearsScenarioExec(*eng);
  exec->Execute(sce, outputFile, nullptr);
  delete exec;

  return 0;
}
//-------------------------------------------------------------------------------
} //namespac biogears