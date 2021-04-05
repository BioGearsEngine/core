%module "biogears::SEUnitScalar"

// Add necessary symbols to generated header
%{
#include<cdm/properties/SEUnitScalar.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
