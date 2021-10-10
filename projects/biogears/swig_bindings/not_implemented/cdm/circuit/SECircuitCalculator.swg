%module "biogears::SECircuitCalculator"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/SECircuitCalculator.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
