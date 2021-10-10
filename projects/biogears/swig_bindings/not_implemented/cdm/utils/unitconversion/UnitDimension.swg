%module "biogears::UnitDimension"

// Add necessary symbols to generated header
%{
#include<cdm/utils/unitconversion/UnitDimension.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
