%module "biogears::SEScenarioInitialParameters"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/SEScenarioInitialParameters.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
