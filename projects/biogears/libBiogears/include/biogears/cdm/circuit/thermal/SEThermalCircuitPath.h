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
#include <biogears/cdm/circuit/thermal/SEThermalCircuitNode.h>
#include <biogears/cdm/properties/SEScalarEnergy.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>
#include <biogears/cdm/properties/SEScalarHeatInductance.h>
#include <biogears/cdm/properties/SEScalarHeatResistance.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>

namespace biogears {
class SECircuitManager;
namespace io {
  class Circuit;
}

class BIOGEARS_API SEThermalCircuitPath : public SECircuitPath<THERMAL_CIRCUIT_PATH> {
  friend class SECircuitManager;
  friend io::Circuit;

protected:
  SEThermalCircuitPath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const char* name);
  SEThermalCircuitPath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name);

public:
  virtual ~SEThermalCircuitPath() override;

  virtual void Clear() override; //clear memory

  bool Load(const CDM::ThermalCircuitPathData& in);
  CDM::ThermalCircuitPathData* Unload() const override;

protected:
  void Unload(CDM::ThermalCircuitPathData& data) const;

public:
  virtual SEThermalCircuitNode& GetSourceNode() const override { return m_ThermalSourceNode; }
  virtual SEThermalCircuitNode& GetTargetNode() const override { return m_ThermalTargetNode; }

  virtual bool HasResistance() const override;
  virtual SEScalarHeatResistance& GetResistance() override;
  virtual double GetResistance(const HeatResistanceUnit& unit) const;
  virtual bool HasNextResistance() const override;
  virtual SEScalarHeatResistance& GetNextResistance() override;
  virtual double GetNextResistance(const HeatResistanceUnit& unit) const;
  virtual bool HasResistanceBaseline() const override;
  virtual SEScalarHeatResistance& GetResistanceBaseline() override;
  virtual double GetResistanceBaseline(const HeatResistanceUnit& unit) const;

  virtual bool HasCapacitance() const override;
  virtual SEScalarHeatCapacitance& GetCapacitance() override;
  virtual double GetCapacitance(const HeatCapacitanceUnit& unit) const;
  virtual bool HasNextCapacitance() const override;
  virtual SEScalarHeatCapacitance& GetNextCapacitance() override;
  virtual double GetNextCapacitance(const HeatCapacitanceUnit& unit) const;
  virtual bool HasCapacitanceBaseline() const override;
  virtual SEScalarHeatCapacitance& GetCapacitanceBaseline() override;
  virtual double GetCapacitanceBaseline(const HeatCapacitanceUnit& unit) const;

  virtual bool HasInductance() const override;
  virtual SEScalarHeatInductance& GetInductance() override;
  virtual double GetInductance(const HeatInductanceUnit& unit) const;
  virtual bool HasNextInductance() const override;
  virtual SEScalarHeatInductance& GetNextInductance() override;
  virtual double GetNextInductance(const HeatInductanceUnit& unit) const;
  virtual bool HasInductanceBaseline() const override;
  virtual SEScalarHeatInductance& GetInductanceBaseline() override;
  virtual double GetInductanceBaseline(const HeatInductanceUnit& unit) const;

  virtual bool HasHeatTransferRate() const;
  virtual SEScalarPower& GetHeatTransferRate();
  virtual double GetHeatTransferRate(const PowerUnit& unit) const;
  virtual bool HasNextHeatTransferRate() const;
  virtual SEScalarPower& GetNextHeatTransferRate();
  virtual double GetNextHeatTransferRate(const PowerUnit& unit) const;
  virtual bool HasHeatSource() const;
  virtual SEScalarPower& GetHeatSource();
  virtual double GetHeatSource(const PowerUnit& unit) const;
  virtual bool HasNextHeatSource() const;
  virtual SEScalarPower& GetNextHeatSource();
  virtual double GetNextHeatSource(const PowerUnit& unit) const;
  virtual bool HasHeatSourceBaseline() const;
  virtual SEScalarPower& GetHeatSourceBaseline();
  virtual double GetHeatSourceBaseline(const PowerUnit& unit) const;

  virtual bool HasTemperatureSource() const;
  virtual SEScalarTemperature& GetTemperatureSource();
  virtual double GetTemperatureSource(const TemperatureUnit& unit) const;
  virtual bool HasNextTemperatureSource() const;
  virtual SEScalarTemperature& GetNextTemperatureSource();
  virtual double GetNextTemperatureSource(const TemperatureUnit& unit) const;
  virtual bool HasTemperatureSourceBaseline() const;
  virtual SEScalarTemperature& GetTemperatureSourceBaseline();
  virtual double GetTemperatureSourceBaseline(const TemperatureUnit& unit) const;
  virtual bool HasValveBreakdownTemperature() const;
  virtual SEScalarTemperature& GetValveBreakdownTemperature();
  virtual double GetValveBreakdownTemperature(const TemperatureUnit& unit) const;

protected:
  SEThermalCircuitNode& m_ThermalSourceNode;
  SEThermalCircuitNode& m_ThermalTargetNode;
};
}