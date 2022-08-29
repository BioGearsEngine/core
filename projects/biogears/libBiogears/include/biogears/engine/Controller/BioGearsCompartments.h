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

#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

namespace biogears {
class BioGearsEngine;
class SEGasCompartmentGraph;
/**
* @brief Manages all compartments and graphs associated with all %BioGears systems/equipement
*/
class BIOGEARS_API BioGearsCompartments : public SECompartmentManager {
  friend class BioGearsSubstances;

public:
  static auto make_unique(BioGearsEngine& bg) -> std::unique_ptr<BioGearsCompartments>;
  BioGearsCompartments(BioGearsEngine& data);
  virtual ~BioGearsCompartments();

  void Clear();

  virtual bool Load(const CDM::CompartmentManagerData& in, SECircuitManager* circuits = nullptr);

  void StateChange();
  void UpdateAirwayGraph()
  {
    m_UpdateActiveAirwayGraph = true;
    m_UpdateActiveAerosolGraph = true;
  }

  SELiquidCompartmentGraph& GetActiveCardiovascularGraph();
  SELiquidCompartmentGraph& GetCardiovascularGraph();
  SELiquidCompartmentGraph& GetCerebralGraph();
  SELiquidCompartmentGraph& GetRenalGraph();

  SEGasCompartmentGraph& GetActiveRespiratoryGraph();
  SEGasCompartmentGraph& GetRespiratoryGraph();
  SEGasCompartmentGraph& GetRespiratoryAndAnesthesiaMachineGraph();
  SEGasCompartmentGraph& GetRespiratoryAndInhalerGraph();
  SEGasCompartmentGraph& GetRespiratoryAndMechanicalVentilatorGraph();

  SEGasCompartmentGraph& GetAnesthesiaMachineGraph();

  SELiquidCompartmentGraph& GetActiveAerosolGraph();
  SELiquidCompartmentGraph& GetAerosolGraph();
  SELiquidCompartmentGraph& GetAerosolAndInhalerGraph();

  // Compartment Helpers
  const std::vector<SELiquidCompartment*>& GetAerosolCompartments() ;
  const std::vector<SELiquidCompartment*>& GetAerosolLeafCompartments() ;

  const std::vector<SELiquidCompartment*>& GetChymeCompartments() ;
  const std::vector<SELiquidCompartment*>& GetChymeLeafCompartments() ;

  const std::vector<SEGasCompartment*>& GetPulmonaryCompartments() ;
  const std::vector<SEGasCompartment*>& GetPulmonaryLeafCompartments() ;

  const std::vector<SEThermalCompartment*>& GetTemperatureCompartments() ;
  const std::vector<SEThermalCompartment*>& GetTemperatureLeafCompartments() ;

  const std::vector<SETissueCompartment*>& GetTissueCompartments() ;
  const std::vector<SETissueCompartment*>& GetTissueLeafCompartments() ;

  const std::vector<SELiquidCompartment*>& GetUrineCompartments() ;
  const std::vector<SELiquidCompartment*>& GetUrineLeafCompartments() ;

  const std::vector<SELiquidCompartment*>& GetVascularCompartments() ;
  const std::vector<SELiquidCompartment*>& GetVascularLeafCompartments() ;

  const std::vector<SEGasCompartment*>& GetAnesthesiaMachineCompartments() ;
  const std::vector<SEGasCompartment*>& GetAnesthesiaMachineLeafCompartments() ;

  const std::vector<SEGasCompartment*>& GetInhalerCompartments() ;
  const std::vector<SEGasCompartment*>& GetInhalerLeafCompartments() ;

  const std::vector<SELiquidCompartment*>& GetInhalerAerosolCompartments() ;
  const std::vector<SELiquidCompartment*>& GetInhalerAerosolLeafCompartments() ;

  const std::vector<SEGasCompartment*>& GetMechanicalVentilatorCompartments() ;
  const std::vector<SEGasCompartment*>& GetMechanicalVentilatorLeafCompartments() ;

