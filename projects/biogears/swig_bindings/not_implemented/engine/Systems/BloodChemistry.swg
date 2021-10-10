%module "biogears::BloodChemistry"

// Add necessary symbols to generated header
%{
#include<engine/Systems/BloodChemistry.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
