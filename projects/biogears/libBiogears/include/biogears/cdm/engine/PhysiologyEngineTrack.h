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

#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentLink.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentLink.h>
#include <biogears/cdm/compartment/tissue/SETissueCompartment.h>
#include <biogears/cdm/properties/SEGenericScalar.h>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>
#include <biogears/cdm/utils/DataTrack.h>
namespace biogears {
class SESystem;
class SEPatient;
class SEEnvironment;
class PhysiologyEngine;
class SESubstanceManager;
class SECompartmentManager;

enum class CompartmentUpdate { None,
                               InFlow,
                               OutFlow,
                               HeatTransferRateIn,
                               HeatTransferRateOut,
                               Volume,
                               Pressure,
                               Heat,
                               Temperature,
                               VolumeFraction,
                               Mass,
                               Concentration,
                               pH,
                               Molarity,
                               PartialPressure,
                               Saturation };

class SEDataRequestScalar : public SEGenericScalar {
public:
  SEDataRequestScalar(Logger* logger)
    : SEGenericScalar(logger)
  {
    Heading.clear();

    UpdateProperty = CompartmentUpdate::None;
    GasCmpt = nullptr;
    GasSubstance = nullptr;
    LiquidCmpt = nullptr;
    LiquidSubstance = nullptr;
    ThermalCmpt = nullptr;
  }

  void UpdateScalar();
  void SetScalar(const SEScalar* s, SEDataRequest& dr); // SEScalar* in order to internnally throw error if the Track cannot find the requested property, it will pass in nullptr if it cannot find it

  std::string Heading;

  // Compartment related variables
  CompartmentUpdate UpdateProperty;
  SEGasCompartment* GasCmpt;
  SEGasSubstanceQuantity* GasSubstance;
  SELiquidCompartment* LiquidCmpt;
  SELiquidSubstanceQuantity* LiquidSubstance;
  SEThermalCompartment* ThermalCmpt;
  // Tissue cmpts don't have children and they don't have computed data that changes on call (like flow)
};

class BIOGEARS_API PhysiologyEngineTrack : public Loggable {
public:
  PhysiologyEngineTrack(PhysiologyEngine& engine);
  PhysiologyEngineTrack(SEPatient& patient,
                        SESubstanceManager& subMgr,
                        SECompartmentManager& cmptMgr,
                        const std::vector<SESystem*>& physiology,
                        const std::vector<SESystem*>& equipment);
  PhysiologyEngineTrack(PhysiologyEngineTrack&&);
  PhysiologyEngineTrack& operator=(PhysiologyEngineTrack&& rhs);
  virtual ~PhysiologyEngineTrack();

  void Clear(); // Remove all requests and close the results file

  DataTrack& GetDataTrack();
  SEDataRequestManager& GetDataRequestManager() { return m_DataRequestMgr; }

  void ResetFile(); // Close file, so next Track Data will re hook up everything and make a new file

  bool ConnectRequest(SEDataRequest& dr, SEDataRequestScalar& ds);

  virtual void SetupRequests();
  virtual void TrackData(double currentTime_s);
  virtual void PullData();
  virtual bool TrackRequest(SEDataRequest& dr);
  virtual void ForceConnection() { m_ForceConnection = true; }

protected:
  bool m_ForceConnection;
  DataTrack m_DataTrack;

  std::stringstream m_ss;
  std::ofstream m_ResultsStream;
  SEDataRequestManager m_DataRequestMgr;

  SEPatient* m_Patient;
  SESubstanceManager* m_SubMgr;
  SECompartmentManager* m_CmptMgr;

  SEEnvironment* m_Environment;
  std::vector<SESystem*> m_PhysiologySystems;
  std::vector<SESystem*> m_EquipmentSystems;
  std::map<const SEDataRequest*, SEDataRequestScalar*> m_Request2Scalar;
};
}