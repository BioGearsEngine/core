%module "biogears::SESubstanceTransport"

// Add necessary symbols to generated header
%{
#include<cdm/substance/SESubstanceTransport.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
