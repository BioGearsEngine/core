%module "biogears::SESubstance"

// Add necessary symbols to generated header
%{
#include<cdm/substance/SESubstance.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
