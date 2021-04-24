%module "biogears::SECircuit"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/SECircuit.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
