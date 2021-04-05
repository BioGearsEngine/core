%module "biogears::SEScalarTime"

// Add necessary symbols to generated header
%{
#include<cdm/properties/SEScalarTime.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
