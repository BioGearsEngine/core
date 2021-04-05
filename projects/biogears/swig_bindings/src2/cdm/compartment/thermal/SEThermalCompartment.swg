%module "biogears::SEThermalCompartment"

// Add necessary symbols to generated header
%{
#include<cdm/compartment/thermal/SEThermalCompartment.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
