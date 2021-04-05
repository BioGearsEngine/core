%module "biogears::SEProperty"

// Add necessary symbols to generated header
%{
#include<cdm/properties/SEProperty.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
