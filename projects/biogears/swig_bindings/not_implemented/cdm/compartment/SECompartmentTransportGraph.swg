%module "biogears::SECompartmentTransportGraph"

// Add necessary symbols to generated header
%{
#include<cdm/compartment/SECompartmentTransportGraph.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
