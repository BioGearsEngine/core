%module "biogears::GeneralMath"

// Add necessary symbols to generated header
%{
#include<cdm/utils/GeneralMath.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
