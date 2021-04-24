%module "biogears::Macros"

// Add necessary symbols to generated header
%{
#include<cdm/Macros.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
