%module "biogears::SEScenarioExec"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/SEScenarioExec.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
