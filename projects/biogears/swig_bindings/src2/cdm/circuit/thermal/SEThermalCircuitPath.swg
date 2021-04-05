%module "biogears::SEThermalCircuitPath"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/thermal/SEThermalCircuitPath.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
