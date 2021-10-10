%module "biogears::SEElectricalCircuitNode"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/electrical/SEElectricalCircuitNode.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
