%module "biogears::SEThermalCircuitCalculator"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/thermal/SEThermalCircuitCalculator.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
