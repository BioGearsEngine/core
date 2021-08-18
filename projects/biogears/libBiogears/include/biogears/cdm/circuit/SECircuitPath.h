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
#include <biogears/schema/cdm/Properties.hxx>

#include <biogears/cdm/properties/SEScalarElectricCapacitance.h>
#include <biogears/cdm/properties/SEScalarElectricCharge.h>
#include <biogears/cdm/properties/SEScalarElectricCurrent.h>
#include <biogears/cdm/properties/SEScalarElectricInductance.h>
#include <biogears/cdm/properties/SEScalarElectricPotential.h>
#include <biogears/cdm/properties/SEScalarElectricResistance.h>
#include <biogears/cdm/properties/SEScalarEnergy.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFlowInertance.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>
#include <biogears/cdm/properties/SEScalarHeatInductance.h>
#include <biogears/cdm/properties/SEScalarHeatResistance.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

#define CIRCUIT_PATH_TEMPLATE typename FluxScalar, typename ResistanceScalar, typename CapacitanceScalar, typename InductanceScalar, typename PotentialScalar, typename QuantityScalar
#define CIRCUIT_PATH_TYPES FluxScalar, ResistanceScalar, CapacitanceScalar, InductanceScalar, PotentialScalar, QuantityScalar
#define ELECTRICAL_CIRCUIT_PATH SEScalarElectricCurrent, SEScalarElectricResistance, SEScalarElectricCapacitance, SEScalarElectricInductance, SEScalarElectricPotential, SEScalarElectricCharge
#define FLUID_CIRCUIT_PATH SEScalarVolumePerTime, SEScalarFlowResistance, SEScalarFlowCompliance, SEScalarFlowInertance, SEScalarPressure, SEScalarVolume
#define THERMAL_CIRCUIT_PATH SEScalarPower, SEScalarHeatResistance, SEScalarHeatCapacitance, SEScalarHeatInductance, SEScalarTemperature, SEScalarEnergy

CDM_BIND_DECL(CircuitPathData)

namespace biogears {

namespace io {
  class Circuit;
}

template <CIRCUIT_PATH_TEMPLATE>
class SECircuitPath : public Loggable {
  template <typename CircuitBindType, typename NodeType, typename CircuitNodeBindType, typename PathType, typename CircuitPathBindType>
  friend class SECircuit;
  friend io::Circuit;

protected:
  SECircuitPath(SECircuitNode<PotentialScalar, QuantityScalar>& src, SECircuitNode<PotentialScalar, QuantityScalar>& tgt, const char* name);
  SECircuitPath(SECircuitNode<PotentialScalar, QuantityScalar>& src, SECircuitNode<PotentialScalar, QuantityScalar>& tgt, const std::string& name);

public:
  virtual ~SECircuitPath();

  virtual void Clear();

  virtual bool Load(const CDM::CircuitPathData& in);
  virtual CDM::CircuitPathData* Unload() const = 0;

protected:
  virtual void Unload(CDM::CircuitPathData& data) const;

public:
  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;

  virtual SECircuitNode<CIRCUIT_NODE_TYPES>& GetSourceNode() const;
  virtual SECircuitNode<CIRCUIT_NODE_TYPES>& GetTargetNode() const;

  virtual bool HasFlux() const;
  virtual FluxScalar& GetFlux();
  virtual bool HasNextFlux() const;
  virtual FluxScalar& GetNextFlux();
  virtual bool HasFluxSource() const;
  virtual FluxScalar& GetFluxSource();
  virtual bool HasNextFluxSource() const;
  virtual FluxScalar& GetNextFluxSource();
  virtual bool HasFluxSourceBaseline() const;
  virtual FluxScalar& GetFluxSourceBaseline();

  virtual bool HasResistance() const;
  virtual ResistanceScalar& GetResistance();
  virtual bool HasNextResistance() const;
  virtual ResistanceScalar& GetNextResistance();
  virtual bool HasResistanceBaseline() const;
  virtual ResistanceScalar& GetResistanceBaseline();

  virtual bool HasCapacitance() const;
  virtual CapacitanceScalar& GetCapacitance();
  virtual bool HasNextCapacitance() const;
  virtual CapacitanceScalar& GetNextCapacitance();
  virtual bool HasCapacitanceBaseline() const;
  virtual CapacitanceScalar& GetCapacitanceBaseline();

  virtual bool HasInductance() const;
  virtual InductanceScalar& GetInductance();
  virtual bool HasNextInductance() const;
  virtual InductanceScalar& GetNextInductance();
  virtual bool HasInductanceBaseline() const;
  virtual InductanceScalar& GetInductanceBaseline();

