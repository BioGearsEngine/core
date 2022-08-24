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
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/controller/BioGears.h>


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
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToScenarioBase.log");
  bg->GetLogger()->Info("HowToScenarioBase");

  //Let's read the scenario we want to base this engine on, we are going to use the test state
  SEScenario sce(bg->GetSubstanceManager());
  sce.Load("./states/Roy@0s.xml");

  if (sce.HasEngineStateFile()) {
    if (!bg->LoadState(sce.GetEngineStateFile())) {
      bg->GetLogger()->Error("Could not load state, check the error");
      return 1;
    }
  } else if (sce.HasInitialParameters()) {
    SEScenarioInitialParameters& sip = sce.GetInitialParameters();
    if (sip.HasPatientFile()) {
      std::vector<const SECondition*> conditions;
      for (SECondition* c : sip.GetConditions())
        conditions.push_back(c); // Copy to const
      if (!bg->InitializeEngine(sip.GetPatientFile(), &conditions, &sip.GetConfiguration())) {
        bg->GetLogger()->Error("Could not load state, check the error");
        return 2;
      }
    } else if (sip.HasPatient()) {
      std::vector<const SECondition*> conditions;
      for (SECondition* c : sip.GetConditions())
        conditions.push_back(c); // Copy to const
      if (!bg->InitializeEngine(sip.GetPatient(), &conditions, &sip.GetConfiguration())) {
        bg->GetLogger()->Error("Could not load state, check the error");
        return 3;
      }
    }
  }
  CDM::DataRequestsData* drData;

  //test that all the active states are registering 
  
  
  // NOTE : You can just make a DataRequests xml file that holds only data requests
  // And serialize that in instead of a sceanrio file, if all you want is a consistent
  // set of data requests for all your scenarios
  std::unique_ptr<CDM::ObjectData> obj = biogears::Serializer::ReadFile("YourDataRequestsFile.xml", bg->GetLogger());
  drData = dynamic_cast<CDM::DataRequestsData*>(obj.get());
  bg->GetEngineTrack()->GetDataRequestManager().Load(*drData, bg->GetSubstanceManager());
  // Don't need to delete drData as obj is wrapped in a unique_ptr

  // Make a copy of the data requests, not this clears out data requests from the engine
  // This will clear out the data requests if any exist in the DataRequestManager
  drData = sce.GetDataRequestManager().Unload();
  bg->GetEngineTrack()->GetDataRequestManager().Load(*drData, bg->GetSubstanceManager());
  delete drData;

  if (!bg->GetEngineTrack()->GetDataRequestManager().HasResultsFilename()) {
    bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("./ResultsFileName.csv");
  }
  // Let's request data do be tracked that is in the scenario
  biogears::SEPatientActionCollection action(bg->GetSubstanceManager());

  //pneumothorax: 
  if (action.HasLeftClosedTensionPneumothorax()) {
    bg->GetLogger()->Info("tensionPneumo");
  }

  //hemorrhage
  if (action.HasHemorrhage()) {
    bg->GetLogger()->Info("tensionHem");
  }

  if (action.HasAcuteStress()) {
    bg->GetLogger()->Info("tensionStress");
  }

  if (action.HasBrainInjury()) {
    bg->GetLogger()->Info("tensionBrain");
  }

  if (action.HasAsthmaAttack()) {
    bg->GetLogger()->Info("tensionAsthma");
  }

  //SEAdvanceTime* adv;
  //// Now run the scenario actions
  //for (SEAction* a : sce.GetActions()) {
  //  // We want the tracker to process an advance time action so it will write each time step of data to our track file
  //  adv = dynamic_cast<SEAdvanceTime*>(a);
  //  if (adv != nullptr)
  //    bg->AdvanceModelTime(adv->GetTime(TimeUnit::s), TimeUnit::s); // you could just do bg->AdvanceModelTime without tracking timesteps
  //  else
  //    bg->ProcessAction(*a);
  //}

  // At this point your engine is where you want it to be
  // You could read in a new scenario and run it's actions
  // or programatically add actions as your applications sees fit

  return 0;
}

int main(int argc, char* argv[])
{
  return HowToScenarioBase();
}
