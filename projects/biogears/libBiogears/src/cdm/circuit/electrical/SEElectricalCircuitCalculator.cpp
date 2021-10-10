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

#include <biogears/cdm/circuit/electrical/SEElectricalCircuitCalculator.h>

namespace biogears {
  template class SECircuitCalculator<SEElectricalCircuit, SEElectricalCircuitNode, SEElectricalCircuitPath, ElectricCapacitanceUnit, ElectricCurrentUnit, ElectricInductanceUnit, ElectricPotentialUnit, ElectricChargeUnit, ElectricResistanceUnit>;
SEElectricalCircuitCalculator::SEElectricalCircuitCalculator(Logger* logger)
  : SECircuitCalculator<SEElectricalCircuit, SEElectricalCircuitNode, SEElectricalCircuitPath, ElectricCapacitanceUnit, ElectricCurrentUnit, ElectricInductanceUnit, ElectricPotentialUnit, ElectricChargeUnit, ElectricResistanceUnit>(
      ElectricCapacitanceUnit::F, ElectricCurrentUnit::A, ElectricInductanceUnit::H, ElectricPotentialUnit::V, ElectricChargeUnit::C, ElectricResistanceUnit::Ohm, logger)
{
}
SEElectricalCircuitCalculator::SEElectricalCircuitCalculator(const ElectricCapacitanceUnit& c, const ElectricCurrentUnit& f, const ElectricInductanceUnit& i, const ElectricPotentialUnit& p, const ElectricChargeUnit& q, const ElectricResistanceUnit& r, Logger* logger)
  : SECircuitCalculator<SEElectricalCircuit, SEElectricalCircuitNode, SEElectricalCircuitPath, ElectricCapacitanceUnit, ElectricCurrentUnit, ElectricInductanceUnit, ElectricPotentialUnit, ElectricChargeUnit, ElectricResistanceUnit>(c, f, i, p, q, r, logger)
{
}
SEElectricalCircuitCalculator::~SEElectricalCircuitCalculator()
{
}
}