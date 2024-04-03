/**************************************************************************************
Copyright 2022 Applied Research Associates, Inc.
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
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/schema/cdm/Properties.hxx>

CDM_BIND_DECL(CircuitManagerData);

#define CIRCUIT_LEDGER_TEMPLATE typename NodeType, typename PathType, typename CircuitType
#define CIRCUIT_LEDGER_TYPES NodeType, PathType, CircuitType

namespace biogears {
namespace io {
  class Circuit;
}
template <CIRCUIT_LEDGER_TEMPLATE>
class SECircuitLedger {
  friend class SECircuitManager;
  friend io::Circuit;

protected:
  SECircuitLedger() {};

public:
  virtual ~SECircuitLedger() { Clear(); };
  void Clear()
  {
    DELETE_MAP_SECOND(nodes);
    DELETE_MAP_SECOND(paths);
    DELETE_MAP_SECOND(circuits);
  }
  std::map<std::string, NodeType*> nodes;
  std::map<std::string, PathType*> paths;
  std::map<std::string, CircuitType*> circuits;
};

}