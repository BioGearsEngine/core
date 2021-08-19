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
#include <string>
#include <vector>

#include <biogears/cdm/compartment/SECompartmentGraph.h>

#define COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE typename GraphType, typename GraphVertexType, typename GraphEdgeType, typename CompartmentType, typename CompartmentLinkType
#define COMPARTMENT_TRANSPORT_GRAPH_TYPES GraphType, GraphVertexType, GraphEdgeType, CompartmentType, CompartmentLinkType

namespace biogears {


template <COMPARTMENT_TRANSPORT_GRAPH_TEMPLATE>
class SECompartmentTransportGraph : public GraphType, public SECompartmentGraph<COMPARTMENT_GRAPH_TYPES> {
public:
  SECompartmentTransportGraph(const char* name, Logger* logger);
  SECompartmentTransportGraph(const std::string& name, Logger* logger);
  virtual ~SECompartmentTransportGraph();

  virtual void Clear() override;
  /*
  virtual bool Load(const BindType& in);
  virtual BindType* Unload() = 0;
protected:
  virtual void Unload(BindType& data);
*/

public:
  virtual std::string GetName() const override;
  virtual const char* GetName_cStr() const override;

  virtual void StateChange();

protected:
  virtual void BalanceByIntensive() override = 0;
  virtual size_t GetVertexIndex(const GraphVertexType& v) const override;
  virtual const std::vector<GraphVertexType*>& GetVerticies() const override;
  virtual const std::vector<GraphEdgeType*>* GetSourceEdges(const GraphVertexType& v) const override;
  virtual const std::vector<GraphEdgeType*>* GetTargetEdges(const GraphVertexType& v) const override;

  std::vector<GraphVertexType*> m_Verticies;
  std::map<const GraphVertexType*, size_t> m_VertexIndicies;
  std::map<const GraphVertexType*, std::vector<GraphEdgeType*>*> m_SourceEdgeMap;
  std::map<const GraphVertexType*, std::vector<GraphEdgeType*>*> m_TargetEdgeMap;
};
}