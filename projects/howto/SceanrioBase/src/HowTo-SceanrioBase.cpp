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
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


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
  
	
	//Load will search for a file in several fall back locations listed in 
    // io-manager.h.  The primary places are the current_directory and 
    // the current_directory/Scenarios.  Each Scenario file may have references to 
    // Additional Patient and DataRequest files. See AsthmaAttack.xml for an example
	SEScenario sce(bg->GetSubstanceManager());
	sce.Load("Showcase/AsthmaAttack.xml");

  if (sce.HasEngineStateFile())
  {
    if (!bg->LoadState(sce.GetEngineStateFile()))
    {
      bg->GetLogger()->Error("Could not load state, check the error");
      return 1;
    }
  }
  else if (sce.HasInitialParameters())
  {
    SEScenarioInitialParameters& sip = sce.GetInitialParameters();
    if (sip.HasPatientFile())
    {
      std::vector<const SECondition*> conditions;
      for (SECondition* c : sip.GetConditions())
        conditions.push_back(c);// Copy to const
      if (!bg->InitializeEngine(sip.GetPatientFile(), &conditions, &sip.GetConfiguration()))
      {
        bg->GetLogger()->Error("Could not load state, check the error");
        return 2;
      }
    }
    else if (sip.HasPatient())
    {
      std::vector<const SECondition*> conditions;
      for (SECondition* c : sip.GetConditions())
        conditions.push_back(c);// Copy to const
      if (!bg->InitializeEngine(sip.GetPatient(), &conditions, &sip.GetConfiguration()))
      {
        bg->GetLogger()->Error("Could not load state, check the error");
        return 3;
      }
    }
  }

  if (!bg->GetEngineTrack()->GetDataRequestManager().HasResultsFilename()) {
    bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("./ResultsFileName.csv");
  }
  // Let's request data do be tracked that is in the scenario	
  
	SEAdvanceTime* adv;
	// Now run the scenario actions
	for (SEAction* a : sce.GetActions())
	{
		// We want the tracker to process an advance time action so it will write each time step of data to our track file
		adv = dynamic_cast<SEAdvanceTime*>(a);
		if (adv != nullptr)
			bg->AdvanceModelTime(adv->GetTime(TimeUnit::s), TimeUnit::s);// you could just do bg->AdvanceModelTime without tracking timesteps
		else
			bg->ProcessAction(*a);
	}

	// At this point your engine is where you want it to be
	// You could read in a new scenario and run it's actions 
	// or programatically add actions as your applications sees fit

  return 0;

}


int main ( int argc, char* argv[] ) {
  return HowToScenarioBase();
}
