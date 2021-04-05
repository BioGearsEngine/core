%module "biogears::SEElectricalCircuitCalculator"

// Add necessary symbols to generated header
%{
#include<cdm/circuit/electrical/SEElectricalCircuitCalculator.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
