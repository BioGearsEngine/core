#include <biogears/cdm/properties/SEProperties.h>

#include <biogears/cdm/circuit/SECircuitNode.h>
#include <biogears/cdm/circuit/SECircuitNode.inl>
namespace biogears {

template class SECircuitNode<ELECTRICAL_CIRCUIT_NODE>;
template class SECircuitNode<FLUID_CIRCUIT_NODE>;
template class SECircuitNode<THERMAL_CIRCUIT_NODE>;

}
