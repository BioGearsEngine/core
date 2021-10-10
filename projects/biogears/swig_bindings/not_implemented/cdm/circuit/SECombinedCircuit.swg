%module "biogears::SECombinedCircuit"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/SECombinedCircuit.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
