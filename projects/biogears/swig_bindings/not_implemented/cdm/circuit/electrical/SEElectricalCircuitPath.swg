%module "biogears::SEElectricalCircuitPath"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/electrical/SEElectricalCircuitPath.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
