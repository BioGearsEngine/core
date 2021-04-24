%module "biogears::SELiquidCompartmentGraph"

// Add necessary symbols to generated header
%{
#include<cdm/compartment/fluid/SELiquidCompartmentGraph.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
