%module "biogears::SESubstanceCompound"

// Add necessary symbols to generated header
%{
#include<cdm/substance/SESubstanceCompound.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
