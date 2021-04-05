%module "biogears::SEHistogram"

// Add necessary symbols to generated header
%{
#include<cdm/properties/SEHistogram.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
