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


#include "TestDriver.h"
#include "Verification.h"

#include <biogears/cdm/Serializer.h>

#include "biogears/engine/test/BioGearsEngineTest.h"

using namespace biogears;

int main(int argc, char* argv[])
{

  if (argc < 4) {
    std::cout << "Usuage\n\t " << argv[0] << ": <type (BGE or CDM)> <test> <output_dir>" << std::endl;
  }
  else {
    std::string type = argv[1];
    std::string test= argv[2];
    std::string dir = argv[3];
    if (type =="CDM") {
      //CommonDataModelTest executor;
      //executor.GetLogger()->LogToConsole(true);
      //executor.RunTest(test, dir);
    } else if(type =="BGE") {
      BioGearsEngineTest executor;
      executor.RunTest(test, dir);
    } else {
      std::cout << "Invalid type.  Options are BGE and CDM" << std::endl;
      }

  }

  return 0;
}