  SELiquidCompartment& GetExtracellularFluid(SETissueCompartment& t) ;
  SELiquidCompartment& GetIntracellularFluid(SETissueCompartment& t) ;
  const std::map<SETissueCompartment*, SELiquidCompartment*>& GetExtracellularFluid() ;
  const std::map<SETissueCompartment*, SELiquidCompartment*>& GetIntracellularFluid() ;

protected:
  // I don't want these exposed in BioGears, you should be calling the Substance manager
  virtual void AddGasCompartmentSubstance(SESubstance& sub);
  virtual void AddLiquidCompartmentSubstance(SESubstance& sub);

  virtual bool AllowGasSubstance(SESubstance& s, SEGasCompartment& cmpt) const;
  virtual bool AllowLiquidSubstance(SESubstance& s, SELiquidCompartment& cmpt) const;

  BioGearsEngine& m_data;

  bool m_UpdateActiveAirwayGraph;
  bool m_UpdateActiveAerosolGraph;

  SELiquidCompartmentGraph* m_CombinedCardiovascularGraph;
  SELiquidCompartmentGraph* m_CardiovascularGraph;
  SELiquidCompartmentGraph* m_CerebralGraph;
  SELiquidCompartmentGraph* m_RenalGraph;

  SEGasCompartmentGraph* m_RespiratoryGraph;
  SEGasCompartmentGraph* m_CombinedRespiratoryAnesthesiaGraph;
  SEGasCompartmentGraph* m_CombinedRespiratoryInhalerGraph;
  SEGasCompartmentGraph* m_CombinedRespiratoryMechanicalVentilatorGraph;
  SEGasCompartmentGraph* m_AnesthesiaMachineGraph;

  SELiquidCompartmentGraph* m_AerosolGraph;
  SELiquidCompartmentGraph* m_CombinedAerosolInhalerGraph;

  std::vector<SELiquidCompartment*> m_AerosolCompartments;
  std::vector<SELiquidCompartment*> m_AerosolLeafCompartments;
  std::vector<SELiquidCompartment*> m_ChymeCompartments;
  std::vector<SELiquidCompartment*> m_ChymeLeafCompartments;
  std::vector<SEGasCompartment*> m_PulmonaryCompartments;
  std::vector<SEGasCompartment*> m_PulmonaryLeafCompartments;
  std::vector<SEThermalCompartment*> m_TemperatureCompartments;
  std::vector<SEThermalCompartment*> m_TemperatureLeafCompartments;
  std::vector<SETissueCompartment*> m_TissueCompartments;
  std::vector<SETissueCompartment*> m_TissueLeafCompartments;
  std::vector<SELiquidCompartment*> m_UrineCompartments;
  std::vector<SELiquidCompartment*> m_UrineLeafCompartments;
  std::vector<SELiquidCompartment*> m_VascularCompartments;
  std::vector<SELiquidCompartment*> m_VascularLeafCompartments;
  std::vector<SEGasCompartment*> m_AnesthesiaMachineCompartments;
  std::vector<SEGasCompartment*> m_AnesthesiaMachineLeafCompartments;
  std::vector<SEGasCompartment*> m_InhalerCompartments;
  std::vector<SEGasCompartment*> m_InhalerLeafCompartments;
  std::vector<SELiquidCompartment*> m_InhalerAerosolCompartments;
  std::vector<SELiquidCompartment*> m_InhalerAerosolLeafCompartments;
  std::vector<SEGasCompartment*> m_MechanicalVentilatorCompartments;
  std::vector<SEGasCompartment*> m_MechanicalVentilatorLeafCompartments;

  std::map<SETissueCompartment*, SELiquidCompartment*> m_ExtracellularFluid;
  std::map<SETissueCompartment*, SELiquidCompartment*> m_IntracellularFluid;
};
}