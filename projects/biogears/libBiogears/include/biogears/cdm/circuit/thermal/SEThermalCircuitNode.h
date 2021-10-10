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
#include <biogears/cdm/circuit/SECircuitNode.h>
#include <biogears/cdm/properties/SEScalarEnergy.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>

namespace biogears {
class SECircuitManager;
namespace io {
  class Circuit;
}
class BIOGEARS_API SEThermalCircuitNode : public SECircuitNode<THERMAL_CIRCUIT_NODE> {
  friend class SECircuitManager;
  friend io::Circuit;

protected:
  SEThermalCircuitNode(const char* name, Logger* logger);
  SEThermalCircuitNode(const std::string& name, Logger* logger);

public:
  virtual ~SEThermalCircuitNode();

  virtual void Clear(); //clear memory

  virtual bool Load(const CDM::ThermalCircuitNodeData& in);
  virtual CDM::ThermalCircuitNodeData* Unload() const;

protected:
  virtual void Unload(CDM::ThermalCircuitNodeData& data) const;

public:
  virtual bool HasTemperature() const;
  virtual SEScalarTemperature& GetTemperature();
  virtual double GetTemperature(const TemperatureUnit& unit) const;

  virtual bool HasNextTemperature() const;
  virtual SEScalarTemperature& GetNextTemperature();
  virtual double GetNextTemperature(const TemperatureUnit& unit) const;

  virtual bool HasHeat() const;
  virtual SEScalarEnergy& GetHeat();
  virtual double GetHeat(const EnergyUnit& unit) const;

  virtual bool HasNextHeat() const;
  virtual SEScalarEnergy& GetNextHeat();
  virtual double GetNextHeat(const EnergyUnit& unit) const;

  virtual bool HasHeatBaseline() const;
  virtual SEScalarEnergy& GetHeatBaseline();
  virtual double GetHeatBaseline(const EnergyUnit& unit) const;
};
}