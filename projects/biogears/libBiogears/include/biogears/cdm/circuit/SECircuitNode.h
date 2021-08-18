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

#include <biogears/cdm/properties/SEScalarElectricCharge.h>
#include <biogears/cdm/properties/SEScalarElectricPotential.h>
#include <biogears/cdm/properties/SEScalarEnergy.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarVolume.h>

CDM_BIND_DECL(CircuitNodeData);

namespace biogears {

#define CIRCUIT_NODE_TEMPLATE typename PotentialScalar, typename QuantityScalar
#define CIRCUIT_NODE_TYPES PotentialScalar, QuantityScalar
#define ELECTRICAL_CIRCUIT_NODE SEScalarElectricPotential, SEScalarElectricCharge
#define FLUID_CIRCUIT_NODE SEScalarPressure, SEScalarVolume
#define THERMAL_CIRCUIT_NODE SEScalarTemperature, SEScalarEnergy

namespace io {
  class Circuit;
}

template <typename PotentialScalar, typename QuantityScalar>
class SECircuitNode : public Loggable {
  template <typename CircuitBindType, typename NodeType, typename CircuitNodeBindType, typename PathType, typename CircuitPathBindType>
  friend class SECircuit;
  friend class io::Circuit;

protected:
  SECircuitNode(const char* name, Logger* logger);
  SECircuitNode(const std::string& name, Logger* logger);

public:
  virtual ~SECircuitNode();

  virtual void Clear(); //clear memory

  virtual bool Load(const CDM::CircuitNodeData& in);
  virtual CDM::CircuitNodeData* Unload() const = 0;

protected:
  virtual void Unload(CDM::CircuitNodeData& data) const;

public:
  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;

  virtual bool HasPotential() const;
  virtual PotentialScalar& GetPotential();
  virtual bool HasNextPotential() const;
  virtual PotentialScalar& GetNextPotential();

  virtual bool HasQuantity() const;
  virtual QuantityScalar& GetQuantity();
  virtual bool HasNextQuantity() const;
  virtual QuantityScalar& GetNextQuantity();
  virtual bool HasQuantityBaseline() const;
  virtual QuantityScalar& GetQuantityBaseline();

protected:
  std::string m_Name;

  PotentialScalar* m_Potential;
  PotentialScalar* m_NextPotential;

  ////////////////////
  // Quantity Types //
  ////////////////////
  QuantityScalar* m_Quantity;
  QuantityScalar* m_NextQuantity;
  QuantityScalar* m_QuantityBaseline;
};

}
#include <biogears/cdm/circuit/SECircuitNode.inl>

namespace biogears{
BG_EXT template class SECircuitNode<SEScalarElectricPotential, SEScalarElectricCharge>;
BG_EXT template class SECircuitNode<SEScalarPressure, SEScalarVolume>;
BG_EXT template class SECircuitNode<SEScalarTemperature, SEScalarEnergy>;
}