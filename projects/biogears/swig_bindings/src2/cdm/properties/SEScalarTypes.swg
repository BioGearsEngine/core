%module "biogears::SEScalarTypes"

// Add necessary symbols to generated header
%{
#include<cdm/properties/SEScalarTypes.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
