%module "biogears::SESubstanceQuantity"

// Add necessary symbols to generated header
%{
#include<cdm/compartment/substances/SESubstanceQuantity.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
