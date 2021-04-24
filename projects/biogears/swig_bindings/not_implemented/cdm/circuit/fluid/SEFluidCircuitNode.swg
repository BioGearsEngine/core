%module "biogears::SEFluidCircuitNode"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/fluid/SEFluidCircuitNode.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
