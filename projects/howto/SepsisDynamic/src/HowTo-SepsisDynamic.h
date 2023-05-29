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

OPEN_BIOGEARS_NAMESPACE
class SEInfection;
class SEInflammatoryResponse;
class SESubstance;
class SESubstanceInfusion;
class SESubstanceCompoundInfusion;
CLOSE_BIOGEARS_NAMESPACE

int HowToDynamicSepsis();

class DynamicSepsis {
public:
  DynamicSepsis(const std::string& logfile, int infectionInput);
  virtual ~DynamicSepsis();

  void SetAntibiotic();
  void SetNorepinephrine(double& concentration);
  void SetIVFluids(int compound, double rate);
  void UpdateMIC(double mic);
  void Status();

  BIOGEARS_NAMESPACE Logger* GetLogger() { return m_bg->GetLogger(); }

protected:
  void AdvanceTime();

  std::thread m_sepsisThread;
  std::mutex m_mutex;
  bool m_runThread;

  double m_StartTime_min;

  std::unique_ptr<BIOGEARS_NAMESPACE PhysiologyEngine> m_bg;

  BIOGEARS_NAMESPACE SEInfection* m_septicInfection;
  BIOGEARS_NAMESPACE SESubstanceInfusion* m_pressor;
  BIOGEARS_NAMESPACE SESubstanceCompoundInfusion* m_saline;
  BIOGEARS_NAMESPACE SESubstanceCompoundInfusion* m_ringers;
  BIOGEARS_NAMESPACE SESubstanceCompoundInfusion* m_antibiotic;
};
