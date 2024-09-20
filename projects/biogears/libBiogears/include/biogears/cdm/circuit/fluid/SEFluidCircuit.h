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

#include <map>
#include <vector>

#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/circuit/SECircuitLedger.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>
#include <biogears/schema/cdm/Circuit.hxx>

#pragma warning(disable : 4661)
namespace std {
extern template class vector<biogears::SEFluidCircuitNode*>;
extern template class vector<biogears::SEFluidCircuitPath*>;
extern template class map<const biogears::SEFluidCircuitNode*, vector<biogears::SEFluidCircuitPath*>*>;
extern template class map<const biogears::SEFluidCircuitNode*, size_t>;
}
#pragma warning(default : 4661)
#define FLUID_CIRCUIT_TYPES CDM::FluidCircuitData, SEFluidCircuitNode, CDM::FluidCircuitNodeData, SEFluidCircuitPath, CDM::FluidCircuitPathData

#define FLUID_LEDGER_TYPES SEFluidCircuitNode, SEFluidCircuitPath, SEFluidCircuit

namespace biogears {
class SECircuitManager;
namespace io {
  class Circuit;
}
#pragma warning(disable : 4661)
extern template class SECircuit<CDM::FluidCircuitData, SEFluidCircuitNode, CDM::FluidCircuitNodeData, SEFluidCircuitPath, CDM::FluidCircuitPathData>;

class BIOGEARS_API SEFluidCircuit : public SECircuit<CDM::FluidCircuitData, SEFluidCircuitNode, CDM::FluidCircuitNodeData, SEFluidCircuitPath, CDM::FluidCircuitPathData> {
  friend class SECircuitManager;
  friend io::Circuit;

protected:
  SEFluidCircuit(const std::string& name, SECircuitManager& mgr);
  SEFluidCircuit(const char* name, SECircuitManager& mgr);

public:
  virtual ~SEFluidCircuit();

  SEFluidCircuitNode& CreateNode(const std::string& name);
  SEFluidCircuitNode& CreateNode(const char* name);
  SEFluidCircuitPath& CreatePath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const std::string& name);
  SEFluidCircuitPath& CreatePath(SEFluidCircuitNode& src, SEFluidCircuitNode& tgt, const char* name);

  void AddCircuit(SEFluidCircuit& circuit);

protected:
  SECircuitManager& m_Mgr;
};

extern template class SECircuitLedger<FLUID_LEDGER_TYPES>;
#pragma warning(default : 4661)
}
