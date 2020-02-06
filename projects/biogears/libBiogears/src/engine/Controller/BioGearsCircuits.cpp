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
#include <biogears/engine/Controller/BioGearsCircuits.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {

auto BioGearsCircuits::make_unique(BioGears& bg) -> std::unique_ptr<BioGearsCircuits>
{
  return std::unique_ptr<BioGearsCircuits>(new BioGearsCircuits(bg));
}

BioGearsCircuits::BioGearsCircuits(BioGears& bg)
  : SECircuitManager(bg.GetLogger())
  , m_data(bg)
{
  Clear();
}

BioGearsCircuits::~BioGearsCircuits()
{
  Clear();
}

void BioGearsCircuits::Clear()
{
  SECircuitManager::Clear();
  m_CombinedCardiovascularCircuit = nullptr;
  m_CardiovascularCircuit = nullptr;
  m_CerebralCircuit = nullptr;
  m_RenalCircuit = nullptr;
  m_RespiratoryCircuit = nullptr;
  m_AnesthesiaMachineCircuit = nullptr;
  m_CombinedRespiratoryAnesthesiaCircuit = nullptr;
  m_CombinedRespiratoryInhalerCircuit = nullptr;
  m_CombinedRespiratoryMechanicalVentilatorCircuit = nullptr;
  m_TemperatureCircuit = nullptr;
  m_InternalTemperatureCircuit = nullptr;
  m_ExternalTemperatureCircuit = nullptr;
}

bool BioGearsCircuits::Load(const CDM::CircuitManagerData& in)
{
  if (!SECircuitManager::Load(in))
    return false;
  m_CombinedCardiovascularCircuit = GetFluidCircuit(BGE::Circuits::FullCardiovascular);
  if (m_CombinedCardiovascularCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::FullCardiovascular));
    return false;
  }
  m_CardiovascularCircuit = GetFluidCircuit(BGE::Circuits::Cardiovascular);
  if (m_CardiovascularCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Cardiovascular));
    return false;
  }
  m_CerebralCircuit = GetFluidCircuit(BGE::Circuits::Cerebral);
  if (m_CardiovascularCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Cerebral));
    return false;
  }
  m_RenalCircuit = GetFluidCircuit(BGE::Circuits::Renal);
  if (m_RenalCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Renal));
    return false;
  }
  m_RespiratoryCircuit = GetFluidCircuit(BGE::Circuits::Respiratory);
  if (m_RespiratoryCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Respiratory));
    return false;
  }
  m_AnesthesiaMachineCircuit = GetFluidCircuit(BGE::Circuits::AnesthesiaMachine);
  if (m_AnesthesiaMachineCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::AnesthesiaMachine));
    return false;
  }
  m_CombinedRespiratoryAnesthesiaCircuit = GetFluidCircuit(BGE::Circuits::RespiratoryAnesthesia);
  if (m_CombinedRespiratoryAnesthesiaCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::RespiratoryAnesthesia));
    return false;
  }
  m_CombinedRespiratoryInhalerCircuit = GetFluidCircuit(BGE::Circuits::RespiratoryInhaler);
  if (m_CombinedRespiratoryInhalerCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::RespiratoryInhaler));
    return false;
  }
  m_CombinedRespiratoryMechanicalVentilatorCircuit = GetFluidCircuit(BGE::Circuits::RespiratoryMechanicalVentilator);
  if (m_CombinedRespiratoryMechanicalVentilatorCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::RespiratoryMechanicalVentilator));
    return false;
  }
  m_TemperatureCircuit = GetThermalCircuit(BGE::Circuits::Temperature);
  if (m_TemperatureCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Temperature));
    return false;
  }
  m_InternalTemperatureCircuit = GetThermalCircuit(BGE::Circuits::InternalTemperature);
  if (m_InternalTemperatureCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::InternalTemperature));
    return false;
  }
  m_ExternalTemperatureCircuit = GetThermalCircuit(BGE::Circuits::ExternalTemperature);
  if (m_ExternalTemperatureCircuit == nullptr) {
    Error("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::ExternalTemperature));
    return false;
  }
  return true;
}

void BioGearsCircuits::SetReadOnlyFluid(bool b)
{
  SECircuitManager::SetReadOnlyFluid(b);
  // These don't need to be locked
  GetFluidNode(BGE::EnvironmentNode::Ambient)->GetPressure().SetReadOnly(false);
  GetFluidNode(BGE::EnvironmentNode::Ambient)->GetNextPressure().SetReadOnly(false);
  GetFluidNode(BGE::RespiratoryNode::Stomach)->GetPressure().SetReadOnly(false);
  GetFluidNode(BGE::RespiratoryNode::Stomach)->GetNextPressure().SetReadOnly(false);
  GetFluidNode(BGE::InhalerNode::Mouthpiece)->GetPressure().SetReadOnly(false);
  GetFluidNode(BGE::InhalerNode::Mouthpiece)->GetNextPressure().SetReadOnly(false);
}

