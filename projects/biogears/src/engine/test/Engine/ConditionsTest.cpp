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
#include <biogears/engine/test/BioGearsEngineTest.h>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/patient/conditions/SEChronicAnemia.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/utils/testing/SETestCase.h>
#include <biogears/cdm/utils/testing/SETestReport.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>

namespace biogears {
void BioGearsEngineTest::ConditionCombinations(const std::string& rptDirectory)
{
  Logger log("ConditionsPermutationsReport.log");
  std::vector<SECondition*> testConditions;
  SEChronicAnemia cAnem;
  cAnem.GetReductionFactor().SetValue(0.1);
  testConditions.push_back(&cAnem);

  std::vector<int> conditionSwitches;
  for (unsigned int i = 0; i < testConditions.size(); i++)
    conditionSwitches.push_back(1);
  // Conditions will either be on or off when we run
  // Now calculate all the permutations with our conditions
  std::vector<std::vector<int>> permutations;
  GeneralMath::Combinations(conditionSwitches, permutations);
  std::stringstream ss;

  SETestReport testReport(&log);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName("ConditionPermutations");
  std::vector<const SECondition*> sceConditions;

  //for (int i = 0; i < 3; i++)
  {
    // Step severity from 0.1 to 0.5 to 1.0
    for (std::vector<int> combo : permutations) {
      ss.clear();
      sceConditions.clear();
      for (unsigned int c = 0; c < testConditions.size(); c++) {
        if (combo[c] == 1) {
          sceConditions.push_back(testConditions[c]);
          ss << testConditions[c]->GetName() << "-";
        }
      }
      if (ss.str().empty())
        ss << "NoConditions";
      SETestCase& testCase = testSuite.CreateTestCase();
      log.Info(ss);
      std::unique_ptr<PhysiologyEngine> physEng = CreateBioGearsEngine(&log);
      if (!physEng->InitializeEngine("StandardMale.xml", &sceConditions)) {
        testCase.AddFailure("Unable to stabilize condition permutation");
      }
    }
  }
  testReport.WriteFile("ConditionsPermutationsReport.xml");
  DELETE_VECTOR(testConditions);
}
}