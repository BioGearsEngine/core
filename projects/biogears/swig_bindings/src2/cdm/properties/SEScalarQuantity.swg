%module "biogears::SEScalarQuantity"

// Add necessary symbols to generated header
%{
#include<cdm/properties/SEScalarQuantity.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
