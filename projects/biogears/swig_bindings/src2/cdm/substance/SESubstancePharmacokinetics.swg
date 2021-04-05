%module "biogears::SESubstancePharmacokinetics"

// Add necessary symbols to generated header
%{
#include<cdm/substance/SESubstancePharmacokinetics.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
