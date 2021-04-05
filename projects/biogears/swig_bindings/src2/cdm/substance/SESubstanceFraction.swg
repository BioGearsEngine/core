%module "biogears::SESubstanceFraction"

// Add necessary symbols to generated header
%{
#include<cdm/substance/SESubstanceFraction.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
