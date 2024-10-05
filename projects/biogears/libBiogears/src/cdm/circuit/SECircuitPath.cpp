#include <biogears/cdm/circuit/SECircuitPath.h>
#include <biogears/cdm/circuit/SECircuitPath.inl>
#include <biogears/cdm/properties/SEProperties.h>

namespace biogears {

template class SECircuitPath<ELECTRICAL_CIRCUIT_PATH>;
template class SECircuitPath<FLUID_CIRCUIT_PATH>;
template class SECircuitPath<THERMAL_CIRCUIT_PATH>;

}
