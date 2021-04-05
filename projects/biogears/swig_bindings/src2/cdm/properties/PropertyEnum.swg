%module "biogears::PropertyEnum"

// Add necessary symbols to generated header
%{
#include<cdm/properties/PropertyEnum.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
