#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>

#include "StateGenerator.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
namespace biogears {

  // length 26
std::string patientFiles[] = { "Bradycardic.xml",
                               "Carol.xml",
                               "Cynthia.xml",
                               "DefaultFemale.xml",
                               "DefaultMale.xml",
                               "DefaultTemplateFemale.xml",
                               "DefaultTemplateMale.xml",
                               "ExtremeFemale.xml",
                               "ExtremeMale.xml",
                               "Gus.xml",
                               "Hassan.xml",
                               "Jane.xml",
                               "Jeff.xml",
                               "Joel.xml",
                               "Nathan.xml",
                               "Overweight.xml",
                               "Ricky.xml",
                               "Roy.xml",
                               "Soldier.xml",
                               "StandardFemale.xml",
                               "StandardMale.xml",
                               "Tachycardic.xml",
                               "ToughGirl.xml",
                               "ToughGuy.xml",
                               "Tristan.xml",
                               "Underweight.xml" };

std::string findAndReplace(std::string& S, const std::string& toReplace,
                           const std::string& replaceWith)
{
  size_t start = 0;
  while (true) {
    size_t pos = S.find(toReplace, start);
    if (pos == std::string::npos) {
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
void StateGenerator::GenStates()
{
   
  //for(int i = 0;i < patientFiles->size();i++){
  for(int i = 0;i < 2;i++){
    runScenario(i,"Scenarios/InitialPatientStateAll.xml");
  }
}
//-------------------------------------------------------------------------------

//!
//! \param patientNum -- WHAT AM I
//! \param XMLString   -- WHAT AM I
//! \return --
int StateGenerator::runScenario(int patientNum, std::string&& XMLString)
{
  std::string patientXML(patientFiles[patientNum]);

  std::string patientLog = "-" + patientXML;
  patientLog = findAndReplace(patientLog, ".xml", ".log");

  std::string patientResults = "-" + patientXML;
  patientResults = findAndReplace(patientResults, ".xml", "Results.csv");

  std::string logFile(patientFiles[patientNum]);
  std::string outputFile(patientFiles[patientNum]);
  logFile = findAndReplace(logFile, ".xml", "Results.log");
  outputFile = findAndReplace(outputFile, ".xml", "Results.csv");

  std::unique_ptr<PhysiologyEngine> eng ;
  try {
	  eng = CreateBioGearsEngine(logFile);
  } catch ( std::exception e) {
	std::cout << e.what();
	return 0;
  }
  DataTrack* trk = &eng->GetEngineTrack()->GetDataTrack();
  BioGearsScenario sce(eng->GetSubstanceManager());
  sce.Load(XMLString);
  sce.GetInitialParameters().SetPatientFile(patientXML);

  BioGearsScenarioExec* exec = new BioGearsScenarioExec(*eng);
  exec->Execute(sce, outputFile, nullptr);
  delete exec;

  return 0;
}
//-------------------------------------------------------------------------------
} // namespace biogears
