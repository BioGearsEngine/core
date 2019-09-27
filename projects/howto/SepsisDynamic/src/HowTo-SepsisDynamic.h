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

namespace biogears {
class SEInfection;
class SEInflammatoryResponse;
class SESubstance;
class SESubstanceInfusion;
class SESubstanceCompoundInfusion;
}

void HowToDynamicSepsis();

class DynamicSepsis {
public:
  DynamicSepsis(const std::string& logfile, int infectionInput);
  virtual ~DynamicSepsis();

  void SetAntibiotic();
  void SetNorepinephrine(double& concentration);
  void SetIVFluids(int compound, double rate);
  void UpdateMIC(double mic);
  void Status();

  biogears::Logger* GetLogger() { return m_bg->GetLogger(); }

protected:
  void AdvanceTime();

  std::thread m_sepsisThread;
  std::mutex m_mutex;
  bool m_runThread;

  double m_StartTime_min;

  std::unique_ptr<biogears::PhysiologyEngine> m_bg;

  biogears::SEInfection* m_septicInfection;
  biogears::SESubstanceInfusion* m_pressor;
  biogears::SESubstanceCompoundInfusion* m_saline;
  biogears::SESubstanceCompoundInfusion* m_ringers;
  biogears::SESubstanceCompoundInfusion* m_antibiotic;
};
