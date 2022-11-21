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

// Include the various types you will be using in your code
#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SEPatientActionCollection.h>
#include <biogears/cdm/scenario/SEActionManager.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsEngine.h>



using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Creating an engine based on a scenario file
///
/// \details
/// Read a scenario file and pull things out for your use case
//--------------------------------------------------------------------------------------------------
int HowToScenarioBase()
{
  // Create our engine

  auto bg = std::make_unique<BioGearsEngine>("statloading.log");
  auto biogears = dynamic_cast<BioGears*>(bg.get());

  bg->GetLogger()->Info("HowToScenarioBase");

  //Let's read the scenario we want to base this engine on, we are going to use the test state
  if (!bg->LoadState("./states/Jeff@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return 1;
  }

  //advance one time step to test
  bg->AdvanceModelTime(0.5, TimeUnit::s);


  // Let's request data do be tracked that is in the scenario
  biogears::SEPatientActionCollection* action;
  auto& patientactions = biogears->GetActions().GetPatientActions();

  //pneumothorax: 
  if (patientactions.HasLeftClosedTensionPneumothorax()) {
    bg->GetLogger()->Info("tensionPneumo");
  }

  //hemorrhage
  if (patientactions.HasHemorrhage()) {
    bg->GetLogger()->Info("tensionHem");
  }

  if (patientactions.HasAcuteStress()) {
    bg->GetLogger()->Info("tensionStress");
  }

  if (patientactions.HasBrainInjury()) {
    bg->GetLogger()->Info("tensionBrain");
  }

  if (patientactions.HasAsthmaAttack()) {
    bg->GetLogger()->Info("tensionAsthma");
  }


  return 0;
}

int main(int argc, char* argv[])
{
  return HowToScenarioBase();
}
