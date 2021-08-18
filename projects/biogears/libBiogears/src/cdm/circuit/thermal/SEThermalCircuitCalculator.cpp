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

#include <biogears/cdm/circuit/thermal/SEThermalCircuitCalculator.h>

namespace biogears {
  template class SECircuitCalculator<SEThermalCircuit, SEThermalCircuitNode, SEThermalCircuitPath, HeatCapacitanceUnit, PowerUnit, HeatInductanceUnit, TemperatureUnit, EnergyUnit, HeatResistanceUnit>;
SEThermalCircuitCalculator::SEThermalCircuitCalculator(Logger* logger)
  : SECircuitCalculator<SEThermalCircuit, SEThermalCircuitNode, SEThermalCircuitPath, HeatCapacitanceUnit, PowerUnit, HeatInductanceUnit, TemperatureUnit, EnergyUnit, HeatResistanceUnit>(
      HeatCapacitanceUnit::J_Per_K, PowerUnit::W, HeatInductanceUnit::K_s_Per_W, TemperatureUnit::K, EnergyUnit::J, HeatResistanceUnit::K_Per_W, logger)
{
}
SEThermalCircuitCalculator::SEThermalCircuitCalculator(const HeatCapacitanceUnit& c, const PowerUnit& f, const HeatInductanceUnit& i, const TemperatureUnit& p, const EnergyUnit& q, const HeatResistanceUnit& r, Logger* logger)
  : SECircuitCalculator<SEThermalCircuit, SEThermalCircuitNode, SEThermalCircuitPath, HeatCapacitanceUnit, PowerUnit, HeatInductanceUnit, TemperatureUnit, EnergyUnit, HeatResistanceUnit>(c, f, i, p, q, r, logger)
{
}
SEThermalCircuitCalculator::~SEThermalCircuitCalculator()
{
}
}