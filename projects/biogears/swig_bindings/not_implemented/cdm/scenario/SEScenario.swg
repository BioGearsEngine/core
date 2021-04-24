%module "biogears::SEScenario"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/SEScenario.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
