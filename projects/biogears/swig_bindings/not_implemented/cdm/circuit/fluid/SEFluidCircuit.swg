%module "biogears::SEFluidCircuit"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/fluid/SEFluidCircuit.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
