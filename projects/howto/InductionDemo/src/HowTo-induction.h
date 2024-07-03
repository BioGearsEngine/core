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

int HowToInduction();

namespace biogears { 
class SEHemorrhage;
class SESubstance;
class SESubstanceBolus;
class SESubstanceCompoundInfusion;
}

class InductionThread {
public:
  InductionThread(const std::string logFile, std::string stateFile);
  virtual ~InductionThread();

  void AdministerInduction();
  void SetRingersInfusionRate(double& volume, double& rate);
  void SetBloodInfusionRate(double& volume, double& rate);
  void SetPlasmaInfusionRate(double& volume, double& rate);
  void SetPlateletInfusionRate(double& volume, double& rate);
  void Ventilation();
  void Status();
  void FluidLoading();

  enum fluidType
  {
    ringers, 
    blood 
  };

  biogears::Logger* GetLogger() { return m_bg->GetLogger(); }

  protected:
  void AdvanceTime();



  std::thread m_inductionThread;
  std::mutex m_mutex;
  bool m_runThread;

  std::unique_ptr<biogears::PhysiologyEngine> m_bg;

  biogears::SEHemorrhage* m_hemorrhage;
  biogears::SESubstanceBolus* m_FentanylBolus;
  biogears::SESubstanceBolus* m_PropofolBolus;
  biogears::SESubstanceBolus* m_RocuroniumBolus;
  biogears::SESubstanceCompoundInfusion* m_ringers;
  biogears::SESubstanceCompoundInfusion* m_blood;
  biogears::SESubstanceCompoundInfusion* m_plasma;
  biogears::SESubstanceCompoundInfusion* m_platelet;

  double m_ivBagVolume_mL;
  double m_ivBagVolumeBlood_mL;
  double m_ivBagVolumePlasma_mL;
  double m_ivBagVolumePlatelet_mL;

  double m_TotalVolumeBlood_mL = 0.0;
  double m_TotalVolumePlasma_mL = 0.0;
  double m_TotalVolume_mL = 0.0;
  fluidType fluid = ringers; //set the type of fluid here


};