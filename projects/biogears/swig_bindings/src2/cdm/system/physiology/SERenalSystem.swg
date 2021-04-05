%module "biogears::SERenalSystem"

// Add necessary symbols to generated header
%{
#include<cdm/system/physiology/SERenalSystem.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