  virtual bool HasPotentialSource() const;
  virtual PotentialScalar& GetPotentialSource();
  virtual bool HasNextPotentialSource() const;
  virtual PotentialScalar& GetNextPotentialSource();
  virtual bool HasPotentialSourceBaseline() const;
  virtual PotentialScalar& GetPotentialSourceBaseline();
  virtual bool HasValveBreakdownPotential() const;
  virtual PotentialScalar& GetValveBreakdownPotential();

  virtual bool HasValidElements() const;
  virtual unsigned short NumberOfElements() const { return m_NumElements; }
  virtual unsigned short NumberOfNextElements() const { return m_NumNextElements; }

  virtual CDM::enumOpenClosed::value GetSwitch() const;
  virtual void SetSwitch(CDM::enumOpenClosed::value state);
  virtual void FlipSwitch();
  virtual bool HasSwitch() const;
  virtual void InvalidateSwitch();

  virtual CDM::enumOpenClosed::value GetNextSwitch() const;
  virtual void SetNextSwitch(CDM::enumOpenClosed::value state);
  virtual void FlipNextSwitch();
  virtual bool HasNextSwitch() const;
  virtual void InvalidateNextSwitch();

  virtual CDM::enumOpenClosed::value GetValve() const;
  virtual void SetValve(CDM::enumOpenClosed::value state);
  virtual void FlipValve();
  virtual bool HasValve() const;
  virtual void InvalidateValve();

  virtual CDM::enumOpenClosed::value GetNextValve() const;
  virtual void SetNextValve(CDM::enumOpenClosed::value state);
  virtual void FlipNextValve();
  virtual bool HasNextValve() const;
  virtual void InvalidateNextValve();

  virtual CDM::enumOpenClosed::value GetPolarizedState() const;
  virtual void SetPolarizedState(CDM::enumOpenClosed::value state);
  virtual void FlipPolarizedState();
  virtual bool HasPolarizedState() const;
  virtual void InvalidatePolarizedState();

  virtual CDM::enumOpenClosed::value GetNextPolarizedState() const;
  virtual void SetNextPolarizedState(CDM::enumOpenClosed::value state);
  virtual void FlipNextPolarizedState();
  virtual bool HasNextPolarizedState() const;
  virtual void InvalidateNextPolarizedState();

protected:
  std::string m_Name;
  SECircuitNode<CIRCUIT_NODE_TYPES>& m_SourceNode;
  SECircuitNode<CIRCUIT_NODE_TYPES>& m_TargetNode;
  mutable unsigned short m_NumElements;
  mutable unsigned short m_NumNextElements;
  /////////////////////////
  // Valves and Switches //
  /////////////////////////
  CDM::enumOpenClosed::value m_Switch;
  CDM::enumOpenClosed::value m_NextSwitch;
  CDM::enumOpenClosed::value m_Valve;
  CDM::enumOpenClosed::value m_NextValve;
  CDM::enumOpenClosed::value m_PolarizedState;
  CDM::enumOpenClosed::value m_NextPolarizedState;

  ResistanceScalar* m_Resistance;
  ResistanceScalar* m_NextResistance;
  ResistanceScalar* m_ResistanceBaseline;

  CapacitanceScalar* m_Capacitance;
  CapacitanceScalar* m_NextCapacitance;
  CapacitanceScalar* m_CapacitanceBaseline;

  InductanceScalar* m_Inductance;
  InductanceScalar* m_NextInductance;
  InductanceScalar* m_InductanceBaseline;

  FluxScalar* m_Flux;
  FluxScalar* m_NextFlux;
  FluxScalar* m_FluxSource;
  FluxScalar* m_NextFluxSource;
  FluxScalar* m_FluxSourceBaseline;

  PotentialScalar* m_PotentialSource;
  PotentialScalar* m_NextPotentialSource;
  PotentialScalar* m_PotentialSourceBaseline;
  PotentialScalar* m_ValveBreakdownPotential;
};

BG_EXT template class BIOGEARS_API SECircuitPath<ELECTRICAL_CIRCUIT_PATH>;
BG_EXT template class BIOGEARS_API SECircuitPath<FLUID_CIRCUIT_PATH>;
BG_EXT template class BIOGEARS_API SECircuitPath<THERMAL_CIRCUIT_PATH>;

}
#include <biogears/cdm/circuit/SECircuitPath.inl>
