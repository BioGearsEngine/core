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
#include <mutex>
#include <thread>

// #include <biogears/schema/biogears-cdm.hxx>
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
// This class will run BioGears in it's own thread and accept changes to hemorrage and iv flow as the engine is running

// Forward declare what we will use in our thread
namespace biogears{
  class SEHemorrhage;
  class SESubstanceCompoundInfusion;
  class PhysiologyEngine;
}

void HowToThreadedBioGears();

class BioGearsThread
{
public:
  BioGearsThread(const std::string& logfile);
  virtual ~BioGearsThread();

  void SetHemorrhage(std::string& location, double& severity);
  void SetIVFluidsFlow_mL_Per_min(double rate);
  virtual void Status();

  biogears::Logger* GetLogger() { return m_bg->GetLogger(); }

protected:
  void AdvanceTime();

  std::thread m_thread;
  std::mutex  m_mutex;
  bool m_runThread;

  std::unique_ptr<biogears::PhysiologyEngine> m_bg;

  biogears::SEHemorrhage*                     m_hemorrhage;
  biogears::SESubstanceCompoundInfusion*      m_infusion;
};
