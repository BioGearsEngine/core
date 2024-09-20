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

#include <vector>
#include <map>

#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/circuit/SECircuitLedger.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuitNode.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuitPath.h>
#include <biogears/schema/cdm/Circuit.hxx>

#pragma warning(disable : 4661)

namespace std {
extern template class vector<biogears::SEThermalCircuitNode*>;
extern template class vector<biogears::SEThermalCircuitPath*>;
extern template class map<const biogears::SEThermalCircuitNode*, vector<biogears::SEThermalCircuitPath*>*>;
extern template class map<const biogears::SEThermalCircuitNode*, size_t>;
}
#pragma warning(default : 4661)


#define THERMAL_CIRCUIT_TYPES CDM::ThermalCircuitData, SEThermalCircuitNode, CDM::ThermalCircuitNodeData, SEThermalCircuitPath, CDM::ThermalCircuitPathData
#define THERMAL_LEDGER_TYPES SEThermalCircuitNode, SEThermalCircuitPath, SEThermalCircuit

namespace biogears {
class SECircuitManager;
#pragma warning(disable : 4661)

extern template class SECircuit<CDM::ThermalCircuitData, SEThermalCircuitNode, CDM::ThermalCircuitNodeData, SEThermalCircuitPath, CDM::ThermalCircuitPathData>;

class BIOGEARS_API SEThermalCircuit : public SECircuit<CDM::ThermalCircuitData, SEThermalCircuitNode, CDM::ThermalCircuitNodeData, SEThermalCircuitPath, CDM::ThermalCircuitPathData> {
  friend class SECircuitManager;

protected:
  SEThermalCircuit(const char* name, SECircuitManager& mgr);
  SEThermalCircuit(const std::string& name, SECircuitManager& mgr);

public:
  virtual ~SEThermalCircuit() override;

  SEThermalCircuitNode& CreateNode(const char* name);
  SEThermalCircuitNode& CreateNode(const std::string& name);
  SEThermalCircuitPath& CreatePath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const char* name);
  SEThermalCircuitPath& CreatePath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name);

  void AddCircuit(SEThermalCircuit& circuit);

protected:
  SECircuitManager& m_Mgr;
};

extern template class SECircuitLedger<THERMAL_LEDGER_TYPES>;
#pragma warning(default : 4661)

}
