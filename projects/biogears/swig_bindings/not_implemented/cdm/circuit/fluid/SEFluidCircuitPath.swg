%module "biogears::SEFluidCircuitPath"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/fluid/SEFluidCircuitPath.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
