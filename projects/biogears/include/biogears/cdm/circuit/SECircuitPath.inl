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

#include <biogears/cdm/circuit/SECircuitNode.h>
#include <biogears/cdm/circuit/SECircuitPath.h>
#include <biogears/schema/cdm/Circuit.hxx>

namespace biogears {
  template <CIRCUIT_PATH_TEMPLATE>
  SECircuitPath<CIRCUIT_PATH_TYPES>::SECircuitPath(SECircuitNode<PotentialScalar, QuantityScalar>& src, SECircuitNode<PotentialScalar, QuantityScalar>& tgt, const char* name)
    : SECircuitPath(src, tgt, std::string{ name })
{}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
SECircuitPath<CIRCUIT_PATH_TYPES>::SECircuitPath(SECircuitNode<PotentialScalar, QuantityScalar>& src, SECircuitNode<PotentialScalar, QuantityScalar>& tgt, const std::string& name)
  : Loggable(src.GetLogger())
  , m_SourceNode(src)
  , m_TargetNode(tgt)
  , m_Name(name)
{
  m_Resistance = nullptr;
  m_NextResistance = nullptr;
  m_ResistanceBaseline = nullptr;
  m_Capacitance = nullptr;
  m_NextCapacitance = nullptr;
  m_CapacitanceBaseline = nullptr;
  m_Inductance = nullptr;
  m_NextInductance = nullptr;
  m_InductanceBaseline = nullptr;
  m_Flux = nullptr;
  m_NextFlux = nullptr;
  m_FluxSource = nullptr;
  m_NextFluxSource = nullptr;
  m_FluxSourceBaseline = nullptr;
  m_PotentialSource = nullptr;
  m_NextPotentialSource = nullptr;
  m_PotentialSourceBaseline = nullptr;
  m_ValveBreakdownPotential = nullptr;
  m_Switch = (CDM::enumOpenClosed::value) - 1;
  m_Valve = (CDM::enumOpenClosed::value) - 1;
  m_NextSwitch = (CDM::enumOpenClosed::value) - 1;
  m_NextValve = (CDM::enumOpenClosed::value) - 1;
  m_NextPolarizedState = (CDM::enumOpenClosed::value) - 1;
  m_PolarizedState = (CDM::enumOpenClosed::value) - 1;

  m_NumElements = 0;
  m_NumNextElements = 0;

  if (&m_SourceNode == &m_TargetNode) {
    std::stringstream ss;
    ss << "Path " << GetName() << " has the same Source and Target Nodes.  They cannot be the same Node.";
    Fatal(ss);
  }
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
SECircuitPath<CIRCUIT_PATH_TYPES>::~SECircuitPath()
{
  Clear();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::Clear()
{
  m_Switch = (CDM::enumOpenClosed::value)-1;
  m_Valve = (CDM::enumOpenClosed::value)-1;
  m_NextSwitch = (CDM::enumOpenClosed::value)-1;
  m_NextValve = (CDM::enumOpenClosed::value)-1;
  m_NextPolarizedState = (CDM::enumOpenClosed::value)-1;
  m_PolarizedState = (CDM::enumOpenClosed::value)-1;
  SAFE_DELETE(m_Resistance);
  SAFE_DELETE(m_NextResistance);
  SAFE_DELETE(m_ResistanceBaseline);
  SAFE_DELETE(m_Capacitance);
  SAFE_DELETE(m_NextCapacitance);
  SAFE_DELETE(m_CapacitanceBaseline);
  SAFE_DELETE(m_Inductance);
  SAFE_DELETE(m_NextInductance);
  SAFE_DELETE(m_InductanceBaseline);
  SAFE_DELETE(m_Flux);
  SAFE_DELETE(m_NextFlux);
  SAFE_DELETE(m_FluxSource);
  SAFE_DELETE(m_NextFluxSource);
  SAFE_DELETE(m_FluxSourceBaseline);
  SAFE_DELETE(m_PotentialSource);
  SAFE_DELETE(m_NextPotentialSource);
  SAFE_DELETE(m_PotentialSourceBaseline);
  SAFE_DELETE(m_ValveBreakdownPotential);
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::Load(const CDM::CircuitPathData& in)
{
  Clear();
  if (in.Switch().present())
    SetSwitch(in.Switch().get());
  if (in.NextSwitch().present())
    SetNextSwitch(in.NextSwitch().get());
  if (in.Valve().present())
    SetValve(in.Valve().get());
  if (in.NextValve().present())
    SetNextValve(in.NextValve().get());
  if (in.PolarizedState().present())
    SetPolarizedState(in.PolarizedState().get());
  if (in.NextPolarizedState().present())
    SetNextPolarizedState(in.NextPolarizedState().get());
  return true;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::Unload(CDM::CircuitPathData& data) const
{
  data.Name(m_Name);
  data.SourceNode(m_SourceNode.GetName());
  data.TargetNode(m_TargetNode.GetName());
  if (HasSwitch())
    data.Switch(m_Switch);
  if (HasNextSwitch())
    data.NextSwitch(m_NextSwitch);
  if (HasValve())
    data.Valve(m_Valve);
  if (HasNextValve())
    data.NextValve(m_NextValve);
  if (HasPolarizedState())
    data.PolarizedState(m_PolarizedState);
  if (HasNextPolarizedState())
    data.NextPolarizedState(m_NextPolarizedState);
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
std::string SECircuitPath<CIRCUIT_PATH_TYPES>::GetName() const
{
  return m_Name;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
const char* SECircuitPath<CIRCUIT_PATH_TYPES>::GetName_cStr() const
{
  return m_Name.c_str();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
SECircuitNode<PotentialScalar, QuantityScalar>& SECircuitPath<CIRCUIT_PATH_TYPES>::GetSourceNode() const
{
  return m_SourceNode;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
SECircuitNode<PotentialScalar, QuantityScalar>& SECircuitPath<CIRCUIT_PATH_TYPES>::GetTargetNode() const
{
  return m_TargetNode;
}
//-------------------------------------------------------------------------------
/////////////////////////
// Valves and Switches //
/////////////////////////
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasValidElements() const
{
  int numRCI = (HasResistance() ? 1 : 0) + (HasCapacitance() ? 1 : 0) + (HasInductance() ? 1 : 0);
  int numNextRCI = (HasNextResistance() ? 1 : 0) + (HasNextCapacitance() ? 1 : 0) + (HasNextInductance() ? 1 : 0);
  int numSrcState = (HasSwitch() ? 1 : 0) + (HasValve() ? 1 : 0) + (HasFluxSource() ? 1 : 0) + (HasPotentialSource() ? 1 : 0);
  int numNextSrcState = (HasNextSwitch() ? 1 : 0) + (HasNextValve() ? 1 : 0) + (HasNextFluxSource() ? 1 : 0) + (HasNextPotentialSource() ? 1 : 0);
  m_NumElements = numRCI + numSrcState;
  m_NumNextElements = numNextRCI + numNextSrcState;
  if (m_NumElements > 1 || m_NumNextElements > 1) {
    Error("There is only one element allowed per path.");
    return false;
  }
  if (HasPolarizedState()) {
    if (numRCI == 0) {
      Error("You must have a Resistance, Capacitance or Inductance on the path if you have a polarized state.");
      return false;
    }
    if (numSrcState > 0) {
      Error("You cannot have a Switch, Valve, FluxSource or PotentialSource on a path with a polarized state.");
      return false;
    }
  }
  if (HasNextPolarizedState()) {
    if (numNextRCI == 0) {
      Error("You must have a Resistance, Capacitance or Inductance on the path if you have a polarized state.");
      return false;
    }
    if (numNextSrcState > 0) {
      Error("You cannot have a Switch, Valve, FluxSource or PotentialSource on a path with a polarized state.");
      return false;
    }
  }
  if (HasValveBreakdownPotential()) {
    if (!HasValve()) {
      Error("You cannot have a ValveBreakdownPotential if you do not have a valve.");
      return false;
    }
    if (m_ValveBreakdownPotential->IsNegative()) {
      Error("A negative Breakdown Potential is not allowed. Consider adding a Valve with reversed polarity.");
      return false;
    }
  }
  return true;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
CDM::enumOpenClosed::value SECircuitPath<CIRCUIT_PATH_TYPES>::GetSwitch() const
{
  return m_Switch;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetSwitch(CDM::enumOpenClosed::value state)
{
  m_Switch = state;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipSwitch()
{
  m_Switch = (m_Switch == CDM::enumOpenClosed::Open) ? CDM::enumOpenClosed::Closed : CDM::enumOpenClosed::Open;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasSwitch() const
{
  return m_Switch == (CDM::enumOpenClosed::value)-1 ? false : true;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidateSwitch()
{
  m_Switch = (CDM::enumOpenClosed::value)-1;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
CDM::enumOpenClosed::value SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextSwitch() const
{
  return m_NextSwitch;
}
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetNextSwitch(CDM::enumOpenClosed::value state)
{
  m_NextSwitch = state;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipNextSwitch()
{
  m_NextSwitch = (m_NextSwitch == CDM::enumOpenClosed::Open) ? CDM::enumOpenClosed::Closed : CDM::enumOpenClosed::Open;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextSwitch() const
{
  return m_NextSwitch == (CDM::enumOpenClosed::value)-1 ? false : true;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidateNextSwitch()
{
  m_NextSwitch = (CDM::enumOpenClosed::value)-1;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
CDM::enumOpenClosed::value SECircuitPath<CIRCUIT_PATH_TYPES>::GetValve() const
{
  return m_Valve;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetValve(CDM::enumOpenClosed::value state)
{
  m_Valve = state;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipValve()
{
  m_Valve = (m_Valve == CDM::enumOpenClosed::Open) ? CDM::enumOpenClosed::Closed : CDM::enumOpenClosed::Open;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasValve() const
{
  return m_Valve == (CDM::enumOpenClosed::value)-1 ? false : true;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidateValve()
{
  m_Valve = (CDM::enumOpenClosed::value)-1;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
CDM::enumOpenClosed::value SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextValve() const
{
  return m_NextValve;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetNextValve(CDM::enumOpenClosed::value state)
{
  m_NextValve = state;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipNextValve()
{
  m_NextValve = (m_NextValve == CDM::enumOpenClosed::Open) ? CDM::enumOpenClosed::Closed : CDM::enumOpenClosed::Open;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextValve() const
{
  return m_NextValve == (CDM::enumOpenClosed::value)-1 ? false : true;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidateNextValve()
{
  m_NextValve = (CDM::enumOpenClosed::value)-1;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
CDM::enumOpenClosed::value SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextPolarizedState() const
{
  return m_NextPolarizedState;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetNextPolarizedState(CDM::enumOpenClosed::value state)
{
  m_NextPolarizedState = state;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipNextPolarizedState()
{
  m_NextPolarizedState = (m_NextPolarizedState == CDM::enumOpenClosed::Open) ? CDM::enumOpenClosed::Closed : CDM::enumOpenClosed::Open;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextPolarizedState() const
{
  return m_NextPolarizedState == (CDM::enumOpenClosed::value)-1 ? false : true;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidateNextPolarizedState()
{
  m_NextPolarizedState = (CDM::enumOpenClosed::value)-1;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
CDM::enumOpenClosed::value SECircuitPath<CIRCUIT_PATH_TYPES>::GetPolarizedState() const
{
  return m_PolarizedState;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::SetPolarizedState(CDM::enumOpenClosed::value state)
{
  m_PolarizedState = state;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::FlipPolarizedState()
{
  m_PolarizedState = (m_PolarizedState == CDM::enumOpenClosed::Open) ? CDM::enumOpenClosed::Closed : CDM::enumOpenClosed::Open;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasPolarizedState() const
{
  return m_PolarizedState == (CDM::enumOpenClosed::value)-1 ? false : true;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
void SECircuitPath<CIRCUIT_PATH_TYPES>::InvalidatePolarizedState()
{
  m_PolarizedState = (CDM::enumOpenClosed::value)-1;
}
//-------------------------------------------------------------------------------
/////////////////
//  Resistance //
/////////////////
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasResistance() const
{
  return m_Resistance == nullptr ? false : m_Resistance->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
ResistanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetResistance()
{
  if (m_Resistance == nullptr)
    m_Resistance = new ResistanceScalar();
  return *m_Resistance;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextResistance() const
{
  return m_NextResistance == nullptr ? false : m_NextResistance->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
ResistanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextResistance()
{
  if (m_NextResistance == nullptr)
    m_NextResistance = new ResistanceScalar();
  return *m_NextResistance;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasResistanceBaseline() const
{
  return m_ResistanceBaseline == nullptr ? false : m_ResistanceBaseline->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
ResistanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetResistanceBaseline()
{
  if (m_ResistanceBaseline == nullptr)
    m_ResistanceBaseline = new ResistanceScalar();
  return *m_ResistanceBaseline;
}
//-------------------------------------------------------------------------------
///////////////////////
// Capacitance Types //
///////////////////////
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasCapacitance() const
{
  return m_Capacitance == nullptr ? false : m_Capacitance->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
CapacitanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetCapacitance()
{
  if (m_Capacitance == nullptr)
    m_Capacitance = new CapacitanceScalar();
  return *m_Capacitance;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextCapacitance() const
{
  return m_NextCapacitance == nullptr ? false : m_NextCapacitance->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
CapacitanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextCapacitance()
{
  if (m_NextCapacitance == nullptr)
    m_NextCapacitance = new CapacitanceScalar();
  return *m_NextCapacitance;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasCapacitanceBaseline() const
{
  return m_CapacitanceBaseline == nullptr ? false : m_CapacitanceBaseline->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
CapacitanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetCapacitanceBaseline()
{
  if (m_CapacitanceBaseline == nullptr)
    m_CapacitanceBaseline = new CapacitanceScalar();
  return *m_CapacitanceBaseline;
}
//-------------------------------------------------------------------------------
//////////////////////
// Inductance Types //
//////////////////////
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasInductance() const
{
  return m_Inductance == nullptr ? false : m_Inductance->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
InductanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetInductance()
{
  if (m_Inductance == nullptr)
    m_Inductance = new InductanceScalar();
  return *m_Inductance;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextInductance() const
{
  return m_NextInductance == nullptr ? false : m_NextInductance->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
InductanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextInductance()
{
  if (m_NextInductance == nullptr)
    m_NextInductance = new InductanceScalar();
  return *m_NextInductance;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasInductanceBaseline() const
{
  return m_InductanceBaseline == nullptr ? false : m_InductanceBaseline->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
InductanceScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetInductanceBaseline()
{
  if (m_InductanceBaseline == nullptr)
    m_InductanceBaseline = new InductanceScalar();
  return *m_InductanceBaseline;
}
//-------------------------------------------------------------------------------
////////////////
// Flux Types //
////////////////
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasFlux() const
{
  return m_Flux == nullptr ? false : m_Flux->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
FluxScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetFlux()
{
  if (m_Flux == nullptr)
    m_Flux = new FluxScalar();
  return *m_Flux;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextFlux() const
{
  return m_NextFlux == nullptr ? false : m_NextFlux->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
FluxScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextFlux()
{
  if (m_NextFlux == nullptr)
    m_NextFlux = new FluxScalar();
  return *m_NextFlux;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasFluxSource() const
{
  return m_FluxSource == nullptr ? false : m_FluxSource->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
FluxScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetFluxSource()
{
  if (m_FluxSource == nullptr)
    m_FluxSource = new FluxScalar();
  return *m_FluxSource;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextFluxSource() const
{
  return m_NextFluxSource == nullptr ? false : m_NextFluxSource->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
FluxScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextFluxSource()
{
  if (m_NextFluxSource == nullptr)
    m_NextFluxSource = new FluxScalar();
  return *m_NextFluxSource;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasFluxSourceBaseline() const
{
  return m_FluxSourceBaseline == nullptr ? false : m_FluxSourceBaseline->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
FluxScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetFluxSourceBaseline()
{
  if (m_FluxSourceBaseline == nullptr)
    m_FluxSourceBaseline = new FluxScalar();
  return *m_FluxSourceBaseline;
}
//-------------------------------------------------------------------------------
/////////////////////
// Potential Types //
/////////////////////
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasPotentialSource() const
{
  return m_PotentialSource == nullptr ? false : m_PotentialSource->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
PotentialScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetPotentialSource()
{
  if (m_PotentialSource == nullptr)
    m_PotentialSource = new PotentialScalar();
  return *m_PotentialSource;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasNextPotentialSource() const
{
  return m_NextPotentialSource == nullptr ? false : m_NextPotentialSource->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
PotentialScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetNextPotentialSource()
{
  if (m_NextPotentialSource == nullptr)
    m_NextPotentialSource = new PotentialScalar();
  return *m_NextPotentialSource;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasPotentialSourceBaseline() const
{
  return m_PotentialSourceBaseline == nullptr ? false : m_PotentialSourceBaseline->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
PotentialScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetPotentialSourceBaseline()
{
  if (m_PotentialSourceBaseline == nullptr)
    m_PotentialSourceBaseline = new PotentialScalar();
  return *m_PotentialSourceBaseline;
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
bool SECircuitPath<CIRCUIT_PATH_TYPES>::HasValveBreakdownPotential() const
{
  return m_ValveBreakdownPotential == nullptr ? false : m_ValveBreakdownPotential->IsValid();
}
//-------------------------------------------------------------------------------
template <CIRCUIT_PATH_TEMPLATE>
PotentialScalar& SECircuitPath<CIRCUIT_PATH_TYPES>::GetValveBreakdownPotential()
{
  if (m_ValveBreakdownPotential == nullptr)
    m_ValveBreakdownPotential = new PotentialScalar();
  return *m_ValveBreakdownPotential;
}
//-------------------------------------------------------------------------------
}