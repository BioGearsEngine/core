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
#include <biogears/cdm/properties/SEScalarHeatResistance.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>
#include <biogears/cdm/properties/SEScalarHeatInductance.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarEnergy.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuitNode.h>
#include <biogears/schema/ThermalCircuitPathData.hxx>

class BIOGEARS_API SEThermalCircuitPath : public SECircuitPath<THERMAL_CIRCUIT_PATH> {
  friend class SECircuitManager;

protected:
  SEThermalCircuitPath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name);

public:
  virtual ~SEThermalCircuitPath();

  virtual void Clear(); //clear memory

  bool Load(const CDM::ThermalCircuitPathData& in);
  CDM::ThermalCircuitPathData* Unload() const;

protected:
  void Unload(CDM::ThermalCircuitPathData& data) const;

public:
  virtual SEThermalCircuitNode& GetSourceNode() const { return m_ThermalSourceNode; }
  virtual SEThermalCircuitNode& GetTargetNode() const { return m_ThermalTargetNode; }

  virtual bool HasResistance() const;
  virtual SEScalarHeatResistance& GetResistance();
  virtual double GetResistance(const HeatResistanceUnit& unit) const;
  virtual bool HasNextResistance() const;
  virtual SEScalarHeatResistance& GetNextResistance();
  virtual double GetNextResistance(const HeatResistanceUnit& unit) const;
  virtual bool HasResistanceBaseline() const;
  virtual SEScalarHeatResistance& GetResistanceBaseline();
  virtual double GetResistanceBaseline(const HeatResistanceUnit& unit) const;

  virtual bool HasCapacitance() const;
  virtual SEScalarHeatCapacitance& GetCapacitance();
  virtual double GetCapacitance(const HeatCapacitanceUnit& unit) const;
  virtual bool HasNextCapacitance() const;
  virtual SEScalarHeatCapacitance& GetNextCapacitance();
  virtual double GetNextCapacitance(const HeatCapacitanceUnit& unit) const;
  virtual bool HasCapacitanceBaseline() const;
  virtual SEScalarHeatCapacitance& GetCapacitanceBaseline();
  virtual double GetCapacitanceBaseline(const HeatCapacitanceUnit& unit) const;

  virtual bool HasInductance() const;
  virtual SEScalarHeatInductance& GetInductance();
  virtual double GetInductance(const HeatInductanceUnit& unit) const;
  virtual bool HasNextInductance() const;
  virtual SEScalarHeatInductance& GetNextInductance();
  virtual double GetNextInductance(const HeatInductanceUnit& unit) const;
  virtual bool HasInductanceBaseline() const;
  virtual SEScalarHeatInductance& GetInductanceBaseline();
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