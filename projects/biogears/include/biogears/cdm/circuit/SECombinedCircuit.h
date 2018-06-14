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

#define COMBINED_CIRCUIT_TEMPLATE typename CombinedCircuitBindType, typename CircuitType, typename CircuitBindType, typename NodeType, typename CircuitNodeBindType, typename PathType, typename CircuitPathBindType
#define COMBINED_CIRCUIT_TYPES CombinedCircuitBindType, CircuitType, CircuitBindType, NodeType, CircuitNodeBindType, PathType, CircuitPathBindType

template <COMBINED_CIRCUIT_TEMPLATE>
class SECombinedCircuit : public CircuitType {
public:
  SECombinedCircuit(const std::string& name, Logger* logger);
  virtual ~SECombinedCircuit();

  virtual void Clear(); //clear memory

  bool Load(const CombinedCircuitBindType& in);
  CombinedCircuitBindType* Unload() const;

protected:
  void Unload(CombinedCircuitBindType& data) const;

public:
  virtual void AddCircuit(CircuitType& c);
  virtual void RemoveCircuit(CircuitType& c);
  virtual CircuitType* GetCircuit(const std::string& name);

  virtual NodeType& CreateNode(const std::string& name);
  virtual NodeType* GetNode(const std::string& name) const;
  virtual void DeleteNode(const NodeType& node);
  virtual void DeleteNode(const std::string& name);
  virtual void RemoveNode(const NodeType& node);
  virtual void RemoveNode(const std::string& name);

  virtual PathType& CreatePath(NodeType& src, NodeType& tgt, const std::string& name);
  virtual PathType* GetPath(const std::string& path) const;
  virtual void DeletePath(const PathType& path);
  virtual void DeletePath(const std::string& name);
  virtual void RemovePath(const PathType& path);
  virtual void RemovePath(const std::string& name);

protected:
  std::vector<NodeType*> m_LinkNodes;
  std::vector<PathType*> m_LinkPaths;
  std::vector<std::string> m_RemovedNodes;
  std::vector<std::string> m_RemovedPaths;
  std::map<std::string, CircuitType*> m_Circuits;
};
#include <biogears/cdm/circuit/SECombinedCircuit.inl>