SEFluidCircuit& BioGearsCircuits::GetActiveCardiovascularCircuit()
{
  if (m_CombinedCardiovascularCircuit == nullptr)
    m_CombinedCardiovascularCircuit = &CreateFluidCircuit(BGE::Circuits::FullCardiovascular);
  return *m_CombinedCardiovascularCircuit;
}
SEFluidCircuit& BioGearsCircuits::GetCardiovascularCircuit()
{
  if (m_CardiovascularCircuit == nullptr)
    m_CardiovascularCircuit = &CreateFluidCircuit(BGE::Circuits::Cardiovascular);
  return *m_CardiovascularCircuit;
}
SEFluidCircuit& BioGearsCircuits::GetCerebralCircuit()
{
  if (m_CerebralCircuit == nullptr)
    m_CerebralCircuit = &CreateFluidCircuit(BGE::Circuits::Cerebral);
  return *m_CerebralCircuit;
}

SEFluidCircuit& BioGearsCircuits::GetRenalCircuit()
{
  if (m_RenalCircuit == nullptr)
    m_RenalCircuit = &CreateFluidCircuit(BGE::Circuits::Renal);
  return *m_RenalCircuit;
}

SEFluidCircuit& BioGearsCircuits::GetActiveRespiratoryCircuit()
{
  switch (m_data.GetAirwayMode()) {
  case CDM::enumBioGearsAirwayMode::Free:
    return *m_RespiratoryCircuit;
  case CDM::enumBioGearsAirwayMode::AnesthesiaMachine:
    return *m_CombinedRespiratoryAnesthesiaCircuit;
  case CDM::enumBioGearsAirwayMode::Inhaler:
    return *m_CombinedRespiratoryInhalerCircuit;
  case CDM::enumBioGearsAirwayMode::MechanicalVentilator:
    return *m_CombinedRespiratoryMechanicalVentilatorCircuit;
  default:
    throw CommonDataModelException("Unknown airway mode");
  }
}
SEFluidCircuit& BioGearsCircuits::GetRespiratoryCircuit()
{
  if (m_RespiratoryCircuit == nullptr)
    m_RespiratoryCircuit = &CreateFluidCircuit(BGE::Circuits::Respiratory);
  return *m_RespiratoryCircuit;
}
SEFluidCircuit& BioGearsCircuits::GetAnesthesiaMachineCircuit()
{
  if (m_AnesthesiaMachineCircuit == nullptr)
    m_AnesthesiaMachineCircuit = &CreateFluidCircuit(BGE::Circuits::AnesthesiaMachine);
  return *m_AnesthesiaMachineCircuit;
}
SEFluidCircuit& BioGearsCircuits::GetRespiratoryAndAnesthesiaMachineCircuit()
{
  if (m_CombinedRespiratoryAnesthesiaCircuit == nullptr)
    m_CombinedRespiratoryAnesthesiaCircuit = &CreateFluidCircuit(BGE::Circuits::RespiratoryAnesthesia);
  return *m_CombinedRespiratoryAnesthesiaCircuit;
}
SEFluidCircuit& BioGearsCircuits::GetRespiratoryAndInhalerCircuit()
{
  if (m_CombinedRespiratoryInhalerCircuit == nullptr)
    m_CombinedRespiratoryInhalerCircuit = &CreateFluidCircuit(BGE::Circuits::RespiratoryInhaler);
  return *m_CombinedRespiratoryInhalerCircuit;
}
SEFluidCircuit& BioGearsCircuits::GetRespiratoryAndMechanicalVentilatorCircuit()
{
  if (m_CombinedRespiratoryMechanicalVentilatorCircuit == nullptr)
    m_CombinedRespiratoryMechanicalVentilatorCircuit = &CreateFluidCircuit(BGE::Circuits::RespiratoryMechanicalVentilator);
  return *m_CombinedRespiratoryMechanicalVentilatorCircuit;
}
SEThermalCircuit& BioGearsCircuits::GetTemperatureCircuit()
{
  if (m_TemperatureCircuit == nullptr)
    m_TemperatureCircuit = &CreateThermalCircuit(BGE::Circuits::Temperature);
  return *m_TemperatureCircuit;
}
SEThermalCircuit& BioGearsCircuits::GetInternalTemperatureCircuit()
{
  if (m_InternalTemperatureCircuit == nullptr)
    m_InternalTemperatureCircuit = &CreateThermalCircuit(BGE::Circuits::InternalTemperature);
  return *m_InternalTemperatureCircuit;
}
SEThermalCircuit& BioGearsCircuits::GetExternalTemperatureCircuit()
{
  if (m_ExternalTemperatureCircuit == nullptr)
    m_ExternalTemperatureCircuit = &CreateThermalCircuit(BGE::Circuits::ExternalTemperature);
  return *m_ExternalTemperatureCircuit;
}
}