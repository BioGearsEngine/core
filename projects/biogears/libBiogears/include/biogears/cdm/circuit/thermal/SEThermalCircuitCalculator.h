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
#include <biogears/cdm/circuit/SECircuitCalculator.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>

#include <biogears/cdm/properties/SEScalarEnergy.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>
#include <biogears/cdm/properties/SEScalarHeatInductance.h>
#include <biogears/cdm/properties/SEScalarHeatResistance.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>

namespace biogears {

BG_EXT template class BIOGEARS_API SECircuitCalculator<SEThermalCircuit, SEThermalCircuitNode, SEThermalCircuitPath, HeatCapacitanceUnit, PowerUnit, HeatInductanceUnit, TemperatureUnit, EnergyUnit, HeatResistanceUnit>;

class BIOGEARS_API SEThermalCircuitCalculator : public SECircuitCalculator<SEThermalCircuit, SEThermalCircuitNode, SEThermalCircuitPath, HeatCapacitanceUnit, PowerUnit, HeatInductanceUnit, TemperatureUnit, EnergyUnit, HeatResistanceUnit> {
public:
  SEThermalCircuitCalculator(Logger* logger);
  SEThermalCircuitCalculator(const HeatCapacitanceUnit&, const PowerUnit&, const HeatInductanceUnit&, const TemperatureUnit&, const EnergyUnit&, const HeatResistanceUnit&, Logger* logger);
  virtual ~SEThermalCircuitCalculator();
};
}