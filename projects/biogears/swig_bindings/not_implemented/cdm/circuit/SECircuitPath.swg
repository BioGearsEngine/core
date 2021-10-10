%module "biogears::SECircuitPath"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/SECircuitPath.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
