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
#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>

namespace std {
BG_EXT template class BIOGEARS_API vector<biogears::SEFluidCircuitNode*>;
BG_EXT template class BIOGEARS_API vector<biogears::SEFluidCircuitPath*>;
BG_EXT template class BIOGEARS_API map<const biogears::SEFluidCircuitNode*, vector<biogears::SEFluidCircuitPath*>*>;
BG_EXT template class BIOGEARS_API map<const biogears::SEFluidCircuitNode*, size_t>;
}

namespace biogears {
class SECircuitManager;
namespace io {
  class Circuit;
}
BG_EXT template class BIOGEARS_API SECircuit<CDM::FluidCircuitData, SEFluidCircuitNode, CDM::FluidCircuitNodeData, SEFluidCircuitPath, CDM::FluidCircuitPathData>;

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
}