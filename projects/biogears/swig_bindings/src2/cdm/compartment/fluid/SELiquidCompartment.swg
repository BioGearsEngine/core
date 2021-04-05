%module "biogears::SELiquidCompartment"

// Add necessary symbols to generated header
%{
#include<cdm/compartment/fluid/SELiquidCompartment.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
