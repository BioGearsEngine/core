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

#include <string>

namespace biogears {
template <COMBINED_CIRCUIT_TEMPLATE>
class SECombinedCircuit : public CircuitType {
public:
  SECombinedCircuit(const char* name, Logger* logger);
  SECombinedCircuit(const std::string& name, Logger* logger);
  virtual ~SECombinedCircuit() override;

  virtual void Clear() override; //clear memory

  bool Load(const CombinedCircuitBindType& in);
  CombinedCircuitBindType* Unload() const;

protected:
  void Unload(CombinedCircuitBindType& data) const;

public:
  virtual void AddCircuit(CircuitType& c) override;
  virtual void RemoveCircuit(CircuitType& c) override;
  virtual CircuitType* GetCircuit(const char* name) override;
  virtual CircuitType* GetCircuit(const std::string& name) override;

  virtual NodeType& CreateNode(const char* name) override;
  virtual NodeType& CreateNode(const std::string& name) override;
  virtual NodeType* GetNode(const char* name) const override;
  virtual NodeType* GetNode(const std::string& name) const override;
  virtual void DeleteNode(const NodeType& node) override;
  virtual void DeleteNode(const char* name) override;
  virtual void DeleteNode(const std::string& name) override;
  virtual void RemoveNode(const NodeType& node) override;
  virtual void RemoveNode(const char* name) override;
  virtual void RemoveNode(const std::string& name) override;

  virtual PathType& CreatePath(NodeType& src, NodeType& tgt, const char* name) override;
  virtual PathType& CreatePath(NodeType& src, NodeType& tgt, const std::string& name) override;
  virtual PathType* GetPath(const char* path) const override;
  virtual PathType* GetPath(const std::string& path) const override;
  virtual void DeletePath(const PathType& path) override;
  virtual void DeletePath(const char* name) override;
  virtual void DeletePath(const std::string& name) override;
  virtual void RemovePath(const PathType& path) override;
  virtual void RemovePath(const char* name) override;
  virtual void RemovePath(const std::string& name) override;

protected:
  std::vector<NodeType*> m_LinkNodes;
  std::vector<PathType*> m_LinkPaths;
  std::vector<std::string> m_RemovedNodes;
  std::vector<std::string> m_RemovedPaths;
  std::map<std::string, CircuitType*> m_Circuits;
};
}
#include <biogears/cdm/circuit/SECombinedCircuit.inl>