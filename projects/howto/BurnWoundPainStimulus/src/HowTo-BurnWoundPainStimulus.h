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

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>

void HowToBurnWoundPainStimulus();

namespace biogears {
class SEBurnWound;
class SESubstance;
class SESubstanceBolus;
class SESubstanceCompoundInfusion;
}

class BurnThread {
public:
  BurnThread(const std::string& logFile, double &tbsa);
  virtual ~BurnThread();

  void AdministerKetamine(double &bolus);
  void SetRingersInfusionRate(double& volume, double& rate);
  void Status();

  biogears::Logger* GetLogger() { return m_bg->GetLogger(); }

  protected:
  void AdvanceTime();

  std::thread m_burnThread;
  std::mutex m_mutex;
  bool m_runThread;

  std::unique_ptr<biogears::PhysiologyEngine> m_bg;

  biogears::SEBurnWound* m_burnWound;
  biogears::SESubstanceBolus* m_ketamineBolus;
  biogears::SESubstanceCompoundInfusion* m_ringers;
  double m_ivBagVolume_mL;

};