%module "biogears::SECircuitManager"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/SECircuitManager.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
