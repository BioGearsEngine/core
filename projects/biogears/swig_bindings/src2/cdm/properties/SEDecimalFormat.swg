%module "biogears::SEDecimalFormat"

// Add necessary symbols to generated header
%{
#include<cdm/properties/SEDecimalFormat.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
