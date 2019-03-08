/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/test/CommonDataModelTest.h>
#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/cdm/utils/TimingProfile.h>
#include <biogears/cdm/utils/testing/SETestCase.h>
#include <biogears/cdm/utils/testing/SETestReport.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>
// Systems
#include <biogears/cdm/system/environment/SEEnvironment.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>
#include <biogears/cdm/system/equipment/Inhaler/SEInhaler.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SEEndocrineSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>
#include <biogears/cdm/system/physiology/SERenalSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/system/physiology/SETissueSystem.h>

namespace biogears {
void CommonDataModelTest::ReadScenarios(const std::string& rptDirectory)
{
  TimingProfile pTimer;
  std::string testName = "ReadScenarios";
  m_Logger->ResetLogFile(rptDirectory + "/" + testName + ".log");
  SESubstanceManager subMgr(m_Logger);
  subMgr.LoadSubstanceDirectory();
  SEScenario scenario(subMgr);

  SECompartmentManager cmptMgr(subMgr);

  std::string dir = GetCurrentWorkingDirectory();
  dir.append("Scenarios");

  SETestReport testReport(m_Logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName(testName);

  std::vector<SESystem*> physiology;
  physiology.push_back(new SEBloodChemistrySystem(m_Logger));
  physiology.push_back(new SECardiovascularSystem(m_Logger));
  physiology.push_back(new SEEndocrineSystem(m_Logger));
  physiology.push_back(new SEEnergySystem(m_Logger));
  physiology.push_back(new SERenalSystem(m_Logger));
  physiology.push_back(new SEGastrointestinalSystem(m_Logger));
  physiology.push_back(new SERespiratorySystem(m_Logger));
  physiology.push_back(new SEDrugSystem(m_Logger));
  physiology.push_back(new SETissueSystem(m_Logger));
  std::vector<SESystem*> equipment;
  equipment.push_back(new SEAnesthesiaMachine(subMgr));
  equipment.push_back(new SEElectroCardioGram(m_Logger));
  equipment.push_back(new SEInhaler(subMgr));

  std::vector<std::string> files;
  ListFiles(dir, files, R"(\.xml)");
  for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
    if (it->find("xml") != std::string::npos) {
      if (it->find("PFT@") != std::string::npos || it->find("CBC@") != std::string::npos || it->find("MP@") != std::string::npos || it->find("Urinalysis@") != std::string::npos) // Ignore PFT, CBC, UPanel  and MP files
        continue; // TODO should actually peek the file and ensure it starts with a <scenario> tag

      pTimer.Start("Case");
      SETestCase& testCase = testSuite.CreateTestCase();
      Info(it->c_str());
      try {
        if (scenario.Load(*it)) {
          if (!scenario.IsValid())
            testCase.AddFailure(*it + " is not a valid scenario!");

          //if (scenario.GetInitialParameters().HasPatientFile())
          //{
          //  SEPatient patient(m_Logger);
          //  patient.Load(scenario.GetInitialParameters().GetPatientFile());

          //  PhysiologyEngineTrack trk(patient, subMgr, cmptMgr, physiology, equipment);
          //  for (SEDataRequest* dr : scenario.GetRequestedData())
          //  {
          //    SECompartmentDataRequest*cmptDR = dynamic_cast<SECompartmentDataRequest*>(dr);
          //    if (cmptDR != nullptr)
          //    {
          //      // I need to make the cmpt that this is pulling data off of, I don't care what the value is
          //      // just need to make sure it can get it
          //      switch (cmptDR->GetType())
          //      {
          //      case CDM::enumCompartmentType::Gas:
          //        if(!cmptMgr.HasGasCompartment(cmptDR->GetCompartment()))
          //          cmptMgr.CreateGasCompartment(cmptDR->GetCompartment());
          //        if (cmptDR->HasSubstance())
          //          cmptMgr.AddGasCompartmentSubstance(*cmptDR->GetSubstance());
          //        break;
          //      case CDM::enumCompartmentType::Liquid:
          //        if (!cmptMgr.HasLiquidCompartment(cmptDR->GetCompartment()))
          //          cmptMgr.CreateLiquidCompartment(cmptDR->GetCompartment());
          //        if (cmptDR->HasSubstance())
          //          cmptMgr.AddLiquidCompartmentSubstance(*cmptDR->GetSubstance());
          //        break;
          //      case CDM::enumCompartmentType::Thermal:
          //        if (!cmptMgr.HasThermalCompartment(cmptDR->GetCompartment()))
          //          cmptMgr.CreateThermalCompartment(cmptDR->GetCompartment());
          //        break;
          //      case CDM::enumCompartmentType::Tissue:
          //        if (!cmptMgr.HasTissueCompartment(cmptDR->GetCompartment()))
          //          cmptMgr.CreateTissueCompartment(cmptDR->GetCompartment());
          //        break;
          //      default:
          //        testCase.AddFailure("Unhandled compartment type :" + cmptDR->GetType());
          //      }
          //    }

          //    if(!trk.ProcessRequest(*dr))
          //      testCase.AddFailure("Could not hook up the following data request " + cmptDR->GetName());
          //  }
          //}

        } else {
          testCase.AddFailure(*it + " has failed to load!");
        }
      } catch (...) {
        testCase.AddFailure(*it + " has failed to load! unknown exception.");
      }
      testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Case"), TimeUnit::s);
      testCase.SetName(*it);
    }
  }
  testReport.WriteFile(rptDirectory + "/" + testName + "Report.xml");
}
}