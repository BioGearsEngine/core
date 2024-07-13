#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/circuit/SECircuit.inl>
#include <biogears/cdm/properties/SEScalarTypes.h>

#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>
#include <biogears/cdm/circuit/electrical/SEElectricalCircuit.h>

namespace biogears {

template class SECircuit<ELECTRICAL_CIRCUIT_TYPES>;
template class SECircuit<FLUID_CIRCUIT_TYPES>;
template class SECircuit<THERMAL_CIRCUIT_TYPES>;

}

