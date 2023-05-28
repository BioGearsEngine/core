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
#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/circuit/electrical/SEElectricalCircuitNode.h>
#include <biogears/cdm/circuit/electrical/SEElectricalCircuitPath.h>

namespace std {
BG_EXT template class BIOGEARS_API vector<biogears::SEElectricalCircuitNode*>;
BG_EXT template class BIOGEARS_API vector<biogears::SEElectricalCircuitPath*>;
BG_EXT template class BIOGEARS_API map<const biogears::SEElectricalCircuitNode*, vector<biogears::SEElectricalCircuitPath*>*>;
BG_EXT template class BIOGEARS_API map<const biogears::SEElectricalCircuitNode*, size_t>;
}
OPEN_BIOGEARS_NAMESPACE
class SECircuitManager;

BG_EXT template class BIOGEARS_API SECircuit<CDM::ElectricalCircuitData, SEElectricalCircuitNode, CDM::ElectricalCircuitNodeData, SEElectricalCircuitPath, CDM::ElectricalCircuitPathData>;

class BIOGEARS_API SEElectricalCircuit : public SECircuit<CDM::ElectricalCircuitData, SEElectricalCircuitNode, CDM::ElectricalCircuitNodeData, SEElectricalCircuitPath, CDM::ElectricalCircuitPathData> {
  friend class SECircuitManager;

protected:
  SEElectricalCircuit(const std::string& name, SECircuitManager& mgr);
  SEElectricalCircuit(const char* name, SECircuitManager& mgr);

public:
  virtual ~SEElectricalCircuit();

  SEElectricalCircuitNode& CreateNode(const std::string& name);
  SEElectricalCircuitNode& CreateNode(const char* name);
  SEElectricalCircuitPath& CreatePath(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const std::string& name);
  SEElectricalCircuitPath& CreatePath(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const char* name);
  void AddCircuit(SEElectricalCircuit& circuit);

protected:
  SECircuitManager& m_Mgr;
};
CLOSE_BIOGEARS_NAMESPACE