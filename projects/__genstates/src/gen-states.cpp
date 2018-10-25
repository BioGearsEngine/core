#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/schema/Scenario.hxx>

#include <biogears/stateGenerator.h>

#include <iostream>
#include <thread>
#include <fstream>
#include <string>

int main()
{
  std::cout << "main" << std::endl;

  int availableThreads = std::thread::hardware_concurrency(); //max # of threads 
  int engineThreads = availableThreads - 1; // # of threads devoted to running tests
  int activeThreads = 0;
  stateGenerator* SG = new stateGenerator();
  std::string XMLFile("Scenarios/InitialPatientStateAll.xml");
  std::ifstream file(XMLFile);
  std::string line;
  std::string XMLS("");
  while(std::getline(file,line)){
    XMLS.append(line);
  }
  for(int i = 0;i < 2;++i){
    std::string XMLString(XMLS);
    SG->runScenario(i,XMLFile,XMLString);
  }
  delete SG;
  std::cout << "Scenario Successful" << std::endl;
  return 0;
}