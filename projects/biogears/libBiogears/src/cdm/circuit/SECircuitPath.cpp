#include <biogears/cdm/circuit/SECircuitPath.h>

#include <biogears/cdm/properties/SEScalarTypes.h>

namespace biogears {

template class SECircuitPath<ELECTRICAL_CIRCUIT_PATH>;
template class SECircuitPath<FLUID_CIRCUIT_PATH>;
template class SECircuitPath<THERMAL_CIRCUIT_PATH>;

}
