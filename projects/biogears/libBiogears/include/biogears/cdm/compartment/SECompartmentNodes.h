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
#include <biogears/cdm/compartment/SECompartment.h>

#define COMPARTMENT_NODE_TEMPLATE typename NodeType, typename PotentialScalar, typename PotentialUnit, typename QuantityScalar, typename QuantityUnit
#define COMPARTMENT_NODE_TYPES NodeType, PotentialScalar, PotentialUnit, QuantityScalar, QuantityUnit
#define ELECTRICAL_COMPARTMENT_NODE SEElectricalCircuitNode, SEScalarElectricPotential, ElectricPotentialUnit, SEScalarElectricCharge, ElectricChargeUnit
#define FLUID_COMPARTMENT_NODE SEFluidCircuitNode, SEScalarPressure, PressureUnit, SEScalarVolume, VolumeUnit
#define THERMAL_COMPARTMENT_NODE SEThermalCircuitNode, SEScalarTemperature, TemperatureUnit, SEScalarEnergy, EnergyUnit

namespace biogears {
namespace io {
  class Compartment;
}

template <COMPARTMENT_NODE_TEMPLATE>
class SECompartmentNodes : public Loggable {
  friend io::Compartment;

public:
  SECompartmentNodes(Logger* logger);
  virtual ~SECompartmentNodes();

  virtual void Clear();
  virtual void StateChange(); //analyze and cache for optimization

  virtual bool HasMapping() const { return !m_AllNodes.empty(); }
  virtual std::vector<NodeType*>& GetNodes() { return m_AllNodes; }
  virtual void RemoveNode(NodeType& node);
  virtual void MapNode(NodeType& node);

  virtual bool HasQuantity() const { return !m_QuantityNodes.empty(); }
  virtual QuantityScalar& GetQuantity();
  virtual double GetQuantity(const QuantityUnit& unit) const;

  virtual bool HasPotential() const;
  virtual PotentialScalar& GetPotential();
  virtual double GetPotential(const PotentialUnit& unit) const;

protected:
  void SortNode(NodeType& node);
  virtual const std::vector<NodeType*>& GetAllNodes() const { return m_AllNodes; }
  virtual const std::vector<NodeType*>& GetQuantityNodes() const { return m_QuantityNodes; }

  // I assume all nodes will have a pressure
  std::vector<NodeType*> m_QuantityNodes;
  std::vector<NodeType*> m_AllNodes; //Even nodes without quantity (ex. volume)

  QuantityScalar* m_Quantity;
  PotentialScalar* m_Potential;
};

BG_EXT template class BIOGEARS_API SECompartmentNodes<ELECTRICAL_COMPARTMENT_NODE>;
BG_EXT template class BIOGEARS_API SECompartmentNodes<FLUID_COMPARTMENT_NODE>;
BG_EXT template class BIOGEARS_API SECompartmentNodes<THERMAL_COMPARTMENT_NODE>;

}
#include <biogears/cdm/compartment/SECompartmentNodes.inl>
