%module "biogears::SEThermalCircuit"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/thermal/SEThermalCircuit.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
