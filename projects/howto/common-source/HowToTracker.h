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
#pragma once
#include <biogears/cdm/CommonDataModel.h>            // Needs to be included before BioGearsPhysiologyEngine.h
#include <biogears/engine/BioGearsPhysiologyEngine.h>

/// This class is here to demonstrate executing the engine
/// and populating a csv file with data from the engine 
/// This class will handle advancing time on the engine
class HowToTracker
{
private:
  double m_dT_s;  // Cached Engine Time Step
  double m_dT_s_remainder = 0.0;
  biogears::PhysiologyEngine& m_Engine;
public:
  HowToTracker(biogears::PhysiologyEngine& engine);
   ~HowToTracker();

  // This class will operate on seconds
  void AdvanceModelTime(double time_s);
};
