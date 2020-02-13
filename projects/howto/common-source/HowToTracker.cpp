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

// The following how-to functions demonstrate various ways of interacting with the BioGears engine
#include "HowToTracker.h"
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/scenario/requests/SEDataRequest.h>
#include <biogears/cdm/properties/SEScalarTime.h>

using namespace biogears;
HowToTracker::HowToTracker(PhysiologyEngine& engine) : m_Engine(engine)
{
  m_dT_s = m_Engine.GetTimeStep(TimeUnit::s);
}

HowToTracker::~HowToTracker()
{
}

void HowToTracker::AdvanceModelTime(double time_s)
{	
	// This samples the engine at each time step
  double elapsed_time = 0.0;
	int count = static_cast<int>(time_s / m_dT_s);
	for (int i = 0; i <= count; i++)
	{		
		m_Engine.AdvanceModelTime();  // Compute 1 time step

		// Pull Track will pull data from the engine and append it to the file
    if (elapsed_time >= 1.0 / m_Engine.GetEngineTrack()->GetDataRequestManager().GetSamplesPerSecond()) {
      m_Engine.GetEngineTrack()->TrackData(m_Engine.GetSimulationTime(TimeUnit::s));
      elapsed_time -= 1.0 / m_Engine.GetEngineTrack()->GetDataRequestManager().GetSamplesPerSecond();
    }
    elapsed_time += m_dT_s;
	}
}
