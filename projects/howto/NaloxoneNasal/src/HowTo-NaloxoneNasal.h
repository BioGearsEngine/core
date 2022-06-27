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

int HowToNasalNaloxone();

namespace biogears {
class SESubstanceNasalDose;
class SESubstanceInfusion;
class SESubstance;
}

class NaloxoneThread {
public:
  NaloxoneThread(const std::string logFile, double opioidDose, const std::string opioidName);
  virtual ~NaloxoneThread();

  void AdministerOpioid(double& conc, double& rate);
  void SetNaloxoneInfusionRate(double& dose);
  void Status();
  void FluidLoading(std::string overdoseSubstance, double opioidDose);


  biogears::Logger* GetLogger() { return m_bg->GetLogger(); }

  protected:
  void AdvanceTime();
  void AdvanceTimeFluids();

  std::thread m_naloxoneThread;
  std::mutex m_mutex;
  bool m_runThread;

  std::unique_ptr<biogears::PhysiologyEngine> m_bg;

  biogears::SESubstanceInfusion* m_opioid;
  biogears::SESubstanceNasalDose* m_naloxone;
  double m_totalNaloxone_mg;
  //fluidType fluid = ringers; //set the type of fluid here


};