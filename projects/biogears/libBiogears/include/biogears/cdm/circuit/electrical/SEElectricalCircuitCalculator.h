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
#include <biogears/cdm/circuit/electrical/SEElectricalCircuit.h>

#include <biogears/cdm/properties/SEScalarElectricCapacitance.h>
#include <biogears/cdm/properties/SEScalarElectricCharge.h>
#include <biogears/cdm/properties/SEScalarElectricCurrent.h>
#include <biogears/cdm/properties/SEScalarElectricInductance.h>
#include <biogears/cdm/properties/SEScalarElectricPotential.h>
#include <biogears/cdm/properties/SEScalarElectricResistance.h>

namespace biogears {
  BG_EXT template class BIOGEARS_API SECircuitCalculator<SEElectricalCircuit, SEElectricalCircuitNode, SEElectricalCircuitPath, ElectricCapacitanceUnit, ElectricCurrentUnit, ElectricInductanceUnit, ElectricPotentialUnit, ElectricChargeUnit, ElectricResistanceUnit>;
class BIOGEARS_API SEElectricalCircuitCalculator : public SECircuitCalculator<SEElectricalCircuit, SEElectricalCircuitNode, SEElectricalCircuitPath, ElectricCapacitanceUnit, ElectricCurrentUnit, ElectricInductanceUnit, ElectricPotentialUnit, ElectricChargeUnit, ElectricResistanceUnit> {
public:
  SEElectricalCircuitCalculator(Logger* logger);
  SEElectricalCircuitCalculator(const ElectricCapacitanceUnit&, const ElectricCurrentUnit&, const ElectricInductanceUnit&, const ElectricPotentialUnit&, const ElectricChargeUnit&, const ElectricResistanceUnit&, Logger* logger);
  virtual ~SEElectricalCircuitCalculator();
};
}