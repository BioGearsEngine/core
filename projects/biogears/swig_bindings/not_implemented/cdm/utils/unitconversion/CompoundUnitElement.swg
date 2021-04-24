%module "biogears::CompoundUnitElement"

// Add necessary symbols to generated header
%{
#include<cdm/utils/unitconversion/CompoundUnitElement.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
