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
#include <biogears/cdm/circuit/SECircuitPath.h>
#include <biogears/cdm/circuit/electrical/SEElectricalCircuitNode.h>
#include <biogears/cdm/properties/SEScalarElectricCapacitance.h>
#include <biogears/cdm/properties/SEScalarElectricCharge.h>
#include <biogears/cdm/properties/SEScalarElectricCurrent.h>
#include <biogears/cdm/properties/SEScalarElectricInductance.h>
#include <biogears/cdm/properties/SEScalarElectricPotential.h>
#include <biogears/cdm/properties/SEScalarElectricResistance.h>

namespace biogears {
class SECircuitManager;
class BIOGEARS_API SEElectricalCircuitPath : public SECircuitPath<SEScalarElectricCurrent, SEScalarElectricResistance, SEScalarElectricCapacitance, SEScalarElectricInductance, SEScalarElectricPotential, SEScalarElectricCharge> {
  friend class SECircuitManager;

protected:
  SEElectricalCircuitPath(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const std::string& name);
  SEElectricalCircuitPath(SEElectricalCircuitNode& src, SEElectricalCircuitNode& tgt, const char* name);

public:
  virtual ~SEElectricalCircuitPath();

  virtual void Clear(); //clear memory

  bool Load(const CDM::ElectricalCircuitPathData& in);
  CDM::ElectricalCircuitPathData* Unload() const;

protected:
  void Unload(CDM::ElectricalCircuitPathData& data) const;

public:
  virtual SEElectricalCircuitNode& GetSourceNode() const { return m_ElectricalSourceNode; }
  virtual SEElectricalCircuitNode& GetTargetNode() const { return m_ElectricalTargetNode; }

  virtual bool HasResistance() const;
  virtual SEScalarElectricResistance& GetResistance();
  virtual double GetResistance(const ElectricResistanceUnit& unit) const;
  virtual bool HasNextResistance() const;
  virtual SEScalarElectricResistance& GetNextResistance();
  virtual double GetNextResistance(const ElectricResistanceUnit& unit) const;
  virtual bool HasResistanceBaseline() const;
  virtual SEScalarElectricResistance& GetResistanceBaseline();
  virtual double GetResistanceBaseline(const ElectricResistanceUnit& unit) const;

  virtual bool HasCapacitance() const;
  virtual SEScalarElectricCapacitance& GetCapacitance();
  virtual double GetCapacitance(const ElectricCapacitanceUnit& unit) const;
  virtual bool HasNextCapacitance() const;
  virtual SEScalarElectricCapacitance& GetNextCapacitance();
  virtual double GetNextCapacitance(const ElectricCapacitanceUnit& unit) const;
  virtual bool HasCapacitanceBaseline() const;
  virtual SEScalarElectricCapacitance& GetCapacitanceBaseline();
  virtual double GetCapacitanceBaseline(const ElectricCapacitanceUnit& unit) const;

  virtual bool HasInductance() const;
  virtual SEScalarElectricInductance& GetInductance();
  virtual double GetInductance(const ElectricInductanceUnit& unit) const;
  virtual bool HasNextInductance() const;
  virtual SEScalarElectricInductance& GetNextInductance();
  virtual double GetNextInductance(const ElectricInductanceUnit& unit) const;
  virtual bool HasInductanceBaseline() const;
  virtual SEScalarElectricInductance& GetInductanceBaseline();
  virtual double GetInductanceBaseline(const ElectricInductanceUnit& unit) const;

  virtual bool HasCurrent() const;
  virtual SEScalarElectricCurrent& GetCurrent();
  virtual double GetCurrent(const ElectricCurrentUnit& unit) const;
  virtual bool HasNextCurrent() const;
  virtual SEScalarElectricCurrent& GetNextCurrent();
  virtual double GetNextCurrent(const ElectricCurrentUnit& unit) const;
  virtual bool HasCurrentSource() const;
  virtual SEScalarElectricCurrent& GetCurrentSource();
  virtual double GetCurrentSource(const ElectricCurrentUnit& unit) const;
  virtual bool HasNextCurrentSource() const;
  virtual SEScalarElectricCurrent& GetNextCurrentSource();
  virtual double GetNextCurrentSource(const ElectricCurrentUnit& unit) const;
  virtual bool HasCurrentSourceBaseline() const;
  virtual SEScalarElectricCurrent& GetCurrentSourceBaseline();
  virtual double GetCurrentSourceBaseline(const ElectricCurrentUnit& unit) const;

  virtual bool HasVoltageSource() const;
  virtual SEScalarElectricPotential& GetVoltageSource();
  virtual double GetVoltageSource(const ElectricPotentialUnit& unit) const;
  virtual bool HasNextVoltageSource() const;
  virtual SEScalarElectricPotential& GetNextVoltageSource();
  virtual double GetNextVoltageSource(const ElectricPotentialUnit& unit) const;
  virtual bool HasVoltageSourceBaseline() const;
  virtual SEScalarElectricPotential& GetVoltageSourceBaseline();
  virtual double GetVoltageSourceBaseline(const ElectricPotentialUnit& unit) const;
  virtual bool HasValveBreakdownVoltage() const;
  virtual SEScalarElectricPotential& GetValveBreakdownVoltage();
  virtual double GetValveBreakdownVoltage(const ElectricPotentialUnit& unit) const;

protected:
  SEElectricalCircuitNode& m_ElectricalSourceNode;
  SEElectricalCircuitNode& m_ElectricalTargetNode;
};
}