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
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/circuit/SECircuitNode.h>
#include <biogears/cdm/circuit/SECircuitPath.h>

#define CIRCUIT_TEMPLATE typename CircuitBindType, typename NodeType, typename CircuitNodeBindType, typename PathType, typename CircuitPathBindType
#define CIRCUIT_TYPES CircuitBindType, NodeType, CircuitNodeBindType, PathType, CircuitPathBindType

CDM_BIND_DECL(CircuitData)

namespace biogears {
template <CIRCUIT_TEMPLATE>
class SECircuit : public Loggable {
public:
  SECircuit(const char* name, Logger* logger);
  SECircuit(const std::string& name, Logger* logger);
  virtual ~SECircuit();

  virtual void Clear(); //clear memory

  virtual bool Load(const CircuitBindType& in, const std::map<std::string, NodeType*>& nodes, const std::map<std::string, PathType*>& paths);
  virtual CircuitBindType* Unload() const;

protected:
  virtual void Unload(CircuitBindType& data) const;

public:
  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;

  virtual bool HasReferenceNode() const;
  virtual bool IsReferenceNode(NodeType& node) const;
  virtual const std::vector<NodeType*>& GetReferenceNodes() const;
  virtual void AddReferenceNode(NodeType& node);

  // Nodes //
  virtual void AddNode(NodeType& node);
  virtual bool HasNode(NodeType& node);
  virtual bool HasNode(const char* name);
  virtual bool HasNode(const std::string& name);
  virtual NodeType* GetNode(const char* name);
  virtual NodeType* GetNode(const std::string& name);
  virtual const NodeType* GetNode(const char* name) const;
  virtual const NodeType* GetNode(const std::string& name) const;
  virtual const std::vector<NodeType*>& GetNodes() const;
  virtual void RemoveNode(const NodeType& node);
  virtual void RemoveNode(const char* name);
  virtual void RemoveNode(const std::string& name);
  size_t GetCalculatorIndex(const NodeType& node) const; // Does not count the reference node

  // Paths //
  virtual void AddPath(PathType& node);
  virtual bool HasPath(PathType& node);
  virtual bool HasPath(const char* name);
  virtual bool HasPath(const std::string& name);
  virtual PathType* GetPath(const char* name);
  virtual PathType* GetPath(const std::string& name);
  virtual const PathType* GetPath(const char* name) const;
  virtual const PathType* GetPath(const std::string& name) const;
  virtual const std::vector<PathType*>& GetPaths() const;
  virtual void RemovePath(const PathType& path);
  virtual void RemovePath(const char* name);
  virtual void RemovePath(const std::string& name);
  virtual const std::vector<PathType*>& GetValvePaths();
  virtual const std::vector<PathType*>& GetPolarizedElementPaths();

  virtual const std::vector<PathType*>* GetTargetPaths(const NodeType& node) const;
  virtual const std::vector<PathType*>* GetSourcePaths(const NodeType& node) const;
  virtual const std::vector<PathType*>* GetConnectedPaths(const NodeType& node) const;

  virtual void StateChange();
  virtual void SetNextAndCurrentFromBaselines();

protected:
  std::string m_Name;
  std::stringstream m_ss;

  std::vector<NodeType*> m_ReferenceNodes;
  std::vector<NodeType*> m_Nodes;
  std::vector<PathType*> m_Paths;

  std::map<const NodeType*, std::vector<PathType*>*> m_TargetPathMap;
  std::map<const NodeType*, std::vector<PathType*>*> m_SourcePathMap;
  std::map<const NodeType*, std::vector<PathType*>*> m_ConnectedPathMap;

  std::vector<PathType*> m_ValvePaths;
  std::vector<PathType*> m_PolarizedElementPaths;
  std::map<const NodeType*, size_t> m_CalculatorIndex; // A unique id (starting at 0) for all nodes except the reference node
};
}
#include <biogears/cdm/circuit/SECircuit.inl>
