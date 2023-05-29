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

int HowToBurnWoundPainStimulus();

OPEN_BIOGEARS_NAMESPACE
class SEBurnWound;
class SEEscharotomy;
class SESubstance;
class SESubstanceBolus;
class SESubstanceCompoundInfusion;
CLOSE_BIOGEARS_NAMESPACE

class BurnThread {
public:
  BurnThread(const std::string logFile, double tbsa);
  virtual ~BurnThread();

  void AdministerKetamine(double& bolus);
  void SetRingersInfusionRate(double& volume, double& rate);
  void SetAlbuminInfusionRate(double& volume, double& rate);
  void Status();
  void FluidLoading(double tbsa);

  enum fluidType {
    ringers, ///< Label vessels with radius size
    albumin ///< Label vessels by region (serviced by the anterior, middle, or posterior cerebral artery)
  };

  BIOGEARS_NAMESPACE Logger* GetLogger() { return m_bg->GetLogger(); }

protected:
  void AdvanceTime();
  void AdvanceTimeFluids();
  void AdvanceTimeFluidsAlbumin();

  std::thread m_burnThread;
  std::mutex m_mutex;
  bool m_runThread;

  std::unique_ptr<BIOGEARS_NAMESPACE PhysiologyEngine> m_bg;

  BIOGEARS_NAMESPACE SEBurnWound* m_burnWound;
  BIOGEARS_NAMESPACE SEEscharotomy* m_escharotomy;
  BIOGEARS_NAMESPACE SESubstanceBolus* m_ketamineBolus;
  BIOGEARS_NAMESPACE SESubstanceCompoundInfusion* m_ringers;
  BIOGEARS_NAMESPACE SESubstanceCompoundInfusion* m_albumex;
  double m_ivBagVolume_mL;
  double m_ivBagVolumeAlbumin_mL;
  double m_TotalVolumeAlbumin_mL = 0.0;
  double m_TotalVolume_mL = 0.0;
  fluidType fluid = ringers; // set the type of fluid here
};